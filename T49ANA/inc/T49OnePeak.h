#ifndef _T49OnePeak_h_
#define _T49OnePeak_h_

#include "TH1.h"
#include "TF1.h"

class T49OnePeak : public TF1 {

ClassDef(T49OnePeak,1)  // dE/dx fit function class

public:

  T49OnePeak(Char_t* name);
  virtual ~T49OnePeak(){ };

  virtual Double_t EvalPar(Double_t* x, Double_t* params = 0);
  virtual Double_t Eval(Double_t x, Double_t y, Double_t z);

  void    SetNpHist(TH1F *Hist) {fNpHist=Hist;}

protected:

  TH1F*    fNpHist;

private:

  T49OnePeak() {;}
  TString *fNames;
  Double_t *fParams;
  Double_t *fParErrors;
  Double_t *fParMin;
  Double_t *fParMax;

};

#endif
