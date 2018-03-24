#ifndef _xifind_str_included
#define _xifind_str_included__

typedef struct xi_cuts_t 
{
  Int_t nhits;         /* min no. points on -ve rtrack */
  Int_t npots;         /* min no. potential points on charged track */
  Int_t n_xi_steps;    /* number of steps used to track xiom */
  Int_t sside;         /* 1 : xi and v0 x same side of TPC, -1 : diff, 0 : any */
  Float_t ptarm_v_max; /* maximum ptarm for v0 */
  Float_t step_size;   /* step size along z for v0 */
  Float_t mala_min;    /* minimum mass of la / lb from v0 */
  Float_t mala_max;    /* maximum mass of la / lb from v0 */
  Float_t zmin;        /* min z for which xi search done */
  Float_t dcax;        /* max distance of closest appraoch in x */
  Float_t dcay;        /* max distance of closest approach in y */
  Float_t dcar;        /* max radial distance of closest approach */
  Float_t ztarg;       /* z of target */
  Float_t ptarm_max;   /* maximum allowed ptarm */
  Float_t xtv0_min;    /* min and max x of v0 at target plane.... */
  Float_t xtv0_max;    /* .... v0 must point back within these */
  Float_t ytv0;        /* maximum allowed abs y of v0 at target plane */
  Float_t xtxi;        /* maximum abs x tolerance at target for xi */
  Float_t ytxi;        /* maximum abs y tolerance at target for xi */
  Float_t xtarg_pi_lo; /* CAREFUL!!  xtarg_pi must be OUTSIDE range.... */
  Float_t xtarg_pi_hi; /* .... defined by xtpi_lo and xtpi_hi values */
  Float_t ytarg_pi;    /* target y of daughter pi / ka must be outside |ytpi| */
  Float_t zmin_v;      /* min z of v0 vertex */
  Float_t pt_max;      /* maximum pt allowed */
  Float_t ptot_max;    /* maximum ptot allowed */
} xi_cuts_t;

typedef struct avertex_xi_t          /* first draft design of avertex_xi */
{
  Int_t charge;                      /* particle charge */
  Float_t px;                        /* x momentum at vertex */
  Float_t py;                        /* y */
  Float_t pz;                        /* z */
  Float_t sig_px;                    /* error on x momentum */
  Float_t sig_py;                    /* y */
  Float_t sig_pz;                    /* z */
  Float_t pt;                        /* transverse momentum */
  Float_t yxi;                       /* cascade rapidity */
  Float_t yom;                       /* omega rapidity */
  Float_t alpha;                     /* Armenteros alpha */
  Float_t ptarm;                     /* Armenteros pt */
  Float_t maxi;                      /* mass cascade hypothesis */
  Float_t maom;                      /* mass omega hypothesis */
  Float_t xtarg;                     /* x impact position at the target */
  Float_t ytarg;                     /* y */
  Float_t ztarg;                     /* z */
  Float_t xtarg_pi;                  /* x of charged daughter at target */
  Float_t ytarg_pi;                  /* y */
  Float_t plen;                      /* path length of cascade */
  Float_t x1minx2;                   /* x of daughter 1 - x of 2 at target */
}  avertex_xi_t;
 
typedef struct la_in_t               /* structure for storing data on lambda's */
{
  Float_t x;                         /* x position of decay vertex */
  Float_t y;                         /* y     "    "    "     "    */
  Float_t z;                         /* z     "    "    "     "    */
  Float_t px;                        /* x component of momentum */
  Float_t py;                        /* y     "     "     "     */
  Float_t pz;                        /* z     "     "     "     */
  Float_t dcar_l;                    /* previous radial la / pi track separation */
  Float_t dcar_2;                    /* 2nd last   "       "      "       "      */
  struct rtrack_t *la_pi_rtrk_p;     /* pointer to la daughter pi- rtrack */ 
  struct rtrack_t *lb_pi_rtrk_p;     /* pointer to lb daughter pi+ rtrack */
  struct avertex_v0_t *avtx_v0_p;    /* pointer to avertex_v0 structure */
  struct vertex_t *vertex_p;         /* pointer to vertex structure */
  Int_t flag;                        /* flag = 1 after xi / om found */
  Int_t part;                        /* particle : 1 lambda, -1 antilambda */
}  la_in_t;

#endif





