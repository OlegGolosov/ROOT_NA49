///////////////////////////////////////////////////////////////////////
//                                                                   //
//    3-dim histogram container class                                //
//                                                                   //
//    Author: Christoph Blume                                        //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "T49Histogram3D.h"

ClassImp(T49Histogram3D)

/*
$Log: T49Histogram3D.C,v $
Revision 1.3  2001/11/12 12:11:19  cblume
Update for ROOT v3.01

 * Revision 1.2  2000/04/19  12:56:03  cblume
 * Changed some functions to inline and corrected bug in CreateHistograms()
 *
 * Revision 1.1  1999/11/23  13:55:02  cblume
 * Add T49Histogram3D.C
 *
*/

//______________________________________________________________________________
T49Histogram3D::T49Histogram3D():T49Histogram() 
{
  //
  // T49Histogram3D default constructor
  //

  Init();

}

//______________________________________________________________________________
T49Histogram3D::T49Histogram3D(const Text_t *name, const Text_t *title)
               :T49Histogram(name,title) 
{
  //
  // T49Histogram3D constructor with name and title
  //

  Init();

}

//______________________________________________________________________________
T49Histogram3D::T49Histogram3D(const Text_t *name, const Text_t *title, TH1 *histo
                              , Int_t nx, Float_t xmin, Float_t xmax
                              , Int_t ny, Float_t ymin, Float_t ymax
                              , Int_t nz, Float_t zmin, Float_t zmax)
               :T49Histogram(name,title,histo) 
{
  //
  // T49Histogram3D constructor with name, title and input histogram
  //

  fXN   = nx;
  fXMin = xmin;
  fXMax = xmax;

  fYN   = ny;
  fYMin = ymin;
  fYMax = ymax;

  fZN   = nz;
  fZMin = zmin;
  fZMax = zmax;

  CreateHistograms();

}

//______________________________________________________________________________
T49Histogram3D::T49Histogram3D(const Text_t *name, const Text_t *title
                              , Int_t n , Float_t  min, Float_t  max
                              , Int_t nx, Float_t xmin, Float_t xmax
                              , Int_t ny, Float_t ymin, Float_t ymax
                              , Int_t nz, Float_t zmin, Float_t zmax)
               :T49Histogram(name,title,n,min,max) 
{
  //
  // T49Histogram3D constructor with name, title, and histogram definition
  //

  fXN   = nx;
  fXMin = xmin;
  fXMax = xmax;

  fYN   = ny;
  fYMin = ymin;
  fYMax = ymax;

  fZN   = nz;
  fZMin = zmin;
  fZMax = zmax;

  CreateHistograms();

}

//______________________________________________________________________________
T49Histogram3D::~T49Histogram3D() 
{
  //
  // Destructor
  //

}

//______________________________________________________________________________
void T49Histogram3D::Draw(const Option_t *option) 
{
  //
  // Draw all histograms on one canvas
  //

  printf("T49Histogram3D::Draw - Drawing is not (yet) implemented\n");

}

//______________________________________________________________________________
void T49Histogram3D::Print(const Option_t *option) const 
{
  //
  // Print the parameter
  //

  T49Histogram::Print(option);
  printf("               Number of bins = %d / %d / %d\n",fXN,fYN,fZN);
  printf("                 Lower border = %g / %g / %g\n",fXMin,fYMin,fZMin);
  printf("                 Upper border = %g / %g / %g\n",fXMax,fYMax,fZMax);
  printf("                    Bin width = %g / %g / %g\n",GetWidthX(),GetWidthY(),GetWidthZ());
  printf("\n");

}

//______________________________________________________________________________
void T49Histogram3D::SetDimensions(Int_t nx, Float_t xmin, Float_t xmax
			         , Int_t ny, Float_t ymin, Float_t ymax
                                 , Int_t nz, Float_t zmin, Float_t zmax) 
{
  //
  // Defines the number of bins <nx/ny/nz> and the borders <xmin/ymin/zmin> 
  // <xmax/ymax/zmax> of the histogram array
  //

  if (fXN) {
    printf("T49Histogram3D::SetDimensions - Dimensions are already defined.\n");
  }
  else {
    fXN   = nx;
    fXMin = xmin;
    fXMax = xmax;
    fYN   = ny;
    fYMin = ymin;
    fYMax = ymax;
    fZN   = nz;
    fZMin = zmin;
    fZMax = zmax;
  }

}


//______________________________________________________________________________
void T49Histogram3D::Init() 
{

  fXN   = 0;
  fXMin = 0;
  fXMax = 0;

  fYN   = 0;
  fYMin = 0;
  fYMax = 0;

  fZN   = 0;
  fZMin = 0;
  fZMax = 0;

}

//______________________________________________________________________________
void T49Histogram3D::CreateHistograms() 
{

  Char_t name[25];
  Char_t title[255];

  fHisto = new TObjArray(fXN*fYN*fZN);

  for (Int_t ix = 0; ix < fXN; ix++) {
    for (Int_t iy = 0; iy < fYN; iy++) {
      for (Int_t iz = 0; iz < fZN; iz++) {
        sprintf(name,"%sX%dY%dZ%d",fHName,ix,iy,iz);
        sprintf(title,"%s x-Bin%d %g - %g y-Bin%d %g - %g z-Bin%d %g - %g",fHTitle
                     ,ix,(Float_t) (GetWidthX()* ix    + fXMin)
                        ,(Float_t) (GetWidthX()*(ix+1) + fXMin)
                     ,iy,(Float_t) (GetWidthY()* iy    + fYMin)
                        ,(Float_t) (GetWidthY()*(iy+1) + fYMin)
                     ,iz,(Float_t) (GetWidthZ()* iz    + fZMin)
                        ,(Float_t) (GetWidthZ()*(iz+1) + fZMin));
        TH1D *H = new TH1D(name,title,fHN,fHMin,fHMax);
        H->SetXTitle(fXTitle);
        H->SetYTitle(fYTitle);
        fHisto->Add(H);
      }
    }
  }

}

