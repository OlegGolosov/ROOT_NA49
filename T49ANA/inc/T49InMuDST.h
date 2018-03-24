#ifndef __T49InMuDST__
#define __T49InMuDST__

/*
$Log: T49InMuDST.h,v $
Revision 1.5  2008/01/04 10:48:58  cblume
64bit modifications by Marek Szuba

Revision 1.4  2005/05/20 11:27:54  cblume
Get rid of deprecated warnings

Revision 1.3  1999/11/23 13:52:15  cblume
Remove files

*/

#include <iostream>

using namespace std;

#include "TROOT.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TNamed.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TAxis.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TKey.h"
#include "TClass.h"
#include "TMath.h"


#include "T49ParticleRoot.h"

/*
class T49PartRoot : public T49ParticleRoot
{
 protected:
  Float_t fProb_Pr;
  Float_t fProb_Ka;
  Float_t fProb_Pi;
  Float_t fProb_El;
 public:
  T49PartRoot()
    {
     fProb_Pr=0.0;
     fProb_Ka=0.0;
     fProb_Pi=0.0;
     fProb_El=0.0;
    }
  void SetProb_Pr(Float_t p_in) { fProb_Pr = p_in;}
  void SetProb_Ka(Float_t p_in) { fProb_Ka = p_in;}
  void SetProb_Pi(Float_t p_in) { fProb_Pi = p_in;}
  void SetProb_El(Float_t p_in) { fProb_El = p_in;}
  Float_t GetProb_Pr(void)   { return fProb_Pr;}
  Float_t GetProb_Ka(void)   { return fProb_Ka;}
  Float_t GetProb_Pi(void)   { return fProb_Pi;}
  Float_t GetProb_El(void)   { return fProb_El;}

 ClassDef(T49PartRoot,1)
};
*/


//================================================================================================
//================================================================================================

class T49InMuDST : public TObject
{
 protected:
  Int_t fNTracks;
  Int_t fNRun;
  Int_t fNEvt;
  Int_t fNTrck;
  Int_t fEvtType; 
  Int_t fEvtCount;  
  
  Int_t fdEdxFlag;
  Int_t fVerbose;
  
  Int_t fTestEOF;
  Int_t fTestEvt;
  Int_t fTestTrck;
  Text_t *fName;   //!
  FILE *fp_in;     //!
  TObjArray *TrackList;
  
 public:
  T49InMuDST(Text_t *fN)
    {
      fName=fN;
      printf(" <I> Input from MuDSt initialized\n");
      fTestEOF=OpenFile();
      TrackList = new TObjArray();
    } 
  
  Int_t GetNRun()    {return fNRun;}
  Int_t GetNEvent()  {return fNEvt;}
  Int_t GetNTrck()   {return fNTrck;}
  Int_t GetEvtType() {return fEvtType;}
  Int_t GetEvtNumb() {return fEvtCount;}

  Int_t OpenFile(Text_t *fN);
  Int_t OpenFile();
  void CloseFile()
    {
      if (fp_in!=NULL) {fclose(fp_in);};
      fTestEOF=1;
      printf(" <I> Input muDST-file %s closed \n",fName);
    }

  TObjArray* ReadEvt();
  
  ClassDef(T49InMuDST,1)   // Converts mu-DST to ROOT mini-DSTs

}; 
#endif
