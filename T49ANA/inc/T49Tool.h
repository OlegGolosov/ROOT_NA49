#ifndef __T49Tool__
#define __T49Tool__

/*
$Log: T49Tool.h,v $
Revision 1.4  2001/11/21 17:00:31  cblume
Update

 * Revision 1.3  1999/11/23  13:52:19  cblume
 * Remove files
 *
*/

#include "T49ParticleRoot.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "PhysConst.h"

//////////////////////////////////////////////  
class T49ParticleRoot;

class T49Tool : public TObject
{
 public:

  /*             Two Particle calculations       */
  static Double_t Qinv(T49ParticleRoot* t1,T49ParticleRoot* t2, 
		       Float_t mPart1,Float_t mPart2);
  static Double_t Minv(T49ParticleRoot* t1,T49ParticleRoot* t2, 
		       Float_t mPart1,Float_t mPart2);
  static Double_t ArmAlpha(T49ParticleRoot* t1,T49ParticleRoot* t2, 
			   Float_t mPart1,Float_t mPart2);
  static Double_t ArmPt(T49ParticleRoot* t1,T49ParticleRoot* t2, 
			Float_t mPart1,Float_t mPart2);
  static Double_t PsiLCMS(T49ParticleRoot* t1,T49ParticleRoot* t2, 
			Float_t mPart1,Float_t mPart2);
 
  static Double_t Px(T49ParticleRoot* t1,T49ParticleRoot* t2) { return (t1->GetPx()+t2->GetPx()); };
  static Double_t Py(T49ParticleRoot* t1,T49ParticleRoot* t2) { return (t1->GetPy()+t2->GetPy()); };
  static Double_t Pz(T49ParticleRoot* t1,T49ParticleRoot* t2) { return (t1->GetPz()+t2->GetPz()); };
  static Double_t  P(T49ParticleRoot* t1,T49ParticleRoot* t2)
                   { return  TMath::Sqrt(Px(t1,t2)*Px(t1,t2)+Py(t1,t2)*Py(t1,t2)+Pz(t1,t2)*Pz(t1,t2)); };

  static Double_t Qx(T49ParticleRoot* t1,T49ParticleRoot* t2) { return (t1->GetPx()-t2->GetPx()); };
  static Double_t Qy(T49ParticleRoot* t1,T49ParticleRoot* t2) { return (t1->GetPy()-t2->GetPy()); };
  static Double_t Qz(T49ParticleRoot* t1,T49ParticleRoot* t2) { return (t1->GetPz()-t2->GetPz()); };
  static Double_t  Q(T49ParticleRoot* t1,T49ParticleRoot* t2)
                   { return  TMath::Sqrt(Qx(t1,t2)*Qx(t1,t2)+Qy(t1,t2)*Qy(t1,t2)+Qz(t1,t2)*Qz(t1,t2)); };

  static Double_t QT2(T49ParticleRoot* t1,T49ParticleRoot* t2); 
  static Double_t QL2(T49ParticleRoot* t1,T49ParticleRoot* t2,
		      Float_t mPart1, Float_t mPart2);

  static Double_t XF(T49ParticleRoot* t1,T49ParticleRoot* t2,
		     Float_t mPart1, Float_t mPart2,
		     Float_t SQRTs);

  static Float_t  TwoTrackDist(T49ParticleRoot *t1,T49ParticleRoot *t2);


  /*             General Stuff               */
  static TLorentzVector pLorentz(T49ParticleRoot* t, Float_t mPart, TLorentzVector* pTrans); 
  static TLorentzVector pLorentz(TLorentzVector* t,  TLorentzVector* pTrans); 
  static TLorentzVector zLorentz(T49ParticleRoot* t, Float_t mPart, Double_t Beta);
  static TLorentzVector zLorentz(TLorentzVector* t,                 Double_t Beta); 

  ClassDef(T49Tool,1) // Tools to calculate kinematic variables

};
#endif

