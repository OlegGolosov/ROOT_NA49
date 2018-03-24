#include "T49Tool.h"


ClassImp(T49Tool)

/*
$Log: T49Tool.C,v $
Revision 1.6  2001/11/21 17:02:46  cblume
Update

 * Revision 1.5  1999/11/23  13:54:15  cblume
 * Remove files
 *
*/

//______________________________________________________________________________
Double_t T49Tool::XF(T49ParticleRoot* t1,T49ParticleRoot* t2,
		     Float_t mPart1, Float_t mPart2, 
		     Float_t SQRTs)
{
  //
  // The Feynman-x of the particle pair
  //

  TLorentzVector p1(t1->GetPx(),t1->GetPy(),t1->GetPz(),t1->GetE(mPart1)) ;
  TLorentzVector p2(t2->GetPx(),t2->GetPy(),t2->GetPz(),t2->GetE(mPart2)) ;
  TLorentzVector P = p1 + p2;
  Double_t       gamma = SQRTs/2.0/kPMASS ;
  Double_t       beta  = TMath::Sqrt(1.0-1.0/(gamma*gamma));
  TLorentzVector Ptrans = zLorentz(&P,beta);
  return Ptrans.Z()*0.5/SQRTs;

}

//______________________________________________________________________________
Double_t T49Tool::Qinv(T49ParticleRoot* t1,T49ParticleRoot* t2, 
		       Float_t mPart1,Float_t mPart2)
{
  //
  // Qinv of the particle pair. Generalized expression that also works
  // for non-identical particles.
  //

  
  Double_t e1 = t1->GetE(mPart1);
  Double_t e2 = t2->GetE(mPart2);
  Double_t qinvL;
  Double_t qP;
  Double_t pinv;

  // First calculate -Qinv^2  as usual : 
  qinvL = (e1-e2) * (e1-e2) - Q(t1,t2) * Q(t1,t2);

  qP    = (e1-e2)   * (e1+e2)
        - Qx(t1,t2) * Px(t1,t2)
        - Qy(t1,t2) * Py(t1,t2)
        - Qz(t1,t2) * Pz(t1,t2);

  pinv  = (e1+e2) * (e1+e2) - P(t1,t2) * P(t1,t2);

  return TMath::Sqrt(qP*qP/pinv - qinvL);

}

//______________________________________________________________________________
Double_t T49Tool::Minv(T49ParticleRoot* t1,T49ParticleRoot* t2, 
  		       Float_t mPart1,Float_t mPart2)
{
  //
  // The invariant mass of the particle pair
  //
  
  Double_t e1 = t1->GetE(mPart1);
  Double_t e2 = t2->GetE(mPart2);

  return TMath::Sqrt((e1+e2) * (e1+e2) - P(t1,t2) * P(t1,t2));

}

//______________________________________________________________________________
Double_t T49Tool::ArmAlpha(T49ParticleRoot* t1,T49ParticleRoot* t2, 
			   Float_t mPart1,Float_t mPart2)
{      
  //
  // Armenteros alpha of the particle pair
  //

  Double_t pl1 = (t1->GetPx()*Px(t1,t2)
                + t1->GetPy()*Py(t1,t2)
                + t1->GetPz()*Pz(t1,t2)) / P(t1,t2);
  Double_t pl2 = (t2->GetPx()*Px(t1,t2)
                + t2->GetPy()*Py(t1,t2)
                + t2->GetPz()*Pz(t1,t2)) / P(t1,t2);
  
  return (pl1 - pl2) / (pl1 + pl2);      

}

//______________________________________________________________________________
Double_t T49Tool::ArmPt(T49ParticleRoot* t1,T49ParticleRoot* t2, 
			Float_t mPart1,Float_t mPart2)
{
  //
  // Armeteros Pt of the particle pair
  //
      
  Double_t pl1 = (t1->GetPx()*Px(t1,t2)
                + t1->GetPy()*Py(t1,t2)
                + t1->GetPz()*Pz(t1,t2)) / P(t1,t2);

  return (t1->GetP()*t1->GetP() - pl1*pl1); 

}

//______________________________________________________________________________
Double_t T49Tool::PsiLCMS(T49ParticleRoot *t1,T49ParticleRoot *t2, 
			  Float_t fPart1Mass, Float_t fPart2Mass)
{
  //
  // The angle between Q in pair rest frame and P in LCMS;
  //

  // Momentum sum
  Double_t plx = t1->GetPx()          + t2->GetPx();
  Double_t ply = t1->GetPy()          + t2->GetPy();
  Double_t plz = t1->GetPz()          + t2->GetPz();

  Double_t plE = t1->GetE(fPart1Mass) + t2->GetE(fPart2Mass);
  Double_t plA = T49Tool::P(t1,t2)    * T49Tool::P(t1,t2);
  
  // Momentum difference
  Double_t pkx = t1->GetPx()          - t2->GetPx();
  Double_t pky = t1->GetPy()          - t2->GetPy();
  Double_t pkz = t1->GetPz()          - t2->GetPz();
  Double_t pkE = t1->GetE(fPart1Mass) - t2->GetE(fPart2Mass);
   
  Double_t ppKL = pkx*plx + pky*ply + pkz*plz;
  Double_t aM0  = TMath::Sqrt(plE*plE - plA);
  Double_t ePM  = plE + aM0;
  Double_t h    = (ppKL / ePM - pkE) / aM0;

  // Transfer Q into restframe of pair
  Double_t kx = pkx + plx*h;
  Double_t ky = pky + ply*h;

  Double_t psi = kx*plx + ky*ply;

  return psi;

}

//______________________________________________________________________________
Double_t T49Tool::QT2(T49ParticleRoot* t1,T49ParticleRoot* t2)
{

  return  Qx(t1,t2)*Qx(t1,t2)+Qy(t1,t2)*Qy(t1,t2);

}

//______________________________________________________________________________
Double_t T49Tool::QL2(T49ParticleRoot* t1,T49ParticleRoot* t2,
                      Float_t mPart1,Float_t mPart2)
{

  return  Qz(t1,t2)*Qz(t1,t2) - (t1->GetE(mPart1)-t2->GetE(mPart2))
                               *(t1->GetE(mPart1)-t2->GetE(mPart2));

}

//______________________________________________________________________________
TLorentzVector T49Tool::pLorentz(T49ParticleRoot* t,  Float_t mPart, 
                                 TLorentzVector* pTrans)
{
  //
  // Gives the Lorentz transformed 4-vector of particle momentum 
  // in the rest frame of the 4-Vector pTrans 
  //

  TLorentzVector pInit(t->GetPx(),t->GetPy(),t->GetPz(),t->GetE(mPart)) ;

  return pLorentz(&pInit,pTrans);

} 

//______________________________________________________________________________
TLorentzVector T49Tool::pLorentz(TLorentzVector* pInit, TLorentzVector* pTrans)
{
  //
  // Gives the Lorentz transformed 4-vector of pInit 
  // in the rest frame of the 4-Vector pTrans 
  //

  Double_t AM0 = pTrans->Mag();
  Double_t EPM = pTrans->T() + AM0;
  Double_t PP0 = pInit->X() * pTrans->X() +
                 pInit->Y() * pTrans->Y() +
                 pInit->Z() * pTrans->Z() ;
  Double_t H   = (PP0/EPM-pTrans->T())/AM0;
  Double_t xCoord = pInit->X() + H * pTrans->X();
  Double_t yCoord = pInit->Y() + H * pTrans->Y();
  Double_t zCoord = pInit->Z() + H * pTrans->Z();
  Double_t ECoord = (pInit->X() * pTrans->X()-PP0)/AM0;
  
  TLorentzVector pFini(xCoord,yCoord,zCoord,ECoord);
  return pFini;

} 

//______________________________________________________________________________
TLorentzVector T49Tool::zLorentz(T49ParticleRoot* t, Float_t mPart, Double_t Beta)
{
  //
  // Gives the Lorentz transformed 4-vector of the particle momentum 
  // boostet with beta along z Axis  
  //

  TLorentzVector pInit(t->GetPx(),t->GetPy(),t->GetPz(),t->GetE(mPart));

  return zLorentz(&pInit,Beta);

} 

//______________________________________________________________________________
TLorentzVector T49Tool::zLorentz(TLorentzVector *pInit, Double_t Beta)
{
  //
  // Gives the Lorentz transformed 4-vector of the particle momentum 
  // boostet with beta along z Axis  
  //

  Double_t Gamma  = 1.0/sqrt(1.0-Beta*Beta);
  Double_t xCoord = pInit->X();
  Double_t yCoord = pInit->Y();
  Double_t zCoord = Gamma*(pInit->Z() -  Beta*pInit->T());
  Double_t ECoord = Gamma*(pInit->T() -  Beta*pInit->Z());

  TLorentzVector pFini(xCoord,yCoord,zCoord,ECoord);

  return pFini;

} 

//______________________________________________________________________________
Float_t  T49Tool::TwoTrackDist(T49ParticleRoot *t1,T49ParticleRoot *t2)
{
  //
  // A simple two track distance algorithm using a linear extrapolation
  //

  Float_t zPlane1, zPlane2;
  Float_t dPlane1, dPlane2;
  Float_t x1Plane, x2Plane, y1Plane, y2Plane;

  if (t1->GetZFirst()> t2->GetZFirst()) {
    zPlane1 =t1->GetZFirst();
  }
  else {
    zPlane1 =t2->GetZFirst();
  };    
  
  if (t1->GetZLast()< t2->GetZLast()) {
    zPlane2 =t1->GetZLast();
  }
  else {
    zPlane2 =t2->GetZLast();
  };    
  
  x1Plane = t1->GetX0()+t1->GetXPrime()*zPlane1;
  y1Plane = t1->GetY0()+t1->GetYPrime()*zPlane1;
  x2Plane = t2->GetX0()+t2->GetXPrime()*zPlane1;
  y2Plane = t2->GetY0()+t2->GetYPrime()*zPlane1;
  dPlane1 = TMath::Sqrt((x1Plane-x2Plane)*(x1Plane-x2Plane)
                      + (y1Plane-y2Plane)*(y1Plane-y2Plane));

  x1Plane = t1->GetX0()+t1->GetXPrime()*zPlane2;
  y1Plane = t1->GetY0()+t1->GetYPrime()*zPlane2;
  x2Plane = t2->GetX0()+t2->GetXPrime()*zPlane2;
  y2Plane = t2->GetY0()+t2->GetYPrime()*zPlane2;
  dPlane2 = TMath::Sqrt((x1Plane-x2Plane)*(x1Plane-x2Plane)
                      + (y1Plane-y2Plane)*(y1Plane-y2Plane));

  return (dPlane1+dPlane2) / 2.0;

}  




