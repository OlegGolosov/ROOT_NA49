///////////////////////////////////////////////////////////////////////
//                                                                   //
//    T49CutV0                                                       //
//                                                                   //
//    V0 cut class                                                   //
//                                                                   //
//    Author: Christoph Blume                                        //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include <TPaveText.h>
#include <TObjArray.h>

#include "T49Run.h"
#include "T49ParticleRoot.h"
#include "T49VertexRoot.h"

#include "T49CutV0.h"
#include "T49CutTrack.h"

ClassImp(T49CutV0)

/*
$Log: T49CutV0.C,v $
Revision 1.17  2005/04/26 18:58:13  cblume
SLC3 and rootv4 compatible version

Revision 1.16  2002/03/27 14:05:47  cblume
Add same side cut and cut on vertex X and Y

Revision 1.15  2001/12/12 17:42:32  cblume
Add option to cos(theta) cut

Revision 1.14  2001/11/26 12:20:40  cblume
Change to TObjArray for Vertex2Particle()

Revision 1.13  2001/11/12 12:10:38  cblume
Update for ROOT v3.01

 * Revision 1.12  2000/11/09  08:57:03  cblume
 * Single track cuts are now done via T49CutTrack
 *
 * Revision 1.11  2000/10/27  09:01:28  cblume
 * Remove IFLAG-cut from GSI standard cuts
 *
 * Revision 1.10  2000/10/09  10:25:00  cblume
 * General update
 *
 * Revision 1.9  2000/06/05  12:59:47  cblume
 * Changed particle list to TClonesArray. Update of DrawCutList()
 *
 * Revision 1.8  2000/05/19  15:39:23  cblume
 * Added new functionality
 *
 * Revision 1.7  2000/04/19  15:19:39  cblume
 * Changed GSI standard cuts xtarg from 1.5 to 1.0
 *
 * Revision 1.6  2000/04/18  17:28:04  cblume
 * Adapted to changes in T49VertexRoot. No need for SetDaughter() any more
 *
 * Revision 1.5  2000/04/13  15:14:58  cblume
 * New function to select Bham or GSI V0s and automatic access to the current event
 *
 * Revision 1.4  1999/11/23  13:54:04  cblume
 * Remove files
 *
*/

//______________________________________________________________________________
T49CutV0::T49CutV0():T49Cut() 
{
  //
  // T49CutV0 default constructor
  //

  Reset(); 

  fAccV0List          = NULL;
  fNotAccV0List       = NULL;
  fAccV0ParticleList  = NULL;

  // Default are V0s
  fIdVtx              = 0;

}

//______________________________________________________________________________
T49CutV0::T49CutV0(const Text_t *name, const Text_t *title)
         :T49Cut(name, title) 
{
  //
  // T49CutV0 constructor with name and title
  //

  Reset();

  fAccV0List          = new TObjArray();
  fNotAccV0List       = new TObjArray();
  fAccV0ParticleList  = new TObjArray();

  // Default are V0s
  fIdVtx              = kIdVtxV0;

}

//______________________________________________________________________________
T49CutV0::~T49CutV0() 
{
  //
  // T49CutV0 destructor
  //

  if (fAccV0List) {
    fAccV0List->Delete();
    delete fAccV0List;
  }
  if (fNotAccV0List) {
    fNotAccV0List->Delete();
    delete fNotAccV0List;
  }
  if (fAccV0ParticleList) {
    fAccV0ParticleList->Delete();
    delete fAccV0ParticleList;
  }

}

//_____________________________________________________________________________
void T49CutV0::Copy(TObject &c) const
{
  //
  // Copy function
  //

  //Reset();

  ((T49CutV0 &) c).fFast                  = fFast;
  ((T49CutV0 &) c).fIdVtx                 = fIdVtx;
  ((T49CutV0 &) c).fMinX1minX2            = fMinX1minX2;
  ((T49CutV0 &) c).fMinY1minY2            = fMinY1minY2;
  ((T49CutV0 &) c).fMaxXTarg              = fMaxXTarg;
  ((T49CutV0 &) c).fMaxYTarg              = fMaxYTarg;
  ((T49CutV0 &) c).fMinXV0                = fMinXV0;
  ((T49CutV0 &) c).fMinYV0                = fMinYV0;
  ((T49CutV0 &) c).fMinZV0                = fMinZV0;
  ((T49CutV0 &) c).fMinArmenterosPt       = fMinArmenterosPt;
  ((T49CutV0 &) c).fMaxArmenterosPt       = fMaxArmenterosPt;
  ((T49CutV0 &) c).fMinArmenterosAlpha    = fMinArmenterosAlpha;
  ((T49CutV0 &) c).fMaxArmenterosAlpha    = fMaxArmenterosAlpha;
  ((T49CutV0 &) c).fMinPchi2              = fMinPchi2;
  ((T49CutV0 &) c).fValIflag              = fValIflag;
  ((T49CutV0 &) c).fMass                  = fMass;
  ((T49CutV0 &) c).fMassWindow            = fMassWindow;
  ((T49CutV0 &) c).fExcludedMass          = fExcludedMass;
  ((T49CutV0 &) c).fExcludedMassWindow    = fExcludedMassWindow;
  ((T49CutV0 &) c).fMinCosTheta           = fMinCosTheta;
  ((T49CutV0 &) c).fMaxCosTheta           = fMaxCosTheta;
  ((T49CutV0 &) c).fMinLifetime           = fMinLifetime;
  ((T49CutV0 &) c).fMaxLifetime           = fMaxLifetime;
  ((T49CutV0 &) c).fValGSIBham            = fValGSIBham;
  ((T49CutV0 &) c).fMinCrossYZ            = fMinCrossYZ;
  ((T49CutV0 &) c).fMinPt                 = fMinPt;
  ((T49CutV0 &) c).fMinRapidity           = fMinRapidity;
  ((T49CutV0 &) c).fMaxRapidity           = fMaxRapidity;
  ((T49CutV0 &) c).fMinPsi                = fMinPsi;

  ((T49CutV0 &) c).fV0TypeMass            = fV0TypeMass;
  ((T49CutV0 &) c).fV0TypeCosTheta        = fV0TypeCosTheta;
  ((T49CutV0 &) c).fV0TypeLifetime        = fV0TypeLifetime;
  ((T49CutV0 &) c).fV0TypeExcluded        = fV0TypeExcluded;
  ((T49CutV0 &) c).fV0TypeRapidity        = fV0TypeRapidity;

  ((T49CutV0 &) c).fUseInvMassCosTheta    = fUseInvMassCosTheta;

  ((T49CutV0 &) c).fCutX1minX2            = fCutX1minX2;
  ((T49CutV0 &) c).fCutY1minY2            = fCutY1minY2;
  ((T49CutV0 &) c).fCutXTarg              = fCutXTarg;
  ((T49CutV0 &) c).fCutYTarg              = fCutYTarg;
  ((T49CutV0 &) c).fCutXV0                = fCutXV0;
  ((T49CutV0 &) c).fCutYV0                = fCutYV0;
  ((T49CutV0 &) c).fCutZV0                = fCutZV0;
  ((T49CutV0 &) c).fCutArmenterosPt       = fCutArmenterosPt;
  ((T49CutV0 &) c).fCutArmenterosAlpha    = fCutArmenterosAlpha;
  ((T49CutV0 &) c).fCutPchi2              = fCutPchi2;
  ((T49CutV0 &) c).fCutIflag              = fCutIflag;
  ((T49CutV0 &) c).fCutMass               = fCutMass;
  ((T49CutV0 &) c).fCutExcludedMass       = fCutExcludedMass;
  ((T49CutV0 &) c).fCutCosTheta           = fCutCosTheta;
  ((T49CutV0 &) c).fCutLifetime           = fCutLifetime;
  ((T49CutV0 &) c).fCutGSIBham            = fCutGSIBham;
  ((T49CutV0 &) c).fCutCrossYZ            = fCutCrossYZ;
  ((T49CutV0 &) c).fCutPt                 = fCutPt;
  ((T49CutV0 &) c).fCutRapidity           = fCutRapidity;
  ((T49CutV0 &) c).fCutOutsideTPC         = fCutOutsideTPC;
  ((T49CutV0 &) c).fCutPsi                = fCutPsi;
  ((T49CutV0 &) c).fCutPosTrack           = fCutPosTrack;
  ((T49CutV0 &) c).fCutNegTrack           = fCutNegTrack;
  ((T49CutV0 &) c).fCutSameSide           = fCutSameSide;   

  ((T49CutV0 &) c).fPosTrackCuts          = fPosTrackCuts;
  ((T49CutV0 &) c).fNegTrackCuts          = fNegTrackCuts;

}

//______________________________________________________________________________
void T49CutV0::SetStandardCutsBham() 
{
  //
  // Sets the standard cuts for the Birmingham V0s:
  //             X1minX2 > 2.5     
  //               XTarg < 1.5     
  //               YTarg < 1.0     
  //                 ZV0 > -555.0  
  //               Iflag = 0       
  //                               

  SetX1minX2(2.5);
  SetXTarg(1.5);
  SetYTarg(1.0);
  SetZV0(-555.0);
  SetIflag(0);
  SetBham();

}

//______________________________________________________________________________
void T49CutV0::SetStandardCutsGSI() 
{
  //
  // Sets the standard cuts for the GSI V0s:
  //               XTarg < 1.0     
  //               YTarg < 0.5     
  //                 ZV0 > -555.0  
  //                               

  SetXTarg(1.0);
  SetYTarg(0.5);
  SetZV0(-555.0);
  SetGSI();

}

//______________________________________________________________________________
void T49CutV0::SetMassLambda(Float_t window) 
{
  //
  // Switches on the invariant mass cut for the lambda. The default mass window
  // is +-10 MeV.
  //

  SetMass(kLambda,kLambdaMass,window);

}

//______________________________________________________________________________
void T49CutV0::SetMassAntiLambda(Float_t window) 
{
  //
  // Switches on the invariant mass cut for the anti-lambda. The default mass window
  // is +-10 MeV.
  //

  SetMass(kAntiLambda,kLambdaMass,window);

}

//______________________________________________________________________________
void T49CutV0::SetMassK0s(Float_t window) 
{
  //
  // Switches on the invariant mass cut for the K0short. The default mass window
  // is +-10 MeV.
  //

  SetMass(kK0s,kK0sMass,window);

}

//______________________________________________________________________________
void T49CutV0::SetMassXi(Float_t window) 
{
  //
  // Switches on the invariant mass cut for the Xi. The default mass window
  // is +-10 MeV.
  //

  SetMass(kXi,kXiMass,window);

}

//______________________________________________________________________________
void T49CutV0::SetMassOmega(Float_t window) 
{
  //
  // Switches on the invariant mass cut for the Omega. The default mass window
  // is +-10 MeV.
  //

  SetMass(kOmega,kOmegaMass,window);

}

//______________________________________________________________________________
void T49CutV0::SetMass(Int_t type, Float_t mass, Float_t window) 
{
  //
  // Switches on the invariant mass cut for a specific V0-type.
  //               1 - lambda
  //               2 - anti-lambda
  //               3 - K0s 
  //               4 - Xi
  //               5 - Omega
  // This function can be used to set a mass window around an arbitrary mass.
  //

  if ((type != kK0s)        && 
      (type != kLambda)     && 
      (type != kAntiLambda) &&
      (type != kXi)         &&
      (type != kOmega)) {

    printf(" T49CutV0::SetMass - V0-type ill defined: %d\n",type);

  }
  else {

    fMass       = mass;
    fMassWindow = window;
    fV0TypeMass = type;
    fCutMass    = kTRUE;

  }

}

//______________________________________________________________________________
void T49CutV0::SetExcludedMassLambda(Float_t window) 
{
  //
  // Cuts out a region in the lambda invariant mass. The default mass window
  // is +-10 MeV.
  //

  SetExcludedMass(kLambda,kLambdaMass,window);

}

//______________________________________________________________________________
void T49CutV0::SetExcludedMassAntiLambda(Float_t window) 
{
  //
  // Cuts out a region in the anti-lambda invariant mass. The default mass window
  // is +-10 MeV.
  //

  SetExcludedMass(kAntiLambda,kLambdaMass,window);

}

//______________________________________________________________________________
void T49CutV0::SetExcludedMassK0s(Float_t window) 
{
  //
  // Cuts out a region in the K0s invariant mass. The default mass window
  // is +-10 MeV.
  //

  SetExcludedMass(kK0s,kK0sMass,window);

}

//______________________________________________________________________________
void T49CutV0::SetExcludedMassXi(Float_t window) 
{
  //
  // Cuts out a region in the Xi invariant mass. The default mass window
  // is +-10 MeV.
  //

  SetExcludedMass(kXi,kXiMass,window);

}

//______________________________________________________________________________
void T49CutV0::SetExcludedMassOmega(Float_t window) 
{
  //
  // Cuts out a region in the Omega invariant mass. The default mass window
  // is +-10 MeV.
  //

  SetExcludedMass(kOmega,kOmegaMass,window);

}

//______________________________________________________________________________
void T49CutV0::SetExcludedMass(Int_t type, Float_t mass, Float_t window) 
{
  //
  // Cuts out a region in the invariant mass of a specific V0 type.
  //               1 - lambda
  //               2 - anti-lambda
  //               3 - K0s 
  //               4 - Xi 
  //               5 - Omega 
  // This function can be used to set a mass window around an arbitrary mass.
  //

  if ((type != kK0s)        && 
      (type != kLambda)     && 
      (type != kAntiLambda) &&
      (type != kXi)         &&
      (type != kOmega))     {

    printf(" T49CutV0::SetMass - V0-type ill defined: %d\n",type);

  }
  else {

    fExcludedMass       = mass;
    fExcludedMassWindow = window;
    fV0TypeExcluded     = type;
    fCutExcludedMass    = kTRUE;

  }

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckGSIBham(T49VertexRoot *V0) 
{
  //
  // Cut on the GSI/Bham flag
  //

  Bool_t passed = kTRUE;

  if ((!(V0->GetIflag() & fValGSIBham))) {
    passed = kFALSE;
    fCntGSIBham++;
    if (fVerbose > 1) {
      printf(" V0 failed GSI-Bham cut: %d (%d)\n",V0->GetIflag(),fValGSIBham);
    }
  }
  
  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckX1minX2(T49VertexRoot *V0) 
{
  //
  // Cut on X1minX2
  //

  Bool_t passed = kTRUE;

  if (TMath::Abs(V0->GetX1minX2()) < fMinX1minX2) {
    passed = kFALSE;
    fCntX1minX2++;
    if (fVerbose > 1) {
      printf(" V0 failed X1minX2 cut: %g (%g)\n",V0->GetX1minX2(),fMinX1minX2);
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckY1minY2(T49VertexRoot *V0) 
{
  //
  // Cut on Y1minY2
  //

  Bool_t passed = kTRUE;

  Float_t y1miny2 = 0;
  if (fIdVtx == kIdVtxXi) {
    y1miny2 = TMath::Abs(V0->GetY1minY2Xi());
  }
  else {
    y1miny2 = TMath::Abs(V0->GetY1minY2());
  }    

  if (y1miny2 < fMinY1minY2) {
    passed = kFALSE;
    fCntY1minY2++;
    if (fVerbose > 1) {
      printf(" V0 failed Y1minY2 cut: %g (%g)\n",y1miny2,fMinY1minY2);
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckXTarg(T49VertexRoot *V0) 
{
  //
  // Cut on XTarg
  //

  Bool_t passed = kTRUE;

  if (((fMaxXTarg > 0.0) && 
       (TMath::Abs(V0->GetXTarg()) > fMaxXTarg)) ||
      ((fMaxXTarg < 0.0) && 
       (TMath::Abs(V0->GetXTarg()) < TMath::Abs(fMaxXTarg)))) {
    passed = kFALSE;
    fCntXTarg++;
    if (fVerbose > 1) {
      printf(" V0 failed XTarg cut: %g (%g)\n",V0->GetXTarg(),fMaxXTarg);
    }
  } 

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckYTarg(T49VertexRoot *V0) 
{
  //
  // Cut on YTarg
  //

  Bool_t passed = kTRUE;

  if (((fMaxYTarg > 0.0) && 
       (TMath::Abs(V0->GetYTarg()) > fMaxYTarg)) ||
      ((fMaxYTarg < 0.0) && 
       (TMath::Abs(V0->GetYTarg()) < TMath::Abs(fMaxYTarg)))) {
    passed = kFALSE;
    fCntYTarg++;
    if (fVerbose > 1) {
      printf(" V0 failed YTarg cut: %g (%g)\n",V0->GetYTarg(),fMaxYTarg);
    }
  } 

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckXV0(T49VertexRoot *V0)
{
  //
  // Cut on the V0 x-position
  //
 
  Bool_t passed = kTRUE;
 
  if (TMath::Abs(V0->GetX()) < fMinXV0) {
    passed = kFALSE;
    fCntXV0++;
    if (fVerbose > 1) {
      printf(" V0 failed X cut: %g (%g)\n",V0->GetX(),fMinXV0);
    }
  }
 
  return  passed;
 
}
 
//______________________________________________________________________________
Bool_t T49CutV0::CheckYV0(T49VertexRoot *V0)
{
  //
  // Cut on the V0 y-position
  //
 
  Bool_t passed = kTRUE;
 
  if (TMath::Abs(V0->GetY()) < fMinYV0) {
    passed = kFALSE;
    fCntYV0++;
    if (fVerbose > 1) {
      printf(" V0 failed Y cut: %g (%g)\n",V0->GetY(),fMinYV0);
    }
  }
 
  return  passed;
 
}                                                                               

//______________________________________________________________________________
Bool_t T49CutV0::CheckZV0(T49VertexRoot *V0) 
{
  //
  // Cut on the V0 z-position
  //

  Bool_t passed = kTRUE;

  if (V0->GetZ() < fMinZV0) {
    passed = kFALSE;
    fCntZV0++;
    if (fVerbose > 1) {
      printf(" V0 failed Z cut: %g (%g)\n",V0->GetZ(),fMinZV0);
    }
  }

  return  passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckPt(T49VertexRoot *V0) 
{
  //
  // Cut on the V0 Pt
  //

  Bool_t passed = kTRUE;

  if (V0->GetPt() < fMinPt) {
    passed = kFALSE;
    fCntPt++;
    if (fVerbose > 1) {
      printf(" V0 failed Pt cut: %g (%g)\n",V0->GetPt(),fMinPt);
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckRapidity(T49VertexRoot *V0) 
{
  //
  // Cut on the V0 rapidity
  //

  Bool_t passed = kTRUE;

  Float_t rapidity = 0;
  switch (fV0TypeRapidity) {
  case kLambda:
  case kAntiLambda:
    rapidity = V0->GetRapidityLambda();
    break;
  case kK0s:
    rapidity = V0->GetRapidityK0s();
    break;
  case kXi:
    rapidity = V0->GetRapidityXi();
    break;
  case kOmega:
    rapidity = V0->GetRapidityOmega();
    break;
  };
  if ((rapidity < fMinRapidity) || (rapidity > fMaxRapidity)) {
    passed = kFALSE;
    fCntRapidity++;
    if (fVerbose > 1) {
      printf(" V0 failed rapidity cut: %g (%g - %g)\n",rapidity
                                                      ,fMinRapidity
                                                      ,fMaxRapidity);
    }
  }    

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckArmenterosPt(T49VertexRoot *V0) 
{
  //
  // Cut on the V0 Armenteros-Pt (only for (anti-)lambda and K0s)
  //

  Bool_t passed = kTRUE;

  if ((V0->GetArmenterosPt() < fMinArmenterosPt) ||
      (V0->GetArmenterosPt() > fMaxArmenterosPt)) {
    passed = kFALSE;
    fCntArmenterosPt++;
    if (fVerbose > 1) {
      printf(" V0 failed Armenteros-Pt cut: %g (%g - %g)\n",V0->GetArmenterosPt()
                                                           ,fMinArmenterosPt
                                                           ,fMaxArmenterosPt);
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckArmenterosAlpha(T49VertexRoot *V0) 
{
  //
  // Cut on the V0 Armenteros-alpha (only for (anti-)lambda and K0s)
  //

  Bool_t passed = kTRUE;

  if ((V0->GetArmenterosAlpha() < fMinArmenterosAlpha) ||
      (V0->GetArmenterosAlpha() > fMaxArmenterosAlpha)) {
    passed = kFALSE;
    fCntArmenterosAlpha++;
    if (fVerbose > 1) {
      printf(" V0 failed Armenteros-alpha cut: %g (%g - %g)\n",V0->GetArmenterosAlpha()
                                                              ,fMinArmenterosAlpha
                                                              ,fMaxArmenterosAlpha);
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckPchi2(T49VertexRoot *V0) 
{
  //
  // Cut on the P(chi2)
  //

  Bool_t passed = kTRUE;

  if (V0->GetPchi2() < fMinPchi2) {
    passed = kFALSE;
    fCntPchi2++;
    if (fVerbose > 1) {
      printf(" V0 failed Pchi2 cut: %g (%g)\n",V0->GetPchi2(),fMinPchi2);
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckIflag(T49VertexRoot *V0) 
{
  //
  // Cut on the vertex IFLAG
  //

  Bool_t passed = kTRUE;

  if ((V0->GetIflag() & 0x00ff) != fValIflag) {
    passed = kFALSE;
    fCntIflag++;
    if (fVerbose > 1) {
      printf(" V0 failed Iflag cut: %d (%d)\n",V0->GetIflag(),fValIflag);
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckCosTheta(T49VertexRoot *V0) 
{
  //
  // Cut on the V0 cos(theta)
  //

  Bool_t passed = kTRUE;

  Float_t cosTheta = 0;
  switch (fV0TypeCosTheta) {
  case kLambda:
    cosTheta = V0->GetCosThetaLambda(fUseInvMassCosTheta);
    break;
  case kAntiLambda:
    cosTheta = V0->GetCosThetaAntiLambda(fUseInvMassCosTheta);
    break;
  case kK0s:
    cosTheta = V0->GetCosThetaK0s(fUseInvMassCosTheta);
    break;
  case kXi:
    cosTheta = V0->GetCosThetaXi(fUseInvMassCosTheta);
    break;
  case kOmega:
    cosTheta = V0->GetCosThetaOmega(fUseInvMassCosTheta);
    break;
  };    
  if ((cosTheta < fMinCosTheta) || (cosTheta > fMaxCosTheta)) {
    passed = kFALSE;
    fCntCosTheta++;
    if (fVerbose > 1) {
      printf(" V0 failed cos theta cut: %g (%g - %g)\n",cosTheta
                                                       ,fMinCosTheta
                                                       ,fMaxCosTheta);
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckLifetime(T49VertexRoot *V0) 
{
  //
  // Cut on the V0 lifetime
  //

  Bool_t passed = kTRUE;

  Float_t lifetime = 0;
  switch (fV0TypeLifetime) {
  case kLambda:
  case kAntiLambda:
    lifetime = V0->GetLifetimeLambda() / kLambdaLifetime;
    break;
  case kK0s:
    lifetime = V0->GetLifetimeK0s()    / kK0sLifetime;
    break;
  case kXi:
    lifetime = V0->GetLifetimeXi()     / kXiLifetime;
    break;
  case kOmega:
    lifetime = V0->GetLifetimeOmega()  / kOmegaLifetime;
    break;
  };    
  if ((lifetime < fMinLifetime) || (lifetime > fMaxLifetime)) {
    passed = kFALSE;
    fCntLifetime++;
    if (fVerbose > 1) {
      printf(" V0 failed lifetime cut: %g (%g - %g)\n",lifetime
                                                      ,fMinLifetime
                                                      ,fMaxLifetime);
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckMass(T49VertexRoot *V0) 
{
  //
  // Cut on the V0 invariant mass
  //

  Bool_t passed = kTRUE;

  Float_t massV0 = 0;
  switch (fV0TypeMass) {
  case kLambda:
    massV0 = V0->GetInvariantMassLambda();
    break;
  case kAntiLambda:
    massV0 = V0->GetInvariantMassAntiLambda();
    break;
  case kK0s:
    massV0 = V0->GetInvariantMassK0s();
    break;
  case kXi:
    massV0 = V0->GetInvariantMassXi();
    break;
  case kOmega:
    massV0 = V0->GetInvariantMassOmega();
    break;
  };    
  if (TMath::Abs(massV0 - fMass) > fMassWindow) {
    passed = kFALSE;
    fCntMass++;
    if (fVerbose > 1) {
      printf(" V0 failed invariant mass cut: %g - %g (%g)\n",massV0,fMass,fMassWindow);
    }
  } 

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckExcludedMass(T49VertexRoot *V0) 
{
  //
  // Cut to exclude an invariant mass region of a V0
  //

  Bool_t passed = kTRUE;

  Float_t massV0 = 0;
  switch (fV0TypeExcluded) {
  case kLambda:
    massV0 = V0->GetInvariantMassLambda();
    break;
  case kAntiLambda:
    massV0 = V0->GetInvariantMassAntiLambda();
    break;
  case kK0s:
    massV0 = V0->GetInvariantMassK0s();
    break;
  case kXi:
    massV0 = V0->GetInvariantMassXi();
    break;
  case kOmega:
    massV0 = V0->GetInvariantMassOmega();
    break;
  };    
  if (TMath::Abs(massV0 - fExcludedMass) < fExcludedMassWindow) {
    passed = kFALSE;
    fCntExcludedMass++;
    if (fVerbose > 1) {
      printf(" V0 failed excluded invariant mass cut: %g - %g (%g)\n",massV0
                                                                     ,fExcludedMass
                                                                     ,fExcludedMassWindow);
    }
  } 

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckCrossYZ(T49VertexRoot *V0) 
{
  //
  // Cut on the crossing in the YZ-plane
  //

  Bool_t passed = kTRUE;

  if (V0->GetCrossYZ(fMinCrossYZ) > 0) {
    passed = kFALSE;
    fCntCrossYZ++;
    if (fVerbose > 1) {
      printf(" V0 failed crossYZ cut: %g\n",V0->GetCrossYZ(fMinCrossYZ));
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckOutsideTPC(T49VertexRoot *V0) 
{
  //
  // Cut on V0 vertices inside the VTPCs
  //

  Bool_t passed = kTRUE;

  if ((TMath::Abs(V0->GetX()) >= 10.0) &&
      (((V0->GetZ() >= (-112.0 - 10.0)) && (V0->GetZ() <=  112.0)) ||
       ((V0->GetZ() >= (-492.0 - 10.0)) && (V0->GetZ() <= -267.0)))) {
    passed = kFALSE;
    fCntOutsideTPC++;
    if (fVerbose > 1) {
      printf(" V0 failed outside TPC cut: x = %g, z = %g\n"   
            ,V0->GetX(),V0->GetZ());
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckPsi(T49VertexRoot *V0) 
{
  //
  // Cut on the angle of the V0 relative to the beam axis
  //

  Bool_t passed = kTRUE;

  Float_t psi = TMath::ATan(V0->GetPt() / V0->GetPz());
  if (psi < fMinPchi2) {
    passed = kFALSE;
    fCntPsi++;
    if (fVerbose > 1) {
      printf(" V0 failed Psi cut: %g (%g)\n",psi,fMinPsi);
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckPosTrack(T49VertexRoot *V0) 
{
  //
  // Checks the cuts on the positive daughter track
  //

  Bool_t passed = kTRUE;
  Int_t  ok     = 0;

  T49ParticleRoot *Pos = V0->GetFullPositiveTrack();
  if (!(ok = fPosTrackCuts->CheckParticle(Pos))) {
    passed = kFALSE;
    fCntPosTrack++;
    if (fVerbose > 1) {
      printf(" V0 failed cuts on positive daughter\n");
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckNegTrack(T49VertexRoot *V0) 
{
  //
  // Checks the cuts on the negative daughter track
  //

  Bool_t passed = kTRUE;
  Int_t  ok     = 0;

  T49ParticleRoot *Neg = V0->GetFullNegativeTrack();
  if (!(ok = fNegTrackCuts->CheckParticle(Neg))) {
    passed = kFALSE;
    fCntNegTrack++;
    if (fVerbose > 1) {
      printf(" V0 failed cuts on negative daughter\n");
    }
  }

  return passed;

}

//______________________________________________________________________________
Bool_t T49CutV0::CheckSameSide(T49VertexRoot *V0)
{
  //
  // Checks whether the V0 daughters are on the same detector side
  //
 
  Bool_t passed = kTRUE;
 
  if (fIdVtx == kIdVtxXi) {
    if (!V0->GetSameSideXi()) {
      passed = kFALSE;
      fCntSameSide++;
      if (fVerbose > 1) {
        printf(" V0 failed same side cut: %d\n"
              ,V0->GetSameSideXi());
      }
    }
  }
  else {
    if (!V0->GetSameSide()) {
      passed = kFALSE;
      fCntSameSide++;
      if (fVerbose > 1) {
        printf(" V0 failed same side cut: %d\n"
              ,V0->GetSameSide());
      }
    }
  }
 
  return passed;
 
}
                                                                                
//______________________________________________________________________________
Bool_t T49CutV0::CheckV0(T49VertexRoot *V0) 
{
  //
  // Applies the cuts to a given V0
  //

  Bool_t passed = kTRUE;

  // Cut out non-V0 vertices
  if (V0->GetIdVtx() != fIdVtx) return kFALSE;

  fCntInV0++;

  if (fCutGSIBham         && (!CheckGSIBham(V0))) {
    passed = kFALSE;
    if (fFast) return passed;    
  }

  if (fCutX1minX2         && (!CheckX1minX2(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutY1minY2         && (!CheckY1minY2(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutXTarg           && (!CheckXTarg(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutYTarg           && (!CheckYTarg(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutXV0             && (!CheckXV0(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }
 
  if (fCutYV0             && (!CheckYV0(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }                                                                             

  if (fCutZV0             && (!CheckZV0(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }    

  if (fCutPt              && (!CheckPt(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutRapidity        && (!CheckRapidity(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutArmenterosPt    && (!CheckArmenterosPt(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutArmenterosAlpha && (!CheckArmenterosAlpha(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutPchi2           && (!CheckPchi2(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutIflag           && (!CheckIflag(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutCosTheta        && (!CheckCosTheta(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutLifetime        && (!CheckLifetime(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutMass            && (!CheckMass(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutExcludedMass    && (!CheckExcludedMass(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutCrossYZ         && (!CheckCrossYZ(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutOutsideTPC      && (!CheckOutsideTPC(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutPsi             && (!CheckPsi(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutPosTrack        && (!CheckPosTrack(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutNegTrack        && (!CheckNegTrack(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (fCutSameSide        && (!CheckSameSide(V0))) {
    passed = kFALSE;
    if (fFast) return passed;
  }

  if (passed) {
    fCntOutV0++;
    if (fVerbose > 1) {
      printf(" V0 passed all cuts\n");
    }
  }

  return passed;

} 

//______________________________________________________________________________
TObjArray *T49CutV0::Vertex2Particle(TObjArray *V0List) 
{
  //
  // Converts a list of T49VertexRoot objects into a TObjArray
  // of T49ParticleRoot objects. The vertex position is stored in
  // the first point coordinates.
  //

  T49VertexRoot   *V0;
  T49ParticleRoot *V0Particle;

  // Cleanup before filling new list
  fAccV0ParticleList->Delete();

  for (Int_t idxV0 = 0; idxV0 < V0List->GetEntries(); idxV0++) {
    V0 = (T49VertexRoot *) V0List->UncheckedAt(idxV0);
    V0Particle = new T49ParticleRoot();  
    V0Particle->SetPx(V0->GetPx());
    V0Particle->SetPy(V0->GetPy());
    V0Particle->SetPz(V0->GetPz());
    V0Particle->SetCharge(0);
    V0Particle->SetXFirst(V0->GetX());
    V0Particle->SetYFirst(V0->GetY());
    V0Particle->SetZFirst(V0->GetZ());
    fAccV0ParticleList->Add(V0Particle);
  }

  return fAccV0ParticleList;

}

//______________________________________________________________________________
TObjArray *T49CutV0::GetAcceptedV0s(TObjArray *V0List) 
{
  //
  // Creates a new vertex list with all V0s that have passed the cuts
  //

  T49VertexRoot *V0;

  fAccV0List->Clear();

  for (Int_t idxV0 = 0; idxV0 < V0List->GetEntries(); idxV0++) {
    V0 = (T49VertexRoot *) V0List->UncheckedAt(idxV0);
    if (fVerbose > 1) {
      printf(" Checking V0 no. %d\n",idxV0);
    }
    if (CheckV0(V0)) fAccV0List->Add(V0);
  }

  if (fVerbose) {
    printf(" Accepted %d V0s from %d\n",fAccV0List->GetEntries()
                                       ,V0List->GetEntries());
  }

  return fAccV0List;

}

//______________________________________________________________________________
TObjArray *T49CutV0::GetNotAcceptedV0s(TObjArray *V0List) 
{
  //
  // Creates a new vertex list with all V0s that have NOT passed the cuts
  //

  T49VertexRoot *V0;

  fNotAccV0List->Clear();

  for (Int_t idxV0 = 0; idxV0 < V0List->GetEntries(); idxV0++) {
    V0 = (T49VertexRoot *) V0List->UncheckedAt(idxV0);
    if (fVerbose > 1) {
      printf(" Checking V0 no. %d\n",idxV0);
    }
    if (!CheckV0(V0)) fNotAccV0List->Add(V0);
  }

  if (fVerbose) {
    printf(" Accepted %d V0s from %d\n",fNotAccV0List->GetEntries()
                                       ,V0List->GetEntries());
  }

  return fNotAccV0List;

}

//______________________________________________________________________________
TObjArray *T49CutV0::GetAcceptedV0Particle(TObjArray *V0List) 
{
  //
  // Creates a new list of T49ParticleRoot with all V0s that have passed the cuts
  //

  T49VertexRoot   *V0;
  T49ParticleRoot *V0Particle;

  // Cleanup before filling new list
  fAccV0ParticleList->Delete();

  for (Int_t idxV0 = 0; idxV0 < V0List->GetEntries(); idxV0++) {
    V0 = (T49VertexRoot *) V0List->UncheckedAt(idxV0);
    if (fVerbose > 1) {
      printf("Checking V0 no. %d\n",idxV0);
    }
    if (CheckV0(V0)) {
      V0Particle = new T49ParticleRoot();
      V0Particle->SetPx(V0->GetPx());
      V0Particle->SetPy(V0->GetPy());
      V0Particle->SetPz(V0->GetPz());
      V0Particle->SetCharge(0);
      V0Particle->SetXFirst(V0->GetX());
      V0Particle->SetYFirst(V0->GetY());
      V0Particle->SetZFirst(V0->GetZ());
      fAccV0ParticleList->Add(V0Particle);
    }
  }

  if (fVerbose) {
    printf(" Accepted %d V0s from %d\n",fAccV0ParticleList->GetEntries()
                                       ,V0List->GetEntries());
  }

  return fAccV0ParticleList;

}

//______________________________________________________________________________
void T49CutV0::DrawCutList() 
{
  //
  // Draws a TPaveText with the defined cuts.
  //

  TPaveText *Cuts = CreateCutList();
  Cuts->Draw();

}

//______________________________________________________________________________
TPaveText *T49CutV0::CreateCutList() 
{
  //
  // Creates a TPaveText with the defined cuts.
  //

  TText     *Text;

  Char_t ctext[80];

  TPaveText *Cuts = new TPaveText(0.0,0.0,1.0,1.0,"NDC");
  Cuts->SetFillColor(0);  

  sprintf(ctext," V0-Cuts (%s)",this->GetTitle());
  Text = Cuts->AddText(ctext);
  Text->SetTextAlign(11);
  Text->SetTextColor(46); 

  if (fCutGSIBham) {
    if      (fValGSIBham == 0x4000) {
      Text = Cuts->AddText("       Select only GSI type V0s");
      Text->SetTextAlign(21);
    }
    else if (fValGSIBham == 0x8000) { 
      Text = Cuts->AddText("       Select only Bham type V0s");
      Text->SetTextAlign(21);
    }
  }
  if (fCutX1minX2) {
    sprintf(ctext,"X_{1}minX_{2}     >    %g",fMinX1minX2);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutY1minY2) {
    sprintf(ctext,"Y_{1}minY_{2}     >    %g",fMinY1minY2);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutXTarg) {
    if (fMaxXTarg > 0.0) {
      sprintf(ctext,"X_{Targ}     <     %g",fMaxXTarg);
    }
    else {
      sprintf(ctext,"X_{Targ}     >     %g",fMaxXTarg);
    }
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutYTarg) {
    if (fMaxYTarg > 0.0) {
      sprintf(ctext,"Y_{Targ}     <     %g",fMaxYTarg);
    }
    else {
      sprintf(ctext,"Y_{Targ}     >     %g",fMaxYTarg);
    }
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutXV0) {
    sprintf(ctext,"X-position     >     %g",fMinXV0);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }
  if (fCutYV0) {
    sprintf(ctext,"Y-position     >     %g",fMinYV0);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }                                                                             
  if (fCutZV0) {
    sprintf(ctext,"Z-position     >     %g",fMinZV0);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutArmenterosPt) {
    sprintf(ctext,"Armenteros-P_{t} window     =     %g  -  %g",fMinArmenterosPt
                                                            ,fMaxArmenterosPt);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutArmenterosAlpha) {
    sprintf(ctext,"Armenteros-alpha window     =     %g  -  %g",fMinArmenterosAlpha
                                                               ,fMaxArmenterosAlpha);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutPchi2) {
    sprintf(ctext,"Fit-P(#xi^{2})     >     %g",fMinPchi2);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutIflag) {
    sprintf(ctext,"IFLAG     =     %d",fValIflag);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutCosTheta) {
    if (fUseInvMassCosTheta) {
      sprintf(ctext,"cos(#theta) (minv)  window =     %g  -  %g",fMinCosTheta,fMaxCosTheta);
    }
    else {
      sprintf(ctext,"cos(#theta) (const) window =     %g  -  %g",fMinCosTheta,fMaxCosTheta);
    }
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutLifetime) {
    sprintf(ctext,"#tau / #tau_{0} window     =     %g  -  %g",fMinLifetime,fMaxLifetime);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutMass) {
    switch (fV0TypeMass) {
    case kLambda:
      sprintf(ctext,"#Lambda invariant mass     =     %g,  %g",fMass,fMassWindow);
      Text = Cuts->AddText(ctext);
      Text->SetTextAlign(21);
      break;
    case kAntiLambda:
      sprintf(ctext,"#bar{#Lambda} invariant mass     =     %g,  %g",fMass,fMassWindow);
      Text = Cuts->AddText(ctext);
      Text->SetTextAlign(21);
      break;
    case kK0s:
      sprintf(ctext,"K^{0}_{s} invariant mass     =     %g,  %g",fMass,fMassWindow);
      Text = Cuts->AddText(ctext);
      Text->SetTextAlign(21);
      break;
    };    
  }
  if (fCutExcludedMass) {
    switch (fV0TypeExcluded) {
    case kLambda:
      sprintf(ctext,"#Lambda invariant mass     =     %g,  %g (excluded)"
                   ,fExcludedMass,fExcludedMassWindow);
      Text = Cuts->AddText(ctext);
      Text->SetTextAlign(21);
      break;
    case kAntiLambda:
      sprintf(ctext,"#bar{#Lambda} invariant mass     =     %g,  %g (excluded)"
                   ,fExcludedMass,fExcludedMassWindow);
      Text = Cuts->AddText(ctext);
      Text->SetTextAlign(21);
      break;
    case kK0s:
      sprintf(ctext,"K^{0}_{s} invariant mass     =     %g,  %g (excluded)"
                   ,fExcludedMass,fExcludedMassWindow);
      Text = Cuts->AddText(ctext);
      Text->SetTextAlign(21);
      break;
    };    
  }
  if (fCutCrossYZ) {
    sprintf(ctext,"CrossYZ     >     %g",fMinCrossYZ);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutPt) {
    sprintf(ctext,"P_{t}     >     %g",fMinPt);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutRapidity) {
    sprintf(ctext,"Rapidity window     =     %g  -  %g",fMinRapidity,fMaxRapidity);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }  
  if (fCutOutsideTPC) { 
    Text = Cuts->AddText("Select only V0s that decay outside of the VTPCs");
    Text->SetTextAlign(21);
  }
  if (fCutPsi) {
    sprintf(ctext,"#Psi     >     %g",fMinPsi);
    Text = Cuts->AddText(ctext);
    Text->SetTextAlign(21);
  }
  if (fCutPosTrack) {
    Text = Cuts->AddText("Apply single track cuts on positive daughter");
    Text->SetTextAlign(21);
  }
  if (fCutNegTrack) {
    Text = Cuts->AddText("Apply single track cuts on negative daughter");
    Text->SetTextAlign(21);
  }
  if (fCutSameSide) {
    Text = Cuts->AddText("Apply same side cut on the daughter tracks");
    Text->SetTextAlign(21);
  }                                                                             

  sprintf(ctext," ");
  Text = Cuts->AddText(ctext);

  return Cuts;

}

//______________________________________________________________________________
void T49CutV0::PrintCutList() 
{
  //
  // Prints the defined cuts.
  //

  printf("\n");
  printf(" V0-Cuts (%s)\n\n",this->GetTitle());  
  if (fCutGSIBham) {
    if      (fValGSIBham == 0x4000) 
      printf("       Select only GSI type V0s\n\n");
    else if (fValGSIBham == 0x8000) 
      printf("       Select only Bham type V0s\n\n");
  }
  if (fCutX1minX2) {
    printf("                    X1minX2 > %g\n",fMinX1minX2);
  }
  if (fCutY1minY2) {
    printf("                    Y1minY2 > %g\n",fMinY1minY2);
  }
  if (fCutXTarg) {
    if (fMaxXTarg > 0.0) {
      printf("       x-position at target < %g\n",fMaxXTarg);
    }
    else {
      printf("       x-position at target > %g\n",TMath::Abs(fMaxXTarg));
    }
  }  
  if (fCutYTarg) {
    if (fMaxYTarg > 0.0) {
      printf("       y-position at target < %g\n",fMaxYTarg);
    }
    else {
      printf("       y-position at target > %g\n",TMath::Abs(fMaxYTarg));
    }
  }  
  if (fCutXV0) {
    printf("                 x-position > %g\n",fMinXV0);
  }
  if (fCutYV0) {
    printf("                 y-position > %g\n",fMinYV0);
  }                                                                             
  if (fCutZV0) {
    printf("                 z-position > %g\n",fMinZV0);
  }
  if (fCutArmenterosPt) {
    printf("       Armenteros-Pt window = %g - %g\n",fMinArmenterosPt,fMaxArmenterosPt);
  }
  if (fCutArmenterosAlpha) {
    printf("    Armenteros-alpha window = %g - %g\n",fMinArmenterosAlpha,fMaxArmenterosAlpha);
  }
  if (fCutPchi2) {
    printf("                Fit-P(chi2) > %g\n",fMinPchi2);
  }
  if (fCutIflag) {
    printf("                      IFLAG = %d\n",fValIflag);
  }
  if (fCutCosTheta) {
    if (fUseInvMassCosTheta) {
      printf("    Cos theta (minv) window = %g - %g\n",fMinCosTheta,fMaxCosTheta);
    }
    else {
      printf("   Cos theta (const) window = %g - %g\n",fMinCosTheta,fMaxCosTheta);
    }
  }
  if (fCutLifetime) {
    printf("          Tau / Tau0 window = %g - %g\n",fMinLifetime,fMaxLifetime);
  }
  if (fCutMass) {
    switch (fV0TypeMass) {
    case kLambda:
      printf("      Lambda invariant mass = %g, %g\n",fMass,fMassWindow);
      break;
    case kAntiLambda:
      printf(" Anti-lambda invariant mass = %g, %g\n",fMass,fMassWindow);
      break;
    case kK0s:
      printf("         K0s invariant mass = %g, %g\n",fMass,fMassWindow);
      break;
    };    
  }
  if (fCutExcludedMass) {
    switch (fV0TypeExcluded) {
    case kLambda:
      printf("      Lambda invariant mass = %g, %g (excluded)\n",fExcludedMass
                                                                ,fExcludedMassWindow);
      break;
    case kAntiLambda:
      printf(" Anti-lambda invariant mass = %g, %g (excluded)\n",fExcludedMass
                                                                ,fExcludedMassWindow);
      break;
    case kK0s:
      printf("         K0s invariant mass = %g, %g (excluded)\n",fExcludedMass
                                                                ,fExcludedMassWindow);
      break;
    };    
  }
  if (fCutCrossYZ) {
    printf("         CrossYZ z-position > %g\n",fMinCrossYZ);
  }
  if (fCutPt) {
    printf("                         Pt > %g\n",fMinPt);
  }
  if (fCutRapidity) {
    printf("            Rapidity window = %g - %g\n",fMinRapidity,fMaxRapidity);
  }
  if (fCutOutsideTPC) {
    printf("       Select only V0s that decay outside of the VTPCs\n");
  }
  if (fCutPsi) {
    printf("                  Psi-angle > %g\n",fMinPsi);
  }
  if (fCutPosTrack) {
    printf("       Apply single tracks cuts on positive daughter\n");
  }
  if (fCutNegTrack) {
    printf("       Apply single tracks cuts on negative daughter\n");
  }
  if (fCutSameSide) {
    printf("       Apply same side cut on the daughters\n");
  }                                                                             
  printf("\n");

}

//______________________________________________________________________________
void T49CutV0::ResetStatistics() 
{

  fCntInV0            = 0;
  fCntOutV0           = 0;
  fCntGSIBham         = 0;
  fCntX1minX2         = 0;
  fCntY1minY2         = 0;
  fCntXTarg           = 0;
  fCntYTarg           = 0;
  fCntXV0             = 0;
  fCntYV0             = 0;                                                      
  fCntZV0             = 0;
  fCntArmenterosPt    = 0;
  fCntArmenterosAlpha = 0;
  fCntPchi2           = 0;
  fCntIflag           = 0;  
  fCntCosTheta        = 0;
  fCntLifetime        = 0;
  fCntMass            = 0;
  fCntExcludedMass    = 0;
  fCntCrossYZ         = 0;
  fCntPt              = 0;
  fCntRapidity        = 0;
  fCntOutsideTPC      = 0;
  fCntPsi             = 0;
  fCntPosTrack        = 0;
  fCntNegTrack        = 0;

}

//______________________________________________________________________________
void T49CutV0::Reset() 
{

  fFast               = kFALSE;

  fMinX1minX2         = 0;
  fMinY1minY2         = 0;
  fMaxXTarg           = 0;
  fMaxYTarg           = 0;
  fMinXV0             = 0;
  fMinYV0             = 0;                                                      
  fMinZV0             = 0;
  fMinArmenterosPt    = 0;
  fMaxArmenterosAlpha = 0;
  fMinArmenterosPt    = 0;
  fMaxArmenterosAlpha = 0;
  fMinPchi2           = 0;
  fValIflag           = 0;
  fMass               = 0;
  fMassWindow         = 0;
  fExcludedMass       = 0;
  fExcludedMassWindow = 0;
  fMinCosTheta        = 0;
  fMaxCosTheta        = 0;
  fMinLifetime        = 0;
  fMaxLifetime        = 0;
  fMinCrossYZ         = 0;
  fMinPt              = 0;
  fMinRapidity        = 0;
  fMaxRapidity        = 0;
  fMinPsi             = 0;

  fPosTrackCuts       = NULL;
  fNegTrackCuts       = NULL;

  fV0TypeMass         = 0;
  fV0TypeCosTheta     = 0;
  fV0TypeLifetime     = 0;
  fV0TypeExcluded     = 0;
  fV0TypeRapidity     = 0;

  fUseInvMassCosTheta = kFALSE;

  fCutX1minX2         = kFALSE;
  fCutY1minY2         = kFALSE;
  fCutXTarg           = kFALSE;
  fCutYTarg           = kFALSE;
  fCutXV0             = kFALSE;
  fCutYV0             = kFALSE;
  fCutZV0             = kFALSE;
  fCutArmenterosPt    = kFALSE;
  fCutArmenterosAlpha = kFALSE;
  fCutPchi2           = kFALSE;
  fCutIflag           = kFALSE;
  fCutCosTheta        = kFALSE;
  fCutLifetime        = kFALSE;
  fCutMass            = kFALSE;
  fCutExcludedMass    = kFALSE;
  fCutCrossYZ         = kFALSE;
  fCutPt              = kFALSE;
  fCutRapidity        = kFALSE;
  fCutOutsideTPC      = kFALSE;
  fCutPsi             = kFALSE;
  fCutPosTrack        = kFALSE;
  fCutNegTrack        = kFALSE;
  fCutSameSide        = kFALSE;

  ResetStatistics();

}

//______________________________________________________________________________
void T49CutV0::PrintStatistics() 
{
  //
  // Prints the cut statistic
  //

  Float_t x;

  printf("\n");
  printf(" V0-Cut (%s) statistics (%%):\n\n",this->GetTitle());
  if (fFast)
    printf(" Fast option: Cut statistics is not independent!\n");
  x = (Float_t) fCntOutV0 / (Float_t) fCntInV0 * 100.;
  printf("            Accepted V0s: %g\n\n",x);
  if (fCutGSIBham) {
    x = (Float_t) (fCntInV0 - fCntGSIBham)      
      / (Float_t)  fCntInV0 * 100.;
    printf("                   GSI-Bham: %g\n",x);
  }
  if (fCutX1minX2) {
    x = (Float_t) (fCntInV0 - fCntX1minX2)      
      / (Float_t)  fCntInV0 * 100.;
    printf("                    X1minX2: %g\n",x);
  }
  if (fCutY1minY2) {
    x = (Float_t) (fCntInV0 - fCntY1minY2)      
      / (Float_t)  fCntInV0 * 100.;
    printf("                    Y1minY2: %g\n",x);
  }
  if (fCutXTarg) {
    x = (Float_t) (fCntInV0 - fCntXTarg)         
      / (Float_t)  fCntInV0 * 100.;
    printf("       x-position at target: %g\n",x);
  }  
  if (fCutYTarg) {
    x = (Float_t) (fCntInV0 - fCntYTarg)       
      / (Float_t)  fCntInV0 * 100.;
    printf("       y-position at target: %g\n",x);
  }  
  if (fCutXV0) {
    x = (Float_t) (fCntInV0 - fCntXV0)
      / (Float_t)  fCntInV0 * 100.;
    printf("                 x-position: %g\n",x);
  }
  if (fCutYV0) {
    x = (Float_t) (fCntInV0 - fCntYV0)
      / (Float_t)  fCntInV0 * 100.;
    printf("                 y-position: %g\n",x);
  }                                                                             
  if (fCutZV0) {
    x = (Float_t) (fCntInV0 - fCntZV0)          
      / (Float_t)  fCntInV0 * 100.;
    printf("                 z-position: %g\n",x);
  }
  if (fCutArmenterosPt) {
    x = (Float_t) (fCntInV0 - fCntArmenterosPt) 
      / (Float_t)  fCntInV0 * 100.;
    printf("              Armenteros-Pt: %g\n",x);
  }
  if (fCutArmenterosAlpha) {
    x = (Float_t) (fCntInV0 - fCntArmenterosAlpha) 
      / (Float_t)  fCntInV0 * 100.;
    printf("           Armenteros-Alpha: %g\n",x);
  }
  if (fCutPchi2) {
    x = (Float_t) (fCntInV0 - fCntPchi2)         
      / (Float_t)  fCntInV0 * 100.;
    printf("                Fit-P(chi2): %g\n",x);
  }
  if (fCutIflag) {
    x = (Float_t) (fCntInV0 - fCntIflag)        
      / (Float_t)  fCntInV0 * 100.;
    printf("                      IFLAG: %g\n",x);
  }
  if (fCutCosTheta) {
    x = (Float_t) (fCntInV0 - fCntCosTheta)        
      / (Float_t)  fCntInV0 * 100.;
    printf("                  Cos theta: %g\n",x);
  }
  if (fCutLifetime) {
    x = (Float_t) (fCntInV0 - fCntLifetime)        
      / (Float_t)  fCntInV0 * 100.;
    printf("                   Lifetime: %g\n",x);
  }
  if (fCutMass) {
    x = (Float_t) (fCntInV0 - fCntMass)        
      / (Float_t)  fCntInV0 * 100.;
    switch (fV0TypeMass) {
    case kLambda:
      printf("      Lambda invariant mass: %g\n",x);
      break;
    case kAntiLambda:
      printf(" Anti-lambda invariant mass: %g\n",x);
      break;
    case kK0s:
      printf("         K0s invariant mass: %g\n",x);
      break;
    };    
  }
  if (fCutExcludedMass) {
    x = (Float_t) (fCntInV0 - fCntExcludedMass)        
      / (Float_t)  fCntInV0 * 100.;
    switch (fV0TypeExcluded) {
    case kLambda:
      printf("      Lambda invariant mass: %g (excluded)\n",x);
      break;
    case kAntiLambda:
      printf(" Anti-lambda invariant mass: %g (excluded)\n",x);
      break;
    case kK0s:
      printf("         K0s invariant mass: %g (excluded)\n",x);
      break;
    };    
  }
  if (fCutCrossYZ) {
    x = (Float_t) (fCntInV0 - fCntCrossYZ)        
      / (Float_t)  fCntInV0 * 100.;
    printf("                    CrossYZ: %g\n",x);
  }
  if (fCutPt) {
    x = (Float_t) (fCntInV0 - fCntPt)        
      / (Float_t)  fCntInV0 * 100.;
    printf("                         Pt: %g\n",x);
  }
  if (fCutRapidity) {
    x = (Float_t) (fCntInV0 - fCntRapidity)        
      / (Float_t)  fCntInV0 * 100.;
    printf("                   Rapidity: %g\n",x);
  }
  if (fCutOutsideTPC) {
    x = (Float_t) (fCntInV0 - fCntOutsideTPC)
      / (Float_t)  fCntInV0 * 100.;
    printf("                Outside TPC: %g\n",x);
  }
  if (fCutPsi) {
    x = (Float_t) (fCntInV0 - fCntPsi)
      / (Float_t)  fCntInV0 * 100.;
    printf("                        Psi: %g\n",x);
  }
  if (fCutPosTrack) {
    x = (Float_t) (fCntInV0 - fCntPosTrack)
      / (Float_t)  fCntInV0 * 100.;
    printf("  Cuts on positive daughter: %g\n",x);
  }
  if (fCutNegTrack) {
    x = (Float_t) (fCntInV0 - fCntNegTrack)
      / (Float_t)  fCntInV0 * 100.;
    printf("  Cuts on negative daughter: %g\n",x);
  }
  if (fCutSameSide) {
    x = (Float_t) (fCntInV0 - fCntSameSide)
      / (Float_t)  fCntInV0 * 100.;
    printf("              Same side cut: %g\n",x);
  }                                                                             

  printf("\n");

}

//______________________________________________________________________________
void T49CutV0::Streamer(TBuffer &R__b)
{
  //
  // Stream an object of class T49CutV0.
  // 

  Float_t minRpoint       = 0;
  Float_t minDEDXpositive = 0;
  Float_t maxDEDXpositive = 0;
  Float_t minDEDXnegative = 0;
  Float_t maxDEDXnegative = 0;
  Float_t minPotPoint     = 0;
  Float_t minDaughterMom  = 0;
  Float_t v0TypeDEDX      = 0;
  Float_t cutRpoint       = 0;
  Float_t cutDEDX         = 0;
  Float_t cutPotPoint     = 0;
  Float_t cutDaughterMom  = 0;
  Float_t cntRpoint       = 0;
  Float_t cntDEDX         = 0;
  Float_t cntPotPoint     = 0;
  Float_t cntDaughterMom  = 0;

  UInt_t R__s, R__c;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v > 4) {
      T49CutV0::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
    }
    T49Cut::Streamer(R__b);
    R__b >> fFast;
    R__b >> fMinX1minX2;
    R__b >> fMinY1minY2;
    R__b >> fMaxXTarg;
    R__b >> fMaxYTarg;
    R__b >> fMinZV0;
    R__b >> fMinArmenterosPt;
    R__b >> fMaxArmenterosPt;
    R__b >> fMinArmenterosAlpha;
    R__b >> fMaxArmenterosAlpha;
    R__b >> fMinPchi2;
    R__b >> fValIflag;
    R__b >> fMass;
    R__b >> fMassWindow;
    R__b >> fExcludedMass;
    R__b >> fExcludedMassWindow;
    R__b >> fMinCosTheta;
    R__b >> fMaxCosTheta;
    R__b >> fMinLifetime;
    R__b >> fMaxLifetime;
    if (R__v < 4) {
      R__b >> minRpoint;
    }
    R__b >> fValGSIBham;
    R__b >> fMinCrossYZ;
    R__b >> fMinPt;
    R__b >> fMinRapidity;
    R__b >> fMaxRapidity;
    if (R__v == 3) {
      R__b >> minDEDXpositive;
      R__b >> maxDEDXpositive;
      R__b >> minDEDXnegative;
      R__b >> maxDEDXnegative;
    }
    if (R__v > 2) {
      R__b >> fMinPsi;
    }
    if (R__v == 3) { 
      R__b >> minPotPoint;
      R__b >> minDaughterMom;
    }
    if (R__v > 3) {
      R__b >> fPosTrackCuts;
      R__b >> fNegTrackCuts;
    }
    R__b >> fV0TypeMass;
    R__b >> fV0TypeCosTheta;
    R__b >> fV0TypeLifetime;
    R__b >> fV0TypeExcluded;
    R__b >> fV0TypeRapidity;
    if (R__v == 3) {
      R__b >> v0TypeDEDX;
    }
    R__b >> fCutX1minX2;
    R__b >> fCutY1minY2;
    R__b >> fCutXTarg;
    R__b >> fCutYTarg;
    R__b >> fCutZV0;
    R__b >> fCutArmenterosPt;                                                 
    R__b >> fCutArmenterosAlpha;
    R__b >> fCutPchi2;
    R__b >> fCutIflag;
    R__b >> fCutMass;
    R__b >> fCutExcludedMass;
    R__b >> fCutCosTheta;
    R__b >> fCutLifetime;
    if (R__v < 4) {
      R__b >> cutRpoint;
    }
    R__b >> fCutGSIBham;
    R__b >> fCutCrossYZ;
    R__b >> fCutPt;
    R__b >> fCutRapidity;
    if (R__v == 3) {
      R__b >> cutDEDX;
    }
    if (R__v > 2) {
      R__b >> fCutOutsideTPC;
      R__b >> fCutPsi;
    }
    if (R__v == 3) {
      R__b >> cutPotPoint;
      R__b >> cutDaughterMom;
    }
    if (R__v > 3) {
      R__b >> fCutPosTrack;
      R__b >> fCutNegTrack;
    }
    R__b >> fCntInV0;
    R__b >> fCntOutV0;
    R__b >> fCntX1minX2;
    R__b >> fCntY1minY2;
    R__b >> fCntXTarg;
    R__b >> fCntYTarg;
    R__b >> fCntZV0;
    R__b >> fCntArmenterosPt;
    R__b >> fCntArmenterosAlpha;
    R__b >> fCntPchi2;
    R__b >> fCntIflag;
    R__b >> fCntMass;
    R__b >> fCntExcludedMass;
    R__b >> fCntCosTheta;
    R__b >> fCntLifetime;
    if (R__v < 4) {
      R__b >> cntRpoint;
    }
    R__b >> fCntGSIBham;
    R__b >> fCntCrossYZ;
    R__b >> fCntPt;
    R__b >> fCntRapidity;
    if (R__v == 3) {
      R__b >> cntDEDX;
    }
    if (R__v > 2) { 
      R__b >> fCntOutsideTPC;
      R__b >> fCntPsi;
    }
    if (R__v == 3) {
      R__b >> cntPotPoint;
      R__b >> cntDaughterMom;
    }
    if (R__v > 3) {
      R__b >> fCntPosTrack;
      R__b >> fCntNegTrack;
    }
    R__b.CheckByteCount(R__s, R__c, T49CutV0::IsA());  
  } 
  else {
    T49CutV0::Class()->WriteBuffer(R__b,this);  
  }

}
                                                                                


