#ifndef _T49RingRoot_H_INCLUDED_
#define _T49RingRoot_H_INCLUDED_

/*
$Log: T49RingRoot.h,v $
Revision 1.5  2001/11/12 10:48:57  cblume
Update for ROOT v3.01

 * Revision 1.4  2000/10/09  10:25:06  cblume
 * General update
 *
 * Revision 1.3  2000/04/13  14:53:44  cblume
 * Remove files
 *
 * Revision 1.2  1999/11/23  12:51:27  cblume
 * Remove files
 *
*/

#include <TBuffer.h>
#include "T49Ring.h"

const Int_t kRingADC = 240;

class T49RingRoot : public T49Ring {

public:

  T49RingRoot();
  virtual ~T49RingRoot() { };

  virtual void    SetVersion(Int_t v)                { fVersion      = v;    };
  virtual void    SetADCphoton(Int_t i, Float_t adc) { fADCphoton[i] = adc;  };
  virtual void    SetADChadron(Int_t i, Float_t adc) { fADChadron[i] = adc;  };

  virtual Float_t GetVersion()                       { return fVersion;      };
  virtual Float_t GetADCphoton(Int_t i)              { return fADCphoton[i]; };
  virtual Float_t GetADChadron(Int_t i)              { return fADChadron[i]; };

protected:

  Float_t      fVersion;               // Structure version number
  Float_t      fADCphoton[kRingADC];   // Stations 1-20 of crate 2
  Float_t      fADChadron[kRingADC];   // Stations 1-20 of crate 3

  ClassDef(T49RingRoot,2)              // Ring calorimeter information

};

#endif
