/*----------------------------------------------------------------------*/    
/*                                                                       */
/*                   NA49 Structures                                    */ 
/*                   Event Structures                                   */
/*                                                                      */ 
/*           dev - version 1.0       6.11.1995                          */
/*                                                                      */
/*                                                                      */
/*    Comments:                                                         */
/*          units: GeV, cm  (if not given in the comments)              */
/*                                                                      */
/*          Coordinate systems:                                         */  
/*              Internal coordinate systemr:                            */
/*                     system connected with the center of the detector */
/*                     as defined by the Hepdb data                     */
/*                                                                      */ 
/*              NA49 coordinate system:                                 */
/*                     x - Jura                                         */
/*                     y - up                                           */
/*                     z - NA49 nominal beam                            */
/*                                                                      */  
/*              Beam coordinate system:                                 */
/*                     x - Jura                                         */
/*                     y - up                                           */
/*                     z - beam                                         */
/*                                                                      */
/*              Structures in the internal coordinate system:           */
/*                     itrack_t                                         */
/*              Structures in the NA49 coordinate system:               */
/*                     point_t, rtrack_t, vertex_t, track_t, ktrack_t   */
/*              Structures in the beam coordinate system:               */
/*                     atrack_t                                         */
/*                     avertex_t                                        */
/*                                                                      */
/*              Detector id and index:                                  */
/*                     1 - VT1                                          */
/*                     2 - VT2                                          */
/*                     3 - MTL                                          */
/*                     4 - MTR                                          */ 
/*              id_det =                                                */
/*                   sum(id=1 to 4) [0(non-active) or 1(active)]*2^id   */
/*                                                                      */
/*                                                                      */
/*              Track parameters (NA49 c.s.):                           */
/*                     qpxz = charge/sqrt(px^2 + pz^2)                  */
/*                     tanl = py/sqrt(px^2 + pz^2)                      */
/*                     phi  = atan2(pz, px)                             */
/*                           if(phi<0) then phi = phi + 2.*pi           */
/*                           (0 =< phi < 2*pi)                          */
/*                                                                      */
/*              Track parameters (beam c.s.):                           */
/*                     pt  = sqrt(px^2 +py^2)                           */
/*                     psi = atan2(py, px)                              */
/*                           if(psi<0) then psi = psi + 2.*pi           */
/*                           (0 =< psi < 2*pi)                          */
/*                     y.. = 0.5*ln[(e+pz)/(e-pz)]                      */
/*                     [e = sqrt(px^2 + py^2 + pz^2 + m..^2)]           */ 
/*                                                                      */ 
/*            Vertex types (id_vtx) and kinematical fit types (id_type) */
/*            and the stored number of elements of the covariance       */
/*            matrix vertex.covar(...) are given                        */
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
/*               50 - long target minimum distance point                */
/*                                                                      */
/*              Identical data structures are used to store different   */
/*              magnetic field configurations                           */
/*                                                                      */
/*              Covariance matrix:                                      */ 
/*                    only upper right 'corner' of the covariance matrix*/
/*                    is stored,                                        */
/*                    the elements are stored from left to right, row by row*/
/*                    starting from the top row, e.g.                   */
/*                                         1 2 3                        */
/*                     covarinace matrix:  4 5 6                        */
/*                                         7 8 9                        */ 
/*                     covar(1) = 1                                     */
/*                     covar(2) = 2                                     */
/*                     covar(3) = 3                                     */
/*                     covar(4) = 5                                     */
/*                     covar(5) = 6                                     */
/*                     covar(6) = 9                                     */ 
/*                                                                      */
/*                     The order of the variables in the covarinace     */
/*                     matrix is given by the order in which they are   */
/*                     listed in the structure.                         */
/*                                                                      */
/*                     In the case when covariance matrix is larger than*/
/*                     needed (field off data) the first elements of the*/
/*                     matrix are used according to the above convention.*/
/*                                                                       */
/*                     In the case of ktrack only elements which correspond*/
/*                     to a given track parameter are stored.            */
/*                                                                       */
/*                                                                       */
/*-----------------------------------------------------------------------*/ 

/*********************** includes ******************************/
#ifndef  _na49_event_str_included_
#define  _na49_event_str_included_


typedef struct covar3x3_t  { 
  Float_t   c[6];  
} covar3x3_t;

typedef struct covar4x4_t  { 
  Float_t   c[10];  
} covar4x4_t;

typedef struct covar5x5_t  { 
  Float_t   c[15];  
} covar5x5_t;

typedef struct covar9x9_t  { 
  Float_t   c[45];  
} covar9x9_t;

typedef struct covar13x13_t  { 
  Float_t   covar[91];  
} covar13x13_t;

/********************** event ***********************************/
typedef struct event_t  {   /* Event Header */
   Int_t               n_run;          /* Run number */
   Int_t               n_event;        /* Event number */
   Int_t               mask_trig;      /* Trigger mask */
   Int_t               date;           /* date of the trigger */
   Int_t               time;           /* time of the trigger */
   Float_t             pressure;       /* pressure in mb   */
   Float_t             clock_phase[4]; /* Clock phase (raw TDC) */
   Float_t             gas_gain[4];    /* gas gain (raw)  */
   Float_t             v_drift[4];     /* drift velocity cm/musec (raw) */
   Float_t             temperature[4]; /* gas temperature (raw) */
   Float_t             mag_field[2];   /* magnetic field (Gauss, probes) */
   struct bos_t        *bos_p;         /* Ptr to non-tpc raw data header */
   struct bos_t        *bos_vt1_p;     /* Ptr to raw data header for vt1 */
   struct bos_t        *bos_vt2_p;     /* Ptr to raw data header for vt2 */
   struct bos_t        *bos_mtl_p;     /* Ptr to raw data header for mtl */
   struct bos_t        *bos_mtr_p;     /* Ptr to raw data header for mtr */
   struct tpc_t        *vt1_p;         /* Pointer to VTPC 1 struct */
   struct tpc_t        *vt2_p;         /* Pointer to VTPC 2 struct */
   struct tpc_t        *mtl_p;         /* Pointer to MTPC left struct */
   struct tpc_t        *mtr_p;         /* Pointer to MTPC right struct */
   struct tof_t        *tofl_p;        /* Pointer to TOF left struct */
   struct tof_t        *tofr_p;        /* Pointer to TOF right struct */
   struct tof_t        *tofgl_p;       /* Pointer to left grid TOF struct */
   struct tof_t        *tofgr_p;       /* Pointer to right grid TOF struct */
   struct veto_t       *veto_p;        /* Pointer to VETO struct */
   struct ring_t       *ring_p;        /* Pointer to RING struct */
   struct beam_t       *beam_p;        /* Pointer to BEAM struct */
   struct sihodo_t     *sihodo_p;      /* Pointer to SIHODO struct */
   void                *global_p;      /* Pointer to future global ev. str.  */
} event_t;

/****************************    TPC     ***********************************/

/*---------------------------    bos -------------------------------------*/
typedef struct bos_t  {   /* raw data header */
   Int_t             name_bos;     /* BOS name */
   Int_t             number_bos;   /* BOS number */
   Int_t             length_bos;   /* BOS length in words */
   Int_t             index;        /* index to start of this bank in raw data */
} bos_t;


/*-----------------------------------------------------------------------*/
typedef struct bos_data_t  {   /* raw data */
   Int_t    data[2];   /* ADC data packed in BOS format */
} bos_data_t;


/*-------------------------- detector header ------------------------------*/
typedef struct tpc_t  {   
   Int_t              n_point;     /* number of points in detector */
   Int_t              n_itrack;    /* number of itracks in detector */
   Int_t              n_rtrack;    /* number of rtracks in detector */
   Int_t              n_vertex;    /* number of vertices in detector */
   Float_t            v_drift;     /* cm/us (used for reconstruction ) */
   Int_t              iflag_vdrift;/* flags origin of used v_drift */
   Float_t            t_offset;    /* time offset incl. clock jitter (used) */
   Float_t            pressure;    /* pressure in mb (used)   */
   Float_t            gas_gain;    /* gas gain (used)  */
   Float_t            temperature; /* gas temperature (used) */
   struct point_t     *point_p;    /* ptr to first detector point */
   struct itrack_t    *itrack_p;   /* ptr to first detector itrack  */
   struct rtrack_t    *rtrack_p;   /* ptr to first detector rtrack  */
   struct vertex_t    *vertex_p;   /* ptr to first detector vertex */
   struct vertex_t    *main_vertex_p; /* ptr to main vertex for detector*/
} tpc_t;


/*------------------- coded point structure ------------------------------*/
typedef struct point_t  {   /* NA49 c.s. */
  Int_t       iflag;		/* bitmask of information */
  Int_t    position;		/* sec row position and pad & time sizes */
  Int_t    charge;		/* total charge and max_adc */
  Float_t  x;			/* positions in external coordinates */
  Float_t  y;
  Float_t  z;
  Int_t    sigma;		/* Uncertainty in the position of the point */
  Int_t    variance;		/* Width of the cluster */
  void     *irtrack_p;	/* ptr to itrack/rtrack to which this belongs */
  struct point_t *next_tpoint_p;	/* pointer to next point on rtrack */
} point_t;


/* Packing Conventions: */
/* position contains in the following information in the specified bits: */
/* 0-7		absolute row number, 1-234 */
/* 8-12		sector number, 1-6 (VTPC) or 1-25 (MTPC) */
/* 13-17	row number, 1-18 */
/* 18-20	detector number, 1-4 */
/* 21-24	number of pads */
/* 25-30	number of time slices from end to end */


/* charge contains the following information in the specified bits: */
/* 0-23		Integrated/summed charge */
/* 24-31	The maximum ADC value contained in the cluster */


/* sigma and variance are packed in the following manner: */
/* sigma = (int) (sig_x*1000+0.5) + (int) (sig_y*1000+0.5) * 1000 * 10 */
/* variance = (int) (var_x*1000+0.5) + (int) (var_y*1000+0.5) * 1000 * 10 */
/* where sig_x/y and var_x/y are *positive* values in the range [0,10] cm */
/* with 3 digit precision for an overall possible resolution of 10 microns. */


/* C macros for packing and unpacking these values are in the file */
/* point_macros.h.  The package ppacker provides functions for packing */
/* and unpacking entire points and fat points rather than the individual */
/* values. */

   
/*-------------------- decoded point ------------------------------------*/
/* Note: only for use for writing out points for viewing in PAW, etc. */
/* This structure should not be used in most programs because of its size. */

typedef struct point_fat_t  { /* NA49 c.s. */
  Int_t    iflag;		/* bit flag */
  Int_t    id_det;		/* detector ID number */
  Int_t    n_sec;		/* sector number */
  Int_t    n_row;		/* row number */
  Int_t    abs_row;		/* absolute row number, 1-234 */
  Int_t    max_adc;		/* maximum ADC count in the cluster */
  Int_t    charge;		/* integrated/summed charge of the cluster */
  Int_t    n_pads;		/* number of pads in the cluster */
  Int_t    n_slices;		/* number of timeslices the cluster spans */
  Float_t  x;			/* positions in external coordinates */
  Float_t  y;
  Float_t  z;
  Float_t  sig_x;		/* uncertainty of the point position */
  Float_t  sig_y;
  Float_t  var_x;		/* cluster width */
  Float_t  var_y;
  Float_t  res_x;		/* distance from the track */
  Float_t  res_y;
  void     *irtrack_p;  /* ptr to the itrack/rtrack to which it belongs */
  struct point_t *next_tpoint_p; /* pointer to next point on rtrack */
} point_fat_t;



typedef struct mini_pnt_lst_t  {     /* list of mini points */
        Int_t   packed_pos_amp; /* packed position and amplitude */
} mini_pnt_lst_t;

typedef struct mini_pnt_hdr_t  {     /* header for mini pnt list */
        Int_t   id_det;         /* detector id */
        Int_t   npoints_vt1;    /* points on track in VT1 */
        Float_t z0_vt1;         /* app. midplane of det. */
        Float_t mean_amp_vt1;   /* mean amplitude of hits on track in VT1 */
        struct mini_pnt_lst_t *mini_pnt_vt1_p;  /* pointer to list of pnts. */
        Int_t   npoints_vt2;    /* points on track in VT2 */
        Float_t z0_vt2;         /* app. midplane of det. */
        Float_t mean_amp_vt2;   /* mean amplitude of hits on track in VT2 */
        struct mini_pnt_lst_t *mini_pnt_vt2_p;
        Int_t   npoints_mtp;    /* points on track in MTP */
        Float_t z0_mtp;         /* app. midplane of det. */
        Float_t mean_amp_mtp;   /* mean amplitude of hits on track in MTP */
        struct mini_pnt_lst_t *mini_pnt_mtp_p;
} mini_pnt_hdr_t;


/*------------------------- itrack ---------------------------------*/
typedef struct itrack_t  {   /* Internal c.s. */
   Int_t          iflag;          /* Raw track flag */   
   Int_t          id_det;         /* Detector id */
   Int_t          n_point;        /* Number of points */
   Int_t          n_max_point;    /* The maximum possible number of points */
   Int_t          n_fit_point;    /* Number of points used for the fit */
   Int_t          n_dedx_point;   /* number of dedx points */
   Float_t        slope_xz;       /* straight line slopes */
   Float_t        slope_yz;       /* yz */
   Float_t        x;              /* Point at reference surface (z=0) */
   Float_t        y;              /* y */
   Float_t        pchi2_xz;       /* P(chi^2,ndf) of fit */
   Float_t        pchi2_yz;       /* yz */
   Float_t        tmean_charge;   /* 'truncated mean' charge */
   Float_t        var_tmean;      /* Variance of 'trunc. mean' charge */
   Float_t        x_first;        /* First point on the track (measured) */
   Float_t        y_first;        /* y */
   Float_t        z_first;        /* z */
   Float_t        x_last;         /* Last point on the track (measured)*/
   Float_t        y_last;         /* y */
   Float_t        z_last;         /* z */
   struct covar4x4_t *covar_p;    /* Pointer to covariance matrix */       
   struct point_t    *point_p;    /* Pointer to first space point on itrack */
   struct rtrack_t   *rtrack_p;   /* Pointer to rtrack */
   struct itrack_t   *next_p;     /* Pointer to next detector itrack */
} itrack_t;


/*------------------------- rtrack ---------------------------------*/
typedef struct rtrack_t  {   /* NA49 c.s. */
   Int_t          iflag;          /* Raw track flag */   
   Int_t          id_det;         /* Detector id */
   Int_t          n_point;        /* Number of points */
   Int_t          n_max_point;    /* The maximum possible number of points */
   Int_t          n_fit_point;    /* Number of points used for the fit */
   Int_t          n_dedx_point;   /* number of dedx points */
   Float_t        charge;         /* Particle charge */
   Float_t        qpxz;           /* Momentum vector at fitted first point */
   Float_t        tanl;           /* tan(lam) */
   Float_t        phi;            /* azimuthal angle */
   Float_t        x;              /* x */
   Float_t        y;              /* y  fitted first point */
   Float_t        z;              /* z */
   Float_t        px;             /* Momentum vector at fitted first point */
   Float_t        py;             /*   */
   Float_t        pz;             /*   */
   Float_t        sig_px;         /* Errors of px, py, pz  */
   Float_t        sig_py;         /*   */
   Float_t        sig_pz;         /*   */
   Float_t        pchi2;          /* P(chi^2,ndf) of fit */
   Float_t        tmean_charge;   /* 'truncated mean' charge */
   Float_t        var_tmean;      /* Variance of 'trunc. mean' charge */
   Float_t        x_first;        /* First point on rtrack  (measured) */
   Float_t        y_first;        /* y */
   Float_t        z_first;        /* z */
   Float_t        x_last;         /* Last point on  rtrack (measured) */
   Float_t        y_last;         /* y */
   Float_t        z_last;         /* z */
   struct covar5x5_t *covar_p;      /* Pointer to covariance matrix */       
   struct point_t    *point_p;      /* Pointer to first space point on rtrack */
   struct track_t    *first_track_p; /* Ptr. to first trk gen. by rtrack */
   struct track_pid_t *track_pid_p;  /* Ptr. to pid structure (MC) */
   void              *itrack_p;     /* Pointer to itrack */
   struct rtrack_t   *next_p;       /* Pointer to next detector rtrack */
} rtrack_t;

/*---------------------------- vertex -----------------------------------*/
/*  Vertex flags:                                                        */
/*  iflag = 0   vertex is the one found and fitted by vtx                */
/*  iflag = 1   z is taken from geom_target_t, x,y from bpd_vertex       */
/*  iflag = 2   vertex is taken from HEPDB (geom_target_t)               */

/*---------------------------- vertex -----------------------------------*/
typedef struct vertex_t  { /* NA49 c.s. */
 Int_t          iflag;            /* vertex flag */
 Int_t          id_det;           /* Detector id */
 Int_t          id_vtx;           /* vertex type */
 Int_t          n_trk_fit;        /* Number of tracks used for fit */ 
 Int_t          n_trk_out;        /* number of outgoing tracks */
 Float_t        x;                /* coordinates of vertex */  
 Float_t        y;                /* y */
 Float_t        z;                /* z */
 Float_t        sig_x;            /* error of x */
 Float_t        sig_y;            /* error of y */
 Float_t        sig_z;            /* error of z */
 Float_t        pchi2;            /* P(chi^2,ndf) of vertex fit */
 struct track_t     *daughter_p;  /* ptr to first daughter trk */ 
 struct track_t     *mother_p;    /* ptr to mother trk */ 
 void               *covar_p;     /* ptr to error matrix of vertex */
 void               *avertex_p;   /* ptr to auxillary vertex struct */
 struct kfit_t      *kfit_p;      /* ptr to kinematic fit struct */
 struct vertex_t    *next_p;      /* ptr to next detector vertex */
} vertex_t;


/*--------------------------- track -------------------------------------*/
/*----------- track momentum at the vertex point ------------------------*/
typedef struct track_t  { /* NA49 c.s. */
 Int_t          iflag;            /* track flag  */
 Int_t          id_det;           /* detector id */
 Int_t          n_fit_point;      /* number of fitted points */
 Float_t        qpxz;             /* charge/pxz at the vertex */
 Float_t        tanl;             /* tangent of dip angle */
 Float_t        phi;              /* azimuthal angle */
 Float_t        sig_qpxz;         /* error on momentum */
 Float_t        sig_tanl;         /* */
 Float_t        sig_phi;          /* */
 Float_t        px;               /* momentum at vertex */
 Float_t        py;               /* */
 Float_t        pz;               /* */
 Float_t        sig_px;           /* error on momentum */
 Float_t        sig_py;           /* */
 Float_t        sig_pz;           /* */
 Float_t        trk_length;       /* trk length from vertex to z=400.0cm */
 Float_t        pchi2;            /* P(chi^2,ndf) of fit */
 Float_t        bx;               /* x_track-x_vertex at z_vertex */
 Float_t        by;               /* y_track-y_vertex at z_vertex */
 struct rtrack_t *rtrack_p;       /* ptr to rtrack */
 struct vertex_t *start_vertex_p; /* ptr back to vertex */
 struct vertex_t *stop_vertex_p;  /* ptr to stop vertex of this track */
 struct covar3x3_t *covar_p;      /* ptr to error matrix (only track elements*/
 struct atrack_t *atrack_p;       /* ptr to additional track structure */
 struct track_tof_t *track_tof_p; /* ptr to track_tof */
 struct track_t  *next_track_p;   /* ptr to next trk of the same rtrk */
 struct track_t  *next_daughter_p;  /* ptr to next daughter of the same vtx */
} track_t;


/*--------------------------- global tracks  -----------------------------*/

typedef struct g_predict_pnt_t  { /* predicted pseudo point */
        Float_t x;
        Float_t y;
        Float_t z;
        struct g_predict_pnt_t *next_p;    /* next point on this track */
} g_predict_pnt_t;

typedef struct g_ptr_list_t  { /* list of rtrack pointers */
        rtrack_t *rtrack_vt1_p;           /* pointer to VTP1 rtrack */
        rtrack_t *rtrack_vt2_p;           /* pointer to VTP2 rtrack */
        itrack_t *itrack_mtp_p;           /* pointer to MTPC itrack */
        rtrack_t *rtrack_mtp_p;           /* pointer to MTPC rtrack */
        rtrack_t *rtrack_glb_p;           /* pointer to global rtrack */
        struct g_predict_pnt_t *point_vt1_p; /* pointer to first point in vt1 */
        struct g_predict_pnt_t *point_vt2_p; /* pointer to first point in vt2 */
        struct g_predict_pnt_t *point_mtp_p; /* pointer to first point in mt  */
} g_ptr_list_t;


typedef struct rtrack_quali_t  { /* rtrack quality measures */
        Int_t   n_point[3];     /* number of points in subdets */
        Int_t   n_max_point[3]; /* maximum possible number of points */
        Int_t   n_unfold[3];    /* number of unfolded points */
        Int_t   n_dedx_point[3];/* number of dedx points */
        Float_t dist_next;      /* distance to closest track */
} rtrack_quali_t;

typedef struct g_track_ref_t  {    /* reference for global rtrack info */
      struct g_ptr_list_t   *g_ptr_list_p; /* global pointer list */
      struct mini_pnt_hdr_t *mini_pnt_p;   /* pointer to mini pointheader*/   
      struct rtrack_quali_t *quali_p;      /* pointer to quality structure */
      void   *local_p;                     /* pointer to local rtrack values */
} g_track_ref_t;

/*----------------------------- avertex_v0 -------------------------------*/
typedef struct avertex_v0_t  { /* Beam c.s. */
 Float_t        px;            /* momentum at vertex */  
 Float_t        py;            /* y */
 Float_t        pz;            /* z */
 Float_t        sig_px;        /* error on momentum */
 Float_t        sig_py;        /* y */
 Float_t        sig_pz;        /* z */
 Float_t        pt;            /* transverse momentum */
 Float_t        yla;           /* lambda rapidity */
 Float_t        yk0;           /* k-short rapidity */
 Float_t        alpha;         /* Armenteros alpha */
 Float_t        ptarm;         /* Armenteros pt */
 Float_t        mala;          /* mass lambda hypothesis */
 Float_t        malb;          /* mass anti-lambda hypothesis */
 Float_t        mak0;          /* mass k-short hypothesis */
 Float_t        xtarg;         /* v0 impact position at the target */
 Float_t        ytarg;         /* y */
 Float_t        ztarg;         /* z */
 Float_t        x1minx2;       /* daughter position x1 minus x2 at target */
} avertex_v0_t;


/*------------------------------- atrack -------------------------------*/
typedef struct atrack_t  { /* Beam c.s. */
   Int_t      iflag;          /* iflag       */
   Int_t      id_det;         /* Detector id */
   Int_t      id_part;        /*  Particle id (Geant codes) */
   Float_t    charge;         /* Charge      */
   Float_t    px;             /* px momentum at the vertex   */
   Float_t    py;             /* py momentum    */
   Float_t    pz;             /* pz (=pl) momentum    */
   Float_t    pt;             /* transverse momentum  */
   Float_t    psi;            /* azimuthal angle (x-y plane)  */
   Float_t    ypi;            /* rapidity pion mass */
   Float_t    ypr;            /* rapidity proton mass */
   Float_t    yka;            /* rapidity kaon mass */
} atrack_t;


/*----------------- Kinematical Fits ------------------------------*/

typedef struct kfit_t  { /* NA49 c.s. */
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
   Int_t               iflag;      /*  (0-unfitted,1-fitted,-1-error)*/
   Int_t               id_det;     /*  detector id                   */
   Int_t               id_type;    /*  fit type                      */
   Int_t               id_hyp;     /*  fit hypothesis                */
   Int_t               n_trk;      /*  number of tracks fitted       */    
   Float_t             chi2;       /*  fit chisq                     */ 
   Float_t             pchi2;      /*  fit probability               */ 
   struct ktrack_t    *ktrack_p;   /* Pointer to first fitted track  */
   struct vertex_t    *vertex_p;   /* Pointer to vertex              */
   struct kfit_t      *kfit_p;     /* Pointer to next kfit           */
} kfit_t;

typedef struct ktrack_t  { /* NA49 c.s. */
  Int_t      iflag;
  Float_t   charge;
  Float_t   qpxz;  
  Float_t   tanl;  
  Float_t   phi;
  struct covar3x3_t *covar_p;          /* Ptr to covar, only ktrack elements */ 
  struct ktrack_t   *next_vtx_ktrk_p;  /* Pointer to the vertex ktrack */
} ktrack_t;


/**************************** Beam *************************************/
typedef struct beam_t  {   /* BEAM Header: */
   Int_t      s1;               /* scalars - Station 1 */
   Int_t      s2;               
   Int_t      v0;               
   Int_t      s4;               
   Int_t      scl;              
   Int_t      bs4;              
   Int_t      bung;             /* Station 2 */
   Int_t      bgated;           
   Int_t      bi;               
   Int_t      trigger;          
   Int_t      empty[14];        /* 2 words of station 2, station 3 and 4 */
   Int_t      adc_s1;           /* ADC-Data  Station 7 */
   Int_t      adc_s2;           
   Int_t      adc_s3;           
   Int_t      adc_empty[45];    /* 9 wds of stat. 7, stat. 8,9, and 10 */
   Int_t      tdc_vtpc1;        /* TDC-Data  Station 11 */
   Int_t      tdc_vtpc2;        
   Int_t      tdc_mtpcl;        
   Int_t      tdc_mtpcr;        
   Int_t      tdc_empty[20];    /* 4 wds of stat. 11, stat. 12, and 13 */
   Int_t      pat_unit_1;       /* Pattern-Unit  Station 14 */
   Int_t      pat_unit_2;       /* Station 15 */
   Int_t      pat_unit_empty;   /* Station 16 */
   Float_t    version;          /* structure version number */
} beam_t;

/************ Beam counters additional information ********************/

typedef struct beam_aux_t { /* Additional beam counter iformation */
  Int_t         s3;                   /* s3 scaler         */
  Int_t         t0;                   /* t0 scaler         */
  Int_t         c1;                   /* c1 scaler         */
  Int_t         pulser_trigger;       /* scaler slot #6    */
  Int_t         adc_s4;               /* scaler slot #8    */
  Int_t         adc_t0;               /* scaler slot #8    */
  Int_t         adc_s1_new[12];       /* station #9  QS11,QS12,QS13,QS14  */
  Int_t         tpc_phase;            /* TPC clock - slot #11   */
  Int_t         padc[8];              /* scaler slot #13  Peak-sensitive ADC */
  Int_t         latch_scaler[32];     /* BPD stuff - slot # 17  */
  Int_t         station5[11];
  Int_t         station6[11];
  Int_t         station8[7];
  Int_t         station11[11];
  Int_t         empty1[24];           /*  24 empty reads (udefined) (73-96)  */
  Int_t         empty2[8];            /* empty laser phase  (113-120)        */
  Int_t         empty3[2];            /* empty pattern_unit (122-123) reads  */
} beam_aux_t;

/************************ Si Hodoscope *************************************/
typedef struct sihodo_data_t  {   
   Int_t      iflag;        /* quality flag (several clusters, missing ADC) */
   Int_t      adc[192];     /* ADC values from strip 1 to 192 */
   Float_t    mean;         /* Centroid of highest cluster in strip units */
   Float_t    rms;          /* RMS width in strip units */
   Int_t      maximum;      /* maximum ADC value of cluster */
   Int_t      charge;       /* sum of ADC counts in highest cluster */
   Int_t      sum_of_all;   /* sum of all 192 ADC counts */
   Int_t      detector;     /* H1=1, V1=2, H2=3, V2=4 */
} sihodo_data_t;


/*---------------------------------------------------------------------*/
typedef struct sihodo_t  {   
   Int_t      iflag;      /*quality flag */
   Float_t   x;           /* x offset in cm of beam in SIHODO system */
   Float_t   slope_xz;    /* xz slope of beam in SIHODO system */
   Float_t   y;           /* y offset in cm of beam in SIHODO system */
   Float_t   slope_yz;    /* yz slope of beam in SIHODO system */
   Float_t   covar[10];   /* covariance matrix */
   Float_t   z;           /* z of xy calc in internal system */
   Float_t   ext_x;       /* x offset in cm in survey system at z = T1 */
   Float_t   ext_slope_xz;    /* xz slope of beam in survey system */
   Float_t   ext_y;           /* y offset in cm in survey system at z = T1 */
   Float_t   ext_slope_yz;    /* yz slope of beam in survey system */
   Float_t   ext_z;           /* z of xy calc in survey system (T1 if known) */
   struct sihodo_data_t   *h1_p;        /* pointer to H1 data structure */
   struct sihodo_data_t   *v1_p;        /* pointer to V1 data structure */
   struct sihodo_data_t   *h2_p;        /* pointer to H2 data structure */
   struct sihodo_data_t   *v2_p;        /* pointer to V2 data structure */
} sihodo_t;


/************************ Beam Position Detectors *************************/
/*------structures-to-read-NA49-data-----------------------------------*/
typedef struct bpdNA49_data_t {   
   Int_t      iflag;        /* quality flag (several clusters, missing ADC) */
   Int_t      adc[16];      /* ADC values from strip 1 to 16 */
   Float_t    mean;         /* Centroid of highest cluster in NA49 coord. system */
   Float_t    rms;          /* RMS width of cluster in cm */
   Int_t      maximum;      /* maximum ADC value of cluster */
   Int_t      charge;       /* sum of (ADC - ped)*gain in highest cluster */
   Int_t      sum_of_all;   /* sum of (ADC - ped)*gain>0 over all 16 strips */
   Int_t      detector;     /* H1=1, V1=2, H2=3, V2=4, H3, V3, H4, V4=8 */
} bpdNA49_data_t;

typedef struct bpdNA49_t {   
   Int_t      iflag;      /* always  equal 0 */
   Float_t   x;           /* value that has no reasonable meaning */
   Float_t   slope_xz;    /* value that has no reasonable meaning */
   Float_t   y;           /* value that has no reasonable meaning */
   Float_t   slope_yz;    /* value that has no reasonable meaning */
   Float_t   covar[10];   /* covariance matrix */
   Float_t   z;           /* value that has no reasonable meaning */
   Float_t   ext_x;       /* x offset in NA49 system at z = T1 */
   Float_t   ext_slope_xz;    /* xz slope of beam in NA49 system */
   Float_t   ext_y;           /* y offset in NA49 system at z = T1 */
   Float_t   ext_slope_yz;    /* yz slope of beam in NA49 system */
   Float_t   ext_z;           /* z of x, y calc in NA49 system (T1 if known) */
   struct bpdNA49_data_t   *h1_p;        /* pointer to H1 data structure */
   struct bpdNA49_data_t   *v1_p;        /* pointer to V1 data structure */
   struct bpdNA49_data_t   *h2_p;        /* pointer to H2 data structure */
   struct bpdNA49_data_t   *v2_p;        /* pointer to V2 data structure */
   struct bpdNA49_data_t   *h3_p;        /* pointer to H3 data structure */
   struct bpdNA49_data_t   *v3_p;        /* pointer to V3 data structure */
   struct bpdNA49_data_t   *h4_p;        /* pointer to H4 data structure */
   struct bpdNA49_data_t   *v4_p;        /* pointer to V4 data structure */
} bpdNA49_t;

/*------current-structures---------------------------------------------*/
typedef struct bpd_data_t {   
   Int_t      iflag;        /* quality flag (several clusters, missing ADC) */
   Int_t      adc[32];      /* ADC values from strip 1 to 32 (for 16 strip BPDs only first 16 elements have any meaning) */
   Float_t    mean;         /* Centroid of highest cluster in NA61 coord. system */
   Float_t    rms;          /* RMS width of cluster in cm */
   Int_t      maximum;      /* maximum ADC value of cluster */
   Int_t      charge;       /* sum of (ADC - ped)*gain in highest cluster */
   Int_t      sum_of_all;   /* sum of (ADC - ped)*gain>0 over all 16 strips */
   Int_t      detector;     /* H1=1, V1=2, H2=3, V2=4, H3, V3, H4, V4=8 */
} bpd_data_t;

typedef struct bpd_t {   
   Int_t      iflag;      /* quality flag same as bpd_vertex_t.iflag */
   Float_t   x;           /* always  equal 0 */
   Float_t   slope_xz;    /* always  equal 0 */
   Float_t   y;           /* always  equal 0 */
   Float_t   slope_yz;    /* always  equal 0 */
   Float_t   covar[10];   /* covariance matrix */
   Float_t   z;           /* always  equal 0 */
   Float_t   ext_x;       /* x offset in NA49 system at z = T1 */
   Float_t   ext_slope_xz;    /* xz slope of beam in NA49 system */
   Float_t   ext_y;           /* y offset in NA49 system at z = T1 */
   Float_t   ext_slope_yz;    /* yz slope of beam in NA49 system */
   Float_t   ext_z;           /* z of x, y calc in NA49 system (T1 if known) */
   struct bpd_data_t   *h1_p;        /* pointer to H1 data structure */
   struct bpd_data_t   *v1_p;        /* pointer to V1 data structure */
   struct bpd_data_t   *h2_p;        /* pointer to H2 data structure */
   struct bpd_data_t   *v2_p;        /* pointer to V2 data structure */
   struct bpd_data_t   *h3_p;        /* pointer to H3 data structure */
   struct bpd_data_t   *v3_p;        /* pointer to V3 data structure */
   struct bpd_data_t   *h4_p;        /* pointer to H4 data structure */
   struct bpd_data_t   *v4_p;        /* pointer to V4 data structure */
} bpd_t;

typedef struct bpd_vertex_t {
   Int_t     iflag;       /*quality flag */
   Float_t   x;           /* x vertex of target T1 */
   Float_t   y;           /* y vertex of target T1 */
   Float_t   z;           /* z vertex of target T1 */
   Float_t   sx;           /* sigma x */
   Float_t   sy;           /* sigma y */
   Float_t   sz;           /* sigma z */
} bpd_vertex_t;

/******************** Central Multiplicity Detector  *********************/
typedef struct cmd_data_t {
   Int_t wire[8];
} cmd_data_t;

typedef struct cmd_t {
   Int_t n_wires;      /* Number of wires with signal */
   Int_t n_clusters;   /* Number of clusters */
   Int_t adc[12];      /* Measured total ADC value */
   cmd_data_t *data_p;
} cmd_t;

typedef struct cmd_hit_t {   /* Centrality Det. Hit */
   Int_t      n_pads;             /* number of hitted pads */
   Float_t    x;                  /* x coordinate of hit */
   Float_t    y;                  /* y coordinate of hit */
   Float_t    z;                  /* z coordinate of hit */
   struct cmd_hit_t  *next_p;     /* Pointer to next CMD point */
} cmd_hit_t;

/*************************** Ring Calorimeter ****************************/
typedef struct ring_t  {   /* RING CALO Header: */
   Int_t      adc_photon[240];   /* Stations 1-20 of Crate 2 */
   Int_t      adc_hadron[240];   /* Stations 1-20 of Crate 3 */
   Float_t    version;           /* structure version number */
} ring_t;

typedef struct ring_cal_t {   /* Calibrated RING CALO */
   Float_t      adc_photon[240];   /* Stations 1-20 of Crate 2 */
   Float_t      adc_hadron[240];   /* Stations 1-20 of Crate 3 */
   Float_t      energy;            /* Total energy             */
} ring_cal_t;

/****************************** Veto Chambers  **************************/
typedef struct vpc_t {
   struct     vpc_data_t *l1_p;         /* pointer to L1 data structure */
   struct     vpc_data_t *r1_p;         /* pointer to R1 data structure */
   struct     vpc_data_t *l2_p;         /* pointer to L2 data structure */
   struct     vpc_data_t *r2_p;         /* pointer to R2 data structure */
   struct     vpc_point_t *vpcpts_p;    /* pointer to VPC points        */
   Int_t      points_vpc1;              /* nr. of points in VPC1        */
   Int_t      points_vpc2;              /* nr. of points in VPC2        */
   struct     vpc_track_t *vpctrk_p;    /* pointer to VPC tracks        */
} vpc_t;

typedef struct vpc_data_t {        /**   wc_p   **/
   Int_t      iflag;
   Int_t      adc[80];
   Int_t      sum_of_all;  /* tot_charge ( corr*(adc-ped) > 0 ) in plane  */
   Int_t      detector;    /* detector plane: L1=1, R1=2, L2=3, R2=4 */
   Int_t      nclust;      /* nr. of clusters in plane */
} vpc_data_t;

typedef struct vpc_clust_t {          /**   cl_p   **/
   Int_t      iflag;
   Int_t      det_plane;
   Float_t    mean;          /* cluster centroid in strip units */
   Float_t    rms;
   Float_t    maxadc;        /* adcvalue in the maximum  */
   Float_t    charge;        /* sum_charge of cluster    */
   Int_t      width;         /* width of cluster in strip units */
} vpc_clust_t;

typedef struct vpc_point_t {       /**   pt_p   **/
   Int_t      iflag;
   Int_t      vpc_station;   /* =1, =2, 1st, 2nd VETOMWC  */
   Float_t    x;             /* point coord. in NA49 standard sys. */
   Float_t    y;
   Float_t    z;
   Float_t    rel_q;         /* charge ratio of clust_plan1/clust_plan2 */
} vpc_point_t;

typedef struct vpc_track_t {       /**   tr_p   **/
   Int_t      iflag;
   Int_t      combi ;        /*  point_nr(VPC1)*100 + point_nr(VPC2)  */
   Float_t    x ;
   Float_t    slope_xz ;
   Float_t    y ;
   Float_t    slope_yz ;
   Float_t    z ;
   Float_t    y_target ;     /*  extrapolation to target T1 */
   Float_t    x_calo ;       /*  extrapolation to calorimeter */
   Float_t    y_calo ;
   Float_t    z_calo ;
   Int_t      cell ;         /*  corresponding calor. cell-nr. */
} vpc_track_t ;

/****************************** Veto Calorimeter **************************/
typedef struct veto_t  {   /* VETO CALO Header */
   Int_t      adc_photon[4];          /* Stations 21 */
   Int_t      adc_hadron[4];
   Int_t      adc_empty[4];
   Int_t      atten_photon[4];        /* Stations 22 */
   Int_t      atten_hadron[4];
   Int_t      atten_empty[4];
   Float_t    ped_photon[4];          /* Photon pedestals used */
   Float_t    ped_hadron[4];          /* Hadron pedestals used */
   Float_t    gev_veto;               /* Calibration constant used */
   Float_t    eveto;                  /* Veto energy - calibrated */
   Float_t    version;                /* structure version number */
} veto_t;

/***************************  PEDESTALS  **********************************/
typedef struct pedestal_t {    /* CAMAC pedestals */
  Float_t value;               /* pedestal value  */
  Float_t sigma;               /* pedestal sigma  */
} pedestal_t;

typedef struct rel_gains_t {   /* BPD relative gain factors */
  Float_t value;               /* gain factors value  */
  Float_t sigma;               /* gain factors sigma  */
} rel_gains_t;

/*****************************   TOF    ***********************************/

/*----------------------------  bos --------------------------------------*/
typedef struct bos_tof_t  {   /* TOF raw data */
   Int_t    data[2000];   /* TOF data packed in BOS format */
} bos_tof_t;


/*-----------------------------------------------------------------------*/
typedef struct bos_gtof_t  {   /* Grid TOF raw data */
   Int_t    data[800];    /* Grid TOF data packed in BOS format */
} bos_gtof_t;

/* left and right together */

/*------------------------ detector header ----------------------------*/

typedef struct tof_t  { /*   TOF Header   */ 
  Int_t iflag;			/* 1 if can use the tof inf.    */
  Int_t n_mtpc;			/* # of tracks hited the wall   */
  Int_t n_mtpc_match;           /* # of tracks matched whit tof */
  Int_t n_actdet;		/* # of active detectors        */
  Int_t n_match;		/* # of used tof hits	        */
  struct track_tof_t *track_tof_p; /* ptr to the first track_tof*/	 
  struct tof_hits_t  *hits_tof_p; /* ptr to the first hit_tof */
} tof_t;


/*----------------------------- tof hits ------------------------------*/

typedef struct tof_hits_t  { /*   TOF    */ 
  Int_t iflag;                  /* 1 if can use the tof information */
  Int_t id_det;                 /* Detector id.			            */
  Int_t id_pm;                  /* PM number */
  Int_t n_tracks;               /* # of candidate tracks	        */
  Float_t TDC;                  /* tof measured by the PM in ns.    */ 
  Float_t QDC;                  /* Normalized amplitude             */
  struct track_t *track_p1;     /* Pointer to 1. track candidate    */
  struct track_t *track_p2;	    /* Pointer to 2. track candidate    */
  struct track_t *track_p3;	    /* Pointer to 3. track candidate    */   
} tof_hits_t;


/*----------------------------- track --------------------------------*/

typedef struct track_tof_t  {  /* final PID from TOFR */
  Int_t      iflag;                /* iflag 0,1 TOFL,R; TOF-F: iflag=2 if track
extrapolated from VTPC2 iflag=3 if extrapolated from MTPC*/
  Int_t      id_det;               /* detector id number 5:TOFL 6:TOFR 7:TOF-F*/
  Int_t      id_pm1;                /* ToF-F up PM number of the hit*/
  Int_t      id_pm2;                /* ToF-F down PM number of the hit*/
  Int_t      id_scint;                /* ToF-F Scintillator number of the hit*/
  Float_t    x_global;                    /* hit position in na61 coord. */
  Float_t    y_global;                    /*    */
  Float_t    x;                    /* hit position in det coord. */
  Float_t    y;                    /*                            */
  Int_t      tdc1;                /* ToF-F raw uncalibrated tdc value of up PM*/
  Int_t      tdc2;                /* ToF-F raw uncalibrated tdc value of down PM*/
  Int_t      qdc1;                /* ToF-F adc value of up PM in ps*/
  Int_t      qdc2;                /* ToF-F adc value of down PM in ps*/
  Float_t    pathl;                /* tracklength from target to det */
  Float_t    charge;               /* normalized QDC signal */
  Float_t    tof;                  /* time of flight from target to det */
  Float_t    mass2;                /* squared mass */
  Float_t    sig_mass2;            /* sigma of mass2 */
  struct track_t *track_p;         /* ptr to track_t */
  struct tof_hits_t *hits_tof_p;   /* ptr to the hits if it is a single det. */
  struct track_tof_t *next_tof_p;  /* next tof-track associated w/ same track */
  struct track_tof_t *next_p;      /* next  detector track_tof */
} track_tof_t;

/* ----------------- PesTOF stuctures --------------*/

typedef struct pestof_data_t {
   Int_t data;
} pestof_data_t;

typedef struct bos_pestof_t {
   Int_t event_id;              /* 1. word in raw bos bank      */
   Int_t event_date;            /* 2. word in raw bos bank      */
   Int_t event_num;             /* 3. word in raw bos bank      */
   pestof_data_t *data_slow_p;  /* 4-10. words in raw bos bank  */
   pestof_data_t *data_event_p; /* 5-ende                       */
} bos_pestof_t;

/**************************** Monitor *************************************/

typedef struct monitor_tpc_t  {
  Int_t   n_run;          /* run number */
  Int_t   n_event;        /* event number */
  Int_t   date;           /* date from event structure */
  Int_t   time;           /* time from event structure */
  Int_t   n_point;        /* number of points */
  Int_t   n_itrack;       /* number of itracks */
  Int_t   n_rtrack;       /* number of rtracks */
  Int_t   n_rtrack_kpass; /* number of rtracks found in kink finder passes */
  Int_t   n_track;        /* number of tracks */
  Int_t   n_vertex;       /* number of vertices found */
  Int_t   sum_adc;        /* sum of all rawdata ADC counts */
  Float_t tmean_charge;   /* mean of the truncated mean charge */
  Float_t avg_res_x;      /* mean abs x residuum of all i/rtracks */
  Float_t avg_res_y;      /* mean abs y residuum of all i/rtracks */
  Float_t cur_vdrift;     /* vdrift for this event from charge step */
  Float_t ave_vdrift;     /* the running average vdrift from charge step */
  Float_t raw_vdrift;     /* vdrift from raw data */
  Float_t mon_vdrift;     /* processed time dependent drivt velocity (S') */
  Int_t   iflag_vd_mod;   /* the success flag of the vdrift module */
  Float_t gas_gain;       /* gain factor from raw data */
  Float_t evt_gain;       /* eventwise gain factor */
  Float_t temperature;    /* temperature from raw data */
  Float_t pressure;       /* pressure from raw data */

} monitor_tpc_t;


typedef struct monitor_tof_t  {
  Int_t   n_run;          /* run number */
  Int_t   n_event;        /* event number */
  Int_t   date;           /* date from event structure */
  Int_t   time;           /* time from event structure */
  Int_t   n_mtpc;         /* # of tracks hited the wall   */
  Int_t   n_mtpc_match;   /* # of tracks matched whit tof */
  Int_t   n_actdet;       /* # of active detectors        */
  Int_t   n_match;        /* # of used tof hits           */
  Float_t tmean;          /* average corrected TDC signal   */
  Float_t sig_tmean;      /* rms of tmean   */
  Float_t qmean;          /* average corrected QDC signal   */
  Float_t sig_qmean;      /* rms of qmean   */
} monitor_tof_t;

/*************  WFA **********/ 
typedef struct wfa_t {
  Int_t n_beam;     /*  nr. of beam particles from WFA chan 1 */
  Int_t n_inter;    /*  nr. of interactions from WFA chan 2 */
} wfa_t ;
 
typedef struct wfa_time_t {
  Int_t wfa_time ;  /*  given in nsec  */
} wfa_time_t ;

/* cluster consts */
typedef struct  clust_const_t 
{
  int     ntry;
  int     npmin;
  int     idx;   
  int     idy;   
  float   mindist;
} clust_const_t;

/* morphology consts */

typedef struct  morph_const_t 
{
  int     min_npix;
  int     max_npix;
  float   mean_exctr;
  float   rad_exctr;
  float   max_qmax;
  float   min_qpix;
  float   max_qpix;
  float   min_qtot;
} morph_const_t;


typedef struct  const_sec_t 
{
  float lat_const;
  float base_const[64];
} const_sec_t;

typedef struct  const_head_t 
{
  const_sec_t *const_vt1_p[2][18];
  const_sec_t *const_vt2_p[2][18];
  const_sec_t *const_mtl_p[5][30];
  const_sec_t *const_mtr_p[5][30];
} const_head_t;

#define N_DET     5    /* Verschiebung der Matrizen wegen Det.-nummerierung */
#define N_SEC    26    /* -"- */
#define N_V_SEC   7    /* -"- */
#define MAX_SEC  62
#define N_ROW    25
#define N_PAD   193
#define VTPC_N_SECT_ROW  2
#define VTPC_N_GROUP     18
#define MTPC_N_SECT_ROW  5
#define MTPC_N_GROUP     30
#define MAX_NP_VTPC   72  /* maximale Punktzahl in VTPC */
#define MAX_NP_MTPC   90  /* maximale Punktzahl in MTPC */
#define MAX_NP_GLOBAL 234 /* maximale Gesamtpunktzahl */
#define MAX_CLS  8000
#define MAX_TRM  1000
#define N_PAR    3
#define MAX_PAR  50

typedef struct dedx_charge_t  {
  int    n_point[N_SEC];
  double q_point[N_SEC][MAX_CLS];
} dedx_charge_t;

typedef struct dedx_truncm_t  {
  int    n_truncm[N_SEC];
  double q_truncm[N_SEC][MAX_TRM];
} dedx_truncm_t;


typedef struct dedx_cal_t  {
  double      loss_per_m[N_PAR];   /* charge loss in percent per meter */
  double      press_par[N_PAR];    /* pressure correction parameters */
  double      drift_fact[MTPC_N_SECT_ROW][N_PAR]; /* drift loss corr. par. */
  double      multi_fact[MTPC_N_SECT_ROW];        /* multipl. loss. corr. */
  double      time_fact[N_SEC];    /* time dependence calibration constants */
  double      time_const[N_SEC];   /* time dependence calibration values */
  double      time_ref[N_SEC];     /* time dependence reference values */
  double      sector_const[N_SEC]; /* intersector calibration constants */
  double      multi_const[N_SEC];  /* factors for multiplicity corr. */
  double      det_norm;            /* normalisation to minimum ionising */
} dedx_cal_t;

typedef struct dedx_time_cal_t  {
  double      loss_per_m[N_PAR]; /* see above */
  double      press_par[N_PAR];
  double      drift_fact[MTPC_N_SECT_ROW][N_PAR];
  double      multi_fact[MTPC_N_SECT_ROW];
  double      time_fact[N_SEC];
  double      time_const[N_SEC];
  double      time_ref[N_SEC];
  double      sector_const[N_SEC];
  double      multi_const[N_SEC];
  double      det_norm;
} dedx_time_cal_t;

/**************************************************************************/

#endif  /* _na49_event_str_included_ */


