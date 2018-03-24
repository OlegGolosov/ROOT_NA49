#ifndef _T49HbtMomentumResolutionCalculator_INCLUDED_
#define _T49HbtMomentumResolutionCalculator_INCLUDED_

///////////////////////////////////////////////////////////////
//
// Comments see T49HbtMomentumResolutionCalculation.C
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
#include "TFile.h"
#include "TGraph.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class T49HbtMomentumResolutionCalculator : public TObject 
{
 public:
  // Constructors
  T49HbtMomentumResolutionCalculator(TH3D* MomentumDifferenceOfMCTracks,
			   Int_t nKtBins, Double_t minKt, Double_t maxKt, Int_t nYBins, Double_t minY, Double_t maxY) ;
  ~T49HbtMomentumResolutionCalculator() ;
  
  // Members
  void Use(TObjArray*  MCTrackList, TObjArray*  RETrackList) ; // Enter MC and RC track lists
  void SelectedAssocicatedTracks(TObjArray* MCTrackList, TObjArray* RETrackList) ; // Find best match
  void FillDisplacedMomenta() ; // Loop over pairs, caluclate momentumf differences and store them
  void FillDisplacedMomenta(T49ParticleMCRoot* MC1, T49ParticleMCRoot* MC2) ;
  void PrintResolution3dHisots(Int_t KtYIndex) ; // Print out result
  TObjArray* GetResolution3dHisots() ; // Get the result histograms
  TObjArray* GetResolutionDistributions(Int_t KtYIndex) ; // Get summary of result histograms
  void SaveResolution(Char_t* Name) ; // Save result to file
  Int_t GetKtYIndex(Double_t lKtPair, Double_t lYPair) ;

 private:
  TH3D* mMomentumDifferenceOfMCTracks ; // Master 3d histo
  Int_t mNumberOfCells ;                // Number of cells in master 3d histo

  TObjArray*  mSelectedMCTracks ;       // To hold MC tracks
  TObjArray*  mSelectedRETracks ;       // To hold RE tracks
  
  TObjArray* mListOfListOfDisplacedMomenta ; // List of list of displaced momenta
  TObjArray* mResolution ;                   // Array holding the final result
  Int_t mNumberOfResolutionHistos ;          // Number of final result histos
  
  Double_t mPionMass ;       // Constant
   
  // kt-Y binning
  Double_t* mKtBins ; 
  Double_t* mYBins ;
  Int_t mNKtBins ;
  Int_t mNYBins ;
  Int_t mMaxKtY ;
    
  ClassDef(T49HbtMomentumResolutionCalculator,1)
} ;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class T49HbtMomentumResolutionProvider : public TObject 
{
 public:
  // Constructor: Init class with name of result file produced by T49HbtMomentumResolutionCalculator class
  T49HbtMomentumResolutionProvider(Char_t* Name) ;
  ~T49HbtMomentumResolutionProvider() {} ;

  // Members
  Int_t GetKtYIndex(Double_t lKtPair, Double_t lYPair) ; // Derive Kt-Y index
  
  // Main functionaly of this class: Get Mean and RMS of momentum resolution in a given kt-Y bin for a certain qside, qout, qlong pair
  void GetMeanAndRMS(Double_t ktPair, Double_t YPair, 
		     Double_t qout, Double_t qside, Double_t qlong,
		     Double_t& MeanQout, Double_t& MeanQside, Double_t& MeanQlong,
		     Double_t& RMSQout , Double_t& RMSQside , Double_t& RMSQlong ) ;
  
 private:
  TFile* mResultFile ;      // Pointer to result file written out by T49HbtMomentumResolutionCalculator class
  TObjArray* mResolution ;  // TObjArray holding RMS and Mean TH3D's for each side-out-long bin
  TH3D* mMaster3d ;         // Master 3d Histo to locate side-out-long bin
  
  // Kt-Y binning
  Double_t* mKtBins ;
  Double_t* mYBins ;
  Int_t mNKtBins ;
  Int_t mNYBins ;
  Int_t mMaxKtY ;
  
  ClassDef(T49HbtMomentumResolutionProvider,1)
} ;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
