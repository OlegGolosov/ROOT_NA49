#ifndef _T49Histogram3D_INCLUDED_
#define _T49Histogram3D_INCLUDED_

/*
$Log: T49Histogram3D.h,v $
Revision 1.5  2002/08/28 13:47:25  cblume
Fix typo in GetHistogram()

Revision 1.4  2001/11/12 12:06:14  cblume
Update for ROOT v3.01

 * Revision 1.3  2000/10/09  10:24:56  cblume
 * General update
 *
 * Revision 1.2  2000/04/19  12:52:29  cblume
 * Changed some functions to inline
 *
 * Revision 1.1  1999/11/23  13:53:09  cblume
 * Add T49Histogram3D.h
 *
*/

#include "T49Histogram.h"

class T49Histogram3D : public T49Histogram {

public:

  T49Histogram3D();
  T49Histogram3D(const Text_t *name, const Text_t *title);
  T49Histogram3D(const Text_t *name, const Text_t *title, TH1 *histo
                , Int_t nx, Float_t xmin, Float_t xmax
                , Int_t ny, Float_t ymin, Float_t ymax
                , Int_t nz, Float_t zmin, Float_t zmax);
  T49Histogram3D(const Text_t *name, const Text_t *title
                , Int_t n , Float_t  min, Float_t  max
                , Int_t nx, Float_t xmin, Float_t xmax
                , Int_t ny, Float_t ymin, Float_t ymax
                , Int_t nz, Float_t zmin, Float_t zmax);
  virtual ~T49Histogram3D();

  virtual void     CreateHistograms();
  virtual void     Draw(const Option_t *option = "");
  inline  void     Fill(Float_t x, Float_t y, Float_t z
                      , Float_t val, Float_t weight = 1);
  virtual void     Print(const Option_t *option = "") const;

  virtual void     SetDimensions(Int_t nx, Float_t xmin, Float_t xmax
                               , Int_t ny, Float_t ymin, Float_t ymax
                               , Int_t nz, Float_t zmin, Float_t zmax);

          Int_t    GetNbinX() const    { return fXN;   };
          Float_t  GetMinX() const     { return fXMin; };
          Float_t  GetMaxX() const     { return fXMax; };
          Float_t  GetWidthX() const   { return (fXN ? (fXMax - fXMin) / fXN : 0); };

          Int_t    GetNbinY() const    { return fYN;   };
          Float_t  GetMinY() const     { return fYMin; };
          Float_t  GetMaxY() const     { return fYMax; };
          Float_t  GetWidthY() const   { return (fYN ? (fYMax - fYMin) / fYN : 0); };

          Int_t    GetNbinZ() const    { return fZN;   };
          Float_t  GetMinZ() const     { return fZMin; };
          Float_t  GetMaxZ() const     { return fZMax; };
          Float_t  GetWidthZ() const   { return (fZN ? (fZMax - fZMin) / fZN : 0); };

  inline  TH1D    *GetHistogram(Float_t x, Float_t y, Float_t z);

protected:

          void     Init();
  inline  Int_t    GetIndexX(Float_t x);
  inline  Int_t    GetIndexY(Float_t y);
  inline  Int_t    GetIndexZ(Float_t z);

  Int_t    fXN;                       // Number of bins 
  Float_t  fXMin;                     // Lower border
  Float_t  fXMax;                     // Upper border

  Int_t    fYN;                       // Number of bins 
  Float_t  fYMin;                     // Lower border
  Float_t  fYMax;                     // Upper border

  Int_t    fZN;                       // Number of bins 
  Float_t  fZMin;                     // Lower border
  Float_t  fZMax;                     // Upper border

  ClassDef(T49Histogram3D,1)          // 3-dim histogram container class

};

//______________________________________________________________________________
inline void T49Histogram3D::Fill(Float_t x, Float_t y, Float_t z
                               , Float_t val, Float_t wgt) 
{
  //
  // Fill the histogram for the values <x/y/z> with value <val> and
  // the weight <wgt>
  //

  TH1D *H;

  if ((H = GetHistogram(x,y,z))) {
    H->Fill(val,wgt);
  }

}

//______________________________________________________________________________
inline TH1D* T49Histogram3D::GetHistogram(Float_t x, Float_t y, Float_t z) 
{
  //
  // Return the histogram for the values <x/y/z>
  //

  Int_t ix = GetIndexX(x);
  Int_t iy = GetIndexY(y);
  Int_t iz = GetIndexZ(z);

  if ((ix < 0) || (iy < 0) || (iz < 0))
    return NULL;
  else 
    return ((TH1D *) fHisto->At(ix*fYN*fZN+iy*fZN+iz));

}

//______________________________________________________________________________
inline Int_t T49Histogram3D::GetIndexX(Float_t x) 
{

  if ((x >= fXMin) && (x < fXMax)) {
    Float_t width = GetWidthX();
    if (width) 
      return ((Int_t) ((x - fXMin) / width));
    else
      return -1; 
  }
  else {
    return -1;
  }

}

//______________________________________________________________________________
inline Int_t T49Histogram3D::GetIndexY(Float_t y) 
{

  if ((y >= fYMin) && (y < fYMax)) {
    Float_t width = GetWidthY();
    if (width) 
      return ((Int_t) ((y - fYMin) / width));
    else
      return -1; 
  }
  else {
    return -1;
  }

}

//______________________________________________________________________________
inline Int_t T49Histogram3D::GetIndexZ(Float_t z) 
{

  if ((z >= fZMin) && (z < fZMax)) {
    Float_t width = GetWidthZ();
    if (width) 
      return ((Int_t) ((z - fZMin) / width));
    else
      return -1; 
  }
  else {
    return -1;
  }

}

#endif
