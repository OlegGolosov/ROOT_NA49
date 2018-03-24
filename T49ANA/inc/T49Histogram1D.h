#ifndef _T49Histogram1D_INCLUDED_
#define _T49Histogram1D_INCLUDED_

/*
$Log: T49Histogram1D.h,v $
Revision 1.4  2001/11/12 12:06:04  cblume
Update for ROOT v3.01

 * Revision 1.3  2000/10/09  10:24:55  cblume
 * General update
 *
 * Revision 1.2  2000/04/19  12:51:59  cblume
 * Changed some functions to inline
 *
 * Revision 1.1  1999/11/23  13:53:06  cblume
 * Add T49Histogram1D.h
 *
*/

#include "T49Histogram.h"

class T49Histogram1D : public T49Histogram {

public:

  T49Histogram1D();
  T49Histogram1D(const Text_t *name, const Text_t *title);
  T49Histogram1D(const Text_t *name, const Text_t *title, TH1 *histo
                , Int_t nx, Float_t xmin, Float_t xmax);
  T49Histogram1D(const Text_t *name, const Text_t *title
                , Int_t n , Float_t  min, Float_t  max
                , Int_t nx, Float_t xmin, Float_t xmax);
  virtual ~T49Histogram1D();

  virtual void     CreateHistograms();
  virtual void     Draw(const Option_t *option = "");
  virtual void     Draw(Int_t iFirst, Int_t iLast);
  virtual void     Draw(Float_t xFirst, Float_t xLast);
  inline  void     Fill(Float_t x, Float_t val, Float_t weight = 1);
  virtual void     Print(const Option_t *option = "") const;

  virtual void     SetDimensions(Int_t n, Float_t min, Float_t max);

          Int_t    GetNbin() const      { return fXN;   };
          Float_t  GetMin() const       { return fXMin; };
          Float_t  GetMax() const       { return fXMax; };
          Float_t  GetWidth() const     { return (fXN ? (fXMax - fXMin) / fXN : 0); };

  inline  TH1D    *GetHistogram(Float_t x);

protected:

          void     Init();
  inline  Int_t    GetIndex(Float_t x);

  Int_t    fXN;                       // Number of bins 
  Float_t  fXMin;                     // Lower border
  Float_t  fXMax;                     // Upper border

  ClassDef(T49Histogram1D,1)          // 1-dim histogram container class

};

//______________________________________________________________________________
inline void T49Histogram1D::Fill(Float_t x, Float_t val, Float_t wgt) 
{
  //
  // Fill the histogram for the value <x> with value <val> and
  // the weight <wgt>
  //

  TH1D *H;

  if ((H = GetHistogram(x))) {
    H->Fill(val,wgt);
  }

}

//______________________________________________________________________________
inline TH1D* T49Histogram1D::GetHistogram(Float_t x) 
{
  //
  // Return the histogram for the value <x>
  //

  Int_t ix = GetIndex(x);

  if (ix < 0) 
    return NULL;
  else
    return ((TH1D *) fHisto->At(ix));

}

//______________________________________________________________________________
inline Int_t T49Histogram1D::GetIndex(Float_t x) 
{

  if ((x >= fXMin) && (x < fXMax)) {
    Float_t width = GetWidth();
    if (width) 
      return ((Int_t) ((x - fXMin) / width));
    else
      return -1; 
  }
  else {
    return -1;
  }

}

#endif
