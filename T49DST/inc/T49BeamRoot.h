#ifndef _T49BeamRoot_H_INCLUDED_
#define _T49BeamRoot_H_INCLUDED_

/*
$Log: T49BeamRoot.h,v $
Revision 1.6  2001/12/12 17:33:27  cblume
Update default constructors

Revision 1.5  2001/11/12 10:46:17  cblume
Update for ROOT v3.01

 * Revision 1.4  2000/12/21  12:04:46  cblume
 * Include Get/SetEmpty()
 *
 * Revision 1.3  2000/10/09  10:25:02  cblume
 * General update
 *
 * Revision 1.2  1999/11/23  12:51:05  cblume
 * Remove files
 *
*/

#include <TBuffer.h>
#include "T49Beam.h"

class T49BeamRoot : public T49Beam {

public:

   T49BeamRoot();
   virtual ~T49BeamRoot() { };

   void       SetS1(Int_t i)                 { fS1           = i;     }
   void       SetS2(Int_t i)                 { fS2           = i;     }
   void       SetV0(Int_t i)                 { fV0           = i;     }
   void       SetS4(Int_t i)                 { fS4           = i;     }
   void       SetScl(Int_t i)                { fScl          = i;     }
   void       SetBs4(Int_t i)                { fBs4          = i;     }
   void       SetBung(Int_t i)               { fBung         = i;     }
   void       SetBgated(Int_t i)             { fBgated       = i;     }
   void       SetBi(Int_t i)                 { fBi           = i;     }
   void       SetTrigger(Int_t i)            { fTrigger      = i;     }
   void       SetAdcS1(Int_t i)              { fAdcS1        = i;     }
   void       SetAdcS2(Int_t i)              { fAdcS2        = i;     }
   void       SetAdcS3(Int_t i)              { fAdcS3        = i;     }
   void       SetTdcVtpc1(Int_t i)           { fTdcVtpc1     = i;     }
   void       SetTdcVtpc2(Int_t i)           { fTdcVtpc2     = i;     }
   void       SetTdcMtpcl(Int_t i)           { fTdcMtpcr     = i;     }
   void       SetTdcMtpcr(Int_t i)           { fTdcMtpcr     = i;     }
   void       SetPatUnit1(Int_t i)           { fPatUnit1     = i;     }
   void       SetPatUnit2(Int_t i)           { fPatUnit2     = i;     }
   void       SetPatUnitEmpty(Int_t i)       { fPatUnitEmpty = i;     }
   void       SetEmpty(Int_t i, Int_t empty) { fEmpty[i]     = empty; }

   Int_t      GetS1()            { return fS1;           }
   Int_t      GetS2()            { return fS2;           }
   Int_t      GetV0()            { return fV0;           }
   Int_t      GetS4()            { return fS4;           }
   Int_t      GetScl()           { return fScl;          }
   Int_t      GetBs4()           { return fBs4;          }
   Int_t      GetBung()          { return fBung;         }
   Int_t      GetBgated()        { return fBgated;       }
   Int_t      GetBi()            { return fBi;           }
   Int_t      GetTrigger()       { return fTrigger;      }
   Int_t      GetAdcS1()         { return fAdcS1;        }
   Int_t      GetAdcS2()         { return fAdcS2;        }
   Int_t      GetAdcS3()         { return fAdcS3;        }
   Int_t      GetTdcVtpc1()      { return fTdcVtpc1;     }
   Int_t      GetTdcVtpc2()      { return fTdcVtpc2;     }
   Int_t      GetTdcMtpcl()      { return fTdcMtpcr;     }
   Int_t      GetTdcMtpcr()      { return fTdcMtpcr;     }
   Int_t      GetPatUnit1()      { return fPatUnit1;     }
   Int_t      GetPatUnit2()      { return fPatUnit2;     }
   Int_t      GetPatUnitEmpty()  { return fPatUnitEmpty; }
   Int_t      GetEmpty(Int_t i)  { return fEmpty[i];     }

protected:

   Int_t      fS1;              // Scalers - Station 1
   Int_t      fS2;
   Int_t      fV0;
   Int_t      fS4;
   Int_t      fScl;
   Int_t      fBs4;
   Int_t      fBung;            // Station 2
   Int_t      fBgated;
   Int_t      fBi;
   Int_t      fTrigger;
   Int_t      fEmpty[14];       // 2 words of station 2, station 3 and 4
   Int_t      fAdcS1;           // ADC-Data  Station 7
   Int_t      fAdcS2;
   Int_t      fAdcS3;
   Int_t      fAdcEmpty[45];    // 9 wds of stat. 7, stat. 8,9, and 10
   Int_t      fTdcVtpc1;        // TDC-Data  Station 11
   Int_t      fTdcVtpc2;
   Int_t      fTdcMtpcl;
   Int_t      fTdcMtpcr;
   Int_t      fTdcEmpty[20];    // 4 wds of stat. 11, stat. 12, and 13
   Int_t      fPatUnit1;        // Pattern-Unit  Station 14
   Int_t      fPatUnit2;        // Station 15
   Int_t      fPatUnitEmpty;    // Station 16

   ClassDef(T49BeamRoot,2)      // Beam information

};

#endif
