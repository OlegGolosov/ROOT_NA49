/*=========================================================================*/
/* GTEVAL-STRUCTURES                                                       */
/*  NA49 Event Reconstruction Evaluation                                   */
/*  Author: M.Toy (toy@lbl.gov)                                            */
/*  Date:   September 1995                                                 */
/*  adapted for the use in ROOT - Christof Roland 12.11.96                 */
/*  updated by Christoph Blume 04/06/99                                    */
/*=========================================================================*/

/***************************************************************************/

#ifndef _gteval_match_t_included_
#define _gteval_match_t_included_

/* 
$Log: gteval_match_t_root.h,v $
 * Revision 1.1  1999/11/23  12:48:21  cblume
 * Add inc
 *
*/

typedef struct gteval_event_t {
  Int_t       n_point[4];
  Int_t       n_point_mc[4];
  Int_t       n_epoint[4];
  Int_t       n_epoint_mc[4];
  Int_t       n_point_tot;
  Int_t       n_point_mc_tot;
  Int_t       n_epoint_tot;
  Int_t       n_epoint_mc_tot;
  Int_t       n_gtrack;
  Int_t       n_track_mc;
  Int_t       n_etrack;
  Int_t       n_etrack_mc;
  struct event_t      *event_p;
  struct event_mc_t   *event_mc_p;
  struct etrack_t     *first_etrack_p;           /* ptr to first etrack */
  struct etrack_mc_t  *first_etrack_mc_p;        /* ptr to first MC etrack */
  struct epoint_t     *first_epoint_p;           /* ptr to first detector epoint */
  struct epoint_mc_t  *first_epoint_mc_p;        /* ptr to first MC detector epoint */
} gteval_event_t;

/****************************************************************************/

typedef struct epoint_t {
  Float_t               dx_nearest_mc[2];     /* nearest MC neighbour  point */
  Float_t               dr_nearest_mc;
  struct point_t        *point_p;            /* point data object */
  struct match_point_t  *match_p;            /* point matching structure */
  struct etrack_t       *etrack_p;           /* etrack */
  struct epoint_t       *next_trk_epnt_p;    /* next on-track epoint */
  struct epoint_t       *next_row_epnt_p;    /* next padrow epoint */
} epoint_t;

/****************************************************************************/

/* for global tracking, want etrack associated with global pointer */
/* list which in turn points to global rtrack    pmj 16/10/96 */

typedef struct etrack_t {
  Int_t                 id_det;      /* detector ID */
  Int_t                 n_epoint; /* tpc-wise on-track points */ 
  Int_t                 n_epoint_tpc[4]; /* on-track points */ 
  Int_t                 n_row;     /* tpc-wise length of track */
  Int_t                 n_row_tpc[4]; /* in padrows */
  Int_t                 n_pnt_match; /* total number of matched points */
  Int_t                 n_pnt_match_tpc[4]; /* tpc-wise matched points */
  Float_t               x_ref1[3]; /* coordinates at reference surface 1 */
  Float_t               x_ref2[3]; /* coordinates at reference surface 2 */
  Float_t               x_ref3[3]; /* coordinates at reference surface 3 */
  Float_t               p_ref1[3]; /* momenta at reference surface 1 */
  Float_t               p_ref2[3]; /* momenta at reference surface 2 */
  Float_t               p_ref3[3]; /* momenta at reference surface 3 */
  struct g_track_ref_t  *gtrack_p;   /* track data */
  struct match_track_t  *match_p;    /* matching structure */
  struct epoint_t       *epoint_p;   /* first on-track point */
} etrack_t;

/****************************************************************************/

/* NB for global tracking, reconstructed points are in external             */
/* coord. system, so mc should NOT be transformed to internal coord         */
/* system (unlike MTEVAL)  pmj 16/10/96                                     */

typedef struct epoint_mc_t {
  Int_t                  detector;           /* detector number (not 2^detector */ 
  Float_t                dx_nearest[2];       /* nearest recon neighbour point */
  Float_t                dr_nearest;         
  Float_t                x_geant;            /* original GNA49 generated position */
  Float_t                y_geant;
  Float_t                z_geant;
  Float_t                x_undis;            /* undistorted position from which */
  Float_t                y_undis;            /*   distorted point comes */
  Float_t                z_undis;
  struct point_tpc_mc_t  *point_p;           /* ptr to MC point str */
  struct match_point_t   *match_p;           /* point matching structure */
  struct etrack_mc_t     *etrack_p;          /* ptr to MC etrack */
  struct epoint_mc_t     *next_trk_epnt_p;   /* next on-track epoint */
  struct epoint_mc_t     *next_row_epnt_p;   /* next padrow epoint */
} epoint_mc_t;

/****************************************************************************/

typedef struct etrack_mc_t {
  Int_t                 id_det;      /* detector ID */
  Int_t                 n_epoint;    /* total on-track points */
  Int_t                 n_row;     /* tpc-wise length of track */
  Int_t                 n_row_tpc[4]; /* in padrows */
  Int_t                 n_pnt_match; /* total number of matched points */
  Int_t                 n_pnt_match_tpc[4]; /* tpc-wise number of matched points */
  Int_t                 n_max_pnt; /* potential length */
  Int_t                 n_max_pnt_tpc[4]; /* tpc-wise potential length */
  Float_t               x_ref1[3]; /* coordinates at reference surface 1 */
  Float_t               x_ref2[3]; /* coordinates at reference surface 2 */
  Float_t               x_ref3[3]; /* coordinates at reference surface 3 */
  Float_t               p_ref1[3]; /* momenta at reference surface 1 */
  Float_t               p_ref2[3]; /* momenta at reference surface 2 */
  Float_t               p_ref3[3]; /* momenta at reference surface 3 */
  struct track_mc_t     *track_p;    /* data object */
  struct match_track_t  *match_p;    /* matching structure */
  struct epoint_mc_t    *epoint_p;   /* first on-track point */
  struct etrack_mc_t    *etrk_parent_p; /* for decay: parent track */
  struct etrack_mc_t    *etrk_daughter_p; /* for decay: first daughter */
  struct etrack_mc_t    *etrk_next_vtx_p; /* for decay: ll of daughters */
} etrack_mc_t;

/****************************************************************************/

#endif

