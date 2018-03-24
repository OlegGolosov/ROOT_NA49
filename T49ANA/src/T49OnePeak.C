#include <iostream>
#include <cmath>
using namespace std;

#include "T49OnePeak.h"
#include "TMath.h"
#include "TStyle.h"
#include "TROOT.h"

//____________________________________________________________________
//  dE/dx function class
//  
//  Class inherits from TF1 and overloads Eval and EvalPar member
//  function, in order to provide all TF1 functionality. The function
//  provides the shape of a single dE/dx peak. Specify a number of
//  points histogram before use. This function calculates the sum of
//  Gaussians for each evaluation, and is therefore slow, compared to
//  T49SumGaus, which is the recommended class to use for fits. Also
//  note that T49SumGaus::DrawPeaks() can be used to draw four dE/dx
//  peaks at once.

ClassImp(T49OnePeak)

T49OnePeak::T49OnePeak(Char_t* name): TF1()
{
  //TF1(name,(Double_t (*)(Double_t *, Double_t *)) 0,0.5,2.0,3)

  // Copied from TF1 constructor... 
  SetName(name);
  fNpar = 3;
  fXmin      = 0.5;
  fXmax      = 2.0;
  fNpx       = 100;
  fType      = 0;
  if (fNpar) {
    fNames      = new TString[fNpar];
    fParams     = new Double_t[fNpar];
    fParErrors = new Double_t[fNpar];
    fParMin    = new Double_t[fNpar];
    fParMax    = new Double_t[fNpar];
    for (int i = 0; i < fNpar; i++) {
      fParErrors[i]  = 0;
      fParMin[i]     = 0;
      fParMax[i]     = 0;
    }
  }
  if (gStyle) {
    SetLineColor(gStyle->GetFuncColor());
    SetLineWidth(gStyle->GetFuncWidth());
    SetLineStyle(gStyle->GetFuncStyle());
  }
  TF1 *f1old = (TF1*)gROOT->GetListOfFunctions()->FindObject(name);
  if (f1old) delete f1old;
  gROOT->GetListOfFunctions()->Add(this);

  SetParNames("Amp","Pos","Reso");
  fNpHist=0;
}

Double_t T49OnePeak::EvalPar(Double_t* x, Double_t* params )
{
  if (!fNpHist) {
    cerr << "T49OnePeak: You have to set th number-of-point histogram!!" << endl;
    return 0;
  }
  if (params==0)
    params=fParams;

   Double_t xx =x[0];
   Double_t f=0;
   Double_t norm=0;
   Float_t Bico;
   Double_t sigma;
   Double_t deltasq;
   deltasq=(xx-params[1])*(xx-params[1]);
   Int_t nbin=fNpHist->GetNbinsX();
   for (Int_t j=1;j<=nbin;j++) {
     if ((Bico=fNpHist->GetBinContent(j))) {
       sigma=params[1]*params[2]/sqrt(((Double_t) j));
       f+=Bico/sigma*exp(-0.5*deltasq/(sigma*sigma));
       norm+=Bico;
     }
   }
   if (norm)
     f*=params[0]/norm/sqrt(2*TMath::Pi());
   return f;
}

Double_t T49OnePeak::Eval(Double_t x, Double_t y, Double_t z)
{
  Double_t xx[3];
  xx[0] = x;
  xx[1] = y;
  xx[2] = z;

  return EvalPar(xx,fParams);
}
