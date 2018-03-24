#ifndef _T49DedxInfo_H_INCLUDED_
#define _T49DedxInfo_H_INCLUDED_

#include "TObject.h"

class T49DedxInfo : public TObject {

public:
    T49DedxInfo();

  virtual ~T49DedxInfo();

  void SetPosition(Float_t x,Int_t i)    { fPosition[i] = x;      }
  void SetAmplitude(Float_t x,Int_t i)   { fAmplitude[i] = x;     }

  void SetPosError(Float_t x,Int_t i)    { fPosError[i] = x;      }
  void SetAmpError(Float_t x,Int_t i)    { fAmpError[i] = x;      }

  void SetChiSq(Float_t x)               { fChisq  = x;           } 
  void SetReso(Float_t x)                { fReso  = x;            } 
  void SetResError(Float_t x)            { fResError  = x;        } 
  void SetAsymmetry(Float_t x)           { fAsym  = x;            } 

  Float_t   GetPosition(Int_t i)         { return  fPosition[i];  }
  Float_t   GetAmplitude(Int_t i)        { return  fAmplitude[i]; }

  Float_t   GetPosError(Int_t i)         { return  fPosError[i];  }
  Float_t   GetAmpError(Int_t i)         { return  fAmpError[i];  }

  Float_t   GetChiSq()                   { return fChisq;         } 
  Float_t   GetReso()                    { return fReso;          } 
  Float_t   GetResError()                { return fResError;      } 
  Float_t   GetAsymmetry()               { return fAsym;          } 

  ClassDef(T49DedxInfo,4);  // Class to hold dE/dx fit results in T49Container
   
protected:

  Float_t  fPosition[5];    
  Float_t  fAmplitude[5];

  Float_t  fReso;
  Float_t  fChisq;

  Float_t  fPosError[5];    
  Float_t  fAmpError[5];
  Float_t  fResError;

  Float_t  fAsym;
};

#endif


