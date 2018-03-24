///////////////////////////////////////////////////////////////////////
//                                                                   //
//  Contains the data of Monte-Carlo particles                       //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "TClass.h"

#include "T49ParticleMCRoot.h"

ClassImp(T49ParticleMCRoot)

/*
$Log: T49ParticleMCRoot.C,v $
Revision 1.8  2001/12/12 17:38:16  cblume
Update default constructors

Revision 1.7  2001/11/12 10:51:59  cblume
Update for ROOT v3.01

 * Revision 1.6  2000/11/09  09:07:58  cblume
 * Introduce the byte count in the streamer
 *
 * Revision 1.5  2000/10/31  13:34:47  cblume
 * Initialize vertex links with -1
 *
 * Revision 1.4  2000/10/15  01:08:58  cblume
 * Store also the number of matched points
 *
 * Revision 1.3  2000/02/24  12:58:59  cblume
 * Changed kPriMatch and kSecMatch back to 50, because of problems with the TClonesArray in old files
 *
 * Revision 1.2  2000/02/23  15:44:00  cblume
 * Now with customized streamer to ensure backward compability
 *
 * Revision 1.1  1999/11/23  12:59:51  cblume
 * Add T49ParticleMCRoot.C
 *
*/

//______________________________________________________________________________
T49ParticleMCRoot::T49ParticleMCRoot():T49ParticleRoot()
{
  //
  // T49ParticleMCRoot default constructor
  //

  fPid = 0;

  fNPriMatched = 0;
  for (Int_t iMatch = 0; iMatch < kPriMatch; iMatch++) {
    fPriMatched[iMatch]     = -1;
    fNPriMatchPoint[iMatch] =  0;
  }

  fNSecMatched = 0;
  for (Int_t iMatch = 0; iMatch < kSecMatch; iMatch++) {
    fSecMatched[iMatch]     = -1;
    fNSecMatchPoint[iMatch] =  0;
  }

  fStartVertex = -1;
  fStopVertex  = -1;

}

//______________________________________________________________________________
T49ParticleMCRoot::T49ParticleMCRoot(T49ParticleMCRoot& mcTrack)
                  :T49ParticleRoot(mcTrack)
{
  //
  // T49ParticleMCRoot copy constructor
  //

  fPid = mcTrack.GetPid();

  fNPriMatched = mcTrack.GetNPriMatched();
  for (Int_t iMatch = 0; iMatch < kPriMatch; iMatch++) {
    fPriMatched[iMatch]     = mcTrack.GetPriMatched(iMatch);
    fNPriMatchPoint[iMatch] = mcTrack.GetNPriMatchPoint(iMatch);
  }

  fNSecMatched = mcTrack.GetNSecMatched();
  for (Int_t iMatch = 0; iMatch < kSecMatch; iMatch++) {
    fSecMatched[iMatch]     = mcTrack.GetSecMatched(iMatch);
    fNSecMatchPoint[iMatch] = mcTrack.GetNSecMatchPoint(iMatch);
  }

  fStartVertex = mcTrack.GetStartVertex();
  fStopVertex  = mcTrack.GetStopVertex();

}

//______________________________________________________________________________
Int_t T49ParticleMCRoot::GetCharge()
{

  if ((fPid > 0) && (fPid < kGParticle)) {
    return kGCharge[fPid];
  }
  else {
    printf("T49ParticleMCRoot::GetCharge - No valid pid %d\n",fPid);
    return 0;
  }

}

//______________________________________________________________________________
Float_t T49ParticleMCRoot::GetE()
{

  if ((fPid > 0) && (fPid < kGParticle)) {
    return GetE(kGMass[fPid]);
  }
  else {
    printf("T49ParticleMCRoot::GetE - No valid pid %d\n",fPid);
    return 0;
  }

}

//______________________________________________________________________________
Float_t T49ParticleMCRoot::GetRap()
{

  if ((fPid > 0) && (fPid < kGParticle)) {
    return GetRap(kGMass[fPid]);
  }
  else {
    printf("T49ParticleMCRoot::GetRap - No valid pid %d\n",fPid);
    return 0;
  }

}

//______________________________________________________________________________
Float_t T49ParticleMCRoot::GetRap(Float_t mass)
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
Float_t T49ParticleMCRoot::GetMt()
{

  if ((fPid > 0) && (fPid < kGParticle)) {
    return GetMt(kGMass[fPid]);
  }
  else {
    printf("T49ParticleMCRoot::GetMt - No valid pid %d\n",fPid);
    return 0;
  }

}

//______________________________________________________________________________
Float_t T49ParticleMCRoot::GetMass()
{

  if ((fPid > 0) && (fPid < kGParticle)) {
    return kGMass[fPid];
  }
  else {
    printf("T49ParticleMCRoot::GetMass - No valid pid %d\n",fPid);
    return 0;
  }

}

//______________________________________________________________________________
Int_t T49ParticleMCRoot::GetPriMatched(Int_t iMatch)
{
  //
  // Returns the links to the matched main vertex tracks
  //

  if (iMatch < kPriMatch) {
    return fPriMatched[iMatch];
  }
  else {
    fprintf(stderr,"T49ParticleMCRoot::GetPriMatched - Index out of bounds (%d,%d)\n"
                  ,iMatch,kPriMatch);
    return -1;
  }

}

//______________________________________________________________________________
Int_t T49ParticleMCRoot::GetSecMatched(Int_t iMatch)
{
  //
  // Returns the links to the matched secondary tracks
  //

  if (iMatch < kSecMatch) {
    return fSecMatched[iMatch];
  }
  else {
    fprintf(stderr,"T49ParticleMCRoot::GetSecMatched - Index out of bounds (%d,%d)\n"
                  ,iMatch,kSecMatch);
    return -1;
  }

}

//______________________________________________________________________________
Int_t T49ParticleMCRoot::GetNPriMatchPoint(Int_t iMatch)
{
  //
  // Get the number of matched points for a match to a primary track
  //

  if (iMatch < kPriMatch) {
    return fNPriMatchPoint[iMatch];
  }
  else {
    fprintf(stderr,"T49ParticleMCRoot::GetNPriMatchPoint - Index out of bounds (%d,%d)\n"
                  ,iMatch,kPriMatch);
    return -1;
  }

}

//______________________________________________________________________________
Int_t T49ParticleMCRoot::GetNSecMatchPoint(Int_t iMatch)
{
  //
  // Get the number of matched points for a match to a secondary track
  //

  if (iMatch < kSecMatch) {
    return fNSecMatchPoint[iMatch];
  }
  else {
    fprintf(stderr,"T49ParticleMCRoot::GetNSecMatchPoint - Index out of bounds (%d,%d)\n"
                  ,iMatch,kSecMatch);
    return -1;
  }

}

//______________________________________________________________________________
void T49ParticleMCRoot::SetPriMatched(Int_t index)
{
  //
  // Sets a link to a matched primary track
  //

  if (fNPriMatched < kPriMatch) {
    fPriMatched[fNPriMatched] = index;
    fNPriMatched++;
  }
  else {
    fprintf(stderr,"T49ParticleMCRoot::SetPriMatched - Boundary limits reached (%d,%d)\n"
                  ,fNPriMatched,kPriMatch);
  }

}

//______________________________________________________________________________
void T49ParticleMCRoot::SetSecMatched(Int_t index)
{
  //
  // Sets a link to a matched secondary track
  //

  if (fNSecMatched < kSecMatch) {
    fSecMatched[fNSecMatched] = index;
    fNSecMatched++;
  }
  else {
    fprintf(stderr,"T49ParticleMCRoot::SetSecMatched - Boundary limits reached (%d,%d)\n"
                  ,fNSecMatched,kSecMatch);
  }

}

//______________________________________________________________________________
void T49ParticleMCRoot::SetPriMatched(Int_t index, Int_t npoint)
{
  //
  // Sets a link to a matched primary track
  //

  if (fNPriMatched < kPriMatch) {
    fPriMatched[fNPriMatched]     = index;
    fNPriMatchPoint[fNPriMatched] = npoint;
    fNPriMatched++;
  }
  else {
    fprintf(stderr,"T49ParticleMCRoot::SetPriMatched - Boundary limits reached (%d,%d)\n"
                  ,fNPriMatched,kPriMatch);
  }

}

//______________________________________________________________________________
void T49ParticleMCRoot::SetSecMatched(Int_t index, Int_t npoint)
{
  //
  // Sets a link to a matched secondary track
  //

  if (fNSecMatched < kSecMatch) {
    fSecMatched[fNSecMatched]     = index;
    fNSecMatchPoint[fNSecMatched] = npoint;
    fNSecMatched++;
  }
  else {
    fprintf(stderr,"T49ParticleMCRoot::SetSecMatched - Boundary limits reached (%d,%d)\n"
                  ,fNSecMatched,kSecMatch);
  }

}

//______________________________________________________________________________
void T49ParticleMCRoot::Streamer(TBuffer &R__b)
{
  //
  // Stream an object of class T49ParticleMCRoot.
  //

  UInt_t R__s, R__c;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v > 3) {
      T49ParticleMCRoot::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
    }
    T49ParticleRoot::Streamer(R__b);
    R__b >> fPid;
    R__b >> fNPriMatched;
    R__b.ReadStaticArray(fPriMatched);
    if (R__v > 2) {
      R__b.ReadStaticArray(fNPriMatchPoint);
    }
    R__b >> fNSecMatched;
    R__b.ReadStaticArray(fSecMatched);
    if (R__v > 2) {
      R__b.ReadStaticArray(fNSecMatchPoint);
    }
    R__b >> fStartVertex;
    R__b >> fStopVertex;
    R__b.CheckByteCount(R__s, R__c, T49ParticleMCRoot::IsA());
  }
  else {
    T49ParticleMCRoot::Class()->WriteBuffer(R__b,this);
  }

}







