#ifndef _T49VetoRoot_H_INCLUDED_
#define _T49VetoRoot_H_INCLUDED_

/*
$Log: T49VetoRoot.h,v $
Revision 1.2  2002/03/27 14:01:53  cblume
Include GetEveto() with calibration for 40GeV min bias

Revision 1.1  2002/02/04 16:02:22  cblume
Add T49VetoRoot.h

*/

#include "T49Veto.h"

const Int_t kVetoADC = 4;

class T49VetoRoot : public T49Veto {

public:

  T49VetoRoot();
  virtual ~T49VetoRoot() { };

  virtual void    SetVersion(Int_t v)                { fVersion      = v;    };
  virtual void    SetADCphoton(Int_t i, Float_t adc) { fADCphoton[i] = adc;  };
  virtual void    SetADChadron(Int_t i, Float_t adc) { fADChadron[i] = adc;  };
   
  virtual Float_t GetVersion()                       { return fVersion;      };
  virtual Float_t GetADCphoton(Int_t i)              { return fADCphoton[i]; };
  virtual Float_t GetADChadron(Int_t i)              { return fADChadron[i]; };
  virtual Float_t GetEveto();

protected:

  Float_t      fVersion;               // Structure version number
  Float_t      fADCphoton[kVetoADC];   // ADC values of photon section
  Float_t      fADChadron[kVetoADC];   // ADC values of hadron section

  ClassDef(T49VetoRoot,1)              // Veto calorimeter information

};

#endif

