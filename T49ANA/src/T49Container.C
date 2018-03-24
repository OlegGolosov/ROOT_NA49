#include <iostream>
using namespace std;

#include "T49Container.h"
#include "T49Index.h"
#include "T49DedxInfo.h"
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
#include "TMath.h"

#include "PhysConst.h"

/*
$Log: T49Container.C,v $
Revision 1.11  2008/01/04 10:48:58  cblume
64bit modifications by Marek Szuba

Revision 1.10  2005/05/20 11:26:23  cblume
Get rid of deprecated warnings

Revision 1.9  2002/10/07 09:46:56  cblume
Update for CASTOR

Revision 1.7  2001/11/12 12:09:53  cblume
Update for ROOT v3.01

 * Revision 1.6  2000/08/28  12:19:33  cblume
 * Update by Marco for root 2.25
 *
 * Revision 1.4  2000/07/07  14:11:46  cblume
 * Made ROOT v2.25 compatibel by Marco
 *
 * Revision 1.2  2000/01/30  14:43:23  cblume
 * T49CutPid.C
 *
 * Revision 1.1  2000/01/17  12:38:05  cblume
 * Add T49Container.C
 *
*/

//_________________________________________________________________
//  Container class to hold dE/dx histograms in phasespace-bins. Use
//  Init to set up phase space binning etc.  The class can also hold
//  fit-results (T49DedxInfo). Use T49Index to help retrieving
//  information per phase space bin.


ClassImp(T49Container)


T49Container::T49Container()
{
  //cout << "begin T49Container::T49Container()" << endl;
  fTPC=2;
  DummyIndex = new T49Index();
  fNPointFlag=0;
  fBins[0]=0;
  fInfoList=0;
  fHistList=0;
  fNpHistList=0;
  fYPtBinning=0;
  fChargeAxis=0;
  fPtotAxis=0;  
  fPtAxis=0; 
  fPhiAxis=0;
  fNEvent=0;
  fPowSigScale=1;
  fRootFile=0;    // This meber is defunct
  //cout << "end T49Container::T49Container()" << endl;
}

T49Container::~T49Container()
{
  //cout << "begin T49Container::~T49Container()" << endl;
  delete DummyIndex;
  delete fChargeAxis;
  delete fPtotAxis;
  delete fPtAxis;
  delete fPhiAxis;
  fHistList->Delete();
  delete fHistList;
  if (fNpHistList) fNpHistList->Delete();
  delete fNpHistList;
  fInfoList->Delete();
  delete fInfoList;
  //cout << "end T49Container::~T49Container()" << endl;
}


Int_t T49Container::Init(const Int_t NPBin, Float_t PMin, Float_t PMax,
                         Int_t NPtBin, Float_t PtMin, Float_t PtMax,
                         Int_t NPhiBin, Bool_t ypt_flag, Float_t mass) 
{
  // Sets up data structures to hold dE/dx values in requested
  // binning. Number of bins: 2, total number of bins
  // 2*NPBin*NPtBin*NPhiBin. The range in phi is 0 to 2*pi.
  // 
  // If ypt_flag!=0 the binning will be in rapidity and pt, instead of
  // total momentum and pt. The specified mass will be used to calculate
  // rapidity (default is the kaon mass).
  TH1F    *DummyHist;
  T49DedxInfo *DummyInfo;

  Char_t name[256];
  Char_t text[256];
  Axis_t x[NPBin+1];

  if (ypt_flag)
    fYPtBinning=1;
  fRapMass     = mass;
  fHistBins    = 150;   
  fHistLoBound = 0.5;
  fHistHiBound = 2.0;
  fMinCharge   = -1;
  fMaxCharge   = 1;
  fMinPtot     = PMin;
  fMaxPtot     = PMax;
  fMinPt       = PtMin;
  fMaxPt       = PtMax;
  fMinPhi      = 0.0;
  fMaxPhi      = 2*TMath::Pi();
  fMinChargeBin = 0;
  fMaxChargeBin = 1;
  fMinPtotBin   = 0;
  fMaxPtotBin   = NPBin-1;
  fMinPtBin     = 0;
  fMaxPtBin     = NPtBin-1;
  fMinPhiBin    = 0;
  fMaxPhiBin    = NPhiBin-1;
  fBins[0] = 2;
  fBins[1] = NPBin;
  fBins[2] = NPtBin;
  fBins[3] = NPhiBin;
  if (fYPtBinning) {
    fPtotAxis   = new TAxis((fBins[1]),fMinPtot,fMaxPtot);
  }
  else {
    for(Int_t n = 0;n<(fBins[1]+1);n++) {
      x[n] = exp(log(fMinPtot)+n*(((log(fMaxPtot)-log(fMinPtot))/fBins[1])));
      printf("Ptot-bin#%d:%f\n",n,x[n]);
    } 
    fPtotAxis   = new TAxis((fBins[1]),x);
  }
  fChargeAxis = new TAxis(2,-2,2);
  fPtAxis     = new TAxis(fBins[2],fMinPt,fMaxPt);
  fPhiAxis    = new TAxis(fBins[3],fMinPhi,fMaxPhi);

  //Allocate as many histograms as needed by the number of bins given.
  //Hook them up to a 1d TObjArray
  
  fHistList     = new TObjArray();
  fInfoList     = new TObjArray();

  for(Int_t i = 0; i < fBins[0] ; i++)
    {
      for(Int_t j = 0;j< fBins[1];j++)
	{
	  for(Int_t k = 0;k< fBins[2];k++)
	    {
	      for(Int_t l = 0;l< fBins[3];l++)
		{
		  sprintf(name,"hist_%d_%d_%d_%d",i,j,k,l);
		  if (fYPtBinning)
		    sprintf(text,"histogram bin chrg:%d y:%d pt:%d phi:%d",i,j,k,l);
		  else
		    sprintf(text,"histogram bin chrg:%d ptot:%d pt:%d phi:%d",i,j,k,l);
		  DummyHist = new TH1F(name,text,fHistBins,fHistLoBound,fHistHiBound); 
		  DummyInfo = new T49DedxInfo();
		  
		  fHistList->Add(DummyHist);
		  fInfoList->Add(DummyInfo);
		}
	    }
	}
    }
  //Also allocate TObjArray with structure containing info on the 
  //histograms (pipos pr pos errors etc.
  
  return kTRUE;
}

void T49Container::StoreNPoint(Bool_t flag, Int_t nbins, Float_t min, Float_t max)
{
  // Sets flags to also store number of points while filling container. 
  // Number of point-histograms kan be retrieved using GetNpHist().
  fNPointFlag=flag;
  if (flag) {
    if (fBins[0]==0) { 
      cerr << "T49Container::StoreNPoint: Error: First call Init!" <<endl;
      return;
    }
    if (nbins==0) {
      if (fTPC==0||fTPC==1) {
	nbins=60;
	min=0.5;
	max=60.5;
      }
      if (fTPC==2) {
	nbins=90;
	min=0.5;
	max=90.5;
      }
      if (fTPC==3) {
	nbins=210;
	min=0.5;
	max=210.5;
      }
    }
    if (fNpHistList)
      fNpHistList->Delete();
    else 
      fNpHistList=new TObjArray();
    Char_t name[25];
    Char_t title[100];
    for(Int_t i = 0; i < fBins[0] ; i++) {
      for(Int_t j = 0;j< fBins[1];j++) {
	for(Int_t k = 0;k< fBins[2];k++) {
	  for(Int_t l = 0;l< fBins[3];l++) {
	    sprintf(name,"n_point_%d_%d_%d_%d",i,j,k,l);
	    if (fYPtBinning) 
	      sprintf(title,"n_dedx_point bin chrg:%d y:%d pt:%d phi:%d",i,j,k,l);
	    else
	      sprintf(title,"n_dedx_point bin chrg:%d ptot:%d pt:%d phi:%d",i,j,k,l);
	    fNpHistList->Add(new TH1F(name,title,nbins,min,max));
	  }
	}
      }
    }
  }
}


Int_t T49Container::Bin2ListNum(T49Index *Index)
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

void T49Container::C3Mom2Index(T49Index *Index)
{
  Index->SetChargeBin(fChargeAxis->FindBin(Index->GetCharge())-1);
  Index->SetPtotBin(fPtotAxis->FindBin(Index->GetPtot())-1);
  Index->SetPtBin(fPtAxis->FindBin(Index->GetPt())-1);
  Index->SetPhiBin(fPhiAxis->FindBin(Index->GetPhi())-1);
}

void T49Container::Index2C3Mom(T49Index *Index)
{
  Index->SetCharge(fChargeAxis->GetBinCenter(Index->GetChargeBin()+1));
  Index->SetPtot(fPtotAxis->GetBinCenter(Index->GetPtotBin()+1));
  Index->SetPt(fPtAxis->GetBinCenter(Index->GetPtBin()+1));
  Index->SetPhi(fPhiAxis->GetBinCenter(Index->GetPhiBin()+1));
}

void T49Container::GetIndex(T49ParticleRoot *Particle, T49Index *Index)
{
  // Sets values in index-class corresponding to Particle.

  Index->SetCharge(Particle->GetCharge());

  if (fYPtBinning)
    Index->SetPtot(Particle->GetRap(fRapMass));
  else 
    Index->SetPtot(Particle->GetP());

  Index->SetPt(Particle->GetPt());
  Index->SetPhi(Particle->GetPhi());
  C3Mom2Index(Index);
}

void T49Container::GetIndex(Double_t *X, T49Index *Index)
{
  // Get index for phase space parameters specified by X.
  // X[0]: charge, X[1]: momentum, X[2]: Pt, X[3]: Phi

  Index->SetCharge(X[0]);
  Index->SetPtot(X[1]);
  Index->SetPt(X[2]);
  Index->SetPhi(X[3]);
  C3Mom2Index(Index);
}

Bool_t T49Container::IsInBin(T49ParticleRoot *Particle, T49Index *Index)
{
  GetIndex(Particle,DummyIndex); 
  if((DummyIndex->GetChargeBin() == Index->GetChargeBin())&&
     (DummyIndex->GetPtotBin()   == Index->GetPtotBin()  )&&
     (DummyIndex->GetPtBin()     == Index->GetPtBin()    )&&
     (DummyIndex->GetPhiBin()    == Index->GetPhiBin()   ))
    return kTRUE;
  return kFALSE;
}

Bool_t T49Container::IsInBin(Double_t *X, T49Index *Index)
{
  GetIndex(X,DummyIndex); 
  // FindBin(DummyIndex);
  if((DummyIndex->GetChargeBin() == Index->GetChargeBin())&&
     (DummyIndex->GetPtotBin()   == Index->GetPtotBin()  )&&
     (DummyIndex->GetPtBin()     == Index->GetPtBin()    )&&
     (DummyIndex->GetPhiBin()    == Index->GetPhiBin()   ))
    return kTRUE;
  return kFALSE;
}

Float_t T49Container::GetProbability(T49ParticleRoot* part,Int_t type)
{
  // Get array of probablities for a specific particle type for 
  // particle 'part'. The function returns 0 if there is no phase space bin,
  // corresponding to the particle.
  //

  Float_t prob=0;
  if (part->GetNDedxPoint(fTPC)==0) return 0;
  if (part->GetTmeanCharge(fTPC)==0) return 0;
  if (!IsIn(part)) return 0;

  T49Index I;
  GetIndex(part,&I);
  T49DedxInfo *Info;
  Info = GetInfo(&I);
  TH1F *dedx_hist=GetHist(&I);
  if (Info->GetPosition(0)==0) return 0;
  Float_t dedx=part->GetTmeanCharge(fTPC)/1000.;
  if (dedx_hist->GetBinContent(dedx_hist->FindBin(dedx))==0) return 0;
  // printf("dedx of current track: %f\n",dedx);
  Float_t pos=Info->GetPosition(type);
  Float_t res=Info->GetReso()*pos/TMath::Sqrt(((Double_t) part->GetNDedxPoint(fTPC)));
  prob=Info->GetAmplitude(type)/res/TMath::Sqrt(2*TMath::Pi())*exp(-0.5*(dedx-pos)*(dedx-pos)/(res*res));
  prob/=dedx_hist->GetBinContent(dedx_hist->FindBin(dedx));
  return prob;     
}

Bool_t T49Container::GetProbabilityOld(T49ParticleRoot* part, Float_t* prob)
{
  // Get array of probablities for different particle types for 
  // particle 'part'. The function returns 0 if there is no phase space bin,
  // corresponding to the particle, or 1 for success.
  //
  // Uses old definition of sigma (four Gaus fit)

  if (!IsIn(part))
    return 0;
  T49Index I;
  GetIndex(part,&I);
  T49DedxInfo *Info;
  Info = GetInfo(&I);
  if (Info->GetPosition(0)==0) return 0;
  Float_t dedx=part->GetTmeanCharge(fTPC)/1000;
  // printf("dedx of current track: %f\n",dedx);
  Float_t pos;
  Float_t res=Info->GetReso();
  // printf("resolution: %f \n",res);
  Float_t prob_tot=0;
  for (Int_t p = 0;p<4;p++) {
    pos=Info->GetPosition(p);
    //printf("pos[%d]=%f, ",p,pos);
    prob[p]=Info->GetAmplitude(p)*exp(-0.5*(dedx-pos)*(dedx-pos)/(res*pos*res*pos));
    //printf("prob=%f\n",prob[p]);
    prob_tot += prob[p];
  }
  for (Int_t p=0;p<4;p++) prob[p]/=prob_tot;
  return 1;     
}


Int_t T49Container::GetDeltaDedx(T49ParticleRoot* part, Int_t type, Float_t& delta)
{
// Calculates distance of dE/dx of part from dE/dx expected for type, 
// in units of standard deviations (resolution). 
// If no calibration is found for this kinematical region, 0 is returned. 
// If the calibration has shown that type is not present in this 
// phase space bin, a value < 0 (i.e. -1) is returned.
//
// This uses the new definition of sigma, i.e. resulting from a fit with 
// T49SumGaus.
  
  if (!IsIn(part))
    return 0;
  T49Index I;
  GetIndex(part,&I);
  T49DedxInfo *Info;
  Info = GetInfo(&I);
  if (Info->GetPosition(0)==0) return 0;
  if (Info->GetAmplitude(type)==0) {
    delta=-999;
    return -1;
  }
  if (part->GetNDedxPoint(fTPC)==0) {
    delta=-998;
    return 1;
  }
    
  Float_t pos=Info->GetPosition(type);
  Float_t res=Info->GetReso()*pow(pos,fPowSigScale)/TMath::Sqrt(((Double_t) part->GetNDedxPoint(fTPC)));
  delta=(part->GetTmeanCharge(fTPC)/1000.-pos);
  if (delta<0) 
    res*=1-Info->GetAsymmetry();
  else
    res*=1+Info->GetAsymmetry();
  delta/=res;
  return 1;
}


Int_t T49Container::GetDeltaDedxOld(T49ParticleRoot* part, Int_t type, Float_t& delta)
{
// Calculates distance of dE/dx of part from dE/dx expected for type, 
// in units of standard deviations (resolution). 
// If no calibration is found for this kinematical region, 0 is returned. 
// If the calibration has shown that type is not present in this 
// phase space bin, a value < 0 (i.e. -1) is returned.
//
// This uses the old definition of sigma, i.e. four a four-Gaussian fit to
// the dE/dx histograms
  
  if (!IsIn(part))
    return 0;
  T49Index I;
  GetIndex(part,&I);
  T49DedxInfo *Info;
  Info = GetInfo(&I);
  if (Info->GetPosition(0)==0) return 0;
  if (Info->GetAmplitude(type)==0) {
    delta=-999;
    return -1;
  }
  Float_t pos=Info->GetPosition(type);
  delta=(part->GetTmeanCharge(fTPC)/1000-pos)/(Info->GetReso()*pos);
  return 1;
}

TH1F *T49Container::GetHist(T49Index *Index)
{
// Returns pointer to dE/dx histogram.

  if(IndexIn(Index)) {
    return (TH1F*) fHistList->At(Bin2ListNum(Index));
  }
  else {
    printf("Error! Index out of Range!\n");
    return 0;
  }
}

TH1F *T49Container::GetNpHist(T49Index *Index)
{
// Returns pointer to number of point histogram.

  if(fNpHistList && IndexIn(Index)) {
    return (TH1F*) fNpHistList->At(Bin2ListNum(Index));
  }
  else if (!fNpHistList) 
    printf("Warning: no number of point histograms stored in this T49Container\n");
  else {
    printf("Error! Index out of Range!\n");
  }
  return 0;
}


T49DedxInfo *T49Container::GetInfo(T49Index *Index)
{
// Returns pointer to fit result.

  if(IndexIn(Index)) {
    return (T49DedxInfo *) fInfoList->At(Bin2ListNum(Index));
  }
  else { 
    printf("Error! Index out of Range!\n");
    return 0;
  }
}

Bool_t T49Container::FillContainer(T49ParticleRoot *Particle)
{
// Fill histogram with dE/dx value for particle. Use SetTPC to specify 
// which dE/dx value should be used. Use SetNPointFlag to also store 
// number of points per track.

  TH1F       *DummyHist;
  if(IsIn(Particle))
    {
      GetIndex(Particle,DummyIndex);
      DummyHist = GetHist(DummyIndex);
      DummyHist->Fill(Particle->GetTmeanCharge(fTPC)/1000);
      if (fNPointFlag) {
	DummyHist = GetNpHist(DummyIndex);
	DummyHist->Fill(Particle->GetNDedxPoint(fTPC));
      }
      return kTRUE;
    }
  else
    return kFALSE;
      
}

Bool_t T49Container::IsIn(T49ParticleRoot *Particle)
{
  if (fYPtBinning) {
     Float_t rap=Particle->GetRap(fRapMass);
     if (rap < fMinPtot || rap > fMaxPtot)
       return kFALSE;
  }
  else if (Particle->GetP() < fMinPtot || Particle->GetP() > fMaxPtot)
    return kFALSE;

  if((Particle->GetCharge() >= fMinCharge && Particle->GetCharge() <= fMaxCharge)&&
     (Particle->GetPt()     > fMinPt     && Particle->GetPt()     < fMaxPt)&&
     (Particle->GetPhi()    > fMinPhi    && Particle->GetPhi()    < fMaxPhi))
    return kTRUE;
  else
    return kFALSE;
}

Bool_t T49Container::IsIn(Double_t *X)
{
  if((X[0] >= fMinCharge && X[0] <= fMaxCharge)&&
     (X[1] >  fMinPtot   && X[1] <  fMaxPtot)&&
     (X[2] >  fMinPt     && X[2] <  fMaxPt)&&
     (X[3] >  fMinPhi    && X[3] < fMaxPhi))
    {
      return kTRUE;
    }
  else
    {
      return kFALSE;
    }
}

Bool_t T49Container::IndexIn(T49Index *Index)
{
  if((Index->GetChargeBin() >= fMinChargeBin && Index->GetChargeBin() <= fMaxChargeBin)&&
     (Index->GetPtotBin()   >= fMinPtotBin   && Index->GetPtotBin()   <= fMaxPtotBin)&&
     (Index->GetPtBin()     >= fMinPtBin     && Index->GetPtBin()     <= fMaxPtBin)&&
     (Index->GetPhiBin()    >= fMinPhiBin    && Index->GetPhiBin()    <= fMaxPhiBin))
    {
      return kTRUE;
    }
  else
    {
      return kFALSE;
    }
}


void T49Container::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49Container.

   if (R__b.IsReading()) {
     UInt_t R__s, R__c;
     Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
     if (R__v > 6) {
       T49Container::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
       return;
     }
     TObject::Streamer(R__b);
     R__b >> fRootFile;
     R__b >> fChargeAxis;
     R__b >> fPtotAxis;
     R__b >> fPtAxis;
     R__b >> fPhiAxis;
     R__b >> fInfoList;
     R__b >> fHistList;
     R__b.ReadStaticArray(fBins);
     R__b >> fNHistTot;
     R__b >> fHistBins;
     R__b >> fHistHiBound;
     R__b >> fHistLoBound;
     R__b >> fMinCharge;
     R__b >> fMaxCharge;
     R__b >> fMinPtot;
     R__b >> fMaxPtot;
     if (R__v<4) {
       Float_t tmp;
       R__b >> tmp;
       fMinPt = tmp;
       R__b >> tmp;
       fMaxPt = tmp;
       R__b >> tmp;
       fMinPhi = tmp;
       R__b >> tmp;
       fMaxPhi = tmp;
     }
     else {
       R__b >> fMinPt;
       R__b >> fMaxPt;
       R__b >> fMinPhi;
       R__b >> fMaxPhi;
     }
     R__b >> fMinChargeBin;
     R__b >> fMaxChargeBin;
     R__b >> fMinPtotBin;
     R__b >> fMaxPtotBin;
     R__b >> fMinPtBin;
     R__b >> fMaxPtBin;
     R__b >> fMinPhiBin;
     R__b >> fMaxPhiBin;
     R__b >> fTPC;
     R__b >> DummyIndex;
     if (R__v >= 2) {
       R__b >> fNpHistList;
       R__b >> fNPointFlag;
     }
     if (R__v >= 5) {
       R__b >> fYPtBinning;
       R__b >> fRapMass;
     }
     TObjArray *dum_list=0;
     Bool_t dum_flg;
     if (R__v == 6) {
       R__b >> dum_list;
       R__b >> dum_flg;
       if (dum_list) {
	 dum_list->Delete();
	 delete dum_list;
       }
     }
     R__b.CheckByteCount(R__s, R__c, T49Container::IsA());
   } else {
     T49Container::Class()->WriteBuffer(R__b,this);
   }
}



