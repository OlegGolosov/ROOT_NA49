#ifndef _T49CrDedx_INCLUDED_
#define _T49CrDedx_INCLUDED_

#include "TObject.h"
#include "TFile.h"
#include "TList.h"
#include "TObjArray.h"
#include "TTree.h"
#include "TF1.h"
#include "T49ParticleRoot.h"
#include "T49CrParticle.h"
#include "T49CrContainer.h"
#include "T49CrInfo.h"

class T49CrDedx : public TObject {
  
public: 
  T49CrDedx();
  virtual ~T49CrDedx(){ };
  
  Int_t Init();
  
  void      SetPtot(Float_t x);
  void      UnSetPtot();
  void      SetDedxScale(Float_t x)       { fDedxScale = x;  }
  void      SetFixPos(Float_t x, Int_t i) { fFixPos[i] = x;  }
  void      SetFixAmp(Float_t x, Int_t i) { fFixAmp[i] = x;  }
  void      SetFreePos(Float_t x, Int_t i){ fFreePos[i] = x; } 
  void      SetFreeAmp(Float_t x, Int_t i){ fFreeAmp[i] = x; }
  void      SetFixRes(Float_t x)          { fFixRes = x;     }
  void      SetFreeRes(Float_t x)         { fFreeRes = x;    }
  void      SetFixChisq(Float_t x)        { fFixChisq = x;   }
  void      SetFreeChisq(Float_t x)       { fFreeChisq = x;  }
  void      SetTrackList(TClonesArray *oa,Int_t i)   
                           { fTrackList = oa; fTrackNum = i; }
  
  Float_t   GetPtot()           { return fPtot;       }
  Float_t   GetFixPos(Int_t i)  { return fFixPos[i];  } 
  Float_t   GetFixAmp(Int_t i)  { return fFixAmp[i];  }
  Float_t   GetFreePos(Int_t i) { return fFreePos[i]; }
  Float_t   GetFreeAmp(Int_t i) { return fFreeAmp[i]; }
  Float_t   GetFixRes()         { return fFixRes;     }  
  Float_t   GetFreeRes()        { return fFreeRes;    }  
  Float_t   GetFixChisq()       { return fFixChisq;   }
  Float_t   GetFreeChisq()      { return fFreeChisq;  }


  //men elave etdim
  Float_t free_ChisqRoot;
  Float_t free_NDFRoot;
  Float_t free_NDFCR;
  Float_t free_NDFMy;
    
  void SetFreeChisqRoot(Float_t _ChisqRoot) {free_ChisqRoot = _ChisqRoot;}
  void SetFreeNDFRoot(Float_t _NDFRoot)     {free_NDFRoot   = _NDFRoot;}
  void SetFreeNDFCR(Float_t _NDFCR)         {free_NDFCR     = _NDFCR;}
  void SetFreeNDFMY(Float_t _NDFMy)         {free_NDFMy     = _NDFMy;}
  
  Float_t GetFreeChisqRoot()   {return free_ChisqRoot;}
  Float_t GetFreeNDFRoot()     {return free_NDFRoot;  }
  Float_t GetFreeNDFCR()       {return free_NDFCR;    }
  Float_t GetFreeNDFMY()       {return free_NDFMy;    }
  
  ////

  Float_t fix_ChisqRoot;
  Float_t fix_NDFRoot;
  Float_t fix_NDFCR;
  Float_t fix_NDFMy;
    
  void SetFixChisqRoot(Float_t _ChisqRoot) {fix_ChisqRoot = _ChisqRoot;}
  void SetFixNDFRoot(Float_t _NDFRoot)     {fix_NDFRoot   = _NDFRoot;}
  void SetFixNDFCR(Float_t _NDFCR)         {fix_NDFCR     = _NDFCR;}
  void SetFixNDFMY(Float_t _NDFMy)         {fix_NDFMy     = _NDFMy;}
  
  Float_t GetFixChisqRoot()   {return fix_ChisqRoot;}
  Float_t GetFixNDFRoot()     {return fix_NDFRoot;  }
  Float_t GetFixNDFCR()       {return fix_NDFCR;    }
  Float_t GetFixNDFMY()       {return fix_NDFMy;    }
  

  //// men elave etdim bitdi
  
  TF1      *GetFixFunc()        { return fFixFunc;    }
  TF1      *GetFreeFunc()       { return fFreeFunc;   }
  Float_t  *GetFixPos()         { return fFixPos;     }
  Float_t  *GetFreePos()        { return fFreePos;    }
  
   
  Float_t   GetRelRise(Float_t p, Int_t part);
  Double_t  CalcProb(Double_t *u);
  void      GetProb(Double_t *Prob, Double_t *u, T49CrParticle *Particle);
  void      FillTestHist(TObjArray *TestHistos,Double_t *u);
  void      InitMomProb();
  void      InitMaxLike(TH1F *Hist,Double_t *u);
  void      InitMaxLike(T49CrInfo *Info,Double_t *u);
  void      MaxLikeFit(Double_t *u, Double_t *Down, Double_t *Up, Double_t *Step);
  void      FillTrackList(Float_t *TNewList,Float_t *PtotList,Float_t *NpList, Int_t NTrax);
  Double_t  Integrate(TF1 *Func, Double_t Xmin, Double_t Xmax, Int_t Bins);
  void      InitParam(Float_t *ParaList);
  void      InitFixFunc();
  void      InitFreeFunc();
  void      FitFix(TH1F *Hist,Char_t *Options);
  void      FitFree(TH1F *Hist,Char_t *Options);
  void      FitFix(TH1F *Hist);
  void      FitFree(TH1F *Hist);

  void      SetNormalization(Float_t v,Int_t i)  {fNormalization[i] = v;}
  void      SetEbEProb(T49CrInfo *Info, T49CrParticle *Particle);
  void      InitEbEFit(T49CrContainer *Contain);
  void      EbEFit(Double_t *u, Double_t *Down, Double_t *Up, Double_t *Step);

  void      CalcDedxProb(T49CrInfo *Info, T49ParticleRoot *Particle, Double_t *DedxProb);
  Double_t  GetDedxProb(T49CrContainer *Contain,T49ParticleRoot *Particle,Double_t *Prob);  
  Double_t  GetMomProb(T49CrParticle *Particle,Double_t *MomProb);  
  Double_t  PickMcPart(T49CrContainer *Contain,T49ParticleRoot *Particle,Int_t *Selection);  
  
  void      DeleteFixFunc()     { delete fFixFunc;   }
  void      DeleteFreeFunc()    { delete fFreeFunc;  }
  
  ClassDef(T49CrDedx,1)  // T49CrDedx top class    
    
    
    protected:
  
  Float_t  fParaC;       // Constants for rel rise parametrisation
  Float_t  fParaD;
  Float_t  fParaE;
  Float_t  fParaF;
  Float_t  fDedxScale;
  Float_t  fPtot;
  Float_t  fParaPos[4];
  Float_t  fFixPos[4];
  Float_t  fFreePos[4];
  Float_t  fFixRes;
  Float_t  fFreeRes;
  Float_t  fFixChisq;
  Float_t  fFreeChisq;
  Float_t  fFixAmp[4];
  Float_t  fFreeAmp[4];
  Float_t  fNormalization[4];
  Int_t    fTrackNum;
  TClonesArray *fTrackList;
  TF1      *fFixFunc;
  TF1      *fFreeFunc;
};

#endif
