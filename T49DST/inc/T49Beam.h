#ifndef _T49Beam_H_INCLUDED_
#define _T49Beam_H_INCLUDED_

/*
$Log: T49Beam.h,v $
Revision 1.5  2001/11/12 10:45:57  cblume
Update for ROOT v3.01

 * Revision 1.4  2000/12/21  12:04:38  cblume
 * T49BeamRoot.h
 *
 * Revision 1.3  2000/10/09  10:25:01  cblume
 * General update
 *
 * Revision 1.2  1999/11/23  12:51:04  cblume
 * Remove files
 *
*/

#include "TObject.h"

class T49Beam : public TObject {

public:
   T49Beam() { };
   virtual ~T49Beam() { };

   virtual    void      SetS1(Int_t i)       = 0;
   virtual    void      SetS2(Int_t i)       = 0;   
   virtual    void      SetV0(Int_t i)       = 0;
   virtual    void      SetS4(Int_t i)       = 0;
   virtual    void      SetScl(Int_t i)      = 0;
   virtual    void      SetBs4(Int_t i)      = 0;
   virtual    void      SetBung(Int_t i)     = 0;
   virtual    void      SetBgated(Int_t i)   = 0;
   virtual    void      SetBi(Int_t i)       = 0;
   virtual    void      SetTrigger(Int_t i)  = 0;
   virtual    void      SetAdcS1(Int_t i)    = 0;
   virtual    void      SetAdcS2(Int_t i)    = 0;
   virtual    void      SetAdcS3(Int_t i)    = 0;
   virtual    void      SetTdcVtpc1(Int_t i) = 0;
   virtual    void      SetTdcVtpc2(Int_t i) = 0;
   virtual    void      SetTdcMtpcl(Int_t i) = 0;
   virtual    void      SetTdcMtpcr(Int_t i) = 0;
   virtual    void      SetPatUnit1(Int_t i) = 0;
   virtual    void      SetPatUnit2(Int_t i) = 0;
   virtual    void      SetPatUnitEmpty(Int_t i) = 0;
   virtual    void      SetEmpty(Int_t i, Int_t empty) = 0;

   virtual    Int_t      GetS1()            = 0;
   virtual    Int_t      GetS2()            = 0;
   virtual    Int_t      GetV0()            = 0;
   virtual    Int_t      GetS4()            = 0;
   virtual    Int_t      GetScl()           = 0;
   virtual    Int_t      GetBs4()           = 0;
   virtual    Int_t      GetBung()          = 0;
   virtual    Int_t      GetBgated()        = 0;
   virtual    Int_t      GetBi()            = 0;
   virtual    Int_t      GetTrigger()       = 0;
   virtual    Int_t      GetAdcS1()         = 0;
   virtual    Int_t      GetAdcS2()         = 0;
   virtual    Int_t      GetAdcS3()         = 0;
   virtual    Int_t      GetTdcVtpc1()      = 0;
   virtual    Int_t      GetTdcVtpc2()      = 0;
   virtual    Int_t      GetTdcMtpcl()      = 0;
   virtual    Int_t      GetTdcMtpcr()      = 0;
   virtual    Int_t      GetPatUnit1()      = 0;
   virtual    Int_t      GetPatUnit2()      = 0;
   virtual    Int_t      GetPatUnitEmpty()  = 0;
   virtual    Int_t      GetEmpty(Int_t i)  = 0;

   ClassDef(T49Beam,2) // Beam base class
   
protected:

};

#endif
