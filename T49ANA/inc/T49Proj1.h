/*
 *
 * AUTOR 
 * Thomas Sammer
 * ( Thomas.Sammer@cern.ch )
 *
 * ERSTELLUNGSDATUM
 * 10.01.1999
 *
 * BESCHREIBUNG
 * Klasse zur Histogrammierung von Variablen eines Teilchens
 *
 * SYNOPSIS
 * 
 * 
 */

#ifndef __T49Proj1BASE__
#define __T49Proj1BASE__

/*
$Log: T49Proj1.h,v $
Revision 1.6  2008/01/04 10:48:58  cblume
64bit modifications by Marek Szuba

Revision 1.5  2005/05/20 11:28:17  cblume
Get rid of deprecated warnings

Revision 1.4  1999/12/01 09:39:21  cblume
Changed to root 2.23-09. Updated the makefiles. Changes in T49CutTrack, T49Dedx, T49Proj1, T49Proj2, TRootDS

 * Revision 1.3  1999/11/24  16:03:38  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 *
 * Revision 1.2  1999/11/23  13:52:18  cblume
 * Remove files
 *
*/

#include <iostream>
using namespace std;

#include "TFile.h"
#include "TClass.h"
#include "TKey.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TAxis.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

#include "T49ParticleRoot.h"
#include "PhysConst.h"

/* ============================================ BASE CLASS ============================================ */
/* ====================== T49Proj1BASE ====================== */

class T49Proj1BASE : public TObjArray

{

 protected:

    Int_t    fTRUE;
    Int_t    fFALSE;
    Int_t    fVerbose;

    Int_t    fN_Array;
    Int_t    fNMax_XBins;
    Int_t    fNMax_YBins;
    Int_t    fDim;  

    Float_t  fX_Range[4], fY_Range[4]; // fN_Array = 4
    Int_t    fNX[4], fNY[4];
    Float_t  fX_BinSize[4], fY_BinSize[4];

    Int_t    fNXBin[4];
    Float_t  fXMin[4], fXMax[4]; 

    char     fGenPName[80];
    char     fGenNName[80];
    char     fHistName[80];
    char     fHistTitle[80];
    char     fHistLabelA1[80];
    char     fHistLabelA2[80];

    TRandom  x;

    TObjArray *fHistArray[4];

 public:

    T49Proj1BASE();
    virtual      ~T49Proj1BASE() {};

    void         Book(Int_t n_x, Float_t x_min, Float_t x_max, Int_t n_y, Float_t y_min, Float_t y_max,
		      Int_t nx, Float_t xmin, Float_t xmax,
		      Int_t ny = 1, Float_t ymin = 0.0, Float_t ymax = 0.0,
		      Int_t nz = 1, Float_t zmin = 0.0, Float_t zmax = 0.0);
    void         BookHist1DXY(Int_t n_array, Int_t n_x, Float_t x_min, Float_t x_max, 
			      Int_t n_y, Float_t y_min, Float_t y_max,
			      Int_t nx, Float_t xmin, Float_t xmax); 
    void         BookHist1D(char *HistName, Int_t n_array,
			    Int_t nx, Float_t xmin, Float_t xmax); 
    void         BookHist2D(char *HistName, Int_t n_array,
			    Int_t nx, Float_t xmin, Float_t xmax, 
			    Int_t ny, Float_t ymin, Float_t ymax); 
    void         BookHist3D(char *HistName, Int_t n_array,
			    Int_t nx, Float_t xmin, Float_t xmax, 
			    Int_t ny, Float_t ymin, Float_t ymax, 
			    Int_t nz, Float_t zmin, Float_t zmax); 
    // if necessary add other special cases (BookHist1D/...)

    void         Fill(TObjArray *ParticleList);
    void         Fill(TFile *file, Int_t n_x, Int_t n_y);
    void         FillHist1DXY(Int_t n_array, Int_t Charge, Float_t XValue, Float_t YValue, Float_t FillValue);
    void         FillHistXD(char *name, Int_t n_array, Float_t XValue, Float_t YValue = 0.0, Float_t ZValue = 0.0);
    // if necessary add other special cases (FillHist1D/...)

    void         WriteH();
    void         WriteHistXD(Int_t n_array);
    // if necessary add other special cases (WriteHist1D/...)

    virtual void BookHist(Int_t n_x, Float_t x_min, Float_t x_max, Int_t n_y, Float_t y_min, Float_t y_max,
			  Int_t nx, Float_t xmin, Float_t xmax,
			  Int_t ny = 1, Float_t ymin = 0.0, Float_t ymax = 0.0,
			  Int_t nz = 1, Float_t zmin = 0.0, Float_t zmax = 0.0)=0;
    virtual void FillHist(T49ParticleRoot *Particle)=0;
    virtual void FillHist(TFile *file, Int_t n_x, Int_t n_y)=0;
    virtual void WriteHist()=0;

    void         SetVerbose(Int_t n) { fVerbose = n; }

    ClassDef(T49Proj1BASE,0)  // One-particle projection base class

};


/* ============================================ INHERITED CLASSES ============================================ */
/* ====================== T49Proj1Dedx ====================== */
class T49Proj1Dedx : public T49Proj1BASE
{

 protected:

    void BookHist(Int_t n_x, Float_t x_min, Float_t x_max, Int_t n_y, Float_t y_min, Float_t y_max,
		  Int_t nx,  Float_t xmin, Float_t xmax,
		  Int_t ny = 1, Float_t ymin = 0.0, Float_t ymax = 0.0,
		  Int_t nz = 1, Float_t zmin = 0.0, Float_t zmax = 0.0)
    { 

	if(fVerbose == 2)
	    printf("Calling booking routines. \n");

        // dE/dx histograms
        //sprintf(fHistName, "%s", "Dedx");
 	BookHist1DXY(0, n_x, x_min, x_max, n_y, y_min, y_max, nx, xmin, xmax);

        // p histograms
        sprintf(fHistName, "%s", "P");
	BookHist1DXY(1, n_x, x_min, x_max, n_y, y_min, y_max, nx, 0.0, 180.0);

        // pt histograms
        sprintf(fHistName, "%s", "Pt");
	BookHist1DXY(2, n_x, x_min, x_max, n_y, y_min, y_max, nx, y_min, y_max);

        // pt histograms
        sprintf(fHistName, "%s", "Np");
	BookHist1DXY(3, n_x, x_min, x_max, n_y, y_min, y_max, 150, 0.0, 150.0);

    }

    void FillHist(T49ParticleRoot *Particle)
    {

	Int_t   ParticleCharge;
	Float_t ParticleP, ParticleLogP, ParticlePt, ParticleDedx, ParticleNp;

	ParticleCharge = Particle->GetCharge();
	ParticleP      = Particle->GetP();
	ParticleLogP   = log10(ParticleP);
	ParticlePt     = Particle->GetPt();
	ParticleDedx   = Particle->GetTmeanCharge(2)/1000.0;
	ParticleNp     = Particle->GetNDedxPoint(2);

	//if(fVerbose == 2)
	//    printf("Calling filling routines. \n");

        FillHist1DXY(0, ParticleCharge, ParticleLogP, ParticlePt, ParticleDedx);
        FillHist1DXY(1, ParticleCharge, ParticleLogP, ParticlePt, ParticleP);
        FillHist1DXY(2, ParticleCharge, ParticleLogP, ParticlePt, ParticlePt);
        FillHist1DXY(3, ParticleCharge, ParticleLogP, ParticlePt, ParticleNp);

    }

    void FillHist(TFile *file, Int_t n_x, Int_t n_y)
    {
	printf("Virtual function not used in this class. \n");
    }

    void WriteHist()
    {

	if(fVerbose == 2)
	    printf("Calling writing routines. \n");

	WriteHistXD(0);
	WriteHistXD(1);
	WriteHistXD(2);
	WriteHistXD(3);

    }

 public:

    T49Proj1Dedx(char* name, char* title) 
    {
        fVerbose = 0;
	fN_Array = 4;
	fDim     = 1;
	for(Int_t i=0; i<fN_Array; i++)
	{
	    fX_Range[i] = 0.0; fY_Range[i] = 0.0;
	}
	sprintf(fHistName, "%s", name);
	sprintf(fHistTitle, "%s", title);
    }
    ~T49Proj1Dedx(){}

  ClassDef(T49Proj1Dedx,1) // One-particle projection class for dE/dx-analysis

};

/* ====================== T49Proj1Prob ====================== */
class T49Proj1Prob : public T49Proj1BASE
{

 protected:

    void BookHist(Int_t n_x, Float_t x_min, Float_t x_max, Int_t n_y, Float_t y_min, Float_t y_max,
		  Int_t nx,  Float_t xmin, Float_t xmax,
		  Int_t ny = 1, Float_t ymin = 0.0, Float_t ymax = 0.0,
		  Int_t nz = 1, Float_t zmin = 0.0, Float_t zmax = 0.0)
    { 

	Int_t PID;
	char *HistName = "";

	if(fVerbose == 2)
	    printf("Calling booking routines. \n");

	for(PID=0; PID<5; PID++)
	{

	  // amplitude (p- and pt-dependence)
	  sprintf(HistName, "%sPID%d", "AmpPPt", PID);
	  BookHist3D(HistName, 0, nx, xmin, xmax, ny, ymin, ymax, nz, zmin, zmax);

          // amplitude (p-dependence)
	  sprintf(HistName, "%sPID%d", "AmpP", PID);
	  BookHist2D(HistName, 0, nx, xmin, xmax, nz, zmin, zmax);

          // amplitude (pt-dependence)
          sprintf(HistName, "%sPID%d", "AmpPt", PID);
	  BookHist2D(HistName, 0, ny, ymin, ymax, nz, zmin, zmax);

          // mean dE/dx (p- and pt-dependence)
          sprintf(HistName, "%sPID%d", "IonPPt", PID);
	  BookHist3D(HistName, 0, nx, xmin, xmax, ny, ymin, ymax, 100, 0.8, 1.8);

	  // sigma (p- and pt-dependence)
	  sprintf(HistName, "%sPID%d", "SigPPt", PID);
	  BookHist3D(HistName, 0, nx, xmin, xmax, ny, ymin, ymax, 100, 0.03, 0.09);

	}

    }

    void FillHist(T49ParticleRoot *Particle)
    {
	printf("Virtual function not used in this class. \n");
    }

    void FillHist(TFile *file, Int_t n_x, Int_t n_y)
    {

	Int_t   i, j, n_array; //, PID;
	Float_t XValue = 0;
        Float_t YValue = 0;
        Float_t ZValue = 0;
	Float_t P, Pt;
	//Float_t dEdx[4];
	//Float_t M_Elek, M_Pion, M_Kaon, M_Prot, D_Elek, D_Pion, D_Kaon, D_Prot;
	//Float_t Limit_Fit;
	char hNameGen[80];
	char hHistName[80];
	//char fkttext[800];
	TH1F *histogram;

	n_array = 0;
	for(i=0; i<n_x; i++)
	{
	  for(j=0; j<n_y; j++)
	  {
// noch nicht fertig
           // pos. particles
	    sprintf(hNameGen,"%sA%dX%dY%d", fGenPName, 1, i, j);
	    histogram = (TH1F *) file->Get(hNameGen);
	    P = histogram->GetMean();

	    sprintf(hNameGen,"%sA%dX%dY%d", fGenPName, 2, i, j);
	    histogram = (TH1F *) file->Get(hNameGen);
	    Pt = histogram->GetMean();

	    //for(PID=0; PID<4; PID++)
	      //ParamBB(P, PID, &dEdx);

	    //sprintf(fkttext,"[0]*exp(-1*((x-[1])*(x-[1]))/(2*[1]*[1]*[2]))+[3]*exp(-1*((x-([1]*%f))*(x-([1]*%f)))/(2*([1]*%f)*([1]*%f)*[2]))+[4]*exp(-1*((x-([1]*%f))*(x-([1]*%f)))/(2*([1]*%f)*([1]*%f)*[2]))+[5]*exp(-1*((x-([1]*%f))*(x-([1]*%f)))/(2*([1]*%f)*([1]*%f)*[2]))",D_Kaon,D_Kaon,D_Kaon,D_Kaon,D_Prot,D_Prot,D_Prot,D_Prot,D_Elek,D_Elek,D_Elek,D_Elek);
	    //TF1 *funktion = new TF1("funktion",fkttext,-Limit_Fit,Limit_Fit);

	    sprintf(hNameGen,"%sA%dX%dY%d", fGenPName, 0, i, j);
	    histogram = (TH1F *) file->Get(hNameGen);
	    histogram->Fit("funktion","BR");

	    sprintf(hHistName,"%sA%d", fGenPName, 0);
            FillHistXD(hHistName, n_array, XValue, YValue, ZValue);

            // neg. particles

	    sprintf(hNameGen,"%sA%dX%dY%d", fGenNName, 0, i, j);
	    histogram = (TH1F *) file->Get(hNameGen);
	    histogram->Fit("funktion","BR");

	    sprintf(hHistName,"%sA%d", fGenNName, 0);
            FillHistXD(hHistName, n_array, XValue, YValue, ZValue);

	  }
	}

    }

    void WriteHist()
    {

	if(fVerbose == 2)
	    printf("Calling writing routines. \n");

	WriteHistXD(0);

    }

 public:

    T49Proj1Prob(char* name, char* title) 
    {
        fVerbose = 0;
	fN_Array = 4;
	fDim     = 1;
	for(Int_t i=0; i<fN_Array; i++)
	{
	    fX_Range[i] = 0.0; fY_Range[i] = 0.0;
	}
	sprintf(fHistName, "%s", name);
	sprintf(fHistTitle, "%s", title);
    }
    ~T49Proj1Prob(){}

  ClassDef(T49Proj1Prob,1) // One-particle projection class for pid-propabilities


};

#endif

