/****************************************************************
 *
 * DS2CWN for global tracking
 *
 *             Peter Jacobs, October 1996
 *
 ****************************************************************/

/*****************************************************************************/

/* 
$Log: gteval_nt_root.h,v $
 * Revision 1.2  1999/11/24  16:04:17  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 *
 * Revision 1.1  1999/11/23  12:48:24  cblume
 * Add inc
 *
*/

typedef struct nt_track_mc_t  {
  Int_t       l_track;
  Int_t       id_det;
  Int_t       pid;
  Float_t     y;
  Float_t     ypi;
  Float_t     pt;

  Float_t     x_ref1[3];
  Float_t     x_ref2[3];
  Float_t     x_ref3[3];
  Float_t     p_ref1[3];
  Float_t     p_ref2[3];
  Float_t     p_ref3[3];

  Int_t       proc;

  Int_t       n_point_mc;
  Int_t       n_point_mc_tpc[4];
  Int_t       n_point_mc_match_tpc[4];

  Int_t       n_match;

  Int_t       m1_ltrack;
  Int_t       m1_common[4];
  Int_t       m1_unique[4];
  Float_t     m1_length[4];
  Int_t       m1_n_match_mc;
  Float_t     m1_x_first[3];
  Float_t     m1_x_last[3];
  Int_t       m1_id_det;
  Int_t       m1_n_point_tpc[4];

  Float_t     m1_x_ref1[3];
  Float_t     m1_x_ref2[3];
  Float_t     m1_x_ref3[3];

  Float_t     m1_p_ref1[3];
  Float_t     m1_p_ref2[3];
  Float_t     m1_p_ref3[3];

  Int_t       m2_ltrack;
  Int_t       m2_common[4];
  Int_t       m2_unique[4];
  Float_t     m2_length[4];
  Int_t       m2_n_match_mc;
  Float_t     m2_x_first[3];
  Float_t     m2_x_last[3];
  Int_t       m2_id_det;
  Int_t       m2_n_point_tpc[4];

  Float_t     m2_x_ref1[3];
  Float_t     m2_x_ref2[3];
  Float_t     m2_x_ref3[3];

  Float_t     m2_p_ref1[3];
  Float_t     m2_p_ref2[3];
  Float_t     m2_p_ref3[3];

  Float_t     m1_m2_qinv_ref1;
  Float_t     m1_m2_qinv_ref3;
} nt_track_mc_t;

/*****************************************************************************/

typedef struct nt_gtrack_t  {
  Int_t       l_track;
  Int_t       id_det;
  Int_t       n_point;
  Int_t       n_point_tpc[4];
  Int_t       n_point_match_tpc[4];

  Float_t     x_first[3];
  Float_t     x_last[3];
  Float_t     y;
  Float_t     pt;
  Float_t     ptot;

  Float_t     x_ref1[3];
  Float_t     x_ref2[3];
  Float_t     x_ref3[3];
  Float_t     p_ref1[3];
  Float_t     p_ref2[3];
  Float_t     p_ref3[3];

  Int_t       n_match_mc;

  Int_t       m1_ltrack;
  Int_t       m1_common[4];
  Int_t       m1_unique[4];
  Float_t     m1_length[4];
  Int_t       m1_n_point_mc_tpc[4];
  Float_t     m1_y;
  Float_t     m1_ypi;
  Float_t     m1_pt;

  Float_t     m1_x_ref1[3];
  Float_t     m1_x_ref2[3];
  Float_t     m1_x_ref3[3];
  Float_t     m1_p_ref1[3];
  Float_t     m1_p_ref2[3];
  Float_t     m1_p_ref3[3];

  Int_t       m2_ltrack;
  Int_t       m2_common[4];
  Int_t       m2_unique[4];
  Float_t     m2_length[4];
  Int_t       m2_n_point_mc_tpc[4];
  Float_t     m2_y;
  Float_t     m2_ypi;
  Float_t     m2_pt;

  Float_t     m2_x_ref1[3];
  Float_t     m2_x_ref2[3];
  Float_t     m2_x_ref3[3];
  Float_t     m2_p_ref1[3];
  Float_t     m2_p_ref2[3];
  Float_t     m2_p_ref3[3];

  Float_t     m1_m2_qinv_ref1;
  Float_t     m1_m2_qinv_ref2;
  Float_t     m1_m2_qinv_ref3;
} nt_gtrack_t;

/*****************************************************************************/
typedef struct nt_decay_mc_t  {
  Int_t     l_track;
  Int_t     id_det;
  Int_t     pid;
  Float_t   y;
  Float_t   pt;
  Float_t   p[3];
  Float_t   x_decay[3];
  Int_t     n_daughter;

  Int_t     n_point_mc;
  Int_t     n_point_mc_tpc[4];
  Int_t     n_point_mc_match_tpc[4];

  Int_t     n_match;
  Int_t     match_id_det;
  Int_t     match_npt_tpc[4];
  Float_t   match_momentum[3];

  Int_t     daughter1_id_det;
  Int_t     daughter1_pid;
  Float_t   daughter1_p[3];
  Int_t     daughter1_npt_mc_tpc[4];
  Int_t     daughter1_npt_mc_match_tpc[4];
  Int_t     daughter1_n_match;
  Int_t     daughter1_match_id_det;
  Int_t     daughter1_match_npt_tpc[4];
  Float_t   daughter1_match_momentum[3];

  Int_t     daughter2_id_det;
  Int_t     daughter2_pid;
  Float_t   daughter2_p[3];
  Int_t     daughter2_npt_mc_tpc[4];
  Int_t     daughter2_npt_mc_match_tpc[4];
  Int_t     daughter2_n_match;
  Int_t     daughter2_match_id_det;
  Int_t     daughter2_match_npt_tpc[4];
  Float_t   daughter2_match_momentum[3];
} nt_decay_mc_t;

/**********************************************************/
struct nt_point_mc_t  {
  Float_t   x;
  Float_t   y;
  Float_t   z;
  Float_t   dx_nn;
  Float_t   dy_nn;
  Int_t     match;
}  nt_point_mc_p;

/**********************************************************/
struct nt_point_t  {
  Float_t   x;
  Float_t   y;
  Float_t   z;
  Float_t   dx_nn_mc;
  Float_t   dy_nn_mc;
  Int_t     match;
}  nt_point_p;

/**********************************************************/


