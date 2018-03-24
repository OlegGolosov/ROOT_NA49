#ifndef _T49Ring_H_INCLUDED_
#define _T49Ring_H_INCLUDED_

/*
$Log: T49Ring.h,v $
Revision 1.5  2001/11/12 10:48:50  cblume
Update for ROOT v3.01

 * Revision 1.4  2000/10/09  10:25:05  cblume
 * General update
 *
 * Revision 1.3  2000/04/13  14:53:44  cblume
 * Remove files
 *
 * Revision 1.2  1999/11/23  12:51:24  cblume
 * Remove files
 *
*/

#include "TObject.h"

class T49Ring : public TObject {

public:

  T49Ring() { };
  virtual ~T49Ring() { };

  virtual void    SetVersion(Int_t v)                = 0;
  virtual void    SetADCphoton(Int_t i, Float_t adc) = 0;
  virtual void    SetADChadron(Int_t i, Float_t adc) = 0;
   
  virtual Float_t GetVersion()                       = 0;
  virtual Float_t GetADCphoton(Int_t i)              = 0;
  virtual Float_t GetADChadron(Int_t i)              = 0;

  ClassDef(T49Ring,2) // Ring calorimeter base class
   
protected:

};

#endif
