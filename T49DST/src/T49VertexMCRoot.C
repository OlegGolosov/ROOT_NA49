///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Root mini-DST class for Monte-Carlo vertices                   //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "T49VertexMCRoot.h"

ClassImp(T49VertexMCRoot)

/*
$Log: T49VertexMCRoot.C,v $
Revision 1.6  2001/12/12 17:39:02  cblume
Update default constructors

Revision 1.5  2001/11/12 10:52:30  cblume
Update for ROOT v3.01

 * Revision 1.4  2000/11/09  09:08:17  cblume
 * Introduce the byte count in the streamer
 *
 * Revision 1.3  2000/02/23  15:43:00  cblume
 * Ok ok, lets allow four MC-daughter tracks
 *
 * Revision 1.2  2000/02/23  10:05:21  cblume
 * Added the possibility to store a third daughter particle
 *
 * Revision 1.1  1999/11/23  12:59:47  cblume
 * Add T49VertexMCRoot.C
 *
*/

//______________________________________________________________________________
T49VertexMCRoot::T49VertexMCRoot()
                :T49VertexRoot(kFALSE)
{
  //
  // T49DstMcVertex default constructor
  //

  fParent    = -1;
  fDaughter1 = -1;
  fDaughter2 = -1;
  fDaughter3 = -1;
  fDaughter4 = -1;

}

//______________________________________________________________________________

T49VertexMCRoot::T49VertexMCRoot(T49VertexMCRoot& mcVertex)
                :T49VertexRoot(mcVertex)
{
  //
  // T49DstMcVertex copy constructor
  //

  fParent    = mcVertex.GetParent();
  fDaughter1 = mcVertex.GetFirstDaughter();
  fDaughter2 = mcVertex.GetSecondDaughter();
  fDaughter3 = mcVertex.GetThirdDaughter();
  fDaughter4 = mcVertex.GetForthDaughter();

}

//______________________________________________________________________________
void T49VertexMCRoot::Streamer(TBuffer &R__b)
{
  //
  // Stream an object of class T49VertexMCRoot.
  //

  UInt_t R__s, R__c;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v > 3) {
      T49VertexMCRoot::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
    }
    T49VertexRoot::Streamer(R__b);
    R__b >> fParent;
    R__b >> fDaughter1;
    R__b >> fDaughter2;
    if (R__v > 1) {
      R__b >> fDaughter3;
    }
    if (R__v > 2) {
      R__b >> fDaughter4;
    }
    R__b.CheckByteCount(R__s, R__c, T49VertexMCRoot::IsA());
  }
  else {
    T49VertexMCRoot::Class()->WriteBuffer(R__b,this);
  }

}


