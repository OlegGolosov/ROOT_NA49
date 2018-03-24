#ifdef __CINT__

/*
$Log: T49StrLinkDef.h,v $
 * Revision 1.8  2000/10/25  09:36:06  cblume
 * Include WFA structures
 *
 * Revision 1.7  2000/04/25  09:35:36  cblume
 * Added xi-finder structures
 *
 * Revision 1.6  2000/04/19  11:23:13  cblume
 * Added vpc-structures
 *
 * Revision 1.5  2000/02/04  10:37:21  cblume
 * Update of the NA49 structures. New: bpd_vertex_h, ring_cal_t, bos_pestof_t, pestof_data_t, rel_gains_t, pedestal_t, vpc_data_t, cmd_hit_t, cmd_t, cmd_data_t, sihodo_data_t, beam_aux_t
 *
 * Revision 1.4  2000/01/24  16:50:36  cblume
 * Added v0info_t
 *
 * Revision 1.3  1999/12/16  12:21:28  cblume
 * Added DSPACK mini-DST structures.
 *
 * Revision 1.2  1999/11/24  16:04:03  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 *
 * Revision 1.1  1999/11/23  12:48:16  cblume
 * Add inc
 *
*/

#pragma link off all globals;
#pragma link off all functions;
#pragma link off all classes;

#pragma link C++ class gteval_event_t;
#pragma link C++ class epoint_t;
#pragma link C++ class epoint_mc_t;
#pragma link C++ class etrack_t;
#pragma link C++ class etrack_mc_t;
#pragma link C++ class gteval_point;
#pragma link C++ class gteval_track;
#pragma link C++ class match_point_t;
#pragma link C++ class match_track_t;
#pragma link C++ class nt_track_mc_t;
#pragma link C++ class nt_gtrack_t;
#pragma link C++ class nt_decay_mc_t;
#pragma link C++ class nt_point_mc_t;
#pragma link C++ class nt_point_t;

#pragma link C++ class covar3x3_t;
#pragma link C++ class covar4x4_t;
#pragma link C++ class covar5x5_t;
#pragma link C++ class covar9x9_t;
#pragma link C++ class covar13x13_t;
#pragma link C++ class event_t;
#pragma link C++ class bos_t;
#pragma link C++ class bos_data_t;
#pragma link C++ class tpc_t;
#pragma link C++ class point_t;
#pragma link C++ class point_fat_t;
#pragma link C++ class itrack_t;
#pragma link C++ class rtrack_t;
#pragma link C++ class track_t;
#pragma link C++ class vertex_t;
#pragma link C++ class avertex_v0_t;
#pragma link C++ class atrack_t;
#pragma link C++ class kfit_t;
#pragma link C++ class ktrack_t;
#pragma link C++ class beam_t;
#pragma link C++ class beam_aux_t;
#pragma link C++ class sihodo_data_t;
#pragma link C++ class sihodo_t;
#pragma link C++ class ring_t;
#pragma link C++ class ring_cal_t;
#pragma link C++ class veto_t;
#pragma link C++ class bos_tof_t;
#pragma link C++ class bos_gtof_t;
#pragma link C++ class tof_t;
#pragma link C++ class tof_hits_t;
#pragma link C++ class track_tof_t;
#pragma link C++ class monitor_tpc_t;
#pragma link C++ class monitor_tof_t;
#pragma link C++ class bpd_data_t;
#pragma link C++ class bpd_t;
#pragma link C++ class bpd_vertex_t;
#pragma link C++ class mini_pnt_lst_t;
#pragma link C++ class mini_pnt_hdr_t;
#pragma link C++ class g_track_ref_t;
#pragma link C++ class g_ptr_list_t;
#pragma link C++ class g_predict_pnt_t;
#pragma link C++ class rtrack_quali_t;
#pragma link C++ class cmd_data_t;
#pragma link C++ class cmd_t;
#pragma link C++ class cmd_hit_t;
#pragma link C++ class vpc_t;
#pragma link C++ class vpc_data_t;
#pragma link C++ class vpc_clust_t;
#pragma link C++ class vpc_point_t;
#pragma link C++ class vpc_track_t;
#pragma link C++ class wfa_t;
#pragma link C++ class wfa_time_t;

#pragma link C++ class pedestal_t;
#pragma link C++ class rel_gains_t;
#pragma link C++ class pestof_data_t;
#pragma link C++ class bos_pestof_t;

#pragma link C++ class clust_const_t;
#pragma link C++ class morph_const_t;
#pragma link C++ class const_sec_t;
#pragma link C++ class const_head_t;
#pragma link C++ class dedx_charge_t;
#pragma link C++ class dedx_truncm_t;
#pragma link C++ class dedx_cal_t;
#pragma link C++ class dedx_time_cal_t;

#pragma link C++ class event_mc_t;
#pragma link C++ class vertex_mc_t;
#pragma link C++ class track_mc_t;
#pragma link C++ class point_tpc_mc_t;
#pragma link C++ class point_tof_mc_t;
#pragma link C++ class calring_mc_t;
#pragma link C++ class calveto_mc_t;
#pragma link C++ class calzero_mc_t;
#pragma link C++ class ref_tpc_mc_t;
#pragma link C++ class gmc_header_t;
#pragma link C++ class gmc_event_t;
#pragma link C++ class gmc_particle_t;

#pragma link C++ class mini_covar5x5_t;
#pragma link C++ class mini_event_t;
#pragma link C++ class mini_vertex_t;
#pragma link C++ class mini_track_t;
#pragma link C++ class mini_rtrack_t;
#pragma link C++ class mini_kfit_t;
#pragma link C++ class mini_tracktof_t;
#pragma link C++ class mini_tpc_t;
#pragma link C++ class mini_tof_t;
#pragma link C++ class mini_beam_t;

#pragma link C++ class v0info_t;

#pragma link C++ class xi_cuts_t;
#pragma link C++ class avertex_xi_t;
#pragma link C++ class la_in_t;

#endif









