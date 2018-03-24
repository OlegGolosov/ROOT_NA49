///////////////////////////////////////////////////////////////////////
//                                                                   //
//    1-dim histogram container class                                //
//                                                                   //
//    Author: Christoph Blume                                        //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "T49Histogram1D.h"

ClassImp(T49Histogram1D)

/*
$Log: T49Histogram1D.C,v $
Revision 1.4  2001/11/12 12:11:10  cblume
Update for ROOT v3.01

 * Revision 1.3  2000/06/05  13:00:37  cblume
 * Use gPad in Draw()
 *
 * Revision 1.2  2000/04/19  12:55:23  cblume
 * Changed some functions to inline and corrected bug in CreateHistograms()
 *
 * Revision 1.1  1999/11/23  13:54:53  cblume
 * Add T49Histogram1D.C
 *
*/

//______________________________________________________________________________
T49Histogram1D::T49Histogram1D():T49Histogram() 
{
  //
  // T49Histogram1D default constructor
  //

  Init();

}

//______________________________________________________________________________
T49Histogram1D::T49Histogram1D(const Text_t *name, const Text_t *title)
               :T49Histogram(name,title) 
{
  //
  // T49Histogram1D constructor with name and title
  //

  Init();

}

//______________________________________________________________________________
T49Histogram1D::T49Histogram1D(const Text_t *name, const Text_t *title, TH1 *histo
                              , Int_t nx, Float_t xmin, Float_t xmax)
               :T49Histogram(name,title,histo) 
{
  //
  // T49Histogram1D constructor with name, title, and input histogram
  //

  fXN   = nx;
  fXMin = xmin;
  fXMax = xmax;

  CreateHistograms();

}

//______________________________________________________________________________
T49Histogram1D::T49Histogram1D(const Text_t *name, const Text_t *title
                             , Int_t n , Float_t  min, Float_t  max
                             , Int_t nx, Float_t xmin, Float_t xmax)
               :T49Histogram(name,title,n,min,max) 
{
  //
  // T49Histogram1D constructor with name, title, and histogram definition
  //

  fXN   = nx;
  fXMin = xmin;
  fXMax = xmax;

  CreateHistograms();

}

//______________________________________________________________________________
T49Histogram1D::~T49Histogram1D() 
{
  //
  // Destructor
  //

}

//______________________________________________________________________________
void T49Histogram1D::Draw(const Option_t *option) 
{
  //
  // Draw all histograms on one canvas
  //

  Draw(0,fXN);

}

//______________________________________________________________________________
void T49Histogram1D::Draw(Float_t xFirst, Float_t xLast) 
{
  //
  // Draw all histograms between the values <xFirst> and <xLast>
  //  

  Draw(GetIndex(xFirst),GetIndex(xLast)+1);

}

//______________________________________________________________________________
void T49Histogram1D::Draw(Int_t iFirst, Int_t iLast) 
{
  //
  // Draw all histograms between the bins <iFirst> and <iLast>
  //

  if ((iFirst < 0) || (iLast > fXN)) {
    printf("T49Histogram1D::Draw - Boundary error %d - %d (%d)\n"
          ,iFirst,iLast,fXN);
    return;
  }

  TCanvas *C;
  TPad    *P;

  if (!gPad) {
    Text_t *title = fHTitle;
    C = new TCanvas("plot",title,50,50,600,900);
  }
  P = (TPad *) gPad;

  Int_t idiv = (Int_t) TMath::Sqrt(((Float_t) (iLast - iFirst)));
  Int_t xdiv = TMath::Nint(((Float_t) (4/5 * idiv + 1)));
  Int_t ydiv = TMath::Nint(((Float_t) (5/4 * idiv + 1)));  


  P->Divide(xdiv,ydiv);

  for (Int_t iPlot = iFirst; iPlot < iLast; iPlot++) {

    P->cd(iPlot+1);
    if (fLogy) gPad->SetLogy();
    TH1D *H = (TH1D*) fHisto->At(iPlot);    
    H->Draw();

  }

}

//______________________________________________________________________________
void T49Histogram1D::Print(const Option_t *option) const 
{
  //
  // Print the parameter
  //

  T49Histogram::Print(option);
  printf("               Number of bins = %d\n",fXN);
  printf("                 Lower border = %g\n",fXMin);
  printf("                 Upper border = %g\n",fXMax);
  printf("                    Bin width = %g\n",GetWidth());
  printf("\n");

}

//______________________________________________________________________________
void T49Histogram1D::SetDimensions(Int_t n, Float_t min, Float_t max) 
{
  //
  // Defines the number of bins <n> and the borders <min> <max> of
  // the histogram array
  //

  if (fXN) {
    printf("T49Histogram1D::SetDimensions - Dimensions are already defined.\n");
  }
  else {
    fXN   = n;
    fXMin = min;
    fXMax = max;
  }

}

//______________________________________________________________________________
void T49Histogram1D::Init() 
{

  fXN     = 0;
  fXMin   = 0;
  fXMax   = 0;

}

//______________________________________________________________________________
void T49Histogram1D::CreateHistograms() 
{

  Char_t name[25];
  Char_t title[255];

  fHisto = new TObjArray(fXN);

  for (Int_t ix = 0; ix < fXN; ix++) {
    sprintf(name,"%sX%d",fHName,ix);
    sprintf(title,"%s Bin%d %g - %g",fHTitle
                ,ix,(Float_t) (GetWidth()* ix    + fXMin)
                   ,(Float_t) (GetWidth()*(ix+1) + fXMin));
    TH1D *H = new TH1D(name,title,fHN,fHMin,fHMax);
    H->SetXTitle(fXTitle);
    H->SetYTitle(fYTitle);
    fHisto->Add(H);
  }

}
