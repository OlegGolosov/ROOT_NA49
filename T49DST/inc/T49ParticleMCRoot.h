#ifndef _T49ParticleMCRoot_INCLUDED_
#define _T49ParticleMCRoot_INCLUDED_

/*
$Log: T49ParticleMCRoot.h,v $
Revision 1.10  2002/11/26 12:19:24  cblume
Increase ClassDef Number

Revision 1.9  2002/11/22 09:30:03  cblume
Go back again to kSecMatch = 50

Revision 1.8  2002/10/07 09:58:56  cblume
Update for CASTOR

Revision 1.7  2002/05/22 09:40:03  cblume
Change the size of the fSecMatched array

Revision 1.6  2001/11/12 10:48:28  cblume
Update for ROOT v3.01

 * Revision 1.5  2000/10/15  01:09:18  cblume
 * Store also the number of matched points
 *
 * Revision 1.4  2000/02/24  12:59:11  cblume
 * Changed kPriMatch and kSecMatch back to 50, because of problems with the TClonesArray in old files
 *
 * Revision 1.3  2000/02/23  15:44:36  cblume
 * Now with customized streamer to ensure backward compability
 *
 * Revision 1.2  2000/02/23  09:48:08  cblume
 * Changed kPriMatch and kSecMatch to 100
 *
 * Revision 1.1  1999/11/23  12:55:15  cblume
 * Add T49ParticleMCRoot.h
 *
*/

#include <TBuffer.h>
#include "TMath.h"
#include "T49ParticleRoot.h"
#include "T49Geant.h"

const Int_t kPriMatch =  50;
//const Int_t kSecMatch = 200;
const Int_t kSecMatch = 50;

class T49ParticleMCRoot : public T49ParticleRoot {

public:

  T49ParticleMCRoot();
  T49ParticleMCRoot(T49ParticleMCRoot& mcTrack);
  virtual ~T49ParticleMCRoot() { };

  // Getters
  virtual Int_t   GetPid()                    { return fPid; };
  virtual Int_t   GetCharge();
  virtual Float_t GetE();
  virtual Float_t GetE(Float_t mass)          { return sqrt(GetP()*GetP()+mass*mass); }
  virtual Float_t GetRap();
  virtual Float_t GetRap(Float_t mass);
  virtual Float_t GetMt();
  virtual Float_t GetMt(Float_t mass)         { return sqrt(mass*mass + GetPt()*GetPt()); };
  virtual Float_t GetMass();

  virtual Int_t   GetPriMatched(Int_t iMatch);
  virtual Int_t   GetSecMatched(Int_t iMatch);
  virtual Int_t   GetNPriMatched()            { return fNPriMatched; };
  virtual Int_t   GetNSecMatched()            { return fNSecMatched; };
  virtual Int_t   GetNPriMatchPoint(Int_t iMatch);
  virtual Int_t   GetNSecMatchPoint(Int_t iMatch);

  virtual Int_t   GetStartVertex()            { return fStartVertex; };
  virtual Int_t   GetStopVertex()             { return fStopVertex;  };

  // Setters
  virtual void    SetPid(Int_t i)             { fPid = i; };

  virtual void    SetPriMatched(Int_t index);
  virtual void    SetSecMatched(Int_t index);
  virtual void    SetPriMatched(Int_t index, Int_t npoint);
  virtual void    SetSecMatched(Int_t index, Int_t npoint);

  virtual void    SetStartVertex(Int_t index) { fStartVertex = index; };
  virtual void    SetStopVertex(Int_t index)  { fStopVertex  = index; };

protected:

  Int_t     fPid;                       // Geant particle ID

  Int_t     fNPriMatched;               // Number of matched main vertex tracks
  Int_t     fPriMatched[kPriMatch];     // Links to matched reconstructed main vertex tracks
  Int_t     fNPriMatchPoint[kPriMatch]; // Number of matched points

  Int_t     fNSecMatched;               // Number of matched secondary tracks
  Int_t     fSecMatched[kSecMatch];     // Links to matched reconstructed secondary tracks
  Int_t     fNSecMatchPoint[kSecMatch]; // Number of matched points

  Int_t     fStartVertex;               // Link to the start vertex
  Int_t     fStopVertex;                // Link to the stop vertex

  ClassDef(T49ParticleMCRoot,6)         // Monte Carlo particle

};

#endif
