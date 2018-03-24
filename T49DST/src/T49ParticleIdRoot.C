///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Particle-class. Contains additional                            //
//    information about the pid-propability.                         //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "TClass.h"

#include "T49ParticleIdRoot.h"

ClassImp(T49ParticleIdRoot)

/*
$Log: T49ParticleIdRoot.C,v $
Revision 1.3  2001/12/12 17:38:02  cblume
Update default constructors

Revision 1.2  2001/11/12 10:51:50  cblume
Update for ROOT v3.01

 * Revision 1.1  1999/11/23  13:00:00  cblume
 * Add T49ParticleIdRoot.C
 *
*/

//______________________________________________________________________________
T49ParticleIdRoot::T49ParticleIdRoot():T49ParticleRoot()
{
  //
  // T49ParticleIdRoot default constructor
  //

  fProbElectron = 0;
  fProbPion     = 0;
  fProbKaon     = 0;
  fProbProton   = 0;
  fSigElectron  = 0;
  fSigPion      = 0;
  fSigKaon      = 0;
  fSigProton    = 0;

}

//______________________________________________________________________________
T49ParticleIdRoot::T49ParticleIdRoot(T49ParticleIdRoot *Track)
                  :T49ParticleRoot(Track)
{
  //
  // T49ParticleIdRoot copy constructor
  //

  SetProbElectron(Track->GetProbElectron());
  SetProbPion(Track->GetProbPion());
  SetProbKaon(Track->GetProbKaon());
  SetProbProton(Track->GetProbProton());

  SetSigElectron(Track->GetSigElectron());
  SetSigPion(Track->GetSigPion());
  SetSigKaon(Track->GetSigKaon());
  SetSigProton(Track->GetSigProton());

}

//______________________________________________________________________________
void T49ParticleIdRoot::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49ParticleIdRoot.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
       if (R__v > 1) {
         T49ParticleIdRoot::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
         return;
      }
      T49ParticleRoot::Streamer(R__b);
      R__b >> fProbElectron;
      R__b >> fProbPion;
      R__b >> fProbKaon;
      R__b >> fProbProton;
      R__b >> fSigElectron;
      R__b >> fSigPion;
      R__b >> fSigKaon;
      R__b >> fSigProton;
      R__b.CheckByteCount(R__s, R__c, T49ParticleIdRoot::IsA());
   } else {
      T49ParticleIdRoot::Class()->WriteBuffer(R__b,this);
   }

}










