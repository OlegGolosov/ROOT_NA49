#ifndef _T49Mixing_INCLUDED_
#define _T49Mixing_INCLUDED_

/*
$Log: T49Mixing.h,v $
Revision 1.5  2008/01/04 10:48:58  cblume
64bit modifications by Marek Szuba

Revision 1.4  2001/11/12 12:06:47  cblume
Update for ROOT v3.01

 * Revision 1.3  1999/11/23  13:52:15  cblume
 * Remove files
 *
*/

#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>

/* ROOT includes */
#include "TClass.h"
#include "TKey.h"
#include "TRandom.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TNtuple.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TRandom.h"
#include "TSystem.h"


#include "T49ParticleRoot.h"


class T49ParticleRoot;

class T49Mixing : public TObject 
{
  
protected:
  Int_t nMaxEvents;                    // Maximum number of event
  Int_t nEvent;                        // Total Number of events processed 
  Int_t nCurrent;                      // Degree of filling  ( from 0 to nMaxEvents)
  Int_t fIsReadyFlag;                  // Flag signals nCurrent == nMaxEvents
  Int_t fNumList;                      // Number of diefferent particle lists
          
  Int_t fVerbose;
  
  TObjArray *fPart1Pool;               // Particle 1 Pool
  TObjArray *fPart2Pool;               // Particle 2 Pool 

  TObjArray *fPart1List;               // Current Pacticle 1 List
  TObjArray *fPart2List;               // Current Pacticle 2 List

  Int_t fEvent1;                       // EventNumber for first P. list
  Int_t fEvent2;                       // EventNumber for second P. list
  
  
public: 
  T49Mixing();

  T49Mixing(Int_t n, Int_t nl=2);
  
  Int_t CheckBound(Int_t m) { return (( 0<=m && m < nCurrent) ? 1 : 0 );}
  
  Int_t IsReady() {return fIsReadyFlag;};

  void SetNumList(Int_t n = 2) {fNumList=n;};
  
  void SetVerbose(Int_t n = 1) {fVerbose=n;};
  
  void Reset();
      
  void AddEvent(TObjArray *P1list, TObjArray *P2list=NULL);
    
  void GetEvent(Int_t Evt1=-1, Int_t Evt2=-1);

  Int_t GetEvent1() { return fEvent1; };
  Int_t GetEvent2() { return fEvent2; };

  TObjArray* GetPart1List();  
  TObjArray* GetPart2List();

  ClassDef(T49Mixing,1)   // Event mixer for list of T49ParticleRoot

} ; 
#endif









