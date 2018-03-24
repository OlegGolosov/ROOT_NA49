#ifndef _T49CrIndex_H_INCLUDED_
#define _T49CrIndex_H_INCLUDED_

#include "math.h"

#include "TObject.h"

#include "PhysConst.h"

class T49CrIndex : public TObject {

public:
    T49CrIndex();

    ~T49CrIndex();

    virtual void SetCharge(Float_t x)   { fCharge = x;      } 
    virtual void SetPtot(Float_t x)     { fPtot = x;        }
    virtual void SetPt(Float_t x)       { fPt = x;          }
    virtual void SetPhi(Float_t x)      { fPhi = x;         }

    virtual void SetChargeBin(Int_t i)  { fChargeBin = i;   } 
    virtual void SetPtotBin(Int_t i)    { fPtotBin = i;     }
    virtual void SetPtBin(Int_t i)      { fPtBin = i;       }
    virtual void SetPhiBin(Int_t i)     { fPhiBin = i;      }
    
    virtual Float_t    GetCharge()      { return fCharge;   }  
    virtual Float_t    GetPtot()        { return fPtot;     } 
    virtual Float_t    GetPt()          { return fPt;       } 
    virtual Float_t    GetPhi()         { return fPhi;      }

    virtual Int_t    GetChargeBin()     { return fChargeBin;}  
    virtual Int_t    GetPtotBin()       { return fPtotBin;  } 
    virtual Int_t    GetPtBin()         { return fPtBin;    } 
    virtual Int_t    GetPhiBin()        { return fPhiBin;   }
 

    ClassDef(T49CrIndex,1)
   
protected:

    Float_t    fCharge;
    Float_t    fPtot;    
    Float_t    fPt;    
    Float_t    fPhi;

    Int_t    fChargeBin;
    Int_t    fPtotBin;    
    Int_t    fPtBin;    
    Int_t    fPhiBin;
};

#endif


