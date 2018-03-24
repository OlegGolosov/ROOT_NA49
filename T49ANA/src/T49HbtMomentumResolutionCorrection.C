#include "T49HbtMomentumResolutionCorrection.h"
ClassImp(T49HbtMomentumResolutionCorrection)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This class produces a 3d histogramms containing the momentum resolution correction. The measred correlation function
//should be multiplied with this histogramm to get the momentum.
//This class fills takes as input particle lists. It combines pairs and fills four histogramms, 
//ideal signal distbritution (pairs are weight according to BE and coulomb), ideal background (weight =1)
//and smeared signal and background. The smearing is either done by default momentum resolution or by
//resolution obtain from Monte Carlo (provided via the T49HbtMomentumResolutionProvider class).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
T49HbtMomentumResolutionCorrection::T49HbtMomentumResolutionCorrection(Int_t nKt, Double_t minKt, Double_t maxKt, 
								       Int_t nY, Double_t minY, Double_t maxY,
								       Int_t nx, Float_t xmin, Float_t xmax, 
								       Int_t ny, Float_t ymin, Float_t ymax,
								       Int_t nz, Float_t zmin, Float_t zmax)
{
  
  // Init size of TH3D's
  mMinQside = xmin ;
  mMaxQside = xmax ;
  mMinQout  = ymin ;
  mMaxQout  = ymax ;
  mMinQlong = zmin ;
  mMaxQlong = zmax ;
  
  // Init Kt-Y bins
  mNKtBins = nKt ;
  mKtBins = new Double_t[mNKtBins+1] ;
  for(Int_t index=0 ; index<(mNKtBins+1) ; index++)
    {
      mKtBins[index] = ((maxKt-minKt)/mNKtBins) * (Double_t) index + minKt;
    }
  // Y bins
  mNYBins = nY ;
  mYBins  = new Double_t[mNYBins+1] ;
  for(Int_t index=0 ; index<(mNYBins+1) ; index++)
    {
      mYBins[index]= ((maxY-minY)/mNYBins) * (Double_t) index + minY;
    }
  // Max Number of KtY bins
  mMaxKtY = mNKtBins  *  mNYBins ;
  
  //
  // Set default init values
  //
  // HBT parameters
  mLambda = 0.8 ;
  mRside  = 5.0 ;
  mRout   = 6.0 ;
  mRlong  = 7.0 ;
  // Purity
  mPurity = 0.5 ;
  // CoulombCalculator
  mCoulombCalculator = new T49HbtCoulomb() ;
  mCoulombRadius = 10 ; 
  mCoulombCalculator->SetMeanSeparation(mCoulombRadius) ;
  mIncludeCoulombEffect = 1 ;
  
  // Init constants
  mPionMass = 0.139 ;
  mHbarC2 = 0.0389379 ;

  // Init radom generator
  mRand = new TRandom() ;

  // Per default do not use momentum resolution from MC events
  mMomentumResolutionProvider  = NULL ;
  mTotalNumberOfInquries       = NULL ;
  mNumberBadInquiries          = NULL ; 

  // Set default momentum resolution
  mDefaultResolutionQside = 0.01 ;
  mDefaultResolutionQout  = 0.01 ;
  mDefaultResolutionQlong = 0.01 ;

  //
  // Create 5 Histos for signal and background, smeared and unsmeared, and finally the correction
  //
  mNHistos = 5 ;
  // Names for ideal, smeared and correction distributions
  TString lIdealSignal("IdealSignal") ;
  TString lIdealBackground("IdealBackground") ;
  TString lSmearedSignal("SmearedSignal") ;
  TString lSmearedBackground("SmearedBackground") ;
  TString lCorrection("Correction") ;
  
  mHistoArray = new TObjArray(mMaxKtY*mNHistos) ;
  for(Int_t lKtYIndex = 0 ; lKtYIndex < mMaxKtY ; lKtYIndex++)
    {
      for(Int_t lHistoIndex = 0 ; lHistoIndex < mNHistos ; lHistoIndex++)
	{
	  TString lName("Histo_KtYBin") ;
	  lName += lKtYIndex ;
	  
	  switch (lHistoIndex) 
	    {
	    case 0:
	      lName.Append(lIdealSignal) ;
	      break ;
	    case 1:
	      lName.Append(lIdealBackground) ;
	      break ;
	    case 2:
	      lName.Append(lSmearedSignal) ;
	      break ;
	    case 3:
	      lName.Append(lSmearedBackground) ;
	      break ;
	    case 4 :
	      lName.Append(lCorrection) ;
	      break ;
	    default:
	      cerr << "Serious error: should never occur !" << endl ; 
	      break ;
	    }
	  // Create Histogramm
	  TH3D* lHistogram = new TH3D(lName.Data(),lName.Data(),nx,xmin,xmax,ny,ymin,ymax,nz,zmin,zmax);
	  
	  // Store it in TObjArray
	  mHistoArray->AddAt(lHistogram,(lKtYIndex*mNHistos)+lHistoIndex) ;
	}
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCorrection::SetMomentumResolutionProvider(T49HbtMomentumResolutionProvider* aMomentumResolutionProvider)
{
  mMomentumResolutionProvider  = aMomentumResolutionProvider ;
  if(!aMomentumResolutionProvider)
    {
      cerr << "Warning: Object supposed to contain momentum resolution is empty ! " << endl; 
    }
  // Arrange for some statistics
  mTotalNumberOfInquries = new Double_t[mMaxKtY] ;
  mNumberBadInquiries =  new Double_t[mMaxKtY] ;
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCorrection::SetHBTRadii(Double_t Lambda, Double_t Rside, Double_t Rout, Double_t Rlong)
{
  mLambda = Lambda ;
  mRside  = Rside ;
  mRout   = Rout ;
  mRlong  = Rlong ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCorrection::SetCoulombRadius(Double_t CoulombRadius)
{
  mCoulombRadius = CoulombRadius ; 
  mCoulombCalculator->SetMeanSeparation(mCoulombRadius) ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCorrection::SetPurity(Double_t Purity)
{
   mPurity = Purity ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCorrection::Fill(TObjArray* ListA, TObjArray* ListB, T49Cut2Track* T2Cut)
{
  // Make sure we get two differen lists ! We need to use the uncorrelated background.
  if(ListA==ListB)
    {
      cerr << "We need two set of tracks from DIFFERENT events !" << endl ;
    }
    
  T49ParticleRoot *Part1;
  T49ParticleRoot *Part2;
  
  Int_t IP1 = 0;
  Int_t IP2 = 0;

  // Loop over ListA
  IP1 = 0;
  TIter NextParticle(ListA);
  while ((Part1 = ((T49ParticleRoot *) NextParticle()))) 
    { 
      IP1++; 
      IP2 = 0;
      TIter NextP2(ListB);
      // Loop over ListB
      while ((Part2 = ((T49ParticleRoot *) NextP2()))) 
	{ 
	  IP2++; 
	  if (Part1 == Part2) 
	    {
	      cerr << "Error: Identical track in two different lists! Should never happen!" << endl  ;
	    }
	  if ((T2Cut == NULL) || (T2Cut->CheckPair(Part1,Part2))) 
	    {
	      if ((Part1->GetPx() == Part2->GetPx()) && 
		  (Part1->GetPy() == Part2->GetPy()) && 		  
		  (Part1->GetPz() == Part2->GetPz()) ) 
		{
		  cerr << "Error: Identical track in two different lists! Should never happen!" << endl ;
		}
	      Fill(Part1,Part2);
	    }
	}
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int_t  T49HbtMomentumResolutionCorrection::GetKtYIndex(Double_t lKt, Double_t lYpp)
{
  // kt
  Int_t lKtIndex = 0 ;
  if( lKt < mKtBins[0] || lKt > mKtBins[mNKtBins] )
    { 
       lKtIndex = -1 ;
    } 
  else 
    { 
       while( lKt > mKtBins[lKtIndex+1] )
	 {
	   lKtIndex++;
	 } 
    }
  // Y
  Int_t lYIndex = 0 ;
  if( lYpp < mYBins[0] || lYpp > mYBins[mNYBins] )
    { 
      lYIndex = -1 ;
    } 
  else 
    { 
      while( lYpp > mYBins[lYIndex+1] )
	{
	  lYIndex++;
	} 
    }
  // result
  return (lKtIndex+lYIndex*mNKtBins) ;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCorrection::Fill(T49ParticleRoot* t1, T49ParticleRoot* t2)
{
  //
  // Fill pairs into TH3Ds: weighted and unweighted, smeared and unsmeared
  //
  // Calculate Kt-Y bin
  Double_t lKt  = 0.5 * TMath::Sqrt(T49Tool::Px(t1,t2)*T49Tool::Px(t1,t2) + T49Tool::Py(t1,t2)*T49Tool::Py(t1,t2)) ;
  Double_t lYpp = 0.5 * TMath::Log((t1->GetE(mPionMass)+t2->GetE(mPionMass)
				    +t1->GetPz()+t2->GetPz())
				   /(t1->GetE(mPionMass)+t2->GetE(mPionMass)
				     - t1->GetPz()-t2->GetPz()));
  Int_t lKtYIndex = GetKtYIndex(lKt,lYpp) ;
  
  if ((lKtYIndex > -1) && (lKtYIndex < mMaxKtY)) 
    { 
      // Calculate qout,qside and qlong
      Double_t lQside = 0.0 ;
      Double_t lQout  = 0.0 ;
      Double_t lQlong = 0.0 ;
      QCalc(t1,t2, lQside, lQout, lQlong) ;

      if( lQside > mMinQside && lQside < mMaxQside && 
	  lQout  > mMinQout  && lQout  < mMaxQout  &&
	  lQlong > mMinQlong && lQlong < mMaxQlong)
	{
	  //
	  // Ideal Distributions
	  // 

	  // Calculate weight for signal: Weight = purity * [ C_BoseEinstein(qSide,qOut,qLong;Rout,Rside,Rlong) * Ccoulomb(qinv;Rinv ] + (1-p)
	  Double_t mBoseEinsteinWeight = 
	    1.0 + mLambda * exp(((-1)*(lQout*lQout*mRout*mRout) +(-1)*(lQside*lQside*mRside*mRside)+(-1)*(lQlong*lQlong*mRlong*mRlong) )/ mHbarC2 ) ; 
	  Double_t lQinv = T49Tool::Qinv(t1,t2,mPionMass,mPionMass) ;
	  Double_t mCoulombWeight = 1.0 ;
	  if(mIncludeCoulombEffect==1)
	    {
	      mCoulombWeight =  mCoulombCalculator->Weight(lQinv) ;
	    }
	  Double_t mWeight = mPurity *  (mBoseEinsteinWeight * mCoulombWeight) + (1-mPurity) ;
	  	  
	  // Fill IdealSignal
	  ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos))->Fill(lQside,lQout,lQlong,mWeight) ;

	  // Background
	  ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+1))->Fill(lQside,lQout,lQlong,1.0) ;
     
	  //
	  // Smeared distributions
	  // 
	  // Smear momentum 
	  Double_t MeanQside = 0.0 ;
	  Double_t RMSQside  = 0.0 ;
	  Double_t MeanQout  = 0.0 ;
	  Double_t RMSQout   = 0.0 ;
	  Double_t MeanQlong = 0.0 ;
	  Double_t RMSQlong  = 0.0 ;
	
	  // If defined, get momentum resolution from "database"
	  if(mMomentumResolutionProvider)
	    {
	      mMomentumResolutionProvider->GetMeanAndRMS(lKt,lYpp,lQside,lQout,lQlong,MeanQside,MeanQout,MeanQlong,RMSQside,RMSQout,RMSQlong) ;
	      mTotalNumberOfInquries[lKtYIndex]++;
	      if(RMSQside == 0.0 && RMSQout == 0.0 && RMSQlong == 0.0 && MeanQside== 0.0 && MeanQout == 0.0 && MeanQlong == 0.0)
		{
		  mNumberBadInquiries[lKtYIndex]++;
		}
	    }

	  // Qside
	  Double_t lRandomShiftQside = 0.0 ;
	  if(RMSQside>0.0001 && mMomentumResolutionProvider)
	    {
	      lRandomShiftQside  = mRand->Gaus(MeanQside,RMSQside) ;
	    }
	  else
	    {
	      lRandomShiftQside  = mRand->Gaus(0.0,mDefaultResolutionQside) ;
	    }
	  Double_t lQsideSmeared = lQside + lRandomShiftQside ;
	  if(lQsideSmeared<0.0)
	    {
	      // To ensure symmetry throw these rare cases away
	      return ;
	    }

	  // Qout
	  Double_t lRandomShiftQout = 0.0 ;
	  if(RMSQout>0.0001 && mMomentumResolutionProvider)
	    {
	      lRandomShiftQout  = mRand->Gaus(MeanQout,RMSQout) ;
	    }
	  else
	    {
	      lRandomShiftQout  = mRand->Gaus(0.0,mDefaultResolutionQout) ;
	    }
	  Double_t lQoutSmeared = lQout + lRandomShiftQout ;
	  
	  // Qlong
	  Double_t lRandomShiftQlong = 0.0 ;
	  if(RMSQlong>0.0001 && mMomentumResolutionProvider)
	    {
	      lRandomShiftQlong  = mRand->Gaus(MeanQlong,RMSQlong) ;
	    }
	  else
	    {
	      lRandomShiftQlong  = mRand->Gaus(0.0,mDefaultResolutionQlong) ;
	    }
	  Double_t lQlongSmeared = lQlong + lRandomShiftQlong ;

	  // Fill smeared signal
	  ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+2))->Fill(lQsideSmeared,lQoutSmeared,lQlongSmeared,mWeight) ;
	  
	  // Fill smeared background
	  ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+3))->Fill(lQsideSmeared,lQoutSmeared,lQlongSmeared,1.0) ;
	}
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  T49HbtMomentumResolutionCorrection::
QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2, Double_t& lQside, Double_t& lQout, Double_t& lQlong)
{
  
  // LCMS: calculate pair beta  
  Double_t lBeta = (Double_t) ((t1->GetPz() + t2->GetPz()) /(t1->GetE(mPionMass) + t2->GetE(mPionMass)));
  // Boost pairs to LCMS
  TLorentzVector P1 = T49Tool::zLorentz(t1,mPionMass,lBeta);
  TLorentzVector P2 = T49Tool::zLorentz(t2,mPionMass,lBeta);
  
  // Pair kt,qSide,qOut and qLong
  Double_t ktPAIR = TMath::Sqrt((P1.X()+P2.X())*(P1.X()+P2.X())
				+(P1.Y()+P2.Y())*(P1.Y()+P2.Y()));
  lQside  = ((P1.X()-P2.X())*(P1.Y()+P2.Y()) 
	     -(P1.Y()-P2.Y())*(P1.X()+P2.X())) / ktPAIR; // Qside              
  lQout  = (P1.X()*P1.X()-P2.X()*P2.X() 
	    +P1.Y()*P1.Y()-P2.Y()*P2.Y())     / ktPAIR; // Qout
  lQlong =  P1.Z() - P2.Z();                            // Qlong
  
  // Allow only one orientation
  if ( lQside < 0) 
    {
      lQside = -lQside; 
      lQout  = -lQout; 
      lQlong = -lQlong;
    }      

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TObjArray* T49HbtMomentumResolutionCorrection::Get3dCorrectionHistograms() 
{
  // Create TObjArrays with correction histos
  TObjArray* mCorrectionHistogramArray = new TObjArray(mMaxKtY) ;
  
  for(Int_t lKtYIndex = 0 ; lKtYIndex < mMaxKtY ; lKtYIndex++)
    {
      TH3D* mCorrectionHisto = (TH3D*) mHistoArray->At(lKtYIndex*mNHistos+4) ;
      mCorrectionHistogramArray->AddAt(mCorrectionHisto, lKtYIndex) ;

      // Loop over Histo and fill it
      for( Int_t mBinIndex = 0 ; 
	   mBinIndex < (mCorrectionHisto->GetNbinsX()+2)*(mCorrectionHisto->GetNbinsY()+2)*(mCorrectionHisto->GetNbinsZ()+2);
	   mBinIndex++ )
	{
	  // Per default set every bin to 1.0
	  mCorrectionHisto->SetBinContent(mBinIndex,1.0) ;
	  
	  // Now calculate the correction factor
	  Double_t mISignal = ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+0))->GetBinContent(mBinIndex) ;
	  Double_t mIBack   = ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+1))->GetBinContent(mBinIndex) ;
	  Double_t mSSignal = ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+2))->GetBinContent(mBinIndex) ;
	  Double_t mSBack   = ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+3))->GetBinContent(mBinIndex) ;
	
	  if( mISignal != 0.0 &&  mIBack != 0.0 && mSSignal != 0.0 && mSBack!=0.0 )
	    {
	      Double_t mCorrectionFactor = (mISignal/mIBack) / (mSSignal/mSBack) ;
	      if( mCorrectionFactor > 0.1 && mCorrectionFactor < 10.0 )
		{
		  mCorrectionHisto->SetBinContent(mBinIndex,mCorrectionFactor) ;
		}
	      else
		{
		  cerr << "Warning: correction factor out of bounds for bin " << mBinIndex << endl ;
		}
	    }
	 
	} // Loop over histogram bins
            
      // If we used the database print out some stats
      if(mMomentumResolutionProvider)
	{
	  cout << "Number of inquieries : " << mTotalNumberOfInquries[lKtYIndex]<< "\t" ;
	  cout << "\t bad inquiries : " << mNumberBadInquiries[lKtYIndex] << "  (" 
	       << (mNumberBadInquiries[lKtYIndex]/mTotalNumberOfInquries[lKtYIndex]) * 100.0<< "%)" << endl ;
	}
    } // Loop over Kt-Y bins
  
  // Return resulting TObjArray
  return mCorrectionHistogramArray ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
