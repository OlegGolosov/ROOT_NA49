#ifndef _TRootDSMC_included_
#define _TRootDSMC_included_

/*
$Log: TRootDSMC.h,v $
 * Revision 1.3  2000/04/17  13:57:31  cblume
 * Added fFillMCOnly and FillMCEvent()
 *
 * Revision 1.2  2000/02/28  13:03:07  cblume
 * Introduced FillMCTrack(). Copy now also TOF information for MC tracks
 *
 * Revision 1.1  1999/11/23  12:48:18  cblume
 * Add inc
 *
*/

#include "TRootDS.h"

class T49EventMCRoot;
class T49ParticleMCRoot;

struct etrack_mc_t;
struct track_mc_t;
struct vertex_mc_t;
struct match_track_t;
struct etrack_t;

class TRootDSMC : public TRootDS {
  
public: 

  TRootDSMC(); 
  virtual ~TRootDSMC();

  virtual Bool_t          ReadEventT49();  
  virtual Bool_t          DS2Tree(char *InName, char *OutName, Int_t MaxEvents = 100000000);
    
  virtual TTree*          CreateTree();

  virtual Bool_t          FillEvent();
  virtual Bool_t          FillMCEvent();
  virtual Bool_t          FillMCTrack(T49ParticleMCRoot *track_out, track_mc_t *track_in);
  
  virtual void            SetFillMCTracks(Bool_t n) { fFillMCTracks = n; };
  virtual void            SetFillMCOnly(Bool_t n)   { fFillMCOnly   = n; };

  virtual T49EventMCRoot *GetEventMC()              { return fEventMC;   };

protected:

  Int_t              fNumMCTracks;      // Number of MC tracks in one event
  Int_t              fNumMCVertices;    // Number of MC V0s in one event

  Bool_t             fFillMCTracks;     // Steers the copying of the MC data
  Bool_t             fFillMCOnly;       // Copy only MC data if true

  T49EventMCRoot    *fEventMC;          //!

  ClassDef(TRootDSMC,1)                 // Interface class between DSPACK and ROOT for MC-data

};

#endif

