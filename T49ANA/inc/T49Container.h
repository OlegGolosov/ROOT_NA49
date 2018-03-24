#ifndef _T49Container_INCLUDED_
#define _T49Container_INCLUDED_

/*
$Log: T49Container.h,v $
Revision 1.10  2002/10/07 09:52:13  cblume
Update for CASTOR

 * Revision 1.5  2000/07/07  14:13:59  cblume
 * Made ROOT v2.25 compatibel by Marco
 *
 * Revision 1.3  2000/01/30  14:45:27  cblume
 * New versions from Marco with updated comments
 *
 * Revision 1.2  2000/01/17  15:54:34  cblume
 * Comment out GetList declaration
 *
 * Revision 1.1  2000/01/17  12:37:40  cblume
 * Add T49Container.h
 *
*/

#include "TObject.h"
#include "TFile.h"
#include "TList.h"
#include "TObjArray.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1.h"
#include "TAxis.h"
#include "T49Index.h"
#include "T49DedxInfo.h"
#include "T49ParticleRoot.h"

class T49Container : public TObject {
  
public: 
  T49Container();
  virtual ~T49Container();
  
  Int_t Init(const Int_t NPBin, Float_t PMin, Float_t PMax,
             Int_t NPtBin, Float_t PtMin, Float_t PtMax,
             Int_t NPhiBin, Bool_t ypt_flag=0, Float_t mass=0.49367);

  void  StoreNPoint(Bool_t flag=1, Int_t nbins=0, Float_t min=0, Float_t max=0);
  Int_t     GetTPC()                      { return fTPC;}
  void      SetTPC(Int_t tpc)             { fTPC=tpc; }
  void      SetNEvent(UInt_t n_event)     { fNEvent=n_event;}
  UInt_t    GetNEvent()                   { return fNEvent;}
  void      SetPowSigScale(Float_t power) { fPowSigScale=power;}
  Float_t   GetPowSigScale()              { return fPowSigScale;}
  
  TH1F      *GetHist(T49Index *Index);
  TH1F      *GetNpHist(T49Index *Index);
  T49DedxInfo   *GetInfo(T49Index *Index);
  TAxis     *GetPtotAxis()                  {return fPtotAxis; }
  Bool_t    FillContainer(T49ParticleRoot *Particle);

  void      GetIndex(T49ParticleRoot *Particle,T49Index *Index);
  void      GetIndex(Double_t *X,T49Index *Index);

  void      FindBin(T49Index *Index)     {C3Mom2Index(Index);}
  void      FindPhase(T49Index *Index)   {Index2C3Mom(Index);}
  Float_t   GetProbability(T49ParticleRoot* part,Int_t type);
  Int_t     GetDeltaDedx(T49ParticleRoot* part, Int_t type, Float_t& delta);
  Bool_t    GetProbabilityOld(T49ParticleRoot* part,Float_t* prob);
  Int_t     GetDeltaDedxOld(T49ParticleRoot* part, Int_t type, Float_t& delta);

  Bool_t    IsIn(T49ParticleRoot *particle);
  Bool_t    IsIn(Double_t  *X);
  Bool_t    IndexIn(T49Index *Index);
  Bool_t    IsInBin(T49ParticleRoot *Particle, T49Index *Index);  
  Bool_t    IsInBin(Double_t *X, T49Index *Index);


 private:
  void      Index2C3Mom(T49Index *Index);
  void      C3Mom2Index(T49Index *Index);
  Int_t     Bin2ListNum(T49Index *Index);

  ClassDef(T49Container,9)  // Container class to hold dE/dx information   
    
    protected:

  TFile     *fRootFile;
  TAxis     *fChargeAxis;  //Use TAxis to calculate indices
  TAxis     *fPtotAxis;    //Use TAxis to calculate indices
  TAxis     *fPtAxis;      //Use TAxis to calculate indices
  TAxis     *fPhiAxis;     //Use TAxis to calculate indices

  TObjArray  *fInfoList;     //List of infos on histogramm to store
  TObjArray  *fHistList;     //List of histogramms to store

  Int_t      fBins[4];     //number of bins in different dimensions  
  Int_t      fNHistTot;    //Total Number of histogramms in Container
  Int_t      fHistBins;    //Number of bins of histogramms in Container
  Float_t    fHistHiBound; //Histogram Boundaries
  Float_t    fHistLoBound;
  Float_t    fMinCharge;
  Float_t    fMaxCharge;
  Float_t    fMinPtot;
  Float_t    fMaxPtot;
  Axis_t     fMinPt;       //Changed for root v2.25
  Axis_t     fMaxPt;
  Axis_t     fMinPhi;
  Axis_t     fMaxPhi;

  Int_t      fMinChargeBin;
  Int_t      fMaxChargeBin;
  Int_t      fMinPtotBin;
  Int_t      fMaxPtotBin;
  Int_t      fMinPtBin;
  Int_t      fMaxPtBin;
  Int_t      fMinPhiBin;
  Int_t      fMaxPhiBin;
  Int_t      fTPC;

  T49Index  *DummyIndex;

  // Added in version 2:

  TObjArray  *fNpHistList;   //List of n_point histograms
  Bool_t     fNPointFlag;    //Flags the storage of number of points per track

  // Version 5:
  Bool_t     fYPtBinning;    //Flag to toggle binning in y-pt or log(p)-pt
  Float_t    fRapMass;       //Mass used for rapidity calculation

  // Version 8
  UInt_t     fNEvent;
  
  // Version 9
  Float_t fPowSigScale;
};

#endif







