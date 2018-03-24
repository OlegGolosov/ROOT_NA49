#ifndef _T49CrContainer_INCLUDED_
#define _T49CrContainer_INCLUDED_

#include "TObject.h"
#include "TFile.h"
#include "TList.h"
#include "TObjArray.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1.h"
#include "TAxis.h"
#include "T49CrIndex.h"
#include "T49CrInfo.h"
#include "T49ParticleRoot.h"
#include "T49CrParticle.h"

class T49CrContainer : public TObject 
{
  
public: 
  T49CrContainer();
  virtual ~T49CrContainer(){ ;};
  
  Int_t Init(Int_t *bin,Char_t *FileName);
  
  TH1F      *GetHist(T49CrIndex *Index);
  Int_t     GetList(T49CrIndex *Index,TTree *InTree,TClonesArray *OutList);
  T49CrInfo   *GetInfo(T49CrIndex *Index);
  void      GetIndex(T49ParticleRoot *particle,T49CrIndex *Index);
  void      GetIndex(T49CrParticle *particle,T49CrIndex *Index);
  void      GetIndex(Double_t *X,T49CrIndex *Index);
	
  Double_t  FillContainer(T49ParticleRoot *Particle, Int_t IsMTPC);
  Double_t  FillContainer(T49ParticleRoot *particle);
  Bool_t  IsIn(T49ParticleRoot *particle);
  Bool_t  IsIn(T49CrParticle *particle);
  Double_t  InAcceptance(T49ParticleRoot *particle);
  Double_t  InAcceptance(T49CrParticle *particle);
  Double_t  IsIn(Double_t  *X);
  Double_t  IndexIn(T49CrIndex *Index);
  void      Index2C3Mom(T49CrIndex *Index);
  void      C3Mom2Index(T49CrIndex *Index);
  Int_t     Bin2ListNum(T49CrIndex *Index);
  Double_t  CheckSelect(T49CrIndex *Index);
  Double_t  IsInBin(T49ParticleRoot *Particle, T49CrIndex *Index);  
  Double_t  IsInBin(T49CrParticle *Particle, T49CrIndex *Index);
  Double_t  IsInBin(Double_t *X, T49CrIndex *Index);
  void      Close();
  //  Float_t   GetRap(Float_t mass,Float_t Ptot, Float_t Pt);
  TTree     *GetTree()                   { return fTree;}
  Int_t     GetNParticles()              { return fNParticles;}
  Int_t     GetNEvents()                 { return fNEvents;}
  void      SelectPtot(Int_t i)          {fSelectPtot = i;}
  void      SelectCharge(Int_t i)        {fSelectCharge = i;}
  void      SetFillList(Int_t i)         {fListOn = i;}
  void      FindBin(T49CrIndex *Index)     {C3Mom2Index(Index);}
  void      FindPhase(T49CrIndex *Index)   {Index2C3Mom(Index);}
  void      CountEvent()                 {fNEvents ++;}

  void      SetMaxPtot(Float_t v)          {fMaxPtot = v;}
  void      SetMinPtot(Float_t v)          {fMinPtot = v;}


  ClassDef(T49CrContainer,1)  // T49CrContainer top class    
    
    
    protected:

  TFile     *fRootFile;
  TAxis     *fChargeAxis;  //Use TAxis to calculate indices
  TAxis     *fPtotAxis;    //Use TAxis to calculate indices
  TAxis     *fPtAxis;      //Use TAxis to calculate indices
  TAxis     *fPhiAxis;     //Use TAxis to calculate indices

  TTree      *fTree;         //Tree containing accepted particles
  TObjArray  *fInfoList;     //List of infos on histogramm to store
  TObjArray  *fHistList;     //List of histogramms to store
  TObjArray  *fParticleList; //List of Tracklists to store
  TObjArray  *fReturnList;   //List to pass on Particle info

  Double_t     fX[6];
  T49CrParticle *fParticle;
  Int_t      fListOn;      //Switch if particles should be stored
  Int_t      fBins[4];     //number of bins in different dimensions  
  Int_t      fNHistTot;    //Total Number of histogramms in Container
  Int_t      fHistBins;    //Number of bins of histogramms in Container
  Float_t    fHistHiBound; //Histogramm Boundaries
  Float_t    fHistLoBound;
  Float_t    fMinCharge;
  Float_t    fMaxCharge;
  Float_t    fMinPtot;
  Float_t    fMaxPtot;
  Float_t    fMinPt;
  Float_t    fMaxPt;
  Float_t    fMinPhi;
  Float_t    fMaxPhi;

  Int_t      fSelectPtot;
  Int_t      fSelectCharge;
  Int_t      fMinChargeBin;
  Int_t      fMaxChargeBin;
  Int_t      fMinPtotBin;
  Int_t      fMaxPtotBin;
  Int_t      fMinPtBin;
  Int_t      fMaxPtBin;
  Int_t      fMinPhiBin;
  Int_t      fMaxPhiBin;

  Int_t      fNEvents;    //number of Events put into Container
  Int_t      fNParticles; //number of Particles put into Container

};

#endif
