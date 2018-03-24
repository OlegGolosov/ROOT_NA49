#ifndef _T49Index_H_INCLUDED_
#define _T49Index_H_INCLUDED_

/*
$Log: T49Index.h,v $
 * Revision 1.2  2000/01/30  14:46:11  cblume
 * New versions from Marco with updated comments
 *
 * Revision 1.1  2000/01/17  12:37:47  cblume
 * Add T49Index.h
 *
*/

#include "TObject.h"

class T49Index : public TObject {

public:
    T49Index();

    ~T49Index();

    void SetCharge(Float_t x)   { fCharge = x;      } 
    void SetPtot(Float_t x)     { fPtot = x;        }
    void SetPt(Float_t x)       { fPt = x;          }
    void SetPhi(Float_t x)      { fPhi = x;         }

    void SetChargeBin(Int_t i)  { fChargeBin = i;   } 
    void SetPtotBin(Int_t i)    { fPtotBin = i;     }
    void SetPtBin(Int_t i)      { fPtBin = i;       }
    void SetPhiBin(Int_t i)     { fPhiBin = i;      }
    
    Float_t    GetCharge()      { return fCharge;   }  
    Float_t    GetPtot()        { return fPtot;     } 
    Float_t    GetPt()          { return fPt;       } 
    Float_t    GetPhi()         { return fPhi;      }

    Int_t    GetChargeBin()     { return fChargeBin;}  
    Int_t    GetPtotBin()       { return fPtotBin;  } 
    Int_t    GetPtBin()         { return fPtBin;    } 
    Int_t    GetPhiBin()        { return fPhiBin;   }
 

    ClassDef(T49Index,1) // Index class for use with T49Container
   
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


