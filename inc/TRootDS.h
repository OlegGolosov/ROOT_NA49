#ifndef _TRootDS_included_
#define _TRootDS_included_

/*
$Log: TRootDS.h,v $
Revision 1.10  2002/02/04 16:09:15  cblume
Include T49VetoRoot information

 * Revision 1.9  2001/06/13  08:05:08  cblume
 * Include the copying of the Xi data
 *
 * Revision 1.8  2000/10/27  09:00:30  cblume
 * Introduced switch for RFIO
 *
 * Revision 1.7  2000/10/25  09:35:31  cblume
 * Changed buffersize to 8000. Copy also WFA data
 *
 * Revision 1.6  2000/10/09  10:25:09  cblume
 * General update
 *
 * Revision 1.5  2000/08/28  08:29:04  cblume
 * New possibility to copy dEdx for secondaries also
 *
 * Revision 1.4  2000/05/19  15:49:24  cblume
 * Remove fSkipEvents
 *
 * Revision 1.3  2000/04/13  15:17:44  cblume
 * Introduced cut on GSI V0s. Rtrack info for secondary tracks only when there is no corresponding main vertex track
 *
 * Revision 1.2  1999/12/16  12:22:06  cblume
 * Removed constructor with arguments
 *
 * Revision 1.1  1999/11/23  12:48:17  cblume
 * Add inc
 *
*/

#include "TObject.h"
#include "TTree.h"
#include "TFile.h"

class T49EventRoot;
class T49ParticleRoot;
class T49VertexRoot;
class T49MonitorTPC;
class T49Ring;
class T49Beam;
class T49Veto;

struct tpc_t;
struct event_t;
struct point_t;
struct track_t;
struct vertex_t;
struct avertex_v0_t;
struct rtrack_t;
struct g_track_ref_t;
struct const_head_t;
struct monitor_tpc_t;
struct ring_t;
struct beam_t;
struct veto_t;
struct pedestal_t;

const Int_t kSkipEvents = 50;

class TRootDS : public TObject {
  
public: 

  TRootDS(); 
  virtual ~TRootDS();
  
  virtual Bool_t        InitDS(Char_t *name, Int_t mode = 0, Int_t db = 0);
  virtual void         *IdsgetDS(Char_t *ObjName, Int_t *handle, Int_t *num);
  virtual Bool_t        OpenDS(Char_t *filename, Int_t db = 0);
  virtual Bool_t        CloseDS();
  virtual Bool_t        Close();
  virtual Bool_t        ReadEventDS();
  virtual Bool_t        ReadEventT49();
  virtual Bool_t        DS2Tree(Char_t *InFMDirName, Char_t *OutDirName
				, Char_t *OutFileName, Int_t nEvent = 1000);
  virtual Bool_t        DS2Tree(Char_t *InName, Char_t *OutName, Int_t MaxEvents = 100000000);
    
  virtual TTree        *CreateTree();

  virtual Bool_t        FillEvent();
          Bool_t        FillTrack(T49ParticleRoot *track_out, g_track_ref_t *track_ref, Bool_t tof);
          Bool_t        FillSecTrack(T49ParticleRoot *track_out, track_t *track_in, Bool_t main);
          Bool_t        FillVertex(T49VertexRoot *vertex_out, vertex_t *vertex_in);
          Bool_t        FillSecVertex(T49VertexRoot *vertex_out, vertex_t *vertex_in);
          Bool_t        FillXiVertex(T49VertexRoot *vertex_out, vertex_t *vertex_in);
          Bool_t        FillMonitor(T49MonitorTPC *out, monitor_tpc_t *in);
          Bool_t        FillRing(T49Ring *out, ring_t *in);
          Bool_t        FillBeam(T49Beam *out, beam_t *in);
          Bool_t        FillVeto(T49Veto *out, veto_t *in, pedestal_t *ped);
          Bool_t        CheckV0(vertex_t *vtx_p);
          Bool_t        CheckXi(vertex_t *vtx_p);

          void          SetFillPrimary(Bool_t n)       { fFillPrimary       = n; };
          void          SetFillSecondary(Bool_t n)     { fFillSecondary     = n; };
          void          SetFillXis(Bool_t n)           { fFillXis           = n; };
          void          SetFullSecondary(Int_t n)      { fFullSecondary     = n; };
          void          SetNormalizedDedx(Bool_t n)    { fNormalizedDedx    = n; };
          void          SetUseMTPCPoints(Bool_t n)     { fUseMTPCPoints     = n; };
          void          SetUseEmptyADC(Bool_t n)       { fUseEmptyADC       = n; }; 
          void          SetCheckV0(Bool_t n)           { fCheckV0           = n; };
          void          SetRFIO(Bool_t n)              { fRFIO              = n; };
          void          SetVerbose(Int_t n);
          void          SetSkipEvents();

  virtual Bool_t        GetDSpackOk()                  { return fDSpackOk;          };
  virtual Bool_t        GetDSFlag()                    { return fDSpackOk;          };
  virtual Bool_t        GetDSpackInitialized()         { return fDSpackInitialized; };

  virtual T49EventRoot *GetEvent()                     { return fEvent;             };
  
protected:
    
  Int_t         fInfileId;             // ID number of the DSPACK input file

  Bool_t        fDSpackLocal;          // DSPACK mode
  Bool_t        fDSpackInitialized;    // DSPACK intialization flag
  Bool_t        fDSpackOk;             // DSPACK status flag

  Bool_t        fRFIO;                 // Switch for RFIO 

  Int_t         fNumTracks;            // Number of primary tracks
  Int_t         fNumSecTracks;         // Number of secondary tracks
  Int_t         fNumSecVertices;       // Number of secondary vertices

  Bool_t        fFillPrimary;          // Switch for primary tracks and vertices
  Bool_t        fFillSecondary;        // Switch for secondary tracks and vertices
  Bool_t        fFillXis;              // Switch for Xi tracks and vertices
  Bool_t        fNormalizedDedx;       // Dedx values are normalized if TRUE
  Bool_t        fUseMTPCPoints;        // Copy only coordinates of MTPC points if TRUE
  Bool_t        fUseEmptyADC;          // Copy "empty" beam-ADC if TRUE
  Bool_t        fCheckV0;              // Switch for V0 cuts during copying
  Int_t         fFullSecondary;        // Switch for the secondary tracks (0: min, 1: +dEdx, 2: +loc point)

  Int_t         fSkipRun[kSkipEvents]; // List of runs and events to be skipped
  Int_t         fSkipEvt[kSkipEvents]; //  
  Int_t         fSkipCnt[kSkipEvents]; // 
  Int_t         fSkipFound;            // Number of events to be skipped

  Int_t         fVerbose;              //! Controls debugging output
 
  Char_t       *fInfile;               //!
  Char_t       *fOutfile;              //!
  
  TFile        *fRootFile;             //!
  T49EventRoot *fEvent;                //!
  TTree        *fTree;                 //!
  
  ClassDef(TRootDS,3)                  // Interface-class between DSPACK and ROOT

};

#endif

