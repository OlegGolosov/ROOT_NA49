/*
$Log: T49AbsClasses.C,v $
Revision 1.5  2008/01/04 10:48:59  cblume
64bit modifications by Marek Szuba

Revision 1.4  2002/02/04 16:06:22  cblume
Add T49Veto

Revision 1.3  2001/11/12 10:50:35  cblume
Update for ROOT v3.01

 * Revision 1.2  1999/11/23  12:58:56  cblume
 * Remove files
 *
*/

#include <cstdlib>

#include <TObject.h>
#include <TClass.h>
#include <TBuffer.h>

#include "T49Event.h"
#include "T49Particle.h"
#include "T49Vertex.h"
#include "T49MonitorTPC.h"
#include "T49Beam.h"
#include "T49Ring.h"
#include "T49Veto.h"

ClassImp(T49Event)
//_____________________________________________________________________________
void T49Event::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49Event.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
       if (R__v > 1) {
         T49Event::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      TObject::Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, T49Event::IsA());
   } else {
      T49Event::Class()->WriteBuffer(R__b,this);
//        R__c = R__b.WriteVersion(T49Event::IsA(), kTRUE);
//        TObject::Streamer(R__b);
//        R__b.SetByteCount(R__c, kTRUE);
   }
}

ClassImp(T49Vertex)

//______________________________________________________________________________
void T49Vertex::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49Vertex.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
       if (R__v > 1) {
         T49Vertex::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      TObject::Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, T49Vertex::IsA());
   } else {
      T49Vertex::Class()->WriteBuffer(R__b,this);
//        R__c = R__b.WriteVersion(T49Vertex::IsA(), kTRUE);
//        TObject::Streamer(R__b);
//        R__b.SetByteCount(R__c, kTRUE);
   }
}

ClassImp(T49Particle)

//______________________________________________________________________________
void T49Particle::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49Particle.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
       if (R__v > 1) {
         T49Particle::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      TObject::Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, T49Particle::IsA());
   } else {
      T49Particle::Class()->WriteBuffer(R__b,this);
//        R__c = R__b.WriteVersion(T49Particle::IsA(), kTRUE);
//        TObject::Streamer(R__b);
//        R__b.SetByteCount(R__c, kTRUE);
   }
}

ClassImp(T49MonitorTPC)

//______________________________________________________________________________
void T49MonitorTPC::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49MonitorTPC.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
       if (R__v > 1) {
         T49MonitorTPC::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      TObject::Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, T49MonitorTPC::IsA());
   } else {
      T49MonitorTPC::Class()->WriteBuffer(R__b,this);
//        R__c = R__b.WriteVersion(T49MonitorTPC::IsA(), kTRUE);
//        TObject::Streamer(R__b);
//        R__b.SetByteCount(R__c, kTRUE);
   }
}

ClassImp(T49Beam)

//______________________________________________________________________________
void T49Beam::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49Beam.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
       if (R__v > 1) {
         T49Beam::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      TObject::Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, T49Beam::IsA());
   } else {
      T49Beam::Class()->WriteBuffer(R__b,this);
//        R__c = R__b.WriteVersion(T49Beam::IsA(), kTRUE);
//        TObject::Streamer(R__b);
//        R__b.SetByteCount(R__c, kTRUE);
   }
}

ClassImp(T49Ring)

//______________________________________________________________________________
void T49Ring::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49Ring.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
       if (R__v > 1) {
         T49Ring::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      TObject::Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, T49Ring::IsA());
   } else {
      T49Ring::Class()->WriteBuffer(R__b,this);
//        R__c = R__b.WriteVersion(T49Ring::IsA(), kTRUE);
//        TObject::Streamer(R__b);
//        R__b.SetByteCount(R__c, kTRUE);
   }
}

ClassImp(T49Veto)












