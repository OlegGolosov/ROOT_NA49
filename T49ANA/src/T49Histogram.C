///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Histogram container base class                                 //
//                                                                   //
//    Author: Christoph Blume                                        //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "T49Histogram.h"

ClassImp(T49Histogram)

/*
$Log: T49Histogram.C,v $
Revision 1.6  2002/08/28 13:46:34  cblume
Initialize the histo names with TNamed::GetName()

Revision 1.5  2001/12/12 17:42:54  cblume
Update

Revision 1.4  2001/11/12 12:11:05  cblume
Update for ROOT v3.01

 * Revision 1.3  2000/06/05  13:00:23  cblume
 * Use gPad in Draw()
 *
 * Revision 1.2  2000/04/19  12:54:50  cblume
 * Changed some functions to inline
 *
 * Revision 1.1  1999/11/23  13:54:46  cblume
 * Add T49Histogram.C
 *
*/

//______________________________________________________________________________
T49Histogram::T49Histogram():TNamed() 
{
  //
  // T49Histogram default constructor
  //

  Init();

}

//______________________________________________________________________________
T49Histogram::T49Histogram(const Text_t *name, const Text_t *title)
             :TNamed(name,title) 
{
  //
  // T49Histogram constructor with name and title
  //

  Init();

}

//______________________________________________________________________________
T49Histogram::T49Histogram(const Text_t *name, const Text_t *title, TH1 *histo)
             :TNamed(name,title) 
{
  //
  // T49Histogram constructor with name, title and input histogram
  //

  Init();

  fHName  = (Char_t *) histo->GetName();
  fHTitle = (Char_t *) histo->GetTitle();
  fHN     = histo->GetNbinsX();
  fHMin   = histo->GetMinimum();
  fHMax   = histo->GetMaximum();

}

//______________________________________________________________________________
T49Histogram::T49Histogram(const Text_t *name, const Text_t *title
                         , Int_t n, Float_t min, Float_t max)
             :TNamed(name,title) 
{
  //
  // T49Histogram constructor with name, title and input histogram
  //

  Init();

  fHName  = ((Char_t *) name);
  fHTitle = ((Char_t *) title);
  fHN     = n;
  fHMin   = min;
  fHMax   = max;

}

//______________________________________________________________________________
T49Histogram::~T49Histogram() 
{
  //
  // Destructor
  //

  if (fHisto) {
    fHisto->Delete();
    delete fHisto;
  }

}

//______________________________________________________________________________
void T49Histogram::Print(const Option_t *option) const 
{
  //
  // Print the parameter
  //

  printf(" Histogram-Array %s\n",GetName());
  printf("\n");
  printf("     Number of histogram-bins = %d\n",fHN);
  printf("   Lower border of histograms = %g\n",fHMin);
  printf("   Upper border of histograms = %g\n",fHMax);
  printf("\n");

}

//______________________________________________________________________________
void T49Histogram::SetHistogram(Char_t *name, Char_t *title
       			      , Int_t n, Float_t min, Float_t max) 
{
  //
  // Defines the histogram parameter
  //

  if (fHN) {
    printf("T49Histogram::SetHistogram - Histogram dimensions are already defined.\n");
  }
  else {
    fHName  = name;
    fHTitle = title;
    fHN     = n;
    fHMin   = min;
    fHMax   = max;
  }

}

//______________________________________________________________________________
void T49Histogram::SetHistogram(TH1 *histo) 
{
  //
  // Defines the histogram in the array as a copy of
  // an input histogram
  //

  if (fHN) {
    printf("T49Histogram::SetHistogram - Histogram dimensions are already defined.\n");
  }
  else {
    fHName  = (Char_t *) histo->GetName();
    fHTitle = (Char_t *) histo->GetTitle();
    fHN     = histo->GetNbinsX();
    fHMin   = histo->GetMinimum();
    fHMax   = histo->GetMaximum();
  }

}

//______________________________________________________________________________
void T49Histogram::SetXTitle(Text_t *xtitle) 
{
  //
  // Set the title of the x-axis of the histograms
  //

  fXTitle = xtitle;

  if (fHisto) {
    for (Int_t i = 0; i < fHisto->GetEntries(); i++) {
      TH1D *H = (TH1D *) fHisto->At(i);
      H->SetXTitle(fXTitle);
    }
  }  

}

//______________________________________________________________________________
void T49Histogram::SetYTitle(Text_t *ytitle) 
{
  //
  // Set the title of the y-axis of the histograms
  //

  fYTitle = ytitle;

  if (fHisto) {
    for (Int_t i = 0; i < fHisto->GetEntries(); i++) {
      TH1D *H = (TH1D *) fHisto->At(i);
      H->SetYTitle(fYTitle);
    }
  }  

}

//______________________________________________________________________________
void T49Histogram::SetXYTitle(Text_t *xtitle, Text_t *ytitle) 
{
  //
  // Set the axis titles of the histograms
  //

  fXTitle = xtitle;
  fYTitle = ytitle;

  if (fHisto) {
    for (Int_t i = 0; i < fHisto->GetEntries(); i++) {
      TH1D *H = (TH1D *) fHisto->At(i);
      H->SetXTitle(fXTitle);
      H->SetYTitle(fYTitle);
    }
  }  

}

//______________________________________________________________________________
void T49Histogram::Init() 
{

  fVerbose = 0;

  fHisto   = NULL;

  fHN      = 0;
  fHMin    = 0.0;
  fHMax    = 0.0;
  fHName   = NULL;  
  fHTitle  = NULL;
  fXTitle  = NULL;
  fYTitle  = NULL;

  fLogy    = 0;

}

//______________________________________________________________________________
void T49Histogram::Reset() 
{
  //
  // Resets all histograms
  //

  TIter NextHisto(fHisto);
  while (TH1D *H = (TH1D *) NextHisto()) {
    H->Reset();
  }

}

//______________________________________________________________________________
Int_t T49Histogram::Save(Char_t *filename) 
{
  //
  // Saves all histograms in the ROOT-file <filename>
  //

  printf("T49Histogram::Save - Save the histograms in the file %s\n",filename);

  TFile *File = new TFile(filename,"RECREATE");

  if (!(WriteHistograms())) return 0;

  File->Close();

  return 1;

}

//______________________________________________________________________________
Int_t T49Histogram::WriteHistograms() 
{
  //
  // Writes out all histograms
  //

  if (!(fHisto)) {
    printf("T49Histogram::WriteHistograms - No histograms defined\n");
    return 0;
  }

  TIter NextHisto(fHisto);
  while (TH1D *H = (TH1D *) NextHisto()) {
    H->Write();
  }

  return 1;

}














