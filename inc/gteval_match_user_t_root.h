/***************************************************************************/
/* User defined typedefs for MTEVAL */
/* Modified for gteval    pmj  16/10/96 */
/* Adapted to ROOT Christof Roland 12/11/96 */
/* Updated by Christoph Blume 04/06/99 */
/***************************************************************************/

#ifndef _gteval_match_user_t_included_
#define _gteval_match_user_t_included_

/* 
$Log: gteval_match_user_t_root.h,v $
 * Revision 1.1  1999/11/23  12:48:23  cblume
 * Add inc
 *
*/

/***************************************************************************/
 
typedef struct gteval_point {
  Float_t alloc_match;  /* frac. of # MC points for match_point_t alloc */
  Float_t search_x[5];  /* search distance sector types VT1, VT2, HR, SR, SRprime */
  Float_t search_y;
} gteval_point;


typedef struct gteval_track {
  Float_t alloc_match;  /* frac. of # tracks for match_track_t alloc */
  Int_t   min_hits_mc_vt1;  /* min hits for MC track match candidate */
  Int_t   min_hits_mc_vt2;  /* min hits for MC track match candidate */
  Int_t   min_hits_mc_mtl;  /* min hits for MC track match candidate */
  Int_t   min_hits_mc_mtr;  /* min hits for MC track match candidate */
  Int_t   min_match_vt1;     /* min hits for track match in vt1 */
  Int_t   min_match_vt2;     /* min hits for track match in vt2 */
  Int_t   min_match_mtl;     /* min hits for track match in mtl */
  Int_t   min_match_mtr;     /* min hits for track match in mtr */
} gteval_track;

/***************************************************************************/

typedef struct match_point_t {  /* Point matching structure */
  Float_t               xdist;             /* separation in x */
  Float_t               ydist;             /* separation in y */
  Int_t                 n_match;           /* # recon matched to this MC */
  Int_t                 n_match_mc;        /* # MC matched to this recon */
  struct epoint_t       *epoint_p;         /* ptr to this recon point */
  struct epoint_mc_t    *epoint_mc_p;      /* ptr to this MC point */
  struct match_point_t  *next_match_p;     /* next matched recon to this MC */
  struct match_point_t  *next_match_mc_p;  /* next matched MC to this recon */
} match_point_t;

/****************************************************************************/

typedef struct match_track_t {  /* track matching structure */
  Int_t                 n_pnt_common;     /* # of points in common */
  Int_t                 n_pnt_unique;     /* # of pnts w/ unique matches */
  Int_t                 n_pnt_common_tpc[4]; /* # of points in common per tpc */
  Int_t                 n_pnt_unique_tpc[4]; /* # of pnts w/ unique matches per tpc*/
  Int_t                 n_row_tpc[4];     /* row length of matching region */
  Float_t               length_match_tpc[4]; /* length in cm of matching region per tpc*/
  Int_t                 n_match;          /* # rec trks matched to this MC */
  Int_t                 n_match_mc;       /* # MC trks matched to this rec */
  struct etrack_t       *etrack_p;        /* ptr to recon trk */
  struct etrack_mc_t    *etrack_mc_p;     /* ptr to MC trk */
  struct match_track_t  *next_match_p;    /* another matched recon track */
  struct match_track_t  *next_match_mc_p; /* another matched MC track */
} match_track_t;

/****************************************************************************/

#endif




