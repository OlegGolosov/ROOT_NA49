///////////////////////////////////////////////////////////////
//
//  The TRootDSmini class can be used to copy DSPACK mini-DSTs
//  into ROOT mini-DSTs data structures. It derives from
//  TRootDS and provides only different implementations of
//  the filling functions.
//
///////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

// T49 includes
#include "TRootDSmini.h"

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

// DSPACK includes
#include "dsc_prot.h"
#include "dspack_rootds.h"
#include "na49_event_mc_str_root.h"
#include "na49_event_str_root.h"
#include "na49_mini_root.h"

#define MTL  0
#define MTR  1

ClassImp(TRootDSmini);

//______________________________________________________________________________
TRootDSmini::TRootDSmini():TRootDS() 
{
  //
  //  TRootDSmini destructor 
  //
  
}


//______________________________________________________________________________
TRootDSmini::~TRootDSmini() 
{
  //
  //  TRootDSmini destructor 
  //
    
}

//______________________________________________________________________________
Bool_t TRootDSmini::FillEvent() 
{
  //
  //  Copy event, track and vertex data from
  //  DSPACK mini-DSTs into ROOT NA49
  //  objects for the current event.
  //

  Int_t          i;
  Int_t          Dummy;
  Int_t          TotalTracks;
  Int_t          num;

  Char_t         name[256];

  mini_event_t  *event_p;
  mini_vertex_t *main_vtx;
  mini_vertex_t *main_vtx_fit;
  mini_beam_t   *beam_p;
  mini_rtrack_t *rtrack_p;
  mini_track_t  *track_p;
  mini_vertex_t *vtx_p;

  const Int_t    NRTrackSave = 5000;
  mini_rtrack_t *rtrack_save[NRTrackSave];

  T49ParticleRoot      *track;

  TClonesArray         *PrimaryTrackArray;
  TClonesArray         *SecondaryTrackArray;
  TObjArray            *SecVtxList;

  for (i = 0; i < NRTrackSave; i++) rtrack_save[i] = NULL;
 
  event_p = (mini_event_t *) get_objects("mini_event",&Dummy);
  if (event_p == NULL) {
    cerr << "No mini_event_t in this event" << endl;
    return kFALSE;
  }

  // Skip problematic events
  for (Int_t iFound = 0; iFound < fSkipFound; iFound++) {
    if ((event_p->n_run   == fSkipRun[iFound]) &&
        (event_p->n_event == fSkipEvt[iFound])) {
      printf("Skip run %d, event %d.\n",event_p->n_run,event_p->n_event);
      return kFALSE;
    }
  }

  if (fVerbose) {
    printf("Fill mini_event_t data (%d)\n",fVerbose);
  }
  fEvent->SetNRun(event_p->n_run);
  fEvent->SetNEvent(event_p->n_event);
  fEvent->SetDate(event_p->date);
  fEvent->SetTime(event_p->time);
  fEvent->SetTriggerMask(event_p->mask_trig);
  fEvent->SetEveto(event_p->e_veto);

  if (fVerbose) {
    printf("No monitor_tpc_t data from DSPACK in-DSTs\n");
  }

  if (fVerbose) {
    printf("Copy main vertex position\n");
  }
  main_vtx = (mini_vertex_t *) get_objects("mini_vertex",&Dummy);
  if (main_vtx) {
    if (fVerbose) {
      printf("Vertex structure\n");
    }
    if (fVerbose) {
      printf("Create T49VertexRoot\n");
    }
    T49VertexRoot *Vertex = new T49VertexRoot(kFALSE);    
    if (fVerbose) {
      printf("Fill Main vertex object\n");
    }    
    fEvent->SetMainVertexExists(true);
    FillVertex(Vertex,main_vtx);
    if (fVerbose) {
      printf("Add to Main vertex list\n");
    }
  }
  else {
    printf("vertex = NULL\n");
    fEvent->SetMainVertexExists(false);
  }
  main_vtx_fit = (mini_vertex_t *) get_objects("vertex_fit",&Dummy);
  if (main_vtx_fit) {
    if (fVerbose) {
      printf("Vertex_fit structure\n");
    }
    if (fVerbose) {
      printf("Create T49VertexRoot\n");
    }
    T49VertexRoot *Vertex = new T49VertexRoot(kFALSE);    
    if (fVerbose) {
      printf("Fill Fitted vertex object\n");
    }    
    fEvent->SetFittedVertexExists(true);
    FillVertex(Vertex,main_vtx_fit);
    if (fVerbose) {
      printf("Add to Main vertex list\n");
    }
  }
  else {
    printf("vertex_fit = NULL\n");
    fEvent->SetFittedVertexExists(false);
  }


  if (fVerbose) {
    sprintf(name,"event_%d_%d",fEvent->GetNRun(),fEvent->GetNEvent());
    sprintf(name,"Run %d, Event %d",fEvent->GetNRun(),fEvent->GetNEvent());
  }
 
  if (fVerbose) {
    printf("Copy mini_beam_t data\n");
  }
  beam_p = event_p->beam_p;
  if (!beam_p) beam_p = (mini_beam_t *) get_objects("mini_beam",&Dummy);
  if (beam_p && fEvent->GetBeam()) {
    fEvent->GetBeam()->SetS1(beam_p->s1);
    fEvent->GetBeam()->SetS2(beam_p->s2);
    fEvent->GetBeam()->SetS4(beam_p->s4);
    fEvent->GetBeam()->SetV0(beam_p->v0);
    fEvent->GetBeam()->SetScl(beam_p->scl);
    fEvent->GetBeam()->SetBs4(beam_p->bs4);
    fEvent->GetBeam()->SetBung(beam_p->bung);
    fEvent->GetBeam()->SetBgated(beam_p->bgated);
    fEvent->GetBeam()->SetBi(beam_p->bi);
    fEvent->GetBeam()->SetTrigger(beam_p->trigger);
  }
  else {
    printf("event_t->beam_p = %p\n", event_p->beam_p);
  }

  if (fVerbose) {
    printf("Copy mini_track_t data\n");
  }
  fNumTracks  = 0;
  TotalTracks = 0;

  if (!(PrimaryTrackArray = fEvent->GetPrimaryParticles())) {
    printf("No primary TrackArray found in event\n");
  }

  rtrack_p = (mini_rtrack_t *) get_objects("mini_rtrack",&num);

  if (fVerbose) {
    printf("Found mini_rtrack_t at %p (%d tracks)\n", rtrack_p, num);
  }
  for (i = 0; i < num; i++) {

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
        FillTrack(track,track_p);
        if (fVerbose > 1) {
          printf("FillTrack finished\n");
        }

        // Save the rtrack-pointer of this index for later
        if (fNumTracks < NRTrackSave) {
          rtrack_save[fNumTracks] = rtrack_p;
        }
        else {
          printf("Bondary error: fNumTracks = %d NRTrackSave = %d\n"
                ,fNumTracks,NRTrackSave);
  	}

        fNumTracks++;

      }

    }

    rtrack_p++;

    TotalTracks++;

  }

  if (fVerbose) {
    printf("finished copying main-vertex tracks\n");
  }

  // Copy the secondary vertices and the corresponding daughter tracks
  if (fFillSecondary) {

    if (fVerbose) {
      printf("Copy V0 data\n");
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

    // New version of the loop to also get the pp-data
    Int_t numVtx = 0;
    if ((vtx_p = (mini_vertex_t *) get_objects("mini_vertex",&numVtx))) {

      for (Int_t iVtx = 0; iVtx < numVtx; iVtx++) {

        // Only good V0 vertices
        if (CheckV0(vtx_p)) {

          if (fVerbose > 1) {
            printf("Create T49VertexRoot\n");
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
          if (fVerbose > 1) {
            printf("Call FillSecTrack for first daughter track\n");
          }
          FillSecTrack(track,track_p);
          if (fVerbose > 1) {
            printf("FillSecTrack finished\n");
          }
          if (track_p->charge > 0) {
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
	  // Save the index to the first particle in SecondaryTrackArray       
          SecVertex->SetSecondaryIndex(0,fNumSecTracks);
          if (fVerbose > 1) {
            printf("First particle index (non-vtx) = %d\n",fNumSecTracks);
	  }
          // Save the index to the first particle in PrimaryTrackArray (main-vtx fit)       
          for (i = 0; i < fNumTracks; i++) {
            if (rtrack_p == rtrack_save[i]) {
              SecVertex->SetPrimaryIndex(0,i);
              if (fVerbose > 1) {
                printf("First particle index (main-vtx) = %d\n",i);
	      }
              break;
	    }
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
          if (fVerbose > 1) {
            printf("Call FillSecTrack for second daughter track\n");
          }
          FillSecTrack(track,track_p);
          if (fVerbose > 1) {
            printf("FillSecTrack finished\n");
          }
	  // Save the index to the second particle in SecondaryTrackArray       
          SecVertex->SetSecondaryIndex(1,fNumSecTracks);
          if (fVerbose > 1) {
            printf("Second particle index (non-vtx) = %d\n",fNumSecTracks);
	  }
          // Save the index to the second particle in PrimaryTrackArray (main-vtx fit)       
          for (i = 0; i < fNumTracks; i++) {
            if (rtrack_p == rtrack_save[i]) {
              SecVertex->SetPrimaryIndex(1,i);
              if (fVerbose > 1) {
                printf("Second particle index (main-vtx) = %d\n",i);
	      }
              break;
	    }
          }
          fNumSecTracks++;

          fNumSecVertices++;            

        }

        vtx_p++;

      }

    }
    else {
      printf("No vertices found\n");
    }
      
    if (fVerbose) {
      printf("Finished copying V0 data\n");
    }

  }

  if (fVerbose) {
    printf("Multiplicity: %d out of %d (%g)\n"
          ,fNumTracks,TotalTracks,fEvent->GetEveto());
    if (fFillSecondary) {
      printf("Secondary vertices (tracks): %d (%d)\n"
            ,fNumSecVertices,fNumSecTracks);
    }
  }

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDSmini::CheckV0(mini_vertex_t *vtx_p) 
{
  //
  //  Defines a good V0
  //
 
  mini_rtrack_t  *rtrack_p;
  mini_track_t   *track_p;

  if (vtx_p->id_vtx != 11) return kFALSE; 

  // Get the first daughter track
  if (!(track_p  = (mini_track_t *)  vtx_p->daughter_p)) {
    printf("CheckV0: No first daughter track\n");
    return kFALSE;
  }
  if (!(rtrack_p = (mini_rtrack_t *) track_p->rtrack_p)) {
    printf("CheckV0: No first daughter rtrack\n");
    return kFALSE;
  }

  // Get the second daughter track
  if (!(track_p  = (mini_track_t *)  track_p->next_daughter_p)) {
    printf("CheckV0: No second daughter track\n");
    return kFALSE;
  }
  if (!(rtrack_p = (mini_rtrack_t *) track_p->rtrack_p)) {
    printf("CheckV0: No second daughter rtrack\n");
    return kFALSE;
  }

  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDSmini::FillVertex(T49VertexRoot *vertex_out, mini_vertex_t *vertex_in) 
{
  //
  //  Copy data from mini_vertex_t structure for main vertex
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
Bool_t TRootDSmini::FillSecVertex(T49VertexRoot *vertex_out
                                , mini_vertex_t *vertex_in) 
{
  //
  //  Copy data from a mini_vertex_t structure for V0s into 
  //  T49VertexRoot object.
  //

  mini_track_t  *track1  = NULL;
  mini_track_t  *track2  = NULL;
  mini_rtrack_t *rtrack1 = NULL;
  mini_rtrack_t *rtrack2 = NULL;
  mini_track_t  *track1m = NULL;
  mini_track_t  *track2m = NULL;
  mini_vertex_t *vertex1 = NULL;
  mini_vertex_t *vertex2 = NULL;

  Float_t px = 0;
  Float_t py = 0;
  Float_t pz = 0;
    
  Float_t x  = vertex_in->x;
  Float_t y  = vertex_in->y;
  Float_t z  = vertex_in->z;

  Float_t zTarget  = (fEvent->GetMainVertex())->GetZ();
  Float_t distTarg = z - zTarget;

  // Reconstruct V0 target position
  if ((track1 = (mini_track_t *)  vertex_in->daughter_p) &&
      (track2 = (mini_track_t *)  track1->next_daughter_p)) {
    px = track1->px + track2->px;
    py = track1->py + track2->py;
    pz = track1->pz + track2->pz;
  }

  if (pz > 0) {

    Float_t xTarg    = x - ((px / pz) * distTarg);
    Float_t yTarg    = y - ((py / pz) * distTarg);

    vertex_out->SetXTarg(xTarg);
    vertex_out->SetYTarg(yTarg);

  }
  else {

    if (fVerbose > 0) {
      printf("FillSecVertex: Vertex-pz <= 0\n");
    }
    vertex_out->SetXTarg(0.);
    vertex_out->SetYTarg(0.);

  }
  
  // Reconstruct the x-distance of the daughter tracks 
  // in the target plane (x1minx2)
  if ((track1)                                              && 
      (track2)                                              &&
      (rtrack1 = (mini_rtrack_t *) track1->rtrack_p)        &&
      (rtrack2 = (mini_rtrack_t *) track2->rtrack_p)        &&
      (track1m = (mini_track_t *)  rtrack1->first_track_p)  &&
      (track2m = (mini_track_t *)  rtrack2->first_track_p)  &&
      (vertex1 = (mini_vertex_t *) track1m->start_vertex_p) &&
      (vertex2 = (mini_vertex_t *) track2m->start_vertex_p) &&
      (vertex1->id_vtx == 0)                                &&
      (vertex2->id_vtx == 0)) {

    Float_t x1minx2 = track1m->bx - track2m->bx;

    vertex_out->SetX1minX2(x1minx2);

  }
  else {

    if (fVerbose > 0) {
      printf("FillSecVertex: No main-vertex track found\n");
    }
    
    vertex_out->SetX1minX2(0.);

  }

  return 0; // FillVertex(vertex_out,vertex_in);

}

//______________________________________________________________________________
Bool_t TRootDSmini::FillTrack(T49ParticleRoot *track_out, mini_track_t *track_in) 
{
  //
  //  Copy data from a mini_track_t structure into a T49ParticleRoot 
  //  object.
  //

  for (Int_t i = 0; i < 4; i++) {
    track_out->SetNPoint(0,i);
    track_out->SetNFitPoint(0,i);
    track_out->SetNDedxPoint(0,i);
    track_out->SetNMaxPoint(0,i);
  }

  //for (Int_t i = 0; i < 3; i++) {
  //  track_out->SetXFirst(i,0);
  //  track_out->SetYFirst(i,0);
  //  track_out->SetZFirst(i,0);
  //  track_out->SetXLast(i,0);
  //  track_out->SetYLast(i,0);
  //  track_out->SetZLast(i,0);
  //}

  mini_rtrack_t   *rtrack_in   = track_in->rtrack_p;
  mini_tracktof_t *toftrack_in = track_in->track_tof_p;

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
    track_out->SetCharge(((Int_t) track_in->charge));
    track_out->SetRtrackIflag(track_in->iflag);

    if (rtrack_in) {
      track_out->SetNPoint((rtrack_in->n_point          & 0xFF000000) / 0x01000000,0);
      track_out->SetNPoint((rtrack_in->n_point          & 0x00FF0000) / 0x00010000,1);
      track_out->SetNPoint((rtrack_in->n_point          & 0x0000FF00) / 0x00000100,2);
      track_out->SetNPoint((rtrack_in->n_point          & 0x000000FF) / 0x00000001  );
      track_out->SetNDedxPoint((rtrack_in->n_dedx_point & 0xFF000000) / 0x01000000,0);
      track_out->SetNDedxPoint((rtrack_in->n_dedx_point & 0x00FF0000) / 0x00010000,1);
      track_out->SetNDedxPoint((rtrack_in->n_dedx_point & 0x0000FF00) / 0x00000100,2);
      track_out->SetNDedxPoint((rtrack_in->n_dedx_point & 0x000000FF) / 0x00000001  );
      track_out->SetNMaxPoint((rtrack_in->n_max_point   & 0xFF000000) / 0x01000000,0);
      track_out->SetNMaxPoint((rtrack_in->n_max_point   & 0x00FF0000) / 0x00010000,1);
      track_out->SetNMaxPoint((rtrack_in->n_max_point   & 0x0000FF00) / 0x00000100,2);
      track_out->SetNMaxPoint((rtrack_in->n_max_point   & 0x000000FF) / 0x00000001  );
      //track_out->SetNFitPoint(0,0);
      //track_out->SetNFitPoint(0,1);
      //track_out->SetNFitPoint(0,2);
      //track_out->SetNFitPoint(0, );
      track_out->SetXFirst(rtrack_in->x);
      track_out->SetYFirst(rtrack_in->y);
      track_out->SetZFirst(rtrack_in->z);
      track_out->SetXLast(rtrack_in->x_last);
      track_out->SetYLast(rtrack_in->y_last);
      track_out->SetZLast(rtrack_in->z_last);
      track_out->SetTmeanCharge(rtrack_in->tmean_charge[0],0);
      track_out->SetTmeanCharge(rtrack_in->tmean_charge[1],1);
      track_out->SetTmeanCharge(rtrack_in->tmean_charge[2],2);
      track_out->SetTmeanCharge(rtrack_in->tmean_charge[3]  );
    }

    if (toftrack_in) {
      track_out->SetTofX(toftrack_in->x);
      track_out->SetTofY(toftrack_in->y);
      track_out->SetTofCharge(toftrack_in->charge);
      track_out->SetTofPathl(toftrack_in->pathl);
      track_out->SetTofMass2(toftrack_in->mass2);
      track_out->SetTofSigMass2(toftrack_in->sig_mass2);
      track_out->SetTofId(toftrack_in->id);
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

    //track_out->SetWeight(1);
    //track_out->SetRandom();

  }

  if (fVerbose > 1) {
    printf("p = %g %g %g\n",track_out->GetPx()
                           ,track_out->GetPy()
                           ,track_out->GetPz());
    for (Int_t i = 0; i < 4; i++) {
      printf("%d: %d %d %d\n"
            ,i,track_out->GetNPoint(i)
              ,track_out->GetNFitPoint(i)
              ,track_out->GetNMaxPoint(i));
    }
  }
    
  return kTRUE;

}

//______________________________________________________________________________
Bool_t TRootDSmini::FillSecTrack(T49ParticleRoot *track_out, mini_track_t *track_in) 
{
  //
  //  Copy data from a mini_track_t structure into a T49ParticleRoot 
  //  object for secondary tracks (reduced information)
  //

  mini_rtrack_t *rtrack_in = track_in->rtrack_p;

  // No dE/dx information for secondary tracks
  for (Int_t i = 0; i < 4; i++) {
    track_out->SetNPoint(0,i);
    track_out->SetNFitPoint(0,i);
    track_out->SetNDedxPoint(0,i);
    track_out->SetNMaxPoint(0,i);
    track_out->SetTmeanCharge(0,i);
  }

  //for (Int_t i = 0; i < 3; i++) {
  //  track_out->SetXFirst(i,0.);
  //  track_out->SetYFirst(i,0.);
  //  track_out->SetZFirst(i,0.);
  //}

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
    track_out->SetCharge(((Int_t) track_in->charge));
    track_out->SetRtrackIflag(track_in->iflag);

    // Fill the rtrack_t information
    if (rtrack_in) {
      track_out->SetXFirst(rtrack_in->x);
      track_out->SetYFirst(rtrack_in->y);
      track_out->SetZFirst(rtrack_in->z);
      track_out->SetXLast(rtrack_in->x_last);
      track_out->SetYLast(rtrack_in->y_last);
      track_out->SetZLast(rtrack_in->z_last);
      track_out->SetNPoint((rtrack_in->n_point        & 0x000000FF) / 0x00000001);
      track_out->SetNMaxPoint((rtrack_in->n_max_point & 0x000000FF) / 0x00000001);
    }

    // No TOF information for the secondary tracks
    track_out->SetTofX(0);
    track_out->SetTofY(0);
    track_out->SetTofCharge(0);
    track_out->SetTofPathl(0);
    track_out->SetTofMass2(0);
    track_out->SetTofSigMass2(0);
    track_out->SetTofId(0);
    track_out->SetTofIflag(0);

    //track_out->SetWeight(1);
    //track_out->SetRandom();

  }

  if (fVerbose > 1) {
    printf("p = %g %g %g\n",track_out->GetPx()
                           ,track_out->GetPy()
                           ,track_out->GetPz());
  }
    
  return kTRUE;

}



