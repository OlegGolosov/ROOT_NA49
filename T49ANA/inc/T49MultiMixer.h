#ifndef _T49MULTIMIXER_INCLUDED_
#define _T49MULTIMIXER_INCLUDED_

/*
$Log: T49MultiMixer.h,v $
Revision 1.1  2006/07/28 09:48:07  cblume
Add T49MultiMixer.h

*/

class TObjArray;
class TFile;

class T49EventRoot;

class T49Mixing;
class T49Proj2BASE;

class T49MultiMixer : public TObject {

 public: 

  T49MultiMixer();
  T49MultiMixer(Int_t multi_min, Int_t multi_max, Int_t multi_interval, Int_t Events, Int_t character);
  
  void  Reset();
  // Mulitiplicity or Eveto bins selection
  Int_t MultiEvetoBins(T49EventRoot *Event,Int_t type);                                
  // Definition type of bins selection add each event in proper T49Mixing classes pool
  void  AddEvent(T49EventRoot *Event, TObjArray *P1list, TObjArray *P2list);                                 
  // Fill functions for T49ProjBASE classes with proper mixed event
  void  GetEvents(T49Proj2BASE *MultiSignal, T49Proj2BASE *MultiMixed);                 
  // Write object output into outside ROOT file
  void  WriteEvents(TFile *OutFile,T49Proj2BASE *MultiSignal,T49Proj2BASE *MultiMixed); 
      
 protected:

  Bool_t      fVerbose;         // Output control flag
  Int_t       fType;            // Type of bin selection
  Float_t     fPimass;          // Phyical constants: pion mass
  Float_t     fKmass;           // Phyical constants: kaon mass
  Float_t     fPrmass;          // Phyical constants: proton mass
  Int_t       fNMixed;          // Definition for mixing procedures
  Bool_t      fFirstMix;        // Definition for mixing procedures

  Int_t       fRanges;          // Range definition for custom variable
  Int_t       fMulti_min;       // Range definition for custom variable
  Int_t       fMulti_max;       // Range definition for custom variable
  Int_t       fMulti_interval;  // Range definition for custom variable

  Int_t       fEvents;          // Number of events to be processed
  Int_t       fEvent_counter;   // Number of processed events
 
  T49Mixing **fEventMixer;      // Define the event-mixer with 10 events in the pool

  TObjArray  *fMultiPosParticleList;
  TObjArray  *fMultiNegParticleList;
  TObjArray  *fMultiMixedPosParticleList;
  TObjArray  *fMultiMixedNegParticleList;

  ClassDef(T49MultiMixer,1)     // Event mixer for list of T49ParticleRoot

};

#endif
