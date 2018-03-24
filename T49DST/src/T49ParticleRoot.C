///////////////////////////////////////////////////////////////////////
//
//  Contains the particle information
//  (taken from rtrack and track structures).
//
///////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>

#include "TObject.h"
#include "TClass.h"
#include "TRandom.h"

#include "T49ParticleRoot.h"

ClassImp(T49ParticleRoot)

using namespace std;

/*
$Log: T49ParticleRoot.C,v $
Revision 1.12  2008/01/04 10:48:59  cblume
64bit modifications by Marek Szuba

Revision 1.11  2001/12/12 17:38:25  cblume
Update default constructors

Revision 1.10  2001/11/12 10:52:06  cblume
Update for ROOT v3.01

 * Revision 1.9  2001/06/13  08:12:42  cblume
 * Add the possibility to store the index of the stop vertex
 *
 * Revision 1.8  2000/11/24  12:38:26  cblume
 * Update of GetX/Y0() and GetX/YPrime(). Moved inline functions to implementation file
 *
 * Revision 1.7  2000/11/09  09:07:48  cblume
 * Introduce the byte count in the streamer
 *
 * Revision 1.6  2000/10/31  13:33:21  cblume
 * Change definition of phi for py == 0
 *
 * Revision 1.5  2000/04/18  17:26:12  cblume
 * Part of the member functions are now inline
 *
 * Revision 1.4  2000/04/13  15:10:28  cblume
 * "test"
 *
 * Revision 1.3  1999/11/23  12:59:03  cblume
 * Remove files
 *
*/

//______________________________________________________________________________
T49ParticleRoot::T49ParticleRoot()
{
  //
  // Default constructor
  //

  Int_t i;

  fIdDet       = 0;
  fCharge      = 0;
  fIflag       = 0;
  fPx          = 0;
  fPy          = 0;
  fPz          = 0;
  fSigPx       = 0;
  fSigPy       = 0;
  fSigPz       = 0;
  fBx          = 0;
  fBy          = 0;
  fPchi2       = 0;
  fXFirst      = 0;
  fYFirst      = 0;
  fZFirst      = 0;
  fXLast       = 0;
  fYLast       = 0;
  fZLast       = 0;
  fWeight      = 0;
  fRandom      = 0;
  fLabel       = 0;
  fVertexIndex = -1;
  fTofIflag    = 0;
  fTofId       = 0;
  fTofX        = 0;
  fTofY        = 0;
  fTofPathl    = 0;
  fTofCharge   = 0;
  fTofMass2    = 0;
  fTofSigMass2 = 0;

  for (i = 0; i < 4; i++) {
    fNPoint[i]      = 0;
    fNFitPoint[i]   = 0;
    fNDedxPoint[i]  = 0;
    fNMaxPoint[i]   = 0;
    fTmeanCharge[i] = 0;
  }

  for (i = 0; i < 3; i++) {
    fXLocFirst[i] = 0;
    fYLocFirst[i] = 0;
    fZLocFirst[i] = 0;
    fXLocLast[i]  = 0;
    fYLocLast[i]  = 0;
    fZLocLast[i]  = 0;
  }

}

//______________________________________________________________________________
T49ParticleRoot::T49ParticleRoot(T49ParticleRoot *Track)
{
  //
  // T49ParticleRoot copy constructor
  //
  TRandom *run = new TRandom(); //men elave etdim
  CopyParticle(Track);

}

//______________________________________________________________________________
T49ParticleRoot::~T49ParticleRoot()
{
  //
  // T49ParticleRoot destructor
  //

}

//______________________________________________________________________________
void T49ParticleRoot::SetRandom()
{
  //
  // Assigns a random number to this particle
  //

  fRandom = gRandom->Rndm();

}

//______________________________________________________________________________
void T49ParticleRoot::SetXFirst(Int_t det, Float_t x)
{
  //
  // Sets the x-position of the first point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    fXLocFirst[det] = x;
  }
  else {
    fprintf(stderr,"T49ParticleRoot::SetXFirst - Invalid detector ID (%d)\n"
           ,det);
  }
}

//______________________________________________________________________________
void T49ParticleRoot::SetYFirst(Int_t det, Float_t x)
{
  //
  // Sets the y-position of the first point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    fYLocFirst[det] = x;
  }
  else {
    fprintf(stderr,"T49ParticleRoot::SetYFirst - Invalid detector ID (%d)\n"
           ,det);
  }

}

//______________________________________________________________________________
void T49ParticleRoot::SetZFirst(Int_t det, Float_t x)
{
  //
  // Sets the z-position of the first point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    fZLocFirst[det] = x;
  }
  else {
    fprintf(stderr,"T49ParticleRoot::SetZFirst - Invalid detector ID (%d)\n"
           ,det);
  }

}

//______________________________________________________________________________
void T49ParticleRoot::SetXLast(Int_t det, Float_t x)
{
  //
  // Sets the x-position of the last point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    fXLocLast[det] = x;
  }
  else {
    fprintf(stderr,"T49ParticleRoot::SetXLast - Invalid detector ID (%d)\n"
           ,det);
  }

}

//______________________________________________________________________________
void T49ParticleRoot::SetYLast(Int_t det, Float_t x)
{
  //
  // Set the y-position of the last point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    fYLocLast[det] = x;
  }
  else {
    fprintf(stderr,"T49ParticleRoot::SetYLast - Invalid detector ID (%d)\n"
           ,det);
  }

}

//______________________________________________________________________________
void T49ParticleRoot::SetZLast(Int_t det, Float_t x)
{
  //
  // Set the z-position of the last point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    fZLocLast[det] = x;
  }
  else {
    fprintf(stderr,"T49ParticleRoot::SetZLast - Invalid detector ID (%d)\n"
           ,det);
  }

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetPhi()
{
  //
  // The azimuth angle
  //

  Float_t phi = 0;

  if (GetPx() != 0) phi = atan(fabs(GetPy()/GetPx()));

  if      (GetPx() > 0 && GetPy() >  0) {
    return          phi;
  }
  else if (GetPx() < 0 && GetPy() >= 0) {
    return   M_PI - phi;
  }
  else if (GetPx() < 0 && GetPy() <  0) {
    return   M_PI + phi;
  }
  else if (GetPx() > 0 && GetPy() <  0) {
    return 2*M_PI - phi;
  }

  return phi;

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetRap(Float_t mass)
{
  //
  // The rapidity
  //

  Float_t rap;
  Float_t e;

  e   = TMath::Sqrt(mass*mass + GetPz()*GetPz()+GetPx()*GetPx()+GetPy()*GetPy());
  rap = 0.5 * TMath::Log((e + GetPz()) / (e - GetPz()));

  return rap;

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetEta()
{
  //
  // The pseudo-rapidity
  //

  Float_t theta = TMath::ATan(GetPt() / GetPz());
  if (theta > 0)
    return -TMath::Log(0.5 * theta);
  else
    return 0;

}
//______________________________________________________________________________
Float_t T49ParticleRoot::GetXFirst(Int_t det)
{
  //
  // Get the x-position of the first point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    return fXLocFirst[det];
  }
  else {
    fprintf(stderr,"T49ParticleRoot::GetXFirst - Invalid detector ID (%d)\n"
           ,det);
    return 0;
  }

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetYFirst(Int_t det)
{
  //
  // Get the y-position of the first point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    return fYLocFirst[det];
  }
  else {
    fprintf(stderr,"T49ParticleRoot::GetYFirst - Invalid detector ID (%d)\n"
           ,det);
    return 0;
  }

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetZFirst(Int_t det)
{
  //
  // Get the z-position of the first point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    return fZLocFirst[det];
  }
  else {
    fprintf(stderr,"T49ParticleRoot::GetZFirst - Invalid detector ID (%d)\n"
           ,det);
    return 0;
  }

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetXLast(Int_t det)
{
  //
  // Get the x-position of the last point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    return fXLocLast[det];
  }
  else {
    fprintf(stderr,"T49ParticleRoot::GetXLast - Invalid detector ID (%d)\n"
           ,det);
    return 0;
  }

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetYLast(Int_t det)
{
  //
  // Get the y-position of the last point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    return fYLocLast[det];
  }
  else {
    fprintf(stderr,"T49ParticleRoot::GetYLast - Invalid detector ID (%d)\n"
           ,det);
    return 0;
  }

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetZLast(Int_t det)
{
  //
  // Get the z-position of the last point (det: 0=vt1 1=vt2 2=mtp)
  //

  if ((det >= 0) && (det <= 2)) {
    return fZLocLast[det];
  }
  else {
    fprintf(stderr,"T49ParticleRoot::GetZLast - Invalid detector ID (%d)\n"
           ,det);
    return 0;
  }

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetX0(Float_t z)
{
  //
  // Get the bx at z-position <z> (linear extrapolation)
  //

  Float_t xPrime = GetXPrime();
  if (xPrime > - 9999.0) {
    return (fXFirst - xPrime * (fZFirst - z));
  }
  else {
    return -9999.9;
  }

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetY0(Float_t z)
{
  //
  // Get the by at z-position <z> (linear extrapolation)
  //

  Float_t yPrime = GetYPrime();
  if (yPrime > -9999.0) {
    return (fYFirst - yPrime * (fZFirst - z));
  }
  else {
    return -9999.9;
  }

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetXPrime()
{
  //
  // Get the slope in x (linear extrapolation)
  //

  if ((fZLast  != 0) &&
      (fZFirst != 0) &&
      (fZLast  != fZFirst)) {
    return ((fXLast - fXFirst) / (fZLast - fZFirst));
  }
  else {
    return -9999.9;
  }

}

//______________________________________________________________________________
Float_t T49ParticleRoot::GetYPrime()
{
  //
  // Get the slope in y (linear extrapolation)
  //

  if ((fZLast  != 0) &&
      (fZFirst != 0) &&
      (fZLast  != fZFirst)) {
    return ((fYLast - fYFirst) / (fZLast - fZFirst));
  }
  else {
    return -9999.9;
  }

}

//______________________________________________________________________________
void T49ParticleRoot::CopyParticle(T49ParticleRoot *Track)
{

  SetIdDet(Track->GetIdDet());
  SetIflag(Track->GetIflag());
  SetPx(Track->GetPx());
  SetPy(Track->GetPy());
  SetPz(Track->GetPz());
  SetSigPx(Track->GetSigPx());
  SetSigPy(Track->GetSigPy());
  SetSigPz(Track->GetSigPz());
  SetCharge(Track->GetCharge());
  SetBx(Track->GetBx());
  SetBy(Track->GetBy());

  SetXFirst(Track->GetXFirst());
  SetYFirst(Track->GetYFirst());
  SetZFirst(Track->GetZFirst());

  SetXLast(Track->GetXLast());
  SetYLast(Track->GetYLast());
  SetZLast(Track->GetZLast());

  for (Int_t i = 0; i < 4; i++) {
    SetNPoint(Track->GetNPoint(i),i);
    SetNFitPoint(Track->GetNFitPoint(i),i);
    SetNMaxPoint(Track->GetNMaxPoint(i),i);
    SetNDedxPoint(Track->GetNDedxPoint(i),i);
    SetTmeanCharge(Track->GetTmeanCharge(i),i);
  }

  SetTofIflag(Track->GetTofIflag());
  SetTofId(Track->GetTofId());
  SetTofX(Track->GetTofX());
  SetTofY(Track->GetTofY());
  SetTofPathl(Track->GetTofPathl());
  SetTofCharge(Track->GetTofCharge());
  SetTofMass2(Track->GetTofMass2());
  SetTofSigMass2(Track->GetTofSigMass2());

  SetRandom(Track->GetRandom());
  SetLabel(Track->GetLabel());

  SetStopVertexIndex(Track->GetStopVertexIndex());

  for (Int_t i = 0; i < 3; i++) {
    SetXFirst(i,Track->GetXFirst(i));
    SetYFirst(i,Track->GetYFirst(i));
    SetZFirst(i,Track->GetZFirst(i));
    SetXLast(i,Track->GetXLast(i));
    SetYLast(i,Track->GetYLast(i));
    SetZLast(i,Track->GetZLast(i));
  }

}

//______________________________________________________________________________
void T49ParticleRoot::Streamer(TBuffer &R__b)
{
  //
  // Stream an object of class T49ParticleRoot.
  //

  UInt_t R__s, R__c;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v > 2) {
      T49ParticleRoot::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
    }
    T49Particle::Streamer(R__b);
    R__b >> fIdDet;
    R__b >> fCharge;
    R__b.ReadStaticArray(fNPoint);
    R__b.ReadStaticArray(fNFitPoint);
    R__b.ReadStaticArray(fNDedxPoint);
    R__b.ReadStaticArray(fNMaxPoint);
    R__b >> fIflag;
    R__b.ReadStaticArray(fTmeanCharge);
    R__b >> fPx;
    R__b >> fPy;
    R__b >> fPz;
    R__b >> fSigPx;
    R__b >> fSigPy;
    R__b >> fSigPz;
    R__b >> fBx;
    R__b >> fBy;
    R__b >> fPchi2;
    R__b >> fXFirst;
    R__b >> fYFirst;
    R__b >> fZFirst;
    R__b >> fXLast;
    R__b >> fYLast;
    R__b >> fZLast;
    R__b >> fWeight;
    R__b >> fRandom;
    R__b >> fLabel;
    R__b >> fVertexIndex;
    R__b >> fTofIflag;
    R__b >> fTofId;
    R__b >> fTofX;
    R__b >> fTofY;
    R__b >> fTofPathl;
    R__b >> fTofCharge;
    R__b >> fTofMass2;
    R__b >> fTofSigMass2;
    if (R__v == 2) {
      R__b.ReadStaticArray(fXLocFirst);
      R__b.ReadStaticArray(fYLocFirst);
      R__b.ReadStaticArray(fZLocFirst);
      R__b.ReadStaticArray(fXLocLast);
      R__b.ReadStaticArray(fYLocLast);
      R__b.ReadStaticArray(fZLocLast);
    }
    R__b.CheckByteCount(R__s, R__c, T49ParticleRoot::IsA());
  }
  else {
    T49ParticleRoot::Class()->WriteBuffer(R__b,this);
  }

}








