#ifndef _T49Veto_H_INCLUDED_
#define _T49Veto_H_INCLUDED_

/*
$Log: T49Veto.h,v $
Revision 1.1  2002/02/04 16:02:11  cblume
Add T49Veto.h

*/

#include "TObject.h"

class T49Veto : public TObject {

public:

  T49Veto() { };
  virtual ~T49Veto() { };

  virtual void    SetVersion(Int_t v)                = 0;
  virtual void    SetADCphoton(Int_t i, Float_t adc) = 0;
  virtual void    SetADChadron(Int_t i, Float_t adc) = 0;
   
  virtual Float_t GetVersion()                       = 0;
  virtual Float_t GetADCphoton(Int_t i)              = 0;
  virtual Float_t GetADChadron(Int_t i)              = 0;

  ClassDef(T49Veto,1) // Veto calorimeter base class

};

#endif
