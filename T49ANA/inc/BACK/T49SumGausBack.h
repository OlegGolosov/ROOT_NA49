#ifndef _T49SumGaus_h_
#define _T49SumGaus_h_

#include "TF1.h"
#include "TH1.h"
#include "TObjArray.h"

#define MASS_P_2        0.879844
#define MASS_K_2        0.24374
#define MASS_PI_2       0.019488
#define MASS_E_2        0.00000026112
#define PI    3.14159265359
#define NPOINT          801

class T49SumGaus : public TF1 {
  
ClassDef(T49SumGaus,1)  // dE/dx fit function class   

public: 
  T49SumGaus(Char_t* name);
  ~T49SumGaus();
  
  void      DrawPeaks(Int_t LineWidth=2);
  void      Init(TH1* Hist, Char_t* Opt="");
  void      InitPositions(Float_t pipos);

  virtual Double_t EvalPar(const Double_t* x, const Double_t* params = 0);
  virtual Double_t Eval(Double_t x, Double_t y=0, Double_t z=0);
  Float_t   GetSigCut(Int_t part, Float_t frac=0.9);

  void      SetBBPars(Float_t c, Float_t d, Float_t e, Float_t f)
    {fParaC=c; fParaD=d; fParaE=e; fParaF=f;}
  void      GetBBPars(Float_t &c, Float_t &d, Float_t &e, Float_t &f)
    {c=fParaC; d=fParaD; e=fParaE; f=fParaF;}

  void      SetPowSigScale(Float_t power) {fLinSigScale=0; fPowSigScale=power;}
  Float_t   GetPowSigScale()              {return fPowSigScale;}
  void      SetPtot(Float_t p) {fPtot=p;}
  void      SetPosition(Int_t i, Float_t x, Float_t lowlim=0, Float_t uplim=0);
  void      SetAmplitude(Int_t i, Float_t x, Float_t lowlim, Float_t uplim);
  void      SetAmplitude(Int_t i, Float_t x);
  void      SetFixedAmplitude(Int_t i, Float_t x);
  void      SetFixedAmplitude(Int_t i);
  Bool_t    IsAmplitudeFixed(Int_t i) {return fAmpFix[i];}
  void      SetAsymmetry(Float_t asym);
  void      SetAsymmetry(Float_t asym, Float_t lowlim, Float_t uplim);
  void      SetResolution(Float_t x);
  void      SetFree();
  void      SetFreePosition(Int_t i,Float_t x);
  void      SetFixedPosition(Int_t i,Float_t x);
  void      SetFreePosition(Int_t i);
  void      SetFixedPosition(Int_t i);
  void      ResetFreePosition(Int_t i);
  
  //  void      GetRatio(Int_t part, Float_t &val, Float_t &err);
  //  void      GetRatio2(Int_t part, Float_t &val, Float_t &err);
  void      StoreCovMat();
  Float_t   GetPosError(Int_t i);
  Float_t   GetAmpError(Int_t i);
  Float_t   GetResError();

  void      StoreParameters();
  void      RestoreParameters();
 
  void      SetNpHist(TH1 *Hist);

  Float_t   GetPtot() {return fPtot;}
  Float_t   GetPosition(Int_t i);
  Float_t   GetRelPosition(Int_t i);
  Float_t   GetAmplitude(Int_t i);
  Float_t   GetResolution();
  Float_t   GetMaximum(Int_t i);
  Float_t   GetAsymmetry();

  Float_t   RelRise(Float_t Ptot, Int_t Part);
  Float_t   GetRelRise(Int_t part);
  Float_t   GetStep() {return fXstep;}
    
  Double_t  fCovMatMinuit[9][9];
  Float_t   fCovMat[9][9];
    
    protected:
  
  Float_t  fParaC;       // Constants for rel rise parametrisation
  Float_t  fParaD;
  Float_t  fParaE;
  Float_t  fParaF;
  Float_t  fPtot;
  Bool_t   fAmpFix[4];
  Double_t fStoredPars[10];
  Double_t fStoredLowLim[10];
  Double_t fStoredUpLim[10];
  Bool_t   fStored;

  TH1*    fNpHist;
  Float_t  fXstep;
  Float_t  fYvals[NPOINT];
  TObjArray* fGraphs;   //! To store the graphs for the different peaks
  Bool_t   fLinSigScale;
  Float_t  fPowSigScale;
  Double_t  fTailPar1;
  Double_t  fTailPar2;

 private:
  T49SumGaus() {;}
  Double_t  OnePeak(Double_t x,Double_t *par);

 //Used by Init(TH1F* tmpHist) :

  void PolyFit(const Int_t NPoints, Float_t* x, Float_t* y, Float_t* err,
               Int_t Order, Float_t* Result);  
  Int_t Binom(Int_t n,Int_t k);


}; 

#endif



