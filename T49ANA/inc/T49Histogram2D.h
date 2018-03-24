#ifndef _T49Histogram2D_INCLUDED_
#define _T49Histogram2D_INCLUDED_

/*
$Log: T49Histogram2D.h,v $
Revision 1.4  2001/11/12 12:06:10  cblume
Update for ROOT v3.01

 * Revision 1.3  2000/10/09  10:24:56  cblume
 * General update
 *
 * Revision 1.2  2000/04/19  12:52:13  cblume
 * Changed some functions to inline
 *
 * Revision 1.1  1999/11/23  13:53:08  cblume
 * Add T49Histogram2D.h
 *
*/

#include "T49Histogram.h"

class T49Histogram2D : public T49Histogram {

public:

  T49Histogram2D();
  T49Histogram2D(const Text_t *name, const Text_t *title);
  T49Histogram2D(const Text_t *name, const Text_t *title, TH1 *histo
                , Int_t nx, Float_t xmin, Float_t xmax
                , Int_t ny, Float_t ymin, Float_t ymax);
  T49Histogram2D(const Text_t *name, const Text_t *title
                , Int_t n , Float_t  min, Float_t  max
                , Int_t nx, Float_t xmin, Float_t xmax
                , Int_t ny, Float_t ymin, Float_t ymax);
  virtual ~T49Histogram2D();

  virtual void     CreateHistograms();
  virtual void     Draw(const Option_t *option = "");
  virtual void     Draw(Int_t iFirst, Int_t iLast
                      , Int_t jFirst, Int_t jLast);
  virtual void     Draw(Float_t xFirst, Float_t xLast
                      , Float_t yFirst, Float_t yLast);
  inline  void     Fill(Float_t x, Float_t y, Float_t val, Float_t weight = 1);
  virtual void     Print(const Option_t *option = "") const;

  virtual void     SetDimensions(Int_t nx, Float_t xmin, Float_t xmax
                               , Int_t ny, Float_t ymin, Float_t ymax);

          Int_t    GetNbinX() const    { return fXN;   };
          Float_t  GetMinX() const     { return fXMin; };
          Float_t  GetMaxX() const     { return fXMax; };
          Float_t  GetWidthX() const   { return (fXN ? (fXMax - fXMin) / fXN : 0); };

          Int_t    GetNbinY() const    { return fYN;   };
          Float_t  GetMinY() const     { return fYMin; };
          Float_t  GetMaxY() const     { return fYMax; };
          Float_t  GetWidthY() const   { return (fYN ? (fYMax - fYMin) / fYN : 0); };

  inline  TH1D    *GetHistogram(Float_t x, Float_t y);

protected:

          void     Init();
  inline  Int_t    GetIndexX(Float_t x);
  inline  Int_t    GetIndexY(Float_t y);

  Int_t    fXN;                       // Number of bins 
  Float_t  fXMin;                     // Lower border
  Float_t  fXMax;                     // Upper border

  Int_t    fYN;                       // Number of bins 
  Float_t  fYMin;                     // Lower border
  Float_t  fYMax;                     // Upper border

  ClassDef(T49Histogram2D,1)          // 2-dim histogram container class

};

//______________________________________________________________________________
inline void T49Histogram2D::Fill(Float_t x, Float_t y, Float_t val, Float_t wgt) 
{
  //
  // Fill the histogram for the values <x>/<y> with value <val> and
  // the weight <wgt>
  //

  TH1D *H;

  if ((H = GetHistogram(x,y))) {
    H->Fill(val,wgt);
  }

}

//______________________________________________________________________________
inline TH1D* T49Histogram2D::GetHistogram(Float_t x, Float_t y) 
{
  //
  // Return the histogram for the values <x/y>
  //

  Int_t ix = GetIndexX(x);
  Int_t iy = GetIndexY(y);

  if ((ix < 0) || (iy < 0))
    return NULL;
  else 
    return ((TH1D *) fHisto->At(ix*fYN+iy));

}

//______________________________________________________________________________
inline Int_t T49Histogram2D::GetIndexX(Float_t x) 
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
inline Int_t T49Histogram2D::GetIndexY(Float_t y) 
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

#endif
