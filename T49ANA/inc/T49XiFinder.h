#ifndef _T49XiFinder_INCLUDED_
#define _T49XiFinder_INCLUDED_

#include "TObject.h"

class TObjArray;

class T49Trkstep;

class T49XiFinder : public TObject {

 public:

  T49XiFinder();
  virtual ~T49XiFinder();

  virtual void       InitGlobalParameter();
  virtual Bool_t     FindXis();

  virtual void       SetTrkstepCharged(T49Trkstep *trkstep);
  virtual void       SetTrkstepXi(T49Trkstep *trkstep);

  virtual void       SetNeutralInput(TObjArray *inputArray) { fNeutralInputArray = inputArray; }
  virtual void       SetChargedInput(TObjArray *inputArray) { fChargedInputArray = inputArray; }

  virtual void       CreateNeutralInput(TObjArray *inputArray, Bool_t primary = kFALSE);
  virtual void       CreateChargedInput(TObjArray *inputArray, Bool_t primary = kFALSE);

  virtual void       DeleteNeutralInput();
  virtual void       DeleteChargedInput();

  virtual TObjArray *GetXis()               { return fXiArray;              }
  virtual TObjArray *GetNeutralDaughters()  { return fNeutralDaughterArray; }
  virtual TObjArray *GetChargedDaughters()  { return fChargedDaughterArray; }
  virtual TObjArray *GetNeutralInputArray() { return fNeutralInputArray;    }
  virtual TObjArray *GetChargedInputArray() { return fChargedInputArray;    }

          void       SetV0Zmin(Float_t z)   { fV0Zmin  = z; }
          void       SetVerbose(Int_t v=1)  { fVerbose = v; } 

 private:

  TObjArray  *fNeutralInputArray;    //! Array of neutral input particles
  TObjArray  *fChargedInputArray;    //! Array of charged input particles
 
  TObjArray  *fXiArray;              //! Array of found Xi/Omega candidates
  TObjArray  *fNeutralDaughterArray; //! Array of neutral Xi/Omega daughters
  TObjArray  *fChargedDaughterArray; //! Array of charged Xi/Omega daughters

  T49Trkstep *fTrkstepCharged;       //! Trkstep for the charged daughters
  T49Trkstep *fTrkstepXi;            //! Trkstep for the Xi/Omega candidates

  Float_t     fZtarget;              //  Target z position
  Float_t     fZmin;                 //  Minimal z position for which the Xi search is done
  Float_t     fZstepsize;            //  Step size along z
  Float_t     fDcaXcut;              //  Cut in dca x
  Float_t     fDcaYcut;              //  Cut in dca y
  Float_t     fDcaRcut;              //  Cut in dca r
  Float_t     fSideCut;              //  Same / opposite side cut
  Float_t     fPtarmCut;             //  Cut in Xi/Omega Armenteros Pt
  Float_t     fPtMaxCut;             //  Cut in Xi/Omega Pt
  Float_t     fPtotMaxCut;           //  Cut in Xi/Omega Ptot
  Float_t     fXtargCut;             //  Cut in Xi/Omega xtarget
  Float_t     fYtargCut;             //  Cut in Xi/Omega ytarget
  Float_t     fV0MassMin;            //  Defines the (anti)lambda mass window 
  Float_t     fV0MassMax;            //  Defines the (anti)lambda mass window 
  Float_t     fV0Zmin;               //  Minimal z position for (anti)lambda decay vertex
  Float_t     fV0ArmPtMax;           //  Maximal Armenteros Pt for the (anti)lambda
  Float_t     fChargedXtargMin;      //  Lower cut in the x target impact parameter for the charged track
  Float_t     fChargedYtargMin;      //  Lower cut in the y target impact parameter for the charged track
  Int_t       fChargedNPointMin;     //  Lower cut in the number of points for the charged track
  Int_t       fVerbose;              //  Verbose flag

  ClassDef(T49XiFinder,0)            //  Xifinder class

};

#endif



