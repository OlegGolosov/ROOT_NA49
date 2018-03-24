///////////////////////////////////////////////////////////////////////
//                                                                   //
//    2-dim histogram container class                                //
//                                                                   //
//    Author: Christoph Blume                                        //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "T49Histogram2D.h"

ClassImp(T49Histogram2D)

/*
$Log: T49Histogram2D.C,v $
Revision 1.4  2001/11/12 12:11:15  cblume
Update for ROOT v3.01

 * Revision 1.3  2000/06/05  13:00:56  cblume
 * Use gPad in Draw()
 *
 * Revision 1.2  2000/04/19  12:55:46  cblume
 * Changed some functions to inline and corrected bug in CreateHistograms() and Draw()
 *
 * Revision 1.1  1999/11/23  13:54:59  cblume
 * Add T49Histogram2D.C
 *
*/

//______________________________________________________________________________
T49Histogram2D::T49Histogram2D():T49Histogram() 
{
  //
  // T49Histogram2D default constructor
  //

  Init();

}

//______________________________________________________________________________
T49Histogram2D::T49Histogram2D(const Text_t *name, const Text_t *title)
               :T49Histogram(name,title) 
{
  //
  // T49Histogram2D constructor with name and title
  //

  Init();

}

//______________________________________________________________________________
T49Histogram2D::T49Histogram2D(const Text_t *name, const Text_t *title, TH1 *histo
                              , Int_t nx, Float_t xmin, Float_t xmax
                              , Int_t ny, Float_t ymin, Float_t ymax)
               :T49Histogram(name,title,histo) 
{
  //
  // T49Histogram2D constructor with name, title, and input histogram
  //

  fXN   = nx;
  fXMin = xmin;
  fXMax = xmax;

  fYN   = ny;
  fYMin = ymin;
  fYMax = ymax;

  CreateHistograms();

}

//______________________________________________________________________________
T49Histogram2D::T49Histogram2D(const Text_t *name, const Text_t *title
                              , Int_t n , Float_t  min, Float_t  max
                              , Int_t nx, Float_t xmin, Float_t xmax
                              , Int_t ny, Float_t ymin, Float_t ymax)
               :T49Histogram(name,title,n,min,max) 
{
  //
  // T49Histogram2D constructor with name, title, and histogram definition
  //

  fXN   = nx;
  fXMin = xmin;
  fXMax = xmax;

  fYN   = ny;
  fYMin = ymin;
  fYMax = ymax;

  CreateHistograms();

}

//______________________________________________________________________________
T49Histogram2D::~T49Histogram2D() 
{
  //
  // Destructor
  //

}

//______________________________________________________________________________
void T49Histogram2D::Draw(const Option_t *option) 
{
  //
  // Draw all histograms on one canvas
  //

  Draw(0,fXN,0,fYN);

}

//______________________________________________________________________________
void T49Histogram2D::Draw(Float_t xFirst, Float_t xLast
                        , Float_t yFirst, Float_t yLast) 
{
  //
  // Draw all histograms between the values <x/yFirst> and <x/yLast>
  //  

  Draw(GetIndexX(xFirst),GetIndexX(xLast)+1
      ,GetIndexY(yFirst),GetIndexY(yLast)+1);

}

//______________________________________________________________________________
void T49Histogram2D::Draw(Int_t iFirst, Int_t iLast
                        , Int_t jFirst, Int_t jLast) 
{
  //
  // Draw all histograms between the bins <i/jFirst> and <i/jLast>
  //

  if ((iFirst < 0) || (iLast > fXN)) {
    printf("T49Histogram2D::Draw - Boundary error X %d - %d (%d)\n"
          ,iFirst,iLast,fXN);
    return;
  }
  if ((jFirst < 0) || (jLast > fYN)) {
    printf("T49Histogram2D::Draw - Boundary error X %d - %d (%d)\n"
          ,jFirst,jLast,fYN);
    return;
  }

  TCanvas *C;
  TPad    *P;

  if (!gPad) {
    Text_t *title = fHTitle;
    C = new TCanvas("plot",title,50,50,600,900);
  }
  P = (TPad *) gPad;

  Int_t xdiv = iLast - iFirst;
  Int_t ydiv = jLast - jFirst;
  P->Divide(xdiv,ydiv);

  Int_t iHisto = 0;
  for (Int_t iPlot = iFirst; iPlot < iLast; iPlot++) {
    for (Int_t jPlot = jFirst; jPlot < jLast; jPlot++) {

      Int_t iPad = iPlot + 1 + jPlot * xdiv;
      P->cd(iPad);
      if (fLogy) gPad->SetLogy();
      TH1D *H = (TH1D*) fHisto->At(iHisto);    
      H->Draw();

      iHisto++;

    }
  }

}

//______________________________________________________________________________
void T49Histogram2D::Print(const Option_t *option) const 
{
  //
  // Print the parameter
  //

  T49Histogram::Print(option);
  printf("               Number of bins = %d / %d\n",fXN,fYN);
  printf("                 Lower border = %g / %g\n",fXMin,fYMin);
  printf("                 Upper border = %g / %g\n",fXMax,fYMax);
  printf("                    Bin width = %g / %g\n",GetWidthX(),GetWidthY());
  printf("\n");

}

//______________________________________________________________________________
void T49Histogram2D::SetDimensions(Int_t nx, Float_t xmin, Float_t xmax
			         , Int_t ny, Float_t ymin, Float_t ymax) 
{
  //
  // Defines the number of bins <nx/ny> and the borders <xmin/ymin> 
  // <xmax/ymax> of the histogram array
  //

  if (fXN) {
    printf("T49Histogram2D::SetDimensions - Dimensions are already defined.\n");
  }
  else {
    fXN   = nx;
    fXMin = xmin;
    fXMax = xmax;
    fYN   = ny;
    fYMin = ymin;
    fYMax = ymax;
  }

}

//______________________________________________________________________________
void T49Histogram2D::Init() 
{

  fXN   = 0;
  fXMin = 0;
  fXMax = 0;

  fYN   = 0;
  fYMin = 0;
  fYMax = 0;

}

//______________________________________________________________________________
void T49Histogram2D::CreateHistograms() 
{

  Char_t name[25];
  Char_t title[255];

  fHisto = new TObjArray(fXN*fYN);

  for (Int_t ix = 0; ix < fXN; ix++) {
    for (Int_t iy = 0; iy < fYN; iy++) {
      sprintf(name,"%sX%dY%d",fHName,ix,iy);
      sprintf(title,"%s x-Bin%d %g - %g y-Bin%d %g - %g",fHTitle
                   ,ix,(Float_t) (GetWidthX()* ix    + fXMin)
                      ,(Float_t) (GetWidthX()*(ix+1) + fXMin)
                   ,iy,(Float_t) (GetWidthY()* iy    + fYMin)
                      ,(Float_t) (GetWidthY()*(iy+1) + fYMin));
      TH1D *H = new TH1D(name,title,fHN,fHMin,fHMax);
      H->SetXTitle(fXTitle);
      H->SetYTitle(fYTitle);
      fHisto->Add(H);
    }
  }

}





