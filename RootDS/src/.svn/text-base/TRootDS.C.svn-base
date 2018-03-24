/////////////////////////////////////////////////////////////
//
//  The TRootDS class provides an interface between the 
//  NA49 Standard C data structures and the NA49 
//  ROOT data objects. It provides several member
//  functions for copying data into a ROOT mini-DST,
//  dropping some non-essential information. The mini-
//  DST is designed as the basis for the NA49 DST analysis,
//  with the emphasis on fast I/O and flexible interactive
//  analysis using standard tools within the ROOT 
//  environment.
//  Changing the mini-DST format requires changes or 
//  additions to both the ROOT NA49 Data classes (like
//  T49ParticleRoot) and the filling routines inside TRootDS.
//  As the total data throughput will be on the order 
//  of many hundred GBytes, the DST format will not be 
//  changed frequently. The present approach therefore
//  seems appropriate.
//
//  TRootDS can also be used to interactively access 
//  data stored in DSPACK files on the command line or 
//  in ROOT macros.
//  The OpenDS() and IdsgetDS() member functions can be 
//  used to obtain pointers to the data structures from
//  DSPACK. The standard NA49 data structures (including
//  the mc structures) are automatically available if the 
//  'rootds' executable is used. See the root49 web page
//  for details.
//
//  Author:    G. Roland
//  Changed:   C. Blume 
//             Added secondary vertices and tracks.
//
///////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

// T49 includes
#include "TRootDS.h"

#define _MATH_H_MATHDEF
#include "T49EventRoot.h"
#include "T49VertexRoot.h"
#include "T49ParticleRoot.h"
#undef _MATH_H_MATHDEF

// ROOT includes
#include "TROOT.h"
#include "TTree.h"
#include "TList.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TMath.h"

// DSPACK includes
#include "dsc_prot.h"
#include "dspack_rootds.h"
#include "na49_event_mc_str_root.h"
#include "na49_event_str_root.h"
#include "na49_mini_root.h"
#include "xi_find_root.h"
#include "v0find_root.h"

#define MTL  0
#define MTR  1

extern "C" void   *idsget_ds(const char*, int*, int*);

Int_t gVerbose;

ClassImp(TRootDS);

//______________________________________________________________________________
TRootDS::TRootDS() 
{

  fInfile            = NULL;
  fInfileId          = -1;
  fDSpackInitialized = kFALSE;    
  fDSpackOk          = kFALSE;

  fRootFile          = NULL;
  fEvent             = NULL;
    
  fNumTracks         = 0;
  fNumSecTracks      = 0;
  fNumSecVertices    = 0;

  fFillPrimary       = kTRUE;
  fFillGTPCOnly      = kTRUE;
  fFillSecondary     = kTRUE;
  fFillXis           = kFALSE;
  fFullSecondary     = 0;
  fNormalizedDedx    = kTRUE;
  fUseMTPCPoints     = kFALSE;
  fUseEmptyADC       = kFALSE;
  fCheckV0           = kTRUE;
  fCheckLT           = kFALSE;
  fSkipFound         = 0;
  
}

//______________________________________________________________________________
Bool_t TRootDS::InitDS(Char_t *name, Int_t mode, Int_t db) 
{
  //
  //  Start DSPACK locally or connect to server       
  //

  if (!fDSpackInitialized) {
    printf("TRootDS::InitDS(): Initialize dspack\n");
    if (mode == 0)
      fDSpackLocal = kTRUE;
    else
      fDSpackLocal = kFALSE;
    return init_dspack(mode,name,NULL,db);
  }
  else {
    printf("TRootDS::InitDS(): dspack already initialized\n");
    return 0;
  }

}

//______________________________________________________________________________
Bool_t TRootDS::OpenDS(char *filename,Int_t db) 
{
  //
  //  Open a DSPACK file with the specified filename
  //

  if (fInfile != NULL) {
    cerr << "input file already open" << endl;
    return kFALSE;
  }
    
  if (!fDSpackInitialized) {
    printf("TRootDS::OpenDS(): Initialize dspack in local mode\n");
    init_dspack(0,"TRootDS", NULL,db);
    fDSpackInitialized = kTRUE;
    fDSpackLocal       = kTRUE;
    fDSpackOk          = kTRUE;
  }

  fInfileId = open_infile(filename);
  if (fInfileId < 0) {
    cerr <<  "can't open input file" << endl;
    return kFALSE;
  }
  else {
    fDSpackOk = kTRUE;
    fInfile = filename;
    cout << "Input file " << fInfile << " opened." << endl;
    return kTRUE;
  }

}

//______________________________________________________________________________
TRootDS::~TRootDS() 
{
  //
  //  TRootDS destructor 
  //

  CloseDS();
    
}

//______________________________________________________________________________
Bool_t TRootDS::CloseDS() 
{
  //
  //  Close the DSPACK input file
  //
    
  if (fInfile != NULL) {
    close_infile(fInfileId);
    fInfile   = NULL;
    fInfileId = -1;
    return kTRUE;
  } 
  else {
    return kFALSE;
  }
    
}

//______________________________________________________________________________
Bool_t TRootDS::ReadEventDS() 
{
  //
  //  Read the next event from the DSPACK input file
  //  Returns FALSE if end of input file is reached
  //

  clear_section(1);
  if (!read_file(fInfileId)) {
    cerr << "can't read from file" << endl;
    fDSpackOk = kFALSE;
  }
  else {
    fDSpackOk = kTRUE;
  }

  return fDSpackOk;

}

//______________________________________________________________________________
Bool_t TRootDS::ReadEventT49() 
{
  //
  //  Read the next event from the DSPACK input file
  //  and copies it directly into the T49 data structure.
  //  This allows to use the T49ANA classes on DSPACK data.
  //  Returns FALSE if end of input file is reached.
  //

  if (fEvent) {
    if (fVerbose) printf("Clear ClonesArrays\n");
    fEvent->GetPrimaryParticles()->Delete();
    fEvent->GetSecondaryParticles()->Delete(); 
    fEvent->GetSecondaryVertices()->Delete();
  }

  if (ReadEventDS()) {

    if (fVerbose) printf("Read one event\n");
    if (!(fEvent)) fEvent = new T49EventRoot(kTRUE);    

    if (fVerbose) printf("Fill one event\n");
    return FillEvent();

  }
  else {

    return kFALSE;

  }

}

//______________________________________________________________________________
Bool_t TRootDS::DS2Tree(Char_t *InFMDirName, Char_t *OutDirName
                      , Char_t *OutFileName, Int_t nEvent) 
{
  //
  //  Copy the events from the FATMAN directory 'InFMDirName' (DSPACK)  
  //  into several ROOT mini-DST files 'OutFileName' in the directory 
  //  'OutDirName'. After copying 'nEvent' events one file is closed and
  //  the next one is opened. To the filename 'OutFileName' automatically 
  //  the extension .N.root ist appended, where 'N' is the file number.
  //

#if ROOT_VERSION_CODE >= ROOT_VERSION(5,26,0)
  // OptimizeBaskets is called "after having filled some entries in a Tree"
  const Int_t optimiseAfterNEvents = (nEvent < 20) ? TMath::CeilNint(nEvent / 2.) : 10;
#endif

  Char_t RootFname[256];
  Int_t  iEvent = 0;   
  Int_t  iFile  = 0;

  // Open the DSPACK files
  OpenDS(InFMDirName);

  // Loop over the events in the input file
  while (GetDSpackOk()) {

    if (fRootFile == NULL) {

      fEvent = new T49EventRoot(kTRUE);

      sprintf(RootFname,"%s/%s.%d.root",OutDirName,OutFileName,iFile);
      printf("Open ROOT output file %s\n",RootFname);
      fRootFile = TFile::Open(RootFname,"RECREATE","A Tree DST",1,0);

      printf("SetCompressionLevel(2)\n");
      fRootFile->SetCompressionLevel(2);
      fTree = CreateTree();
      fTree->SetAutoSave(1990000000);
      if (fVerbose > 1) fTree->Print();

      iFile++;

    }

    if (ReadEventDS() == kTRUE) {

      if (fVerbose) printf("Fill one event\n");
      if (FillEvent()) { 
        if (fVerbose) printf("Fill tree\n");
        fTree->Fill();  
        if (fVerbose) printf("Clear ClonesArrays\n");
        fEvent->GetPrimaryParticles()->Delete(); 
        fEvent->GetSecondaryParticles()->Delete(); 
        fEvent->GetSecondaryVertices()->Delete();
      }

      iEvent++;

    }

#if ROOT_VERSION_CODE >= ROOT_VERSION(5,26,0)
    // Optimise buffers. Use the current tree size, the way autoflush does
    if (iEvent == optimiseAfterNEvents)
      fTree->OptimizeBaskets(fTree->GetTotBytes(), 1., "d");
#endif

    // Close the output file
    if (iEvent == nEvent) {

      iEvent = 0;

      fRootFile->Write();
      fRootFile->Close();
      delete fRootFile;
      fRootFile = NULL;

    }

  } 

  CloseDS();

  return kTRUE;  
  
}

//______________________________________________________________________________
Bool_t TRootDS::DS2Tree(Char_t *InFname, Char_t *OutFname, Int_t MaxEvents) 
{
  //
  //  Copy the events from the input file 'InFname' (DSPACK)  
  //  to the ROOT mini-DST 'OutFname'. Up to MaxEvents are
  //  copied. The ROOT T49EventRoot objects are written to the 
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

    fEvent = new T49EventRoot(kTRUE);

    sprintf(RootFname,"%s",OutFname);
    printf("Open File %s\n",RootFname);
    fRootFile = TFile::Open(RootFname,"RECREATE","A Tree DST");

    printf("SetCompressionLevel(2)\n");
    fRootFile->SetCompressionLevel(2);
    fRootFile->ls();
    fTree = CreateTree();
    fTree->SetAutoSave(1990000000);
    if (fVerbose > 1) fTree->Print();

  }

  // Copy DS DST to Root file 
  cerr << "Open Input File: " << InFname << endl;
  OpenDS(InFname);

  cerr << "Loop over events " << InFname << endl;
  while (GetDSpackOk() && NumEvents < MaxEvents) {

    if (ReadEventDS() == kTRUE) {
      if (fVerbose) printf("Fill one event\n");
      if (FillEvent()) { 
        if (fVerbose) printf("Fill tree\n");
        fTree->Fill();  
        if (fVerbose) printf("Clear ClonesArrays\n");
        fEvent->GetPrimaryParticles()->Delete();
        fEvent->GetSecondaryParticles()->Delete();
        fEvent->GetSecondaryVertices()->Delete();
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
Bool_t  TRootDS::Close() 
{
  //
  //  Close the ROOT mini-DST output file
  //

  if (fRootFile) {
    fRootFile->Write();
    fRootFile->Close();
  }
  else {
    return kFALSE;
  }

  return kTRUE;

}

//______________________________________________________________________________
TTree *TRootDS::CreateTree() 
{
  //
  //  Create the TTree object for the DS2Tree member
  //  function.
  //

  TTree   *tree;

  tree = new TTree("T49","An NA49 ROOT tree");

  if (fVerbose) {
    fEvent->Dump();
    printf("Create event branch\n");
  }
  tree->Branch("Event", "T49EventRoot", &fEvent);
  if (fVerbose) {
    printf("tree ok\n");
  }

  return tree;

}

//______________________________________________________________________________
Bool_t TRootDS::FillEvent() 
{
  //
  //  Copy event, track and vertex data into ROOT NA49
  //  objects for the current event.
  //
  //  Updated for V0-tracks and vertices (CBL 10/02/99)
  //  

  std::cout<<"FillEvent "<<std::endl;

  Int_t          i;
  Int_t          Dummy;
  Int_t          TotalTracks = 0;
  Int_t          num;
  Int_t          num2;
  Int_t          num3;

  Char_t         name[256];

  event_t       *event_p;
  rtrack_t      *rtrack_p;
  track_t       *track_p;
  vertex_t      *vtx_p;
  vertex_t      *vtx_list;
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
  rtrack_t	*rtrack =0;
  monitor_tpc_t *moni_p;
  bpd_vertex_t  *bpd_vtx_p;
  wfa_t         *wfa_p;
  wfa_time_t    *wfa_time_p;
 

  static Int_t   nSkip = 0;

  T49ParticleRoot      *track;

  TClonesArray         *PrimaryTrackArray;
  TClonesArray         *SecondaryTrackArray;
  TObjArray            *SecVtxList;

  event_p = (event_t *) get_objects("event",&Dummy);
  if (event_p == NULL) {
    cerr << "No event_t in this event" << endl;
    return kFALSE;
  }

  all_p    = (tpc_t *) get_objects("all"   ,&num   );
  if (all_p == NULL) {
    cerr << "No all structure in this event" << endl;
    return kFALSE;
  }

  // Used to create links between different lists
  Int_t      NRTrackSave = TMath::Max( 250,all_p->n_rtrack + 1);
  Int_t      NVtxSave    = TMath::Max(5000,all_p->n_vertex * 3);
  rtrack_t **rtrack_save = new rtrack_t*[NRTrackSave];
  vertex_t **vtx_save    = new vertex_t*[NVtxSave];
  for (i = 0; i < NRTrackSave; i++) rtrack_save[i] = NULL;
  for (i = 0; i < NVtxSave;    i++) vtx_save[i]    = NULL;

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
    printf("Fill event_t data (%d)\n",fVerbose);
  }
  fEvent->SetNRun(event_p->n_run);
  fEvent->SetNEvent(event_p->n_event);
  fEvent->SetDate(event_p->date);
  fEvent->SetTime(event_p->time);
  fEvent->SetTriggerMask(event_p->mask_trig);
  if (event_p->veto_p) {
    fEvent->SetEveto(event_p->veto_p->eveto);
  }

  if (fVerbose) {
    printf("Copy monitor_tpc_t data\n");
  }
  moni_p = (monitor_tpc_t *) get_objects("monitor_vt1",&Dummy);
  if (moni_p) {
    FillMonitor(fEvent->GetMonitorTPC( 2),moni_p);
  }
  moni_p = (monitor_tpc_t *) get_objects("monitor_vt2",&Dummy);
  if (moni_p) {
    FillMonitor(fEvent->GetMonitorTPC( 4),moni_p);
  }
  moni_p = (monitor_tpc_t *) get_objects("monitor_mtr",&Dummy);
  if (moni_p) {
    FillMonitor(fEvent->GetMonitorTPC( 8),moni_p);
  }
  moni_p = (monitor_tpc_t *) get_objects("monitor_mtl",&Dummy);
  if (moni_p) {
    FillMonitor(fEvent->GetMonitorTPC(16),moni_p);
  }

  if (fVerbose) {
    printf("Copy main vertex position\n");
  }
  main_vtx = (vertex_t *) all_p->main_vertex_p;
  if (fVerbose) {
    printf("Fill Main vertex object\n");
  }
  if(main_vtx) {
    fEvent->SetMainVertexExists(kTRUE);
    FillVertex(fEvent->GetMainVertex(),main_vtx);
  }
  else {
    fEvent->SetMainVertexExists(kFALSE);
  }

  bpd_vtx_p = (bpd_vertex_t *) get_objects("bpd_vertex",&Dummy);
  if (fVerbose) {
    printf("Fill BPD vertex object\n");
  }
  if(bpd_vtx_p) {
    fEvent->SetBPDVertexExists(kTRUE);
    FillBPDVertex(fEvent->GetBPDVertex(),bpd_vtx_p);
  }
  else {
    fEvent->SetBPDVertexExists(kFALSE);
  }

  prim_vtx = (vertex_t *) get_objects("vertex_fit",&Dummy);
  if (fVerbose) {
    printf("Fill Fitted vertex object\n");
  }
  if(prim_vtx) {
    fEvent->SetFittedVertexExists(kTRUE);
    FillVertex(fEvent->GetFittedVertex(),prim_vtx);
  }
  else {
    fEvent->SetFittedVertexExists(kFALSE);
  }


  if ((fEvent->GetNEvent() % 100 == 0) || (fVerbose)) {
    sprintf(name,"event_%d_%d",fEvent->GetNRun(),fEvent->GetNEvent());
    sprintf(name,"Run %d, Event %d",fEvent->GetNRun(),fEvent->GetNEvent());
  }

  if (fVerbose) {
    printf("Copy the WFA data\n");
  }
  wfa_p = (wfa_t *) get_objects("wfa",&Dummy);
  if (wfa_p) {
    fEvent->SetWfaNbeam(wfa_p->n_beam);
    fEvent->SetWfaNinter(wfa_p->n_inter);
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
	fEvent->SetWfaBeamTime(k, (wfa_time_p[k]).wfa_time);
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
	fEvent->SetWfaInterTime(k, (wfa_time_p[k]).wfa_time);
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
  if (beam_p && fEvent->GetBeam()) {
    FillBeam(fEvent->GetBeam(),beam_p);
  }
  else {
    printf("event_t->beam_p = %p\n", event_p->beam_p);
  }

  if (fVerbose) {
    printf("Copy ring data\n");
  }
  ring_p = event_p->ring_p;
  if (!ring_p) ring_p = (ring_t *) get_objects("ring",&Dummy);
  if (ring_p && fEvent->GetRing()) {
    FillRing(fEvent->GetRing(),ring_p);  
  }

  if (fVerbose) {
    printf("Copy veto data\n");
  }
  veto_p = event_p->veto_p;
  if (!veto_p) veto_p = (veto_t *) get_objects("veto",&Dummy);
  veto_ped_p = (pedestal_t *) get_objects("veto_ped",&Dummy);
  if (veto_p && veto_ped_p && fEvent->GetVeto()) {
    FillVeto(fEvent->GetVeto(),veto_p,veto_ped_p);
  }

  if (fFillPrimary) {

    if (fVerbose) {
      printf("Copy primary track data\n");
    }
    fNumTracks  = 0;
    TotalTracks = 0;

    if (!(PrimaryTrackArray = fEvent->GetPrimaryParticles())) {
      printf("No primary TrackArray found in event\n");
    }
    
    // Copy GTPC-only tracks
    if (fFillGTPCOnly){
      rtrack    = (rtrack_t *) 	  get_objects("rtrack",&num3);
      if (fVerbose) {
        printf("Found rtrack_t at %p (%d tracks)\n", rtrack, num3);
      }
      for (i = 0; i < num3; i++) {
         if((rtrack[i].id_det == 32) && (rtrack[i].n_point != 0)){
	  
	   if (fVerbose > 1) {
                printf("Alloc T49ParticleRoot for gtpc-only track\n");
           }
           track = new (PrimaryTrackArray->operator[](fNumTracks)) T49ParticleRoot();
           track->SetLabel(fNumTracks);
           if (fVerbose > 1) {
              printf("Call FillGTPCOnlyTrack\n");
           }
           FillGTPCOnlyTrack(track,&rtrack[i]);
           if (fVerbose > 1) {
             printf("FillGTPCOnlyTrack finished\n");
           }
	  
	   fNumTracks++;
           TotalTracks++;
        }
      }
    }

    track_ref = (g_track_ref_t *) get_objects("g_track_ref",&num);
    ptr_list  = (g_ptr_list_t  *) get_objects("g_ptr_list",&num2);
 
 
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
        if (fVerbose > 2) {
          printf("%d: %p\n", i, track_ref[i].g_ptr_list_p);
        }

        rtrack_p = ptr_list->rtrack_glb_p;
        if (fVerbose > 2) {
          printf("rtrack: %p\n", rtrack_p);
        }
        if (!rtrack_p) {
          fprintf(stderr,"rtrack_p = NULL for %d\n",i);
          break;
        }

        track_p = rtrack_p->first_track_p;
        if (fVerbose > 2) {
          printf("track: %p\n", track_p);
        }
        if (!track_p) {
          fprintf(stderr,"track_p = NULL for %d\n",i);
          break;
        }

        vtx_p = track_p->start_vertex_p;

        if (fVerbose > 2) {
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
              rtrack_save[fNumTracks]    = rtrack_p;
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
    if (!(SecondaryTrackArray = fEvent->GetSecondaryParticles())) {
      printf("No secondary track array found in event\n");
    }
    // The TObjArray for the vertices
    if (!(SecVtxList = fEvent->GetSecondaryVertices())) {
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
            printf("Fill secondary vertex object with V0\n");
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
            printf("Fill secondary vertex object with V0\n");
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
          // for (i = 0; i < fNumTracks; i++) {
//             if (rtrack_p == rtrack_save[i]) {
//               SecVertex->SetPrimaryIndex(0,i);
//               if (fVerbose > 1) {
//                 printf("First particle index (main-vtx) = %d\n",i);
// 	      }
//               mainFound = kTRUE;
//               break;
// 	    }
//           }

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

// 	  cout << event_p->n_event<<" "<<fNumSecVertices
// 	       <<" "<<track->GetNPoint(0)
// 	       <<" "<<track->GetNPoint(1)
// 	       <<" "<<track->GetNPoint(2)
// 	       <<" "<<track->GetNPoint()<<endl;

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
           // for (i = 0; i < fNumTracks; i++) {
//              if (rtrack_p == rtrack_save[i]) {
//                SecVertex->SetPrimaryIndex(1,i);
//                if (fVerbose > 1) {
//                  printf("Second particle index (main-vtx) = %d\n",i);
//  	      }
//                mainFound = kTRUE;
//                break;
//  	    }
//            }

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
              fNumSecTracks++;
              track_p  = track_p->next_daughter_p;
              rtrack_p = track_p->rtrack_p;
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
              SecVtxList->Add(SecVertex);
              if (fNumSecVertices < NVtxSave) {
                vtx_save[fNumSecVertices] = vtx_stop_p;
              }
              fNumSecTracks++;
              fNumSecVertices++;            
	    }

	  }
          else {
            fprintf(stderr,"No stop vertex found\n");
	  } 

	}
        //vtx_p++;
        //vtx_p = vtx_p->next_p;

      }

    }
    else {
      printf("No vertices found %d\n", event_p->n_event);
    }
      
    if (fVerbose) {
      printf("Finished copying V0 data\n");
    }

  }

  if (fVerbose) {
    printf("Multiplicity: %d out of %d (%g)\n"
          ,fNumTracks,TotalTracks,fEvent->GetEveto());
    if (fFillSecondary) {
      printf("Secondary vertices (tracks): %d/%d (%d)\n"
            ,fNumSecVertices,numVtx,fNumSecTracks);
    }
  }

  delete [] rtrack_save;
  delete [] vtx_save;

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::CheckV0(vertex_t *vtx_p) 
{
  //
  //  Defines a good V0
  //

  // Additional cuts on the GSI V0s
  const Float_t  xtargMin = 1.0;
  const Float_t  ytargMin = 0.5;
 
  const Int_t    kIdV0    = 11;

  rtrack_t      *rtrack_p;
  track_t       *track_p;
  avertex_v0_t  *avtx_p;

  Bool_t         firstpositive;

  Float_t        chargefirst;
  Float_t        qpxzfirst;

  if (vtx_p->id_vtx != kIdV0) return kFALSE; 

  if (!(avtx_p = (avertex_v0_t *) vtx_p->avertex_p)) return kFALSE;

  // Get the first daughter track
  if (!(track_p  = vtx_p->daughter_p)) {
    printf("CheckV0: No first daughter track\n");
    return kFALSE;
  }
  if (!(rtrack_p = track_p->rtrack_p)) {
    printf("CheckV0: No first daughter rtrack\n");
    return kFALSE;
  }
  if (rtrack_p->qpxz > 0) 
    firstpositive = kTRUE;
  else 
    firstpositive = kFALSE;
  chargefirst = rtrack_p->charge;
  qpxzfirst   = rtrack_p->qpxz;

  // Get the second daughter track
  if (!(track_p  = track_p->next_daughter_p)) {
    printf("CheckV0: No second daughter track\n");
    return kFALSE;
  }
  if (!(rtrack_p = track_p->rtrack_p)) {
    printf("CheckV0: No second daughter rtrack\n");
    return kFALSE;
  }
  if ((rtrack_p->qpxz > 0) && (firstpositive)) {  
    printf("CheckV0: Two positive daughter tracks (charge1/2: %g/%g) (qpxz1/2: %g/%g)\n"
                   ,chargefirst,rtrack_p->charge,qpxzfirst,rtrack_p->qpxz);
    return kFALSE;
  }
  if ((rtrack_p->qpxz < 0) && (!firstpositive)) {
    printf("CheckV0: Two negative daughter tracks (charge1/2: %g/%g) (qpxz1/2: %g/%g)\n"
                   ,chargefirst,rtrack_p->charge,qpxzfirst,rtrack_p->qpxz);
    return kFALSE;
  }

  // Apply additional cuts on the GSI V0s
  if ((fCheckV0) && (vtx_p->iflag & 0x4000)) {
    if (TMath::Abs(avtx_p->xtarg) > xtargMin) {
      return kFALSE;
    }
    if (TMath::Abs(avtx_p->ytarg) > ytargMin) {
      return kFALSE;
    }
  }

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::CheckLTVtx(vertex_t *vtx_p) 
{
  //
  //  Defines a good LT extrapolation
  //

  rtrack_t      *rtrack_p;
  track_t       *track_p;
  //  avertex_v0_t  *avtx_p;

  const Int_t kLT_id_vtx = 50;

  if (vtx_p == NULL) return kFALSE;
  if (vtx_p->id_vtx != kLT_id_vtx) return kFALSE; 

//   //-------------
//    bool LTvtxFlag = kFALSE;
//    if (vtx_p->iflag == -10 ) LTvtxFlag = kTRUE; // propagation not succefully
//    if (vtx_p->iflag == -11 ) LTvtxFlag = kTRUE; // minimum not found	       
//    if (vtx_p->iflag == -12 ) LTvtxFlag = kTRUE; // minimum found	       
//    if (vtx_p->iflag == -13 ) LTvtxFlag = kTRUE; // in target

//    if (LTvtxFlag == kFALSE) return kFALSE;
//   //-------------

  // Get the first daughter track
  if (!(track_p  = vtx_p->daughter_p)) {
    if(fVerbose >2) printf("CheckLTVtx: No daughter track\n");
    return kFALSE;
  }
  if (!(rtrack_p = track_p->rtrack_p)) {
    if(fVerbose >2) printf("CheckLTVtx: No daughter rtrack\n");
    return kFALSE;
  }
  if (rtrack_p->charge == 0) {
    if(fVerbose >2) printf("CheckLTVtx: rtrack with no charge definite\n");
    return kFALSE;
  }
  if (rtrack_p->n_point == 0) {
    if(fVerbose >2) printf("CheckLTVtx: rtrack with no point attached\n");
    return kFALSE;
  }
  if (rtrack_p->px == 0 && rtrack_p->py == 0 && rtrack_p->pz == 0) {
    if(fVerbose >2) printf("CheckLTVtx: rtrack with no momentum definite\n");
    return kFALSE;
  }
  if (rtrack_p->n_point <= 10) {
    if(fVerbose >2) printf("CheckLTVtx: rtrack with less than 10 point attached\n");
    return kFALSE;
  }

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::CheckXi(vertex_t *vtx_p) 
{
  //
  //  Defines a good Xi candidate
  //

  const Int_t kIdXi = 41;

  if (vtx_p->id_vtx != kIdXi) return kFALSE; 

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::FillVertex(T49Vertex *vertex_out, vertex_t *vertex_in) 
{
  //
  //  Copy data from vertex_t structure for main vertex
  //  into T49VertexRoot object.
  //

  vertex_out->SetIflag(vertex_in->iflag);
  vertex_out->SetIdDet(vertex_in->id_det);
  vertex_out->SetIdVtx(vertex_in->id_vtx);
  vertex_out->SetX(vertex_in->x);
  vertex_out->SetY(vertex_in->y);
  vertex_out->SetZ(vertex_in->z);
  vertex_out->SetNTrkFit(vertex_in->n_trk_fit);
  vertex_out->SetNTrkOut(vertex_in->n_trk_out);
  vertex_out->SetSigX(vertex_in->sig_x);
  vertex_out->SetSigY(vertex_in->sig_y);
  vertex_out->SetSigZ(vertex_in->sig_z);
  vertex_out->SetPchi2(vertex_in->pchi2);

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::FillBPDVertex(T49Vertex *vertex_out, bpd_vertex_t *vertex_in)
{
  //
  //  Copy data from vertex_t structure for bpd vertex
  //  into T49VertexRoot object.
  //

  vertex_out->SetIflag(vertex_in->iflag);
  vertex_out->SetX(vertex_in->x);
  vertex_out->SetY(vertex_in->y);
  vertex_out->SetZ(vertex_in->z);
  vertex_out->SetSigX(vertex_in->sx);
  vertex_out->SetSigY(vertex_in->sy);
  vertex_out->SetSigZ(vertex_in->sz);

  return kTRUE;

}
//______________________________________________________________________________
Bool_t TRootDS::FillSecVertex(T49VertexRoot *vertex_out, vertex_t *vertex_in)
{
  //
  //  Copy data from vertex_t structure for V0s into 
  //  T49VertexRoot object.
  //

  vertex_out->SetIflag(vertex_in->iflag);
  vertex_out->SetIdDet(vertex_in->id_det);
  vertex_out->SetIdVtx(vertex_in->id_vtx);
  vertex_out->SetX(vertex_in->x);
  vertex_out->SetY(vertex_in->y);
  vertex_out->SetZ(vertex_in->z);
  // No n_trk_fit and n_trk_out for V0 vertices
  vertex_out->SetNTrkFit(0);
  vertex_out->SetNTrkOut(0);
  vertex_out->SetSigX(vertex_in->sig_x);
  vertex_out->SetSigY(vertex_in->sig_y);
  vertex_out->SetSigZ(vertex_in->sig_z);
  vertex_out->SetPchi2(vertex_in->pchi2);

  avertex_v0_t *avertex_in = (avertex_v0_t *) vertex_in->avertex_p;
    
  if (avertex_in) {
    vertex_out->SetXTarg(avertex_in->xtarg);
    vertex_out->SetYTarg(avertex_in->ytarg);
    vertex_out->SetX1minX2(avertex_in->x1minx2);
  }
  else {
    if (fVerbose > 0) {
      printf("FillSecVertex: No avertex_v0_t structure found\n");
    }
    vertex_out->SetXTarg(0.);
    vertex_out->SetYTarg(0.);
    vertex_out->SetX1minX2(0.);
  } 

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::FillXiVertex(T49VertexRoot *vertex_out, vertex_t *vertex_in) 
{
  //
  //  Copy data from vertex_t structure for Xis into 
  //  T49VertexRoot object.
  //

  vertex_out->SetIflag(vertex_in->iflag);
  //vertex_out->SetIdDet(vertex_in->id_det);
  vertex_out->SetIdDet(0);
  vertex_out->SetIdVtx(vertex_in->id_vtx);
  vertex_out->SetX(vertex_in->x);
  vertex_out->SetY(vertex_in->y);
  vertex_out->SetZ(vertex_in->z);
  vertex_out->SetSigX(vertex_in->sig_x);
  vertex_out->SetSigY(vertex_in->sig_y);
  vertex_out->SetSigZ(vertex_in->sig_z);
  // No n_trk_fit and n_trk_out for Xi vertices
  vertex_out->SetNTrkFit(0);
  vertex_out->SetNTrkOut(0);
  vertex_out->SetPchi2(vertex_in->pchi2);

  avertex_xi_t *avertex_in = (avertex_xi_t *) vertex_in->avertex_p;
    
  if (avertex_in) {
    vertex_out->SetXTarg(avertex_in->xtarg);
    vertex_out->SetYTarg(avertex_in->ytarg);
    vertex_out->SetX1minX2(avertex_in->x1minx2);
    vertex_out->SetPathlengthXi(avertex_in->plen);
    vertex_out->SetPxXi(avertex_in->px);
    vertex_out->SetPyXi(avertex_in->py);
    vertex_out->SetPzXi(avertex_in->pz);
  }
  else {
    if (fVerbose > 0) {
      printf("FillXiVertex: No avertex_xi_t structure found\n");
    }
    vertex_out->SetXTarg(0.);
    vertex_out->SetYTarg(0.);
    vertex_out->SetX1minX2(0.);
    vertex_out->SetPathlengthXi(0.);
    vertex_out->SetPxXi(0.);
    vertex_out->SetPyXi(0.);
    vertex_out->SetPzXi(0.);
  } 

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::FillTrack(T49ParticleRoot *track_out, g_track_ref_t *track_ref
                        , Bool_t tof) 
{
  //
  //  Copy data from track_t structure into T49ParticleRoot 
  //  object. <tof> = kTRUE also copied the TOF information.
  //

  Int_t   i;
  Int_t   sum_n_point      = 0;
  Int_t   sum_n_fit_point  = 0;
  Int_t   sum_n_dedx_point = 0;
  Int_t   sum_n_max_point  = 0;
  Int_t   points_filled    = kFALSE;

  for (i = 0; i < 4; i++) {
    track_out->SetNPoint(0,i);
    track_out->SetNFitPoint(0,i);
    track_out->SetNDedxPoint(0,i);
    track_out->SetNMaxPoint(0,i);
    track_out->SetTmeanCharge(0,i);
  }

  rtrack_t       *rtrack;  
  g_ptr_list_t   *ptr_list    = track_ref->g_ptr_list_p;
  rtrack_t       *rtrack_in   = ptr_list->rtrack_glb_p;
  track_t        *track_in    = rtrack_in->first_track_p;
  track_tof_t    *toftrack_in = track_in->track_tof_p;
  rtrack_quali_t *quali       = track_ref->quali_p;

  Int_t NGPC=0, NMTPC=0;
  Int_t NGPC_MAX=0, NMTPC_MAX=0;


  // Old procedure
//    Int_t flag = 1;
//    Int_t num  = 0;
//    if (toftrack_in) {

//      while (toftrack_in) {
//        if (toftrack_in->iflag > 0) num++;
//        toftrack_in = toftrack_in->next_tof_p;
//      }

//      toftrack_in = track_in->track_tof_p;
//      while (flag && toftrack_in->iflag <= 0) {
//        if (toftrack_in->next_tof_p)
//          toftrack_in = toftrack_in->next_tof_p;
//        else
//          flag = 0;
//      }
//      if (toftrack_in->iflag <= 0)
//        toftrack_in = track_in->track_tof_p;
    
//      if (num > 1) {
//        toftrack_in->iflag |= 0x8;
//      }

//    }


// Remove this stupid piece of code... BP 08/05/08

//   // New procedure (4/02/02)
//   if (toftrack_in) {
//     // Look for the first tof hit with x >= 1
//     while ((toftrack_in->x < 1) &&
//            (toftrack_in->next_tof_p)) {
//       toftrack_in = toftrack_in->next_tof_p;
//     }
//     // Take only tof hits with x >= 1
//     if (toftrack_in->x < 1) {
//       toftrack_in = NULL;
//     }
//   }

  if (quali) {

    sum_n_point         = 0;
    sum_n_fit_point     = 0;
    sum_n_max_point     = 0;

    NGPC=0; 
    NMTPC=0;
    NGPC_MAX=0; 
    NMTPC_MAX=0;

    for (i = 0; i < 3; i++) {
//      cout << "n_point[" << i << "] = " << quali->n_point[i] << endl;
      if(i == 2 && quali->n_point[i] > 1000)
       {
         NGPC = quali->n_point[i]/1000; 
         NMTPC = quali->n_point[i]%1000; 
//         cout << "NGPC = " << NGPC << "  NMTPC = " << NMTPC << endl;
         sum_n_point += NGPC;
         sum_n_point += NMTPC;
         track_out->SetNPoint(NMTPC,i);
       }
      else 
       {
         sum_n_point += quali->n_point[i];
         track_out->SetNPoint(quali->n_point[i],i);
       }

//      cout << "n_max_point[" << i << "] = " << quali->n_max_point[i] << endl;
      if(i == 2 && quali->n_max_point[i] > 1000)
       {
         NGPC_MAX = quali->n_max_point[i]/1000;
         NMTPC_MAX = quali->n_max_point[i]%1000;
//         cout << "NGPC_MAX = " << NGPC_MAX << "  NMTPC_MAX = " << NMTPC_MAX << endl;
         sum_n_max_point += NGPC_MAX;
         sum_n_max_point += NMTPC_MAX;
         track_out->SetNMaxPoint(NMTPC_MAX,i);
       }
      else
       {
         sum_n_max_point += quali->n_max_point[i];
         track_out->SetNMaxPoint(quali->n_max_point[i],i);
       }
    }

    track_out->SetNPoint(sum_n_point);
    track_out->SetNMaxPoint(sum_n_max_point);

  }
  

  if ((rtrack = ptr_list->rtrack_vt1_p)) {

    sum_n_fit_point += rtrack->n_fit_point;
    track_out->SetNFitPoint(rtrack->n_fit_point,0);
    track_out->SetNDedxPoint(rtrack->n_dedx_point,0);
    
    if (!isnan(rtrack->tmean_charge)) {
      if (fNormalizedDedx)
        track_out->SetTmeanCharge((1000.0*rtrack->tmean_charge),0);
      else
        track_out->SetTmeanCharge(rtrack->tmean_charge,0);
    }

    track_out->SetXFirst(0,rtrack->x_first);
    track_out->SetYFirst(0,rtrack->y_first);
    track_out->SetZFirst(0,rtrack->z_first);
    track_out->SetXLast(0,rtrack->x_last);
    track_out->SetYLast(0,rtrack->y_last);
    track_out->SetZLast(0,rtrack->z_last);

  }
  else {
  
    track_out->SetXFirst(0,0);
    track_out->SetYFirst(0,0);
    track_out->SetZFirst(0,0);
    track_out->SetXLast(0,0);
    track_out->SetYLast(0,0);
    track_out->SetZLast(0,0);
  
  }

  if ((rtrack = ptr_list->rtrack_vt2_p)) {

    sum_n_fit_point += rtrack->n_fit_point;
    track_out->SetNFitPoint(rtrack->n_fit_point,1);
    track_out->SetNDedxPoint(rtrack->n_dedx_point,1);
    
    if (!isnan(rtrack->tmean_charge)) {
      if (fNormalizedDedx)
        track_out->SetTmeanCharge((1000.0*rtrack->tmean_charge),1);
      else
        track_out->SetTmeanCharge(rtrack->tmean_charge,1);
    }

    track_out->SetXFirst(1,rtrack->x_first);
    track_out->SetYFirst(1,rtrack->y_first);
    track_out->SetZFirst(1,rtrack->z_first);
    track_out->SetXLast(1,rtrack->x_last);
    track_out->SetYLast(1,rtrack->y_last);
    track_out->SetZLast(1,rtrack->z_last);

  }
  else {
  
    track_out->SetXFirst(1,0);
    track_out->SetYFirst(1,0);
    track_out->SetZFirst(1,0);
    track_out->SetXLast(1,0);
    track_out->SetYLast(1,0);
    track_out->SetZLast(1,0);
  
  }

  if ((rtrack = ptr_list->rtrack_mtp_p)) {

    sum_n_fit_point += rtrack->n_fit_point;
    track_out->SetNFitPoint(rtrack->n_fit_point,2);
    track_out->SetNDedxPoint(rtrack->n_dedx_point,2);
    
    if (!isnan(rtrack->tmean_charge)) {
      if (fNormalizedDedx)
        track_out->SetTmeanCharge((1000.0*rtrack->tmean_charge),2);
      else
        track_out->SetTmeanCharge(rtrack->tmean_charge,2);
    }

    if (fUseMTPCPoints) {
      track_out->SetXFirst(rtrack->x_first);
      track_out->SetYFirst(rtrack->y_first);
      track_out->SetZFirst(rtrack->z_first);
      track_out->SetXLast(rtrack->x_last);
      track_out->SetYLast(rtrack->y_last);
      track_out->SetZLast(rtrack->z_last);
      points_filled = kTRUE;
    }

    track_out->SetXFirst(2,rtrack->x_first);
    track_out->SetYFirst(2,rtrack->y_first);
    track_out->SetZFirst(2,rtrack->z_first);
    track_out->SetXLast(2,rtrack->x_last);
    track_out->SetYLast(2,rtrack->y_last);
    track_out->SetZLast(2,rtrack->z_last);
  
  }
  else {
  
    track_out->SetXFirst(2,0);
    track_out->SetYFirst(2,0);
    track_out->SetZFirst(2,0);
    track_out->SetXLast(2,0);
    track_out->SetYLast(2,0);
    track_out->SetZLast(2,0);
  
  }

  if ((rtrack = ptr_list->rtrack_glb_p)) {

    track_out->SetNFitPoint(sum_n_fit_point);
    if (!isnan(rtrack->tmean_charge)) {
      if (fNormalizedDedx)
        track_out->SetTmeanCharge((1000.0*rtrack->tmean_charge));
      else
        track_out->SetTmeanCharge(rtrack->tmean_charge);
    }

  }

  if (track_in) {    
    track_out->SetIdDet(track_in->id_det);
    track_out->SetPx(track_in->px);
    track_out->SetPy(track_in->py);
    track_out->SetPz(track_in->pz);
    track_out->SetSigPx(track_in->sig_px);
    track_out->SetSigPy(track_in->sig_py);
    track_out->SetSigPz(track_in->sig_pz);
    track_out->SetBx(track_in->bx);
    track_out->SetBy(track_in->by);
    track_out->SetPchi2(track_in->pchi2);

    if      (track_in->qpxz > 0) {
      track_out->SetCharge( 1);
    }
    else if (track_in->qpxz < 0) {
      track_out->SetCharge(-1);
    }
    else {
      track_out->SetCharge( 0);
    }

    if (rtrack_in) {
      track_out->SetRtrackIflag(rtrack_in->iflag);
      track_out->SetVarTmean(rtrack_in->var_tmean);
      track_out->SetNDedxPoint(rtrack_in->n_dedx_point);
      if (!points_filled) {
        track_out->SetXFirst(rtrack_in->x_first);
        track_out->SetYFirst(rtrack_in->y_first);
        track_out->SetZFirst(rtrack_in->z_first);
        track_out->SetXLast(rtrack_in->x_last);
        track_out->SetYLast(rtrack_in->y_last);
        track_out->SetZLast(rtrack_in->z_last);
      }
    }

/* temporary fix while we are obliged to use the old NA49 track_tof structure *//* BP 26/11/08 */
/* it was valid for the software versions up to v1r3mc */
//#define TEMPORARY_TOF_FIX

    if ((toftrack_in) && (tof)) {
/*
#ifdef TEMPORARY_TOF_FIX
      track_out->SetTofX(toftrack_in->sig_tof);
      track_out->SetTofY(toftrack_in->sig_beta);
#else
      track_out->SetTofX(toftrack_in->x);
      track_out->SetTofY(toftrack_in->y);
#endif
*/
/* using new track_tof structure introduced in June,2009 */ 
      track_out->SetTofX(toftrack_in->x_global);
      track_out->SetTofY(toftrack_in->y_global);

      track_out->SetTofCharge(toftrack_in->charge);
      track_out->SetTofPathl(toftrack_in->pathl);
      track_out->SetTofMass2(toftrack_in->mass2);
      track_out->SetTofSigMass2(toftrack_in->sig_mass2);
/* using new track_tof structure introduced in June,2009 */
//      track_out->SetTofId(toftrack_in->id_pm);
      track_out->SetTofId(toftrack_in->id_det);
 
      cout<<"%%%%TOF%%%% "<<toftrack_in->pathl<<" "<<toftrack_in->id_scint<<" "<<toftrack_in->tdc1<<" "<<endl;
      track_out->SetTofIdScint(toftrack_in->id_scint);
      track_out->SetTofTdc1(toftrack_in->tdc1);
      track_out->SetTofTdc2(toftrack_in->tdc2);

      track_out->SetTofIflag(toftrack_in->iflag);
    }
    else {
      track_out->SetTofX(0);
      track_out->SetTofY(0);
      track_out->SetTofCharge(0);
      track_out->SetTofPathl(0);
      track_out->SetTofMass2(0);
      track_out->SetTofSigMass2(0);
      track_out->SetTofId(0);
      track_out->SetTofIflag(0);
      track_out->SetTofIdScint(0);
      track_out->SetTofTdc1(0);
      track_out->SetTofTdc2(0);
    }

    track_out->SetWeight(0);
    track_out->SetRandom();

  }

  if (fVerbose > 1) {
    printf("p = %g %g %g\n",track_out->GetPx()
                           ,track_out->GetPy()
                           ,track_out->GetPz());
    for (i = 0; i < 4; i++) {
      printf("%d: %d %d %d\n"
            ,i,track_out->GetNPoint(i)
              ,track_out->GetNFitPoint(i)
              ,track_out->GetNMaxPoint(i));
    }
  }
    
  return kTRUE;

}
//______________________________________________________________________________
Bool_t TRootDS::FillTrackLT(T49ParticleRoot *track_out,
			    g_track_ref_t  *track_ref,
			    track_t        *track_in,
			    Bool_t tof) 
{
  //
  //  Copy data from track_t structure into T49ParticleRoot 
  //  object. <tof> = kTRUE also copied the TOF information.
  //

  Int_t   i;
  Int_t   sum_n_point      = 0;
  Int_t   sum_n_fit_point  = 0;
  Int_t   sum_n_dedx_point = 0;
  Int_t   sum_n_max_point  = 0;
  Int_t   points_filled    = kFALSE;

  for (i = 0; i < 4; i++) {
    track_out->SetNPoint(0,i);
    track_out->SetNFitPoint(0,i);
    track_out->SetNDedxPoint(0,i);
    track_out->SetNMaxPoint(0,i);
    track_out->SetTmeanCharge(0,i);
  }

  rtrack_t       *rtrack;  
  g_ptr_list_t   *ptr_list    = track_ref->g_ptr_list_p;
  rtrack_t       *rtrack_in   = ptr_list->rtrack_glb_p;
  //track_t        *track_in    = rtrack_in->first_track_p;
  track_tof_t    *toftrack_in = track_in->track_tof_p;
  rtrack_quali_t *quali       = track_ref->quali_p;

  //  vertex_t       *vtx_p = track_in->start_vertex_p;

  Int_t NGPC=0, NMTPC=0;
  Int_t NGPC_MAX=0, NMTPC_MAX=0;

  if (quali) {

    sum_n_point         = 0;
    sum_n_fit_point     = 0;
    sum_n_max_point     = 0;

    NGPC=0; 
    NMTPC=0;
    NGPC_MAX=0; 
    NMTPC_MAX=0;

    for (i = 0; i < 3; i++) {
      //      cout << "n_point[" << i << "] = " << quali->n_point[i] << endl;
      if(i == 2 && quali->n_point[i] > 1000)
	{
	  NGPC = quali->n_point[i]/1000; 
	  NMTPC = quali->n_point[i]%1000; 
	  //         cout << "NGPC = " << NGPC << "  NMTPC = " << NMTPC << endl;
	  sum_n_point += NGPC;
	  sum_n_point += NMTPC;
	  track_out->SetNPoint(NMTPC,i);
	}
      else 
	{
	  sum_n_point += quali->n_point[i];
	  track_out->SetNPoint(quali->n_point[i],i);
	}

      //      cout << "n_max_point[" << i << "] = " << quali->n_max_point[i] << endl;
      if(i == 2 && quali->n_max_point[i] > 1000)
	{
	  NGPC_MAX = quali->n_max_point[i]/1000;
	  NMTPC_MAX = quali->n_max_point[i]%1000;
	  //         cout << "NGPC_MAX = " << NGPC_MAX << "  NMTPC_MAX = " << NMTPC_MAX << endl;
	  sum_n_max_point += NGPC_MAX;
	  sum_n_max_point += NMTPC_MAX;
	  track_out->SetNMaxPoint(NMTPC_MAX,i);
	}
      else
	{
	  sum_n_max_point += quali->n_max_point[i];
	  track_out->SetNMaxPoint(quali->n_max_point[i],i);
	}
    }

    track_out->SetNPoint(sum_n_point);
    track_out->SetNMaxPoint(sum_n_max_point);

  }

  if ((rtrack = ptr_list->rtrack_vt1_p)) {

    sum_n_fit_point += rtrack->n_fit_point;
    track_out->SetNFitPoint(rtrack->n_fit_point,0);
    track_out->SetNDedxPoint(rtrack->n_dedx_point,0);

    if (!isnan(rtrack->tmean_charge)) {
      if (fNormalizedDedx)
        track_out->SetTmeanCharge((1000.0*rtrack->tmean_charge),0);
      else
        track_out->SetTmeanCharge(rtrack->tmean_charge,0);
    }

    track_out->SetXFirst(0,rtrack->x_first);
    track_out->SetYFirst(0,rtrack->y_first);
    track_out->SetZFirst(0,rtrack->z_first);
    track_out->SetXLast(0,rtrack->x_last);
    track_out->SetYLast(0,rtrack->y_last);
    track_out->SetZLast(0,rtrack->z_last);

  }
  else {

    track_out->SetXFirst(0,0);
    track_out->SetYFirst(0,0);
    track_out->SetZFirst(0,0);
    track_out->SetXLast(0,0);
    track_out->SetYLast(0,0);
    track_out->SetZLast(0,0);

  }

  if ((rtrack = ptr_list->rtrack_vt2_p)) {

    sum_n_fit_point += rtrack->n_fit_point;
    track_out->SetNFitPoint(rtrack->n_fit_point,1);
    track_out->SetNDedxPoint(rtrack->n_dedx_point,1);

    if (!isnan(rtrack->tmean_charge)) {
      if (fNormalizedDedx)
        track_out->SetTmeanCharge((1000.0*rtrack->tmean_charge),1);
      else
        track_out->SetTmeanCharge(rtrack->tmean_charge,1);
    }

    track_out->SetXFirst(1,rtrack->x_first);
    track_out->SetYFirst(1,rtrack->y_first);
    track_out->SetZFirst(1,rtrack->z_first);
    track_out->SetXLast(1,rtrack->x_last);
    track_out->SetYLast(1,rtrack->y_last);
    track_out->SetZLast(1,rtrack->z_last);

  }
  else {

    track_out->SetXFirst(1,0);
    track_out->SetYFirst(1,0);
    track_out->SetZFirst(1,0);
    track_out->SetXLast(1,0);
    track_out->SetYLast(1,0);
    track_out->SetZLast(1,0);

  }

  if ((rtrack = ptr_list->rtrack_mtp_p)) {

    sum_n_fit_point += rtrack->n_fit_point;
    track_out->SetNFitPoint(rtrack->n_fit_point,2);
    track_out->SetNDedxPoint(rtrack->n_dedx_point,2);

    if (!isnan(rtrack->tmean_charge)) {
      if (fNormalizedDedx)
        track_out->SetTmeanCharge((1000.0*rtrack->tmean_charge),2);
      else
        track_out->SetTmeanCharge(rtrack->tmean_charge,2);
    }

    if (fUseMTPCPoints) {
      track_out->SetXFirst(rtrack->x_first);
      track_out->SetYFirst(rtrack->y_first);
      track_out->SetZFirst(rtrack->z_first);
      track_out->SetXLast(rtrack->x_last);
      track_out->SetYLast(rtrack->y_last);
      track_out->SetZLast(rtrack->z_last);
      points_filled = kTRUE;
    }

    track_out->SetXFirst(2,rtrack->x_first);
    track_out->SetYFirst(2,rtrack->y_first);
    track_out->SetZFirst(2,rtrack->z_first);
    track_out->SetXLast(2,rtrack->x_last);
    track_out->SetYLast(2,rtrack->y_last);
    track_out->SetZLast(2,rtrack->z_last);

  }
  else {

    track_out->SetXFirst(2,0);
    track_out->SetYFirst(2,0);
    track_out->SetZFirst(2,0);
    track_out->SetXLast(2,0);
    track_out->SetYLast(2,0);
    track_out->SetZLast(2,0);

  }

  if ((rtrack = ptr_list->rtrack_glb_p)) {

    track_out->SetNFitPoint(sum_n_fit_point);
    if (!isnan(rtrack->tmean_charge)) {
      if (fNormalizedDedx)
        track_out->SetTmeanCharge((1000.0*rtrack->tmean_charge));
      else
        track_out->SetTmeanCharge(rtrack->tmean_charge);
    }

  }

  if (track_in) {    
    track_out->SetIdDet(track_in->id_det);
    track_out->SetPx(track_in->px);
    track_out->SetPy(track_in->py);
    track_out->SetPz(track_in->pz);
    track_out->SetSigPx(track_in->sig_px);
    track_out->SetSigPy(track_in->sig_py);
    track_out->SetSigPz(track_in->sig_pz);
    track_out->SetBx(track_in->bx);
    track_out->SetBy(track_in->by);
    track_out->SetPchi2(track_in->pchi2);

    if      (track_in->qpxz > 0) {
      track_out->SetCharge( 1);
    }
    else if (track_in->qpxz < 0) {
      track_out->SetCharge(-1);
    }
    else {
      track_out->SetCharge( 0);
    }

    if (rtrack_in) {
      track_out->SetRtrackIflag(rtrack_in->iflag);
      track_out->SetVarTmean(rtrack_in->var_tmean);
      track_out->SetNDedxPoint(rtrack_in->n_dedx_point);
      if (!points_filled) {
        track_out->SetXFirst(rtrack_in->x_first);
        track_out->SetYFirst(rtrack_in->y_first);
        track_out->SetZFirst(rtrack_in->z_first);
        track_out->SetXLast(rtrack_in->x_last);
        track_out->SetYLast(rtrack_in->y_last);
        track_out->SetZLast(rtrack_in->z_last);
      }
    }

    /* temporary fix while we are obliged to use the old NA49 track_tof structure *//* BP 26/11/08 */
    /* it was valid for the software versions up to v1r3mc */
    //#define TEMPORARY_TOF_FIX


    track_out->SetWeight(0);
    track_out->SetRandom();

  }

  if ((toftrack_in) && (tof)) {
    track_out->SetTofX(toftrack_in->x_global);
    track_out->SetTofY(toftrack_in->y_global);

    track_out->SetTofCharge(toftrack_in->charge);
    track_out->SetTofPathl(toftrack_in->pathl);
    track_out->SetTofMass2(toftrack_in->mass2);
    track_out->SetTofSigMass2(toftrack_in->sig_mass2);
    track_out->SetTofId(toftrack_in->id_det); 

    track_out->SetTofIflag(toftrack_in->iflag);
  }
  else {
    track_out->SetTofX(0);
    track_out->SetTofY(0);
    track_out->SetTofCharge(0);
    track_out->SetTofPathl(0);
    track_out->SetTofMass2(0);
    track_out->SetTofSigMass2(0);
    track_out->SetTofId(0);
    track_out->SetTofIflag(0);
  }

  if (fVerbose > 1) {
    printf("p = %g %g %g\n",track_out->GetPx()
	   ,track_out->GetPy()
	   ,track_out->GetPz());
    for (i = 0; i < 4; i++) {
      printf("%d: %d %d %d\n"
	     ,i,track_out->GetNPoint(i)
	     ,track_out->GetNFitPoint(i)
	     ,track_out->GetNMaxPoint(i));
    }
  }
    
  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::FillGTPCOnlyTrack(T49ParticleRoot *track_out, rtrack_t *rtrack_in) 
{
  //
  //  Copy data from rtrack_t structure into T49ParticleRoot 
  //  for GTPC only tracks (Id_det > 32 && n_points != 0)
  // 

  if (rtrack_in) {
    track_out->SetIdDet(rtrack_in->id_det);
    track_out->SetXFirst(rtrack_in->x_first);
    track_out->SetYFirst(rtrack_in->y_first);
    track_out->SetZFirst(rtrack_in->z_first);
    track_out->SetXLast(rtrack_in->x_last);
    track_out->SetYLast(rtrack_in->y_last);
    track_out->SetZLast(rtrack_in->z_last);
  }
  
  return kTRUE;

}
//______________________________________________________________________________
Bool_t TRootDS::FillSecTrack(T49ParticleRoot *track_out, track_t *track_in
                           , Bool_t main) 
{
  //
  //  Copy data from track_t structure into T49ParticleRoot 
  //  object for secondary tracks (reduced information)
  //

  rtrack_t *rtrack_in = track_in->rtrack_p;

  // No dE/dx information for secondary tracks
  if (fFullSecondary < 1) {
    for (Int_t i = 0; i < 4; i++) {
      track_out->SetNPoint(0,i);
      track_out->SetNFitPoint(0,i);
      track_out->SetNDedxPoint(0,i);
      track_out->SetNMaxPoint(0,i);
      track_out->SetTmeanCharge(0.,i);
    }
  }

  if (fFullSecondary < 2) {
    for (Int_t i = 0; i < 3; i++) {
      track_out->SetXFirst(i,0.);
      track_out->SetYFirst(i,0.);
      track_out->SetZFirst(i,0.);
      track_out->SetXLast(i,0);
      track_out->SetYLast(i,0);
      track_out->SetZLast(i,0);
    }
  }

  if (track_in) {

    // Fill the track_t information
    track_out->SetIdDet(track_in->id_det);
    track_out->SetPx(track_in->px);
    track_out->SetPy(track_in->py);
    track_out->SetPz(track_in->pz);
    track_out->SetSigPx(track_in->sig_px);
    track_out->SetSigPy(track_in->sig_py);
    track_out->SetSigPz(track_in->sig_pz);
    track_out->SetBx(track_in->bx);
    track_out->SetBy(track_in->by);
    track_out->SetPchi2(track_in->pchi2);
    if      (track_in->qpxz > 0) {
      track_out->SetCharge( 1);
    }
    else if (track_in->qpxz < 0) {
      track_out->SetCharge(-1);
    }
    else {
      track_out->SetCharge( 0);
    }


    if(rtrack_in){
      
      track_out->SetNFitPoint(rtrack_in->n_fit_point);
      track_out->SetNDedxPoint(rtrack_in->n_dedx_point);
      if (fNormalizedDedx)
	track_out->SetTmeanCharge((1000.0*rtrack_in->tmean_charge));
      else
        track_out->SetTmeanCharge(rtrack_in->tmean_charge);

    }//

    // Fill the rtrack_t information only if
    // there is no corresponding main vertex track
    // stored
    if (rtrack_in && (!(main))) {
      if (fVerbose > 1) {
        printf("Fill rtrack info into secondary track\n");
      }
      //track_out->SetIdDet(rtrack_in->id_det);
      track_out->SetRtrackIflag(rtrack_in->iflag);
      track_out->SetXFirst(rtrack_in->x_first);
      track_out->SetYFirst(rtrack_in->y_first);
      track_out->SetZFirst(rtrack_in->z_first);
      track_out->SetXLast(rtrack_in->x_last);
      track_out->SetYLast(rtrack_in->y_last);
      track_out->SetZLast(rtrack_in->z_last);
      track_out->SetNPoint(rtrack_in->n_point,3);
      track_out->SetNMaxPoint(rtrack_in->n_max_point,3);
    }
    else if (!(fFullSecondary)) {
      if (fVerbose > 1) {
        printf("Don't fill rtrack info into secondary track\n");
      }
      //track_out->SetIdDet(0);
      track_out->SetRtrackIflag(0);
      track_out->SetXFirst(0);
      track_out->SetYFirst(0);
      track_out->SetZFirst(0);
      track_out->SetXLast(0);
      track_out->SetYLast(0);
      track_out->SetZLast(0);
      track_out->SetNPoint(0,3);
      track_out->SetNMaxPoint(0,3);
    }

    //Commented to port tof information for LT
//     // No TOF information for the secondary tracks
//     track_out->SetTofX(0);
//     track_out->SetTofY(0);
//     track_out->SetTofCharge(0);
//     track_out->SetTofPathl(0);
//     track_out->SetTofMass2(0);
//     track_out->SetTofSigMass2(0);
//     track_out->SetTofId(0);
//     track_out->SetTofIflag(0);

//     track_out->SetWeight(0);
//     track_out->SetRandom();

  }

  if (fVerbose > 1) {
    printf("p = %g %g %g\n",track_out->GetPx()
                           ,track_out->GetPy()
                           ,track_out->GetPz());
  }
    
  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::FillRing(T49Ring *out, ring_t *in)
{
  //
  // Copies data from the ring_cal_t structure into
  // T49RingRoot
  //

  out->SetVersion(((Int_t) in->version));
  for (Int_t iRingADC = 0; iRingADC < kRingADC; iRingADC++) {
    out->SetADCphoton(iRingADC,in->adc_photon[iRingADC]);
    out->SetADChadron(iRingADC,in->adc_hadron[iRingADC]);
  }

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::FillVeto(T49Veto *out, veto_t *in, pedestal_t *ped)
{
  //
  // Copies data from the veto_t structure into
  // T49VetoRoot
  //

  const Int_t kC = 1024;
  const Int_t kN = 24;

  Int_t   iN;
  Float_t iX;
  Float_t cnon[kC];
  Float_t cnonam[kC];

  // Non-linearity correction factors for LRS2249A ADCs
  Float_t cnond[kN]  = { 1.3141, 1.3094, 1.3048, 1.2959, 1.2915, 1.2832
                       , 1.2791, 1.2731, 1.2675, 1.2638, 1.2566, 1.2531
                       , 1.2479, 1.2431, 1.2398, 1.2351, 1.2306, 1.2276
		       , 1.2232, 1.2190, 1.2163, 1.2122, 1.2083, 1.2058 };
  Float_t cnonda[kN] = { 1.9565, 1.9161, 1.8735, 1.8344, 1.8021, 1.7755
                       , 1.7468, 1.7198, 1.7004, 1.6760, 1.6560, 1.6391
                       , 1.6209, 1.6035, 1.5887, 1.5752, 1.5615, 1.5445
		       , 1.5325, 1.5205, 1.5093, 1.4981, 1.4877, 1.4773 };

  Float_t ap;
  Float_t bp;
  Float_t cp;
  Float_t ah;
  Float_t bh;
  Float_t ch;

  for (iN = 0; iN < 24; iN++) {
    cnon[iN]   = cnond[iN];
    cnonam[iN] = cnonda[iN];
  }

  ap = 0.135950;
  bp = 0.641686;
  ah = 0.250962;
  bh = 0.330524;
  for (iN = 24; iN < 32; iN++) {
    iX = ((Float_t) iN);
    cnon[iN]   = 1.0 / (ap * TMath::Log10(iX) + bp);
    cnonam[iN] = 1.0 / (ah * TMath::Log10(iX) + bh);
  }

  ap = 0.145878;
  bp = 0.626742;
  ah = 0.283347;
  bh = 0.281787;
  for (iN = 32; iN < 50; iN++) {
    iX = ((Float_t) iN);
    cnon[iN]   = 1.0 / (ap * TMath::Log10(iX) + bp);
    cnonam[iN] = 1.0 / (ah * TMath::Log10(iX) + bh);
  }

  ap = 0.153316;
  bp = 0.614105;
  ah = 0.282674;
  bh = 0.282926;
  for (iN = 50; iN < 80; iN++) {
    iX = ((Float_t) iN);
    cnon[iN]   = 1.0 / (ap * TMath::Log10(iX) + bp);
    cnonam[iN] = 1.0 / (ah * TMath::Log10(iX) + bh);
  }

  ap = 0.137100;
  bp = 0.644966;
  ah = 0.251420;
  bh = 0.342409;
  for (iN = 80; iN < 150; iN++) {
    iX = ((Float_t) iN);
    cnon[iN]   = 1.0 / (ap * TMath::Log10(iX) + bp);
    cnonam[iN] = 1.0 / (ah * TMath::Log10(iX) + bh);
  }

  ch = 18.629974;
  cp = 9.014940;
  for (iN = 150; iN < 1024; iN++) {
    iX = ((Float_t) iN);
    cnon[iN]   = (iX + cp) / iX;
    cnonam[iN] = (iX + ch) / iX;
  }

  out->SetVersion(((Int_t) in->version));

  Int_t   adcph;
  Int_t   adchd;
  Int_t   pedph;
  Int_t   pedhd; 

  Float_t coefph;
  Float_t coefhd;
  Float_t coradcph;
  Float_t coradchd;

  for (Int_t iVetoADC = 0; iVetoADC < kVetoADC; iVetoADC++) {

    adcph    = in->adc_photon[iVetoADC];
    pedph    = ((Int_t) in->ped_photon[iVetoADC]);
    adchd    = in->adc_hadron[iVetoADC];
    pedhd    = ((Int_t) ped[iVetoADC+kVetoADC].value);
    coefph   = cnon[adcph];
    coefhd   = cnonam[adchd];
    coradcph = ((Float_t) adcph * coefph - pedph);
    coradchd = ((Float_t) adchd * coefhd - pedhd);

    out->SetADCphoton(iVetoADC,coradcph);
    out->SetADChadron(iVetoADC,coradchd);

  }

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::FillBeam(T49Beam *out, beam_t *in)
{
  //
  // Copies data from the beam_t structure into
  // T49BeamRoot
  //
  Int_t          Dummy;
  bpd_t	   	*bpd_p;
  bpd_p      =  (bpd_t *) get_objects("bpd",&Dummy);
  bpd_data_t 	*H1;
  bpd_data_t 	*H2;
  bpd_data_t 	*H3;
  bpd_data_t 	*V1;
  bpd_data_t 	*V2;
  bpd_data_t 	*V3;
  H1 = bpd_p->h1_p;
  H2 = bpd_p->h2_p;
  H3 = bpd_p->h3_p;
  V1 = bpd_p->v1_p;
  V2 = bpd_p->v2_p;
  V3 = bpd_p->v3_p; 
   
  out->SetS1(in->s1);
  out->SetS2(in->s2);
  out->SetS4(in->s4);
  out->SetV0(in->v0);
  out->SetScl(in->scl);
  out->SetBs4(in->bs4);
  out->SetBung(in->bung);
  out->SetBgated(in->bgated);
  out->SetBi(in->bi);
  out->SetTrigger(in->trigger);
  out->SetBPD1x(H1->mean);
  out->SetBPD2x(H2->mean);
  out->SetBPD3x(H3->mean);
  out->SetBPD1y(V1->mean);
  out->SetBPD2y(V2->mean);
  out->SetBPD3y(V3->mean);
  out->SetBPD1RMSx(H1->rms);
  out->SetBPD2RMSx(H2->rms);
  out->SetBPD3RMSx(H3->rms);
  out->SetBPD1RMSy(V1->rms);
  out->SetBPD2RMSy(V2->rms);
  out->SetBPD3RMSy(V3->rms);  
  out->SetBPD1z(-999.9);
  out->SetBPD2z(-999.9);
  out->SetBPD3z(-999.9);
  out->SetBPDSlopeXZ(bpd_p->ext_slope_xz);
  out->SetBPDSlopeYZ(bpd_p->ext_slope_yz); 
  if (fUseEmptyADC) {
    out->SetAdcS1(in->adc_empty[9]);
    out->SetAdcS2(in->adc_empty[10]);
    out->SetAdcS3(in->adc_empty[13]);
    out->SetS4(in->adc_empty[12]);
  }
  else {
    out->SetAdcS1(in->adc_s1);
    out->SetAdcS2(in->adc_s2);
    out->SetAdcS3(in->adc_s3);
  }
  out->SetTdcVtpc1(in->tdc_vtpc1);
  out->SetTdcVtpc2(in->tdc_vtpc2);
  out->SetTdcMtpcl(in->tdc_mtpcl);
  out->SetTdcMtpcr(in->tdc_mtpcr);
  out->SetPatUnit1(in->pat_unit_1);
  out->SetPatUnit2(in->pat_unit_2);
  out->SetPatUnitEmpty(in->pat_unit_empty);
  for (Int_t i = 0; i < 45; i++) {
    out->SetEmpty(i,in->empty[i]);
  }

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDS::FillMonitor(T49MonitorTPC *out, monitor_tpc_t *in) 
{
  //
  // Copies data from the monitor_tpc_t structure into 
  // T49MonitorTPCRoot 
  //
    
  out->SetNRun(in->n_run);
  out->SetNEvent(in->n_event);
  out->SetDate(in->date);
  out->SetTime(in->time);
  out->SetNPoint(in->n_point);
  out->SetNItrack(in->n_itrack);
  out->SetNRtrack(in->n_rtrack);
  out->SetNRtrackKpass(in->n_rtrack_kpass);
  out->SetNTrack(in->n_track);
  out->SetNVertex(in->n_vertex);
  out->SetSumAdc(in->sum_adc);
  out->SetTmeanCharge(in->tmean_charge);
  out->SetAvgResX(in->avg_res_x);
  out->SetAvgResY(in->avg_res_y);
  out->SetCurVdrift(in->cur_vdrift);
  out->SetAveVdrift(in->ave_vdrift);
  out->SetRawVdrift(in->raw_vdrift);
  out->SetMonVdrift(in->mon_vdrift);
  out->SetIflagVdMod(in->iflag_vd_mod);
  out->SetGasGain(in->gas_gain);
  out->SetEvtGain(in->evt_gain);
  out->SetTemperature(in->temperature);
  out->SetPressure(in->pressure);

  return kTRUE;

}

//______________________________________________________________________________
void *TRootDS::IdsgetDS(char *ObjName,Int_t *handle, Int_t *num) 
{
  //
  // Interface to the DSPACK idsget() function with identical calling sequence
  //

  *num = 0;
  return (void *) idsget_ds(ObjName, handle, num);

}

//______________________________________________________________________________
void TRootDS::SetVerbose(Int_t n) 
{

  fVerbose = n;
  gVerbose = fVerbose;

}

//______________________________________________________________________________
void TRootDS::SetSkipEvents() 
{
  //
  // Enable the list of events that should be skipped
  // during copying. The list is read from the file
  // skipevents.d.
  //

  Int_t n;

  fSkipFound = 0;

  Char_t *inFile = "skipevents.d";
  FILE   *file   = fopen(inFile,"r");
  if (!(file)) {
    printf("Cannot open file %s.\n",inFile);
    return;
  }

  if (fVerbose) printf("Scan file %s.\n",inFile);
  Int_t runN       = 0;
  Int_t evtN       = 0;
  Int_t skipN      = 0;
  while ((n = fscanf(file,"%d %d %d",&runN,&evtN,&skipN)) > 0) {
    if (fVerbose > 1) printf("Found run %d, event %d, skip %d.\n"
                            ,runN,evtN,skipN);
    if (fSkipFound < kSkipEvents) {
      fSkipRun[fSkipFound] = runN;
      fSkipEvt[fSkipFound] = evtN;
      fSkipCnt[fSkipFound] = skipN;
      fSkipFound++;
    }
    else {
      printf("SetSkipEvents: Boundary error %d (%d)\n",fSkipFound,kSkipEvents);
    }
  }
  if (fVerbose) printf("Skip %d events\n",fSkipFound);

}


