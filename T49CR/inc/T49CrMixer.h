#ifndef _T49CrMixer_INCLUDED_
#define _T49CrMixer_INCLUDED_

#include "TObject.h"
#include "TObjArray.h"
#include "TRandom.h"

class T49ParticleRoot;

class T49CrMixer : public TObject {
    
  public: 
    T49CrMixer() { fParticlePool = NULL; };
    virtual ~T49CrMixer() { };

    T49CrMixer(Int_t MaxSize, Int_t MaxEventSize = 2000);

    void       AddParticleList(TObjArray *l);
    Int_t      GetSize() { return fPoolSize; };
    Int_t      GetNStoredEvents() { return fNStoredEvents; };
    Int_t      PickEvent(Int_t CheckMult = kFALSE);
    void       SetVerbose(Int_t n) { fVerbose = n; }
    void       SetSeed(Int_t seed);
    Int_t      Rndm(Float_t max);
    void       ShowStat();
    Bool_t     IsReady(Int_t mult = 0);

    T49ParticleRoot  *GetParticle(Int_t NEvent);
    TObjArray        *GetParticleList(Int_t m);
    TObjArray        *GetEvent(Int_t NEvent);
    void              GetPair(T49ParticleRoot **p1, T49ParticleRoot **p2);

    ClassDef(T49CrMixer,1)  // T49CrMixer 

  protected:
 
    Int_t         fVerbose;             //! Controls debugging output
    Int_t         fMaxPoolSize;         //! Max size of mixing pool
    Int_t         fPoolSize;            //! Current size of mixing pool
    Int_t         fCurrentEvent;        //! Pointer to current event for track storage
    Int_t         fMaxStoredEvents;     //! Max number of different events in pool
    Int_t         fNStoredEvents;       //! Current number of different events in pool
    Int_t         fMaxTracksPerEvent;   //! Max number of tracks kept per event    
    Int_t        *fNTracksPerEvent;     //! Actual number of tracks kept for event
    Int_t        *fNOrgTracksPerEvent;  //! Number of tracks in original event     
    Int_t        *fEventUsed;           //! Flag used in event mixing             
    T49ParticleRoot     *fParticlePool;        //! Pointer to pool of tracks
    TRandom      *fRandom;              //! Random number generator
    Int_t        *fIndexStat;           //! Statistics on index selection
    Int_t        *fEventStat;           //! Statistics on event selection
    Int_t         fTotalCalls;          //! Statistics on GetParticle() calls
    Int_t         fTotalEventCalls;     //! Statistics on PickEvent() calls
    Int_t         fMaxOrgTracks;        //! Number of tracks in biggest original event
    Double_t      fMeanPt;              //! mean pt quality control
    Long_t        fIdum;                //! Random number generator register
};

#endif


