#ifndef  _na49_mini_included_
#define  _na49_mini_included_

/*----------------------------------------------------------------------*/    
/*                                                                      */
/*                   NA49 Mini Structures                               */ 
/*                                                                      */ 
/*                   Mini Event Structures                              */
/*                                                                      */ 
/*                                                                      */ 
/*           dev - version 1.1       30.01.1998                         */
/*                                                                      */
/*    Comments                                                          */
/*          Units                                                       */
/*              GeV, cm  (if not given in the comments)                 */
/*                                                                      */
/*          Coordinate systems                                          */  
/*              Internal coordinate system                              */
/*                     system connected with the center of the detector */
/*                     as defined by the Hepdb data                     */
/*                                                                      */ 
/*              NA49 coordinate system                                  */
/*                     x - Jura                                         */
/*                     y - up                                           */
/*                     z - NA49 nominal beam                            */
/*                                                                      */  
/*              Beam coordinate system                                  */
/*                     x - Jura                                         */
/*                     y - up                                           */
/*                     z - beam                                         */
/*                                                                      */
/*              Structures in the NA49 coordinate system                */
/*                     mini_vertex_t                                    */
/*                     mini_track_t                                     */
/*                     mini_rtrack_t                                    */
/*                                                                      */
/*          Detector id and index:                                      */
/*                     1 - VT1                                          */
/*                     2 - VT2                                          */
/*                     3 - MTL                                          */
/*                     4 - MTR                                          */ 
/*              id_det =                                                */
/*                   sum(id=1 to 4) [0(non-active) or 1(active)]*2^id   */
/*                                                                      */
/*          Vertex types (id_vtx) and kinematical fit types (id_type)   */
/*           and the stored number of elements of the covariance        */
/*           matrix vertex.covar(...) are given                         */
/*                0 - Main vertex (6)                                   */
/*               11 - V0 (both tracks in the magnetic field - ff) (45)  */
/*               12 - V0 (positive track in field, negative outside- fn)(36)*/
/*               13 - V0 (positive track outside field, negative in- nf)(36)*/
/*               14 - V0 (both tracks outside field - nn) (28)          */
/*               21 - kink+ (ff) (45)                                   */
/*               22 - kink+ (fn) (36)                                   */
/*               23 - kink+ (nn) (28)                                   */
/*               21 - kink- (ff) (45)                                   */
/*               22 - kink- (fn) (36)                                   */
/*               23 - kink- (nn) (28)                                   */
/*               31 - tau+  (ffff) (190)                                */
/*               32 - tau+  (nnnn) (120)                                */
/*               31 - tau-  (ffff) (190)                                */
/*               32 - tau-  (nnnn) (120)                                */
/*               41 - xi+                                               */
/*               41 - xi-                                               */
/*                                                                      */
/*              Identical data structures are used to store different   */
/*              magnetic field configurations                           */
/*                                                                      */
/*-----------------------------------------------------------------------*/ 

/* #include "dspack_types.h" */

typedef struct mini_covar5x5_t { 
  Float_t   c[15];  
} mini_covar5x5_t;

/*---------------------------------------------------------------------*/
typedef struct mini_event_t {                       /* mini-DST Event Header */
   Int_t                      n_run;                /* Run number */
   Int_t                      n_event;              /* Event number */
   Int_t                      n_vertex;             /* number of vertices */
   Int_t                      mask_trig;            /* Trigger mask */
   Int_t                      date;                 /* date of the trigger */
   Int_t                      time;                 /* time of the trigger */
   Float_t                    e_veto;               /* veto energy - calibrated */
   struct ring_t              *ring_p;              /* Pointer to RING struct */
   struct bpd_t               *bpd_p;               /* Pointer to BPD  struct */
   struct cmd_t               *cmd_p;               /* Pointer to CD   struct */
   struct mini_beam_t         *beam_p;              /* Pointer to BEAM struct */
   struct mini_tpc_t          *tpc_p;               /* Pointer to TPC  struct */
   struct mini_tof_t          *tof_p;               /* Pointer to TOF  struct */
   struct mini_vertex_t       *vertex_p;            /* Pointer to vertex linked list */
} mini_event_t;

/*---------------------------------------------------------------------*/
typedef struct mini_vertex_t {          /* NA49 c.s. */
 Int_t                iflag;            /* vertex flag */
 Int_t                id_det;           /* Detector id */
 Int_t                id_vtx;           /* vertex type */
 Int_t                n_trk_fit;        /* Number of tracks used for fit */ 
 Int_t                n_trk_out;        /* number of outgoing tracks */
 Float_t              x;                /* x coordinate of vertex */  
 Float_t              y;                /* y coordinate of vertex */  
 Float_t              z;                /* z coordinate of vertex */  
 Float_t              sig_x;            /* error of x */
 Float_t              sig_y;            /* error of y */
 Float_t              sig_z;            /* error of z */
 Float_t              pchi2;            /* P(chi^2,ndf) of vertex fit */
 struct mini_track_t  *daughter_p;      /* ptr to first daughter trk */ 
 struct mini_track_t  *mother_p;        /* ptr to mother trk */ 
 struct mini_kfit_t   *kfit_p;          /* ptr to kinematic fit struct */
 struct mini_vertex_t *next_p;          /* ptr to next vertex in vertex list*/
} mini_vertex_t;

/*---------------------------------------------------------------------*/
/*----------- particle momentum at the vertex point ---------------*/
typedef struct mini_track_t {                  /* NA49 c.s. */
 Int_t          iflag;                         /* track flag  */
 Int_t          id_det;                        /* detector id */
 Float_t        charge;                        /* charge */
 Float_t        global_dedx;                   /* global dE/dx information */
 Float_t        mass2;                         /* mass squared from TOF */
 Float_t        px;                            /* momentum at vertex */
 Float_t        py;                            /* -"- */
 Float_t        pz;                            /* -"- */
 Float_t        sig_px;                        /* error of momentum */
 Float_t        sig_py;                        /* -"- */
 Float_t        sig_pz;                        /* -"- */
 Float_t        pchi2;                         /* P(chi^2,ndf) of fit */
 Float_t        bx;                            /* x_track-x_vertex at z_vertex */
 Float_t        by;                            /* y_track-y_vertex at z_vertex */
 struct mini_vertex_t     *start_vertex_p;     /* ptr back to vertex */
 struct mini_vertex_t     *stop_vertex_p;      /* ptr to stop vertex of this track */
 struct mini_rtrack_t     *rtrack_p;           /* ptr to rtrack */
 struct mini_tracktof_t   *track_tof_p;        /* ptr to track_tof */
 struct mini_track_t      *next_daughter_p;    /* ptr to next track of the same VERTEX */
 struct mini_track_t      *next_track_p;       /* ptr to next track of the same RTRACK */
} mini_track_t;

/*---------------------------------------------------------------------*/
typedef struct mini_rtrack_t {     /* NA49 c.s. */
 Int_t          n_point;           /* Number of points (3 TPCs + sum packed) */
 Int_t          n_max_point;       /* Maximum possible number of points (3 TPCs + sum packed) */
 Int_t          n_dedx_point;      /* number of dedx points (3 TPCs + sum packed) */
 Int_t          n_unfold;          /* number of unfolded points (3 TPCs + sum packed) */
 Float_t        tmean_charge[5];   /* 'truncated mean' charge (real value = tmean_charge / 10 ) */
 Float_t        dist_next;         /* distance to closest track */
 Float_t        x;                 /* x coordinate of first fitted point */
 Float_t        y;                 /* y coordinate of first fitted point */
 Float_t        z;                 /* z coordinate of first fitted point */
 Float_t        x_last;            /* x coordinate of last measured point */
 Float_t        y_last;            /* y coordinate of last measured point */
 Float_t        z_last;            /* z coordinate of last measured point */
 Float_t        px;                /* momentum according to rtrack */
 Float_t        py;                /* -"- */
 Float_t        pz;                /* -"- */
 Float_t        pchi2;             /* P(chi^2,ndf) of fit */
 struct         mini_covar5x5_t  *covar_p;         /* ptr to momentum error matrix */
 struct         mini_track_t  *first_track_p;   /* ptr to first track generated by RTRACK */
} mini_rtrack_t;

/*------------------------------------------------------------------*/
/*  Kinematical fit types and hypthesis                             */
/*       type = 1x (V0 fit) = vertex type                           */
/*                    1 - L -> p+ + pi-    (non-vertex constrain)   */
/*                    2 - AL -> pi+ + ap-  (non-vertex constrain)   */
/*                    3 - K0 -> pi+ + pi-  (non-vertex constrain)   */
/*                    4 - gamma -> e+ + e- (non-vertex constrain)   */
/*                    5 - L -> p+ + pi-    (vertex constrain)       */
/*                    6 - AL -> pi+ + ap-  (vertex constrain)       */
/*                    7 - K0 -> pi+ + pi-  (vertex constrain)       */
/*                    8 - gamma -> e+ + e- (vertex constrain)       */
/*       type = 2x (kink fit) = vertex type                         */
/*                    1 - K+/-  -> mu+/- + nu                       */
/*                    2 - K+/-  -> pi+/- + pi0                      */
/*                    3 - pi+/- -> mu+/- + nu                       */
/*                    4 - mu+/- -> e+/- + 2nu                       */
/*       type = 3x (tau fit) = vertex type                          */
/*                    1 - K+/-  -> pi+/- + pi+/- + pi-/+            */
/*       type = 4x (xi fit)  = veretx type                          */
/*                    1 - xi-/+  -> pi-/+ + Lambda/Antilambda       */
/*------------------------------------------------------------------*/
typedef struct mini_kfit_t {          /* NA49 c.s. */
   Int_t                   iflag;     /*  (0-unfitted,1-fitted,-1-error)*/
   Int_t                   id_type;   /*  fit type                      */
   Int_t                   id_hyp;    /*  fit hypothesis                */
   Int_t                   n_trk;     /*  number of tracks fitted       */
   Float_t                 chi2;      /*  fit chisq                     */
   Float_t                 pchi2;     /*  fit probability               */
   struct mini_vertex_t    *vertex_p; /* Pointer to vertex              */
   struct mini_kfit_t      *kfit_p;   /* Pointer to next kfit           */
} mini_kfit_t;

/*---------------------------------------------------------------------*/
typedef struct mini_tracktof_t {    /* TOF info for particle */
   Int_t      iflag;                /* quality flag */
   Int_t      id;                   /* detector - and PM number (packed) */
   Float_t    x;                    /* hit position in det coord. */
   Float_t    y;                    /* -"- */
   Float_t    pathl;                /* tracklength from target to det */
   Float_t    charge;               /* normalized QDC signal */
   Float_t    tof;                  /* time of flight from target to det */
   Float_t    sig_tof;              /* sigma of tof */
   Float_t    mass2;                /* squared mass */
   Float_t    sig_mass2;            /* sigma of mass2 */
} mini_tracktof_t;

/*---------------------------------------------------------------------*/
typedef struct mini_tpc_t {                       /* mini-DST TPC Header */
   Int_t                      id_det;             /* detector ID */
   Int_t                      n_vertex;           /* number of vertices */
   Int_t                      n_track;            /* number of tracks */
   Int_t                      n_bad_trk;          /* number of bad tracks */
   Int_t                      n_point;            /* number of points */
   Int_t                      n_point_on_trk;     /* number of points on tracks */
   Int_t                      sum_adc;            /* sum of all rawdata ADC counts */
   Float_t                    pressure;           /* pressure in mbar */
   Float_t                    temperature;        /* gas temperature (raw) */
   Float_t                    gas_gain;           /* gas gain (raw)  */
   Float_t                    v_drift;            /* drift velocity cm/musec (raw) */
   Float_t                    mag_field;          /* magnetic field (Gauss, probes) */
   Float_t                    clock_phase;        /* Clock phase (raw TDC) */
   struct mini_tpc_t          *next_p;            /* Pointer to TPC  struct */
} mini_tpc_t;

/*---------------------------------------------------------------------*/
typedef struct mini_tof_t {                       /* mini-DST TOF Header */
   Int_t                      iflag;              /* TOF flag */
   Int_t                      n_tof_hit;          /* number of hits in TOF walls */
   Int_t                      n_tof_match;        /* number of matched hits in TOF walls */
   struct mini_tof_t          *next_p;            /* Pointer to next TOF struct */
} mini_tof_t;

/*---------------------------------------------------------------------*/
typedef struct mini_beam_t {   /* BEAM Header: */
   char       beam_type[20];   /* beam type */
   Float_t    proj_a;          /* projectile  mass number */
   Float_t    target_a;        /* target  mass number */
   Int_t      e_lab;           /* Beam energy per nucleon (GeV) */
   Int_t      s1;              /* scalars - Station 1 */
   Int_t      s2;               
   Int_t      s3;
   Int_t      s4;
   Int_t      v0;               
   Int_t      scl;              
   Int_t      bs4;              
   Int_t      bung;             /* Station 2 */
   Int_t      bgated;           
   Int_t      bi;               
   Int_t      trigger;          
} mini_beam_t;

#endif  /* _na49_mini_included_ */
