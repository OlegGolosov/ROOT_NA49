///////////////////////////////////////////////////////////////////////
//
//  Contains the ring calorimeter information
//
///////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TClass.h"

#include "T49RingRoot.h"

ClassImp(T49RingRoot)

/*
$Log: T49RingRoot.C,v $
Revision 1.4  2001/11/12 10:52:14  cblume
Update for ROOT v3.01

 * Revision 1.3  2000/04/13  15:11:36  cblume
 * Changed to real ring calo class
 *
 * Revision 1.2  1999/11/23  12:59:04  cblume
 * Remove files
 *
*/

//______________________________________________________________________________
T49RingRoot::T49RingRoot()
{
  //
  // T49RingRoot constructor
  //

  fVersion = 0;
  for (Int_t iRingADC = 0; iRingADC < kRingADC; iRingADC++) {
    fADCphoton[iRingADC] = 0;
    fADChadron[iRingADC] = 0;
  }

}

//______________________________________________________________________________
void T49RingRoot::Streamer(TBuffer &R__b)
{
  //
  // Stream an object of class T49RingRoot.
  //

  UInt_t R__s, R__c;
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v > 1) {
      T49RingRoot::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
    }
    T49Ring::Streamer(R__b);
    R__b >> fVersion;
    R__b.ReadStaticArray(fADCphoton);
    R__b.ReadStaticArray(fADChadron);
    R__b.CheckByteCount(R__s, R__c, T49RingRoot::IsA());
  }
  else {
    T49RingRoot::Class()->WriteBuffer(R__b,this);
//      R__c = R__b.WriteVersion(T49RingRoot::IsA(), kTRUE);
//      T49Ring::Streamer(R__b);
//      R__b << fVersion;
//      R__b.WriteArray(fADCphoton, 240);
//      R__b.WriteArray(fADChadron, 240);
//      R__b.SetByteCount(R__c, kTRUE);
  }

}

