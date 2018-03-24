///////////////////////////////////////////////////////////////////////
//
//  Contains the beam information
//
///////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TClass.h"

#include "T49BeamRoot.h"

ClassImp(T49BeamRoot)

/*
$Log: T49BeamRoot.C,v $
Revision 1.4  2001/12/12 17:37:01  cblume
Update default constructors

Revision 1.3  2001/11/12 10:50:47  cblume
Update for ROOT v3.01

 * Revision 1.2  1999/11/23  12:58:57  cblume
 * Remove files
 *
*/

//______________________________________________________________________________
T49BeamRoot::T49BeamRoot()
{
  //
  // Default constructor
  //

  Int_t i;

  fS1           = 0;
  fS2           = 0;
  fV0           = 0;
  fS4           = 0;
  fScl          = 0;
  fBs4          = 0;
  fBung         = 0;
  fBgated       = 0;
  fBi           = 0;
  fTrigger      = 0;
  for (i = 0; i < 14; i++) {
    fEmpty[i] = 0;
  }
  fAdcS1        = 0;
  fAdcS2        = 0;
  fAdcS3        = 0;
  for (i = 0; i < 45; i++) {
    fAdcEmpty[i] = 0;
  }
  fTdcVtpc1     = 0;
  fTdcVtpc2     = 0;
  fTdcMtpcl     = 0;
  fTdcMtpcr     = 0;
  for (i = 0; i < 20; i++) {
    fTdcEmpty[i] = 0;
  }
  fPatUnit1     = 0;
  fPatUnit2     = 0;
  fPatUnitEmpty = 0;

}

//______________________________________________________________________________
void T49BeamRoot::Streamer(TBuffer &R__b)
{
  //
  // Stream an object of class T49BeamRoot.
  //

  UInt_t R__s, R__c;
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v > 1) {
      T49BeamRoot::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
    }
    T49Beam::Streamer(R__b);
    R__b >> fS1;
    R__b >> fS2;
    R__b >> fV0;
    R__b >> fS4;
    R__b >> fScl;
    R__b >> fBs4;
    R__b >> fBung;
    R__b >> fBgated;
    R__b >> fBi;
    R__b >> fTrigger;
    R__b.ReadStaticArray(fEmpty);
    R__b >> fAdcS1;
    R__b >> fAdcS2;
    R__b >> fAdcS3;
    R__b.ReadStaticArray(fAdcEmpty);
    R__b >> fTdcVtpc1;
    R__b >> fTdcVtpc2;
    R__b >> fTdcMtpcl;
    R__b >> fTdcMtpcr;
    R__b.ReadStaticArray(fTdcEmpty);
    R__b >> fPatUnit1;
    R__b >> fPatUnit2;
    R__b >> fPatUnitEmpty;
    R__b.CheckByteCount(R__s, R__c, T49BeamRoot::IsA());
  }
  else {
    T49BeamRoot::Class()->WriteBuffer(R__b,this);
  }

}


