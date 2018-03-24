#include <iostream>
using namespace std;

#include "T49Dedx.h"

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


////////////////////////////////////////////////////////////////////    

Float_t  RelRise(Float_t p, Float_t mass);
Double_t FuncDedx(Double_t *x,Double_t *par); 
void     TestFunc(Int_t &npar, Double_t *gin, Double_t &f, Double_t *u, Int_t flag); 

////////////////////////////////////////////////////////////////////    

#define MINPOINTS       25
#define MINDEDXPOINTS   50

#define MAXPART  15000
Float_t   val[MAXPART][2]; 
Int_t     NumPart;

Double_t Resolution = 0.04;
Double_t MinIon     = 326.0;
Double_t Norm       = 1.0/sqrt(2.0*3.14159);


TF2 *F2Dedx;

////////////////////////////////////////////////////////////////////

ClassImp(T49Dedx)

/*
$Log: T49Dedx.C,v $
Revision 1.5  2008/01/04 10:48:58  cblume
64bit modifications by Marek Szuba

Revision 1.4  2005/05/20 11:26:47  cblume
Get rid of deprecated warnings

Revision 1.3  1999/12/01 09:39:25  cblume
Changed to root 2.23-09. Updated the makefiles. Changes in T49CutTrack, T49Dedx, T49Proj1, T49Proj2, TRootDS

 * Revision 1.2  1999/11/24  16:03:44  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 *
 * Revision 1.1  1999/11/24  15:33:44  cblume
 * Add T49Dedx.C
 *
*/

T49Dedx::T49Dedx()
{
    fRes0 = 0.23;
    fRes1 = 0.0003;
    fMinIonPos = 303.5;
    fMinIonNeg = 298.5;
    sprintf(fCalibHome,"/home/data1");

}

///////////////////////// init //////////////////////

Int_t T49Dedx::Init() 
{
    cerr << "T49Dedx::init() has been called" << endl;

    F2Dedx   = new TF2("F2Dedx",FuncDedx,4);
    F2Dedx->SetName("F2Dedx");
    F2Dedx->SetParameters(0.1,0.1,0.1,100);

    cerr << "T49Dedx::init() has finished" << endl;

    return kTRUE;
}

TF2 *T49Dedx::GetFunc()
{
    return F2Dedx;
}

#define DEDXNORM 0.6245

Float_t T49Dedx::GetRelRise(Float_t mom, Float_t mass)
{
    return RelRise(mom,mass);
}

Double_t FuncDedx(Double_t *x,Double_t *par)
{
    Int_t i;
    Double_t pos[4];
    Double_t sig[4];
    Double_t res;
    Double_t f;
    Double_t PiConst;

#ifdef DEBUGFUNC
    printf("FuncDedx called : %g %g,  %g %g %g %g\n",x[0],x[1],par[0],par[1],par[2],par[3]);
#endif

    for(i=0;i<3;i++)
    {
       if(par[i] < 0)
       {  
           return 100000000.0;
       }
    }
    PiConst = 1.0 - (par[0]+par[1]+par[2]);
    if(PiConst < 0)
      return 10000000.0;
      
    res = Resolution + 0.001 * x[0];
    pos[0] = MinIon * RelRise((Float_t) x[0],(Float_t) kPMASS);
    pos[1] = MinIon * RelRise((Float_t) x[0],(Float_t) kKMASS);
    pos[2] = MinIon * RelRise((Float_t) x[0],(Float_t) kPIMASS);
    pos[3] = MinIon * RelRise((Float_t) x[0],(Float_t) kEMASS);
    sig[0] = pos[0] * res;
    sig[1] = pos[1] * res;
    sig[2] = pos[2] * res;
    sig[3] = pos[3] * res;
    f = par[3]*par[0] *exp(-(x[1]-pos[0])*(x[1]-pos[0])/(2.0*sig[0]*sig[0])) +
        par[3]*par[1] *exp(-(x[1]-pos[1])*(x[1]-pos[1])/(2.0*sig[1]*sig[1])) +
        par[3]*PiConst*exp(-(x[1]-pos[2])*(x[1]-pos[2])/(2.0*sig[2]*sig[2])) +
        par[3]*par[2] *exp(-(x[1]-pos[3])*(x[1]-pos[3])/(2.0*sig[3]*sig[3]));

#ifdef DEBUGFUNC
    printf("f = %g\n",f);
#endif

    return f;
}

void T49Dedx::EventFit(T49EventRoot *event, Double_t *p2pi, Double_t *k2pi)
{
    //Int_t     j;
    //Int_t     i;
    Int_t     ErrFlag;
    //Float_t  *row;
    Double_t arglist[10];
    Double_t we, al, bl;
    Double_t pNum = 0;
    Double_t kNum = 0;
    Double_t par;
    const Double_t Pmin = 5;
    const Double_t Pmax = 50;
    T49Particle  *track;

    //Double_t kPar[5] = {1.0443E2,2.0724E1,-4.067,-5.349E-1,-1.3876E-2};
    //Double_t pPar[5] = {2.46E2,-9.706E-1,3.1679E-1,0,0};
    //Double_t piPar[5] ={-1.3874E3,1.68399E3,-1.73976E2,6.25518,-7.50058E-2};
    //Double_t pNorm  = 15695.7; 
    //Double_t kNorm  = 30905.2; 
    //Double_t piNorm = 105105.2; 
    
    static Int_t index = 0;
    static TMinuit *myMinuit = NULL;

    NumPart = 0;
    //T49ParticleIter *iter = event->GetParticleIter();
    //while((track = (T49Particle *) iter->Next()))
    //{
    TClonesArray *ParticleList = event->GetPrimaryParticles();
    for (Int_t iTrack = 0; iTrack < ParticleList->GetEntries(); iTrack++) {
        track = (T49Particle *) ParticleList->UncheckedAt(iTrack);
        if(track->GetP() > Pmin && 
           track->GetP() < Pmax &&
           track->GetTmeanCharge() > 300 && 
           track->GetTmeanCharge() < 500 && 
           track->GetNFitPoint() > MINDEDXPOINTS)
        {
            val[NumPart][0] = track->GetP();
            val[NumPart][1] = track->GetTmeanCharge();
            NumPart++;
        }
    }
    index += MAXPART;
    printf("Number of particles is %d\n",NumPart);

////// Check if Minuit is initialized /////////////////////////////


   printf("Create myMinuit\n");
   if(myMinuit == NULL)
   {
       myMinuit = new TMinuit();
       printf("Init Minuit\n");
       myMinuit->mninit(5,6,7);
       printf("Call SetFCN\n");
       myMinuit->SetFCN(TestFunc);
    }

   printf("Start event by event dedx fit\n");
   myMinuit->mncler();
   myMinuit->mnparm(1,"rypr",0.1,0.01,0.0,1.0,ErrFlag);
   myMinuit->mnparm(2,"ryka",0.1,0.01,0.0,1.0,ErrFlag);
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

   TString str;
   myMinuit->mnpout(1,str,par,we,al,bl,ErrFlag);
   //myMinuit->mnpout(1,"rypr",par,we,al,bl,ErrFlag);
   *p2pi = par;
   myMinuit->mnpout(2,str,par,we,al,bl,ErrFlag);
   //myMinuit->mnpout(2,"ryka",par,we,al,bl,ErrFlag);
   *k2pi = par;

   myMinuit->mnexcm("CLEAR",arglist,0,ErrFlag);
 
   printf("Finished event by event dedx fit, found %g %g\n",pNum,kNum);

}

Double_t T49Dedx::GetMinIon()
{
   return MinIon;
}

Double_t T49Dedx::GetResolution()
{
   return Resolution;
}

void T49Dedx::SetMinIon(Double_t x)
{
   MinIon = x;
}

void T49Dedx::SetResolution(Double_t x)
{
   Resolution = x;
}

void TestFunc(Int_t &npar, Double_t *gin, Double_t &f, Double_t *u, Int_t flag)
{
    Int_t i;
    Int_t j;
    Double_t Const[3];
    Double_t Mean[3];
    Double_t Sigma[3];
    Double_t DedxProb[3];
    Double_t MomProb[3];
    Double_t Mom[5];
    Double_t SumProb;
    Double_t prob;
    Double_t pPar[5] = {2.46E2,-9.706E-1,3.1679E-1,0,0};
    Double_t kPar[5] = {2.98385E2,-3.14546E1,6.72776E-1,3.66295E-1,-1.19806E-2};
    Double_t piPar[5] ={-1.3874E3,1.68399E3,-1.73976E2,6.25518,-7.50058E-2};
    Double_t MomNorm[3] = {15695.7,30905.2,105105.2}; 
    
    //Int_t index = 0;
    //static TMinuit *myMinuit = NULL;

    Const[0] = u[0];
    Const[1] = u[1];
    Const[2] = 1.0 - (Const[0]+Const[1]);
 
    SumProb = 0;
    for(i=0;i<NumPart;i++)
    {
        Mom[0] = 1;
        for(j=1;j<5;j++)
           Mom[j] = Mom[j-1] * val[i][0];
        Mean[0] = MinIon * RelRise((Float_t) Mom[1],(Float_t) kPMASS);
        Mean[1] = MinIon * RelRise((Float_t) Mom[1],(Float_t) kKMASS);
        Mean[2] = MinIon * RelRise((Float_t) Mom[1],(Float_t) kPIMASS);
        MomProb[0] = 0;
        for(j=0;j<5;j++)
            MomProb[0] += pPar[j]*Mom[j];
        MomProb[1] = 0;
        for(j=0;j<5;j++)
            MomProb[1] += kPar[j]*Mom[j];
        MomProb[2] = 0;
        for(j=0;j<5;j++)
            MomProb[2] += piPar[j]*Mom[j];
        prob = 0;
//        printf("p,dedx = %g,%g\n", Mom[1],val[i][1]);
        for(j=0;j<3;j++)
        {
            Sigma[j] = Mean[j] * (Resolution + 0.001 * Mom[1]);
            DedxProb[j] = Norm/Sigma[j] * 
                        exp(-(val[i][1]-Mean[j])*(val[i][1]-Mean[j])/(2.0*Sigma[j]*Sigma[j]));
            
            MomProb[j] /= MomNorm[j];
//            printf("MomProb, DedxProb %d: %g - %g \n",j, MomProb[j],DedxProb[j]);
            prob += MomProb[j] * DedxProb[j] * Const[j];
        }
        prob /= (Const[0]+Const[1]+Const[2]);
        if(prob > 0)
            SumProb  -= log(prob);  
    }
    f = SumProb;

}

void T49Dedx::FitPRatio(Double_t Pmin, Double_t Pmax)
{
  //char CutStr[256];
  //char DrawStr[256];
  //char hname[256];
  //Double_t p;
  //Double_t step = 2;
  //Double_t ratios[5];

}

Float_t T49Dedx::GetMultCorr(Int_t Mult, Float_t x)
{

    Float_t slope = -0.032 * (1 - 0.7 * fabs(x)/350);

    return 1 - ((slope * (Mult-654))/420);
}

void T49Dedx::GetDedxPred(T49Particle *track, Float_t mass, Float_t *pos, Float_t *sigma)
{
    Float_t res;

    if(track->GetCharge() > 0)
    {
        *pos = GetRelRise(track->GetP(),mass)*fMinIonPos *(1 + 0.0008*track->GetP());
    }
    else
    {
        *pos = GetRelRise(track->GetP(),mass)*fMinIonNeg *(1 + 0.0005*track->GetP());
    }
    res    = pow(track->GetNDedxPoint(),0.43);
    *sigma = *pos * (fRes0/res + fRes1 * track->GetP());
}

void T49Dedx::ReadCalib(Int_t RunNumber)
{
    
     Char_t FileName[512];

     sprintf(FileName,"%s/%d_dedx.root",fCalibHome,RunNumber);
     
     printf("Read calibration for run %d from %s\n",RunNumber,FileName);

     TFile *f = new TFile(FileName);
     if(!f)
     {
          fprintf(stderr,"Can't open file %s\n",FileName);
     }
     else
     {
          TProfile *h1 = (TProfile *) f->Get("dedx_vs_num_p");
          TProfile *h2 = (TProfile *) f->Get("dedx_vs_num_n");
      
          h1->Fit("pol1","0Q");
          TF1 *f1 = h1->GetFunction("pol1");

          h2->Fit("pol1","0Q");
          TF1 *f2 = h2->GetFunction("pol1");

          fCalibPos0 = f1->GetParameter(0);
          fCalibPos1 = f1->GetParameter(1);
          fCalibNeg0 = f2->GetParameter(0);
          fCalibNeg1 = f2->GetParameter(1);
          printf("Found Calibration Parameters %g,%g and %g,%g\n",fCalibPos0,fCalibPos1,fCalibNeg0,fCalibNeg1);
     }
     f->Close();
}

void T49Dedx::DoEventCalib(T49EventRoot *event, Bool_t DoMultCorr)
{
    T49Particle *track;
    Float_t x;
    Float_t y;
    //Float_t z;
    Float_t dx;
    Float_t dy;
    Float_t dz;

    Float_t dE;
    //Int_t   i;
    Int_t   mult;
    //Int_t   max;
    Float_t   kZCenter = 550;

    static Int_t OldRun = -1;

//  Primitve rejection of MC runs

    if(event->GetNRun() < 100)
    {
        return;
    }

    if(event->GetNRun() != OldRun)
    {
        ReadCalib(event->GetNRun());
        OldRun = event->GetNRun();
    }
    Float_t corr_pos = 400/(fCalibPos0 + fCalibPos1 * event->GetNEvent());
    Float_t corr_neg = 400/(fCalibNeg0 + fCalibNeg1 * event->GetNEvent());

    mult  = event->GetNParticles();
    //T49ParticleIter *iter = event->GetParticleIter();
    if(DoMultCorr)
    {
      //while((track = (T49Particle *) iter->Next()))
      //{
        TClonesArray *ParticleList = event->GetPrimaryParticles();
        for (Int_t iTrack = 0; iTrack < ParticleList->GetEntries(); iTrack++) {
            track = (T49Particle *) ParticleList->UncheckedAt(iTrack);
            if(track->GetCharge() > 0)
                track->SetTmeanCharge(track->GetTmeanCharge()*corr_pos,0);
            else
                track->SetTmeanCharge(track->GetTmeanCharge()*corr_neg,0);

            dx = track->GetXLast()-track->GetXFirst();
            dy = track->GetYLast()-track->GetYFirst();
            dz = track->GetZLast()-track->GetZFirst();
            x  = track->GetXFirst() + dx/dz*(kZCenter - track->GetZFirst());
            y  = track->GetYFirst() + dy/dz*(kZCenter - track->GetZFirst());
            dE = track->GetTmeanCharge() * GetMultCorr(mult,x);
            track->SetTmeanCharge(dE,0);
        }
    }
    else
    {
      //while((track = (T49Particle *) iter->Next()))
      //{
        TClonesArray *ParticleList = event->GetPrimaryParticles();
        for (Int_t iTrack = 0; iTrack < ParticleList->GetEntries(); iTrack++) {
            track = (T49Particle *) ParticleList->UncheckedAt(iTrack);
            if(track->GetCharge() > 0)
                track->SetTmeanCharge(track->GetTmeanCharge()*corr_pos,0);
            else
                track->SetTmeanCharge(track->GetTmeanCharge()*corr_neg,0);
        }
    }
}

void T49Dedx::EventFit(Int_t Num)
{
    //Int_t     j;
    Int_t     i;
    Int_t     ErrFlag;
    //Float_t  *row;
    Float_t  row[7] = { 0 };
    Double_t arglist[10];
    Double_t we, al, bl;
    Double_t pNum = 0;
    Double_t kNum = 0;
    Double_t par;
    const Double_t Pmin = 5;
    const Double_t Pmax = 25;
    static Int_t index = 0;
    //T49Particle  *track;

    //Double_t kPar[5] = {1.0443E2,2.0724E1,-4.067,-5.349E-1,-1.3876E-2};
    //Double_t pPar[5] = {2.46E2,-9.706E-1,3.1679E-1,0,0};
    //Double_t piPar[5] ={-1.3874E3,1.68399E3,-1.73976E2,6.25518,-7.50058E-2};
    //Double_t pNorm  = 15695.7; 
    //Double_t kNorm  = 30905.2; 
    //Double_t piNorm = 105105.2; 
    Double_t k2pi;
    Double_t p2pi;
    
    static TMinuit *myMinuit = NULL;

    NumPart = 0;
    for(i=0;i<Num;i++) 
    {
        if(row[0] > Pmin && 
           row[0] < Pmax &&
           row[4] > 300 && 
           row[4] < 500 && 
           row[6] > MINDEDXPOINTS)
        {
            val[NumPart][0] = row[0];
            val[NumPart][1] = row[4];
            NumPart++;
        }
    }
    index += Num;    
    printf("Number of particles is %d\n",NumPart);

////// Check if Minuit is initialized /////////////////////////////


   printf("Create myMinuit\n");
   if(myMinuit == NULL)
   {
       myMinuit = new TMinuit();
       printf("Init Minuit\n");
       myMinuit->mninit(5,6,7);
       printf("Call SetFCN\n");
       myMinuit->SetFCN(TestFunc);
    }

   printf("Start event by event dedx fit\n");
   myMinuit->mncler();
   myMinuit->mnparm(1,"rypr",0.1,0.01,0.0,1.0,ErrFlag);
   myMinuit->mnparm(2,"ryka",0.1,0.01,0.0,1.0,ErrFlag);
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

   TString str;
   myMinuit->mnpout(1,str,par,we,al,bl,ErrFlag);
   //myMinuit->mnpout(1,"rypr",par,we,al,bl,ErrFlag);
   p2pi = par;
   myMinuit->mnpout(2,str,par,we,al,bl,ErrFlag);
   //myMinuit->mnpout(2,"ryka",par,we,al,bl,ErrFlag);
   k2pi = par;

   myMinuit->mnexcm("CLEAR",arglist,0,ErrFlag);
 
   printf("Finished event by event dedx fit, found %g %g\n",pNum,kNum);

}

Float_t RelRise(Float_t mom, Float_t mass)
{
	Float_t epsilon; 		/**..dE/dx..**/ 
	Float_t zeta, K, Q;             /**..Target density,atomic shell, **/
        Float_t delta = 0;              /**.. charge,density corr...**/
	Float_t gamma, beta;
	Float_t p1, p2;		/**..Pressure in TPC in bar..**/
	Float_t a, m, X1, X0, XA;	/**..XA determines plateau height..**/
	Float_t epsmin, logbg, X;
        //Float_t betagamma, 

        Float_t corr;
        Int_t   sign;

        if(mom < 0)
            sign = -1;
        else
            sign = 1;

        mom = fabs(mom);

	m = 3.0;	/**..for density correction..**/
	Q = 1.0;
	p1 = 4.0;
	p2 = 1.0;
	epsmin = 30.0;

/***............Constants measured for Ar(88%)CH4(9.4%)iso-C4H10(2.6%) with JADE...........***/

	zeta = 0.42;
	K = 13.85;
	XA = 2.28;
	a = 0.26;


/***............Constants measured for Ar(88%)CH4(9.4%)iso-C4H10(2.6%) with OPAL...........

	zeta = 0.5;
	K = 11.3;
	XA = 2.28;
	a = 0.19;
		...***/

/***............Pressure dependencies...........***/

	zeta = zeta * (p2/p1);
	K = K + log(p2/p1);
	XA = XA - 0.5*log10(p2/p1);

	X0 = 1.771087313;
	X1 = 4.200915361;





/***............Defines unnormalized dE/dx as function of beta*gamma or mom/mass...........***/

		X = mom/mass;
		beta = sqrt((X*X)/(1+X*X));
		gamma = sqrt(1 + X*X);

		if(beta*gamma > 0.0)
		{
		logbg = log10(beta*gamma);
		if(logbg < X0)
		{
			delta = 0;
		}
		else if(X0 < logbg && logbg < X1)
		{
			delta = 2.0*log(10.0)*(logbg-XA) + a*(pow(X1-logbg,m));
		}
		else if(X1 < logbg)
		{
			delta = 2.0*log(10.0) * (logbg-XA);
		}

		epsilon = zeta * (Q*Q/(beta*beta))
			       * ( K + log(Q*Q)
			       + log(gamma*gamma) 
			       - beta*beta - delta);
		}
		else
			epsilon = 0.0;
		
                if(mom > 5 && mom < 40)
                {
                    if(sign < 0)
                    {
                       corr = 9.20540e-01 + 
                           2.19456e-02 * mom +
                          -2.31632e-03 * mom * mom +
                           1.12909e-04 * mom * mom * mom +
                          -2.53639e-06 * mom * mom * mom * mom + 
                           2.11768e-08 * mom * mom * mom * mom * mom ;
                    }
                    else
                    {
                       corr = 9.54256e-01  + 
                           7.81015e-03 * mom +
                          -3.57403e-04 * mom * mom +
                           4.44680e-06 * mom * mom * mom;
                    }
               }
               else
                   corr = 1;

                

		return(epsilon * DEDXNORM / corr);
}

