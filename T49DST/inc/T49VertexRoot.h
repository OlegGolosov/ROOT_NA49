#ifndef _T49VertexRoot_INCLUDED_
#define _T49VertexRoot_INCLUDED_

/*
$Log: T49VertexRoot.h,v $
Revision 1.13  2006/08/03 20:05:39  cblume
Remove unneccessary declarations

Revision 1.12  2002/03/27 14:00:58  cblume
Add GetSameSide() and GetSameSideXi()

Revision 1.11  2001/12/12 17:35:51  cblume
Update default constructors

Revision 1.10  2001/11/12 10:49:42  cblume
Update for ROOT v3.01

 * Revision 1.9  2001/06/13  08:15:04  cblume
 * Modification to include also Xi vertices
 *
 * Revision 1.8  2000/10/15  01:08:10  cblume
 * Fix bug in GetFullTrack()
 *
 * Revision 1.7  2000/08/28  08:33:40  cblume
 * Changes in the access of rtrack info. Moved some inline functions to implementation file
 *
 * Revision 1.6  2000/05/19  15:41:09  cblume
 * Added GetY1minY2() and GetCrossYZ()
 *
 * Revision 1.5  2000/04/19  15:22:08  cblume
 * Included GetFullPositiveTrack() and GetFullNegativeTrack()
 *
 * Revision 1.4  2000/04/18  17:25:50  cblume
 * Part of the member functions are now inline. Pointer reconstruction via gT49. Removed fZTarg
 *
 * Revision 1.3  2000/04/13  14:53:45  cblume
 * Remove files
 *
 * Revision 1.2  1999/11/23  12:51:34  cblume
 * Remove files
 *
*/

#include "TObjArray.h"
#include "TClonesArray.h"
#include "TArrayF.h"
#include "TMath.h"

#include "T49Run.h"
#include "T49Vertex.h"
#include "T49Particle.h"
#include "T49EventRoot.h"

const Float_t kPionMass      = 0.13956755;
const Float_t kProtonMass    = 0.93827231;
const Float_t kLambdaMass    = 1.11568;
const Float_t kK0sMass       = 0.497672;
const Float_t kKaonMass      = 0.493677;
const Float_t kXiMass        = 1.32131;
const Float_t kOmegaMass     = 1.67245;

const Float_t kPionMassSq    = kPionMass*kPionMass;
const Float_t kProtonMassSq  = kProtonMass*kProtonMass;
const Float_t kLambdaMassSq  = kLambdaMass*kLambdaMass;
const Float_t kK0sMassSq     = kK0sMass*kK0sMass;
const Float_t kKaonMassSq    = kKaonMass*kKaonMass;
const Float_t kXiMassSq      = kXiMass*kXiMass;
const Float_t kOmegaMassSq   = kOmegaMass*kOmegaMass;

const Float_t kCLight        = 2.99792458;              // Velocity of light (cm/s*10^10)

class T49VertexRoot : public T49Vertex {

public:

  T49VertexRoot(Bool_t allocObjects = kFALSE);
  T49VertexRoot(T49VertexRoot& Vertex);

  virtual ~T49VertexRoot();
  
  virtual void             SetIflag(Int_t flag)              { fIflag = flag;            };
  virtual void             SetIdDet(UChar_t detectorId)      { fIdDet = detectorId;      };
  virtual void             SetIdVtx(UChar_t vertexId)        { fIdVtx = vertexId;        };
  virtual void             SetNTrkFit(Int_t n)               { fNTrkFit = n;             };
  virtual void             SetNTrkOut(Int_t n)               { fNTrkOut = n;             };
  virtual void             SetX(Float_t x)                   { fX = x;                   };
  virtual void             SetY(Float_t y)                   { fY = y;                   };
  virtual void             SetZ(Float_t z)                   { fZ = z;                   };
  virtual void             SetSigX(Float_t x)                { fSigX = x;                };
  virtual void             SetSigY(Float_t y)                { fSigY = y;                };
  virtual void             SetSigZ(Float_t z)                { fSigZ = z;                };
  virtual void             SetPchi2(Float_t fitProb)         { fPchi2 = fitProb;         };
  virtual void             SetFirstTrackIsPositive(Bool_t b) { fFirstTrackIsPositive = b;};

  virtual void             SetPrimaryIndex(Int_t track, Int_t index);
  virtual void             SetSecondaryIndex(Int_t track, Int_t index); 
  
  virtual void             SetXTarg(Float_t x)               { fXTarg = x;         };
  virtual void             SetYTarg(Float_t y)               { fYTarg = y;         };
  virtual void             SetX1minX2(Float_t x1minx2)       { fX1minX2 = x1minx2; };
  virtual Bool_t           SetDaughters(T49ParticleRoot *Pos, T49ParticleRoot *Neg);

  virtual void             SetPathlengthXi(Float_t plen)     { fPlen    = plen;    };
  virtual void             SetPxXi(Float_t px)               { fSigX    = px;      };
  virtual void             SetPyXi(Float_t py)               { fSigY    = py;      };
  virtual void             SetPzXi(Float_t pz)               { fSigZ    = pz;      };

          Int_t            GetIflag()                        { return fIflag;   };
          UChar_t          GetIdDet()                        { return fIdDet;   };
          UChar_t          GetIdVtx()                        { return fIdVtx;   };
          Int_t            GetNTrkFit()                      { return fNTrkFit; };
          Int_t            GetNTrkOut()                      { return fNTrkOut; };
          Float_t          GetX()                            { return fX;       };
          Float_t          GetY()                            { return fY;       };
          Float_t          GetZ()                            { return fZ;       };
          Float_t          GetSigX()                         { return fSigX;    };
          Float_t          GetSigY()                         { return fSigY;    };
          Float_t          GetSigZ()                         { return fSigZ;    };
          Float_t          GetPchi2()                        { return fPchi2;   };
 
          Bool_t           GetFirstTrackIsPositive()         { return fFirstTrackIsPositive; };

          T49ParticleRoot *GetFirstTrack();
          T49ParticleRoot *GetSecondTrack();
          T49ParticleRoot *GetPositiveTrack()                { if (fFirstTrackIsPositive)
                                                                 return GetFirstTrack();
                                                               else 
                                                                 return GetSecondTrack(); };
          T49ParticleRoot *GetNegativeTrack()                { if (!fFirstTrackIsPositive)
                                                                 return GetFirstTrack();
                                                               else 
                                                                 return GetSecondTrack(); };
          T49ParticleRoot *GetFullPositiveTrack();
          T49ParticleRoot *GetFullNegativeTrack();

          T49ParticleRoot *GetPositiveMainTrack();
          T49ParticleRoot *GetNegativeMainTrack();

          T49ParticleRoot *GetChargedXiTrack()               { return GetFirstTrack();  };
          T49ParticleRoot *GetNeutralXiTrack()               { return GetSecondTrack(); };

          T49ParticleRoot *GetFullChargedXiTrack();
          T49ParticleRoot *GetFullNeutralXiTrack()           { return GetSecondTrack(); };

          T49ParticleRoot *GetFullTrack(T49ParticleRoot *SecTrack
                                       ,T49ParticleRoot *MainTrack);

          Float_t          GetInvariantMassLambda();
          Float_t          GetInvariantMassAntiLambda();
          Float_t          GetInvariantMassK0s();
          Float_t          GetInvariantMassXi();
          Float_t          GetInvariantMassOmega();
  inline  Float_t          GetRapidity(Float_t mass);
          Float_t          GetRapidityLambda()               { return GetRapidity(kLambdaMass); };
          Float_t          GetRapidityK0s()                  { return GetRapidity(kK0sMass);    };
  inline  Float_t          GetRapidityXi();
  inline  Float_t          GetRapidityOmega();
  inline  Float_t          GetEnergy(Float_t mass);
          Float_t          GetEnergyLambda()                 { return GetEnergy(kLambdaMass);   };
          Float_t          GetEnergyK0s()                    { return GetEnergy(kK0sMass);      };
  inline  Float_t          GetEnergyXi();
  inline  Float_t          GetEnergyOmega();
  inline  Float_t          GetPt();
  inline  Float_t          GetP();
  inline  Float_t          GetPx();
  inline  Float_t          GetPy();
  inline  Float_t          GetPz();
          Float_t          GetArmenterosPt();
          Float_t          GetArmenterosPtXi();
          Float_t          GetArmenterosAlpha();
          Float_t          GetArmenterosAlphaXi();
          Int_t            GetSameSide();
          Int_t            GetSameSideXi();                                     

          Int_t            GetPrimaryIndex(Int_t track);
          Int_t            GetSecondaryIndex(Int_t track);

          Float_t          GetXTarg()                        { return fXTarg;   };
          Float_t          GetYTarg()                        { return fYTarg;   };
          Float_t          GetX1minX2()                      { return fX1minX2; };
          Float_t          GetY1minY2();
          Float_t          GetY1minY2Xi();
          Float_t          GetCrossYZ(Float_t zmin);

          Float_t          GetCosThetaLambda(Bool_t useInvMass = kFALSE);
          Float_t          GetCosThetaAntiLambda(Bool_t useInvMass = kFALSE);
          Float_t          GetCosThetaK0s(Bool_t useInvMass = kFALSE);
          Float_t          GetCosThetaXi(Bool_t useInvMass = kFALSE);
          Float_t          GetCosThetaOmega(Bool_t useInvMass = kFALSE);
          Float_t          GetLifetimeLambda();
          Float_t          GetLifetimeLambda(Float_t x, Float_t y, Float_t z);
          Float_t          GetLifetimeK0s();
          Float_t          GetLifetimeXi();
          Float_t          GetLifetimeOmega();
  inline  Float_t          GetBetaLambda();             
  inline  Float_t          GetBetaK0s();                
  inline  Float_t          GetBetaXi();                
  inline  Float_t          GetBetaOmega();                
  inline  Float_t          GetGammaLambda();          
  inline  Float_t          GetGammaK0s();              
  inline  Float_t          GetGammaXi();              
  inline  Float_t          GetGammaOmega();              

          Float_t          GetPathlengthXi()                 { return fPlen;    };
          Float_t          GetPxXi()                         { return fSigX;    };
          Float_t          GetPyXi()                         { return fSigY;    };
          Float_t          GetPzXi()                         { return fSigZ;    };
  inline  Float_t          GetPXi();  
  inline  Float_t          GetPtXi();  

protected:

  Int_t             fIflag;                 //  Vertex flag
  UChar_t           fIdDet;                 //  Detector id (2^det-nr.)
  UChar_t           fIdVtx;                 //  Vertex type
  Int_t             fNTrkFit;               //  No of fitted tracks      
  Int_t             fNTrkOut;               //  No of tracks      
  Float_t           fX;                     //  x coordinate of vertex
  Float_t           fY;                     //  y coordinate of vertex
  Float_t           fZ;                     //  z coordinate of vertex
  Float_t           fSigX;                  //  Error of the x coordinate of the vertex
  Float_t           fSigY;                  //  Error of the y coordinate of the vertex
  Float_t           fSigZ;                  //  Error of the z coordinate of the vertex
  Float_t           fPchi2;                 //  P(chi^2,ndf) of the vertex fit

  T49ParticleRoot  *fFirstTrack;            // !First daughter track of vertex
  T49ParticleRoot  *fSecondTrack;           // !Second daughter track of vertex
  Bool_t            fFirstTrackIsPositive;  //  Flag to indicate charge of 1st track

  // Added for version 2
  Int_t             fFirstPriIndex;         //  Index to PrimaryParticleArray in event
  Int_t             fSecondPriIndex;        //  Index to PrimaryParticleArray in event

  Int_t             fFirstSecIndex;         //  Index to SecondaryParticleArray in event
  Int_t             fSecondSecIndex;        //  Index to SecondaryParticleArray in event

  Float_t           fXTarg;                 //  V0 impact x-position at the target
  Float_t           fYTarg;                 //  V0 impact y-position at the target
  Float_t           fX1minX2;               //  Daughter position x1 minus x2 at target

  // Added for version 5
  Float_t           fPlen;                  //  Xi pathlength

  // Protected member functions
          T49ParticleRoot *GetPositiveMainTrack(TClonesArray *PrimaryTrackArray);
          T49ParticleRoot *GetNegativeMainTrack(TClonesArray *PrimaryTrackArray);
          Bool_t           SetDaughters();
          Float_t          CosTheta(Float_t *pV0, Float_t *pPD);
          Int_t            Lorentz(Float_t *p4cm, Float_t *p4in, Float_t *p4out);

  ClassDef(T49VertexRoot,6)                 //  Vertex information

};

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetBetaLambda() 
{

  if (GetEnergy(kLambdaMass))
    return (GetP() / GetEnergy(kLambdaMass));
  else
    return 0; 

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetBetaK0s() 
{

  if (GetEnergy(kK0sMass))
    return (GetP() / GetEnergy(kK0sMass));
  else
    return 0; 

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetBetaXi() 
{

  if (GetEnergyXi())
    return (GetPXi() / GetEnergyXi());
  else
    return 0; 

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetBetaOmega() 
{

  if (GetEnergyOmega())
    return (GetPXi() / GetEnergyOmega());
  else
    return 0; 

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetGammaLambda() 
{

  if (GetEnergy(kLambdaMass)) 
    return (GetEnergy(kLambdaMass) / kLambdaMass);
  else
    return 0;

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetGammaK0s() 
{

  if (GetEnergy(kK0sMass)) 
    return (GetEnergy(kK0sMass)    / kK0sMass);
  else
    return 0;

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetGammaXi() 
{

  if (GetEnergyXi()) 
    return (GetEnergyXi()          / kXiMass);
  else
    return 0;

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetGammaOmega() 
{

  if (GetEnergyOmega()) 
    return (GetEnergyOmega()       / kOmegaMass);
  else
    return 0;

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetRapidity(Float_t mass) 
{
  //
  // The rapidity assuming mass <mass>.
  //

  Float_t etot = GetEnergy(mass);
  Float_t pz   = GetPz();
  
  return log((etot + pz) / (etot - pz)) * 0.5;
  
}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetRapidityXi() 
{
  //
  // The rapidity assuming the Xi hypothesis.
  //

  Float_t etot = GetEnergyXi();
  Float_t pz   = GetPzXi();
  
  return log((etot + pz) / (etot - pz)) * 0.5;
  
}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetRapidityOmega() 
{
  //
  // The rapidity assuming the Omega hypothesis.
  //

  Float_t etot = GetEnergyOmega();
  Float_t pz   = GetPzXi();
  
  return log((etot + pz) / (etot - pz)) * 0.5;
  
}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetEnergy(Float_t mass) 
{
  //
  // The energy assuming mass <mass>.
  //  
  
  Float_t p = GetP();
  return sqrt(mass*mass + p*p);

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetEnergyXi() 
{
  //
  // The energy assuming a Xi.
  //  
  
  Float_t p = GetPXi();
  return sqrt(kXiMassSq + p*p);

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetEnergyOmega() 
{
  //
  // The energy assuming an Omega.
  //  
  
  Float_t p = GetPXi();
  return sqrt(kOmegaMassSq + p*p);

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetPt() 
{
  //
  // The transverse momentum.
  //

  Double_t px = GetPx();
  Double_t py = GetPy();
  return sqrt(px*px + py*py);

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetP() 
{
  //
  // The total momentum
  //

  Double_t px = GetPx();
  Double_t py = GetPy();
  Double_t pz = GetPz();
  return sqrt(px*px + py*py + pz*pz);

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetPtXi() 
{
  //
  // The transverse momentum of a Xi candidate.
  //

  Double_t px = GetPxXi();
  Double_t py = GetPyXi();
  return sqrt(px*px + py*py);

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetPXi() 
{
  //
  // The total momentum of a Xi candidate
  //

  Double_t px = GetPxXi();
  Double_t py = GetPyXi();
  Double_t pz = GetPzXi();
  return sqrt(px*px + py*py + pz*pz);

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetPx() 
{
  //
  // The momentum x-component.
  //

  return (GetPositiveTrack()->GetPx() + GetNegativeTrack()->GetPx());

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetPy() 
{
  //
  // The momentum y-component.
  //

  return (GetPositiveTrack()->GetPy() + GetNegativeTrack()->GetPy());

}

//______________________________________________________________________________
inline Float_t T49VertexRoot::GetPz() 
{
  //
  // The momentum z-component.
  //

  return (GetPositiveTrack()->GetPz() + GetNegativeTrack()->GetPz());

}

#endif








