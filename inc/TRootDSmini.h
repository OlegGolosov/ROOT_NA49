#ifndef _TRootDSmini_included_
#define _TRootDSmini_included_

/*
$Log: TRootDSmini.h,v $
 * Revision 1.1  1999/12/16  12:19:17  cblume
 * Add TRootDSmini.h
 *
*/

#include "TRootDS.h"

struct mini_tpc_t;
struct mini_event_t;
struct mini_rtrack_t;
struct mini_track_t;
struct mini_vertex_t;
struct mini_beam_t;

class TRootDSmini : public TRootDS {
  
public: 

  TRootDSmini(); 
  virtual ~TRootDSmini();

  virtual Bool_t        FillEvent();
          Bool_t        FillTrack(T49ParticleRoot *track_out, mini_track_t *track_in);
          Bool_t        FillSecTrack(T49ParticleRoot *track_out, mini_track_t *track_in);
          Bool_t        FillVertex(T49VertexRoot *vertex_out, mini_vertex_t *vertex_in);
          Bool_t        FillSecVertex(T49VertexRoot *vertex_out, mini_vertex_t *vertex_in);
          Bool_t        CheckV0(mini_vertex_t *vtx_p);
  
protected:
  
  ClassDef(TRootDSmini,1)      // Interface-class between DSPACK mini-DSTs and ROOT

};

#endif








