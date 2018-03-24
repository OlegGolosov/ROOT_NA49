///////////////////////////////////////////////////////////////////////
//
//
// AUTOR 
// Thomas Sammer
// ( Thomas.Sammer@cern.ch )
//
// ERSTELLUNGSDATUM
// 10.01.1999
//
// BESCHREIBUNG
// Klasse zur Histogrammierung von Variablen eines Teilchens
//
// SYNOPSIS
// 
// 
//
///////////////////////////////////////////////////////////////////////

#include "T49Proj1.h"


ClassImp(T49Proj1BASE)
ClassImp(T49Proj1Dedx)
ClassImp(T49Proj1Prob)

/*
$Log: T49Proj1.C,v $
Revision 1.4  2005/02/08 10:16:08  flierl
Default arguments must only be given in the header files

Revision 1.3  1999/11/24 16:03:48  cblume
Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.

 * Revision 1.2  1999/11/23  13:54:13  cblume
 * Remove files
 *
*/

T49Proj1BASE::T49Proj1BASE()
{

  fTRUE    = 1;
  fFALSE   = 0;
  fVerbose = 0;

  fN_Array = 4;
  fNMax_XBins = 100;
  fNMax_YBins = 100;
  fDim = 0;

  for(Int_t i=0; i<fN_Array; i++)
  {

    fX_Range[i] = fY_Range[i] = 0.0;
    fNX[i] = fNY[i] = 1;
    fX_BinSize[i] = fX_BinSize[i] = 0.0;

    fNXBin[i] = 1;
    fXMin[i]  = fXMax[i] = 0.0;

    fHistArray[i] = new TObjArray();

  }

  sprintf(fGenPName, "hHistArrayP");
  sprintf(fGenNName, "hHistArrayN");
  sprintf(fHistLabelA1, "%s", fHistTitle);
  sprintf(fHistLabelA2, "N");

}


void T49Proj1BASE::Book(Int_t n_x, Float_t x_min, Float_t x_max, Int_t n_y, Float_t y_min, Float_t y_max,
			Int_t nx,  Float_t xmin, Float_t xmax,
			Int_t ny , Float_t ymin , Float_t ymax ,
			Int_t nz , Float_t zmin , Float_t zmax )
{ 

  if(n_x > fNMax_XBins || n_x < 0)
  {
    printf(" Invalid number of bins in X-dimension. \n");
    return;
  }
  if(n_y > fNMax_YBins || n_y < 0)
  {
    printf(" Invalid number of bins in Y-dimension. \n");
    return;
  }

  BookHist(n_x, x_min, x_max, n_y, y_min, y_max, nx, xmin, xmax, ny, ymin, ymax, nz, zmin, zmax);

}


void T49Proj1BASE::BookHist1DXY(Int_t n_array, Int_t n_x, Float_t x_min, Float_t x_max,
				Int_t n_y, Float_t y_min, Float_t y_max,
				Int_t nx, Float_t xmin, Float_t xmax)		     
{

  // booking of 1D histograms for positive and negative particles in bins of x and y (e.g. p and pt bins)

  Int_t      i, j;

  TH1F       *hHistArrayP[fNMax_XBins][fNMax_YBins];
  TH1F       *hHistArrayN[fNMax_XBins][fNMax_YBins];
  char       hNameGen[80];
  char       hNameTit[80];

  if(n_array > fN_Array || n_array < 0)
  {
    printf("Invalid array number. \n");
    return;
  }

  fNX[n_array] = n_x;   
  fNY[n_array] = n_y;
  fX_Range[n_array] = x_max - x_min;
  fY_Range[n_array] = y_max - y_min;
  fX_BinSize[n_array] = fX_Range[n_array] / fNX[n_array];
  fY_BinSize[n_array] = fY_Range[n_array] / fNY[n_array];

  fNXBin[n_array] = nx; 
  fXMin[n_array]  = xmin; fXMax[n_array] = xmax;

  if(fVerbose)
    printf("Booking HistArray[%d]. \n", n_array);

  for(i=0; i<n_x; i++)
  {
    for(j=0; j<n_y; j++)
    {
      sprintf(hNameGen,"%sA%dX%dY%d", fGenPName, n_array, i, j);
      sprintf(hNameTit,"%s%sA%dX%dY%d", fGenPName, fHistName, n_array, i, j);
      //sprintf(hNameTit,"%sA%dX%dY%d", fGenPName, n_array, i, j);
      hHistArrayP[i][j] = new TH1F(hNameGen, hNameTit, nx, xmin, xmax);
      hHistArrayP[i][j]->SetXTitle(fHistLabelA1);
      hHistArrayP[i][j]->SetYTitle(fHistLabelA2);
      fHistArray[n_array]->Add(hHistArrayP[i][j]);
      if(fVerbose)
	printf("Histogram %s booked. \n" , hNameGen);

      sprintf(hNameGen,"%sA%dX%dY%d", fGenNName, n_array, i, j);
      sprintf(hNameTit,"%s%sA%dX%dY%d", fGenNName, fHistName, n_array, i, j);
      //sprintf(hNameTit,"%sA%dX%dY%d", fGenNName, n_array, i, j);
      hHistArrayN[i][j] = new TH1F(hNameGen, hNameTit, nx, xmin, xmax);
      hHistArrayN[i][j]->SetXTitle(fHistLabelA1);
      hHistArrayN[i][j]->SetYTitle(fHistLabelA2);
      fHistArray[n_array]->Add(hHistArrayN[i][j]);
      if(fVerbose)
	printf("Histogram %s booked. \n" , hNameGen);
    }
  }

}


void T49Proj1BASE::BookHist1D(char *HistName, Int_t n_array,
			      Int_t nx, Float_t xmin, Float_t xmax)
{

  char       hNameGen[80];
  char       hNameTit[80];
  TH1F       *histogram;

  sprintf(hNameGen, "%s%sA%d", fGenPName, HistName, n_array);
  sprintf(hNameTit, "%s%sA%d", fGenPName, HistName, n_array);
  histogram = new TH1F(hNameGen, hNameTit, nx, xmin, xmax);
  fHistArray[n_array]->Add(histogram);
  if(fVerbose)
    printf("1D Histogram %s booked. \n" , hNameGen);

  sprintf(hNameGen, "%s%sA%d", fGenNName, HistName, n_array);
  sprintf(hNameTit, "%s%sA%d", fGenNName, HistName, n_array);
  histogram = new TH1F(hNameGen, hNameTit, nx, xmin, xmax);
  fHistArray[n_array]->Add(histogram);
  if(fVerbose)
    printf("1D Histogram %s booked. \n" , hNameGen);

}


void T49Proj1BASE::BookHist2D(char *HistName, Int_t n_array,
			      Int_t nx, Float_t xmin, Float_t xmax, 
			      Int_t ny, Float_t ymin, Float_t ymax)
{

  char       hNameGen[80];
  char       hNameTit[80];
  TH2F       *histogram;

  sprintf(hNameGen, "%s%sA%d", fGenPName, HistName, n_array);
  sprintf(hNameTit, "%s%sA%d", fGenPName, HistName, n_array);
  histogram = new TH2F(hNameGen, hNameTit, nx, xmin, xmax, ny, ymin, ymax);
  fHistArray[n_array]->Add(histogram);
  if(fVerbose)
    printf("2D Histogram %s booked. \n" , hNameGen);

  sprintf(hNameGen, "%s%sA%d", fGenNName, HistName, n_array);
  sprintf(hNameTit, "%s%sA%d", fGenNName, HistName, n_array);
  histogram = new TH2F(hNameGen, hNameTit, nx, xmin, xmax, ny, ymin, ymax);
  fHistArray[n_array]->Add(histogram);
  if(fVerbose)
    printf("2D Histogram %s booked. \n" , hNameGen);

}


void T49Proj1BASE::BookHist3D(char *HistName, Int_t n_array,
			      Int_t nx, Float_t xmin, Float_t xmax, 
			      Int_t ny, Float_t ymin, Float_t ymax, 
			      Int_t nz, Float_t zmin, Float_t zmax)
{

  char       hNameGen[80];
  char       hNameTit[80];
  TH3F       *histogram;

  sprintf(hNameGen, "%s%sA%d", fGenPName, HistName, n_array);
  sprintf(hNameTit, "%s%sA%d", fGenPName, HistName, n_array);
  histogram = new TH3F(hNameGen, hNameTit, nx, xmin, xmax, ny, ymin, ymax, nz, zmin, zmax);
  fHistArray[n_array]->Add(histogram);
  if(fVerbose)
    printf("2D Histogram %s booked. \n" , hNameGen);

  sprintf(hNameGen, "%s%sA%d", fGenNName, HistName, n_array);
  sprintf(hNameTit, "%s%sA%d", fGenNName, HistName, n_array);
  histogram = new TH3F(hNameGen, hNameTit, nx, xmin, xmax, ny, ymin, ymax, nz, zmin, zmax);
  fHistArray[n_array]->Add(histogram);
  if(fVerbose)
    printf("2D Histogram %s booked. \n" , hNameGen);

}


void T49Proj1BASE::Fill(TObjArray *ParticleList)
{

  // Project list of Particles

  T49ParticleRoot *Particle;

  if(fVerbose == 2)
    printf("Filling Histograms. \n");

  TIter NextParticle(ParticleList);
  while( (Particle = (T49ParticleRoot *) NextParticle()) != 0 )
  {

    FillHist(Particle);

  }

}


void T49Proj1BASE::Fill(TFile *file, Int_t n_x, Int_t n_y)
{ 

  if(fVerbose)
    printf("Filling Histograms. \n");

  FillHist(file, n_x, n_y);

}


void T49Proj1BASE::FillHist1DXY(Int_t n_array, Int_t Charge, Float_t XValue, Float_t YValue, Float_t FillValue)
{ 

  Int_t      i, j;
  char       hNameGen[80];

  i = 0; j = 0;
  while(i<fNX[n_array])
  {
    if( (i*fX_BinSize[n_array] < XValue) && ( XValue < (i+1)*fX_BinSize[n_array]) )
      break;
    i++;
  }
  while(j<fNY[n_array])
  {
    if( (j*fY_BinSize[n_array] < YValue) && ( YValue < (j+1)*fY_BinSize[n_array]) )
      break;
    j++;
  }  

  if(Charge != 1 && Charge != -1)
  {
    if(fVerbose)
      printf("Error - particle has wrong charge information. \n");
  }
  else
  {
    if(Charge == 1)
    {
      sprintf(hNameGen,"%sA%dX%dY%d", fGenPName, n_array, i, j);

      if(fVerbose == 2)
	printf("Filling histogram %s for pos. particle. \n", hNameGen);

      TH1F *histogram = (TH1F *) fHistArray[n_array]->FindObject(hNameGen);
      if(histogram != NULL)
	histogram->Fill(FillValue);
    }
    if(Charge == -1)
    {
      sprintf(hNameGen,"%sA%dX%dY%d", fGenNName, n_array, i, j);

      if(fVerbose == 2)
	printf("Filling histogram %s for neg. particle. \n", hNameGen);

      TH1F *histogram = (TH1F *) fHistArray[n_array]->FindObject(hNameGen);
      if(histogram != NULL)
	histogram->Fill(FillValue);
    }
  }

      /*
      // Alternativloesung zur Histogrammidentifikation
      for (Int_t k=0; k < fHistArray[n_array]->GetEntries(); k++)
      {
	TH1F *histogram = (TH1F *)fHistArray[n_array]->At(k);
	if(strcmp(histogram->GetName(), hNameGen) == 0)
        {                
	  if(fVerbose == 2)
	    printf("Filling histogram %s. \n", hNameGen);
	  histogram->Fill(FillValue);
	}
      }     
      */

}


void T49Proj1BASE::FillHistXD(char *name, Int_t n_array, Float_t XValue, Float_t YValue , Float_t ZValue )
{ 

  if(fVerbose)
    printf("Filling XD Histograms. \n");

}


void T49Proj1BASE::WriteH()
{

  WriteHist();

}


void T49Proj1BASE::WriteHistXD(Int_t n_array)
{

  if(fVerbose)
    printf("Writing HistArray[%d]. \n", n_array);

  fHistArray[n_array]->Write();

}

