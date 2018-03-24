#include "T49DedxInfo.h"
#include "TClass.h"

/*
$Log: T49DedxInfo.C,v $
Revision 1.3  2002/10/07 09:48:44  cblume
Update for CASTOR

 * Revision 1.2  2000/07/07  14:12:49  cblume
 * Made ROOT v2.25 compatible by Marco
 *
 * Revision 1.1  2000/01/17  12:38:10  cblume
 * Add T49DedxInfo.C
 *
*/

ClassImp(T49DedxInfo)


T49DedxInfo::T49DedxInfo()
{ 
  for(Int_t i = 0;i<4;i++) {
    fPosition[i]  = 0.0;    
    fAmplitude[i] = 0.0;      
    fPosError[i]  = 0.0;    
    fAmpError[i] = 0.0;      
  }

  fReso   = 0.0;
  fChisq  = 0.0;
  fAsym   = 0.0;
}

T49DedxInfo::~T49DedxInfo()
{
}

void T49DedxInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49DedxInfo.

   if (R__b.IsReading()) {
     UInt_t R__s, R__c;
     Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
     if (R__v > 2) {
       T49DedxInfo::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
       return;
     }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray(fPosition);
      R__b.ReadStaticArray(fAmplitude);
      R__b >> fReso;
      R__b >> fChisq;
      if (R__v > 1) {
	R__b.ReadStaticArray(fPosError);
	R__b.ReadStaticArray(fAmpError);
	R__b >> fResError;
      }
   } else {
      T49DedxInfo::Class()->WriteBuffer(R__b,this);
   }
}







