/***************************************************************************
 * Author: Dominik Flierl, flierl@ikf.uni-frankfurt.de
 ***************************************************************************/

// Revision 2004/09/10 skniege 
// pass pointer to TheoreticalValue() instead of reference ( in get1dProjection(..) ( in get2dProjection(..)) ) 
// changed boarders for projections (errors) in get1dProjection(..) (for -loop)
// changed core to qube  
// changed protection against fluctuation 

using namespace std ;

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "T49HbtFitBase.h"

ClassImp(T49HbtFitBase)

// this is the stupid way ROOT wants it: the fcn which is going to be minimzed must
// either be static or a global function. here it's global to make sur that we got
// the connection to this class we need the  TMinuit->SetObjectFit(this) statment in doFit()  
void gfcn(Int_t &nParamters, Double_t *gin, Double_t &finalChi2, Double_t *parameterSet, Int_t iflag)
{
  T49HbtFitBase* dummy = dynamic_cast<T49HbtFitBase*>(gMinuit->GetObjectFit()) ;
  dummy->mfcn(nParamters, gin, finalChi2 , parameterSet, iflag) ;
}

//____________________________________________________________________________________________________________________________________________
T49HbtFitBase::T49HbtFitBase(TMinuit* gMinuit,TH3D* numerator, TH3D* denominator)
{
  // Get TMinuit in
  mMinuit = gMinuit ;
       
  // Get numerator and denominator in
  mNumerator = numerator ; 
  mDenominator = denominator ;
    
  // Create ratio which will be filled later
  mRatio = (TH3D*) mNumerator->Clone("Correlationfunction") ;
  mRatio->Reset() ;
    
  // Set constants
  mhc  = 0.197326960277 ; // GeV * fm
  mhc2 = 0.038937929230 ; // GeV^2 * fm^2 ;
 
  // Set defaults for thresholds = 1 means accept every bin with at least one entry
  mThresholdNumerator = 1.0 ;
  mThresholdDenominator = 1.0 ;

  // Per default the normalization is a free fit parameter 
  mfixNormalization = 0 ;

  // Set defaults for fitting regions
  mSphereLimit = 0.5 ;
  mCore = 0.0 ;

  // Set default for coulomb 
  unCorrectedBackground = 0 ;
  mMeanQinv3dHisto = NULL ;
  mPurity = 1.0 ;
  
  // CERES like fitting needs these parameters
  mMomentumRes = 1.0;
  fCoulombFit  = 0  ;
  
  
  // Set arrays to null
  mNumeratorInternalArray   = NULL ;
  mDenominatorInternalArray = NULL ;
  mErrorInternalArray       = NULL ;
  mVariablePositionArray    = NULL ;
  mCenterOfGravityInQX      = NULL ;
  mCenterOfGravityInQY      = NULL ;
  mCenterOfGravityInQZ      = NULL ;

}
//_______________
T49HbtFitBase::~T49HbtFitBase()
{
  // Delete arrays;
  if(mNumeratorInternalArray)   delete [] mNumeratorInternalArray ;
  if(mDenominatorInternalArray) delete [] mDenominatorInternalArray ;
  if(mErrorInternalArray)       delete [] mErrorInternalArray ;
  if(mVariablePositionArray)    delete [] mVariablePositionArray ;
}
//____________________________
Int_t T49HbtFitBase::FillInternalArrays(Double_t previouslyUsedNormalization)
{
  //  Some output
  cout << " Filling internal arrays with numerator threshold : " << mThresholdNumerator 
       << "  denominator threshold : " << mThresholdDenominator << endl ;
    
    
  // Get the maximum number of bins
  Int_t maximumInternalArraySize = (mNumerator->GetNbinsX()+2) * (mNumerator->GetNbinsY()+2) * (mNumerator->GetNbinsZ()+2) ;
  if ( maximumInternalArraySize != (mDenominator->GetNbinsX()+2) * (mDenominator->GetNbinsY()+2) * (mDenominator->GetNbinsZ()+2) )
    cerr << "Warning : different histogram sizes in numerator and denominator\n " ;
    
  // Now create arrays
  mNumeratorInternalArray   = new Double_t[maximumInternalArraySize] ;
  mDenominatorInternalArray = new Double_t[maximumInternalArraySize] ;
  mErrorInternalArray       = new Double_t[maximumInternalArraySize] ;
  mVariablePositionArray    = new TVector3[maximumInternalArraySize] ;
        
  // Count number of actually used bins
  mInternalArraySize = 0 ;
    
  // Loop over histogram and fill values into vectors 
  for (Int_t index = 0 ; index < maximumInternalArraySize ; index++)
    {
      if ( mNumerator->GetBinContent(index) > mThresholdNumerator && mDenominator->GetBinContent(index) > mThresholdDenominator )
	{
	  // Take center of the BIN ( as corresponding position in qlong,qside,qnull )
	  Int_t binX = 0 ; 
	  Int_t binY = 0 ; 
	  Int_t binZ = 0 ;
	  Bin1ToBin3(mNumerator, index, binX, binY, binZ) ;
	  Double_t qx = mNumerator->GetXaxis()->GetBinCenter(binX) ;
	  Double_t qy = mNumerator->GetYaxis()->GetBinCenter(binY) ;
	  Double_t qz = mNumerator->GetZaxis()->GetBinCenter(binZ) ;

	  //  Check if bin is in fitting range
	  if ( TMath::Abs(qx) < mSphereLimit && TMath::Abs(qy)< mSphereLimit &&  TMath::Abs(qz)< mSphereLimit && 
	      (TMath::Abs(qx) > mCore        || TMath::Abs(qy)> mCore        ||  TMath::Abs(qz) > mCore) )
	    {
	      // Fill the 3d histograms into vectors
	      Double_t num   = mNumerator->GetBinContent(index) ;
	      Double_t denom = mDenominator->GetBinContent(index) ;
	      // Fill numerator
	      mNumeratorInternalArray[mInternalArraySize]    = num ;
	      // Normalize and fill background
	      mDenominatorInternalArray[mInternalArraySize]  = denom ;
			    
	      // The error is cacluated using Gauss' error propagation : e = num/denom * sqrt(1/num + 1/(denom))
	      Double_t error = num/(denom) * sqrt( (1.0/num) + (1.0/denom) ) ;
	      // In case we did the normalization before
	      if(previouslyUsedNormalization!=0.0)
		{
		  if(previouslyUsedNormalization>1.0) cerr << "I need the normalziation, you gave me 1.0/normalziation" << endl ;
		  mfixNormalization = 1 ;
		  // error = num/(denom*(1.0/previouslyUsedNormalization)) * sqrt( (1.0/num) + 1.0/(denom*(1.0/previouslyUsedNormalization)) ) ; // flierl
		  
		  // first calculate the error of the unscaled ratio
		  error = num/(denom*(1.0/previouslyUsedNormalization)) * sqrt( (1.0/num) + 1.0/(denom*(1.0/previouslyUsedNormalization)) ) ;
		  // the rescale the errors with the normalization constant
		  error *= 1.0/previouslyUsedNormalization;
		  
		}
	      mErrorInternalArray[mInternalArraySize]  = error ;

	      if(mCenterOfGravityInQX == NULL || mCenterOfGravityInQY == NULL || mCenterOfGravityInQZ == NULL )
		{
		  // The most simple case : take center of the BIN ( as according position in qlong,qside,qnull )
		  mVariablePositionArray[mInternalArraySize].SetX(qx) ;
		  mVariablePositionArray[mInternalArraySize].SetY(qy) ;
		  mVariablePositionArray[mInternalArraySize].SetZ(qz) ;
		}
	      else
		{
		  // More realisic: take center of gravity in qside,qlong,qout cell
		  mVariablePositionArray[mInternalArraySize].SetX(mCenterOfGravityInQX->GetBinContent(index)) ;
		  mVariablePositionArray[mInternalArraySize].SetY(mCenterOfGravityInQY->GetBinContent(index)) ;
		  mVariablePositionArray[mInternalArraySize].SetZ(mCenterOfGravityInQZ->GetBinContent(index)) ;
		}

	      // Some output for debugging only
	      if (num/(denom) <0.8 && num/(denom)>0.0 && 0) 
		{
		  cout << (Double_t) num << "\t" << (Double_t) (denom) ; 
		  cout << "\t" << (Double_t) (num/(denom)) ;
		  Double_t xx = mNumerator->GetXaxis()->GetBinCenter(binX) ;
		  Double_t yy = mNumerator->GetYaxis()->GetBinCenter(binY) ;
		  Double_t zz = mNumerator->GetZaxis()->GetBinCenter(binZ) ;
		  cout << "xa " << xx << "\t" << binX << "\t" ;
		  cout << "ya " << yy << "\t" << binY << "\t" ;
		  cout << "za " << zz << "\t" << binZ << endl ;
		}
			  
	      // Fill 3d ratio for projection purposes
	      mRatio->SetBinContent(index,num/(denom)) ;
	      mRatio->SetBinError(index, error) ;
	      
	      // Count entries
	      mInternalArraySize++ ;
	    }
	}
    }
  // Done
  cout << "Internal array size :" << mInternalArraySize << endl ;
  return mInternalArraySize; // for chi^2/NDF
}
//________________________________
void T49HbtFitBase::SetHistos(TH3D* numerator, TH3D* denominator)
{
  // Set the numerator and denominator
  mNumerator = numerator ;
  mDenominator = denominator ;

  // Calculate the ratio 
  mRatio = (TH3D*) numerator->Clone() ;
  mRatio->Reset() ;
  mRatio->Divide(numerator,denominator) ;
}
//________________________________
void T49HbtFitBase::SetCentersOfGravity(TH3D* CenterOfGravityInQX, TH3D* CenterOfGravityInQY, TH3D* CenterOfGravityInQZ) 
{
  mCenterOfGravityInQX = CenterOfGravityInQX ;
  mCenterOfGravityInQY = CenterOfGravityInQY ;
  mCenterOfGravityInQZ = CenterOfGravityInQZ ;
}
//____________________________
void T49HbtFitBase::Bin1ToBin3(TH3D* histo, Int_t bin, Int_t& binx, Int_t& biny, Int_t&  binz)
{
  // Actually this should be provided by ROOT ... grrrr
  // return bin number of every axis (binx,biny,binz) if the general bin number (bin) is known
  // general bin = binX + binY * (nbinX+2) + binZ * ((nbinx+2) * (nbiny+2) )   (+2 acounts for over and underflow!)
  Int_t nbbinX = (histo->GetNbinsX()) + 2 ;
  Int_t nbbinY = (histo->GetNbinsY()) + 2 ;
    
  binx = bin%nbbinX ;
  biny = (bin/nbbinX) % nbbinY ;
  binz = (bin/nbbinX) / nbbinY ;
    
}
//____________________________
TH3D* T49HbtFitBase::Ratio()
{
  // If normalization was part of the fit we have to scale the ratio histogram for projection etc. ...
  Double_t currentValue ;
  Double_t currentError ;
  mMinuit->GetParameter(0, currentValue, currentError) ;
  mRatio->Scale(1.0/currentValue) ; 
  return mRatio ;
}
//____________________________
void T49HbtFitBase::SetCoulombCalculator(TH3D* MeanQinv3dHisto, T49HbtCoulomb* CoulombCalculator, Double_t purity , Double_t momentumres) 
{
  // Switch coulomb correction on
  unCorrectedBackground = 1 ; 
  mMeanQinv3dHisto = MeanQinv3dHisto ; 
  mCoulombCalculator = CoulombCalculator ;  
  if(!mMeanQinv3dHisto || !mCoulombCalculator) cerr << "Error: Didn't get histos correctly ! " << endl ; 
  mPurity = purity ;
  mMomentumRes = momentumres;
}
//________________________________
Double_t T49HbtFitBase::Lnfactorial(Double_t arg)
{
    // Return the factorial of arg
    Double_t fac = 1.0 ;
    Int_t iarg = (Int_t) arg ;
    if(iarg<50)
	{
	    for (int index = 1; index < iarg+1 ; index++)
		{ fac *=(Double_t)index; } ;
	    fac = log(fac) ;
	}
    else 
	{
	  // If the argument is too large : use Stirlings formula
	    fac = 0.91 + (iarg+0.5) * log((float)iarg) - iarg ;
	}

    return fac;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

T49FitProjector::T49FitProjector(T49HbtFitBase* dFitter) 
{ 
  // Init my fitter
  mFitter = dFitter ;
  
  // Get the 3d hist
  mRatio = mFitter->Ratio() ;
  
  // If the ratio histogram does not exist 
  if (!mRatio)
    {
      cerr << " Attention no ratio histogram ! " << endl ;
    }

  // Get minuit to extract the fit result
  TMinuit* tMinuit = dFitter->getMinuit() ;

  // Fill fit result into array
  mFitParameters = new Double_t[6] ;
  mFitParameterErrors = new Double_t[6] ;
  cout << "Found fit parameters : " << "\t" ;
  for( Int_t index = 0 ; index < 6 ; index++ )
    {
      Double_t value = 0 ;
      Double_t error = 0 ;
      tMinuit->GetParameter(index, value, error) ;
      mFitParameters[index] = value ;
      mFitParameterErrors[index]= error;
      cout << value << "\t" ;
    }
  cout << endl ;
} ;
//___________________
T49FitProjector::~T49FitProjector() 
{ 
  delete [] mFitParameters ;
  delete [] mFitParameterErrors ;
} ;
//___________________
void T49FitProjector::Clear()
{
  if(m1dpro)      delete m1dpro ;
  if(m1dfit)      delete m1dfit ;
  if(m1dweight)   delete m1dweight ;
  if(m1dcounter)  delete m1dcounter ;
  if(m1derror)    delete m1derror ;
}
//___________________
TH1D* T49FitProjector::get1dProjection(TString axis, Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax)
{
  // The 1d projection is done by summing over the two axis not equal to "axis" in the function call.
  // The summing is done by weighting each addend with its inverse error  S = sigma(a_i * (1.0/e_i) / E) with E = sigma(1.0/e_i)
  // The error is calculated based on regular error propagation :  totalerror = sqrt(n) / E  with n = number of addends

  // Get ranges of the ratio 3d histogram 
  Int_t xminBin = mRatio->GetXaxis()->FindFixBin(xmin) ;
  Int_t xmaxBin = mRatio->GetXaxis()->FindFixBin(xmax) ;  Int_t yminBin = mRatio->GetYaxis()->FindFixBin(ymin) ;
  Int_t ymaxBin = mRatio->GetYaxis()->FindFixBin(ymax) ;
  Int_t zminBin = mRatio->GetZaxis()->FindFixBin(zmin) ;
  Int_t zmaxBin = mRatio->GetZaxis()->FindFixBin(zmax) ;

  // Calculate size of the projection histo
  Int_t proNbins = 0 ;
  Double_t proMin = 0.0 ;
  Double_t proMax = 0.0 ;
  char* axistitle = new char[60] ;
  if (axis == "x")        
    {  
      proNbins = abs(xmaxBin-xminBin)+1 ;  
      proMin   = mRatio->GetXaxis()->GetBinLowEdge(xminBin) ; 
      proMax   = mRatio->GetXaxis()->GetBinUpEdge(xmaxBin) ; 
      strcpy(axistitle,mRatio->GetXaxis()->GetTitle()) ; 
    }
  else if (axis == "y")   
    {  
      proNbins = abs(ymaxBin-yminBin)+1 ;  
      proMin   = mRatio->GetYaxis()->GetBinLowEdge(yminBin) ; 
      proMax   = mRatio->GetYaxis()->GetBinUpEdge(ymaxBin) ; 
      strcpy(axistitle,mRatio->GetYaxis()->GetTitle()) ; 
    }
  else if (axis == "z")   
    {  
      proNbins = abs(zmaxBin-zminBin)+1 ;  
      proMin   =  mRatio->GetZaxis()->GetBinLowEdge(zminBin) ; 
      proMax   =  mRatio->GetZaxis()->GetBinUpEdge(zmaxBin) ; 
      strcpy(axistitle,mRatio->GetZaxis()->GetTitle()) ; }
  else 
    { 
      cerr << "Wrong axis in projector, this should not happen ! "<< endl ; 
      return 0; 
    } ;

  
  // Create 1d histos
  Int_t ran = rand() ; // to distinguish between objects give them a random id ...
 
  // Result histo
  m1dpro = new TH1D("my1dpro","my1dpro",proNbins,proMin,proMax) ; 
  m1dpro->SetXTitle(axistitle) ;
  m1dpro->SetTitle(axistitle) ;
  Char_t restitle[100] ; 
  sprintf(restitle,"fit%s%d\n",axis.Data(),ran) ;
  m1dpro->SetName(restitle) ;

  // Fit histo
  m1dfit = new TH1D("my1dfit","my1dfit",proNbins,proMin,proMax) ;
  Char_t fittitle[100] ; 
  sprintf(fittitle,"fit%s%d\n",axis.Data(),ran) ;
  m1dfit->SetName(fittitle);
  m1dfit->SetLineColor(2) ;
  m1dfit->SetLineStyle(1) ;
  m1dfit->SetLineWidth(1) ;

  // Weight histo
  m1dweight = new TH1D("my1dweight","my1dweight",proNbins,proMin,proMax) ;
  Char_t weightmtitle[100] ; 
  sprintf(weightmtitle,"weightm%s%d\n",axis.Data(),ran) ;
  m1dweight->SetName(weightmtitle);

  // Counter histo
  m1dcounter = new TH1D("my1dcounter","my1dcounter",proNbins,proMin,proMax) ;
  Char_t countermtitle[100] ; 
  sprintf(countermtitle,"counterm%s%d\n",axis.Data(),ran) ;
  m1dcounter->SetName(countermtitle);

  // Fill histo
  for(Int_t indexX = xminBin ; indexX <= xmaxBin ; indexX++)
    {
      for(Int_t indexY = yminBin ; indexY <= ymaxBin ; indexY++)
	{
	  for(Int_t indexZ = zminBin ; indexZ <= zmaxBin ; indexZ++)
	    {
	      
	      // Get content of the 3d cell in ratio histo
	      Double_t cellContent = mRatio->GetBinContent(indexX,indexY,indexZ) ;
	      
	      
	      // Get error of this cell
	      Double_t cellContentError = mRatio->GetBinError(indexX,indexY,indexZ) ;
	      
	      // Get the content of this 3d cell using the fit function
	      TVector3 position(mRatio->GetXaxis()->GetBinCenter(indexX),
				mRatio->GetYaxis()->GetBinCenter(indexY),
				mRatio->GetZaxis()->GetBinCenter(indexZ));
	      Double_t cellContentFromFit = mFitter->TheoreticalValue(&position,mFitParameters) ;
	      
	      // Fill the 1d histos
	      Int_t proBin = -100 ;
	      if (axis == "x")        { proBin = m1dpro->GetXaxis()->FindFixBin(mRatio->GetXaxis()->GetBinCenter(indexX)); }  
	      else if (axis == "y")   { proBin = m1dpro->GetXaxis()->FindFixBin(mRatio->GetYaxis()->GetBinCenter(indexY)); }
	      else if (axis == "z")   { proBin = m1dpro->GetXaxis()->FindFixBin(mRatio->GetZaxis()->GetBinCenter(indexZ)); }
	      
	      // Accept only meaningfull cells
	      if( cellContent > 0.0 && cellContentError > 0.0 )
		{
		  // Fill projection 1d histo, weighted with 1/error
		  m1dpro->AddBinContent(proBin,cellContent/cellContentError ) ;
		  
		  // Fill fit 1d histo equally weighted with 1/error
		  m1dfit->AddBinContent(proBin,cellContentFromFit/cellContentError ) ;
		  
		  // Fill 1/error histogram to normalize
		  m1dweight->AddBinContent(proBin,1.0/cellContentError) ;
		  
		  // Count number of addends (German: Summanden :))
		  m1dcounter->AddBinContent(proBin,1.0) ;
		  
		  // Some ouput for debugging only
	   	  if ( proBin == 10 && 0) 
		    {
		       cout << proBin << " " ;
		      cout << "posx " << mRatio->GetXaxis()->GetBinCenter(indexX) << "  ";
		      cout << "posy " << mRatio->GetYaxis()->GetBinCenter(indexY) << "  ";
		      cout << "posz " << mRatio->GetZaxis()->GetBinCenter(indexZ) << "  ";
		      cout << "content " << cellContent << "  " ;
		      cout << "fit " <<  cellContentFromFit << "  " ;
		      cout << "error " << cellContentError  << "   " ;
		      cout << endl ;
		    }
		}
	    }
	}
    }
  
  // Normalize fit and projection histos 
  m1dpro->Divide(m1dweight) ;
  m1dfit->Divide(m1dweight) ;
 
  // Now attach an error bar to each data point, with error = number of addens / (sum over 1 over error) ;
  for(Int_t binIndex=1 ; binIndex <= m1dpro->GetNbinsX() ;  binIndex++)
    {
      if( m1dcounter->GetBinContent(binIndex) > 0.0 && m1dweight->GetBinContent(binIndex) != 0.0 )
	{
	  // Use sqrt since the errors are squard 
	  m1dpro->SetBinError(binIndex,sqrt(m1dcounter->GetBinContent(binIndex))/m1dweight->GetBinContent(binIndex)) ;
	 
	}
    }
  
  // Return projection
  return m1dpro ;
}
//___________________
TH2D* T49FitProjector::get2dProjection(TString axis, Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax)
{
  // The projection is done by summing over entries in direction "axis" specified when the function is called
  // The entries are weighted by 1 over the statistical error

  // Get ranges of the ratio 3d histogram 
  Int_t xminBin = mRatio->GetXaxis()->FindFixBin(xmin) ;
  Int_t xmaxBin = mRatio->GetXaxis()->FindFixBin(xmax) ;
  Int_t yminBin = mRatio->GetYaxis()->FindFixBin(ymin) ;
  Int_t ymaxBin = mRatio->GetYaxis()->FindFixBin(ymax) ;
  Int_t zminBin = mRatio->GetZaxis()->FindFixBin(zmin) ;
  Int_t zmaxBin = mRatio->GetZaxis()->FindFixBin(zmax) ;
    
  // Get dimensions of the projection histos
  Int_t proNbinsX = 0 ;
  Double_t proMinX = 0.0 ;
  Double_t proMaxX = 0.0 ;
  Char_t* axistitleX = new char[100] ;
  Int_t proNbinsY = 0 ;
  Double_t proMinY = 0.0 ;
  Double_t proMaxY = 0.0 ;
  Char_t* axistitleY = new char[100] ;

  // Determine the dimension of the 2d projection histogram
  if (axis == "x")        	
    {  
      proNbinsX = abs(ymaxBin-yminBin)+1 ;  
      proMinX   = mRatio->GetYaxis()->GetBinLowEdge(yminBin) ; 
      proMaxX   = mRatio->GetYaxis()->GetBinUpEdge(ymaxBin) ;
      strcpy(axistitleX,mRatio->GetYaxis()->GetTitle()) ; 
      proNbinsY = abs(zmaxBin-zminBin)+1 ;  
      proMinY   = mRatio->GetZaxis()->GetBinLowEdge(zminBin) ; 
      proMaxY   = mRatio->GetZaxis()->GetBinUpEdge(zmaxBin) ; 
      strcpy(axistitleY,mRatio->GetZaxis()->GetTitle()) ; 
    }
  else if (axis == "y")   
    {  
      proNbinsX =  abs(zmaxBin-zminBin)+1 ;  
      proMinX   =  mRatio->GetZaxis()->GetBinLowEdge(zminBin) ;
      proMaxX   =  mRatio->GetZaxis()->GetBinUpEdge(zmaxBin) ; 
      strcpy(axistitleX,mRatio->GetZaxis()->GetTitle()) ; 
      proNbinsY =  abs(xmaxBin-xminBin)+1 ;  
      proMinY   =  mRatio->GetXaxis()->GetBinLowEdge(xminBin) ; 
      proMaxY   =  mRatio->GetXaxis()->GetBinUpEdge(xmaxBin) ; 
      strcpy(axistitleY,mRatio->GetXaxis()->GetTitle()) ;
    }
  else if (axis == "z")   
    {  
      proNbinsX = abs(xmaxBin-xminBin)+1 ;  
      proMinX   = mRatio->GetXaxis()->GetBinLowEdge(xminBin) ; 
      proMaxX   = mRatio->GetXaxis()->GetBinUpEdge(xmaxBin) ; 
      strcpy(axistitleX,mRatio->GetXaxis()->GetTitle()) ; 
      proNbinsY = abs(ymaxBin-yminBin)+1 ;  
      proMinY   = mRatio->GetYaxis()->GetBinLowEdge(yminBin) ; 
      proMaxY   = mRatio->GetYaxis()->GetBinUpEdge(ymaxBin) ;
      strcpy(axistitleY,mRatio->GetYaxis()->GetTitle()) ;
    }
  else 
    { 
      cerr << "Wrong axis in projector, this should not happen ! "<< endl ; 
      return 0; 
    } ;
    
  //
  // Create 2d histos
  //
  Int_t ran = (Int_t) rand() ; // to distinguish between objects give them a special random id ...

  // Projection
  m2dpro = new TH2D("my2dpro","my2dpro",proNbinsX,proMinX,proMaxX,proNbinsY,proMinY,proMaxY) ; 
  Char_t resName[100] ; sprintf(resName,"res%s%d\n",axis.Data(),ran) ;  m2dpro->SetName(resName) ;
  m2dpro->SetXTitle(axistitleX) ;
  m2dpro->SetYTitle(axistitleY) ;
  Char_t restitle[100] ; sprintf(restitle,"%s\n",axis.Data()) ;m2dpro->SetTitle(restitle) ;

  // Fit
  m2dfit = new TH2D("my2dfit","my2dfit",proNbinsX,proMinX,proMaxX,proNbinsY,proMinY,proMaxY) ;
  Char_t fittitle[100] ; sprintf(fittitle,"fit%s%d\n",axis.Data(),ran) ; m2dfit->SetName(fittitle) ;

  // Norm
  m2dweight = new TH2D("my2dweight","my2dweight",proNbinsX,proMinX,proMaxX,proNbinsY,proMinY,proMaxY) ;
  char weightmtitle[100] ; sprintf(weightmtitle,"weightm%s%d\n",axis.Data(),ran) ; m2dweight->SetName(weightmtitle) ;
    
  // Fill histo
  for(Int_t indexX = xminBin ; indexX <= xmaxBin ; indexX++)
    {
      for(Int_t indexY = yminBin ; indexY <= ymaxBin ; indexY++)
	{
	  for(Int_t indexZ = zminBin ; indexZ <= zmaxBin ; indexZ++)
	    {
	      // Get the content of the 3d cell
	      Double_t cellcontent = mRatio->GetBinContent(indexX,indexY,indexZ) ;

	      // Get error of this cell
	      Double_t cellContentError = mRatio->GetBinError(indexX,indexY,indexZ) ;
			  
	      // Get the content of the 3d cell using the fit function
	      TVector3 position(mRatio->GetXaxis()->GetBinCenter(indexX),
				mRatio->GetYaxis()->GetBinCenter(indexY),
				mRatio->GetZaxis()->GetBinCenter(indexZ));

	      Double_t cellContentFromFit = mFitter->TheoreticalValue(&position,mFitParameters) ;
	      
	      // Fill the 2d histos
	      Int_t proBin = -100 ;
			  
	      if (axis == "x")        
		{ 
		  proBin = m2dpro->FindBin(mRatio->GetYaxis()->GetBinCenter(indexY),mRatio->GetZaxis()->GetBinCenter(indexZ)) ; 
		}  
	      else if (axis == "y")   
		{ 
		  proBin = m2dpro->FindBin(mRatio->GetZaxis()->GetBinCenter(indexZ),mRatio->GetXaxis()->GetBinCenter(indexX)) ; 
		}
	      else if (axis == "z")   
		{ 
		  proBin = m2dpro->FindBin(mRatio->GetXaxis()->GetBinCenter(indexX),mRatio->GetYaxis()->GetBinCenter(indexY)) ; 
		}
	      
	      if( cellcontent>0.5 && cellcontent <1.5 && cellContentError > 0.0 )
		{
		  // Fill projection 2d histo 
		  m2dpro->AddBinContent(proBin,cellcontent/cellContentError) ;
				    
		  // Fill fit 2d histo
		  m2dfit->AddBinContent(proBin,cellContentFromFit/cellContentError) ;

		  // Fill 1/error histogram to normalize
		  m2dweight->AddBinContent(proBin,1.0/cellContentError) ;
		}
	    }
	}
    }

  // normalize fit and projection histos
  m2dpro->Divide(m2dweight) ;
  m2dfit->Divide(m2dweight) ;
    
  // return projection
  return m2dpro ;    
}
//___________________
void T49HbtFitBase::doFit()
{
  // everything is set up: start minimization !

  // Errorflag
  Int_t ierflg = 0 ;
  
  // check whether the TMinuit-object was initialized or not
  if (mMinuit->GetNumPars() == 0)
    {
      cout << "TMinuit not intitialized => calling standard initialization." << endl;
      InitMinuit();
    }

  // if normalization is derived elsewhere we simply fix it here
  if(mfixNormalization)
    {
      mMinuit->mnparm(0, "Normalization",  1.0, 0.01,  0,0,ierflg) ;
      mMinuit->FixParameter(0) ;
    }
  
  // set function to call
  mMinuit->SetObjectFit(this) ;
  mMinuit->SetFCN(gfcn) ;
  mMinuit->Migrad() ;
}
