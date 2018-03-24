/***************************************************************************
 * Author: Dominik Flierl, flierl@ikf.uni-frankfurt.de
 ***************************************************************************/

// Revision 2004/09/10 skniege 
// Add Ceres methods  T49HbtFit_BP_LS_CHI2::TheoreticalValue()


using namespace std ;

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "T49HbtFitDivers.h"

ClassImp(T49HbtFit_BP_LS_CHI2)
ClassImp(T49HbtFit_BP_LS_MML)
ClassImp(T49HbtFit_BP_ULS_CHI2)
ClassImp(T49HbtFit_YKP_LS_CHI2)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtFit_BP_LS_CHI2::mfcn(Int_t &nParameters, Double_t *gin, Double_t &finalChi2, Double_t *parameterSet, Int_t iflag) 
{
  // calculate chi2 of the given paramset 'parameterSet'
            
  // loop over Internal Array and caluclate chi2
  double chi2 = 0.0 ;
  for (Int_t index = 0 ;  index < mInternalArraySize ; index++)
    {
      // calculate the expected52 value at this point (variablePosition) for this set of paramters, 
      // attention the return value might be coulomb corrected !
      
     
      
      Double_t theoreticalValue  = TheoreticalValue( &mVariablePositionArray[index], parameterSet) ; 
      //cout <<  &mVariablePositionArray[index] << endl ;

      // now we have to consider the normalization
      theoreticalValue = theoreticalValue *  parameterSet[0] ;
      
      // get the difference between measured and predicted value
      double delta = (mNumeratorInternalArray[index]/mDenominatorInternalArray[index]-theoreticalValue)/mErrorInternalArray[index];
     
      // sum the chi2
      chi2 += delta*delta;
    }
    
  // chi2 is calculated
  finalChi2 = chi2 ;
};
//____________________________
Double_t T49HbtFit_BP_LS_CHI2::TheoreticalValue(TVector3* position, double* parameterSet )
{

  //if(position->X()<0.0) { cout << "unmoeglich !" << endl ; } 

  // return C2 at 'position' with 'parameterSet' for Bertsch-Pratt parametrization, if set, with coulomb correction

  // position->x = qside
  // position->y = qout 
  // position->z = qlong  
  // parameterSet[0] = normalization
  // parameterSet[1] = lambda        // corresponds to the measured reduction of the correlationfunction for CERES fit procedure 
  // parameterSet[2] = Rside 
  // parameterSet[3] = Rout  
  // parameterSet[4] = Rlong
  // parameterSet[5] = Routlong
  
  Double_t valueFromCorrelationFunction = 1.0;
  Double_t coulombWeight = 1.0;

  switch (fCoulombFit) 
    {
      
    case 0 :   //Sinyukov coulomb weight (Na49 fit function)
	
      valueFromCorrelationFunction  = 1.0 + parameterSet[1] * exp(
								  (
								   (-1.0) * pow(position->x(),2) * pow(parameterSet[2],2) 
								   - pow(position->y(),2) * pow(parameterSet[3],2) 
								   - pow(position->z(),2) * pow(parameterSet[4],2) 
								   - 2* position->y() * position->z() * pow(parameterSet[5],2) 
								   ) 
								  / mhc2 );
      
      if(unCorrectedBackground == 1 && mCoulombCalculator)
	{
	  coulombWeight =  
	    mCoulombCalculator->Weight(mMeanQinv3dHisto->GetBinContent(mMeanQinv3dHisto->GetXaxis()->FindBin(position->X()),
								       mMeanQinv3dHisto->GetYaxis()->FindBin(position->Y()),
								       mMeanQinv3dHisto->GetZaxis()->FindBin(position->Z()))) ;
	}
      
      // see equation 7 : C++ = p A Cqs + (1-p) 
      //valueFromCorrelationFunction =  mPurity * coulombWeight * valueFromCorrelationFunction +  (1.0-mPurity)  ;
      // added mom resolution to compare to ceres procedure  
      valueFromCorrelationFunction =  mPurity * valueFromCorrelationFunction*(mMomentumRes*coulombWeight  +(1-mMomentumRes)) +  (1.0-mPurity)  ;
     
      
      break;
      
    case 1 : // PBM coulomb weight (Na49 fit function)
     
      valueFromCorrelationFunction  = 1.0 + parameterSet[1] * exp(
								  (
								   (-1.0) * pow(position->x(),2) * pow(parameterSet[2],2) 
								   - pow(position->y(),2) * pow(parameterSet[3],2) 
								   - pow(position->z(),2) * pow(parameterSet[4],2) 
								   - 2* position->y() * position->z() * pow(parameterSet[5],2) 
								   ) 
								  / mhc2 );
      if(unCorrectedBackground == 1 && mCoulombCalculator)
	{
	  coulombWeight = 
	    mCoulombCalculator->ClassicalWeight(mMeanQinv3dHisto->GetBinContent(mMeanQinv3dHisto->GetXaxis()->FindBin(position->X()),
										mMeanQinv3dHisto->GetYaxis()->FindBin(position->Y()),
										mMeanQinv3dHisto->GetZaxis()->FindBin(position->Z()))) ;
	}
      valueFromCorrelationFunction =  mPurity * coulombWeight * valueFromCorrelationFunction +  (1.0-mPurity)  ;
      
    
      
      break;
      
    case 2 :  // PBM coulomb weight ( CERES fit function )
      
      valueFromCorrelationFunction  = 1.0 +  exp(
						 (
						   (-1.0) * pow(position->x(),2) * pow(parameterSet[2],2) 
						   - pow(position->y(),2) * pow(parameterSet[3],2) 
						   - pow(position->z(),2) * pow(parameterSet[4],2) 
						   - 2* position->y() * position->z() * pow(parameterSet[5],2) 
						   ) 
						  / mhc2 );
      if(unCorrectedBackground == 1 && mCoulombCalculator)
	{
	  coulombWeight = 
	    mCoulombCalculator->ClassicalWeight(mMeanQinv3dHisto->GetBinContent(mMeanQinv3dHisto->GetXaxis()->FindBin(position->X()),
										mMeanQinv3dHisto->GetYaxis()->FindBin(position->Y()),
										mMeanQinv3dHisto->GetZaxis()->FindBin(position->Z()))) ;
	}

      valueFromCorrelationFunction =  parameterSet[1]*(valueFromCorrelationFunction*(mMomentumRes*coulombWeight  +(1-mMomentumRes))) +  (1.0-parameterSet[1])  ;
      
      break;
      
    }
  
  if(valueFromCorrelationFunction==0)
    {       cout << " coulombWeight " << coulombWeight << endl ;
    cout << " valueFromCorrelationFunction    " <<  valueFromCorrelationFunction << endl;
    }
  // return result
  return valueFromCorrelationFunction ;
}

//____________________________
void T49HbtFit_BP_LS_CHI2::InitMinuit() 
{
  // initialization with default start values

  // Errorflag
  Int_t ierflg = 0 ;
  mMinuit->mnparm(0, "Normalization", 0.1, 0.01, 0,0,ierflg);
  mMinuit->mnparm(1, "lambda",        0.5, 0.1, 0,0,ierflg);
  mMinuit->mnparm(2, "Rside",         5.0,  0.1, 0,0,ierflg);
  mMinuit->mnparm(3, "Rout",          5.0,  0.1, 0,0,ierflg);
  mMinuit->mnparm(4, "Rlong",         5.0,  0.1, 0,0,ierflg);
  mMinuit->mnparm(5, "Routlong",      5.0,  0.1, 0,0,ierflg);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtFit_BP_LS_MML::mfcn(Int_t &nParameters, Double_t *gin, Double_t &finalMLH, Double_t *parameterSet, Int_t iflag) 
{
  // calculate maximum likelihood asuming an underlying poisson distribution, for details see for example
  // http://nedwww.ipac.caltech.edu/level5/Leo/Stats_contents.html
  /*
    poison distribution : p(r) = y^r * exp(-y) / r!      y = mean
    likelihood distribution to be maximized L, n number of measurements (x1....xn) :
    L = -n*y + Sum(xi * ln(y)) - Sum(ln(xi!)) = Sum( -y + xi * ln(y) - ln(xi!) )
  */
      
  // some definitions
  Double_t MLH  = 0.0 ;
             
  // loop over Internal Array and caluclate chi2 
  for (Int_t index = 0 ;  index < mInternalArraySize ; index++)
	{
	  // calculate the expected value at this point (variablePosition) for this set of paramters
	  Double_t expectedValue  = TheoreticalValue( &mVariablePositionArray[index] , parameterSet) * mDenominatorInternalArray[index] ; 

	  // now we have to consider the normalization
	  expectedValue = expectedValue  *  parameterSet[0] ;
	  
	  // measured value
	  Double_t measueredValue = mNumeratorInternalArray[index] ;
	  // sum up
	  MLH += -expectedValue + measueredValue * log(expectedValue) - Lnfactorial(measueredValue) ;
	}
    // likelihood for this set of paramters
    finalMLH = - MLH ;
}
//____________________________
Double_t T49HbtFit_BP_LS_MML::TheoreticalValue(TVector3* position, double* parameterSet )
{
  // return C2 at 'position' with 'parameterSet' for Bertsch-Pratt parametrization, if set, with coulomb correction

  // position.x = qside
  // position.y = qout 
  // position.z = qlong  
  // parameterSet[0] = normalization
  // parameterSet[1] = lambda 
  // parameterSet[2] = Rside 
  // parameterSet[3] = Rout  
  // parameterSet[4] = Rlong
  // parameterSet[5] = Routlong
  Double_t valueFromCorrelationFunction  = 1.0 + parameterSet[1] * exp(
						       (
							(-1.0) * pow(position->x(),2) * pow(parameterSet[2],2) 
							- pow(position->y(),2) * pow(parameterSet[3],2) 
							- pow(position->z(),2) * pow(parameterSet[4],2) 
							- 2* position->y() * position->z() * pow(parameterSet[5],2) 
							) 
						       / mhc2 );
  
  // now check coulomb must be included
  if(unCorrectedBackground == 1 && mCoulombCalculator)
    {
      Double_t coulombWeight = 
	mCoulombCalculator->Weight(mMeanQinv3dHisto->GetBinContent(mMeanQinv3dHisto->GetXaxis()->FindBin(position->X()),
								   mMeanQinv3dHisto->GetYaxis()->FindBin(position->Y()),
								   mMeanQinv3dHisto->GetZaxis()->FindBin(position->Z()))) ;

      // see equation 7 : C++ = p A Cqs + (1-p) 
      valueFromCorrelationFunction =  mPurity * coulombWeight * valueFromCorrelationFunction +  (1.0-mPurity)  ;
    
    }

  // return result
  return valueFromCorrelationFunction ;
}
//____________________________
void T49HbtFit_BP_LS_MML::InitMinuit() 
{  
  // initialization with default start values

  // Errorflag
  Int_t ierflg = 0 ;
  mMinuit->mnparm(0, "Normalization", 0.05, 0.01, 0,0,ierflg);
  mMinuit->mnparm(1, "lambda",        0.5, 0.1, 0,0,ierflg);
  mMinuit->mnparm(2, "Rside",         5.0,  0.1, 0,0,ierflg);
  mMinuit->mnparm(3, "Rout",          5.0,  0.1, 0,0,ierflg);
  mMinuit->mnparm(4, "Rlong",         5.0,  0.1, 0,0,ierflg);
  mMinuit->mnparm(5, "Routlong",      5.0,  0.1, 0,0,ierflg);
  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtFit_BP_ULS_CHI2::mfcn(Int_t &nParameters, Double_t *gin, Double_t &finalChi2, Double_t *parameterSet, Int_t iflag) 
{
  // calculate chi2 of the given paramset 'parameterSet'
  Double_t chi2 = 0.0 ;

  // set coulomb radius, according to parameterSet (the second parameter (normalization) is varied below)
  mCoulombCalculator->SetMeanSeparation(parameterSet[1]) ;

  // loop over Internal Array and caluclate chi2 
  for (Int_t index = 0 ;  index < mInternalArraySize ; index++)
    {
      // get the coulomb weight for the mean qinv in this (qout,qside,qlong)-bin for a given coulomb radius and vary normalization
      Double_t theoreticalValue  =  parameterSet[0] * TheoreticalValue(&mVariablePositionArray[index], parameterSet) ;
     	  
      // get the difference between measured and predicted value
      double delta = (mNumeratorInternalArray[index]/mDenominatorInternalArray[index]-theoreticalValue)/mErrorInternalArray[index];
      
      // sum the chi2
      chi2 += delta*delta;
    }
   
  // chi2 has been calculated:
  finalChi2 = chi2 ;
};
//____________________________
Double_t T49HbtFit_BP_ULS_CHI2::TheoreticalValue(TVector3* position, double* parameterSet )
{
  // calculate the expected value in this (qout,qside,qlong) cell
  // in the coulomb case (unlikesign pairs) the expected value only depends on qinv, so first we go and get the mean qinv
  Double_t qinv = mMeanQinv3dHisto->GetBinContent(mMeanQinv3dHisto->GetXaxis()->FindBin(position->X()),
						  mMeanQinv3dHisto->GetYaxis()->FindBin(position->Y()),
						  mMeanQinv3dHisto->GetZaxis()->FindBin(position->Z())) ;

  // get the coulomb weight for this qinv, where the coulomb radius has been set before !
  Double_t theoreticalValue  = mCoulombCalculator->WeightUnlikeSignPairs(qinv) ;
  
  // if you want it the classical way, use this one !
  // Double_t theoreticalValue  = mCoulombCalculator->ClassicalWeightUnlikeSignPairs(qinv) ;   

  // take the purity into account
  theoreticalValue =  mPurity * theoreticalValue + (1.0-mPurity)  ;
  
  return theoreticalValue ;
}
//____________________________
void T49HbtFit_BP_ULS_CHI2::InitMinuit() 
{
  // initialization with default start values

  // Errorflag
  Int_t ierflg = 0 ;
  mMinuit->mnparm(0, "Normalization",  0.11, 0.01,  0,0,ierflg) ;
  mMinuit->mnparm(1, "MeanSeparation", 9.0,  1.0, 0,0,ierflg) ;
  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtFit_YKP_LS_CHI2::mfcn(Int_t &nParameters, Double_t *gin, Double_t &finalChi2, Double_t *parameterSet, Int_t iflag) 
{
  // calculate chi2 of the given paramset 'parameterSet'
            
  // loop over Internal Array and caluclate chi2
  double chi2 = 0.0 ;
  for (Int_t index = 0 ;  index < mInternalArraySize ; index++)
    {
      // calculate the expected value at this point (variablePosition) for this set of paramters, 
      // attention the return value might be coulomb corrected !
      Double_t theoreticalValue  = TheoreticalValue( &mVariablePositionArray[index], parameterSet) ; 

      // now we have to consider the normalization
      theoreticalValue = theoreticalValue *  parameterSet[0] ;
      
      // get the difference between measured and predicted value
      double delta = (mNumeratorInternalArray[index]/mDenominatorInternalArray[index]-theoreticalValue)/mErrorInternalArray[index];
     
      // sum the chi2
      chi2 += delta*delta;
    }
    
  // chi2 is calculated
  finalChi2 = chi2 ;
};
//____________________________
Double_t T49HbtFit_YKP_LS_CHI2::TheoreticalValue(TVector3* position, double* parameterSet )
{
  // return C2 at 'position' with Parameterset 'parameterSet' for YKP parametrization
    
  // position.x = qlong 
  // position.y = qperp 
  // position.z = qnull    
  // parameterSet[0] = constant 
  // parameterSet[1] = lambda 
  // parameterSet[2] = Rlong  
  // parameterSet[3] = Rperp  
  // parameterSet[4] = Rnull
  // parameterSet[5] = beta
  
  Double_t gamma2 = 1.0/fabs(1.0-pow(parameterSet[5],2)) ;
  Double_t valueFromCorrelationFunction =  parameterSet[0] +  parameterSet[1] * exp(
						    (
						     (-1.0) * pow(position->y(),2) * pow(parameterSet[3],2) 
						     - gamma2 * pow((position->x() - parameterSet[5]*position->z()),2) * pow(parameterSet[2],2)
						     - gamma2 * pow((position->z() - parameterSet[5]*position->x()),2) * pow(parameterSet[4],2)
						     ) 
						    / double(mhc2) );
   
  // now check coulomb must be included
  if(unCorrectedBackground == 1 && mCoulombCalculator)
    {
      Double_t coulombWeight = 
	mCoulombCalculator->Weight(mMeanQinv3dHisto->GetBinContent(mMeanQinv3dHisto->GetXaxis()->FindBin(position->X()),
								   mMeanQinv3dHisto->GetYaxis()->FindBin(position->Y()),
								   mMeanQinv3dHisto->GetZaxis()->FindBin(position->Z()))) ;

      // see equation 7 : C++ = p A Cqs + (1-p) 
      valueFromCorrelationFunction =  mPurity * coulombWeight * valueFromCorrelationFunction +  (1.0-mPurity)  ;
    
    }

  // return result
  return valueFromCorrelationFunction ;
}
//____________________________
void T49HbtFit_YKP_LS_CHI2::InitMinuit() 
{
  // initialization with default start values
  
  // Errorflag
  Int_t ierflg = 0 ;
  mMinuit->mnparm(0, "Normalization", 0.1, 0.01, 0,0,ierflg);
  mMinuit->mnparm(1, "lambda",        0.05, 0.1, 0,0,ierflg);
  mMinuit->mnparm(2, "Rlong",         5.0,  0.1, 0,0,ierflg);
  mMinuit->mnparm(3, "Rperp",          5.0,  0.1, 0,0,ierflg);
  mMinuit->mnparm(4, "Rnull",         5.0,  0.1, 0,0,ierflg);
  mMinuit->mnparm(5, "beta",      5.0,  0.1, 0,0,ierflg);
  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
