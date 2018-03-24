#ifndef __T49Proj2Def__
#define __T49Proj2Def__

/*
$Log: T49Proj2.h,v $
Revision 1.12  2008/01/04 10:48:58  cblume
64bit modifications by Marek Szuba

Revision 1.11  2005/05/20 11:28:21  cblume
Get rid of deprecated warnings

Revision 1.10  2001/12/12 17:41:40  cblume
Caluclate k in the pair cm

Revision 1.9  2001/11/27 16:15:18  cblume
Change dimensions of the character arrays

Revision 1.8  2001/11/21 16:59:59  cblume
Add T49Proj2K

Revision 1.7  2001/11/12 12:07:25  cblume
Update for ROOT v3.01

 * Revision 1.6  2000/07/07  14:15:28  cblume
 * Made ROOT v2.25 compatible
 *
 * Revision 1.5  1999/11/24  16:03:40  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 *
 * Revision 1.4  1999/11/23  13:52:19  cblume
 * Remove files
 *
*/

#include <iostream>
#include <fstream> 
using namespace std;

#include "TROOT.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TAxis.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TCollection.h"
#include "TKey.h"
#include "TClass.h"

#include "T49ParticleRoot.h"
#include "T49Cut2Track.h"
#include "PhysConst.h"

class T49ParticleRoot;
class T49Cut2Track;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  T49Proj2.h                                                                //
//  Version 2.0       R.Ganz  Jan.20  1999                                    //
//  Update            C.Blume Nov.16  2001                                    //
//                                                                            //
//  Implementation of 2 part projection classes:                              //
//  1D  2D  3D  parameriztions are supported                                  //
//  If you want to add a new type of para-                                    //
//  metrization just add a new class                                          //
//  implemetation which inherits from the base class                          //
//  (examples below)                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class T49Proj2BASE : public TObject
{

 public:

  T49Proj2BASE();
  virtual ~T49Proj2BASE();

  void     Book(char* nam, char* tit,
	        Int_t nx,Float_t xmin,Float_t xmax, 
  	        Int_t ny=1,Float_t ymin=0.0,Float_t ymax=0.0,
	        Int_t nz=1,Float_t zmin=0.0,Float_t zmax=0.0); 
  void     BookOne(char* nam, char* tit,
	           Int_t nx,Float_t xmin,Float_t xmax, 
	           Int_t ny=1,Float_t ymin=0.0,Float_t ymax=0.0,
	           Int_t nz=1,Float_t zmin=0.0,Float_t zmax=0.0); 
  void     Fill(TObjArray* l1, TObjArray* l2, T49Cut2Track* T2Cut = NULL);  
  void     Fill(T49ParticleRoot* t1, T49ParticleRoot* t2); 
  Int_t    GetArrayIndex(T49ParticleRoot* t1, T49ParticleRoot* t2, Int_t MirrorThis);

  void     SetArrayKt(Int_t n, Float_t* mima)              { fKtAxis  = new TAxis(n,(Axis_t *) mima); }
  void     SetArrayKt(TAxis a)                             { fKtAxis  = new TAxis(a);                 }
  void     SetArrayKt(Int_t n, Float_t min, Float_t max)   { fKtAxis  = new TAxis(n,min,max);         }

  void     SetArrayYp(Int_t n, Float_t* mima)              { fYpAxis  = new TAxis(n,(Axis_t *) mima); }
  void     SetArrayYp(TAxis a)                             { fYpAxis  = new TAxis(a);                 }
  void     SetArrayYp(Int_t n, Float_t min, Float_t max)   { fYpAxis  = new TAxis(n,min,max);         }

  void     SetArrayY(Int_t n, Float_t* mima)               { fYAxis   = new TAxis(n,(Axis_t *) mima); }
  void     SetArrayY(TAxis a)                              { fYAxis   = new TAxis(a);                 }
  void     SetArrayY(Int_t n, Float_t min, Float_t max)    { fYAxis   = new TAxis(n,min,max);         }

  void     SetArrayPt(Int_t n, Float_t* mima)              { fPtAxis  = new TAxis(n,(Axis_t *) mima); }
  void     SetArrayPt(TAxis a)                             { fPtAxis  = new TAxis(a);                 }
  void     SetArrayPt(Int_t n, Float_t min, Float_t max)   { fPtAxis  = new TAxis(n,min,max);         }

  void     SetArrayPhi(Int_t n, Float_t* mima)             { fPhiAxis = new TAxis(n,(Axis_t *) mima); }
  void     SetArrayPhi(TAxis a)                            { fPhiAxis = new TAxis(a);                 }
  void     SetArrayPhi(Int_t n, Float_t min, Float_t max)  { fPhiAxis = new TAxis(n,min,max);         }

  void     SetArrayXF(Int_t n, Float_t* mima)              { fXFAxis  = new TAxis(n,(Axis_t *) mima); }
  void     SetArrayXF(TAxis a)                             { fXFAxis  = new TAxis(a);                 }
  void     SetArrayXF(Int_t n, Float_t min, Float_t max)   { fXFAxis  = new TAxis(n,min,max);         }
  
  void     WriteHist();

  void     SetCMS(Double_t pLab);
  void     SetQinvFlag(Int_t m)                            { fQinvFlag   = m; }  
  void     SetMirrorFlag(Int_t m)                          { fMirrorFlag = 1; }
  void     SetParticleMass(Int_t a, Int_t m);                
  void     SetParticleMass(Int_t a, Float_t mass);                
  void     System(Double_t b)                              { fBeta       = b;   fSysID = 1; }
  void     System()                                        { fBeta       = 0.0; fSysID = 2; }
  void     SetSystemLCMS()                                 { fBeta       = 0.0; fSysID = 2; }
  void     SetSystemPairCM()                               { fBeta       = 0.0; fSysID = 3; }
  void     SetVerbose(Int_t m)                             { fVerbose    = m; } 

  Int_t    GetDim()                                        { return fDim;     }    
  Int_t    GetNumPairs()                                   { return fNPairs;  }        
  Int_t    CheckMirror(T49ParticleRoot* t1,T49ParticleRoot* t2);
  Double_t GetPartMass(Int_t i)                            { return (i==1 ? fPart1Mass : fPart2Mass); } 

  Double_t GetQx()                                         { return fQx;      }
  Double_t GetQy()                                         { return fQy;      }
  Double_t GetQz()                                         { return fQz;      }
  Double_t GetBeta()                                       { return fBeta;    }

 protected: 
   
  virtual void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2,Int_t MirrorFlag = 0) = 0;

  Float_t    fPart1Mass;      // Mass of first particle
  Float_t    fPart2Mass;      // Mass of second particle
  Int_t      fVerbose;        // Verbose mode 0 1 2 
  Int_t      fSysID;          // ID of Lorentz frame 1 = CMS (or any other fixed frame), 2 = LCMS, 3 = pair CM
  Double_t   fBeta;           // Beta (set for LCMS for ea. pair)
  Int_t      fQinvFlag;       // Flag for booking Qinv spectra (eg for Coulomb corr in HBT study)
  Int_t      fMirrorFlag;     // Flag to mirror from backward to forward rapidity (makes sense only for symmetric systems)
  Double_t   fYcms;           // Mid Rapidity for mirroring  
  Double_t   fBetaCMS;
  Double_t   fPprojCMS; 
  Int_t      fDim;            // Dimension for projection
  Int_t      fNPairs;         // Number of pairs in projection 
  Int_t      fDimArray;       // Number of Array entries  
  Double_t   fQx;             // Quantities to be projected (calc. in derived classes for ea. pair)
  Double_t   fQy;
  Double_t   fQz; 
  Int_t      fNXBin;          // X Binning for histogram 
  Float_t    fXMin;   
  Float_t    fXMax;  
  char       fXLabel[40];
  Int_t      fNYBin;          // Y Binning for histogram 
  Float_t    fYMin;   
  Float_t    fYMax;   
  char       fYLabel[40];
  Int_t      fNZBin;          // Z Binning for histogram 
  Float_t    fZMin;   
  Float_t    fZMax;   
  char       fZLabel[40];  
  char       fHistTitle[120]; // Global histogram title							      

  TObjArray *fHist;           // Pointers to histogram arrays 
  TObjArray *fXHist;
  TObjArray *fYHist;
  TObjArray *fZHist;
  TObjArray *fAHist; 

  TAxis     *fKtAxis;         // Axis for kt=1/2(pt(p1)+pt(p2)) 
  TAxis     *fYpAxis;         // Axis for y=1/2(y(p1)+y(p2)) 
  TAxis     *fYAxis;          // Axis for y(P=p1+p2) 
  TAxis     *fPtAxis;         // Axis for Pt(P)   
  TAxis     *fPhiAxis;        // Axis for Phi(P)  
  TAxis     *fXFAxis;         // Axis for XF(P) Feynman x for total momentum  

  TH1D      *fBinStat;        // Statistic in ea. array entry

  TProfile  *fAverKtHist;     // Profile for average Kt  value in all bins
  TProfile  *fAverYpHist;     // Profile for average Yp  value in all bins
  TProfile  *fAverYHist;      // Profile for average Y   value in all bins
  TProfile  *fAverPtHist;     // Profile for average Pt  value in all bins
  TProfile  *fAverPhiHist;    // Profile for average Phi value in all bins
  TProfile  *fAverXFHist;     // Profile for average XF  value in all bins
							       
  ClassDef(T49Proj2BASE,4)    // Two-particle projection base class

};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  BP parametrization (3D)                                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class T49Proj2BP : public T49Proj2BASE
{

 public:

  T49Proj2BP()
  {
    fDim = 3;
    sprintf(fHistTitle,"BP");
    sprintf(fXLabel,"Q_{side}");
    sprintf(fYLabel,"Q_{out}");
    sprintf(fZLabel,"Q_{long}");
  }
  virtual ~T49Proj2BP() {}

 protected:

  void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2,Int_t MirrorThis = 0)
  { 

    TLorentzVector P1 = T49Tool::zLorentz(t1,fPart1Mass,fBeta);
    TLorentzVector P2 = T49Tool::zLorentz(t2,fPart2Mass,fBeta);

     Double_t ktPAIR = TMath::Sqrt((P1.X()+P2.X())*(P1.X()+P2.X())
			          +(P1.Y()+P2.Y())*(P1.Y()+P2.Y()));
     fQx = ((P1.X()-P2.X())*(P1.Y()+P2.Y())
	   -(P1.Y()-P2.Y())*(P1.X()+P2.X())) / ktPAIR; // Qside              
     fQy = (P1.X()*P1.X()-P2.X()*P2.X() 
	   +P1.Y()*P1.Y()-P2.Y()*P2.Y())     / ktPAIR; // Qout
     fQz =  P1.Z() - P2.Z();                           // Qlong
     if (MirrorThis ==1 ) { 
       fQz = -fQz; 
       fQx = -fQx;
     }	
     if (fQx < 0) {
       fQx = -fQx; 
       fQy = -fQy; 
       fQz = -fQz;
     };

  } 

  ClassDef(T49Proj2BP,1) // HBT-analysis class with Bertsch-Pratt parametrization

};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  YKP parametrization (3D)                                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class T49Proj2YKP : public T49Proj2BASE
{

 public:

  T49Proj2YKP()
  {
    fDim = 3;
    sprintf(fHistTitle,"YKP");
    sprintf(fXLabel,"Q_{#perp}");
    sprintf(fYLabel,"Q_{0}");
    sprintf(fZLabel,"Q_{#parallel}");     
  }
  virtual ~T49Proj2YKP() {}
    
 protected:

  void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2,Int_t MirrorThis=0)
  {
 
    TLorentzVector P1 = T49Tool::zLorentz(t1,fPart1Mass,fBeta);
    TLorentzVector P2 = T49Tool::zLorentz(t2,fPart2Mass,fBeta);

    fQx = TMath::Sqrt((P1.X()-P2.X())*(P1.X()-P2.X())
                     +(P1.Y()-P2.Y())*(P1.Y()-P2.Y())); // Qperp 
    fQy = P1.T()-P2.T();                                // Qnought 
    fQz = P1.Z()-P2.Z();                                // Qpara

    if (MirrorThis == 1) {
      fQz= -fQz;
    };	
    if (fQy < 0) {
      fQy = -fQy; 
      fQz = -fQz;
    };
  }

  ClassDef(T49Proj2YKP,1) // HBT-analysis class with Yano-Koonin-Podgoretski parametrization

};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  QtQl parametrization (2D)                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class T49Proj2QtQl : public T49Proj2BASE
{

 public:

  T49Proj2QtQl()
  {
    fDim = 2;
    sprintf(fHistTitle,"Q_{t}^{2}Q_{l}^{2}");
    sprintf(fXLabel,"Q_{t}^{2}");
    sprintf(fYLabel,"Q_{l}^{2}");
  }  
  virtual ~T49Proj2QtQl() {}
    
 protected:

  void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2,Int_t MirrorThis = 0)
  { 

    fQx = T49Tool::QT2(t1,t2);                          // Qt^2 
    fQy = T49Tool::QL2(t1,t2,fPart1Mass,fPart2Mass);    // Ql^2

  }

  ClassDef(T49Proj2QtQl,1) // HBT-analysis class with Qt-Ql parametrization

};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Y-Distance parametrization (2D)                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class T49Proj2DistY : public T49Proj2BASE
{

 public:

  T49Proj2DistY()
  {
    fDim = 2;
    sprintf(fHistTitle,"Dist vs Y");
    sprintf(fXLabel,"Dist cm");
    sprintf(fYLabel,"Y");
  }
  virtual ~T49Proj2DistY() {}
    
 protected:

  void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2,Int_t MirrorThis = 0)
  { 

    fQx = T49Tool::TwoTrackDist(t1,t2);                          //  Two track distance 
    fQy = 0.5*(t1->GetRap(fPart1Mass)+t2->GetRap(fPart2Mass));

  }    

  ClassDef(T49Proj2DistY,1) // Two-particle projection class, y-distance

};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Armenteros plot (2D)                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class T49Proj2Armenteros : public T49Proj2BASE
{

 public:

  T49Proj2Armenteros()
  {
    fDim = 2;
    sprintf(fHistTitle,"Armenteros");
    sprintf(fXLabel,"#alpha^{Arm}");
    sprintf(fYLabel,"p_{t}^{Arm}");
  }  
  virtual ~T49Proj2Armenteros() {}
    
 protected:

  void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2,Int_t MirrorThis = 0)
  { 

    fQx = T49Tool::ArmAlpha(t1,t2,fPart1Mass,fPart2Mass);
    fQy = T49Tool::ArmPt(t1,t2,   fPart1Mass,fPart2Mass);

  }    

  ClassDef(T49Proj2Armenteros,1) // Two-particle projection class, Armenteros variables

};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Qinv parametrization (1D)                                                 //
//  Generalized to non identical particles                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class T49Proj2QINV : public T49Proj2BASE
{

 public:

  T49Proj2QINV()
  {
    fDim = 1; 
    sprintf(fHistTitle,"Q_{inv}");
    sprintf(fXLabel,"Q_{inv}");
  }
  virtual ~T49Proj2QINV() {}
    
 protected:

  void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2,Int_t MirrorThis = 0) 
  {
 
    fQx  = T49Tool::Qinv(t1,t2,fPart1Mass,fPart2Mass);

  }    

  ClassDef(T49Proj2QINV,1) // Two-particle projection class for Qinv analysis

};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Qinv^2 parametrization (1D)                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class T49Proj2QINV2 : public T49Proj2BASE
{

 public:

  T49Proj2QINV2()
  {
    fDim = 1;  
    sprintf(fHistTitle,"Q_{inv}^{2}");
    sprintf(fXLabel,"Q_{inv}^{2}");
  }
  virtual ~T49Proj2QINV2() {}
  
 protected:

  void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2,Int_t MirrorThis = 0) 
  { 

    fQx = T49Tool::Qinv(t1,t2,fPart1Mass,fPart2Mass);
    fQx = fQx*fQx;

  }
    

  ClassDef(T49Proj2QINV2,1) // Two-particle projection class for Qinv2 analysis

};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Invariant mass parametrization (1D)                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class T49Proj2INVmass : public T49Proj2BASE
{

 public:

  T49Proj2INVmass()
  {
    fDim = 1;
    sprintf(fHistTitle,"m_{inv}");
    sprintf(fXLabel,"m_{inv}");
  }
  virtual ~T49Proj2INVmass() {}
  
 protected:

  void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2,Int_t MirrorThis = 0) 
  { 

    fQx = T49Tool::Minv(t1,t2,fPart1Mass,fPart2Mass);

  }    

  ClassDef(T49Proj2INVmass,1) // Two-particle projection class for invariant mass analysis
  
};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  k = 0.5*|p1-p2| in pair c.m. (1D)                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class T49Proj2K : public T49Proj2BASE
{

 public:

  T49Proj2K()
  {
    fDim = 1;
    sprintf(fHistTitle,"k");
    sprintf(fXLabel,"k");
  }
  virtual ~T49Proj2K() {}
  
 protected:

  void QCalc(T49ParticleRoot* t1,T49ParticleRoot* t2,Int_t MirrorThis = 0) 
  { 

    Double_t epair = 0;
    Double_t betax = 0;
    Double_t betay = 0;
    Double_t betaz = 0;

    TLorentzVector P1;
    TLorentzVector P2;

    switch (fSysID) {

    case 2:

      // LCMS
      P1 = T49Tool::zLorentz(t1,fPart1Mass,fBeta);
      P2 = T49Tool::zLorentz(t2,fPart2Mass,fBeta);
      break;

    case 3:

      // Pair c.m.
      epair = t1->GetE(fPart1Mass) + t2->GetE(fPart2Mass);
      betax = -1.0 * (t1->GetPx() + t2->GetPx()) / epair;
      betay = -1.0 * (t1->GetPy() + t2->GetPy()) / epair;
      betaz = -1.0 * (t1->GetPz() + t2->GetPz()) / epair;
      P1.SetPxPyPzE(t1->GetPx(),t1->GetPy(),t1->GetPz(),t1->GetE(fPart1Mass));
      P2.SetPxPyPzE(t2->GetPx(),t2->GetPy(),t2->GetPz(),t2->GetE(fPart2Mass));
      P1.Boost(betax,betay,betaz);      
      P2.Boost(betax,betay,betaz);      
      break;
    
    default:

      // CMS
      P1.SetPxPyPzE(t1->GetPx(),t1->GetPy(),t1->GetPz(),t1->GetE(fPart1Mass));
      P2.SetPxPyPzE(t2->GetPx(),t2->GetPy(),t2->GetPz(),t2->GetE(fPart2Mass));
      break;

    };

    fQx = 0.5 * TMath::Sqrt((P1.X()-P2.X())*(P1.X()-P2.X())
                          + (P1.Y()-P2.Y())*(P1.Y()-P2.Y())
                          + (P1.Z()-P2.Z())*(P1.Z()-P2.Z()));

  }    

  ClassDef(T49Proj2K,1) // Two-particle projection class for k = 0.5*(p1-p2) in pair c.m.
  
};

#endif







