#ifndef _T49VertexMCRoot_INCLUDED_
#define _T49VertexMCRoot_INCLUDED_

/*
$Log: T49VertexMCRoot.h,v $
Revision 1.5  2001/11/12 10:49:35  cblume
Update for ROOT v3.01

 * Revision 1.4  2000/02/23  15:44:55  cblume
 * Ok ok, lets allow four MC-daughter tracks
 *
 * Revision 1.3  2000/02/23  10:04:15  cblume
 * Added the possibility to store a third daughter particle
 *
 * Revision 1.2  2000/02/09  13:30:07  cblume
 * Bug fix in SetSecondDaughter()
 *
 * Revision 1.1  1999/11/23  12:55:27  cblume
 * Add T49VertexMCRoot.h
 *
*/

#include "TMath.h"
#include "T49VertexRoot.h"

class T49VertexMCRoot : public T49VertexRoot {

public:

  T49VertexMCRoot();
  T49VertexMCRoot(T49VertexMCRoot& mcVertex);
  virtual ~T49VertexMCRoot() { };

  virtual Int_t      GetParent()                { return fParent;    };
  virtual Int_t      GetFirstDaughter()         { return fDaughter1; };
  virtual Int_t      GetSecondDaughter()        { return fDaughter2; };
  virtual Int_t      GetThirdDaughter()         { return fDaughter3; };
  virtual Int_t      GetForthDaughter()         { return fDaughter4; };

  virtual void       SetParent(Int_t i)         { fParent    = i; };
  virtual void       SetFirstDaughter(Int_t i)  { fDaughter1 = i; };
  virtual void       SetSecondDaughter(Int_t i) { fDaughter2 = i; };
  virtual void       SetThirdDaughter(Int_t i)  { fDaughter3 = i; };
  virtual void       SetForthDaughter(Int_t i)  { fDaughter4 = i; };

protected:

  Int_t     fParent;                // Link to the MC parent track
  Int_t     fDaughter1;             // Link to first MC daughter track
  Int_t     fDaughter2;             // Link to second MC daughter track
  Int_t     fDaughter3;             // Link to third MC daughter track
  Int_t     fDaughter4;             // Link to forth MC daughter track

  ClassDef(T49VertexMCRoot,4)       // Monte Carlo vertex

};

#endif
