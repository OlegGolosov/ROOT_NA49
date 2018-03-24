#ifndef T49DEDXFUNCTIONS_H
#define T49DEDXFUNCTIONS_H
#include "TNamed.h"
#include "fstream"
#include "iostream"
#include "TString.h"
#include "stdlib.h"
#include <iostream>
#include <sstream>
#include <cstdio>
#include <math.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include "TString.h"
#include "TObject.h"

class TFile;
class TTree;
class TH1D;
class T49Container;
class T49Index;
class T49SumGaus;

class T49dEdxFunctions/*:public TObject*/
{
 public:
  T49dEdxFunctions();
  virtual ~T49dEdxFunctions();
  Double_t getValue(Double_t x, Int_t p);
  Double_t getValueH(Double_t *xval, Double_t *par);  
  void fillHistograms2();
  void fillHistigrams1();
  Double_t prFunction(Double_t x) {return x*x;}
  void SetInputContDir(TString _inputContDir) {inputContDir = _inputContDir;}
  void SetInputFitDir(TString _inputFitDir) {inputFitDir = _inputFitDir;}
  void SetFitFileName(TString _fitFileName) {fitFileName = _fitFileName;}
  void SetContFileName(TString _contFileName) {contFileName = _contFileName;}
  void init();
  void Init();
  void SetBins(Int_t *bins);
  Bool_t isFitted(Int_t i = 0);
  //void ResetBins(Int_t *);
  Double_t GetIdentity(Int_t, Double_t, Float_t *);
 private:
  Int_t binFitted[2][20][10][8];
  Int_t binFitted2[2][20][10][8];
  T49Index *I;
  T49Container *mtpc_cont;
  T49SumGaus *sumG;
  Int_t fitEnt;
  Int_t size_size;
  TFile* fitFile;
  TTree* fitTree;
  TFile* contFile;
  TString inputContDir;
  TString inputFitDir;
  TString fitFileName;
  TString contFileName; 
  TH1D*  hElectron[2][20][10][8];
  TH1D*  hPion[2][20][10][8];
  TH1D*  hKaon[2][20][10][8];
  TH1D*  hProton[2][20][10][8];
  Double_t my_par[12];
  Int_t bin1_M, bin2_M, bin3_M, bin4_M;
  Char_t eName[255], piName[255], kName[255], prName[255];
  Double_t GetIdentity(Int_t, Double_t);
};
#endif

