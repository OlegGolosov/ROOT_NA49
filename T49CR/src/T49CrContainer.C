#include <iostream>
using namespace std;
//#include <unistd.h>   // for sleep()
//#include <stdlib.h>   // for atoi()
//#include <math.h>

#include "T49CrContainer.h"
#include "T49CrIndex.h"
#include "T49CrInfo.h"
#include "T49CrParticle.h"
#include "T49ParticleRoot.h"
#include "T49Particle.h"


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

#include "Rtypes.h"

#include "PhysConst.h"
#include "TClonesArray.h"

////////////////////////////////////////////////////////////////////    

////////////////////////////////////////////////////////////////////    

#define TRUE  1.0;
#define FALSE 0.0;
#define PI    3.14159265359
#define MAXPART  15000

T49CrIndex   *FillIndex;
T49CrIndex   *DummyIndex = new T49CrIndex();
TTree      *Tree;

////////////////////////////////////////////////////////////////////

ClassImp(T49CrContainer)

T49CrContainer::T49CrContainer()
{
  fMinPtot     = 1.0;
  fMaxPtot     = 40.0;
  fListOn        = 0;
  fSelectPtot    = -1;
  fSelectCharge  = -1;
  fHistBins    = 150;   
  fHistLoBound = 0.5;
  fHistHiBound = 2.0;
  fMinCharge   = -1;
  fMaxCharge   = 1;
  fMinPt       = 0.0;
  fMaxPt       = 2.0;
  fMinPhi      = 0.0;
  fMaxPhi      = 2*PI;
  fMinChargeBin = 0;
  fMaxChargeBin = 1;
  fMinPtotBin   = 0;
}

///////////////////////// init //////////////////////

Int_t T49CrContainer::Init(Int_t *Bins,Char_t *FileName) 
{
  TH1F    *DummyHist;
  T49CrInfo *DummyInfo;
  TObjArray *DummyList;
  TBranch  *branch;
  FillIndex = new T49CrIndex();
  Int_t bsize = 32000;
  //  Int_t split = 1;

  Char_t name[256];
  Char_t text[256];
  Axis_t x[21];
  cerr << "T49CrContainer::init() has been called" << endl;

  fMaxPtotBin   = Bins[1]-1;
  fMinPtBin     = 0;
  fMaxPtBin     = Bins[2]-1;
  fMinPhiBin    = 0;
  fMaxPhiBin    = Bins[3]-1;
  fBins[0] = Bins[0];
  fBins[1] = Bins[1];
  fBins[2] = Bins[2];
  fBins[3] = Bins[3];
  for(Int_t n = 0;n<(Bins[1]+1);n++)
    {
      x[n] = exp(log(fMinPtot)+n*(((log(fMaxPtot)-log(fMinPtot))/Bins[1])));
      printf("Ptot-bin#%d:%f\n",n,x[n]);
    } 

  fChargeAxis = new TAxis(2,-2,2);
  fPtotAxis   = new TAxis((Bins[1]),&x[0]);
  fPtAxis     = new TAxis(Bins[2],fMinPt,fMaxPt);
  fPhiAxis    = new TAxis(Bins[3],fMinPhi,fMaxPhi);

  //Allocate as many histograms as needed by the number of bins given.
  //Hook them up to a 1d TObjArray
  
  fHistList     = new TObjArray();
  fInfoList     = new TObjArray();
  fParticleList = new TObjArray();
  fReturnList   = new TObjArray();
  fParticle     = new T49CrParticle();

  fRootFile    = new TFile(FileName,"RECREATE","A Tree DST");
  fRootFile->cd();
  fRootFile->ls();
  printf("Open File %s\n",FileName);
  fRootFile->SetCompressionLevel(2);
  Tree = new TTree("part","particles for fitting");
  branch = Tree->Branch("Array", &fX[0], "charge/D:ptot/D:pt/D:phi/D:tmean/D:np/D",bsize);
  Tree->Print();
  //  branch = Tree->Branch("Data","T49CrParticle",fParticle,bsize,split);
  //  printf("branch @ %x\n",branch);
  Tree->Print();
  
  for(Int_t i = 0; i < Bins[0] ; i++)
    {
      for(Int_t j = 0;j< Bins[1];j++)
	{
	  for(Int_t k = 0;k< Bins[2];k++)
	    {
	      for(Int_t l = 0;l< Bins[3];l++)
		{
		  sprintf(name,"hist_%d_%d_%d_%d",i,j,k,l);
		  sprintf(text,"histogram bin chrg:%d ptot:%d pt:%d phi:%d",i,j,k,l);
		  DummyHist = new TH1F(name,text,fHistBins,fHistLoBound,fHistHiBound); 
		  DummyInfo = new T49CrInfo();
		  DummyList = new TObjArray();
		  
		  fHistList->Add(DummyHist);
		  fInfoList->Add(DummyInfo);
		  fParticleList->Add(DummyList);
		}
	    }
	}
    }
  //Also allocate TObjArray with structure containing info on the 
  //histograms (pipos pr pos errors etc.
  
  
  cerr << "T49CrContainer::init() has finished" << endl;
  
  return kTRUE;
}

void T49CrContainer::Close()
{
  fRootFile->cd();
  Tree->Write();
  fRootFile->Close();
}

Int_t T49CrContainer::Bin2ListNum(T49CrIndex *Index)
{
  Int_t Num = 0;

  Num = ((fBins[1] *
	  fBins[2] *
	  fBins[3] * Index->GetChargeBin()) +
	 (fBins[2] *
	  fBins[3] * Index->GetPtotBin()) +
	 (fBins[3] * Index->GetPtBin()) +
	 (Index->GetPhiBin()) );
   
  return Num;
}
void T49CrContainer::C3Mom2Index(T49CrIndex *Index)
{
  Index->SetChargeBin(fChargeAxis->FindBin(Index->GetCharge())-1);
  Index->SetPtotBin(fPtotAxis->FindBin(Index->GetPtot())-1);
  Index->SetPtBin(fPtAxis->FindBin(Index->GetPt())-1);
  Index->SetPhiBin(fPhiAxis->FindBin(Index->GetPhi())-1);
}

void T49CrContainer::Index2C3Mom(T49CrIndex *Index)
{
  Index->SetCharge(fChargeAxis->GetBinCenter(Index->GetChargeBin()+1));
  Index->SetPtot(fPtotAxis->GetBinCenter(Index->GetPtotBin()+1));
  Index->SetPt(fPtAxis->GetBinCenter(Index->GetPtBin()+1));
  Index->SetPhi(fPhiAxis->GetBinCenter(Index->GetPhiBin()+1));
}

void T49CrContainer::GetIndex(T49ParticleRoot *Particle, T49CrIndex *Index)
{
  Index->SetCharge(Particle->GetCharge());
  Index->SetPtot(Particle->GetP());
  Index->SetPt(Particle->GetPt());
  Index->SetPhi(Particle->GetPhi());
  C3Mom2Index(Index);
}

void T49CrContainer::GetIndex(T49CrParticle *Particle, T49CrIndex *Index)
{
  //  printf("set index of %f %f %f %f\n",Particle->GetCharge(),Particle->GetPtot(),Particle->GetPt(),Particle->GetPhi());
  Index->SetCharge(Particle->GetCharge());
  Index->SetPtot(Particle->GetPtot());
  Index->SetPt(Particle->GetPt());
  Index->SetPhi(Particle->GetPhi());
  C3Mom2Index(Index);
}

void T49CrContainer::GetIndex(Double_t *X, T49CrIndex *Index)
{
  //  printf("set index of %f %f %f %f\n",Particle->GetCharge(),Particle->GetPtot(),Particle->GetPt(),Particle->GetPhi());
  Index->SetCharge(X[0]);
  Index->SetPtot(X[1]);
  Index->SetPt(X[2]);
  Index->SetPhi(X[3]);
  C3Mom2Index(Index);
}

Double_t T49CrContainer::IsInBin(T49CrParticle *Part, T49CrIndex *Index)
{
  GetIndex(Part,DummyIndex); 
  //  FindBin(DummyIndex);
  if(IndexIn(DummyIndex))
    {
      if((DummyIndex->GetChargeBin() == Index->GetChargeBin())&&
	 (DummyIndex->GetPtotBin()   == Index->GetPtotBin()  )&&
	 (DummyIndex->GetPtBin()     == Index->GetPtBin()    )&&
	 (DummyIndex->GetPhiBin()    == Index->GetPhiBin()   ))
	return TRUE;
    }
  return FALSE;
}

Double_t T49CrContainer::IsInBin(T49ParticleRoot *Particle, T49CrIndex *Index)
{
  GetIndex(Particle,DummyIndex); 
  // FindBin(DummyIndex);
  if((DummyIndex->GetChargeBin() == Index->GetChargeBin())&&
     (DummyIndex->GetPtotBin()   == Index->GetPtotBin()  )&&
     (DummyIndex->GetPtBin()     == Index->GetPtBin()    )&&
     (DummyIndex->GetPhiBin()    == Index->GetPhiBin()   ))
    return TRUE;
  return FALSE;
}

Double_t T49CrContainer::IsInBin(Double_t *X, T49CrIndex *Index)
{
  GetIndex(X,DummyIndex); 
  // FindBin(DummyIndex);
  if((DummyIndex->GetChargeBin() == Index->GetChargeBin())&&
     (DummyIndex->GetPtotBin()   == Index->GetPtotBin()  )&&
     (DummyIndex->GetPtBin()     == Index->GetPtBin()    )&&
     (DummyIndex->GetPhiBin()    == Index->GetPhiBin()   ))
    return TRUE;
  return FALSE;
}

Int_t T49CrContainer::GetList(T49CrIndex *Index,TTree *InTree,TClonesArray *OutList)
{
  Int_t Entry;
  Int_t Count;
  fReturnList->Clear();
  Double_t nX[6];
  T49CrParticle *NuParticle;

  TBranch *InBranch  = InTree->GetBranch("Array");
  //  InTree->Print();
  InBranch->SetAddress(nX);
  Count = 0;
  if(IndexIn(Index))
    {
      for(Entry = 0; Entry < InTree->GetEntries();Entry++)
	{
	  InTree->GetEvent(Entry);
	  if(IsIn(nX))
	     {
	       if(IsInBin(nX,Index))
		 {
		   NuParticle = (T49CrParticle *)OutList->operator[](Count);
		   NuParticle->SetCharge(nX[0]);
		   NuParticle->SetPtot(nX[1]);
		   NuParticle->SetPt(nX[2]);
		   NuParticle->SetPhi(nX[3]);
		   NuParticle->SetTmean(nX[4]);
		   NuParticle->SetNPoint((Int_t) nX[5]);
		   Count++;
		 }
	     }
	}
      return Count;
    }
  else
    {
      printf("Error! Index out of Range!\n");
      return  0;
    }
}

TH1F *T49CrContainer::GetHist(T49CrIndex *Index)
{
  TH1F *Hist;

  if(IndexIn(Index))
    {
      Hist = (TH1F *)fHistList->At(Bin2ListNum(Index));
      return Hist;
    }
  else
    {
      printf("Error! Index out of Range!\n");
      Hist = NULL;
      return Hist;
    }
}

T49CrInfo *T49CrContainer::GetInfo(T49CrIndex *Index)
{
  T49CrInfo *Info;
  if(IndexIn(Index))
    {
      Info = (T49CrInfo *) fInfoList->At(Bin2ListNum(Index));
      return Info;
    }
  else
    {
      printf("Error! Index out of Range!\n");
      Info = NULL;
      return Info;
    }
}

Double_t T49CrContainer::FillContainer(T49ParticleRoot *Particle)
{
  TH1F       *DummyHist;

  GetIndex(Particle,FillIndex);
  fRootFile->cd();
  //  fRootFile->ls();
  //  TBranch *branch;
 
  if(IsIn(Particle)&&CheckSelect(FillIndex))
    {
      DummyHist = GetHist(FillIndex);
      DummyHist->Fill(Particle->GetTmeanCharge(2)/1000);

      if(fListOn>0)
	{
	  //chrg/D:ptot/D:pt/D:phi/D:tmean/D:np/D
	  fX[0] = Particle->GetCharge();
	  fX[1] = Particle->GetP();
	  fX[2] = Particle->GetPt();
	  fX[3] = Particle->GetPhi();
	  fX[4] = Particle->GetTmeanCharge(2)/1000;
	  fX[5] = Particle->GetNPoint(2);
	  Tree->Fill();
	}
      fNParticles++;
      return TRUE;
    }
  else
    return FALSE;
      
}

Double_t T49CrContainer::FillContainer(T49ParticleRoot *Particle, Int_t IsMTPC)
{
  TH1F       *DummyHist;

  GetIndex(Particle,FillIndex);
  fRootFile->cd();
  //  fRootFile->ls();
  //  TBranch *branch;
 
  if(IsIn(Particle)&&CheckSelect(FillIndex))
    {
      DummyHist = GetHist(FillIndex);
      if(IsMTPC)
	DummyHist->Fill(Particle->GetTmeanCharge(2)/1000);
      else
	DummyHist->Fill(Particle->GetTmeanCharge()/1000);

      if(fListOn>0)
	{
	  //chrg/D:ptot/D:pt/D:phi/D:tmean/D:np/D
	  fX[0] = Particle->GetCharge();
	  fX[1] = Particle->GetP();
	  fX[2] = Particle->GetPt();
	  fX[3] = Particle->GetPhi();
	  if(IsMTPC)
	    fX[4] = Particle->GetTmeanCharge(2)/1000;
	  else
	    fX[4] = Particle->GetTmeanCharge()/1000;
	  fX[5] = Particle->GetNPoint(2);
	  Tree->Fill();
	}
      fNParticles++;
      return TRUE;
    }
  else
    return FALSE;
      
}

Bool_t T49CrContainer::IsIn(T49ParticleRoot *Particle)
{
  if((Particle->GetCharge() >= fMinCharge && Particle->GetCharge() <= fMaxCharge)&&
     (Particle->GetP()      > fMinPtot   && Particle->GetP()      < fMaxPtot)&&
     (Particle->GetPt()     > fMinPt     && Particle->GetPt()     < fMaxPt)&&
     (Particle->GetPhi()    > fMinPhi    && Particle->GetPhi()    < fMaxPhi))
    {
      //cout<<"minPtot "<<fMinPtot<<endl;
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}
Bool_t T49CrContainer::IsIn(T49CrParticle *Particle)
{
  if((Particle->GetCharge() >= fMinCharge && Particle->GetCharge() <= fMaxCharge)&&
     (Particle->GetPtot()      > fMinPtot   && Particle->GetPtot()      < fMaxPtot)&&
     (Particle->GetPt()     > fMinPt     && Particle->GetPt()     < fMaxPt)&&
     (Particle->GetPhi()    > fMinPhi    && Particle->GetPhi()    < fMaxPhi))
    {
      
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

Double_t T49CrContainer::IsIn(Double_t *X)
{
  /*  fX[0] = Particle->GetCharge();
      fX[1] = Particle->GetP();
      fX[2] = Particle->GetPt();
      fX[3] = Particle->GetPhi();
      */
  if((X[0] >= fMinCharge && X[0] <= fMaxCharge)&&
     (X[1] >  fMinPtot   && X[1] <  fMaxPtot)&&
     (X[2] >  fMinPt     && X[2] <  fMaxPt)&&
     (X[3] >  fMinPhi    && X[3] < fMaxPhi))
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

Double_t T49CrContainer::InAcceptance(T49ParticleRoot *Particle)
{
  if(IsIn(Particle))
    {
      GetIndex(Particle,DummyIndex);
      T49CrInfo *Info = GetInfo(DummyIndex);
      if(Info->GetAmplitude(1)>0)
	{
	  return TRUE;
	}
      return FALSE;
      
    }
  else
    {
      return FALSE;
    }
}

Double_t T49CrContainer::InAcceptance(T49CrParticle *Particle)
{
  if(!IsIn(Particle))
    {
      T49CrInfo  *Info;
      T49CrIndex  Index;
      GetIndex(Particle,&Index);
      Info = GetInfo(&Index);
      if(Info->GetAmplitude(1)>0)
	{
	  return TRUE;
	}
      return FALSE;
      
    }
  else
    {
      return FALSE;
    }

}

Double_t T49CrContainer::CheckSelect(T49CrIndex *Index)
{
  Int_t PtotOk = 0;
  Int_t ChargeOk = 0;

  if(fSelectPtot <0 || (fSelectPtot == Index->GetPtotBin()))
    {
      PtotOk = 1;
    }
  if(fSelectCharge <0 || (fSelectCharge == Index->GetChargeBin()) )
    {
      ChargeOk = 1;
    }

  if(PtotOk > 0 && ChargeOk > 0)
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

Double_t T49CrContainer::IndexIn(T49CrIndex *Index)
{
  if((Index->GetChargeBin() >= fMinChargeBin && Index->GetChargeBin() <= fMaxChargeBin)&&
     (Index->GetPtotBin()   >= fMinPtotBin   && Index->GetPtotBin()   <= fMaxPtotBin)&&
     (Index->GetPtBin()     >= fMinPtBin     && Index->GetPtBin()     <= fMaxPtBin)&&
     (Index->GetPhiBin()    >= fMinPhiBin    && Index->GetPhiBin()    <= fMaxPhiBin))
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}
/*
  Float_t T49CrContainer::GetRap(Float_t mass,Float_t Ptot, Float_t Pt)
  {
  Float_t rap,Pz;
  Float_t E;
  
  Pz = sqrt((Ptot*Ptot)-(Pt*Pt));
  E = sqrt(mass*mass + Ptot*Ptot);
  rap = 0.5 * log((E+Pz)/(E-Pz));
  
  return rap;
  }
  */
