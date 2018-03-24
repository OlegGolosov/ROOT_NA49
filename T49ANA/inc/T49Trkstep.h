#ifndef _T49Trkstep_INCLUDED_
#define _T49Trkstep_INCLUDED_

#include "TObject.h"

class T49Trkstep : public TObject {

 public:

  T49Trkstep();
  virtual ~T49Trkstep() { };

  Int_t    InitDS(Char_t *dspack_server = "");
  Int_t    TrackTo(Float_t z);

  void     SetKeys(Char_t *key1, Char_t *key3, Char_t *key4
                 , Char_t *key5, Char_t *key6, Char_t *key7);
  void     SetKeys96STDp();
  void     SetP(Float_t px, Float_t py, Float_t pz); 
  void     SetPos(Float_t x, Float_t y, Float_t z);

  void     SetCharge(Float_t q)      { fCharge   = q;     };
  void     SetFixStep(Int_t step)    { fFixStep  = step;  };
  void     SetStepSize(Float_t size) { fStepSize = size;  };
  void     SetCheck(Int_t check)     { fCheck    = check; };

  Float_t  GetPx()       { return fPstop[0]; }; 
  Float_t  GetPy()       { return fPstop[1]; }; 
  Float_t  GetPz()       { return fPstop[2]; }; 
  Float_t  GetX()        { return fXstop[0]; }; 
  Float_t  GetY()        { return fXstop[1]; }; 
  Float_t  GetZ()        { return fXstop[2]; }; 
  Float_t  GetCharge()   { return fCharge;   };
  Int_t    GetFixStep()  { return fFixStep;  };
  Float_t  GetStepSize() { return fStepSize; };
  Int_t    GetCheck()    { return fCheck;    };

 private:

  Float_t fCharge;       // Track charge
  Float_t fXstart[3];    // Initial position
  Float_t fPstart[3];    // Momentum components at the initial point
  Float_t fXstop[3];     // Final position
  Float_t fPstop[3];     // Momentum components at the final point
  Int_t   fFixStep;      // 1: track in fixed steps of fStepSize. 0: use adaptive step size algorithm
  Float_t fStepSize;     // Distance (cm) to be used in fixed step approach
  Int_t   fCheck;        // 1: check against bounds of field map. 0: don't - you know where you're going
  
  ClassDef(T49Trkstep,0) // Trkstep interface class

};

#endif



