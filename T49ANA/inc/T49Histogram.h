#ifndef _T49Histogram_INCLUDED_
#define _T49Histogram_INCLUDED_

/*
$Log: T49Histogram.h,v $
Revision 1.4  2001/11/12 12:05:58  cblume
Update for ROOT v3.01

 * Revision 1.3  2000/10/09  10:24:54  cblume
 * General update
 *
 * Revision 1.2  2000/04/19  12:54:28  cblume
 * Changed some functions to inline
 *
 * Revision 1.1  1999/11/23  13:53:02  cblume
 * Add T49Histogram.h
 *
*/

#include "TMath.h"
#include "TFile.h"
#include "TNamed.h"
#include "TObjArray.h"
#include "TCanvas.h"
#include "TH1.h"

class T49Histogram : public TNamed {

public:

  T49Histogram();
  T49Histogram(const Text_t *name, const Text_t *title);
  T49Histogram(const Text_t *name, const Text_t *title, TH1 *histo);
  T49Histogram(const Text_t *name, const Text_t *title
             , Int_t n, Float_t min, Float_t max);
  virtual ~T49Histogram();

  //virtual void       Draw() = 0;
  virtual void       Print(const Option_t *option = "") const;
  virtual void       Reset();
  virtual Int_t      Save(Char_t *filename);
  virtual Int_t      WriteHistograms();

  virtual void       SetVerbose(Int_t v = 1)          { fVerbose = v; };

  virtual void       SetHistogram(Char_t *name, Char_t *title
                                , Int_t n, Float_t min, Float_t max);
  virtual void       SetHistogram(TH1 *histo);
  virtual void       SetHistogramName(Char_t *name)   { fHName  = name;  };
  virtual void       SetHistogramTitle(Char_t *title) { fHTitle = title; };

  virtual void       SetLogy(Int_t i = 1)             { fLogy   = i; };

  virtual void       SetXTitle(Text_t *xtitle);
  virtual void       SetYTitle(Text_t *ytitle);
  virtual void       SetXYTitle(Text_t *xtitle, Text_t *ytitle);

          TObjArray *GetHistogramArray()              { return fHisto;  };
          Int_t      GetHistogramNbin()               { return fHN;     };
          Float_t    GetHistogramMin()                { return fHMin;   };
          Float_t    GetHistogramMax()                { return fHMax;   };
          Char_t    *GetHistogramName()               { return fHName;  };
          Char_t    *GetHistogramTitle()              { return fHTitle; };
          Float_t    GetHistogramWidth()          
                               { return (fHN ? (fHMax - fHMin) / fHN : 0); };
  inline  TH1D      *GetHistogram(Int_t index);

protected:

  virtual void  Init();

  Int_t      fVerbose;     //  Switch for verbose mode

  TObjArray *fHisto;       //  Array of histograms
  
  Int_t      fHN;          //  Number of bins in the histograms
  Float_t    fHMin;        //  Lower border of the histograms
  Float_t    fHMax;        //  Upper border of the histograms
  Char_t    *fHName;       //! Name of the histograms
  Char_t    *fHTitle;      //! Title of the histograms
  Text_t    *fXTitle;      //! Title of the x-axis
  Text_t    *fYTitle;      //! Title of the y-axis

  Int_t      fLogy;        //! Switch for logarythmic drawing

  ClassDef(T49Histogram,1) //  Histogram container base class

};

//______________________________________________________________________________
inline TH1D *T49Histogram::GetHistogram(Int_t index) 
{
  //
  // Returns the histogram at position <index>
  //

  if (!(fHisto)) {
    printf("T49Histogram::GetHistogram - No histograms defined\n");
    return NULL;
  }

  return (TH1D *) fHisto->At(index);

}

#endif
