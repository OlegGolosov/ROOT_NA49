#ifndef _T49CrSdynFit_INCLUDED_
#define _T49CrSdynFit_INCLUDED_

#include "TObject.h"
#include "TH1F.h"

class T49CrSdynFit : public TObject {
  
public: 
  T49CrSdynFit();
  virtual ~T49CrSdynFit(){ };
  
  void      InitSdyn(TH1F *Hdata,TH1F *Hmix);

  void      SdynFit(Double_t *u, Double_t *Down, Double_t *Up, Double_t *Step);
  void      SetStrategy(Int_t i) {fStrategy = i;}
  void      SetVerbose(Int_t i) {fVerbose = i;}
  Double_t  ReturnChi2(Double_t *u);

  ClassDef(T49CrSdynFit,1)  // T49CrSdynFit top class    
    
    
    protected:
  
  Double_t *fValList;
  Double_t fSdyn;
  Int_t    fVerbose;
  Int_t    fStrategy;
  TH1F     *fHdata;
  TH1F     *fHmix;
  TH1F     *fHtest;
};

#endif
