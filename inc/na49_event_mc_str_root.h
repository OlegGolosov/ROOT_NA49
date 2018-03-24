#ifndef  _na49_event_mc_str_root_included_
#define  _na49_event_mc_str_root_included_

/************************************************************************/

typedef struct event_mc_t {   /* MC event header */
   Int_t	    n_run;             /* Run number */
   Int_t        n_event;           /* Event number */
   Int_t        grndm1;            /* First Geant random seed */
   Int_t        grndm2;            /* Second Geant random seed */
   Float_t      b_impact;          /* Impact parameter */
   Float_t      phi_impact;        /* Azimuth of b wrt +x axis, [0, 2*pi] */
   Int_t        nneut_proj_spec;   /* Projectile spectator neutrons */
   Int_t        nprot_proj_spec;   /* Projectile spectator protons */
   Int_t        nneut_targ_spec;   /* Target spectator neutrons */
   Int_t        nprot_targ_spec;   /* Target spectator protons */
   Int_t        n_vertex;          /* Number of vertices */
   Int_t        n_track_prim;      /* Number of primary tracks */
   Int_t        n_track_tot;       /* Total num of tracks */
   Int_t        n_point_tof;       /* Total num of points in tof */
   Float_t      dxdz_beam;         /* dx/dz of beam at primary vertex */
   Float_t      dydz_beam;         /* dy/dz of beam at primary vertex */
   struct vertex_mc_t  *vertex_p;  /* Pointer to first (primary) vertex */
   struct track_mc_t   *track_p;   /* Pointer to first track */
   struct point_tpc_mc_t   *pnt_vt1_p;   /* Pointer to first point in VT1 */
   struct point_tpc_mc_t   *pnt_vt2_p;   /* Pointer to first point in VT2 */
   struct point_tpc_mc_t   *pnt_mtl_p;   /* Pointer to first point in MTL */
   struct point_tpc_mc_t   *pnt_mtr_p;   /* Pointer to first point in MTR */
   struct point_tof_mc_t   *pnt_tof_p;   /* Pointer to first point in TOF */
   struct calring_mc_t  *pnt_clr_p;  /* Pointer to first pnt in ring cal. */
   struct calveto_mc_t  *pnt_clv_p;  /* Pointer to first pnt in veto cal. */
   struct calzero_mc_t  *pnt_clz_p;  /* Pointer to first pnt in zero cal. */
   struct point_tpc_mc_t    *pdr_vt1_p[144];    /* First point on padrow */
   struct point_tpc_mc_t    *pdr_vt2_p[144];    /* First point on padrow */
   struct point_tpc_mc_t    *pdr_mtl_p[450];    /* First point on padrow */
   struct point_tpc_mc_t    *pdr_mtr_p[450];    /* First point on padrow */
} event_mc_t;

/************************************************************************/

typedef struct vertex_mc_t {   /* Monte Carlo vertex structure */
   Int_t                 l_vertex;       /* Vertex number */
   Int_t                 n_daughter;     /* Number of daughter tracks */
   Float_t               x;              /* x */
   Float_t               y;              /* y */
   Float_t               z;              /* z vertex coords */
   Int_t                 process;        /* Mechanism */
   Int_t                 medium;         /* Medium */
   Float_t               path_length;    /* Path length wrt primary vtx */
   Float_t               tof;            /* TOF wrt primary vtx */
   char                  volume[4];      /* GEANT volume name */
   struct track_mc_t     *parent_p;      /* Parent track */
   struct track_mc_t     *daughter_p;    /* Pointer to l.l. of daughters */
   struct vertex_mc_t    *next_itrmd_p;  /* Next intermediate vertex */
} vertex_mc_t;

/************************************************************************/

typedef struct track_mc_t {   /* Monte Carlo track structure */
   Int_t                    l_track;          /* Track number */
   Int_t                    pid;              /* Particle ID (GEANT codes) */
   Float_t                  charge;           /* Charge */
   Float_t                  px;               /* px momentum */
   Float_t                  py;               /* py momentum */
   Float_t                  pz;               /* pz momentum */
   Float_t                  e;                /* Total energy */
   Float_t                  rapidity;         /* Rapidity */
   Float_t                  pt;               /* Transverse momentum */
   Float_t                  ptot;             /* Total momentum */
   Int_t                    is_shower;        /* 1 if shower track, 0 if not */
   Int_t                    part_mc_id;       /* ORIGINAL MC particle ID */
   Int_t                    parent1_orig;     /* Parent 1 origin */
   Int_t                    parent2_orig;     /* Parent 2 origin */   
   Float_t                  x_freeze;         /* x-coord. of freezeout */
   Float_t                  y_freeze;         /* x-coord. of freezeout */
   Float_t                  z_freeze;         /* x-coord. of freezeout */
   Float_t                  t_freeze;         /* time of freezeout */
   Int_t                    n_vt1_hit;        /* Nhits in vertex tpc1 */
   Int_t                    n_vt2_hit;        /* Nhits in vertex tpc2 */
   Int_t                    n_mtl_hit;        /* Nhits in mt left */
   Int_t                    n_mtr_hit;        /* Nhits in mt right */
   Int_t                    n_tof_hit;        /* Nhits in tof */
   struct vertex_mc_t     *start_vertex_p;   /* Pnter to start vtx of track */
   struct vertex_mc_t     *stop_vertex_p;    /* Pnter to stop vtex of track */
   struct track_mc_t      *parent_p;         /* Pointer to parent track */
   struct track_mc_t      *ur_track_p;       /* Pnter to ancestor prim. trk */
   struct track_mc_t      *next_vtx_trk_p;   /* Nxt ll track of start vertex */
   struct vertex_mc_t    *itrmd_vertex_p;   /* First intermedate vertex */
   struct point_tpc_mc_t *pnt_vt1_p;        /* 1st l.l. pnt of track VTPC1 */
   struct point_tpc_mc_t *pnt_vt2_p;        /* 1st l.l. pnt of track VTPC2 */
   struct point_tpc_mc_t *pnt_mtl_p;        /* 1st l.l. pnt of track MTPCL */
   struct point_tpc_mc_t *pnt_mtr_p;        /* 1st l.l. pnt of track MTPCR */
   struct point_tof_mc_t *pnt_tof_p;        /* 1st l.l. pnt of track TOF */
   struct ref_tpc_mc_t   *ref_vt1_p;  /* Pnter to trk param. at ref. surf. */
   struct ref_tpc_mc_t   *ref_vt2_p;  /* Pnter to trk param. at ref. surf. */
   struct ref_tpc_mc_t   *ref_mtl_p;  /* Pnter to trk param. at ref. surf. */
   struct ref_tpc_mc_t   *ref_mtr_p;  /* Pnter to trk param. at ref. surf. */
} track_mc_t;

/************************************************************************/

typedef struct point_tpc_mc_t {   /* Monte Carlo point structure for TPCs */
   Float_t                  x;                /* x */
   Float_t                  y;                /* y */
   Float_t                  z;                /* z */
   Float_t                  px;               /* px momentum */
   Float_t                  py;               /* py momentum */
   Float_t                  pz;               /* pz momentum */
   Float_t                  de;               /* Energy deposition at point */
   struct track_mc_t      *track_p;          /* Track to which this point belongs */
   struct point_tpc_mc_t  *next_trk_pnt_p;   /* Next point on track */
   struct point_tpc_mc_t  *next_pdr_pnt_p;   /* Next point on this padrow */
} point_tpc_mc_t;

/************************************************************************/

typedef struct point_tof_mc_t {   /* Monte Carlo TOF point structure */
   Int_t                   id_det;           /* TOFL = 1,TOFR = 2,TOFG = 3 */
   Float_t                 x;                /* x */
   Float_t                 y;                /* y */
   Float_t                 z;                /* z */
   Int_t                   no_scint;         /* number of scintillator */
   Float_t                 px;               /* px momentum */
   Float_t                 py;               /* py momentum */
   Float_t                 pz;               /* pz momentum */
   Float_t                 de;               /* Energy deposition at point */
   Float_t                 path_length;      /* Path length from primary vtx */
   Float_t                 tof;              /* Time of flight */
   struct track_mc_t      *track_p;    /* Track to which this point belongs */
   struct point_tof_mc_t  *next_trk_pnt_p;   /* Next tofpoint on track */
} point_tof_mc_t;

/************************************************************************/

typedef struct calring_mc_t   {   /* ring calorimeter data */
   Float_t                x;         /* x */
   Float_t                y;         /* y */
   Float_t                z;         /* z point */
   Float_t                px;        /* px momentum */
   Float_t                py;        /* py momentum */
   Float_t                pz;        /* pz momentum */
   Float_t                energy;    /* Energy deposition at point */
   struct track_mc_t     *track_p;   /* Track to which point belongs */
} calring_mc_t;

/************************************************************************/

typedef struct calveto_mc_t   {   /* veto calorimeter data */
   Float_t                x;         /* x */
   Float_t                y;         /* y */
   Float_t                z;         /* z point */
   Float_t                px;        /* px momentum */
   Float_t                py;        /* py momentum */
   Float_t                pz;        /* pz momentum */
   Float_t                energy;    /* Energy deposition at point */
   struct track_mc_t      *track_p;   /* Track to which point belongs */
} calveto_mc_t;

/************************************************************************/

typedef struct calzero_mc_t   {   /* zero calorimeter data */
   Float_t                x;         /* x */
   Float_t                y;         /* y */
   Float_t                z;         /* z point */
   Float_t                px;        /* px momentum */
   Float_t                py;        /* py momentum */
   Float_t                pz;        /* pz momentum */
   Float_t                energy;    /* Energy deposition at point */
   struct track_mc_t     *track_p;   /* Track to which point belongs */
} calzero_mc_t;

/************************************************************************/

typedef struct ref_tpc_mc_t   {   /* Track parameters at reference surface */
   Float_t  x;    /* x */
   Float_t  y;    /* y */
   Float_t  z;    /* z */
   Float_t  px;   /* px momentum */
   Float_t  py;   /* py momentum */
   Float_t  pz;   /* pz momentum */
} ref_tpc_mc_t;

/************************************************************************/

typedef struct gmc_header_t   {  /* Monte Carlo data set header */
  char machine[64];                        /* Host name   */
  Int_t date;                              /* Creation date   */
  char generator[12];                      /* Event generator   */
  double r_seed;                           /* Event generator random seed */
  Int_t extras;                            /* Flag of extras */
  Int_t options;                           /* Flag of options */
  Int_t ref_sys;                           /* LAB=1,CMS=0  */
  Int_t target_a;                          /* Projectile mass number */
  Int_t target_z;                          /* Projectile proton number */
  Int_t proj_a;                            /* Target mass number */
  Int_t proj_z;                            /* Target proton number */
  Float_t elab;                            /* Beam energy per nucleon (Lab) */
  Float_t b_min;                           /* Min. impact parameter */
  Float_t b_max;                           /* Max. impact parameter */
  Float_t psi_min;                         /* Min. angle of reaction plane */
  Float_t psi_max;                         /* Max. angle of reaction plane */
  Int_t decay_flags[2];                   /* Flags for decay suppresion ([1]=ndecay,[2]=ndecax */
} gmc_header_t;

 
/************************************************************************/

typedef struct gmc_event_t   {   /* Monte Carlo event */
  Int_t event_num;                         /* Event number   */
  Float_t b;                               /* Impact parameter   */
  Float_t psi;                             /* Angle of reaction plane */
  Int_t n_part;                            /* Number of particles   */
  Int_t n_created;                         /* Number of created particles   */
  Int_t n_ptspect;                         /* Number of proton target spectators   */
  Int_t n_ntspect;                         /* Number of neutron target spectators   */
  Int_t n_ppspect;                         /* Number of proton projectile spectators */
  Int_t n_npspect;                         /* Number of neutron projectile spectators */
} gmc_event_t;

 
/************************************************************************/

typedef struct gmc_particle_t   {
  Int_t part_id;                           /* Geant particle ID */
  Int_t part_mc_id;                        /* ORIGINAL MC particle ID */
  Int_t parent1_orig;                      /* Parent 1 origin */
  Int_t parent2_orig;                      /* Parent 2 origin */
  Int_t unstable;                          /* unstable=0 if particle is stable */
  Float_t x;                               /* x coordinate at freezout */
  Float_t y;                               /* y coordinate at freezout */
  Float_t z;                               /* z coordinate at freezout */
  Float_t time;                            /* freezout time */
  Float_t px;                              /* px momentum */
  Float_t py;                              /* py momentum */ 
  Float_t pz;                              /* pz momentum */  
  Float_t mass;                            /* mass */ 
} gmc_particle_t;

#endif  /* _na49_event_mc_str_root_included_ */

