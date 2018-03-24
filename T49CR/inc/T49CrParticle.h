#ifndef _T49CrParticle_H_INCLUDED_
#define _T49CrParticle_H_INCLUDED_

#include "math.h"

#include "TObject.h"

#include "PhysConst.h"
#include "T49ParticleRoot.h"

class T49CrParticle : public TObject {

public:
    T49CrParticle();

    ~T49CrParticle();

  virtual void SetCharge(Float_t x)            { fCharge  = x;    } 
  virtual void SetPtot(Float_t x)              { fPtot    = x;    } 
  virtual void SetPt(Float_t x)                { fPt      = x;    } 
  virtual void SetPhi(Float_t x)               { fPhi     = x;    } 
  virtual void SetTmean(Float_t x)             { fTmean   = x;    } 
  virtual void SetProb(Float_t x,Int_t i)      { fProb[i] = x;    } 
  virtual void SetNPoint(Int_t i)              { fNPoint  = i;    } 

  virtual void FillParticle(T49ParticleRoot *Particle,Int_t IsMTPC);
  virtual void FillParticle(T49ParticleRoot *T49Particle);
  virtual Float_t GetCharge()            { return fCharge;  } 
  virtual Float_t GetPtot()              { return fPtot;    } 
  virtual Float_t GetPt()                { return fPt;      } 
  virtual Float_t GetPhi()               { return fPhi;     } 
  virtual Float_t GetTmean()             { return fTmean;   } 
  virtual Float_t GetProb(Int_t i)       { return fProb[i]; } 
  virtual Int_t   GetNPoint()            { return fNPoint;  } 


  ClassDef(T49CrParticle,1)
   
protected:

  Float_t  fCharge;    
  Float_t  fPtot;
  Float_t  fPt;
  Float_t  fPhi;
  Float_t  fTmean;
  Int_t    fNPoint;
  Float_t  fProb[4];
};

#endif


