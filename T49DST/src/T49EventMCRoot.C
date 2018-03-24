
///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Root mini-DST event class. Includes additional lists for       //
//    Monte Carlo tracks and vertices                                //
//                                                                   //
///////////////////////////////////////////////////////////////////////

/*
$Log: T49EventMCRoot.C,v $
Revision 1.9  2002/10/07 09:56:11  cblume
Update for CASTOR

Revision 1.8  2001/12/12 17:37:14  cblume
Update default constructors

Revision 1.7  2001/11/27 16:13:39  cblume
Include static TClonesArrays again because of problems in ReCreateArrays()

Revision 1.6  2001/11/26 12:15:50  cblume
Remove static arrays

Revision 1.5  2001/11/12 10:51:20  cblume
Update for ROOT v3.01

 * Revision 1.4  2001/06/28  15:13:12  cblume
 * Take out clear statement in the destructor
 *
 * Revision 1.3  2001/06/13  08:11:40  cblume
 * Modifications from Marco to avoid memory leaks
 *
 * Revision 1.2  1999/11/25  13:57:53  cblume
 * Introduced gT49KeepEvent in T49Event(MC)Root to make events cloneable
 *
 * Revision 1.1  1999/11/23  12:59:55  cblume
 * Add T49EventMCRoot.C
 *
*/

#include "TClass.h"

#include "T49EventMCRoot.h"

TClonesArray *T49EventMCRoot::fgStaticMCParticles = NULL;

ClassImp(T49EventMCRoot)

//______________________________________________________________________________
T49EventMCRoot::T49EventMCRoot():T49EventRoot()
{
  //
  // T49EventMCRoot default constructor
  //

  fMCVertices  = NULL;
  fMCParticles = NULL;

}

//______________________________________________________________________________
T49EventMCRoot::T49EventMCRoot(Int_t AllocObjects)
               :T49EventRoot(AllocObjects)
{
  //
  // T49EventMCRoot default constructor
  //

  static Int_t FirstCall = kTRUE;

  if (AllocObjects) {
    fMCVertices = new TObjArray(50);
  }
  else {
    fMCVertices = NULL;
  }

  if (gT49KeepEvent) {

    fMCParticles = new TClonesArray("T49ParticleMCRoot",200);

  }
  else {

    if (FirstCall) {
      fgStaticMCParticles = new TClonesArray("T49ParticleMCRoot",200);
      FirstCall = kFALSE;
    }

    if (fgStaticMCParticles) {
      fMCParticles = fgStaticMCParticles;
      fMCParticles->Clear();
    }
    else {
      fMCParticles = NULL;
    }

  }

}

//______________________________________________________________________________
T49EventMCRoot::~T49EventMCRoot()
{
  //
  // T49EventMCRoot destructor
  //

  if (fMCVertices) {
    fMCVertices->Delete();
    delete fMCVertices;
  }

}

//______________________________________________________________________________
void T49EventMCRoot::ClearParticleLists()
{
  //
  // Clears all particle lists
  //

  T49EventRoot::ClearParticleLists();

  if (fMCParticles) {
    fMCParticles->Clear();
  }
  if (fMCVertices) {
    fMCVertices->Delete();
    delete fMCVertices;
    fMCVertices = NULL;
  }

}

//______________________________________________________________________________
Bool_t T49EventMCRoot::ReCreateArrays()
{
  //
  // Creates new static particle lists
  //

  delete fgStaticMCParticles;

  fgStaticMCParticles = new TClonesArray("T49ParticleMCRoot", 200);

  if (fgStaticMCParticles != NULL) {
   fMCParticles = fgStaticMCParticles;
   return T49EventRoot::ReCreateArrays();
  }
  else {
    return kFALSE;
  }

}

//______________________________________________________________________________
void T49EventMCRoot::Streamer(TBuffer &R__b)
{
//
//   Stream an object of class T49EventMCRoot.
//

  UInt_t R__s, R__c;
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v > 1) {
      T49EventMCRoot::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
    }
    T49EventRoot::Streamer(R__b);
    fMCParticles->Streamer(R__b);
    R__b >> fMCVertices;
    R__b.CheckByteCount(R__s, R__c, T49EventMCRoot::IsA());
  }
  else {
    T49EventMCRoot::Class()->WriteBuffer(R__b,this);
  }

}



























