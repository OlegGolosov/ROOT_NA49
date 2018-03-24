#ifndef _T49EventMCRoot_INCLUDED_
#define _T49EventMCRoot_INCLUDED_

/*
$Log: T49EventMCRoot.h,v $
Revision 1.5  2002/10/07 09:58:32  cblume
Update for CASTOR

Revision 1.4  2001/12/12 17:34:37  cblume
Update default constructors

Revision 1.3  2001/11/12 10:47:17  cblume
Update for ROOT v3.01

 * Revision 1.2  2000/10/09  10:25:03  cblume
 * General update
 *
 * Revision 1.1  1999/11/23  12:55:36  cblume
 * Add T49EventMCRoot.h
 *
*/

#include "T49EventRoot.h"

class T49EventMCRoot : public T49EventRoot {

 public:

  T49EventMCRoot();
  T49EventMCRoot(Int_t AllocObjects);
  virtual ~T49EventMCRoot();

  virtual void          ClearParticleLists();    
  virtual void          ResetMCParticles()                    { fMCParticles->Clear(); };
  virtual void          ResetMCVertices()                     { fMCVertices->Clear();  };
  virtual Bool_t        ReCreateArrays();    

  virtual TClonesArray *GetMCParticles()                      { return fMCParticles; };
  virtual TObjArray    *GetMCVertices()                       { return fMCVertices; };

  virtual void          SetMCParticleList(TClonesArray *list) { fMCParticles = list; };
  virtual void          SetMCVertexList(TObjArray *list)      { fMCVertices  = list; };

 protected:

  TClonesArray  *fMCParticles;      // List of Monte Carlo particles
  TObjArray     *fMCVertices;       // List of Monte Carlo vertices

  // Added for version 3
  static TClonesArray *fgStaticMCParticles;
 
  ClassDef(T49EventMCRoot,3)        // Event information including Monte Carlo data

};

#endif










