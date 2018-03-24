//Author A. Rustamov
//define your finctions here
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1D.h"
#include "T49dEdxFunctions.h"
#include "T49SumGaus.h"
#include "T49Container.h"
#include "T49Index.h"

using namespace std;

T49dEdxFunctions::T49dEdxFunctions()
{
  sumG  = new T49SumGaus((Char_t*)"sumG");
  sumG  -> SetPowSigScale(0.625);  
  I     = new T49Index();
  size_size = 3000;
  
  for (Int_t i = 0; i < 2; i++)
    for(Int_t k = 0; k < 20; k++)
      for(Int_t j = 0; j < 10; j++)
	for(Int_t m = 0; m < 8; m++)
	  {
	    binFitted[i][k][j][m]   = -10;
	    binFitted2[i][k][j][m]  = -10;
	  }
}

Bool_t T49dEdxFunctions::isFitted(Int_t i)
{
  if(i == 0)
    {
      if (binFitted[bin1_M][bin2_M][bin3_M][bin4_M] > 0)
	{
	   binFitted2[bin1_M][bin2_M][bin3_M][bin4_M] = 1;
	  return kTRUE;
	}
      else
	return kFALSE; 
    }
  else
    {
      if (binFitted2[bin1_M][bin2_M][bin3_M][bin4_M] > 0)
        return kTRUE;
      else
	return kFALSE;
    }
}


void T49dEdxFunctions::init()
{
  fitFile   = new TFile(inputFitDir+fitFileName);
  contFile  = new TFile(inputContDir+contFileName);
  mtpc_cont = (T49Container *)contFile -> Get("cont");  
  fitTree   = (TTree*)fitFile -> Get("NT");
  fitTree -> SetBranchAddress("myPAR",my_par);
  fitTree -> SetBranchAddress("ChargeBin",&bin1_M);
  fitTree -> SetBranchAddress("PtotBin",&bin2_M);
  fitTree -> SetBranchAddress("PtBin",&bin3_M);
  fitTree -> SetBranchAddress("PhiBin",&bin4_M);
  fitEnt  = (Int_t)fitTree -> GetEntries();
}

T49dEdxFunctions::~T49dEdxFunctions()
{
  ;
}

Double_t T49dEdxFunctions::getValue(Double_t x, Int_t p)
{
  Double_t par[4];
  if (p==1) 
    {
      par[1] = my_par[2];
      par[2] = my_par[0];
    }
  else 
    {
      par[1] = my_par[2]*my_par[p+1];
      par[2]=my_par[0]*pow(fabs(my_par[p+1]),0.625);
    }
  par[0] = my_par[p+5+1];
  par[3] = my_par[11];
  return sumG -> OnePeak(x,par);
}

Double_t T49dEdxFunctions::getValueH(Double_t *xval, Double_t *par)
{
  
  Double_t xx = xval[0];
  Int_t    bNum = hElectron[bin1_M][bin2_M][bin3_M][bin4_M] -> GetXaxis() -> FindBin(xx);
  Double_t elValue  = hElectron[bin1_M][bin2_M][bin3_M][bin4_M] -> GetBinContent(bNum);
  Double_t piValue  = hPion[bin1_M][bin2_M][bin3_M][bin4_M]     -> GetBinContent(bNum);
  Double_t kValue   = hKaon[bin1_M][bin2_M][bin3_M][bin4_M]     -> GetBinContent(bNum);
  Double_t prValue  = hProton[bin1_M][bin2_M][bin3_M][bin4_M]   -> GetBinContent(bNum);
  if(par[0] == 0)
    {
      return prValue;
      // return elValue;
    }
  else
    if(par[0] == 1)
      return piValue;
    else
      if(par[0] == 2)
	return kValue;
      else
	if(par[0] == 3)
	  {
	    return elValue;///prValue;
	  }
	else
	  return (elValue + piValue + kValue + prValue);
} 

void T49dEdxFunctions::fillHistograms2()
{
  hElectron[bin1_M][bin2_M][bin3_M][bin4_M]   -> Reset();
  hPion[bin1_M][bin2_M][bin3_M][bin4_M]       -> Reset();    
  hKaon[bin1_M][bin2_M][bin3_M][bin4_M]       -> Reset();
  hProton[bin1_M][bin2_M][bin3_M][bin4_M]     -> Reset();
  for(Int_t i = 1; i< size_size+1; i++)
    {
      Double_t xx = hElectron[bin1_M][bin2_M][bin3_M][bin4_M] -> GetXaxis() -> GetBinCenter(i);
      hElectron[bin1_M][bin2_M][bin3_M][bin4_M]  -> SetBinContent(i, getValue(xx,0));
      hPion[bin1_M][bin2_M][bin3_M][bin4_M]      -> SetBinContent(i, getValue(xx,1));;
      hKaon[bin1_M][bin2_M][bin3_M][bin4_M]      -> SetBinContent(i, getValue(xx,2));;
      hProton[bin1_M][bin2_M][bin3_M][bin4_M]    -> SetBinContent(i, getValue(xx,3));;
    }
}

void T49dEdxFunctions::fillHistigrams1()
{
  for(Int_t i = 0; i < fitEnt; i++)
    {
      fitTree -> GetEntry(i);
      I -> SetChargeBin(bin1_M);
      I -> SetPtotBin(bin2_M);
      I -> SetPtBin(bin3_M);
      I -> SetPhiBin(bin4_M);

      binFitted[bin1_M][bin2_M][bin3_M][bin4_M] = 1;

      mtpc_cont -> FindPhase(I);
      sprintf(eName,"hElectron_%d_%d_%d_%d",bin1_M, bin2_M, bin3_M, bin4_M);
      sprintf(piName,"hPion_%d_%d_%d_%d",bin1_M, bin2_M, bin3_M, bin4_M);
      sprintf(kName,"hKaon_%d_%d_%d_%d",bin1_M, bin2_M, bin3_M, bin4_M);
      sprintf(prName,"hProton_%d_%d_%d_%d",bin1_M, bin2_M, bin3_M, bin4_M);
      if(!hElectron[bin1_M][bin2_M][bin3_M][bin4_M])
	{	  
	  hElectron[bin1_M][bin2_M][bin3_M][bin4_M]  = new TH1D(eName,eName,size_size,0.5,2.);
	  hPion[bin1_M][bin2_M][bin3_M][bin4_M]      = new TH1D(piName,piName,size_size,0.5,2.);
	  hKaon[bin1_M][bin2_M][bin3_M][bin4_M]      = new TH1D(kName,kName,size_size,0.5,2.);
	  hProton[bin1_M][bin2_M][bin3_M][bin4_M]    = new TH1D(prName,prName,size_size,0.5,2.);
	}
      sumG -> SetNpHist((TH1F*)mtpc_cont -> GetNpHist(I));
     
      //remove later
      //my_par[6] /= 169278;
      //my_par[7] /= 169278;
      //my_par[8] /= 169278;
      //my_par[9] /= 169278;  
      //remove later
      fillHistograms2();
    }
}

void T49dEdxFunctions::Init()
{
  init();
  fillHistigrams1();
}

void T49dEdxFunctions::SetBins(Int_t *bins)
{
  bin1_M = bins[0];
  bin2_M = bins[1];
  bin3_M = bins[2];
  bin4_M = bins[3];
}

Double_t T49dEdxFunctions::GetIdentity(Int_t i, Double_t x)
{    
  if( !isFitted(0) ) { cout<<"bin is not fitted "<<endl; return -100;}
  Double_t xval[] = {x};
  Double_t par[]  = {i}; 
  Double_t par0[] = {4};
  Double_t totValue = getValueH(xval,par0);
  if(totValue == 0) {cout<<"dEdX is out of range "<<endl; return -100;}
  return getValueH(xval, par)/totValue;  
}


Double_t T49dEdxFunctions::GetIdentity(Int_t i, Double_t x, Float_t *values)
{
   I -> SetCharge(values[0]);
   I -> SetPtot(values[1]);
   I -> SetPt(values[2]);
   I -> SetPhi(values[3]);
   mtpc_cont -> FindBin(I);
   bin1_M = I -> GetChargeBin();
   bin2_M = I -> GetPtotBin();
   bin3_M = I -> GetPtBin();
   bin4_M = I -> GetPhiBin(); 
   return GetIdentity(i,x);
}
  
