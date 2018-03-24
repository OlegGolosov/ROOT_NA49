#ifndef _T49CutV0_INCLUDED_
#define _T49CutV0_INCLUDED_

/*
$Log: T49CutV0.h,v $
Revision 1.14  2005/04/26 18:58:12  cblume
SLC3 and rootv4 compatible version

Revision 1.13  2002/03/27 14:05:06  cblume
Add same side cut and cut on vertex X and Y

Revision 1.12  2001/12/12 17:40:35  cblume
Add option to cos(theta) cut

Revision 1.11  2001/11/26 12:21:26  cblume
Change to TObjArray for Vertex2Particle()

Revision 1.10  2001/11/12 12:05:24  cblume
Update for ROOT v3.01

 * Revision 1.9  2000/11/09  08:56:49  cblume
 * Removed header files. Single track cuts are now done via T49CutTrack
 *
 * Revision 1.8  2000/10/09  10:24:53  cblume
 * General update
 *
 * Revision 1.7  2000/06/05  12:59:21  cblume
 * Changed particle list to TClonesArray. Update of DrawCutList()
 *
 * Revision 1.6  2000/05/19  15:39:11  cblume
 * Added new functionality
 *
 * Revision 1.5  2000/04/18  17:27:52  cblume
 * Adapted to changes in T49VertexRoot. No need for SetDaughter() any more
 *
 * Revision 1.4  2000/04/13  15:13:50  cblume
 * New function to select Bham or GSI V0s
 *
 * Revision 1.3  1999/11/23  13:52:13  cblume
 * Remove files
 *
*/

#include "T49Cut.h"

const Int_t   kTypes           = 5;
const Int_t   kLambda          = 1;
const Int_t   kAntiLambda      = 2;
const Int_t   kK0s             = 3;
const Int_t   kXi              = 4;
const Int_t   kOmega           = 5;

const Float_t kLambdaLifetime  = 2.632;    // Lifetime (s*10^10)
const Float_t kK0sLifetime     = 0.8934;
const Float_t kXiLifetime      = 1.639;
const Float_t kOmegaLifetime   = 0.821;

const Float_t kLambdaWidth     = 0.0100;   // Width used for the extraction of the
const Float_t kAntiLambdaWidth = 0.0100;   // peak contents
const Float_t kK0sWidth        = 0.0100;
const Float_t kXiWidth         = 0.0100;
const Float_t kOmegaWidth      = 0.0100;

const Int_t   kIdVtxV0         = 11;       // Normal V0
const Int_t   kIdVtxXi         = 41;       // Xi or Omega candidates
const Int_t   kIdVtxXiDaughter = 15;       // Xi or Omega daughter V0

class TPaveText;
class TObjArray;

class T49VertexRoot;

class T49CutTrack;

class T49CutV0 : public T49Cut {

 public:

  T49CutV0();   
  T49CutV0(const Text_t *name, const Text_t *title);
  virtual ~T49CutV0();

  virtual void            Copy(TObject &c) const;  
  virtual TPaveText      *CreateCutList();
  virtual void            DrawCutList();
  virtual void            PrintCutList();
  virtual void            PrintStatistics();
  virtual void            ResetStatistics();
  virtual void            Reset();

  virtual Bool_t          CheckV0(T49VertexRoot *V0);
  virtual TObjArray      *Vertex2Particle(TObjArray *V0List);

  // Setters

          void            SetFast()                            { fFast               = kTRUE;  };
          void            SetSlow()                            { fFast               = kFALSE; };
          void            SetStandardCutsBham();
          void            SetStandardCutsGSI();
          void            SetGSI()                             { fCutGSIBham         = kTRUE;
                                                                 fValGSIBham         = 0x4000; };
          void            SetBham()                            { fCutGSIBham         = kTRUE;
                                                                 fValGSIBham         = 0x8000; };
          void            SetXi()                              { fIdVtx              = kIdVtxXi;         };
          void            SetXiDaughter()                      { fIdVtx              = kIdVtxXiDaughter; };
          void            SetX1minX2(Float_t x1minx2)          { fCutX1minX2         = kTRUE;
                                                                 fMinX1minX2         = x1minx2; };
          void            SetY1minY2(Float_t y1miny2)          { fCutY1minY2         = kTRUE;
                                                                 fMinY1minY2         = y1miny2; };
          void            SetXTarg(Float_t xtarg)              { fCutXTarg           = kTRUE;
                                                                 fMaxXTarg           = xtarg; };
          void            SetYTarg(Float_t ytarg)              { fCutYTarg           = kTRUE;
                                                                 fMaxYTarg           = ytarg; };
          void            SetXV0(Float_t xv0)                  { fCutXV0             = kTRUE;
                                                                 fMinXV0             = xv0; };
          void            SetYV0(Float_t yv0)                  { fCutYV0             = kTRUE;
                                                                 fMinYV0             = yv0; }; 
          void            SetZV0(Float_t zv0)                  { fCutZV0             = kTRUE;
                                                                 fMinZV0             = zv0; };
          void            SetArmenterosPt(Float_t ptmin, Float_t ptmax) 
                                                               { fCutArmenterosPt    = kTRUE;
                                                                 fMinArmenterosPt    = ptmin; 
                                                                 fMaxArmenterosPt    = ptmax; };
          void            SetArmenterosAlpha(Float_t almin, Float_t almax)  
                                                               { fCutArmenterosAlpha = kTRUE;
                                                                 fMinArmenterosAlpha = almin;
                                                                 fMaxArmenterosAlpha = almax; };
          void            SetPchi2(Float_t pchi2)              { fCutPchi2           = kTRUE;
                                                                 fMinPchi2           = pchi2; };
          void            SetIflag(Int_t iflag = 0)            { fCutIflag           = kTRUE;
                                                                 fValIflag           = iflag; }; 
          void            SetCosThetaLambda(Float_t ctmin, Float_t ctmax, Bool_t minv = kFALSE)
                                                               { fCutCosTheta        = kTRUE;
                                                                 fMinCosTheta        = ctmin;
                                                                 fMaxCosTheta        = ctmax; 
                                                                 fV0TypeCosTheta     = kLambda;
                                                                 fUseInvMassCosTheta = minv; };
          void            SetCosThetaAntiLambda(Float_t ctmin, Float_t ctmax, Bool_t minv = kFALSE)
                                                               { fCutCosTheta        = kTRUE;
                                                                 fMinCosTheta        = ctmin;
                                                                 fMaxCosTheta        = ctmax;
                                                                 fV0TypeCosTheta     = kAntiLambda;
                                                                 fUseInvMassCosTheta = minv; };
          void            SetCosThetaK0s(Float_t ctmin, Float_t ctmax, Bool_t minv = kFALSE)
                                                               { fCutCosTheta        = kTRUE;
                                                                 fMinCosTheta        = ctmin;
                                                                 fMaxCosTheta        = ctmax;
                                                                 fV0TypeCosTheta     = kK0s;
                                                                 fUseInvMassCosTheta = minv; };
          void            SetCosThetaXi(Float_t ctmin, Float_t ctmax, Bool_t minv = kFALSE)
                                                               { fCutCosTheta        = kTRUE;
                                                                 fMinCosTheta        = ctmin;
                                                                 fMaxCosTheta        = ctmax;
                                                                 fV0TypeCosTheta     = kXi; 
                                                                 fUseInvMassCosTheta = minv; };
          void            SetCosThetaOmega(Float_t ctmin, Float_t ctmax, Bool_t minv = kFALSE)
                                                               { fCutCosTheta        = kTRUE;
                                                                 fMinCosTheta        = ctmin;
                                                                 fMaxCosTheta        = ctmax;
                                                                 fV0TypeCosTheta     = kOmega;
                                                                 fUseInvMassCosTheta = minv;};
          void            SetLifetimeLambda(Float_t lmin, Float_t lmax)
                                                               { fCutLifetime        = kTRUE;
                                                                 fMinLifetime        = lmin;
                                                                 fMaxLifetime        = lmax;
                                                                 fV0TypeLifetime     = kLambda; };
          void            SetLifetimeAntiLambda(Float_t lmin, Float_t lmax)
                                                               { fCutLifetime        = kTRUE;
                                                                 fMinLifetime        = lmin;
                                                                 fMaxLifetime        = lmax;
                                                                 fV0TypeLifetime     = kAntiLambda; };
          void            SetLifetimeK0s(Float_t lmin, Float_t lmax)
                                                               { fCutLifetime        = kTRUE;
                                                                 fMinLifetime        = lmin;
                                                                 fMaxLifetime        = lmax;
                                                                 fV0TypeLifetime     = kK0s; };
          void            SetLifetimeXi(Float_t lmin, Float_t lmax)
                                                               { fCutLifetime        = kTRUE;
                                                                 fMinLifetime        = lmin;
                                                                 fMaxLifetime        = lmax;
                                                                 fV0TypeLifetime     = kXi; };
          void            SetLifetimeOmega(Float_t lmin, Float_t lmax)
                                                               { fCutLifetime        = kTRUE;
                                                                 fMinLifetime        = lmin;
                                                                 fMaxLifetime        = lmax;
                                                                 fV0TypeLifetime     = kOmega; };
          void            SetMassLambda(Float_t window = kLambdaWidth);
          void            SetMassAntiLambda(Float_t window = kAntiLambdaWidth);
          void            SetMassK0s(Float_t window = kK0sWidth);
          void            SetMassXi(Float_t window = kXiWidth);
          void            SetMassOmega(Float_t window = kOmegaWidth);
          void            SetMass(Int_t type, Float_t mass, Float_t window);
          void            SetExcludedMassLambda(Float_t window = kLambdaWidth);
          void            SetExcludedMassAntiLambda(Float_t window = kAntiLambdaWidth);
          void            SetExcludedMassK0s(Float_t window = kK0sWidth);
          void            SetExcludedMassXi(Float_t window = kXiWidth);
          void            SetExcludedMassOmega(Float_t window = kOmegaWidth);
          void            SetExcludedMass(Int_t type, Float_t mass, Float_t window);
          void            SetCrossYZ(Float_t zmin)             { fCutCrossYZ         = kTRUE;
                                                                 fMinCrossYZ         = zmin; };
          void            SetPt(Float_t ptmin)                 { fCutPt              = kTRUE;
                                                                 fMinPt              = ptmin; };
          void            SetRapidity(Int_t type, Float_t ymin, Float_t ymax)
                                                               { fCutRapidity        = kTRUE;
                                                                 fMinRapidity        = ymin;
                                                                 fMaxRapidity        = ymax;
                                                                 fV0TypeRapidity     = type; };
          void            SetRapidityLambda(Float_t ymin, Float_t ymax)
	                                                       { SetRapidity(kLambda,ymin,ymax);     };
          void            SetRapidityAntiLambda(Float_t ymin, Float_t ymax)
	                                                       { SetRapidity(kAntiLambda,ymin,ymax); };
          void            SetRapidityK0s(Float_t ymin, Float_t ymax)
	                                                       { SetRapidity(kK0s,ymin,ymax);        };
          void            SetRapidityXi(Float_t ymin, Float_t ymax)
	                                                       { SetRapidity(kXi,ymin,ymax);         };
          void            SetRapidityOmega(Float_t ymin, Float_t ymax)
	                                                       { SetRapidity(kOmega,ymin,ymax);      };
          void            SetOutsideTPC()                      { fCutOutsideTPC      = kTRUE; };
          void            SetPsi(Float_t psimin)               { fCutPsi             = kTRUE;
	                                                         fMinPsi             = psimin; };
          void            SetPosTrackCut(T49CutTrack *cut)     { fCutPosTrack        = kTRUE;
	                                                         fPosTrackCuts       = cut; };
          void            SetNegTrackCut(T49CutTrack *cut)     { fCutNegTrack        = kTRUE;
	                                                         fNegTrackCuts       = cut; };
          void            SetSameSide()                        { fCutSameSide        = kTRUE; }; 

  // Getters

          Float_t         GetX1minX2()                         { return fMinX1minX2; };
          Float_t         GetY1minY2()                         { return fMinY1minY2; };
          Float_t         GetXTarg()                           { return fMaxXTarg; };
          Float_t         GetYTarg()                           { return fMaxYTarg; };
          Float_t         GetXV0()                             { return fMinXV0; };
          Float_t         GetYV0()                             { return fMinYV0; }; 
          Float_t         GetZV0()                             { return fMinZV0; };
          Float_t         GetArmenterosPtMin()                 { return fMinArmenterosPt; };
          Float_t         GetArmenterosPtMax()                 { return fMaxArmenterosPt; };
          Float_t         GetArmenterosAlphaMin()              { return fMinArmenterosAlpha; };
          Float_t         GetArmenterosAlphaMax()              { return fMaxArmenterosAlpha; };
          Float_t         Getpchi2()                           { return fMinPchi2; };
          Int_t           GetIflag()                           { return fValIflag; };
          Float_t         GetCosThetaMin()                     { return fMinCosTheta; };
          Float_t         GetCosThetaMax()                     { return fMaxCosTheta; };
          Float_t         GetMass()                            { return fMass; };
          Float_t         GetMassWindow()                      { return fMassWindow; };
          Float_t         GetExcludedMass()                    { return fExcludedMass; };
          Float_t         GetExcludedMassWindow()              { return fExcludedMassWindow; };
          Float_t         GetLifetimeMin()                     { return fMinLifetime; };
          Float_t         GetLifetimeMax()                     { return fMaxLifetime; };
          Int_t           GetGSIBham()                         { return fValGSIBham; };
          Float_t         GetCrossYZ()                         { return fMinCrossYZ; };
          Float_t         GetPtMin()                           { return fMinPt; };
          Float_t         GetRapidityMin()                     { return fMinRapidity; };
          Float_t         GetRapidityMax()                     { return fMaxRapidity; };
          Float_t         GetMinPsi()                          { return fMinPsi; };
          T49CutTrack    *GetPosTrackCut()                     { return fPosTrackCuts; };
          T49CutTrack    *GetNegTrackCut()                     { return fNegTrackCuts; };

          TObjArray      *GetAcceptedV0s(TObjArray *V0List);
          TObjArray      *GetNotAcceptedV0s(TObjArray *V0List);
          TObjArray      *GetAcceptedV0Particle(TObjArray *V0List);

 protected:

          Bool_t          CheckGSIBham(T49VertexRoot *V0);
          Bool_t          CheckX1minX2(T49VertexRoot *V0);
          Bool_t          CheckY1minY2(T49VertexRoot *V0);
          Bool_t          CheckXTarg(T49VertexRoot *V0);
          Bool_t          CheckYTarg(T49VertexRoot *V0);
          Bool_t          CheckXV0(T49VertexRoot *V0);
          Bool_t          CheckYV0(T49VertexRoot *V0);
          Bool_t          CheckZV0(T49VertexRoot *V0);
          Bool_t          CheckPt(T49VertexRoot *V0);
          Bool_t          CheckRapidity(T49VertexRoot *V0);
          Bool_t          CheckArmenterosPt(T49VertexRoot *V0);
          Bool_t          CheckArmenterosAlpha(T49VertexRoot *V0);
          Bool_t          CheckPchi2(T49VertexRoot *V0);
          Bool_t          CheckIflag(T49VertexRoot *V0);
          Bool_t          CheckCosTheta(T49VertexRoot *V0);
          Bool_t          CheckLifetime(T49VertexRoot *V0);
          Bool_t          CheckMass(T49VertexRoot *V0);
          Bool_t          CheckExcludedMass(T49VertexRoot *V0);
          Bool_t          CheckCrossYZ(T49VertexRoot *V0);
          Bool_t          CheckOutsideTPC(T49VertexRoot *V0);
          Bool_t          CheckPsi(T49VertexRoot *V0);
          Bool_t          CheckPosTrack(T49VertexRoot *V0);
          Bool_t          CheckNegTrack(T49VertexRoot *V0);
          Bool_t          CheckSameSide(T49VertexRoot *V0);    

  Bool_t        fFast;               // Fast option: Cuts are only checked until the first failure

  Int_t         fIdVtx;              // Vertex ID (11: V0, 41 Xis/Omegas)
  Float_t       fMinX1minX2;         // Track x-distance in the target plane
  Float_t       fMinY1minY2;         // Track y-distance in the target plane
  Float_t       fMaxXTarg;           // Vertex x-position in the target plane 
  Float_t       fMaxYTarg;           // Vertex y-position in the target plane 
  Float_t       fMinXV0;             // Vertex x-position
  Float_t       fMinYV0;             // Vertex y-position                      
  Float_t       fMinZV0;             // Vertex z-position 
  Float_t       fMinArmenterosPt;    // Armenteros-Pt lower cut
  Float_t       fMaxArmenterosPt;    // Armenteros-Pt upper cut
  Float_t       fMinArmenterosAlpha; // Armenteros-Alpha lower cut
  Float_t       fMaxArmenterosAlpha; // Armenteros-Alpha upper cut
  Float_t       fMinPchi2;           // P(chi2) of the V0-fit
  Int_t         fValIflag;           // V0 Iflag
  Float_t       fMass;               // V0 mass
  Float_t       fMassWindow;         // Mass window
  Float_t       fExcludedMass;       // Excluded V0 mass
  Float_t       fExcludedMassWindow; // Excluded mass window
  Float_t       fMinCosTheta;        // Cos theta lower cut
  Float_t       fMaxCosTheta;        // Cos theta upper cut
  Float_t       fMinLifetime;        // Lifetime lower cut
  Float_t       fMaxLifetime;        // Lifetime upper cut
  Int_t         fValGSIBham;         // 1: GSI type V0, 2: Bham type V0
  Float_t       fMinCrossYZ;         // Vertex z-position for the crossYZ cut
  Float_t       fMinPt;              // Transverse momentum lower cut
  Float_t       fMinRapidity;        // Rapidity lower cut
  Float_t       fMaxRapidity;        // Rapidity upper cut
  Float_t       fMinPsi;             // Lower cut on the angle of the V0 relative to the beam axis

  T49CutTrack  *fPosTrackCuts;       // The cuts on the positive tracks
  T49CutTrack  *fNegTrackCuts;       // The cuts on the negative tracks

  Int_t         fV0TypeMass;         // V0 type for the invariant mass cut
  Int_t         fV0TypeCosTheta;     // V0 type for the cos theta cut
  Int_t         fV0TypeLifetime;     // V0 type for the lifetime cut
  Int_t         fV0TypeExcluded;     // V0 type for the excluded invariant mass cut
  Int_t         fV0TypeRapidity;     // V0 type for the rapidity cut

  Bool_t        fUseInvMassCosTheta; // Use invariant mass instead of constant mass

  Bool_t        fCutX1minX2;         // Flags for the various cuts
  Bool_t        fCutY1minY2;
  Bool_t        fCutXTarg;     
  Bool_t        fCutYTarg;
  Bool_t        fCutXV0;
  Bool_t        fCutYV0;                                                          
  Bool_t        fCutZV0; 
  Bool_t        fCutArmenterosPt; 
  Bool_t        fCutArmenterosAlpha;
  Bool_t        fCutPchi2;  
  Bool_t        fCutIflag;  
  Bool_t        fCutMass;
  Bool_t        fCutExcludedMass;
  Bool_t        fCutCosTheta;
  Bool_t        fCutLifetime;
  Bool_t        fCutGSIBham;
  Bool_t        fCutCrossYZ;
  Bool_t        fCutPt;
  Bool_t        fCutRapidity;
  Bool_t        fCutOutsideTPC;
  Bool_t        fCutPsi;
  Bool_t        fCutPosTrack;
  Bool_t        fCutNegTrack;
  Bool_t        fCutSameSide;          

  Int_t         fCntInV0;            // Counters for the various cuts
  Int_t         fCntOutV0;
  Int_t         fCntX1minX2;
  Int_t         fCntY1minY2;
  Int_t         fCntXTarg;
  Int_t         fCntYTarg;
  Int_t         fCntXV0;
  Int_t         fCntYV0;                                                        
  Int_t         fCntZV0;
  Int_t         fCntArmenterosPt;
  Int_t         fCntArmenterosAlpha;
  Int_t         fCntPchi2;
  Int_t         fCntIflag;
  Int_t         fCntMass;
  Int_t         fCntExcludedMass;
  Int_t         fCntCosTheta;
  Int_t         fCntLifetime;
  Int_t         fCntGSIBham;
  Int_t         fCntCrossYZ;
  Int_t         fCntPt;
  Int_t         fCntRapidity;
  Int_t         fCntOutsideTPC;
  Int_t         fCntPsi;
  Int_t         fCntPosTrack;
  Int_t         fCntNegTrack;
  Int_t         fCntSameSide;  

  TObjArray    *fAccV0List;          // !List with all accepted V0s
  TObjArray    *fNotAccV0List;       // !List with all NOT accepted V0s
  TObjArray    *fAccV0ParticleList;  // !List with all accepted V0s stored as T49ParticleRoot

  ClassDef(T49CutV0,8)               // V0 cut class

};

#endif













