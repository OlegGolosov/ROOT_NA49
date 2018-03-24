#include <iostream>
//#include <unistd.h>   // for sleep()
//#include <stdlib.h>   // for atoi()
//#include <math.h>
using namespace std;

#include "T49CrDedx.h"
#include "T49CrIndex.h"
#include "T49CrInfo.h"
#include "T49CrContainer.h"
#include "T49CrParticle.h"
#include "TClonesArray.h"

//*-*-* ROOT includes

#include "TROOT.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"
#include "TF1.h"
#include "TF2.h"
#include "TProfile.h"
#include "TKey.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TObjectTable.h"
#include "TClassTable.h"
#include "TMinuit.h"
#include "TRandom.h"
#include "TIterator.h"
#include "Rtypes.h"

#include "PhysConst.h"
#include "T49ParticleRoot.h"

Float_t  RelRise(Float_t p, Int_t part);
Double_t FuncDedx(Double_t *x,Double_t *par); 
void     LogLike(Int_t &npar, Double_t *gin, Double_t &f, Double_t *u, Int_t flag);
void     EbELogLike(Int_t &npar, Double_t *gin, Double_t &f, Double_t *u, Int_t flag);

#define MASS_P_2        0.879844
#define MASS_K_2        0.24374
#define MASS_PI_2       0.019488
#define MASS_E_2        0.00000026112
#define PI    3.14159265359
#define MINPOINTS       25
#define MINDEDXPOINTS   50

#define MAXPART  15000
TClonesArray  *TrackList;
Int_t    TrackNum;
Double_t CrNorm       = 1.0/sqrt(2.0*3.14159);
Float_t  ParaC;       // Constants for rel rise parametrisation
Float_t  ParaD;
Float_t  ParaE;
Float_t  ParaF;
Float_t  ParaPos[4];

T49CrDedx  *CrDedx;
T49CrIndex  *HelpIndex = new T49CrIndex();

Double_t mass[4] = {MASS_E_2, MASS_PI_2, MASS_K_2, MASS_P_2};
TMinuit *myMinuit = new TMinuit(3);
//TF1 *testgaus = new TF1("testgaus","[0]*exp(-1*((x-[1])*(x-[1]))/(2*[1]*[1]*[2]))",0.5,2.0);

TF1 *F1Fix;
TF1 *F1Free;

//Functions for Momentum Propability

TF1 *elposgaus;
TF1 *elneggaus;
TF1 *piposgaus;
TF1 *pineggaus;
TF1 *koposgaus;
TF1 *koneggaus;
TF1 *prposgaus;
TF1 *prneggaus;

TF1 *elptfit;
TF1 *piptfit;
TF1 *koptfit;
TF1 *prptfit;
////////////////////////////////////////////////////////////////////

ClassImp(T49CrDedx)

T49CrDedx::T49CrDedx()
{
  fParaC =  1.597546;// 1.5624;
  fParaD =  9.8;
  fParaE =  2.38;
  fParaF =  0.2;
  fDedxScale = 1.15;

  ParaC =  1.597546;// 1.5624;
  ParaD =  9.8;
  ParaE =  2.38;
  ParaF =  0.2;
   
}

///////////////////////// init //////////////////////

Int_t T49CrDedx::Init() 
{
    cerr << "T49CrDedx::init() has been called" << endl;

    /*    F2Dedx   = new TF2("F2Dedx",FuncDedx,4);
	  F2Dedx->SetName("F2Dedx");
	  F2Dedx->SetParameters(0.1,0.1,0.1,100);
	  */
    cerr << "T49CrDedx::init() has finished" << endl;
    CrDedx = new T49CrDedx();

    return kTRUE;
}

void T49CrDedx::InitMomProb()
{
  //   printf("initializing Momentum Probability calculation...\n");
  Char_t  text[1000];
  
  sprintf(text,"[0]*(x*x)*exp(-1*(x/[1]))");
  
  if(elptfit==0)
    elptfit = new TF1("elptfit",text,0,2);
  if(piptfit==0)
    piptfit = new TF1("piptfit",text,0,2);
  if(koptfit==0)
    koptfit = new TF1("koptfit",text,0,2);
  if(prptfit==0)
    prptfit = new TF1("prptfit",text,0,2);

  elptfit->SetRange(0.0,2.0);
  piptfit->SetRange(0.0,2.0);
  koptfit->SetRange(0.0,2.0);
  prptfit->SetRange(0.0,2.0);
  
  elptfit->SetParameters(1829.534,0.065);	
  piptfit->SetParameters(164.1196,0.145);
  koptfit->SetParameters(67.61346,0.195);
  prptfit->SetParameters(16.13318,0.32);
  
  sprintf(text,"[0]*exp(-1*((x-[1])*(x-[1]))/(2*[1]*[1]*[2]))");

  if(elposgaus==0)
    elposgaus = new TF1("elposgaus",text,0.0,6.0);
  elposgaus->SetParameters(0.658555,2.99,4.077516445965e-02);

  if(elneggaus==0)
    elneggaus = new TF1("elneggaus",text,0.0,6.0);
  elneggaus->SetParameters(0.443446,2.99,9.012279267626e-02);
  if(piposgaus==0)
    piposgaus = new TF1("piposgaus",text,0.0,6.0);
  piposgaus->SetParameters(0.337819,2.99,1.592285411437e-01);

  if(pineggaus==0)
    pineggaus = new TF1("pineggaus",text,0.0,6.0);
  pineggaus->SetParameters(0.293537,2.99,2.207705711859e-01);
  if(koposgaus==0)
    koposgaus = new TF1("koposgaus",text,0.0,6.0);
  koposgaus->SetParameters(0.268136,2.99,2.814590646789e-01);
  if(koneggaus==0)
    koneggaus = new TF1("koneggaus",text,0.0,6.0);
  koneggaus->SetParameters(0.316433,2.99,1.857487175660e-01);
  if(prposgaus==0)
    prposgaus = new TF1("prposgaus","[5]*([0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x)",0.0,6.0);

  prposgaus->SetParameter(0,-9.193436303095e+01);
  prposgaus->SetParameter(1,1.110240575372e+02);
  prposgaus->SetParameter(2,-4.884691167209e+01);
  prposgaus->SetParameter(3,9.403171728158e+00);
  prposgaus->SetParameter(4,-6.676765192039e-01);
  prposgaus->SetParameter(5,3.353870000000e-01);
  if(prneggaus==0)
    prneggaus = new TF1("prneggaus",text,0.0,6.0);
  prneggaus->SetParameters(0,0.648936,2.99,4.199302564821e-02);
  //  printf("....done!\n");
}


void T49CrDedx::SetPtot(Float_t x)
{
  fPtot      = (Double_t) x;
  ParaPos[0] = (Double_t)GetRelRise(x,0);
  ParaPos[1] = (Double_t)GetRelRise(x,1);
  ParaPos[2] = (Double_t)GetRelRise(x,2);
  ParaPos[3] = (Double_t)GetRelRise(x,3);

}

void T49CrDedx::UnSetPtot()
{
  fPtot       = 0.0;
  ParaPos[0] = 1.0;
  ParaPos[1] = 1.0;
  ParaPos[2] = 1.0;
  ParaPos[3] = 1.0;
  
  
}

void T49CrDedx::InitParam(Float_t *ParaList) 
{
  fParaC =  ParaList[0];
  fParaD =  ParaList[1];
  fParaE =  ParaList[2];
  fParaF =  ParaList[3];

  ParaC =  ParaList[0];
  ParaD =  ParaList[1];
  ParaE =  ParaList[2];
  ParaF =  ParaList[3];
}
void T49CrDedx::FillTrackList(Float_t *TNewList, Float_t *PTotList, Float_t *NpList, Int_t NTrax)
{
  TrackList = fTrackList; 
}

void T49CrDedx::InitFixFunc()
{
  Char_t  text[1000];
  Float_t dedx[4];
  Float_t pos[4];
  
  if(fFixFunc != NULL)
    delete fFixFunc;

  dedx[0] = RelRise(fPtot,0);
  dedx[1] = RelRise(fPtot,1);
  dedx[2] = RelRise(fPtot,2);
  dedx[3] = RelRise(fPtot,3);
  dedx[0] = RelRise(fPtot,0);

  pos[0] = dedx[0]/dedx[1];
  pos[1] = dedx[1]/dedx[1];  
  pos[2] = dedx[2]/dedx[1];
  pos[3] = dedx[3]/dedx[1];

  fFixPos[0] = pos[0];
  fFixPos[1] = pos[1];
  fFixPos[2] = pos[2];
  fFixPos[3] = pos[3];
  /*
    printf("electrons @ %f\n",pos[0]);
    printf("pions     @ %f\n",pos[1]);
    printf("kaons     @ %f\n",pos[2]);
    printf("protons   @ %f\n",pos[3]);
    */
  
  sprintf(text,"[0]*exp(-1*((x-[1])*(x-[1]))/(2*[1]*[1]*[2]))+[3]*exp(-1*((x-([1]*%f))*(x-([1]*%f)))/(2*([1]*%f)*([1]*%f)*[2]))+[4]*exp(-1*((x-([1]*%f))*(x-([1]*%f)))/(2*([1]*%f)*([1]*%f)*[2]))+[5]*exp(-1*((x-([1]*%f))*(x-([1]*%f)))/(2*([1]*%f)*([1]*%f)*[2]))",pos[2],pos[2],pos[2],pos[2],pos[3],pos[3],pos[3],pos[3],pos[0],pos[0],pos[0],pos[0]);
  //  printf("%s\n",text);
  fFixFunc = new TF1("FixFunc",text,0,5);
  fFixFunc->SetParLimits(0,1.0,10000000);
  fFixFunc->SetParLimits(1,0.5,1.7);
  fFixFunc->SetParLimits(2,0.001,0.0064);
  fFixFunc->SetParLimits(3,0.0,10000000);
  fFixFunc->SetParLimits(4,0.0,10000000);
  fFixFunc->SetParLimits(5,0.0,10000000);
  fFixFunc->SetParameters(100,dedx[1],0.0016,5,5,5);
  fFixFunc->SetRange(0.5,2.0);
  // return fFixFunc;
}

void T49CrDedx::FitFix(TH1F *Hist,Char_t *Options)
{
  Float_t PiShift;
  Float_t Chisq;
  Float_t ChisqRoot;
  Float_t NDFRoot;
  Float_t NDFCR = Hist->GetNbinsX();
  Float_t NDFMy = 0;
  for(Int_t i = 1; i < Hist->GetNbinsX() + 1; i++)
    {
      if(Hist->GetBinContent(i) > 1e-6 ) NDFMy ++; 
    }


  Hist->Fit("FixFunc",Options);

  PiShift = fFixFunc   -> GetParameter(1)/RelRise(fPtot,1);
  Chisq   = fFixFunc   -> GetChisquare()/Hist->GetNbinsX();
  ChisqRoot = fFixFunc -> GetChisquare();
  NDFRoot   = fFixFunc -> GetNDF(); 

  SetFixPos(PiShift*RelRise(fPtot,0),0);
  SetFixPos(fFixFunc->GetParameter(1),1);
  SetFixPos(PiShift*RelRise(fPtot,2),2);
  SetFixPos(PiShift*RelRise(fPtot,3),3);

  SetFixAmp(fFixFunc->GetParameter(5),0);
  SetFixAmp(fFixFunc->GetParameter(0),1);
  SetFixAmp(fFixFunc->GetParameter(3),2);
  SetFixAmp(fFixFunc->GetParameter(4),3);

  SetFixRes(sqrt(fFixFunc->GetParameter(2)));

  SetFixChisq(Chisq);

  SetFixChisqRoot(ChisqRoot);
  SetFixNDFRoot(NDFRoot);
  SetFixNDFCR(NDFCR);
  SetFixNDFMY(NDFMy);
}

void T49CrDedx::FitFix(TH1F *Hist)
{
  Float_t PiShift;
  Float_t Chisq;
  Float_t ChisqRoot;
  Float_t NDFRoot;
  Float_t NDFCR = Hist->GetNbinsX();
  Float_t NDFMy = 0;
  for(Int_t i = 1; i < Hist->GetNbinsX() + 1; i++)
    {
      if(Hist->GetBinContent(i) > 1e-6 ) NDFMy ++; 
    }


  Hist->Fit("FixFunc","BR0Q");

  PiShift = fFixFunc->GetParameter(1)/RelRise(fPtot,1);
  Chisq = fFixFunc->GetChisquare()/Hist->GetNbinsX();
  ChisqRoot = fFixFunc -> GetChisquare();
  NDFRoot   = fFixFunc -> GetNDF(); 


  SetFixPos(PiShift*RelRise(fPtot,0),0);
  SetFixPos(fFixFunc->GetParameter(1),1);
  SetFixPos(PiShift*RelRise(fPtot,2),2);
  SetFixPos(PiShift*RelRise(fPtot,3),3);

  SetFixAmp(fFixFunc->GetParameter(5),0);
  SetFixAmp(fFixFunc->GetParameter(0),1);
  SetFixAmp(fFixFunc->GetParameter(3),2);
  SetFixAmp(fFixFunc->GetParameter(4),3);

  SetFixRes(sqrt(fFixFunc->GetParameter(2)));

  SetFixChisq(Chisq);
  SetFixChisqRoot(ChisqRoot);
  SetFixNDFRoot(NDFRoot);
  SetFixNDFCR(NDFCR);
  SetFixNDFMY(NDFMy);
}

void T49CrDedx::InitMaxLike(TH1F *Hist,Double_t *u)
{
  InitFixFunc();
  FitFix(Hist);
  u[0] = GetFixAmp(0);
  u[1] = GetFixAmp(1);
  u[2] = GetFixAmp(2);
  u[3] = GetFixAmp(3);
  u[4] = GetFixPos(0);
  u[5] = GetFixPos(1);
  u[6] = GetFixPos(2);
  u[7] = GetFixPos(3);
  u[8] = 0.34;
}

void T49CrDedx::InitMaxLike(T49CrInfo *Info,Double_t *u)
{
  //  Float_t nel,npi,nko,npr,nall;

  /*
    nel = 100*sqrt(2*PI)*Info->GetAmplitude(0)*Info->GetPosition(0)*Info->GetReso();
    npi = 100*sqrt(2*PI)*Info->GetAmplitude(1)*Info->GetPosition(1)*Info->GetReso();
    nko = 100*sqrt(2*PI)*Info->GetAmplitude(2)*Info->GetPosition(2)*Info->GetReso();
    npr = 100*sqrt(2*PI)*Info->GetAmplitude(3)*Info->GetPosition(3)*Info->GetReso();
    nall = nel+npi+nko+npr;
    u[0] = nel/nall;
    u[1] = npi/nall;
    u[2] = nko/nall;
    u[3] = npr/nall;
    */
  u[0] = Info->GetAmplitude(0);
  u[1] = Info->GetAmplitude(1);
  u[2] = Info->GetAmplitude(2);
  u[3] = Info->GetAmplitude(3);
  u[4] = Info->GetPosition(0);
  u[5] = Info->GetPosition(1);
  u[6] = Info->GetPosition(2);
  u[7] = Info->GetPosition(3);
  u[8] = Info->GetReso();//0.34;
}

void T49CrDedx::FitFree(TH1F *Hist,Char_t *Options)
{
  Float_t Chisq;
  Float_t ChisqRoot;
  Float_t NDFRoot;
  Float_t NDFCR = Hist->GetNbinsX();
  Float_t NDFMy = 0;
  for(Int_t i = 1; i < Hist->GetNbinsX() + 1; i++)
    {
      if(Hist->GetBinContent(i) > 1e-6 ) NDFMy ++; 
    }


  printf("##########################\n");
  printf("# fitting with old ROOTCR!!!\n");
  printf("##########################\n");
  Hist->Fit("FreeFunc",Options);
  
  Chisq = fFreeFunc->GetChisquare()/Hist->GetNbinsX();
  ChisqRoot = fFreeFunc -> GetChisquare();
  NDFRoot   = fFreeFunc -> GetNDF(); 


  SetFreePos(fFreeFunc->GetParameter(1)*fFreeFunc->GetParameter(8),0);
  SetFreePos(fFreeFunc->GetParameter(1),1);
  SetFreePos(fFreeFunc->GetParameter(1)*fFreeFunc->GetParameter(4),2);
  SetFreePos(fFreeFunc->GetParameter(1)*fFreeFunc->GetParameter(6),3);
  
  SetFreeAmp(fFreeFunc->GetParameter(7),0);
  SetFreeAmp(fFreeFunc->GetParameter(0),1);
  SetFreeAmp(fFreeFunc->GetParameter(3),2);
  SetFreeAmp(fFreeFunc->GetParameter(5),3);

  SetFreeRes(fFreeFunc->GetParameter(2));  
  SetFreeChisq(Chisq);
  SetFreeChisqRoot(ChisqRoot);
  SetFreeNDFRoot(NDFRoot);
  SetFreeNDFCR(NDFCR);
  SetFreeNDFMY(NDFMy);
}

void T49CrDedx::FitFree(TH1F *Hist)
{
  Float_t Chisq;
  Float_t ChisqRoot;
  Float_t NDFRoot;
  Float_t NDFCR = Hist->GetNbinsX();
  Float_t NDFMy = 0;
  for(Int_t i = 1; i < Hist->GetNbinsX() + 1; i++)
    {
      if(Hist->GetBinContent(i) > 1e-6 ) NDFMy ++; 
    }

  //  printf("##########################\n");
  //  printf("# fitting with old ROOTCR!!!\n");
  //  printf("##########################\n");
  Hist->Fit("FreeFunc","RB0Q");
  
  Chisq     = fFreeFunc->GetChisquare()/Hist->GetNbinsX();
  ChisqRoot = fFreeFunc -> GetChisquare();
  NDFRoot   = fFreeFunc -> GetNDF(); 


  SetFreePos(fFreeFunc->GetParameter(1)*fFreeFunc->GetParameter(8),0);
  SetFreePos(fFreeFunc->GetParameter(1),1);
  SetFreePos(fFreeFunc->GetParameter(1)*fFreeFunc->GetParameter(4),2);
  SetFreePos(fFreeFunc->GetParameter(1)*fFreeFunc->GetParameter(6),3);
  
  SetFreeAmp(fFreeFunc->GetParameter(7),0);
  SetFreeAmp(fFreeFunc->GetParameter(0),1);
  SetFreeAmp(fFreeFunc->GetParameter(3),2);
  SetFreeAmp(fFreeFunc->GetParameter(5),3);

  SetFreeRes(fFreeFunc->GetParameter(2));  
  SetFreeChisq(Chisq);
  SetFreeChisqRoot(ChisqRoot);
  SetFreeNDFRoot(NDFRoot);
  SetFreeNDFCR(NDFCR);
  SetFreeNDFMY(NDFMy);
}

void T49CrDedx::InitFreeFunc()
{
  Char_t  text[1000];
  Float_t dedx[4];
  Float_t RelDist[4];
  if(fFreeFunc != NULL)
    delete fFreeFunc;
  Float_t expo = fDedxScale;
  dedx[0] = RelRise(fPtot,0);
  dedx[1] = RelRise(fPtot,1);
  dedx[2] = RelRise(fPtot,2);
  dedx[3] = RelRise(fPtot,3);

  fFreePos[0] = dedx[0];
  fFreePos[1] = dedx[1];
  fFreePos[2] = dedx[2];
  fFreePos[3] = dedx[3];

  RelDist[0] = dedx[0]/dedx[1];
  RelDist[1] = 1.0;
  RelDist[2] = dedx[2]/dedx[1];
  RelDist[3] = dedx[3]/dedx[1];
  
  sprintf(text,"[0]*exp(-1*((x-[1])*(x-[1]))/(2*%f*[2]*[2]))+[3]*exp(-1*((x-[1]*[4])*(x-[1]*[4]))/(2*%f*[2]*[2]))+[5]*exp(-1*((x-[1]*[6])*(x-[1]*[6]))/(2*%f*[2]*[2]))+[7]*exp(-1*((x-[1]*[8])*(x-[1]*[8]))/(2*%f*[2]*[2]))",pow(dedx[1],2*expo),pow(dedx[2],2*expo),pow(dedx[3],2*expo),pow(dedx[0],2*expo));
  //  sprintf(text,"[0]*exp(-1*((x-[1])*(x-[1]))/(2*%f*[2]*[2]))+[3]*exp(-1*((x-[4])*(x-[4]))/(2*(%f)*[2]*[2]))+[5]*exp(-1*((x-[6])*(x-[6]))/(2*%f*[2]*[2]))+[7]*exp(-1*((x-[8])*(x-[8]))/(2*%f*[2]*[2]))",dedx[1]*dedx[1]*dedx[1],dedx[2]*dedx[2]*dedx[2],dedx[3]*dedx[3]*dedx[3],dedx[0]*dedx[0]*dedx[0]);
  
  fFreeFunc = new TF1("FreeFunc",text,0,5);

  fFreeFunc->SetParLimits(0,1.0,10000000);
  fFreeFunc->SetParLimits(1,0.5,1.6);
  fFreeFunc->SetParLimits(2,0.03,0.08);
  fFreeFunc->SetParLimits(3,0.0,10000000);
  fFreeFunc->SetParLimits(5,0.0,10000000);
  fFreeFunc->SetParLimits(7,0.0,10000000);

  if(fFixFunc==NULL)
    { 
      fFreeFunc->SetParLimits(0,1.0,10000000);
      fFreeFunc->SetParLimits(1,0.5,1.6);
      fFreeFunc->SetParLimits(2,0.03,0.08);
      fFreeFunc->SetParLimits(3,0.0,10000000);
      fFreeFunc->SetParLimits(5,0.0,10000000);
      fFreeFunc->SetParLimits(7,0.0,10000000);
      fFreeFunc->SetParameters(100,dedx[1],0.0016,5,RelDist[2],5,RelDist[3],5,RelDist[0]);
    }
  else
    { 
      RelDist[0] = GetFixPos(0)/GetFixPos(1);
      RelDist[1] = 1.0;
      RelDist[2] = GetFixPos(2)/GetFixPos(1);
      RelDist[3] = GetFixPos(3)/GetFixPos(1);
      fFreeFunc->SetParLimits(0,GetFixAmp(1)*0.5,GetFixAmp(1)*1.5);
      fFreeFunc->SetParLimits(1,GetFixPos(1)*0.95,GetFixPos(1)*1.05);
      fFreeFunc->SetParLimits(2,GetFixRes()*0.5,GetFixRes()*1.5);
      fFreeFunc->SetParLimits(3,GetFixAmp(2)*0.01,GetFixAmp(2)*10);
      fFreeFunc->SetParLimits(4,RelDist[2]*0.96,RelDist[2]*1.04);
      fFreeFunc->SetParLimits(5,GetFixAmp(3)*0.01,GetFixAmp(3)*10);
      fFreeFunc->SetParLimits(6,RelDist[3]*0.9,RelDist[3]*1.1);
      fFreeFunc->SetParLimits(7,GetFixAmp(0)*0.1,GetFixAmp(0)*1.9);
      fFreeFunc->SetParLimits(8,RelDist[0]*0.95,RelDist[0]*1.2);
      fFreeFunc->SetParameters(GetFixAmp(1),
			       GetFixPos(1),
			       GetFixRes(),
			       GetFixAmp(2),
			       RelDist[2],
			       GetFixAmp(3),
			       RelDist[3],
			       GetFixAmp(0),
			       RelDist[0]);
    }
  fFreeFunc->SetRange(0.5,1.8);
}

Float_t T49CrDedx::GetRelRise(Float_t Ptot, Int_t Part)
{
  return RelRise(Ptot,Part);
}

Double_t T49CrDedx::CalcProb(Double_t *u)
{
   Int_t     i;
  Int_t     npar;
  Int_t     Flag = 0;
  Double_t *gin = 0;
  Double_t  f;
  TrackList = fTrackList;
  TrackNum  = fTrackNum;
  LogLike(npar,gin,f,u,Flag);  
  for(i=0;i<9;i++)
    printf("par%d = %f\n",i,u[i]);
  printf("prob = %f\n",f);
  return f;
}

void T49CrDedx::GetProb(Double_t *Prob, Double_t *u, T49CrParticle *Particle)
{
  Int_t i;
  Int_t j;
  Float_t  Ptot;
  Float_t  TruncMean;
  Double_t Resolution;
  Double_t Amp[4];
  Double_t Mean[4];
  Double_t ParaMean[4];
  Double_t Sigma[4];
  Double_t DedxProb[4];
  Double_t AmpAll;
  
  TruncMean = (Float_t)Particle->GetTmean();
  Ptot = (Float_t)Particle->GetPtot();
  Resolution = u[8];///sqrt(Particle->GetNPoint());
  //  npi = Info->GetAmplitude(1)/ sqrt(2*PI)*Info->GetPosition(1)*Info->GetReso();
  Amp[0] = u[0]*u[4]*Resolution;
  Amp[1] = u[1]*u[5]*Resolution;
  Amp[2] = u[2]*u[6]*Resolution;
  Amp[3] = u[3]*u[7]*Resolution;
  
  AmpAll = (Amp[0]+Amp[1]+Amp[2]+Amp[3]);
  
  Amp[0] /= AmpAll;
  Amp[1] /= AmpAll;
  Amp[2] /= AmpAll;
  Amp[3] /= AmpAll;
  
  
  for(i = 0;i<4;i++)
    {
      ParaMean[i] = 1.0;//GetRelRise(Ptot,i)/ParaPos[i];
    }
  Mean[0] = u[4]*ParaMean[0];
  Mean[1] = u[5]*ParaMean[1];
  Mean[2] = u[6]*ParaMean[2];
  Mean[3] = u[7]*ParaMean[3];
  
  for(j=0;j<4;j++)
    {
      Sigma[j] = Mean[j] * Resolution;
      DedxProb[j] = CrNorm/Sigma[j] * exp(-1*(((Double_t) TruncMean-Mean[j])*((Double_t) TruncMean-Mean[j]))/(2.0*Sigma[j]*Sigma[j]));
      Prob[j] = (DedxProb[j] * Amp[j]);
    }
}

void T49CrDedx::FillTestHist(TObjArray *TestHistos, Double_t *u)
{
  Int_t i;
  Int_t j;
  Int_t part;

  Float_t binwidth,val;
  Float_t nall,nel,npi, nko, npr;
  //Float_t pipos,kopos,prpos,elpos;
  Float_t pos;
  Double_t Reso;
  Double_t Amp[4];
  Double_t Mean[4];
  Double_t ParaMean[4];
  Double_t AmpAll;
  Double_t Ptot;
  Char_t  text[1048];
  Int_t   nbins;
  TAxis   *axis;

  T49CrParticle *Particle;

  TH1F *testhist;
  TH1F *testel;
  TH1F *testpi;
  TH1F *testko;
  TH1F *testpr;

  testhist =(TH1F *)TestHistos->At(0);
  testel   =(TH1F *)TestHistos->At(1);
  testpi   =(TH1F *)TestHistos->At(2);
  testko   =(TH1F *)TestHistos->At(3);
  testpr   =(TH1F *)TestHistos->At(4);

  nel = u[0];
  npi = u[1];
  nko = u[2];
  npr = u[3];
  nall = nel+npi+nko+npr;
  
  Mean[0] = u[4];
  Mean[1] = u[5];
  Mean[2] = u[6];
  Mean[3] = u[7];

  sprintf(text,"[0]*1.0/sqrt(2.0*3.14159)*(1/(%f*[1]))*exp(-1*((x-%f)*(x-%f))/(2*%f*[1]*[1]))",Mean[0],Mean[0],Mean[0],Mean[0]*Mean[0]);
  TF1 *elprob = new TF1("elprob",text,0.8,2.0);
  
  //  sprintf(text,"[0]*1.0/sqrt(2.0*3.14159)*(1/(%f*[1]))*exp(-1*((x-%f)*(x-%f))/(2*%f*[1]*[1]))",Mean[1],Mean[1],Mean[1],Mean[1]*Mean[1]);
  sprintf(text,"[0]*1.0/sqrt(2.0*3.14159)*(1/(%f*[1]))*exp(-1*((x-%f)*(x-%f))/(2*%f*[1]*[1]))",Mean[1],Mean[1],Mean[1],Mean[1]*Mean[1]);
  TF1 *piprob = new TF1("piprob",text,0.8,2.0);
  
  sprintf(text,"[0]*1.0/sqrt(2.0*3.14159)*(1/(%f*[1]))*exp(-1*((x-%f)*(x-%f))/(2*%f*[1]*[1]))",Mean[2],Mean[2],Mean[2],Mean[2]*Mean[2]);
  TF1 *koprob = new TF1("koprob",text,0.8,2.0);
  
  sprintf(text,"[0]*1.0/sqrt(2.0*3.14159)*(1/(%f*[1]))*exp(-1*((x-%f)*(x-%f))/(2*%f*[1]*[1]))",Mean[3],Mean[3],Mean[3],Mean[3]*Mean[3]);
  TF1 *prprob = new TF1("prprob",text,0.8,2.0);

  axis = testhist->GetXaxis();
  Float_t testsum = 0;
  TIter NextTrack(fTrackList);
  part = 0;
  while(part++<fTrackNum)
    {
      Particle = (T49CrParticle *) NextTrack();
      Reso = u[8];///sqrt(Particle->GetNPoint());
      Ptot = Particle->GetPtot();
 
      for(i = 0;i<4;i++)
	{
	  ParaMean[i] = 1.0;//GetRelRise(Ptot,i)/ParaPos[i]; 
	}
      Mean[0] = u[4]*ParaMean[0];
      Mean[1] = u[5]*ParaMean[1];
      Mean[2] = u[6]*ParaMean[2];
      Mean[3] = u[7]*ParaMean[3];
      // GetProb(Amp,u,Particle);
      
      Amp[0]  = u[0] * Mean[0] * Reso;
      Amp[1]  = u[1] * Mean[1] * Reso;
      Amp[2]  = u[2] * Mean[2] * Reso;
      Amp[3]  = u[3] * Mean[3] * Reso;

      AmpAll = (Amp[0]+Amp[1]+Amp[2]+Amp[3]);

      Amp[0] /= AmpAll;
      Amp[1] /= AmpAll;
      Amp[2] /= AmpAll;
      Amp[3] /= AmpAll;

      elprob->SetParameters(Amp[0],Reso);
      piprob->SetParameters(Amp[1],Reso);
      koprob->SetParameters(Amp[2],Reso);
      prprob->SetParameters(Amp[3],Reso);
  
      if(!(i%100))printf("#%d\n",i);
      testsum = 0;
      nbins = testhist->GetNbinsX();
      for(j=1;j<nbins;j++)
	{
	  pos = axis->GetBinCenter(j);
	  binwidth = axis->GetBinWidth(j);
	  val = elprob->Eval(pos)*binwidth;
	  testhist->Fill(pos,val);
	  testel->Fill(pos,val);
	  //	  testsum +=val;
	  val = piprob->Eval(pos)*binwidth;
	  testhist->Fill(pos,val);
	  testpi->Fill(pos,val);
	  //testsum +=val;

	  val = koprob->Eval(pos)*binwidth;
	  testhist->Fill(pos,val);
	  testko->Fill(pos,val);
	  //testsum +=val;

	  val = prprob->Eval(pos)*binwidth;
	  testhist->Fill(pos,val); 
	  testpr->Fill(pos,val); 
	  //testsum +=val;	    
	}
    }
}

void T49CrDedx::MaxLikeFit(Double_t *Para, Double_t *Down, Double_t *Up, Double_t *Step)
{
}

Double_t T49CrDedx::Integrate(TF1 *Func, Double_t Xmin, Double_t Xmax, Int_t Bins)
{
   Double_t Xstep;
  Double_t x = 0;
  Double_t y = 0;
  
  Xstep = (Xmax-Xmin)/Bins;
  
  y  = Xstep/2 * (Func->Eval(Xmin)+Func->Eval(Xmin+Xstep/2))/2;
  x  = Xmin + Xstep * 1.5;
  while(x<Xmax)
    {
      y += Xstep * (Func->Eval(x-Xstep)+Func->Eval(x))/2;
      //        printf("x,y,f: %g %g %g \n",x,y,0.25*x*x);
      x += Xstep;
    }
  
  return y;
}


void T49CrDedx::EbEFit(Double_t *Para, Double_t *Down, Double_t *Up, Double_t *Step)
{
  //  Int_t     i;
  Int_t     ErrFlag;
  Double_t we, al, bl;
  Double_t arglist[10];
  Double_t par;
  
  //  printf("Number of particles is %d\n",TrackNum);
  
  
  ////// Check if Minuit is initialized /////////////////////////////
  
  //  printf("Init Minuit\n");
  myMinuit->mninit(5,6,7);
  //printf("Call SetFCN\n");
  myMinuit->SetFCN(EbELogLike);
  
  //  printf("Start dedx fit\n");
  myMinuit->mncler();
  //give parameters names and boundaries
  /*Parameters to fit:
    0,1,2: 3 Amplitudes relative to pi to get normalizetion right
    3: piposition 
    4,5,6 scaling factors to positions relative to pipos given 
    by parametrisation
    */
  //                       start/step /lo limit/hi limit
  //cut the noise
  arglist[0] = -1;
  
  myMinuit->mnexcm("SET PRI",arglist,1,ErrFlag);

  //  printf("Init Parameters\n");
  myMinuit->mnparm(0,"k2pi",Para[0],Para[0]*Step[0],Para[0]*Down[0],Para[0]*Up[0],ErrFlag);
  myMinuit->mnparm(1,"pr2pi",Para[1],Para[1]*Step[1],Para[1]*Down[1],Para[1]*Up[1],ErrFlag);
  myMinuit->mnparm(2,"e2pi",Para[2],Para[2]*Step[2],Para[2]*Down[2],Para[2]*Up[2],ErrFlag);
  //  myMinuit->mnparm(2,"pr2pi",Para[2],Para[2]*Step[2],Para[2]*Down[2],Para[2]*Up[2],ErrFlag);
  /*    myMinuit->mnparm(2,"npr",Para[2],Para[2]*Step[2],Para[2]*Down[2],Para[2]*Up[2],ErrFlag);
  myMinuit->mnparm(1,"pr2pi",Para[1],Para[1]*Step[1],Para[1]*Down[1],Para[1]*Up[1],ErrFlag);
    myMinuit->mnparm(2,"npr",Para[2],Para[2]*Step[2],Para[2]*Down[2],Para[2]*Up[2],ErrFlag);
  */
  arglist[0] = 0.5;
  
  myMinuit->mnexcm("SET ERR",arglist,1,ErrFlag);
  
  arglist[0] = 2;
  myMinuit->mnexcm("SET STR",arglist,1,ErrFlag);
  
  arglist[0] = 1;
  myMinuit->mnexcm("CALL",arglist,1,ErrFlag);
  
  arglist[0] = 0;
  myMinuit->mnexcm("MINI",arglist,0,ErrFlag);
  
  arglist[0] = 3;
  myMinuit->mnexcm("CALL",arglist,1,ErrFlag);
  
  arglist[0] = 6;
  myMinuit->mnexcm("CALL",arglist,1,ErrFlag);

  TString str = "npi";
  myMinuit->mnpout(0,str,par,we,al,bl,ErrFlag);
  Para[0] = par;
  str = "k2pi";
  myMinuit->mnpout(1,str,par,we,al,bl,ErrFlag);
  Para[1] = par;
  str = "e2pi";
  myMinuit->mnpout(2,str,par,we,al,bl,ErrFlag);
  Para[2] = par;
  /*  myMinuit->mnpout(2,"pr2pi",par,we,al,bl,ErrFlag);
      Para[2] = par;
  */
  /*    myMinuit->mnpout(2,"npr",par,we,al,bl,ErrFlag);
      Para[2] = par;
      */
  
  myMinuit->mnexcm("CLEAR",arglist,0,ErrFlag);
  
  //  printf("Finished event by event fit:\n");
  //for(i=0;i<3;i++)
    //    printf("par%d = %f\n",i,Para[i]);
  
}

void  T49CrDedx::SetEbEProb(T49CrInfo *Info, T49CrParticle *Particle)
{
  Int_t i;
  Int_t j;
  Float_t  Charge,Ptot;
  Float_t  TruncMean;
  Double_t Resolution;
  Double_t Mean[4];
  Double_t ParaMean[4];
  Double_t Sigma[4];
  Double_t DedxProb[4];
  Double_t MomProb[4];
  Float_t amp,pos;

  /* 20 GeV global m30 */
  if(fNormalization[0] == 0)
    {
      fNormalization[0] = 1;
      fNormalization[1] = 10;
      fNormalization[2] = 1;
      fNormalization[3] = 1;
    }
  Charge = Particle->GetCharge();
  TruncMean = (Float_t)Particle->GetTmean();
  Resolution = Info->GetReso();
  Ptot = Particle->GetPtot();
  for(i = 0;i<4;i++)
    { 
      ParaMean[i] = GetRelRise(Ptot,i)/ParaPos[i];
      Mean[i]    = Info->GetPosition(i)*ParaMean[i];
      amp = Info->GetAmplitude(i);
      pos = Info->GetPosition(i);
      MomProb[i] = 600*amp/fNormalization[i];
      //      GetMomProb(Particle,MomProb);
    }
  for(j=0;j<4;j++)
    {
      //Danger Beware dEdx Scaling
      /*      Sigma[j] = Mean[j] * Resolution;
	      DedxProb[j] = CrNorm/Sigma[j] * exp(-1*(((Double_t) TruncMean-Mean[j])*((Double_t) TruncMean-Mean[j]))/(2.0*Sigma[j]*Sigma[j]));
      */     
      Sigma[j] = pow((Double_t) Mean[j],(Double_t) 2*fDedxScale) * Resolution * Resolution;
      DedxProb[j] = CrNorm/Sigma[j] * exp(-1*(((Double_t) TruncMean-Mean[j])*((Double_t) TruncMean-Mean[j]))/(2.0*Sigma[j]));
      DedxProb[j] *= MomProb[j];
      Particle->SetProb(DedxProb[j],j);
    }
}

Double_t  T49CrDedx::GetMomProb(T49CrParticle *Particle,Double_t *MomProb)
{
  Float_t rap[4],Pz,Pt;
  Float_t E,Ptot;
  Float_t Charge;

  Double_t PtProb[4];
  Double_t RapProb[4];
  Double_t AllProb;
  Pt = Particle->GetPt();
  Ptot = Particle->GetPtot();
  Charge = Particle->GetCharge();

  Pz = sqrt((Ptot*Ptot)-(Pt*Pt));
  for(Int_t Part = 0;Part<4;Part++)
    {
      E = sqrt(mass[Part] + Ptot*Ptot);
      rap[Part] = 0.5 * log((E+Pz)/(E-Pz));
    }
  
  if(elptfit!=NULL)
    {
      PtProb[0] = elptfit->Eval(Pt);
      PtProb[1] = piptfit->Eval(Pt);
      PtProb[2] = koptfit->Eval(Pt);
      PtProb[3] = prptfit->Eval(Pt);

      if(Charge<0)
	{ 
	  RapProb[0] = elneggaus->Eval(rap[0]);
	  RapProb[1] = pineggaus->Eval(rap[1]);
	  RapProb[2] = koneggaus->Eval(rap[2]);
	  RapProb[3] = prneggaus->Eval(rap[3]);
	}
      else
	{
	  RapProb[0] = elposgaus->Eval(rap[0]);
	  RapProb[1] = piposgaus->Eval(rap[1]);
	  RapProb[2] = koposgaus->Eval(rap[2]);
	  RapProb[3] = prposgaus->Eval(rap[3]);
	  if(RapProb[3]<0) 
	    RapProb[3] = 0.0;
	}
      AllProb = 0.0;
      for(Int_t Part = 0;Part<4;Part++)
	{
	  MomProb[Part] = RapProb[Part]*PtProb[Part];
	  AllProb += MomProb[Part];
	}
      for(Int_t Part = 0;Part<4;Part++)
	MomProb[Part] /= AllProb;
    }
  else
    {
      for(Int_t Part = 0;Part<4;Part++)
	MomProb[Part] /= 1.0;
    }
  return 1.0;
}

void T49CrDedx::InitEbEFit(T49CrContainer *Contain)
{
  Int_t i = 0;
  Int_t j;
  T49CrInfo  *Info;
  T49CrParticle *Particle;

  TrackList = fTrackList;
  TrackNum  = fTrackNum;  
  
  //  TIter NextTrack(TrackList);
  //  printf("got %d particles!\n",TrackNum);
  while(i++<TrackNum)
    {
      //      printf("handling#%d of %d\n",i,TrackNum);
      //Particle = (T49CrParticle *) NextTrack();
      Particle = (T49CrParticle *)TrackList->operator[](i);
      //printf("checking part#%d! Ptot = %f\n",i,Particle->GetPtot());
      if(Contain->IsIn(Particle))
	{
	  //	  printf("found one!\n");
	  //find bin
	  Contain->GetIndex(Particle,HelpIndex);
	  //get info
	  Info = Contain->GetInfo(HelpIndex);
	  if(Info->GetAmplitude(1)>0)
	    {
	      SetPtot(HelpIndex->GetPtot());
	      SetEbEProb(Info,Particle);
	    }
	  else
	    {
	      for(j = 0;j<4;j++)
		{
		  Particle->SetProb(0.0,j);
		}
	    } 
	  
	}
      else
	{
	  // printf("part not in!\n");
	} 
      
    }
}

Double_t T49CrDedx::GetDedxProb(T49CrContainer *Contain,T49ParticleRoot *Particle,Double_t *DedxProb)
{
  //  Int_t i = 0;
  Int_t j;
  T49CrInfo  *Info;

  if(Contain->IsIn(Particle))
    {
      Contain->GetIndex(Particle,HelpIndex);
      //get info
      Info = Contain->GetInfo(HelpIndex);
      if(Info->GetAmplitude(1)>0)
	{
	  SetPtot(HelpIndex->GetPtot());
	  CalcDedxProb(Info,Particle,DedxProb);
	  return 1;
	}
      else
	{
	  for(j = 0;j<4;j++)
	    {
	      DedxProb[j] = 0.0;
	    }
	  return 0;
	} 
      
    }
  else
    {
      for(j = 0;j<4;j++)
	{
	  DedxProb[j] = 0.0;
	}
      return 0;
      // printf("part not in!\n");
    } 
}

void  T49CrDedx::CalcDedxProb(T49CrInfo *Info, T49ParticleRoot *Particle, Double_t *DedxProb)
{
  Int_t i;
  Int_t j;
  Float_t  Ptot;
  Float_t  TruncMean;
  Double_t Resolution;
  Double_t Mean[4];
  Double_t ParaMean[4];
  Double_t Sigma[4];
  Double_t Sum = 0;
  Double_t MomProb[4];
  //  Float_t  pos,res;

  TruncMean = (Float_t) Particle->GetTmeanCharge()/1000;
  Resolution = Info->GetReso();
  Ptot = Particle->GetP();
  
  for(i = 0;i<4;i++)
    { 
      ParaMean[i] = GetRelRise(Ptot,i)/ParaPos[i];
      Mean[i]    = Info->GetPosition(i)*ParaMean[i];
      MomProb[i] =  Info->GetAmplitude(i);
      Sum += MomProb[i];
      //      printf("i:%d ParaM %f Mean %f MomProb %f\n",i,ParaMean[i],Mean[i],MomProb[i]);
    }
  for(j=0;j<4;j++)
    {
      MomProb[j]/=Sum;
      Sigma[j] = pow((Double_t) Mean[j],(Double_t) 2*fDedxScale) * Resolution * Resolution;
      DedxProb[j] = exp(-1*(((Double_t) TruncMean-Mean[j])*((Double_t) TruncMean-Mean[j]))/(2.0*Sigma[j]));
      DedxProb[j]*=MomProb[j];
    }
  Sum = DedxProb[0]+DedxProb[1]+DedxProb[2]+DedxProb[3];
  DedxProb[0]/= Sum;
  DedxProb[1]/= Sum;
  DedxProb[2]/= Sum;
  DedxProb[3]/= Sum;
}

Double_t T49CrDedx::PickMcPart(T49CrContainer *Contain,T49ParticleRoot *Particle, Int_t *Selection)
{
   Int_t j;
  Double_t DedxProb[4];

  if(GetDedxProb(Contain,Particle,DedxProb)>0)
    {
      for(j = 0;j<4;j++)
	{
	  if(gRandom->Rndm()<DedxProb[j])
	    Selection[j] = 1;
	  else
	    Selection[j] = 0;
	    
	}
      return 1;
    }
  else
    return 0;
}

Float_t RelRise(Float_t Ptot, Int_t Part)
{
  Float_t bg, dedx;
  Float_t c, d, e, f, x1,x2, p0, dfq, d1, d2, d3;
  // char *names [4] = {"elec","Pions", "Kaons", "Protons"};
  
  bg = Ptot / sqrt (mass[Part]);
  
  c =  ParaC;
  d =  ParaD;
  e =  ParaE;
  f =  ParaF;

  x1 = pow(10,(e - 1.0/3.0 * sqrt(2.0*log(10.0)/(3.0 * f))));
  x2 = pow(10,(e + 2.0/3.0 * sqrt(2.0*log(10.0)/(3.0 * f))));
  
  p0 = c/(d + 2.0*log(10.0)*e - 1.0);
  
  if (bg<x1)
    dfq = 0;
  else
    {
      dfq = -2.0 * log(bg) + 2.0 * log(10.0) * e;
      if(bg<x2)
	{
	  d1 = 2.0/3.0*sqrt(2.0*log(10.0)/(3.0 * f));
	  d2 = log(bg)/log(10.0);
	  d3 = pow(( e + d1 - d2),3);
	  dfq -= f*d3;
	}
    }
  
  dedx = p0*( (1+ bg*bg)/(bg*bg) * ( d + log(1+(bg*bg)) + dfq)-1.0 );
  
  return dedx;
}

void LogLike(Int_t &npar, Double_t *gin, Double_t &f, Double_t *u, Int_t flag)
{
  Int_t i = 0;
  Int_t j;
  Double_t SumProb;
  Double_t Prob;
  Double_t DedxProb[4];
  T49CrParticle *Particle;

  //  static TMinuit *myMinuit = NULL;

  SumProb = 0;

  TIter NextTrack(TrackList);
  while(i++<TrackNum)
    {
      Particle = (T49CrParticle *) NextTrack();
      Prob = 0; 
      //  printf("part#%d ptot = %f npoints = %d\n",i++,Particle->GetPtot(),Particle->GetNPoint());
      CrDedx->GetProb(DedxProb, u, Particle);
      //
      for(j=0;j<4;j++)
	{
	  Prob += DedxProb[j];
	  //	  printf("tm = %f prob%d = %f\n",Particle->GetTmean(),j,DedxProb[j]); 
	}
      if(Prob > 0)
	SumProb  -= log(Prob);  
    }
  f = SumProb; 
}

void EbELogLike(Int_t &npar, Double_t *gin, Double_t &f, Double_t *u, Int_t flag)
{
  Int_t i = 0;
  Int_t j;
  Double_t SumProb;
  Double_t Prob;
  T49CrParticle *Particle;
  Float_t Const[4];
  //  static TMinuit *myMinuit = NULL;
  
  SumProb = 0;

  Const[0] = u[2];
  //  Const[1] = 1;// - u[0] - u[1] - u[2];
  Const[1] = 1 - u[0] - u[1] - u[2];
  Const[2] = u[0];
  Const[3] = u[1];

  TIter NextTrack(TrackList);
  while(i++<TrackNum)
    {
      Particle = (T49CrParticle *) NextTrack();
      Prob = 0; 
      for(j=0;j<4;j++)
	{
	  Prob += Particle->GetProb(j)*Const[j];
	}
      Prob /= (Const[0]+Const[1]+Const[2]+Const[3]);
      if(Prob > 0)
	SumProb  -= log(Prob);  
    }
  //  printf("probsum = %f\n",SumProb);
  f = SumProb; 
}
