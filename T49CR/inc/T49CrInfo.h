#ifndef _T49CrInfo_H_INCLUDED_
#define _T49CrInfo_H_INCLUDED_

#include "math.h"

#include "TObject.h"

#include "PhysConst.h"

class T49CrInfo : public TObject {

public:
    T49CrInfo();

    ~T49CrInfo();

  virtual void SetPosition(Float_t x,Int_t i)    { fPosition[i] = x;      }
  virtual void SetAmplitude(Float_t x,Int_t i)   { fAmplitude[i] = x;     }

  virtual void SetChisqFix(Float_t x)            { fChisq  = x;           } 

  virtual void SetReso(Float_t x)                { fReso  = x;            } 

  virtual Float_t   GetPosition(Int_t i)         {  return  fPosition[i]; }
  virtual Float_t   GetAmplitude(Int_t i)        {  return  fAmplitude[i];}

  virtual Float_t   GetChisq()                   { return fChisq;         } 
  virtual Float_t   GetReso()                    { return fReso;          } 

    ClassDef(T49CrInfo,1)
   
protected:

  Float_t  fPosition[4];    
  Float_t  fAmplitude[4];

  Float_t  fReso;

  Float_t  fChisq;

};

#endif


