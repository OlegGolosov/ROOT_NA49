#include <iostream>
using namespace std;
//#include <unistd.h>   // for sleep()
//#include <stdlib.h>   // for atoi()
//#include <math.h>

#include "T49CrSdynFit.h"

//*-*-* ROOT includes

#include "TROOT.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TMinuit.h"
#include "TRandom.h"
#include "math.h"
#include "Rtypes.h"

#include "PhysConst.h"

#define MAXNUM 150000

void     CalcChi2(Int_t &npar, Double_t *gin, Double_t &f, Double_t *u, Int_t flag);

T49CrSdynFit  *CrSdynFit;

Double_t ValList[MAXNUM];
Int_t Verbose;
TH1F  *gHdiv;
TH1F  *gHdata;
TH1F  *gHtest;
TH1F  *gHmix;

TMinuit *sdynMinuit = new TMinuit(1);

////////////////////////////////////////////////////////////////////

ClassImp(T49CrSdynFit)

T49CrSdynFit::T49CrSdynFit()
{

  fVerbose = 0;
  fStrategy = 1;
}

///////////////////////// init //////////////////////

void T49CrSdynFit::InitSdyn(TH1F *Hdata,TH1F *Hmix) 
{
    cerr << "T49CrSdynFit::FitSdyn() has been called" << endl;
    gRandom->SetSeed(0);

    fHdata = Hdata;
    fHmix  = Hmix;
    
    gHdata = Hdata;
    gHmix  = Hmix;
    Verbose = fVerbose; 

}

void T49CrSdynFit::SdynFit(Double_t *Para, Double_t *Down, Double_t *Up, Double_t *Step)
{

  //  Int_t     i;
  Int_t     ErrFlag;
  Double_t we, al, bl;
  Double_t arglist[10];
  Double_t par;
  
  ////// Check if Minuit is initialized /////////////////////////////
  
  //  printf("Init Minuit\n");
  sdynMinuit->mninit(5,6,7);
  //printf("Call SetFCN\n");
  sdynMinuit->SetFCN(CalcChi2);
  
  //  printf("Start dedx fit\n");
  sdynMinuit->mncler();
  //                       start/step /lo limit/hi limit
  //cut the noise
  arglist[0] = -1;
  
  sdynMinuit->mnexcm("SET PRI",arglist,1,ErrFlag);

  //  printf("Init Parameters\n");
  sdynMinuit->mnparm(0,"sdyn",Para[0],Para[0]*Step[0],Para[0]*Down[0],Para[0]*Up[0],ErrFlag);

  arglist[0] = 1.0;
  
  sdynMinuit->mnexcm("SET ERR",arglist,1,ErrFlag);
  
  arglist[0] = fStrategy;//set strategy low val less calls less precision 0,1(default),2 
  sdynMinuit->mnexcm("SET STR",arglist,1,ErrFlag);
  
  arglist[0] = 1;
  sdynMinuit->mnexcm("CALL",arglist,1,ErrFlag);
  
  arglist[0] = 0;
  sdynMinuit->mnexcm("MINI",arglist,0,ErrFlag);
  
  arglist[0] = 3;
  sdynMinuit->mnexcm("CALL",arglist,1,ErrFlag);
  
  arglist[0] = 6;
  sdynMinuit->mnexcm("CALL",arglist,1,ErrFlag);

  TString str = "sdyn";
  sdynMinuit->mnpout(0,str,par,we,al,bl,ErrFlag);
  Para[0] = par;
  printf("result par %f err %f al %f bl %f ErrFl %d\n",par,we,al,bl,ErrFlag);

  sdynMinuit->mnexcm("CLEAR",arglist,0,ErrFlag);
  
}

Double_t  T49CrSdynFit::ReturnChi2(Double_t *u)
{
  Int_t npar = 1;
  Double_t gin[3];
  Double_t f = 0;
  //  Double_t *u;
  Int_t flag = 1;
  CalcChi2(npar,gin,f,u,flag);
  return f;
}
void CalcChi2(Int_t &npar, Double_t *gin, Double_t &f, Double_t *u, Int_t flag)
{

  gHdiv = new TH1F(*gHdata);
  //  gHdiv->Sumw2();
  gHtest = new TH1F(*gHmix);
  gHtest->Reset();
  // gHtest->Sumw2();

  TF1 f1("f1","gaus",-1,2);  
  f1.SetParameter(0,1);
  f1.SetParameter(1,1);
  f1.SetParameter(2,u[0]);

  for(Int_t i = 0; i<MAXNUM;i++)
    {
      Double_t rval = f1.GetRandom();
      ValList[i] = (Double_t ) gHmix->GetRandom();

      if((ValList[i]*(rval))>0)
	gHtest->Fill(ValList[i]*(rval));
      else
	gHtest->Fill(0);
    }

  gHtest->Scale(1/gHtest->GetEntries());
  gHdiv->Scale(1/gHdiv->GetEntries());

  gHdiv->Add(gHtest,-1);
  //  gHdiv->Divide(gHtest);

  Int_t ndf = 0;
  Double_t chi2 = 0.0;

  for(Int_t i =2;i<gHdiv->GetNbinsX();i++)
    {
      if(gHdata->GetBinContent(i)>=100&&
	 gHtest->GetBinContent(i)!=0)
	{
	  chi2+=pow((gHdiv->GetBinContent(i))/gHdiv->GetBinError(i),2);
	  ndf++;
	}
    }
  if(Verbose>0)
    printf("sdyn: %f chi2 %f ndf %d chi2/ndf: %f\n",u[0], chi2,ndf,chi2/ndf);

  delete gHtest;
  delete gHdiv;

  f = chi2/ndf; 

}
