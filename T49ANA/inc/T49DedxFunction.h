#ifndef _T49DedxFunction_h_
#define _T49DedxFunction_h_

/*
$Log: T49DedxFunction.h,v $
Revision 1.4  2008/01/04 10:48:57  cblume
64bit modifications by Marek Szuba

Revision 1.3  2000/07/07 14:14:22  cblume
Made ROOT v2.25 compatibel by Marco

 * Revision 1.2  2000/01/30  14:45:47  cblume
 * New versions from Marco with updated comments
 *
 * Revision 1.1  2000/01/17  12:37:44  cblume
 * Add T49DedxFunction.h
 *
*/

#include "TF1.h"
#include "TH1.h"

#define MASS_P_2        0.879844
#define MASS_K_2        0.24374
#define MASS_PI_2       0.019488
#define MASS_E_2        0.00000026112

class T49DedxFunction : public TF1 {
  
ClassDef(T49DedxFunction,1)  // dE/dx fit function class   

public: 
  T49DedxFunction(Char_t* name);
  ~T49DedxFunction(){ };

  void      Init(TH1F* Hist, Char_t* Opt="");
  void      InitPositions(Float_t pipos);

  void      SetBBPars(Float_t c, Float_t d, Float_t e, Float_t f)
    {fParaC=c; fParaD=d; fParaE=e; fParaF=f;}
  void      GetBBPars(Float_t &c, Float_t &d, Float_t &e, Float_t &f)
    {c=fParaC; d=fParaD; e=fParaE; f=fParaF;}

  void      SetPtot(Float_t p) {fPtot=p;}
  void      SetPosition(Int_t i, Float_t x, Float_t lowlim=0, Float_t uplim=0);
  void      SetAmplitude(Int_t i, Float_t x, Float_t lowlim, Float_t uplim);
  void      SetAmplitude(Int_t i, Float_t x);
  void      SetFixedAmplitude(Int_t i, Float_t x);
  void      SetFixedAmplitude(Int_t i);
  Bool_t    IsAmplitudeFixed(Int_t i) {return fAmpFix[i];}
  void      SetResolution(Float_t x);
  void      SetFree();
  void      SetFreePosition(Int_t i,Float_t x);
  void      SetFixedPosition(Int_t i,Float_t x);
  void      SetFreePosition(Int_t i);
  void      SetFixedPosition(Int_t i);
  void      ResetFreePosition(Int_t i);
  
  void      GetRatio(Int_t part, Float_t &val, Float_t &err);
  void      GetRatio2(Int_t part, Float_t &val, Float_t &err);
  void      StoreCovMat();

  void      StoreParameters();
  void      RestoreParameters();
 
  Float_t   GetPtot() {return fPtot;}
  Float_t   GetPosition(Int_t i);
  Float_t   GetRelPosition(Int_t i);
  Float_t   GetAmplitude(Int_t i);
  Float_t   GetResolution();

  Float_t   RelRise(Float_t Ptot, Int_t Part);
  Float_t   GetRelRise(Int_t part);
    
  Double_t  fCovMatMinuit[9][9];
  Float_t   fCovMat[9][9];
    
    protected:
  
  Float_t  fParaC;       // Constants for rel rise parametrisation
  Float_t  fParaD;
  Float_t  fParaE;
  Float_t  fParaF;
  Float_t  fPtot;
  Bool_t   fAmpFix[4];
  Double_t  fStoredPars[9];
  Double_t  fStoredLowLim[9];
  Double_t  fStoredUpLim[9];
  Bool_t   fStored;

 private:
  T49DedxFunction() {;}

 //Used by Init(TH1F* tmpHist) :

  void PolyFit(const Int_t NPoints, Float_t* x, Float_t* y, Float_t* err,
               Int_t Order, Float_t* Result);  
  Int_t Binom(Int_t n,Int_t k);


}; 

#endif
