///////////////////////////////////////////////////////////////////////
//
//  Contains the monitor information
//
///////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TClass.h"

#include "T49MonitorTPCRoot.h"

ClassImp(T49MonitorTPCRoot)

/*
$Log: T49MonitorTPCRoot.C,v $
Revision 1.4  2001/12/12 17:37:48  cblume
Update default constructors

Revision 1.3  2001/11/12 10:51:38  cblume
Update for ROOT v3.01

 * Revision 1.2  1999/11/23  12:59:02  cblume
 * Remove files
 *
*/

//______________________________________________________________________________
T49MonitorTPCRoot::T49MonitorTPCRoot()
{
  //
  // T49MonitorTPCRoot default constructor
  //

  fNRun         = 0;
  fNEvent       = 0;
  fDate         = 0;
  fTime         = 0;
  fNPoint       = 0;
  fNItrack      = 0;
  fNRtrack      = 0;
  fNRtrackKpass = 0;
  fNTrack       = 0;
  fNVertex      = 0;
  fSumAdc       = 0;
  fTmeanCharge  = 0;
  fAvgResX      = 0;
  fAvgResY      = 0;
  fCurVdrift    = 0;
  fAveVdrift    = 0;
  fRawVdrift    = 0;
  fMonVdrift    = 0;
  fIflagVdMod   = 0;
  fGasGain      = 0;
  fEvtGain      = 0;
  fTemperature  = 0;
  fPressure     = 0;

}

//______________________________________________________________________________
void T49MonitorTPCRoot::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49MonitorTPCRoot.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
        T49MonitorTPCRoot::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
        return;
      }
      T49MonitorTPC::Streamer(R__b);
      R__b >> fNRun;
      R__b >> fNEvent;
      R__b >> fDate;
      R__b >> fTime;
      R__b >> fNPoint;
      R__b >> fNItrack;
      R__b >> fNRtrack;
      R__b >> fNRtrackKpass;
      R__b >> fNTrack;
      R__b >> fNVertex;
      R__b >> fSumAdc;
      R__b >> fTmeanCharge;
      R__b >> fAvgResX;
      R__b >> fAvgResY;
      R__b >> fCurVdrift;
      R__b >> fAveVdrift;
      R__b >> fRawVdrift;
      R__b >> fMonVdrift;
      R__b >> fIflagVdMod;
      R__b >> fGasGain;
      R__b >> fEvtGain;
      R__b >> fTemperature;
      R__b >> fPressure;
      R__b.CheckByteCount(R__s, R__c, T49MonitorTPCRoot::IsA());
   } else {
      T49MonitorTPCRoot::Class()->WriteBuffer(R__b,this);
   }

}


