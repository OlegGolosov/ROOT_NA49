#ifndef _v0find_str_included
#define _v0find_str_included__

/* 
$Log: v0find_root.h,v $
 * Revision 1.1  2000/01/24  16:49:15  cblume
 * Add v0find_root.h
 *
*/

typedef struct v0info_t {
  Int_t event;
  Int_t id_det;
  Int_t global;
  Int_t cuts;
  Float_t mala;
  Float_t malb;
  Float_t mak0;
  Float_t ptarm;
  Float_t alpha;
  Float_t x;
  Float_t y;
  Float_t z;
  Float_t dcax;
  Float_t dcay;
  Float_t phi;
  Float_t xtargv;
  Float_t ytargv;
  Float_t xtargp;
  Float_t ytargp;
  Float_t xtargn;
  Float_t ytargn;
  Int_t sside;
  struct vertex_t *vertex_p;
  struct avertex_v0_t *avertex_v0_p;
} v0info_t;

#endif












