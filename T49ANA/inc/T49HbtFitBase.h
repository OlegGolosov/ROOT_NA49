/***************************************************************************
 * Author: Dominik Flierl, flierl@ikf.uni-frankfurt.de
 ***************************************************************************
 *   This class uses TMinuit to fit HBT correlationfunctions.
 *   The idea is to fill measured 3d correlation histos into internal vectors 
 *   and perform the chi2 summing on these vectors.
 *   TMinuit is used in the doFit function to minimize the chi2 or mml function
 *   with respect to the parameter set.
 *   Normalization can be used as an additional fit parameter
 *   Coulomb correction and purity can also be set.
 *   Like sign as well as unlikesing pairs can be used
 *   projections can be plotted using the T49FitProjector class.
 **************************************************************************/

// revision 2004/10/18 calt
// int FillInternalArrays(..) returns the internal array size for Chi Square over degrees of freedom

// revision 2004/10/07 calt
// moved 'doFit()' and 'gfcn(..)' functions to base class
// added virtual function 'InitMinuit()' to initialize TMinuit with standard start values
// in 'FillInternalArrays(..)': removed bug in the calculation of the errors if the
// background distribution was normalized before invoking this function

// Revision 2004/09/10 skniege 
// added flag  for fitmethod  (SetCoulombFitFunction) 
// added float for momentum resolution 
// added set and get function for fit results 
 
#ifndef T49HbtFitBase_hh
#define T49HbtFitBase_hh

#include "TH3.h"
#include "TH2.h"
#include "TH1.h"
#include "TMinuit.h"
#include "TVector3.h"
#include "TString.h"
#include "TObject.h"
#include "T49HbtCoulomb.h"

class T49HbtFitBase : public TObject
{
public :
  // Constructors
  T49HbtFitBase( TMinuit* gMinuit, TH3D* numerator, TH3D* denominator) ;
  ~T49HbtFitBase() ;
  
  // Members
  Int_t  FillInternalArrays(Double_t previouslyUsedNormalization = 0.0 ) ;  // Fill internal arrays
  void SetCoulombCalculator(TH3D* MeanQinv3dHisto, T49HbtCoulomb* CoulombCalculator, Double_t purity , Double_t momentumres = 1 ) ;  // Cacluate coulomb correction
  void SetCoulombFitFunction(Int_t coulfit = 0 ){ fCoulombFit = coulfit ;}   // Implemented for T49HbtFit_BP_LS_CHI2 for the time beeing 14.09.2004
  void doFit();   // Start minimazation algorithm (using TMinuit)

  // Pure virtual functions
  virtual void mfcn(Int_t &nParamters, Double_t *gin, Double_t &finalChi2, Double_t *parameterSet, Int_t iflag)  = 0 ; // The general fcn 
  virtual Double_t TheoreticalValue(TVector3* position, Double_t* parameterSet ) = 0;
  virtual void InitMinuit() = 0;

  // Get histos
  TH3D* Numerator() {return mNumerator;};
  TH3D* Denominator() {return mDenominator;};
  TH3D* Ratio() ;
	
  // Get/set thresholod
  void SetThresholdNumerator(Double_t thresN) { mThresholdNumerator = thresN ; }; 
  void SetThresholdDenominator(Double_t thresD) { mThresholdDenominator = thresD ; };
  Double_t ThresholdNumerator() {return mThresholdNumerator; }; 
  Double_t ThresholdDenominator() {return mThresholdDenominator; };
  
 
  
  // Set input histos
  void SetHistos(TH3D* numerator, TH3D* denominator) ;
  void SetCentersOfGravity(TH3D* CenterOfGravityInQX, TH3D* CenterOfGravityInQY, TH3D* CenterOfGravityInQZ) ;
  
  // Get Minuit
  TMinuit* getMinuit() { return mMinuit ; } ; 

  // Options
  void SetSphereLimit(Double_t limit) { mSphereLimit = limit; } ;
  Double_t GetSphereLimit() { return mSphereLimit; } ;

  void SetUnfittedCore(Double_t core) { mCore = core ; } 
  Double_t GetUnfittedCore() { return mCore; } ;

  
	
protected :	
  // Pointer to TMinuit itself ( it's already global, but well you know ... :) )
  TMinuit* mMinuit ;
  
  // Internal Arrays
  int mInternalArraySize ;
  Double_t* mRatioInternalArray ;
  Double_t* mNumeratorInternalArray ;
  Double_t* mDenominatorInternalArray ;
  Double_t* mErrorInternalArray ;
  TVector3* mVariablePositionArray ;

  // Options
  Int_t mfixNormalization ;          // If 1 normalization is done elsewhere and not a fit parameter
  Double_t mSphereLimit ; 	     // Spherelimit
  Double_t mCore ;	             // Core
  Int_t    unCorrectedBackground ;   // Switch on/off coulomb effect included in the fit
  Double_t mPurity ;                 // Purity needed to estimate number of pairs which show BE
  Double_t mMomentumRes ;            // Reduction of lambda due to momentum resolution (CERES ) 
  Int_t    fCoulombFit  ;            // Flag for coulomb-fit-function ( 0 : Sinyukov ; 1 : PBM ; 2 : CERES )
  

  // Coulomb calculator provides the coulomb weight
  T49HbtCoulomb* mCoulombCalculator ;
    
  // Basic functions which are not yet provided by ROOT itself 
  void Bin1ToBin3(TH3D* histo, int bin, int& binx, int& biny, int& binz) ;
  
  // Helper function for maximul likelihood fit
  Double_t Lnfactorial(Double_t arg) ;
		
  // Count how often we called fcn -> check minuit :)
  Int_t countMinuitCalls ;
	
  // The histos 
  TH3D* mNumerator ; 
  TH3D* mDenominator ;
  TH3D* mRatio ; 
  TH3D* mMeanQinv3dHisto ;
  TH3D* mCenterOfGravityInQX ;
  TH3D* mCenterOfGravityInQY ;
  TH3D* mCenterOfGravityInQZ ;

  // Parameters
  Double_t mThresholdNumerator ;
  Double_t mThresholdDenominator ; 
	


  // Constants
  Double_t mhc ;  // 0.197 GeVfm
  Double_t mhc2 ; // 0.038 (GeVfm)^2
	
  // Root dictionary
  ClassDef(T49HbtFitBase, 0) 
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class T49FitProjector:public TObject
{
public :
  // Constructors
  T49FitProjector(T49HbtFitBase* mFitter) ;
  ~T49FitProjector() ;
  
  // Reset
  void Clear() ;

  // Do project
  TH1D* get1dProjection(TString axis, Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax) ;
  TH2D* get2dProjection(TString axis, Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax) ;
  
  // Set/get 3d ratio
  void setRatio(TH3D* ratio) { mRatio = ratio ; } ;
  TH3D* getRatio() { return mRatio ; } ;
    
  // Get histos
  TH1D* get1dFit() { return m1dfit ; } ;
  TH1D* get1dNorm() { return m1dnor ; } ;
  TH1D* get1dError() { return m1derror ; } ;
  TH2D* get2dFit() { return m2dfit ; } ;
  TH2D* get2dNorm() { return m2dweight ; } ;

  // Get Fit Results
  Double_t* GetErrors() {return  mFitParameterErrors;} ;
  Double_t* GetFitParameters() {return  mFitParameters;};
  

private :	
	
  // The histos 
  TH3D* mRatio ;

  // Projections
  // 1d
  TH1D* m1dpro ;
  TH1D* m1dfit ;
  TH1D* m1dnor ;
  TH1D* m1derror ;
  TH1D* m1dcounter ;
  TH1D* m1dweight ;

  // 2d
  TH2D* m2dpro ;
  TH2D* m2dfit ;
  TH2D* m2dweight ;

  // Fit parameters
  Double_t* mFitParameters; 
  Double_t* mFitParameterErrors;
  T49HbtFitBase* mFitter;
      
  // Root dictionary
  ClassDef(T49FitProjector, 0) 
} ;

// ifndef T49HbtFitBase_hh
#endif
