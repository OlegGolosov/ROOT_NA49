#ifndef _T49HbtMomentumResolutionCorrection_INCLUDED_
#define _T49HbtMomentumResolutionCorrection_INCLUDED_

///////////////////////////////////////////////////////////////
//
// Comments see T49HbtMomentumResolutionCorrection.C
//
// written by D. Flierl 
//
//
//////////////////////////////////////////////////////////////

#include <iostream>
#include "TClonesArray.h"
#include "TH3D.h"
#include "T49ParticleRoot.h"
#include "T49ParticleMCRoot.h"
#include "TLorentzVector.h"
#include "T49Tool.h"
#include "T49Proj2.h"
#include "T49HbtCoulomb.h"
#include "TF1.h"
#include "T49Cut2Track.h"
#include "T49HbtMomentumResolutionCalculation.h"

using namespace std;

class T49HbtMomentumResolutionCorrection : public TObject
{

 public:
  T49HbtMomentumResolutionCorrection(Int_t nKt, Double_t minKt, Double_t maxKt, 
				     Int_t nY, Double_t minY, Double_t maxY,
				     Int_t nx, Float_t xmin, Float_t xmax, 
				     Int_t ny, Float_t ymin, Float_t ymax,
				     Int_t nz, Float_t zmin, Float_t zmax ) ;
				     

  ~T49HbtMomentumResolutionCorrection() {} ;
  void Fill(TObjArray* ListA, TObjArray* ListB, T49Cut2Track* T2Cut = NULL);
  void Fill(T49ParticleRoot* t1, T49ParticleRoot* t2) ;
  TObjArray* Get3dCorrectionHistograms() ;
  Int_t GetKtYIndex(Double_t lKt, Double_t lYpp) ;
  void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2, Double_t& lQside, Double_t& lQout, Double_t& lQlong) ;
  TH3D* GetIdealSignal(Int_t lKtYIndex)       {  return ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+0)) ; } ;
  TH3D* GetIdealBackground(Int_t lKtYIndex)   {  return ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+1)) ; } ;
  TH3D* GetSmearedSignal(Int_t lKtYIndex)     {  return ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+2)) ; } ;
  TH3D* GetSmearedBackground(Int_t lKtYIndex) {  return ((TH3D*) mHistoArray->At(lKtYIndex*mNHistos+3)) ; } ;

  void SetMomentumResolutionProvider(T49HbtMomentumResolutionProvider* aMomentumResolutionProvider) ;
  void SetHBTRadii(Double_t Lambda, Double_t Rside, Double_t Rout, Double_t Rlong) ;
  void SetCoulombRadius(Double_t CoulombRadius) ;
  void SetPurity(Double_t Purity) ;
  void SetMomentumResolution(Double_t DefaultResolutionQside, Double_t DefaultResolutionQout, Double_t DefaultResolutionQlong) 
      { 
	  mDefaultResolutionQside = DefaultResolutionQside ;
	  mDefaultResolutionQout  = DefaultResolutionQout  ;
	  mDefaultResolutionQlong = DefaultResolutionQlong ;
      }
  void SwitchCoulombOff() { mIncludeCoulombEffect = 0 ;}

 private:
  // Kt-Y bins
  Int_t mMaxKtY ;
  Int_t mNKtBins ;
  Double_t* mKtBins ;
  Int_t mNYBins ;
  Double_t* mYBins ;
  Int_t  mNHistos ;
  // qSide, qOut, qLong
  Double_t mMinQside ;
  Double_t mMaxQside  ;
  Double_t mMinQout ; 
  Double_t mMaxQout ; 
  Double_t mMinQlong ; 
  Double_t mMaxQlong ;
  TObjArray* mHistoArray ;
  // Histograms with the differential momentum resolution
  T49HbtMomentumResolutionProvider* mMomentumResolutionProvider ;
  TRandom* mRand ;
  Double_t mPionMass ;
  // Purity
  Double_t mPurity ;
  // HBT
  Double_t mLambda ;
  Double_t mRside ;
  Double_t mRout ;
  Double_t mRlong ;
  // Coulomb
  T49HbtCoulomb* mCoulombCalculator ;
  Double_t mCoulombRadius ;
  Int_t mIncludeCoulombEffect ;
  // Default momentum resolution
  Double_t mDefaultResolutionQside ;
  Double_t mDefaultResolutionQout ;
  Double_t mDefaultResolutionQlong ;

  // Constants of nature
  Double_t mHbarC2 ;

  // Statistics
  Double_t* mTotalNumberOfInquries ;
  Double_t* mNumberBadInquiries ;

  ClassDef(T49HbtMomentumResolutionCorrection,1)
    
} ;

#endif
