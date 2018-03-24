///////////////////////////////////////////////////////////////////////
//                                                                   //
//    T49VertexRoot                                                  //
//    Contains the vertex information                                //
//                                                                   //
//    Author  : Christian Bormann (bormann@ikf.uni-frankfurt.de)     //
//    Created : 05/11/95                                             //
//    Version : 1.0                                                  //
//    Changed : 19/10/99 Contains now additional information and     //
//                       functions for V0-candidates (C.Blume)       //
//              08/06/01 Changes for Xi and Omega (C.Blume)          //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include <cmath>

#include "TClass.h"

#include "T49ParticleRoot.h"
#include "T49VertexRoot.h"

ClassImp(T49VertexRoot)

/*
$Log: T49VertexRoot.C,v $
Revision 1.16  2008/01/04 10:48:59  cblume
64bit modifications by Marek Szuba

Revision 1.15  2002/03/27 14:03:08  cblume
Add GetSameSide() and GetSameSideXi()

Revision 1.14  2001/12/12 17:39:17  cblume
Update default constructors

Revision 1.13  2001/11/12 10:52:38  cblume
Update for ROOT v3.01

 * Revision 1.12  2001/06/13  08:14:18  cblume
 * Modification to include also Xi vertices
 *
 * Revision 1.11  2000/11/24  12:41:14  cblume
 * Error checking in GetY1minY2() and GetCrossYZ(). Use GetY0() and GetYPrime()
 *
 * Revision 1.10  2000/11/20  14:02:39  cblume
 * Reconstruct number of global points in GetFullTrack()
 *
 * Revision 1.9  2000/11/09  09:08:09  cblume
 * Introduce the byte count in the streamer
 *
 * Revision 1.8  2000/10/15  01:08:28  cblume
 * Fix bug in GetFullTrack()
 *
 * Revision 1.7  2000/08/28  08:32:45  cblume
 * Changes in the access of rtrack info. Moved some inline functions to implementation file
 *
 * Revision 1.6  2000/05/19  15:41:21  cblume
 * Added GetY1minY2() and GetCrossYZ()
 *
 * Revision 1.5  2000/04/18  17:26:41  cblume
 * Part of the member functions are now inline. Pointer reconstruction via gT49. Removed fZTarg
 *
 * Revision 1.4  2000/04/13  15:12:36  cblume
 * Removed fCovar and fKfitList
 *
 * Revision 1.3  1999/11/23  12:59:06  cblume
 * Remove files
 *
*/

//______________________________________________________________________________
T49VertexRoot::T49VertexRoot(Bool_t allocObjects)
{
  //
  // T49VertexRoot default constructor
  //

  fIflag                =  0;
  fIdDet                =  0;
  fIdVtx                =  0;
  fNTrkFit              =  0;
  fNTrkOut              =  0;
  fX                    =  0;
  fY                    =  0;
  fZ                    =  0;
  fSigX                 =  0;
  fSigY                 =  0;
  fSigZ                 =  0;
  fPchi2                =  0;
  fFirstTrack           =  NULL;
  fSecondTrack          =  NULL;
  fFirstTrackIsPositive =  kFALSE;
  fFirstPriIndex        = -1;
  fSecondPriIndex       = -1;
  fFirstSecIndex        = -1;
  fSecondSecIndex       = -1;
  fXTarg                =  0;
  fYTarg                =  0;
  fX1minX2              =  0;
  fPlen                 =  0;

  if (allocObjects) {
    printf("T49VertexRoot::Ctor - No allocated objects any more\n");
  }

}

//______________________________________________________________________________
T49VertexRoot::T49VertexRoot(T49VertexRoot& Vertex)
{
  //
  // T49VertexRoot copy constructor
  //

  fIflag = Vertex.GetIflag();
  fIdDet = Vertex.GetIdDet();
  fIdVtx = Vertex.GetIdVtx();
  fX     = Vertex.GetX();
  fY     = Vertex.GetY();
  fZ     = Vertex.GetZ();
  fPchi2 = Vertex.GetPchi2();

  fFirstTrackIsPositive = Vertex.GetFirstTrackIsPositive();

  if (Vertex.GetFirstTrack())
    fFirstTrack  = Vertex.GetFirstTrack();
  else
    fFirstTrack  = NULL;
  if (Vertex.GetSecondTrack())
    fSecondTrack = Vertex.GetSecondTrack();
  else
    fSecondTrack = NULL;

  fFirstPriIndex  = Vertex.GetPrimaryIndex(0);
  fSecondPriIndex = Vertex.GetPrimaryIndex(1);
  fFirstSecIndex  = Vertex.GetSecondaryIndex(0);
  fSecondSecIndex = Vertex.GetSecondaryIndex(1);
  fXTarg          = Vertex.GetXTarg();
  fYTarg          = Vertex.GetYTarg();
  fX1minX2        = Vertex.GetX1minX2();

  fPlen           = Vertex.GetPathlengthXi();

}

//______________________________________________________________________________
T49VertexRoot::~T49VertexRoot()
{
  //
  // T49VertexRoot destructor
  //

}

//______________________________________________________________________________
void T49VertexRoot::SetPrimaryIndex(Int_t track, Int_t index)
{
  //
  // Set the index of the main vertex track in the primary particle array
  //

  if      (track == 0)
    fFirstPriIndex  = index;
  else if (track == 1)
    fSecondPriIndex = index;
  else
    fprintf(stderr,"T49VertexRoot::SetPrimaryIndex - track out of range (%d)\n"
           ,track);

}

//______________________________________________________________________________
void T49VertexRoot::SetSecondaryIndex(Int_t track, Int_t index)
{
  //
  // Set the index of the V0 track in the secondary particle array
  //

  if      (track == 0)
    fFirstSecIndex  = index;
  else if (track == 1)
    fSecondSecIndex = index;
  else
    fprintf(stderr,"T49VertexRoot::SetSecondaryIndex - track out of range (%d)\n"
           ,track);

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetCosThetaLambda(Bool_t useInvMass)
{
  //
  // Calculate cos(theta*) between the lambda candidate and the decay proton.
  // Uses the invariant mass instead of a constant lambda mass if
  // <useInvMass> = kTRUE
  //

  Float_t pV0[4];
  Float_t pPD[4];

  pV0[0] = GetPx();
  pV0[1] = GetPy();
  pV0[2] = GetPz();
  if (useInvMass) {
    pV0[3] = GetEnergy(GetInvariantMassLambda());
  }
  else {
    pV0[3] = GetEnergy(kLambdaMass);
  }

  pPD[0] = GetPositiveTrack()->GetPx();
  pPD[1] = GetPositiveTrack()->GetPy();
  pPD[2] = GetPositiveTrack()->GetPz();
  pPD[3] = GetPositiveTrack()->GetE(kProtonMass);

  return CosTheta(pV0,pPD);

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetCosThetaAntiLambda(Bool_t useInvMass)
{
  //
  // Calculate cos(theta*) between the anti-lambda candidate and the decay pion
  // Uses the invariant mass instead of a constant lambda mass if
  // <useInvMass> = kTRUE
  //

  Float_t pV0[4];
  Float_t pPD[4];

  pV0[0] = GetPx();
  pV0[1] = GetPy();
  pV0[2] = GetPz();
  if (useInvMass) {
    pV0[3] = GetEnergy(GetInvariantMassAntiLambda());
  }
  else {
    pV0[3] = GetEnergy(kLambdaMass);
  }

  pPD[0] = GetPositiveTrack()->GetPx();
  pPD[1] = GetPositiveTrack()->GetPy();
  pPD[2] = GetPositiveTrack()->GetPz();
  pPD[3] = GetPositiveTrack()->GetE(kPionMass);

  return CosTheta(pV0,pPD);

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetCosThetaK0s(Bool_t useInvMass)
{
  //
  // Calculate cos(theta*) between the K0short candidate and the decay-pi+
  // Uses the invariant mass instead of a constant K0short mass if
  // <useInvMass> = kTRUE
  //

  Float_t pV0[4];
  Float_t pPD[4];

  pV0[0] = GetPx();
  pV0[1] = GetPy();
  pV0[2] = GetPz();
  if (useInvMass) {
    pV0[3] = GetEnergy(GetInvariantMassK0s());
  }
  else {
    pV0[3] = GetEnergy(kK0sMass);
  }

  pPD[0] = GetPositiveTrack()->GetPx();
  pPD[1] = GetPositiveTrack()->GetPy();
  pPD[2] = GetPositiveTrack()->GetPz();
  pPD[3] = GetPositiveTrack()->GetE(kPionMass);

  return CosTheta(pV0,pPD);

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetCosThetaXi(Bool_t useInvMass)
{
  //
  // Calculate cos(theta*) between the the Xi candidate and the most
  // positive daughter.
  // Uses the invariant mass instead of a constant Xi mass if
  // <useInvMass> = kTRUE
  //

  Float_t pV0[4];
  Float_t pPD[4];

  Float_t daughterMass;

  T49ParticleRoot *Daughter1;
  T49ParticleRoot *Charged = GetChargedXiTrack();
  T49ParticleRoot *Neutral = GetNeutralXiTrack();

  if (Charged->GetCharge() > 0) {
    Daughter1    = Charged;
    daughterMass = kPionMass;
  }
  else {
    Daughter1    = Neutral;
    daughterMass = kLambdaMass;
  }

  pV0[0] = Charged->GetPx() + Neutral->GetPx();
  pV0[1] = Charged->GetPy() + Neutral->GetPy();
  pV0[2] = Charged->GetPz() + Neutral->GetPz();
  if (useInvMass) {
    Float_t invMass = GetInvariantMassXi();
    pV0[3] = TMath::Sqrt(pV0[0]*pV0[0]
                       + pV0[1]*pV0[1]
                       + pV0[2]*pV0[2]
                       + invMass*invMass);
  }
  else {
    pV0[3] = GetEnergyXi();
  }

  pPD[0] = Daughter1->GetPx();
  pPD[1] = Daughter1->GetPy();
  pPD[2] = Daughter1->GetPz();
  pPD[3] = Daughter1->GetE(daughterMass);

  return CosTheta(pV0,pPD);

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetCosThetaOmega(Bool_t useInvMass)
{
  //
  // Calculate cos(theta*) between the Omega candidate and the most
  // positive daughter.
  // Uses the invariant mass instead of a constant Omega mass if
  // <useInvMass> = kTRUE
  //

  Float_t pV0[4];
  Float_t pPD[4];

  Float_t daughterMass;

  T49ParticleRoot *Daughter1;
  T49ParticleRoot *Charged = GetChargedXiTrack();
  T49ParticleRoot *Neutral = GetNeutralXiTrack();

  if (Charged->GetCharge() > 0) {
    Daughter1    = Charged;
    daughterMass = kKaonMass;
  }
  else {
    Daughter1    = Neutral;
    daughterMass = kLambdaMass;
  }

  pV0[0] = Charged->GetPx() + Neutral->GetPx();
  pV0[1] = Charged->GetPy() + Neutral->GetPy();
  pV0[2] = Charged->GetPz() + Neutral->GetPz();
  if (useInvMass) {
    Float_t invMass = GetInvariantMassOmega();
    pV0[3] = TMath::Sqrt(pV0[0]*pV0[0]
                       + pV0[1]*pV0[1]
                       + pV0[2]*pV0[2]
                       + invMass*invMass);
  }
  else {
    pV0[3] = GetEnergyOmega();
  }

  pPD[0] = Daughter1->GetPx();
  pPD[1] = Daughter1->GetPy();
  pPD[2] = Daughter1->GetPz();
  pPD[3] = Daughter1->GetE(daughterMass);

  return CosTheta(pV0,pPD);

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetLifetimeLambda(Float_t x, Float_t y, Float_t z)
{
  //
  // Calculate the lifetime of a (anti-)lambda-candidate with
  // production vertex <x>,<y>,<z>
  //

  Float_t distanceX = x - GetX();
  Float_t distanceY = y - GetY();
  Float_t distanceZ = z - GetZ();
  Float_t distance  = TMath::Sqrt(distanceX*distanceX
                                + distanceY*distanceY
                                + distanceZ*distanceZ);

  Float_t xxx = GetBetaLambda() * GetGammaLambda();
  if (xxx > 0)
    return (distance / (xxx * kCLight));
  else
    return -1;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetLifetimeLambda()
{
  //
  // Calculate the lifetime of a (anti-)lambda-candidate
  //

  if (!gT49) return -1;

  T49EventRoot  *Event   = (T49EventRoot *)  gT49->GetCurrentEvent();
  T49VertexRoot *MainVtx = (T49VertexRoot *) Event->GetPrimaryVertex();

  return GetLifetimeLambda(MainVtx->GetX(),MainVtx->GetY(),MainVtx->GetZ());

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetLifetimeK0s()
{
  //
  // Calculate the lifetime of a K0short
  //

  if (!gT49) return -1;

  T49EventRoot  *Event   = (T49EventRoot *)  gT49->GetCurrentEvent();
  T49VertexRoot *MainVtx = (T49VertexRoot *) Event->GetPrimaryVertex();

  Float_t distanceX = MainVtx->GetX() - GetX();
  Float_t distanceY = MainVtx->GetY() - GetY();
  Float_t distanceZ = MainVtx->GetZ() - GetZ();
  Float_t distance  = TMath::Sqrt(distanceX*distanceX
                                + distanceY*distanceY
                                + distanceZ*distanceZ);

  Float_t xxx = GetBetaK0s() * GetGammaK0s();
  if (xxx > 0)
    return (distance / (xxx * kCLight));
  else
    return -1;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetLifetimeXi()
{
  //
  // Calculate the lifetime of a Xi
  //

  Float_t distance = GetPathlengthXi();
  Float_t xxx      = GetBetaXi() * GetGammaXi();
  if (xxx > 0)
    return (distance / (xxx * kCLight));
  else
    return -1;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetLifetimeOmega()
{
  //
  // Calculate the lifetime of a Omega
  //

  Float_t distance = GetPathlengthXi();
  Float_t xxx      = GetBetaOmega() * GetGammaOmega();
  if (xxx > 0)
    return (distance / (xxx * kCLight));
  else
    return -1;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetInvariantMassLambda()
{
  //
  // The invariant mass assuming the V0 is a lambda.
  //

  Float_t pPos = GetPositiveTrack()->GetP();
  Float_t pNeg = GetNegativeTrack()->GetP();

  Float_t eprp = sqrt(kProtonMassSq + pPos*pPos);
  Float_t epin = sqrt(kPionMassSq   + pNeg*pNeg);

  Float_t massSquared = (kPionMassSq + kProtonMassSq +
                         2*(eprp*epin -
                            GetPositiveTrack()->GetPx()*GetNegativeTrack()->GetPx() -
                            GetPositiveTrack()->GetPy()*GetNegativeTrack()->GetPy() -
                            GetPositiveTrack()->GetPz()*GetNegativeTrack()->GetPz()));

  if (massSquared > 0)
    return sqrt(massSquared);
  else
    return 0;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetInvariantMassAntiLambda()
{
  //
  // The invariant mass assuming the V0 is an anti-lambda.
  //

  Float_t pPos = GetPositiveTrack()->GetP();
  Float_t pNeg = GetNegativeTrack()->GetP();

  Float_t eprn = sqrt(kProtonMassSq + pNeg*pNeg);
  Float_t epip = sqrt(kPionMassSq   + pPos*pPos);

  Float_t massSquared = (kPionMassSq + kProtonMassSq +
                         2*(eprn*epip -
                            GetPositiveTrack()->GetPx()*GetNegativeTrack()->GetPx() -
                            GetPositiveTrack()->GetPy()*GetNegativeTrack()->GetPy() -
                            GetPositiveTrack()->GetPz()*GetNegativeTrack()->GetPz()));

  if (massSquared > 0)
    return sqrt(massSquared);
  else
    return 0;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetInvariantMassK0s()
{
  //
  // The invariant mass assuming the V0 is a K0s.
  //

  Float_t pPos = GetPositiveTrack()->GetP();
  Float_t pNeg = GetNegativeTrack()->GetP();

  Float_t epip = sqrt(kPionMassSq + pPos*pPos);
  Float_t epin = sqrt(kPionMassSq + pNeg*pNeg);

  Float_t massSquared = (kPionMassSq + kPionMassSq +
                         2*(epin*epip -
                            GetPositiveTrack()->GetPx()*GetNegativeTrack()->GetPx() -
                            GetPositiveTrack()->GetPy()*GetNegativeTrack()->GetPy() -
                            GetPositiveTrack()->GetPz()*GetNegativeTrack()->GetPz()));

  if (massSquared > 0)
    return sqrt(massSquared);
  else
    return 0;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetInvariantMassXi()
{
  //
  // The invariant mass assuming the Xi hypothesis.
  //

  Float_t pCha = GetChargedXiTrack()->GetP();
  Float_t pNeu = GetNeutralXiTrack()->GetP();

  Float_t epic = sqrt(kPionMassSq   + pCha*pCha);
  Float_t epin = sqrt(kLambdaMassSq + pNeu*pNeu);

  Float_t massSquared = (kPionMassSq + kLambdaMassSq +
                         2*(epic*epin -
                            GetChargedXiTrack()->GetPx()*GetNeutralXiTrack()->GetPx() -
                            GetChargedXiTrack()->GetPy()*GetNeutralXiTrack()->GetPy() -
                            GetChargedXiTrack()->GetPz()*GetNeutralXiTrack()->GetPz()));

  if (massSquared > 0)
    return sqrt(massSquared);
  else
    return 0;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetInvariantMassOmega()
{
  //
  // The invariant mass assuming the Omega hypothesis.
  //

  Float_t pCha = GetChargedXiTrack()->GetP();
  Float_t pNeu = GetNeutralXiTrack()->GetP();

  Float_t epic = sqrt(kKaonMassSq   + pCha*pCha);
  Float_t epin = sqrt(kLambdaMassSq + pNeu*pNeu);

  Float_t massSquared = (kKaonMassSq + kLambdaMassSq +
                         2*(epic*epin -
                            GetChargedXiTrack()->GetPx()*GetNeutralXiTrack()->GetPx() -
                            GetChargedXiTrack()->GetPy()*GetNeutralXiTrack()->GetPy() -
                            GetChargedXiTrack()->GetPz()*GetNeutralXiTrack()->GetPz()));

  if (massSquared > 0)
    return sqrt(massSquared);
  else
    return 0;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetArmenterosPt()
{
  //
  // The Armenteros transverse momentum
  //

  if (GetP() == 0) return 999.0;

  Float_t pppar = ( GetPx()*GetPositiveTrack()->GetPx() +
                    GetPy()*GetPositiveTrack()->GetPy() +
                    GetPz()*GetPositiveTrack()->GetPz() ) / GetP();
  Float_t posP  = GetPositiveTrack()->GetP();
  Float_t ptSq  = posP*posP - pppar*pppar;

  if (ptSq > 0)
    return sqrt(ptSq);
  else
    return 999.0;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetArmenterosPtXi()
{
  //
  // The Armenteros transverse momentum for Xi / Omega candidates
  //

  if (GetPXi() == 0) return 999.0;

  T49ParticleRoot *Daughter1;
  T49ParticleRoot *Charged = GetChargedXiTrack();
  T49ParticleRoot *Neutral = GetNeutralXiTrack();

  if (Charged->GetCharge() > 0) {
    Daughter1 = Charged;
  }
  else {
    Daughter1 = Neutral;
  }

  Float_t pXix = Charged->GetPx() + Neutral->GetPx();
  Float_t pXiy = Charged->GetPy() + Neutral->GetPy();
  Float_t pXiz = Charged->GetPz() + Neutral->GetPz();
  Float_t pXi  = TMath::Sqrt(pXix*pXix + pXiy*pXiy + pXiz*pXiz);

  Float_t pppar = ( pXix*Daughter1->GetPx() +
                    pXiy*Daughter1->GetPy() +
                    pXiz*Daughter1->GetPz() ) / pXi;
  Float_t posP  = Daughter1->GetP();
  Float_t ptSq  = posP*posP - pppar*pppar;

  if (ptSq > 0)
    return sqrt(ptSq);
  else
    return 999.0;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetArmenterosAlpha()
{
  //
  // The Armenteros alpha-parameter
  //

  if (GetP() == 0) return 999.0;

  Float_t pppar = ( GetPx()*GetPositiveTrack()->GetPx() +
                    GetPy()*GetPositiveTrack()->GetPy() +
                    GetPz()*GetPositiveTrack()->GetPz() ) / GetP();
  Float_t pnpar = ( GetPx()*GetNegativeTrack()->GetPx() +
                    GetPy()*GetNegativeTrack()->GetPy() +
                    GetPz()*GetNegativeTrack()->GetPz() ) / GetP();

  if ((pppar + pnpar) != 0)
    return ((pppar - pnpar) / (pppar + pnpar));
  else
    return 999.0;

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetArmenterosAlphaXi()
{
  //
  // The Armenteros alpha-parameter for Xi / Omega candidates
  //

  if (GetPXi() == 0) return 999.0;

  T49ParticleRoot *Daughter1;
  T49ParticleRoot *Daughter2;
  T49ParticleRoot *Charged = GetChargedXiTrack();
  T49ParticleRoot *Neutral = GetNeutralXiTrack();

  if (Charged->GetCharge() > 0) {
    Daughter1 = Charged;
    Daughter2 = Neutral;
  }
  else {
    Daughter1 = Neutral;
    Daughter2 = Charged;
  }

  Float_t pXix = Charged->GetPx() + Neutral->GetPx();
  Float_t pXiy = Charged->GetPy() + Neutral->GetPy();
  Float_t pXiz = Charged->GetPz() + Neutral->GetPz();
  Float_t pXi  = TMath::Sqrt(pXix*pXix + pXiy*pXiy + pXiz*pXiz);

  Float_t pppar = ( pXix*Daughter1->GetPx() +
                    pXiy*Daughter1->GetPy() +
                    pXiz*Daughter1->GetPz() ) / pXi;
  Float_t pnpar = ( pXix*Daughter2->GetPx() +
                    pXiy*Daughter2->GetPy() +
                    pXiz*Daughter2->GetPz() ) / pXi;

  if ((pppar + pnpar) != 0)
    return ((pppar - pnpar) / (pppar + pnpar));
  else
    return 999.0;

}

//______________________________________________________________________________
Int_t T49VertexRoot::GetSameSide()
{
  //
  // Returns 1 if both daughters are on the same side
  //

  T49ParticleRoot *Pos = GetFullPositiveTrack();
  T49ParticleRoot *Neg = GetFullNegativeTrack();

  return (((Pos->GetXFirst() * Neg->GetXFirst()) > 0) ? 1 : 0);

}

//______________________________________________________________________________
Int_t T49VertexRoot::GetSameSideXi()
{
  //
  // Returns 1 if both daughters are on the same side
  //

  T49EventRoot    *Event      = (T49EventRoot *)  gT49->GetCurrentEvent();
  T49ParticleRoot *Charged    = GetFullChargedXiTrack();
  T49ParticleRoot *Neutral    = GetFullNeutralXiTrack();
  Int_t idxNeutralVtx         = Neutral->GetStopVertexIndex();
  T49VertexRoot   *NeutralVtx = (T49VertexRoot *)
                                Event->GetSecondaryVertices()->At(idxNeutralVtx);

  return (((Charged->GetXFirst() * NeutralVtx->GetX()) > 0) ? 1 : 0);

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetY1minY2()
{
  //
  // The by-difference of the daughter tracks in the target plane
  //

  T49EventRoot    *Event   = (T49EventRoot *)  gT49->GetCurrentEvent();
  T49VertexRoot   *MainVtx = (T49VertexRoot *) Event->GetPrimaryVertex();
  T49ParticleRoot *Pos     = GetFullPositiveTrack();
  T49ParticleRoot *Neg     = GetFullNegativeTrack();

  Float_t byPos = Pos->GetY0(MainVtx->GetZ());
  Float_t byNeg = Neg->GetY0(MainVtx->GetZ());

  if ((byPos > -999.0) && (byNeg > -999.0)) {
    return (byPos - byNeg);
  }
  else {
    return 9999.9;
  }

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetY1minY2Xi()
{
  //
  // The by-difference of the daughter tracks in the target plane
  // for Xi and Omega candidates
  //

  T49EventRoot    *Event      = (T49EventRoot *)  gT49->GetCurrentEvent();
  T49VertexRoot   *MainVtx    = (T49VertexRoot *) Event->GetPrimaryVertex();
  T49ParticleRoot *Charged    = GetFullChargedXiTrack();
  T49ParticleRoot *Neutral    = GetFullNeutralXiTrack();
  Int_t idxNeutralVtx         = Neutral->GetStopVertexIndex();
  T49VertexRoot   *NeutralVtx = (T49VertexRoot *)
                                Event->GetSecondaryVertices()->At(idxNeutralVtx);

  Float_t byCharged = Charged->GetY0(MainVtx->GetZ());
  Float_t byNeutral = NeutralVtx->GetYTarg();

  if (byCharged > -999.0) {
    return (byNeutral - byCharged);
  }
  else {
    return 9999.9;
  }

}

//______________________________________________________________________________
Float_t T49VertexRoot::GetCrossYZ(Float_t zmin)
{
  //
  // The crossYZ parameter (Crossing of the daughter tracks in the YZ-plane)
  // CrossYZ = (slopePos - slopeNeg) * (byPos - byNeg)
  //

  T49ParticleRoot *Pos = GetFullPositiveTrack();
  T49ParticleRoot *Neg = GetFullNegativeTrack();

  Float_t slPos = Pos->GetYPrime();
  Float_t slNeg = Neg->GetYPrime();

  if ((slPos > -999.0) && (slNeg > -999.0)) {
    Float_t byPos = Pos->GetY0(zmin);
    Float_t byNeg = Neg->GetY0(zmin);
    return ((slPos - slNeg) * (byPos - byNeg));
  }
  else {
    return -9999.9;
  }

}

//______________________________________________________________________________
Float_t T49VertexRoot::CosTheta(Float_t *pV0, Float_t *pPD)
{
  //
  // Calculate the cos(theta) between V0 and decay-proton
  //

  Float_t pIn[4];

  if (Lorentz(pV0,pPD,pIn)) {

    Float_t pIntot = TMath::Sqrt(pIn[0]*pIn[0] + pIn[1]*pIn[1] + pIn[2]*pIn[2]);
    Float_t xxx    = GetP() * pIntot;
    if (xxx > 0.0)
      return (pV0[0]*pIn[0] + pV0[1]*pIn[1] + pV0[2]*pIn[2]) / xxx;
    else
      return -999.0;

  }
  else {
    return -999.0;
  }

}

//______________________________________________________________________________
Int_t T49VertexRoot::Lorentz(Float_t *p4cm, Float_t *p4in, Float_t *p4out)
{
  //
  // Lorenz transformation
  // input: reference system(P,M,E), input vector p,e
  //        (momentum in sys, observed from refer.sys)
  // output: vector p',e'
  // all 4-vectors, with 0,1,2 -- p-components, 3 -- energy
  //

  // Calculating mass and momentum in cm-system (M, P)
  Float_t pcm2 = p4cm[0]*p4cm[0] + p4cm[1]*p4cm[1] + p4cm[2]*p4cm[2];
  Float_t mcm2 = p4cm[3]*p4cm[3] - pcm2;
  if (mcm2 < 0.   ) return 0;

  Float_t mcm  = sqrt((double) mcm2);
  if (mcm  < 0.001) return 0;

  // Now transforming and calculating values p,e
  Float_t pP = p4in[0]*p4cm[0] + p4in[1]*p4cm[1] + p4in[2]*p4cm[2];
  Float_t f  = pP * (p4cm[3]/mcm - 1.0) / pcm2 - p4in[3]/mcm;

  // Calculating transformed new values p',e'
  p4out[3] = (p4in[3]*p4cm[3] - pP) / mcm;
  for (Int_t i = 0; i < 3; i++)
    p4out[i] = p4in[i] + p4cm[i]*f;

  return 1;

}

//______________________________________________________________________________
Bool_t T49VertexRoot::SetDaughters(T49ParticleRoot *Pos, T49ParticleRoot *Neg)
{
  //
  // Set the pointer to the daughter tracks
  //

  fFirstTrack           = Pos;
  fSecondTrack          = Neg;
  fFirstTrackIsPositive = kTRUE;

  return kTRUE;

}

//______________________________________________________________________________
Bool_t T49VertexRoot::SetDaughters()
{
  //
  // Set the pointer to the daughter tracks
  //

  Int_t Index;

  if (!gT49) return kFALSE;

  T49EventRoot *Event = (T49EventRoot *) gT49->GetCurrentEvent();
  TClonesArray *SecondaryTrackArray = Event->GetSecondaryParticles();

  Index = GetSecondaryIndex(0);
  if ((Index >= 0) && (Index < SecondaryTrackArray->GetEntries())) {
    fFirstTrack = (T49ParticleRoot*) SecondaryTrackArray->At(Index);
  }
  else {
    fprintf(stderr,"T49VertexRoot::SetDaughters - No secondary index for track 1\n");
    return kFALSE;
  }

  Index = GetSecondaryIndex(1);
  if ((Index >= 0) && (Index < SecondaryTrackArray->GetEntries())) {
    fSecondTrack = (T49ParticleRoot*) SecondaryTrackArray->At(Index);
  }
  else {
    fprintf(stderr,"T49VertexRoot::SetDaughters - No secondary index for track 2\n");
    return kFALSE;
  }

  return kTRUE;

}

//______________________________________________________________________________
T49ParticleRoot *T49VertexRoot::GetFirstTrack()
{

  if      (fFirstTrack)
    return fFirstTrack;
  else if (SetDaughters())
    return fFirstTrack;
  else
    return 0;

}

//______________________________________________________________________________
T49ParticleRoot *T49VertexRoot::GetSecondTrack()
{

  if      (fSecondTrack)
    return fSecondTrack;
  else if (SetDaughters())
    return fSecondTrack;
  else
    return 0;

}

//______________________________________________________________________________
Int_t T49VertexRoot::GetPrimaryIndex(Int_t Track)
{
  //
  // Get the index of the main vertex track in the primary particle array
  //

  if      (Track == 0)
    return fFirstPriIndex;
  else if (Track == 1)
    return fSecondPriIndex;
  else {
    return -1;
    fprintf(stderr,"T49VertexRoot::GetPrimaryIndex - track out of range (%d)\n"
           ,Track);
  }

}

//______________________________________________________________________________
Int_t T49VertexRoot::GetSecondaryIndex(Int_t Track)
{
  //
  // Get the index of the V0 track in the secondary particle array
  //

  if      (Track == 0)
    return fFirstSecIndex;
  else if (Track == 1)
    return fSecondSecIndex;
  else {
    return -1;
    fprintf(stderr,"T49VertexRoot::GetSecondaryIndex - track out of range (%d)\n"
           ,Track);
  }

}

//______________________________________________________________________________
T49ParticleRoot* T49VertexRoot::GetFullPositiveTrack()
{
  //
  // Get the positive daughter track including the rtrack information
  //

  T49ParticleRoot *MainTrack = GetPositiveMainTrack();
  T49ParticleRoot *SecTrack  = GetPositiveTrack();

  return GetFullTrack(SecTrack,MainTrack);

}

//______________________________________________________________________________
T49ParticleRoot* T49VertexRoot::GetFullNegativeTrack()
{
  //
  // Get the negative daughter track including the rtrack information
  //

  T49ParticleRoot *MainTrack = GetNegativeMainTrack();
  T49ParticleRoot *SecTrack  = GetNegativeTrack();

  return GetFullTrack(SecTrack,MainTrack);

}

//______________________________________________________________________________
T49ParticleRoot* T49VertexRoot::GetFullChargedXiTrack()
{
  //
  // Get the charged Xi daughter track including the rtrack information
  //

  if (gT49) {

    T49EventRoot    *Event             = (T49EventRoot *) gT49->GetCurrentEvent();
    TClonesArray    *PrimaryTrackArray = Event->GetPrimaryParticles();
    T49ParticleRoot *SecTrack          = GetFirstTrack();

    Int_t index = GetPrimaryIndex(0);
    if (index >= 0) {
      T49ParticleRoot *MainTrack = (T49ParticleRoot*) PrimaryTrackArray->At(index);
      return GetFullTrack(SecTrack,MainTrack);
    }
    else {
      return SecTrack;
    }

  }
  else {

    return 0;

  }

}

//______________________________________________________________________________
T49ParticleRoot* T49VertexRoot::GetFullTrack(T49ParticleRoot *SecTrack
                                            ,T49ParticleRoot *MainTrack)
{
  //
  // Copy the full rtrack information
  //

  if (MainTrack) {
    SecTrack->SetIflag(MainTrack->GetIflag());
    SecTrack->SetXFirst(MainTrack->GetXFirst());
    SecTrack->SetYFirst(MainTrack->GetYFirst());
    SecTrack->SetZFirst(MainTrack->GetZFirst());
    SecTrack->SetXLast(MainTrack->GetXLast());
    SecTrack->SetYLast(MainTrack->GetYLast());
    SecTrack->SetZLast(MainTrack->GetZLast());
    for (Int_t i = 0; i < 4; i++) {
      SecTrack->SetNPoint(MainTrack->GetNPoint(i),i);
      SecTrack->SetNFitPoint(MainTrack->GetNFitPoint(i),i);
      SecTrack->SetNDedxPoint(MainTrack->GetNDedxPoint(i),i);
      SecTrack->SetNMaxPoint(MainTrack->GetNMaxPoint(i),i);
      SecTrack->SetTmeanCharge(MainTrack->GetTmeanCharge(i),i);
    }
    for (Int_t i = 0; i < 3; i++) {
      SecTrack->SetXFirst(i,MainTrack->GetXFirst(i));
      SecTrack->SetYFirst(i,MainTrack->GetYFirst(i));
      SecTrack->SetZFirst(i,MainTrack->GetZFirst(i));
      SecTrack->SetXLast(i,MainTrack->GetXLast(i));
      SecTrack->SetYLast(i,MainTrack->GetYLast(i));
      SecTrack->SetZLast(i,MainTrack->GetZLast(i));
    }
  }
  else {
    // Reconstruct the number of global points. Has to be done
    // because of a bug in TRootDS::FillSecTracks()
    Int_t nPoint    = SecTrack->GetNPoint(0)
                    + SecTrack->GetNPoint(1)
                    + SecTrack->GetNPoint(2);
    SecTrack->SetNPoint(nPoint);
    Int_t nMaxPoint = SecTrack->GetNMaxPoint(0)
                    + SecTrack->GetNMaxPoint(1)
                    + SecTrack->GetNMaxPoint(2);
    SecTrack->SetNMaxPoint(nMaxPoint);
  }

  return SecTrack;

}

//______________________________________________________________________________
T49ParticleRoot* T49VertexRoot::GetPositiveMainTrack()
{
  //
  // Get the pointer to the main-vertex fit of the positive daughter track
  //

  if (gT49) {
    T49EventRoot* Event = (T49EventRoot *) gT49->GetCurrentEvent();
    return GetPositiveMainTrack(Event->GetPrimaryParticles());
  }
  else {
    return 0;
  }

}

//______________________________________________________________________________
T49ParticleRoot* T49VertexRoot::GetPositiveMainTrack(TClonesArray
                                                     *PrimaryTrackArray)
{
  //
  // Get the pointer to the main-vertex fit of the positive daughter track
  //

  Int_t Index;

  if (fFirstTrackIsPositive)
    Index = GetPrimaryIndex(0);
  else
    Index = GetPrimaryIndex(1);

  if (Index >= 0) {
    return (T49ParticleRoot*) PrimaryTrackArray->At(Index);
  }
  else {
    return NULL;
  }

}

//______________________________________________________________________________
T49ParticleRoot* T49VertexRoot::GetNegativeMainTrack()
{
  //
  // Get the pointer to the main-vertex fit of the negative daughter track
  //

  if (gT49) {
    T49EventRoot* Event = (T49EventRoot *) gT49->GetCurrentEvent();
    return GetNegativeMainTrack(Event->GetPrimaryParticles());
  }
  else {
    return 0;
  }

}

//______________________________________________________________________________
T49ParticleRoot* T49VertexRoot::GetNegativeMainTrack(TClonesArray
                                                    *PrimaryTrackArray)
{
  //
  // Get the pointer to the main-vertex fit of the negative daughter track
  //

  Int_t Index;

  if (fFirstTrackIsPositive)
    Index = GetPrimaryIndex(1);
  else
    Index = GetPrimaryIndex(0);

  if (Index >= 0) {
    return (T49ParticleRoot*) PrimaryTrackArray->At(Index);
  }
  else {
    return NULL;
  }

}

//______________________________________________________________________________
void T49VertexRoot::Streamer(TBuffer &R__b)
{
  //
  // Stream an object of class T49VertexRoot.
  //

  UInt_t     R__s,R__c;
  Float_t    DummyFloat;
  TObjArray *DummyObjArray;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v > 5) {
      T49VertexRoot::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
    }
    T49Vertex::Streamer(R__b);
    R__b >> fIflag;
    R__b >> fIdDet;
    R__b >> fIdVtx;
    R__b >> fNTrkFit;
    R__b >> fNTrkOut;
    R__b >> fX;
    R__b >> fY;
    R__b >> fZ;
    R__b >> fSigX;
    R__b >> fSigY;
    R__b >> fSigZ;
    R__b >> fPchi2;
    R__b >> fFirstTrack;
    R__b >> fSecondTrack;
    R__b >> fFirstTrackIsPositive;
    if (R__v < 3) {
      R__b >> DummyObjArray;
    }
    if (R__v > 1) {
      R__b >> fFirstPriIndex;
      R__b >> fSecondPriIndex;
      R__b >> fFirstSecIndex;
      R__b >> fSecondSecIndex;
      R__b >> fXTarg;
      R__b >> fYTarg;
      if (R__v < 4) {
        R__b >> DummyFloat;
      }
      R__b >> fX1minX2;
      if (R__v > 4) {
        R__b >> fPlen;
      }
    }
    R__b.CheckByteCount(R__s, R__c, T49VertexRoot::IsA());
  }
  else {
    T49VertexRoot::Class()->WriteBuffer(R__b,this);
  }

}











