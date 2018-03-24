////////////////////////////////////////////////////////////////
//
//  The TRootDSMC class is an extension of the RootDS class
//  that additionally allows to copy MC data into the 
//  mini-DSTs. For embedded data also the matching between
//  MC tracks and reconstructed tracks is stored.
//
///////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

// T49 includes
#include "TRootDSMC.h"

#define _MATH_H_MATHDEF
#include "T49VertexRoot.h"
#include "T49ParticleRoot.h"
#include "T49EventMCRoot.h"
#include "T49ParticleMCRoot.h"
#include "T49VertexMCRoot.h"
#undef _MATH_H_MATHDEF

// ROOT includes
#include "TROOT.h"
#include "TTree.h"
#include "TList.h"
#include "TClonesArray.h"
#include "TMath.h"

// DSPACK includes
#include "dsc_prot.h"
#include "dspack_rootds.h"
#include "na49_event_mc_str_root.h"
#include "na49_event_str_root.h"
#include "xi_find_root.h"
#include "gteval_match_t_root.h"
#include "gteval_match_user_t_root.h"
//#include "point_macros.h"      

#define MTL  0
#define MTR  1

ClassImp(TRootDSMC);

//______________________________________________________________________________
TRootDSMC::TRootDSMC():TRootDS() 
{
  //
  // TRootDSMC standard constructor
  //

  fFillMCTracks = kFALSE;
  fFillMCOnly   = kFALSE;
  
}

//______________________________________________________________________________
TRootDSMC::~TRootDSMC() 
{
  //
  //  TRootDSMC destructor 
  //

  CloseDS();
    
}

//______________________________________________________________________________
Bool_t TRootDSMC::ReadEventT49() 
{
  //
  //  Read the next event from the DSPACK input file
  //  and copies it directly into the T49 data structure.
  //  This allows to use the T49ANA classes on DSPACK data.
  //  Returns FALSE if end of input file is reached.
  //

  if (fEventMC) {
    if (fVerbose) printf("Clear ClonesArrays\n");
    fEventMC->GetPrimaryParticles()->Delete();
    fEventMC->GetSecondaryParticles()->Delete();
    fEventMC->GetSecondaryVertices()->Delete();
    fEventMC->GetMCParticles()->Delete();
    fEventMC->GetMCVertices()->Delete();
  }

  if (ReadEventDS()) {

    if (fVerbose) printf("Read one event\n");
    if (!(fEventMC)) fEventMC = new T49EventMCRoot(kTRUE);    

    if (fVerbose) printf("Fill one event\n");
    if (fFillMCOnly)
      return FillMCEvent();
    else
      return FillEvent();

  }
  else {

    return kFALSE;

  }

}

//______________________________________________________________________________
Bool_t TRootDSMC::DS2Tree(char *InFname, char *OutFname, Int_t MaxEvents) 
{
  //
  //  Copy the events from the input file 'InFname' (DSPACK)
  //  to the ROOT mini-DST 'OutFname'. Up to MaxEvents are
  //  copied. The ROOT T49EventMCRoot objects are written to the 
  //  mini-DST in a ROOT TTree object, which allows access to 
  //  the event data in an n-tuple like fashion   
  //

#if ROOT_VERSION_CODE >= ROOT_VERSION(5,26,0)
  // OptimizeBaskets is called "after having filled some entries in a Tree"
  const Int_t optimiseAfterNEvents = (MaxEvents < 20) ? TMath::CeilNint(MaxEvents / 2.) : 10;
#endif

  Char_t RootFname[256];
  Int_t  NumEvents = 0;   

  if (fRootFile == NULL) {

    fEventMC = new T49EventMCRoot(kTRUE);

    sprintf(RootFname,"%s",OutFname);
    printf("Open File %s\n",RootFname);
    fRootFile = TFile::Open(RootFname,"RECREATE","A Tree DST");

    printf("SetCompressionLevel(2)\n");
    fRootFile->SetCompressionLevel(2);
    fRootFile->ls();
    printf("Create Tree\n");
    fTree = CreateTree();
    fTree->SetAutoSave(1990000000);
    if(fVerbose > 1) fTree->Print();

  }

  // Copy DS DST to Root file 
  cerr << "Open Input File:  " << InFname << endl;
  OpenDS(InFname);

  cerr << "Loop over events " << InFname << endl;
  while (GetDSpackOk() && NumEvents < MaxEvents) {

    if (ReadEventDS() == kTRUE) {
      if (fVerbose) printf("Fill one event\n");
      Bool_t eventFilled = kFALSE;
      if (fFillMCOnly)
        eventFilled = FillMCEvent();
      else
        eventFilled = FillEvent();
      if (eventFilled) {
        if (fVerbose) printf("Fill tree\n");
        fTree->Fill();
        if (fVerbose) printf("Clear ClonesArrays\n");
        fEventMC->GetPrimaryParticles()->Delete();
        fEventMC->GetSecondaryParticles()->Delete();
        fEventMC->GetSecondaryVertices()->Delete();
        fEventMC->GetMCParticles()->Delete();
        fEventMC->GetMCVertices()->Delete();
      }
      NumEvents++;

#if ROOT_VERSION_CODE >= ROOT_VERSION(5,26,0)
      // Optimise buffers. Use the current tree size, the way autoflush does
      if (NumEvents == optimiseAfterNEvents)
        fTree->OptimizeBaskets(fTree->GetTotBytes(), 1., "d");
#endif
    }

  } 

  CloseDS();

  return kTRUE;

}

//______________________________________________________________________________
TTree *TRootDSMC::CreateTree() 
{
  //
  //  Create the TTree object for the DS2Tree member
  //  function.
  //

  TTree   *tree;

  printf("Create NA49 Monte Carlo ROOT tree\n");
  tree = new TTree("T49MC","A NA49 Monte Carlo ROOT tree");

  if (fVerbose) {
    fEventMC->Dump();
    printf("Create MC event branch\n");
  }
  tree->Branch("Event", "T49EventMCRoot", &fEventMC);
  if (fVerbose) {
    printf("tree ok\n");
  }

  return tree;

}

//______________________________________________________________________________
Bool_t TRootDSMC::FillEvent() 
{
  //
  //  Copy event, track and point data into ROOT NA49
  //  objects for the current event.
  //
  //  Updated for V0-tracks and vertices (CBL 10/02/99)
  // 
  //  Included the Monte Carlo data (CBL 02/06/99)
  //

  Int_t          i;
  Int_t          Dummy;
  Int_t          TotalTracks = 0;
  Int_t          num;
  Int_t          num2;

  Char_t         name[256];

  event_t       *event_p;
  rtrack_t      *rtrack_p;
  track_t       *track_p;
  track_t       *track_tmp_p;
  vertex_t      *vtx_p;
  vertex_t      *vtx_list = 0;
  vertex_t      *vtx_stop_p;
  vertex_t      *main_vtx;
  vertex_t      *prim_vtx;   
  beam_t        *beam_p;
  ring_t        *ring_p;
  veto_t        *veto_p;
  pedestal_t    *veto_ped_p;
  tpc_t         *all_p;
  g_track_ref_t *track_ref = 0;
  g_ptr_list_t  *ptr_list  = 0;
  monitor_tpc_t *moni_p;
  bpd_vertex_t  *bpd_vtx_p;
  wfa_t         *wfa_p;
  wfa_time_t    *wfa_time_p;
    
  // For the Monte Carlo data    
  event_mc_t    *event_mc_p;
  etrack_mc_t   *etrack_mc_p;
  etrack_mc_t   *this_etrack_mc_p;
  track_mc_t    *track_mc_p;
  vertex_mc_t   *vtx_mc_p;
  match_track_t *match_track_p;
  etrack_t      *etrack_p;

  static Int_t   nSkip = 0;

  T49ParticleRoot       *track;
  TClonesArray          *PrimaryTrackArray;
  TClonesArray          *SecondaryTrackArray;
  TObjArray             *SecVtxList;

  // For the Monte Carlo data
  T49ParticleMCRoot     *mcTrack;
  T49VertexMCRoot       *mcVertex;
  TClonesArray          *MCTrackArray;
  TObjArray             *MCVertexList;

  event_p = (event_t *) get_objects("event",&Dummy);
  if (event_p == NULL) {
    cerr << "No event_t in this event" << endl;
    return kFALSE;
  }

  event_mc_p = (event_mc_t *) get_objects("event_mc",&Dummy);
  if (event_mc_p == NULL) {
    cerr << "No event_mc_t in this event" << endl;
    return kFALSE;
  }

  all_p    = (tpc_t *) get_objects("all"   ,&num   );
  if (all_p == NULL) {
    cerr << "No all structure in this event" << endl;
    return kFALSE;
  }

  // Used to create the links between the different lists
  Int_t         NRTrackSave = TMath::Max( 250,all_p->n_rtrack + 1);
  Int_t         NSTrackSave = TMath::Max( 250,all_p->n_vertex * 3);
  Int_t         NVtxSave    = TMath::Max(5000,all_p->n_vertex * 3);
  Int_t         NVtxMCSave  = event_mc_p->n_vertex + 1;
  rtrack_t    **rtrack_save = new rtrack_t*[NRTrackSave];
  track_t     **strack_save = new track_t*[NSTrackSave];
  vertex_t    **vtx_save    = new vertex_t*[NVtxSave];
  vertex_mc_t **vtxMC_save  = new vertex_mc_t*[NVtxMCSave];
  for (i = 0; i < NRTrackSave; i++) rtrack_save[i] = NULL;
  for (i = 0; i < NSTrackSave; i++) strack_save[i] = NULL;
  for (i = 0; i < NVtxSave;    i++) vtx_save[i]    = NULL;
  for (i = 0; i < NVtxMCSave;  i++) vtxMC_save[i]  = NULL;
 
  if (nSkip > 0) {
    printf("Skip also the next event\n");
    nSkip--;
    return kFALSE;
  }

  // Skip problematic events
  for (Int_t iFound = 0; iFound < fSkipFound; iFound++) {
    if ((event_p->n_run   == fSkipRun[iFound]) &&
        (event_p->n_event == fSkipEvt[iFound])) {
      printf("Skip %d events starting with run %d, event %d.\n"
            ,fSkipCnt[iFound],event_p->n_run,event_p->n_event);
      nSkip = fSkipCnt[iFound];
      nSkip--;
      return kFALSE;
    }
  }

  if (fVerbose) {
    printf("Fill event_t data (%d)\n",event_p->n_event);
  }
  fEventMC->SetNRun(event_p->n_run);
  fEventMC->SetNEvent(event_p->n_event);
  fEventMC->SetDate(event_p->date);
  fEventMC->SetTime(event_p->time);
  fEventMC->SetTriggerMask(event_p->mask_trig);
  if (event_p->veto_p) {
    fEventMC->SetEveto(event_p->veto_p->eveto);
  }

  if (fVerbose) {
    printf("Copy monitor_tpc_t data\n");
  }
  moni_p = (monitor_tpc_t *) get_objects("monitor_vt1",&Dummy);
  if (moni_p) {
    FillMonitor(fEventMC->GetMonitorTPC( 2),moni_p);
  }
  moni_p = (monitor_tpc_t *) get_objects("monitor_vt2",&Dummy);
  if (moni_p) {
    FillMonitor(fEventMC->GetMonitorTPC( 4),moni_p);
  }
  moni_p = (monitor_tpc_t *) get_objects("monitor_mtr",&Dummy);
  if (moni_p) {
    FillMonitor(fEventMC->GetMonitorTPC( 8),moni_p);
  }
  moni_p = (monitor_tpc_t *) get_objects("monitor_mtl",&Dummy);
  if (moni_p) {
    FillMonitor(fEventMC->GetMonitorTPC(16),moni_p);
  }

  if (fVerbose) {
    printf("Copy main vertex position\n");
  }
  main_vtx = (vertex_t *) all_p->main_vertex_p;
  if (fVerbose) {
    printf("Fill Main vertex object\n");
  }
  if(main_vtx) {
    fEventMC->SetMainVertexExists(kTRUE);
    FillVertex(fEventMC->GetMainVertex(),main_vtx);
  }
  else {
    fEventMC->SetMainVertexExists(kFALSE);
  }

  bpd_vtx_p = (bpd_vertex_t *) get_objects("bpd_vertex",&Dummy);
  if (fVerbose) {
    printf("Fill BPD vertex object\n");
  }
  if(bpd_vtx_p) {
    fEventMC->SetBPDVertexExists(kTRUE);
    FillBPDVertex(fEventMC->GetBPDVertex(),bpd_vtx_p);
  }
  else {
    fEventMC->SetBPDVertexExists(kFALSE);
  }

  prim_vtx = (vertex_t *) get_objects("vertex_fit",&Dummy);
  if (fVerbose) {
    printf("Fill Fitted vertex object\n");
  }
  if(prim_vtx) {
    fEventMC->SetFittedVertexExists(kTRUE);
    FillVertex(fEventMC->GetFittedVertex(),prim_vtx);
  }
  else {
    fEventMC->SetFittedVertexExists(kFALSE);
  }

  if ((fEventMC->GetNEvent() % 100 == 0) || (fVerbose)) {
    sprintf(name,"event_%d_%d",fEventMC->GetNRun(),fEventMC->GetNEvent());
    sprintf(name,"Run %d, Event %d",fEventMC->GetNRun(),fEventMC->GetNEvent());
  }

  if (fVerbose) {
    printf("Copy the WFA data\n");
  }
  wfa_p = (wfa_t *) get_objects("wfa",&Dummy);
  if (wfa_p) {
    fEventMC->SetWfaNbeam(wfa_p->n_beam);
    fEventMC->SetWfaNinter(wfa_p->n_inter);
  }
  else {
    if (fVerbose) {
      printf("No wfa_t found\n");
    }
  }
  if(wfa_p) {
    wfa_time_p = (wfa_time_t *) get_objects("wfa_beam_time",&Dummy);
    if(wfa_time_p)
      for(Int_t k=0; k<wfa_p->n_beam; k++)
	fEventMC->SetWfaBeamTime(k, (wfa_time_p[k]).wfa_time);
    else {
      if (fVerbose) {
	printf("No wfa_beam_time found\n");
      }
    }
  }
  if (wfa_p) {
    wfa_time_p = (wfa_time_t *) get_objects("wfa_inter_time",&Dummy);
    if (wfa_time_p)
      for(Int_t k=0; k<wfa_p->n_inter; k++)
	fEventMC->SetWfaInterTime(k, wfa_time_p->wfa_time);
    else {
      if (fVerbose) {
	printf("No wfa_inter_time found\n");
      }
    }
  }

  if (fVerbose) {
    printf("Copy beam_t data\n");
  }
  beam_p = event_p->beam_p;
  if (!beam_p) beam_p = (beam_t *) get_objects("beam",&Dummy);
  if (beam_p && fEventMC->GetBeam()) {
    FillBeam(fEventMC->GetBeam(),beam_p);
  }
  else {
    printf("event_t->beam_p = %p\n", event_p->beam_p);
  }

  if (fVerbose) {
    printf("Copy ring data\n");
  }
  ring_p = event_p->ring_p;
  if (!ring_p) ring_p = (ring_t *) get_objects("ring",&Dummy);
  if (ring_p && fEventMC->GetRing()) {
    FillRing(fEventMC->GetRing(),ring_p);  
  }

  if (fVerbose) {
    printf("Copy veto data\n");
  }
  veto_p = event_p->veto_p;
  if (!veto_p) veto_p = (veto_t *) get_objects("veto",&Dummy);
  veto_ped_p = (pedestal_t *) get_objects("veto_ped",&Dummy);
  if (veto_p && veto_ped_p && fEventMC->GetVeto()) {
    FillVeto(fEventMC->GetVeto(),veto_p,veto_ped_p);
  }

  if (fFillPrimary) {

    if (fVerbose) {
      printf("Copy primary track data\n");
    }
    fNumTracks  = 0;
    TotalTracks = 0;

    track_ref = (g_track_ref_t *) get_objects("g_track_ref",&num);
    ptr_list  = (g_ptr_list_t  *) get_objects("g_ptr_list",&num2);

    if (!(PrimaryTrackArray = fEventMC->GetPrimaryParticles())) {
      printf("No primary TrackArray found in event\n");
    }

    if (fVerbose) {
      printf("Found g_track_ref_t at %p (%d tracks)\n", track_ref, num);
    }
    if (num == num2) {

      for (i = 0; i < num; i++) {

        ptr_list = track_ref[i].g_ptr_list_p;
        if (!ptr_list) {
          fprintf(stderr,"ptr_list = NULL for %d\n",i);
          break;
        }
        if (fVerbose > 1) {
          printf("%d: %p\n", i, track_ref[i].g_ptr_list_p);
        }

        rtrack_p = ptr_list->rtrack_glb_p;
        if (fVerbose > 1) {
          printf("rtrack: %p\n", rtrack_p);
        }
        if (!rtrack_p) {
          fprintf(stderr,"rtrack_p = NULL for %d\n",i);
          break;
        }

        track_p = rtrack_p->first_track_p;
        if (fVerbose > 1) {
          printf("track: %p\n", track_p);
        }
        if (!track_p) {
          fprintf(stderr,"track_p = NULL for %d\n",i);
          break;
        }

        vtx_p = track_p->start_vertex_p;
        if (fVerbose > 1) {
          printf("vtx: %p\n", vtx_p);
        }
        if (!vtx_p) {
          fprintf(stderr,"vtx_p = NULL for %d\n",i);
          break;
        }           
        
        if ((track_p->pz           >    0) &&
            (rtrack_p->n_point     >   10) &&
            (track_p->iflag       ==    0) &&
            (track_p->rtrack_p    != NULL)) {

          // Copy tracks attached to the main vertex
          if (vtx_p->id_vtx == 0) {

            if (fVerbose > 1) {
              printf("Alloc T49ParticleRoot for main-vertex track\n");
            }
            track = new (PrimaryTrackArray->operator[](fNumTracks)) T49ParticleRoot();
            track->SetLabel(fNumTracks);
            if (fVerbose > 1) {
              printf("Call FillTrack\n");
            }
            FillTrack(track,&track_ref[i],kTRUE);
            if (fVerbose > 1) {
              printf("FillTrack finished\n");
            }

            // Save the rtrack-pointer of this index for later
            if (fNumTracks < NRTrackSave) {
              rtrack_save[fNumTracks] = rtrack_p;
            }
            else {
              printf("Boundary error: fNumTracks = %d NRTrackSave = %d\n"
                    ,fNumTracks,NRTrackSave);
  	    }

            fNumTracks++;

          }

        }

        TotalTracks++;

      }

    }
    else {
      printf("num = %d, num2 = %d, abort event\n",num,num2);
    }

    if (fVerbose) {
      printf("finished copying main-vertex tracks\n");
    }

  }

  // Copy the secondary vertices and the corresponding daughter tracks
  Int_t  numVtx    = 0;
  Bool_t mainFound = kFALSE;

  // Get pointer to the track_mc_t structure for LT data
  track_mc_p = (track_mc_t *) get_objects("track_mc",&num);

  if (fFillSecondary) {

    if (fVerbose) {
      if (fFillXis) {
        printf("Copy V0 and Xi data\n");
      }
      else {
        printf("Copy V0 data\n");
      }
      if (fCheckLT) printf("Copy LT data\n");
    }
    // The TClonesArray for the non-vertex tracks
    if (!(SecondaryTrackArray = fEventMC->GetSecondaryParticles())) {
      printf("No secondary track array found in event\n");
    }
    // The TObjArray for the vertices
    if (!(SecVtxList = fEventMC->GetSecondaryVertices())) {
      printf("No secondary vertex list found in event\n");
    }

    fNumSecTracks   = 0;
    fNumSecVertices = 0;

    // New version of the loop to work around bug in xifind
    if ((all_p    = (tpc_t *)    get_objects("all"   ,&num   )) &&
        (vtx_list = (vertex_t *) get_objects("vertex",&numVtx)) &&
        (all_p->vertex_p)) {

      if (fFullSecondary) {
        track_ref = (g_track_ref_t *) get_objects("g_track_ref",&num);
        ptr_list  = (g_ptr_list_t  *) get_objects("g_ptr_list",&num2);
        if (num != num2) {
          printf("num = %d, num2 = %d, abort event\n",num,num2);
          return kFALSE;
	}
      }

      for (Int_t iVtx = all_p->vertex_p - vtx_list; iVtx < numVtx; iVtx++) {

        vtx_p = &vtx_list[iVtx];                                                  

	// Only good V0 vertices
        if (CheckV0(vtx_p)) {

          if (fVerbose > 1) {
            printf("Create T49VertexRoot for V0\n");
          }
          T49VertexRoot *SecVertex = new T49VertexRoot(kFALSE);
          if (fVerbose > 1) {
            printf("Fill secondary vertex object\n");
          }
          FillSecVertex(SecVertex,vtx_p);
          if (fVerbose > 1) {
            printf("Add to vertex list\n");
          }
          SecVtxList->Add(SecVertex);

          // Get the first daughter track
          track_p  = vtx_p->daughter_p;
          rtrack_p = track_p->rtrack_p;
          if (fVerbose > 1) {
            printf("Alloc T49ParticleRoot for first daughter track\n");
          }
          track = new (SecondaryTrackArray->operator[](fNumSecTracks)) T49ParticleRoot();
          track->SetLabel(fNumSecTracks);
	  // Save the index to the first particle in SecondaryTrackArray       
          SecVertex->SetSecondaryIndex(0,fNumSecTracks);
          if (fVerbose > 1) {
            printf("First particle index (non-vtx) = %d\n",fNumSecTracks);
	  }

          // Save the index to the first particle in PrimaryTrackArray (main-vtx fit)       
          mainFound = kFALSE;
          for (i = 0; i < fNumTracks; i++) {
            if (rtrack_p == rtrack_save[i]) {
              SecVertex->SetPrimaryIndex(0,i);
              if (fVerbose > 1) {
                printf("First particle index (main-vtx) = %d\n",i);
	      }
              mainFound = kTRUE;
              break;
	    }
          }

          if (fFullSecondary && !mainFound) {

            if (fVerbose > 1) {
              printf("Call FillTrack for first daughter track\n");
            }
            // Find the global track ref
            Bool_t track_ref_found = kFALSE;
            for (Int_t j = 0; j < num; j++) {
              ptr_list = track_ref[j].g_ptr_list_p;
              if (!ptr_list) {
                fprintf(stderr,"ptr_list = NULL for %d\n",i);
                break;
              }
              if (rtrack_p == ptr_list->rtrack_glb_p) {
                if (!track_ref_found) {
                  FillTrack(track,&track_ref[j],kFALSE);
                  FillSecTrack(track,track_p,kTRUE);
                  track_ref_found = kTRUE;
                  if (fVerbose > 1) {
                    printf("FillTrack finished\n");
                  }
		}
                else {
                  fprintf(stderr,"More than one g_track_ref_t found\n");
		}
	      }
	    }
            if (!track_ref_found) {
              fprintf(stderr,"No g_track_ref_t found\n");
	    }

	  }
          else {

            if (fVerbose > 1) {
              printf("Call FillSecTrack for first daughter track. mainFound = %d\n"
                    ,mainFound);
            }
            FillSecTrack(track, track_p, mainFound);
            if (fVerbose > 1) {
              printf("FillSecTrack finished\n");
            }

	  }

          if (rtrack_p->qpxz > 0) {
            if (fVerbose > 1) {
              printf("First track is positive\n");
            }
            SecVertex->SetFirstTrackIsPositive(kTRUE);
	  }
	  else {
            if (fVerbose > 1) {
              printf("First track is negative\n");
            }
            SecVertex->SetFirstTrackIsPositive(kFALSE);
          }
          // Save the track-pointer of this index for later
          if (fNumSecTracks < NSTrackSave) {
            strack_save[fNumSecTracks] = track_p;
	  }
          else {
            printf("Boundary error: fNumSecTracks = %d NSTrackSave = %d\n"
                  ,fNumSecTracks,NSTrackSave);
	  }
          fNumSecTracks++;

          // Get the second daughter track
          track_p  = track_p->next_daughter_p;
          rtrack_p = track_p->rtrack_p;
          if (fVerbose > 1) {
            printf("Alloc T49ParticleRoot for second daughter track\n");
          }
          track = new (SecondaryTrackArray->operator[](fNumSecTracks)) T49ParticleRoot();
          track->SetLabel(fNumSecTracks);
	  // Save the index to the second particle in SecondaryTrackArray       
          SecVertex->SetSecondaryIndex(1,fNumSecTracks);
          if (fVerbose > 1) {
            printf("Second particle index (non-vtx) = %d\n",fNumSecTracks);
	  }

          // Save the index to the second particle in PrimaryTrackArray (main-vtx fit)       
          mainFound = kFALSE;
          for (i = 0; i < fNumTracks; i++) {
            if (rtrack_p == rtrack_save[i]) {
              SecVertex->SetPrimaryIndex(1,i);
              if (fVerbose > 1) {
                printf("Second particle index (main-vtx) = %d\n",i);
	      }
              mainFound = kTRUE;
              break;
	    }
          }

          if (fFullSecondary && !mainFound) {

            if (fVerbose > 1) {
              printf("Call FillTrack for second daughter track\n");
            }
            // Find the global track ref
            Bool_t track_ref_found = kFALSE;
            for (Int_t j = 0; j < num; j++) {
              ptr_list = track_ref[j].g_ptr_list_p;
              if (!ptr_list) {
                fprintf(stderr,"ptr_list = NULL for %d\n",i);
                break;
              }
              if (rtrack_p == ptr_list->rtrack_glb_p) {
                if (!track_ref_found) {
                  FillTrack(track,&track_ref[j],kFALSE);
                  FillSecTrack(track,track_p,kTRUE);
                  track_ref_found = kTRUE;
                  if (fVerbose > 1) {
                    printf("FillTrack finished\n");
                  }
		}
                else {
                  fprintf(stderr,"More than one g_track_ref_t found\n");
		}
	      }
	    }
            if (!track_ref_found) {
              fprintf(stderr,"No g_track_ref_t found\n");
	    }

	  }
          else {

            if (fVerbose > 1) {
              printf("Call FillSecTrack for second daughter track. mainFound = %d\n"
                    ,mainFound);
            }
            FillSecTrack(track,track_p,mainFound);
            if (fVerbose > 1) {
              printf("FillSecTrack finished\n");
            }

	  }
          // Save the track-pointer of this index for later
          if (fNumSecTracks < NSTrackSave) {
            strack_save[fNumSecTracks] = track_p;
	  }
          else {
            printf("Boundary error: fNumSecTracks = %d NSTrackSave = %d\n"
                  ,fNumSecTracks,NSTrackSave);
	  }
          fNumSecTracks++;

          fNumSecVertices++;            

	}

        // LT vertices
        if (fCheckLT && CheckLTVtx(vtx_p)) {

          if (fVerbose > 1) {
            printf("Create T49VertexRoot for LT\n");
          }
          T49VertexRoot *SecVertex = new T49VertexRoot(kFALSE);
          if (fVerbose > 1) {
            printf("Fill secondary vertex object with LT\n");
          }
          FillSecVertex(SecVertex,vtx_p);
          if (fVerbose > 1) {
            printf("Add to vertex list\n");
          }
          SecVtxList->Add(SecVertex);

          // Save the vertex-pointer of this index for later
          if (fNumSecVertices < NVtxSave) {
            vtx_save[fNumSecVertices] = vtx_p;
          }
          else {
            printf("Boundary error: fNumSecVertices = %d NVtxSave = %d\n"
                  ,fNumSecVertices,NVtxSave);
  	  }

          // Get the first daughter track
          track_p  = vtx_p->daughter_p;
          rtrack_p = track_p->rtrack_p;
          if (fVerbose > 1) {
            printf("Alloc T49ParticleRoot for first daughter track\n");
          }
          track = new (SecondaryTrackArray->operator[](fNumSecTracks)) T49ParticleRoot();
          track->SetLabel(fNumSecTracks);
	  // Save the index to the first particle in SecondaryTrackArray       
          SecVertex->SetSecondaryIndex(0,fNumSecTracks);
          if (fVerbose > 1) {
            printf("First particle index (non-vtx) = %d\n",fNumSecTracks);
	  }

          // Save the index to the first particle in PrimaryTrackArray (main-vtx fit)       
          mainFound = kFALSE;
//           for (i = 0; i < fNumTracks; i++) {
//             if (rtrack_p == rtrack_save[i]) {
//               SecVertex->SetPrimaryIndex(0,i);
//               if (fVerbose > 1) {
//                 printf("First particle index (main-vtx) = %d\n",i);
// 	      }
//               mainFound = kTRUE;
//               break;
// 	    }
//           }

	  if (fVerbose > 1) {
	    printf("rtrack flag = %d\n",rtrack_p->iflag);
	  }
	  
	  if(rtrack_p->iflag<0) { //check if it is a MC simulated propagated track to LT
	    if (fVerbose > 1) {
	      printf("Call FillMCTrackLT for first simulated daughter track\n");
	    }
	    FillSecTrack(track, track_p, mainFound);
	    int index = -rtrack_p->iflag - 1;
	    FillMCTrackLT(track, &(track_mc_p[index]) );
	  }
	  else {
	    if (fFullSecondary && !mainFound) {

	      if (fVerbose > 1) {
		printf("Call FillTrack for first daughter track\n");
	      }
	      // Find the global track ref
	      Bool_t track_ref_found = kFALSE;
	      for (Int_t j = 0; j < num; j++) {
		ptr_list = track_ref[j].g_ptr_list_p;
		if (!ptr_list) {
		  fprintf(stderr,"ptr_list = NULL for %d\n",i);
		  break;
		}
		if (rtrack_p == ptr_list->rtrack_glb_p) {
		  if (!track_ref_found) {
		    FillTrackLT(track,&track_ref[j],track_p,kTRUE);
		    FillSecTrack(track,track_p,kTRUE);
		    track_ref_found = kTRUE;
		    if (fVerbose > 1) {
		      printf("FillTrack finished\n");
		    }
		  }
		  else {
		    fprintf(stderr,"More than one g_track_ref_t found\n");
		  }
		}
	      }
	      if (!track_ref_found) {
		fprintf(stderr,"No g_track_ref_t found\n");
	      }

	    }
	    else {

	      if (fVerbose > 1) {
		printf("Call FillSecTrack for first daughter track. mainFound = %d\n"
		       ,mainFound);
	      }
	      FillSecTrack(track, track_p, mainFound);
	      if (fVerbose > 1) {
		printf("FillSecTrack finished\n");
	      }
	    }
	  }

          if (rtrack_p->qpxz > 0) {
            if (fVerbose > 1) {
              printf("First track is positive\n");
            }
            SecVertex->SetFirstTrackIsPositive(kTRUE);
	  }
	  else {
            if (fVerbose > 1) {
              printf("First track is negative\n");
            }
            SecVertex->SetFirstTrackIsPositive(kFALSE);
          }

          // fNumSecTracks++;

	  // Get the second daughter track
 //           track_p  = track_p->next_daughter_p;
 //           rtrack_p = track_p->rtrack_p;
           if (fVerbose > 1) {
             printf("Alloc T49ParticleRoot for second daughter track\n");
           }
           track = new (SecondaryTrackArray->operator[](fNumSecTracks)) T49ParticleRoot();
           track->SetLabel(fNumSecTracks);
 	  // Save the index to the second particle in SecondaryTrackArray       
           SecVertex->SetSecondaryIndex(1,fNumSecTracks);
           if (fVerbose > 1) {
             printf("Second particle index (non-vtx) = %d\n",fNumSecTracks);
 	  }

           // Save the index to the second particle in PrimaryTrackArray (main-vtx fit)       
           mainFound = kFALSE;
//            for (i = 0; i < fNumTracks; i++) {
//              if (rtrack_p == rtrack_save[i]) {
//                SecVertex->SetPrimaryIndex(1,i);
//                if (fVerbose > 1) {
//                  printf("Second particle index (main-vtx) = %d\n",i);
//  	      }
//                mainFound = kTRUE;
//                break;
//  	    }
//            }

           if(rtrack_p->iflag<0) { //check if it is a MC simulated propagated track to LT
	     if (fVerbose > 1) {
	       printf("Call FillMCTrackLT for second simulated daughter track.\n");
	     }
	     FillSecTrack(track, track_p, mainFound);
	     int index = -rtrack_p->iflag - 1;
	     FillMCTrackLT(track, &(track_mc_p[index]) );
	   }
	   else {
	     if (fFullSecondary && !mainFound) {

	       if (fVerbose > 1) {
		 printf("Call FillTrack for second daughter track\n");
	       }
	       // Find the global track ref
	       Bool_t track_ref_found = kFALSE;
	       for (Int_t j = 0; j < num; j++) {
		 ptr_list = track_ref[j].g_ptr_list_p;
		 if (!ptr_list) {
		   fprintf(stderr,"ptr_list = NULL for %d\n",i);
		   break;
		 }
		 if (rtrack_p == ptr_list->rtrack_glb_p) {
		   if (!track_ref_found) {
		     FillTrackLT(track,&track_ref[j],track_p,kTRUE);
		     FillSecTrack(track,track_p,kTRUE);
		     track_ref_found = kTRUE;
		     if (fVerbose > 1) {
		       printf("FillTrack finished\n");
		     }
		   }
		   else {
		     fprintf(stderr,"More than one g_track_ref_t found\n");
		   }
		 }
	       }
	       if (!track_ref_found) {
		 fprintf(stderr,"No g_track_ref_t found\n");
	       }

	     }
	     else {
	     
	       if (fVerbose > 1) {
		 printf("Call FillSecTrack for second daughter track. mainFound = %d\n"
			,mainFound);
	       }
	       FillSecTrack(track,track_p,mainFound);
	       if (fVerbose > 1) {
		 printf("FillSecTrack finished\n");
	       }
	  
	     }

	   }

	   // Save the track-pointer of this index for later
	   if (fNumSecTracks < NSTrackSave) {
	     strack_save[fNumSecTracks] = track_p;
	   }
	   else {
	     printf("Boundary error: fNumSecTracks = %d NSTrackSave = %d\n"
		    ,fNumSecTracks,NSTrackSave);
	   }
	   fNumSecTracks++;

	   fNumSecVertices++;            

	}

        // The Xis
        if ((fFillXis) && (CheckXi(vtx_p))) {

          if (fVerbose > 1) {
            printf("Create T49VertexRoot for Xis\n");
          }
          T49VertexRoot *SecVertex = new T49VertexRoot(kFALSE);
          if (fVerbose > 1) {
            printf("Fill secondary vertex object with Xi\n");
          }
          FillXiVertex(SecVertex,vtx_p);
          if (fVerbose > 1) {
            printf("Add to vertex list\n");
          }
          SecVtxList->Add(SecVertex);
          if (fNumSecVertices < NVtxSave) {
            vtx_save[fNumSecVertices] = vtx_p;
          }

          // Get the first daughter track
          track_p  = vtx_p->daughter_p;
	  // If Xi was found by MTV0 the charged track is the second one
          if (vtx_p->iflag & 0x2000) {
            track_p = track_p->next_daughter_p;
            if (!track_p) {
              printf("Error: No charged track from MTV0-Xi\n");
              continue;
	    }
	  }
          rtrack_p = track_p->rtrack_p;

          // Reconstruct the linked list for the tracks produced by the xifinder
          track_tmp_p = rtrack_p->first_track_p;
	  while (track_tmp_p->next_track_p) {
            track_tmp_p = track_tmp_p->next_track_p;
	  }
          track_tmp_p->next_track_p = track_p;

          if (fVerbose > 1) {
            printf("Alloc T49ParticleRoot for first Xi daughter track\n");
          }
          track = new (SecondaryTrackArray->operator[](fNumSecTracks)) T49ParticleRoot();
          track->SetLabel(fNumSecTracks);
	  // Save the index to the first particle in SecondaryTrackArray       
          SecVertex->SetSecondaryIndex(0,fNumSecTracks);
          if (fVerbose > 1) {
            printf("First particle index (non-vtx) = %d\n",fNumSecTracks);
	  }

          // Save the index to the first particle in PrimaryTrackArray (main-vtx fit)       
          mainFound = kFALSE;
          if (rtrack_p) {
            for (i = 0; i < fNumTracks; i++) {
              if (rtrack_p == rtrack_save[i]) {
                SecVertex->SetPrimaryIndex(0,i);
                if (fVerbose > 1) {
                  printf("First particle index (main-vtx) = %d\n",i);
	        }
                mainFound = kTRUE;
                break;
	      }
	    }
          }

          if (fFullSecondary && !mainFound && rtrack_p) {

            if (fVerbose > 1) {
              printf("Call FillTrack for first Xi daughter track\n");
            }
            // Find the global track ref
            Bool_t track_ref_found = kFALSE;
            for (Int_t j = 0; j < num; j++) {
              ptr_list = track_ref[j].g_ptr_list_p;
              if (!ptr_list) {
                fprintf(stderr,"ptr_list = NULL for %d\n",i);
                break;
              }
              if (rtrack_p == ptr_list->rtrack_glb_p) {
                if (!track_ref_found) {
                  FillTrack(track,&track_ref[j],kFALSE);
                  FillSecTrack(track,track_p,kTRUE);
                  track_ref_found = kTRUE;
                  if (fVerbose > 1) {
                    printf("FillTrack finished\n");
                  }
		}
                else {
                  fprintf(stderr,"More than one g_track_ref_t found\n");
		}
	      }
	    }
            if (!track_ref_found) {
              fprintf(stderr,"No g_track_ref_t found\n");
	    }

	  }
          else {

            if (fVerbose > 1) {
              printf("Call FillSecTrack for first Xi daughter track. mainFound = %d\n"
                    ,mainFound);
            }
            FillSecTrack(track, track_p, mainFound);
            if (fVerbose > 1) {
              printf("FillSecTrack finished\n");
            }

	  }

          if (track_p->qpxz > 0) {
            if (fVerbose > 1) {
              printf("First track is positive\n");
            }
            SecVertex->SetFirstTrackIsPositive(kTRUE);
	  }
	  else {
            if (fVerbose > 1) {
              printf("First track is negative\n");
            }
            SecVertex->SetFirstTrackIsPositive(kFALSE);
          }
          // Save the track-pointer of this index for later
          if (fNumSecTracks < NSTrackSave) {
            strack_save[fNumSecTracks] = track_p;
	  }
          else {
            printf("Boundary error: fNumSecTracks = %d NSTrackSave = %d\n"
                  ,fNumSecTracks,NSTrackSave);
	  }
          fNumSecTracks++;

          // Get the second daughter track (neutral track / V0)
	  // If Xi was found by MTV0 the neutral track is the first one
          if (vtx_p->iflag & 0x2000) {
            track_p  = vtx_p->daughter_p;
	  }
          else {
            track_p  = track_p->next_daughter_p;
	  }
          rtrack_p = track_p->rtrack_p;
          if (fVerbose > 1) {
            printf("Alloc T49ParticleRoot for second Xi daughter track\n");
          }
          track = new (SecondaryTrackArray->operator[](fNumSecTracks)) T49ParticleRoot();
          track->SetLabel(fNumSecTracks);
	  // Save the index to the second particle in SecondaryTrackArray       
          SecVertex->SetSecondaryIndex(1,fNumSecTracks);
          if (fVerbose > 1) {
            printf("Second particle index (non-vtx) = %d\n",fNumSecTracks);
	  }

	  // No rtrack information for the Lambda track 
          mainFound = kFALSE;
          if (fVerbose > 1) {
            printf("Call FillSecTrack for second Xi daughter track. mainFound = %d\n"
                  ,mainFound);
          }
          FillSecTrack(track,track_p,mainFound);
          if (fVerbose > 1) {
            printf("FillSecTrack finished\n");
          }
          // Save the track-pointer of this index for later
          if (fNumSecTracks < NSTrackSave) {
            strack_save[fNumSecTracks] = track_p;
	  }
          else {
            printf("Boundary error: fNumSecTracks = %d NSTrackSave = %d\n"
                  ,fNumSecTracks,NSTrackSave);
	  }
          fNumSecTracks++;

          fNumSecVertices++;            

	  // Find the Lambda vertex and add it to the list if neccessary
          vtx_stop_p = track_p->stop_vertex_p;
          if (vtx_stop_p) {
            // Check whether this V0 is already copied
            Bool_t foundV0 = kFALSE;
            Int_t  indexV0 = 0;
            for (i = 0; i < fNumSecVertices; i++) {
              if (vtx_stop_p == vtx_save[i]) {
                foundV0 = kTRUE;
                indexV0 = i;
                break;
	      }
            }
            if (foundV0) {
              track->SetStopVertexIndex(indexV0);
	    }
            else {
              track->SetStopVertexIndex(fNumSecVertices);
              // Add the V0 to the secondary vertex list
              T49VertexRoot *SecVertex = new T49VertexRoot(kFALSE);
              FillSecVertex(SecVertex,vtx_stop_p);
              track_p  = vtx_stop_p->daughter_p;
              rtrack_p = track_p->rtrack_p;
              track_tmp_p = rtrack_p->first_track_p;
	      while (track_tmp_p->next_track_p) {
                track_tmp_p = track_tmp_p->next_track_p;
	      }
              track_tmp_p->next_track_p = track_p;
              track = new (SecondaryTrackArray->operator[](fNumSecTracks)) T49ParticleRoot();
              track->SetLabel(fNumSecTracks);
              SecVertex->SetSecondaryIndex(0,fNumSecTracks);
              Bool_t track_ref_found = kFALSE;
              for (Int_t j = 0; j < num; j++) {
                ptr_list = track_ref[j].g_ptr_list_p;
                if (rtrack_p == ptr_list->rtrack_glb_p) {
                  if (!track_ref_found) {
                    FillTrack(track,&track_ref[j],kFALSE);
                    FillSecTrack(track,track_p,kTRUE);
                    track_ref_found = kTRUE;
		  }
	        }
	      }
              if (rtrack_p->qpxz > 0) {
                SecVertex->SetFirstTrackIsPositive(kTRUE);
	      }
	      else {
                SecVertex->SetFirstTrackIsPositive(kFALSE);
              }
              if (fNumSecTracks < NSTrackSave) {
                strack_save[fNumSecTracks] = track_p;
	      }
              else {
                printf("Boundary error: fNumSecTracks = %d NSTrackSave = %d\n"
                      ,fNumSecTracks,NSTrackSave);
	      }
              fNumSecTracks++;
              track_p  = track_p->next_daughter_p;
              rtrack_p = track_p->rtrack_p;
              track_tmp_p = rtrack_p->first_track_p;
	      while (track_tmp_p->next_track_p) {
                track_tmp_p = track_tmp_p->next_track_p;
	      }
              track_tmp_p->next_track_p = track_p;
              track = new (SecondaryTrackArray->operator[](fNumSecTracks)) T49ParticleRoot();
              track->SetLabel(fNumSecTracks);
              SecVertex->SetSecondaryIndex(1,fNumSecTracks);
              track_ref_found = kFALSE;
              for (Int_t j = 0; j < num; j++) {
                ptr_list = track_ref[j].g_ptr_list_p;
                if (rtrack_p == ptr_list->rtrack_glb_p) {
                  if (!track_ref_found) {
                    FillTrack(track,&track_ref[j],kFALSE);
                    FillSecTrack(track,track_p,kTRUE);
                    track_ref_found = kTRUE;
		  }
	        }
	      }
              if (fNumSecTracks < NSTrackSave) {
                strack_save[fNumSecTracks] = track_p;
	      }
              else {
                printf("Boundary error: fNumSecTracks = %d NSTrackSave = %d\n"
                      ,fNumSecTracks,NSTrackSave);
	      }
              fNumSecTracks++;
              SecVtxList->Add(SecVertex);
              if (fNumSecVertices < NVtxSave) {
                vtx_save[fNumSecVertices] = vtx_stop_p;
              }
              fNumSecVertices++;            
	    }

	  }
          else {
            fprintf(stderr,"No stop vertex found\n");
	  } 

	}

        //vtx_p = vtx_p->next_p;
        //vtx_p++;

      }

    }
    else {
      printf("No vertices found\n");
    }
      
    if (fVerbose) {
      printf("Finished copying V0 data\n");
    }

  }

  // Copy MC data with the links to the matching reconstructed tracks
  if (fFillMCTracks) {

    fNumMCTracks   = 0;
    fNumMCVertices = 0;

    if (fVerbose) {
      printf("Copy Monte Carlo data\n");
    }
    // The TClonesArray for the MC tracks
    if (!(MCTrackArray  = fEventMC->GetMCParticles())) {
      printf("No Monte Carlo track array found in event\n");
    }
    // The TObjArray for the MC vertices
    if (!(MCVertexList  = fEventMC->GetMCVertices())) {
      printf("No Monte Carlo vertex list found in event\n");
    }

    // Get pointer to the etrack_mc_t structure 
    etrack_mc_p = (etrack_mc_t *) get_objects("etrack_mc",&num);
    if (fVerbose > 1) printf("etrack_mc_p = %p, num = %d\n", etrack_mc_p, num);

//     // Get pointer to the track_t structure
//     int ntrack = 0;
//     track_t       *track_list = (track_t *) get_objects("track",&ntrack);
//     if (fVerbose > 1) printf("track = %p, num = %d\n", track_list, ntrack);
//     track_t    **ltrack_save = new track_t*[ntrack];
//     for (i = 0; i < ntrack; i++) {
//       ltrack_save[i] = (CheckLTVtx(track_list[i].start_vertex_p)) ? &(track_list[i]) : NULL;
//     }

    // Loop over all MC tracks.
    for (Int_t inum = 0; inum < num; inum++) {

      // Get pointer to the MC track
      track_mc_p = etrack_mc_p[inum].track_p;
      this_etrack_mc_p = &(etrack_mc_p[inum]);

      // Create a new MC track and add it to the track array
      if (fVerbose > 1) printf("Create T49ParticleMCRoot for pid %d\n",track_mc_p->pid);
      mcTrack = new (MCTrackArray->operator[](fNumMCTracks)) T49ParticleMCRoot();	

      // Fill the MC track
      FillMCTrack(mcTrack,track_mc_p);

      if (fVerbose > 1) {
	printf("match_track_p[%d] = %p\n", inum, etrack_mc_p[inum].match_p);
      }

      // Set the links to the matching reconstructed tracks
      Int_t iMatchPri = 0;
      Int_t iMatchSec = 0;
      // Loop through all matched global rtracks
      Int_t iMatch    = 0;
      for (match_track_p = etrack_mc_p[inum].match_p;
           match_track_p;
           match_track_p = match_track_p->next_match_p) {
        
	iMatch++;
        if (iMatch > match_track_p->n_match) break;

        if ((etrack_p  = match_track_p->etrack_p  ) &&
            (track_ref = etrack_p->gtrack_p       ) &&
            (ptr_list  = track_ref->g_ptr_list_p  ) &&
            (rtrack_p  = ptr_list->rtrack_glb_p   )) {

	  int i =0;
	  // Loop through all tracks associated to this rtrack
          for (track_p = rtrack_p->first_track_p;
               track_p;
               track_p = track_p->next_track_p) {
            vtx_p = track_p->start_vertex_p;

            if (vtx_p->id_vtx == 0) {
              if (fVerbose > 1) printf("Looking for matched primary track\n");
              for (i = 0; i < fNumTracks; i++) {
                if (rtrack_p == rtrack_save[i]) {
                  //mcTrack->SetPriMatched(i,etrack_p->n_pnt_match);
                  mcTrack->SetPriMatched(i,match_track_p->n_pnt_common);
                  iMatchPri++;
                  if (fVerbose > 1)
                    printf("Found matched primary track. Index = %d\n",i);
		}
	      }
	    }
            else {
              if (fVerbose > 1) printf("Looking for matched secondary track\n");
              for (i = 0; i < fNumSecTracks; i++) {
                if (track_p == strack_save[i]) {
                  //mcTrack->SetSecMatched(i,etrack_p->n_pnt_match);
                  mcTrack->SetSecMatched(i,match_track_p->n_pnt_common);
                  iMatchSec++;
                  if (fVerbose > 1)
                    printf("Found matched secondary track. Index = %d\n",i);
	        }
	      }
	    }
	  }

	  //LT data matching
	  if (fCheckLT) {

	    for(int k=0; k<numVtx; k++) {
	      if(CheckLTVtx( &(vtx_list[k]) ))	      {

		track_p = vtx_list[k].daughter_p;
		if(rtrack_p == track_p->rtrack_p) {

		  if (fVerbose > 1) printf("Looking for matched track for LT data\n");
		  for (i = 0; i < fNumSecTracks; i++) {
		    //		    if (track_p == ltrack_save[i]) {
		    if (track_p == strack_save[i]) {
		      mcTrack->SetSecMatched(i,match_track_p->n_pnt_common);
		      iMatchSec++;
		      if (fVerbose > 1)
			printf("Found matched secondary track for LT data. Index = %d, %p\n", i, track_p);
		    }
		  }
		}
	      }
	    }
	  }
	  //end LT matching

	}

      }
      if (fVerbose > 1) {
        printf("Found %d matched primary tracks\n"  ,iMatchPri);
        printf("Found %d matched secondary tracks\n",iMatchSec);  
      }

      // Save the start-vertex
      if (fVerbose > 1) printf("Save the start vertex\n");
      if ((vtx_mc_p = track_mc_p->start_vertex_p)) {
        Int_t indexVtx = -1;
        for (i = 0; i < fNumMCVertices; i++) {
          if (vtx_mc_p == vtxMC_save[i]) {
            indexVtx = i;
            break;
	  }
        }
        if (indexVtx >= 0) {
          mcVertex = (T49VertexMCRoot *) MCVertexList->At(indexVtx);
          if (fVerbose > 1) printf("Vertex already copied %d\n",indexVtx);
        }
        else {
          if (fVerbose > 1) printf("Create new T49VertexMCRoot\n");
          mcVertex = new T49VertexMCRoot();
          MCVertexList->Add(mcVertex);        
          mcVertex->SetX(vtx_mc_p->x);
          mcVertex->SetY(vtx_mc_p->y);
          mcVertex->SetZ(vtx_mc_p->z);
          vtxMC_save[fNumMCVertices] = vtx_mc_p;
          indexVtx = fNumMCVertices;
          fNumMCVertices++;
        }
        if      (mcVertex->GetFirstDaughter()  < 0) {
          if (fVerbose > 1) printf("Set first daughter\n");
          mcVertex->SetFirstDaughter(fNumMCTracks);
          mcTrack->SetStartVertex(indexVtx);
        }
        else if (mcVertex->GetSecondDaughter() < 0) {
          if (fVerbose > 1) printf("Set second daughter\n");
          mcVertex->SetSecondDaughter(fNumMCTracks);
          mcTrack->SetStartVertex(indexVtx);
        }
        else if (mcVertex->GetThirdDaughter()  < 0) {
          if (fVerbose > 1) printf("Set third daughter\n");
          mcVertex->SetThirdDaughter(fNumMCTracks);
          mcTrack->SetStartVertex(indexVtx);
        }
        else if (mcVertex->GetForthDaughter()  < 0) {
          if (fVerbose > 1) printf("Set forth daughter\n");
          mcVertex->SetForthDaughter(fNumMCTracks);
          mcTrack->SetStartVertex(indexVtx);
        }
        else {
          if (fVerbose) printf("More than four MC-daughters for MC-vertex %d\n"
                              ,indexVtx);
          mcTrack->SetStartVertex(indexVtx);
        }
      }
      else {
        if (fVerbose > 1) printf("No start vertex found\n");
      }

      // Save the stop vertex
      if (fVerbose > 1) printf("Save the stop vertex\n");
      if ((vtx_mc_p = track_mc_p->stop_vertex_p)) {
        Int_t indexVtx = -1;
        for (i = 0; i < fNumMCVertices; i++) {
          if (vtx_mc_p == vtxMC_save[i]) {
            indexVtx = i;
            break;
  	  }
        }
        if (indexVtx >= 0) {
          printf("More than one MC-parent for MC-vertex %d\n",indexVtx);
        }
        else {
          if (fVerbose > 1) printf("Create new T49VertexMCRoot\n");
          mcVertex = new T49VertexMCRoot();
          MCVertexList->Add(mcVertex);        
          mcVertex->SetX(vtx_mc_p->x);
          mcVertex->SetY(vtx_mc_p->y);
          mcVertex->SetZ(vtx_mc_p->z);
          if (fVerbose > 1) printf("Set parent\n");
          mcVertex->SetParent(fNumMCTracks);
          mcTrack->SetStopVertex(fNumMCVertices);
          vtxMC_save[fNumMCVertices] = vtx_mc_p;
          fNumMCVertices++;
        }
      }
      else {
        if (fVerbose > 1) printf("No stop vertex found\n");
      }

      fNumMCTracks++;

    }

    if (fVerbose) {
      printf("Finished copying MC data\n");
    }
    //    delete [] ltrack_save;
  }

  if (!(fEventMC->GetNEvent() % 100) || fVerbose) {
    printf("Multiplicity: %d out of %d (%g)\n"
          ,fNumTracks,TotalTracks,fEventMC->GetEveto());
    if (fFillSecondary) {
      printf("Secondary vertices (tracks): %d (%d)\n"
            ,fNumSecVertices,fNumSecTracks);
    }
    if (fFillMCTracks) {
      printf("MC tracks: %d, MC vertices: %d\n"
            ,fNumMCTracks,fNumMCVertices);
    }
  }

  delete [] rtrack_save;
  delete [] strack_save;
  delete [] vtx_save;
  delete [] vtxMC_save;

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDSMC::FillMCEvent() 
{
  //
  //  Copies only the MC data for the current event.
  //

  Int_t          i;
  Int_t          num;
  Int_t          Dummy;   

  event_mc_t    *event_mc_p;
  track_mc_t    *track_mc_p;
  vertex_mc_t   *vtx_mc_p;

  T49ParticleMCRoot     *mcTrack;
  T49VertexMCRoot       *mcVertex;
  TClonesArray          *MCTrackArray;
  TObjArray             *MCVertexList;

  fNumMCTracks   = 0;
  fNumMCVertices = 0;

  if (fVerbose) {
    printf("Copy Monte Carlo data\n");
  }

  // The TClonesArray for the MC tracks
  if (!(MCTrackArray  = fEventMC->GetMCParticles())) {
    printf("No Monte Carlo track array found in event\n");
  }
  // The TObjArray for the MC vertices
  if (!(MCVertexList  = fEventMC->GetMCVertices())) {
    printf("No Monte Carlo vertex list found in event\n");
  }

  event_mc_p = (event_mc_t *) get_objects("event_mc",&Dummy);
  if (event_mc_p == NULL) {
    cerr << "No event_mc_t in this event" << endl;
    return kFALSE;
  }

  // Used to create the links between the different lists
  Int_t         NVtxMCSave = event_mc_p->n_vertex + 1;
  vertex_mc_t **vtxMC_save = new vertex_mc_t*[NVtxMCSave];
  for (i = 0; i < NVtxMCSave;  i++) vtxMC_save[i] = NULL;

  if (fVerbose) {
    printf("Fill event_mc_t data (%d)\n",fVerbose);
  }
  fEventMC->SetNRun(event_mc_p->n_run);
  fEventMC->SetNEvent(event_mc_p->n_event);

  // Get pointer to the track_mc_t structure 
  track_mc_p = (track_mc_t *) get_objects("track_mc",&num);

  // Loop over all MC tracks.
  for (Int_t inum = 0; inum < num; inum++) {

    // Create a new MC track and add it to the track array
    if (fVerbose > 1) printf("Create T49ParticleMCRoot\n");
    mcTrack = new (MCTrackArray->operator[](fNumMCTracks)) T49ParticleMCRoot();	

    // Fill the MC track
    FillMCTrack(mcTrack,track_mc_p);

    // Save the start-vertex
    if (fVerbose > 1) printf("Save the start vertex\n");
    if ((vtx_mc_p = track_mc_p->start_vertex_p)) {
      Int_t indexVtx = -1;
      for (i = 0; i < fNumMCVertices; i++) {
        if (vtx_mc_p == vtxMC_save[i]) {
          indexVtx = i;
          break;
	}
      }
      if (indexVtx >= 0) {
        mcVertex = (T49VertexMCRoot *) MCVertexList->At(indexVtx);
        if (fVerbose > 1) printf("Vertex already copied %d\n",indexVtx);
      }
      else {
       if (fNumMCVertices < NVtxMCSave) {   
         if (fVerbose > 1) printf("Create new T49VertexMCRoot\n");
          mcVertex = new T49VertexMCRoot();
          MCVertexList->Add(mcVertex);        
          mcVertex->SetX(vtx_mc_p->x);
          mcVertex->SetY(vtx_mc_p->y);
          mcVertex->SetZ(vtx_mc_p->z);
          vtxMC_save[fNumMCVertices] = vtx_mc_p;
          indexVtx = fNumMCVertices;
          fNumMCVertices++;
          if (fNumMCVertices == NVtxMCSave) {
            printf("More than %d MC vertices, skipping rest of vertices\n",NVtxMCSave);
	  }
        }
        else {
          mcVertex = 0;
        }
      }
      if      (mcVertex->GetFirstDaughter()  < 0) {
        if (fVerbose > 1) printf("Set first daughter\n");
        mcVertex->SetFirstDaughter(fNumMCTracks);
        mcTrack->SetStartVertex(indexVtx);
      }
      else if (mcVertex->GetSecondDaughter() < 0) {
        if (fVerbose > 1) printf("Set second daughter\n");
        mcVertex->SetSecondDaughter(fNumMCTracks);
        mcTrack->SetStartVertex(indexVtx);
      }
      else if (mcVertex->GetThirdDaughter()  < 0) {
        if (fVerbose > 1) printf("Set third daughter\n");
        mcVertex->SetThirdDaughter(fNumMCTracks);
        mcTrack->SetStartVertex(indexVtx);
      }
      else if (mcVertex->GetForthDaughter()  < 0) {
        if (fVerbose > 1) printf("Set forth daughter\n");
        mcVertex->SetForthDaughter(fNumMCTracks);
        mcTrack->SetStartVertex(indexVtx);
      }
      else {
        if (fVerbose) printf("More than four MC-daughters for MC-vertex %d\n"
                            ,indexVtx);
        mcTrack->SetStartVertex(indexVtx);
      }
    }
    else {
      if (fVerbose > 1) printf("No start vertex found\n");
    }

    // Save the stop vertex
    if (fVerbose > 1) printf("Save the stop vertex\n");
    if ((vtx_mc_p = track_mc_p->stop_vertex_p)) {
      Int_t indexVtx = -1;
      for (i = 0; i < fNumMCVertices; i++) {
        if (vtx_mc_p == vtxMC_save[i]) {
          indexVtx = i;
          break;
        }
      }
      if (indexVtx >= 0) {
        printf("More than one MC-parent for MC-vertex %d\n",indexVtx);
      }
      else {
        if (fNumMCVertices < NVtxMCSave) {   
          if (fVerbose > 1) printf("Create new T49VertexMCRoot\n");
          mcVertex = new T49VertexMCRoot();
          MCVertexList->Add(mcVertex);        
          mcVertex->SetX(vtx_mc_p->x);
          mcVertex->SetY(vtx_mc_p->y);
          mcVertex->SetZ(vtx_mc_p->z);
          if (fVerbose > 1) printf("Set parent\n");
          mcVertex->SetParent(fNumMCTracks);
          mcTrack->SetStopVertex(fNumMCVertices);
          vtxMC_save[fNumMCVertices] = vtx_mc_p;
          fNumMCVertices++;
          if (fNumMCVertices == NVtxMCSave) {
            printf("More than %d MC vertices, skipping rest of vertices\n",NVtxMCSave);
	  }
        }
      }
    }
    else {
      if (fVerbose > 1) printf("No stop vertex found\n");
    }

    track_mc_p++;
    fNumMCTracks++;

  }

  if (fVerbose) {
    printf("Finished copying MC data\n");
    printf("MC tracks: %d, MC vertices: %d\n"
          ,fNumMCTracks,fNumMCVertices);
  }

  delete [] vtxMC_save;

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDSMC::FillMCTrack(T49ParticleMCRoot *mcTrack, track_mc_t *track_mc_p) 
{
  //
  // Copy data from a track_mc_t structure into a T49ParticleMCRoot object.
  //

  point_tof_mc_t *point_tof_mc_p;
  point_tpc_mc_t *point_mc;
  Float_t xfirst = 0;
  Float_t yfirst = 0;
  Float_t zfirst = 0;
  Float_t xlast  = 0;
  Float_t ylast  = 0;
  Float_t zlast  = 0;

  int num;
  etrack_mc_t   *etrack_mc_p;
  etrack_mc_p = (etrack_mc_t *) get_objects("etrack_mc",&num);

  for(int i=0;i<num;i++){
    if(etrack_mc_p[i].track_p==track_mc_p){
      int id_det=etrack_mc_p[i].id_det; 
      mcTrack->SetIdDet(id_det);
      mcTrack->SetNMaxPoint(etrack_mc_p[i].n_max_pnt_tpc[0],0);
      mcTrack->SetNMaxPoint(etrack_mc_p[i].n_max_pnt_tpc[1],1);
      mcTrack->SetNMaxPoint(etrack_mc_p[i].n_max_pnt_tpc[2],2);
      mcTrack->SetNMaxPoint(etrack_mc_p[i].n_max_pnt,3);   
    }//if
  }//i

  // The track_mc_t information

  float ptot=track_mc_p->px*track_mc_p->px+track_mc_p->py*track_mc_p->py+track_mc_p->pz*track_mc_p->pz;
  if(ptot>0) {ptot=sqrt(ptot);} else {ptot=0;}
  cout<<" DSMC "<<endl;

  mcTrack->SetPid(track_mc_p->pid);
  mcTrack->SetPx(track_mc_p->px);
  mcTrack->SetPy(track_mc_p->py);
  mcTrack->SetPz(track_mc_p->pz);
  mcTrack->SetNPoint(track_mc_p->n_vt1_hit,0);
  mcTrack->SetNPoint(track_mc_p->n_vt2_hit,1);
  mcTrack->SetNPoint(track_mc_p->n_mtl_hit
                   + track_mc_p->n_mtr_hit,2);
  mcTrack->SetNPoint(track_mc_p->n_vt1_hit
                   + track_mc_p->n_vt2_hit
                   + track_mc_p->n_mtl_hit
                   + track_mc_p->n_mtr_hit,3);

  // First and last MC points
  if (track_mc_p->pnt_vt1_p) {
    point_mc = track_mc_p->pnt_vt1_p;
    xlast  = point_mc->x;
    ylast  = point_mc->y;
    zlast  = point_mc->z;
    mcTrack->SetXLast(0,point_mc->x);
    mcTrack->SetYLast(0,point_mc->y);
    mcTrack->SetZLast(0,point_mc->z);
    while (point_mc->next_trk_pnt_p) {
      point_mc = point_mc->next_trk_pnt_p;
    }
    xfirst = point_mc->x;
    yfirst = point_mc->y;
    zfirst = point_mc->z;
    mcTrack->SetXFirst(0,point_mc->x);
    mcTrack->SetYFirst(0,point_mc->y);
    mcTrack->SetZFirst(0,point_mc->z);
  }
  else {
    mcTrack->SetXLast(0,0);
    mcTrack->SetYLast(0,0);
    mcTrack->SetZLast(0,0);
    mcTrack->SetXFirst(0,0);
    mcTrack->SetYFirst(0,0);
    mcTrack->SetZFirst(0,0);
  }
  if (track_mc_p->pnt_vt2_p) {
    point_mc = track_mc_p->pnt_vt2_p;
    xlast  = point_mc->x;
    ylast  = point_mc->y;
    zlast  = point_mc->z;
    mcTrack->SetXLast(1,point_mc->x);
    mcTrack->SetYLast(1,point_mc->y);
    mcTrack->SetZLast(1,point_mc->z);
    while (point_mc->next_trk_pnt_p) {
      point_mc = point_mc->next_trk_pnt_p;
    }
    if (xfirst == 0) {
      xfirst = point_mc->x;
      yfirst = point_mc->y;
      zfirst = point_mc->z;
    }
    mcTrack->SetXFirst(1,point_mc->x);
    mcTrack->SetYFirst(1,point_mc->y);
    mcTrack->SetZFirst(1,point_mc->z);
  }
  else {
    mcTrack->SetXLast(1,0);
    mcTrack->SetYLast(1,0);
    mcTrack->SetZLast(1,0);
    mcTrack->SetXFirst(1,0);
    mcTrack->SetYFirst(1,0);
    mcTrack->SetZFirst(1,0);
  }
  if ((track_mc_p->pnt_mtl_p) || 
      (track_mc_p->pnt_mtr_p)) {
    point_mc = track_mc_p->pnt_mtl_p 
             ? track_mc_p->pnt_mtl_p 
             : track_mc_p->pnt_mtr_p;
    xlast  = point_mc->x;
    ylast  = point_mc->y;
    zlast  = point_mc->z;
    mcTrack->SetXLast(2,point_mc->x);
    mcTrack->SetYLast(2,point_mc->y);
    mcTrack->SetZLast(2,point_mc->z);
    while (point_mc->next_trk_pnt_p) {
      point_mc=point_mc->next_trk_pnt_p;
    }
    if (xfirst == 0) {
      xfirst = point_mc->x;
      yfirst = point_mc->y;
      zfirst = point_mc->z;
    }
    mcTrack->SetXFirst(2,point_mc->x);
    mcTrack->SetYFirst(2,point_mc->y);
    mcTrack->SetZFirst(2,point_mc->z);
  }
  else {
    mcTrack->SetXLast(2,0);
    mcTrack->SetYLast(2,0);
    mcTrack->SetZLast(2,0);
    mcTrack->SetXFirst(2,0);
    mcTrack->SetYFirst(2,0);
    mcTrack->SetZFirst(2,0);
  }
  mcTrack->SetXFirst(xfirst);
  mcTrack->SetYFirst(yfirst);
  mcTrack->SetZFirst(zfirst);
  mcTrack->SetXLast(xlast);
  mcTrack->SetYLast(ylast);
  mcTrack->SetZLast(zlast);

  // The TOF information
  if ((point_tof_mc_p = track_mc_p->pnt_tof_p)) {
    mcTrack->SetTofId(point_tof_mc_p->id_det);
    mcTrack->SetTofX(point_tof_mc_p->x);
    mcTrack->SetTofY(point_tof_mc_p->y);
    mcTrack->SetTofPathl(point_tof_mc_p->path_length);
  }

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDSMC::FillMCTrackLT(T49ParticleRoot *track, track_mc_t *track_mc_p)
{
  //
  // Copy data from a track_mc_t structure into a T49ParticleRoot object.
  // Use in case of LT data
  //

  point_tof_mc_t *point_tof_mc_p;
  point_tpc_mc_t *point_mc;
  Float_t xfirst = 0;
  Float_t yfirst = 0;
  Float_t zfirst = 0;
  Float_t xlast  = 0;
  Float_t ylast  = 0;
  Float_t zlast  = 0;
 
  // The track_mc_t information
  //  track->SetPid(track_mc_p->pid);
  //  track->SetPx(track_mc_p->px);
  //  track->SetPy(track_mc_p->py);
  //  track->SetPz(track_mc_p->pz);
  if(fVerbose>1) {
    cout<<"pid = "<<track_mc_p->pid<<endl;
    cout 
      << track_mc_p->n_vt1_hit<<" "
      << track_mc_p->n_vt2_hit<<" "
      << track_mc_p->n_mtl_hit<<" "
      << track_mc_p->n_mtr_hit<<endl;
  }
  track->SetNPoint(track_mc_p->n_vt1_hit,0);
  track->SetNPoint(track_mc_p->n_vt2_hit,1);
  track->SetNPoint(track_mc_p->n_mtl_hit
                   + track_mc_p->n_mtr_hit,2);
  track->SetNPoint(track_mc_p->n_vt1_hit
                   + track_mc_p->n_vt2_hit
                   + track_mc_p->n_mtl_hit
                   + track_mc_p->n_mtr_hit,3);

  // First and last MC points
  if (track_mc_p->pnt_vt1_p) {
    point_mc = track_mc_p->pnt_vt1_p;
    xlast  = point_mc->x;
    ylast  = point_mc->y;
    zlast  = point_mc->z;
    track->SetXLast(0,point_mc->x);
    track->SetYLast(0,point_mc->y);
    track->SetZLast(0,point_mc->z);
    while (point_mc->next_trk_pnt_p) {
      point_mc = point_mc->next_trk_pnt_p;
    }
    xfirst = point_mc->x;
    yfirst = point_mc->y;
    zfirst = point_mc->z;
    track->SetXFirst(0,point_mc->x);
    track->SetYFirst(0,point_mc->y);
    track->SetZFirst(0,point_mc->z);
  }
  else {
    track->SetXLast(0,0);
    track->SetYLast(0,0);
    track->SetZLast(0,0);
    track->SetXFirst(0,0);
    track->SetYFirst(0,0);
    track->SetZFirst(0,0);
  }
  if (track_mc_p->pnt_vt2_p) {
    point_mc = track_mc_p->pnt_vt2_p;
    xlast  = point_mc->x;
    ylast  = point_mc->y;
    zlast  = point_mc->z;
    track->SetXLast(1,point_mc->x);
    track->SetYLast(1,point_mc->y);
    track->SetZLast(1,point_mc->z);
    while (point_mc->next_trk_pnt_p) {
      point_mc = point_mc->next_trk_pnt_p;
    }
    if (xfirst == 0) {
      xfirst = point_mc->x;
      yfirst = point_mc->y;
      zfirst = point_mc->z;
    }
    track->SetXFirst(1,point_mc->x);
    track->SetYFirst(1,point_mc->y);
    track->SetZFirst(1,point_mc->z);
  }
  else {
    track->SetXLast(1,0);
    track->SetYLast(1,0);
    track->SetZLast(1,0);
    track->SetXFirst(1,0);
    track->SetYFirst(1,0);
    track->SetZFirst(1,0);
  }
  if ((track_mc_p->pnt_mtl_p) || 
      (track_mc_p->pnt_mtr_p)) {
    point_mc = track_mc_p->pnt_mtl_p 
             ? track_mc_p->pnt_mtl_p 
             : track_mc_p->pnt_mtr_p;
    xlast  = point_mc->x;
    ylast  = point_mc->y;
    zlast  = point_mc->z;
    track->SetXLast(2,point_mc->x);
    track->SetYLast(2,point_mc->y);
    track->SetZLast(2,point_mc->z);
    while (point_mc->next_trk_pnt_p) {
      point_mc=point_mc->next_trk_pnt_p;
    }
    if (xfirst == 0) {
      xfirst = point_mc->x;
      yfirst = point_mc->y;
      zfirst = point_mc->z;
    }
    track->SetXFirst(2,point_mc->x);
    track->SetYFirst(2,point_mc->y);
    track->SetZFirst(2,point_mc->z);
  }
  else {
    track->SetXLast(2,0);
    track->SetYLast(2,0);
    track->SetZLast(2,0);
    track->SetXFirst(2,0);
    track->SetYFirst(2,0);
    track->SetZFirst(2,0);
  }
  track->SetXFirst(xfirst);
  track->SetYFirst(yfirst);
  track->SetZFirst(zfirst);
  track->SetXLast(xlast);
  track->SetYLast(ylast);
  track->SetZLast(zlast);

  // The TOF information
  if ((point_tof_mc_p = track_mc_p->pnt_tof_p)) {
    track->SetTofId(point_tof_mc_p->id_det);
    track->SetTofX(point_tof_mc_p->x);
    track->SetTofY(point_tof_mc_p->y);
    track->SetTofPathl(point_tof_mc_p->path_length);
  }

  //PID
  track->SetWeight(track_mc_p->pid);
  
  return kTRUE;

}
