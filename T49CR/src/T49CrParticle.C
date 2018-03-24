#include "stdlib.h"
#include "TObject.h"
#include "T49CrParticle.h"
#include "T49ParticleRoot.h"

#include "math.h"

ClassImp(T49CrParticle)

T49CrParticle::~T49CrParticle()
{
}

T49CrParticle::T49CrParticle()
{ 
  fCharge   = 0.0;
  fPtot     = 0.0;
  fPt       = 0.0;
  fPhi      = 0.0;
  fTmean    = 0.0;
  fNPoint   = 0;
  fProb[0]  = 0.0;
  fProb[1]  = 0.0;
  fProb[2]  = 0.0;
  fProb[3]  = 0.0;
}

void T49CrParticle::FillParticle(T49ParticleRoot *Particle)
{
  SetCharge(Particle->GetCharge());
  SetPtot(Particle->GetP());
  SetPt(Particle->GetPt());
  SetPhi(Particle->GetPhi());
  SetTmean(Particle->GetTmeanCharge(2)/1000);
  SetNPoint(Particle->GetNPoint(2));
}

void T49CrParticle::FillParticle(T49ParticleRoot *Particle,Int_t IsMTPC)
{
  SetCharge(Particle->GetCharge());
  SetPtot(Particle->GetP());
  SetPt(Particle->GetPt());
  SetPhi(Particle->GetPhi());
  if(IsMTPC)
    SetTmean(Particle->GetTmeanCharge(2)/1000);
  else
    SetTmean(Particle->GetTmeanCharge()/1000);
  SetNPoint(Particle->GetNPoint(2));
}
