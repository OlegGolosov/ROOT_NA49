#ifndef _T49EventRoot_included_
#define _T49EventRoot_included_

/*
$Log: T49EventRoot.h,v $
Revision 1.20  2007/11/05 18:30:37  cblume
Update by Banjamin

Revision 1.16  2002/10/07 09:58:00  cblume
Update for CASTOR

Revision 1.15  2002/05/06 07:56:43  cblume
Add function GetBPDVertex()

Revision 1.14  2002/03/27 14:17:37  cblume
Move GetEveto() into the implementation file

Revision 1.13  2002/03/27 14:15:36  cblume

Revision 1.12  2002/02/04 16:03:48  cblume
Add T49Veto

Revision 1.11  2001/12/12 17:34:29  cblume
Update default constructors

Revision 1.10  2001/11/12 10:47:22  cblume
Update for ROOT v3.01

 * Revision 1.9  2000/10/25  09:42:17  cblume
 * Include WFA information
 *
 * Revision 1.8  2000/10/09  10:25:04  cblume
 * General update
 *
 * Revision 1.7  2000/04/18  17:23:09  cblume
 * Introduced GetPrimaryVertex() and GetFittedVertex()
 *
 * Revision 1.6  2000/04/13  14:53:43  cblume
 * Remove files
 *
 * Revision 1.5  1999/12/01  09:39:28  cblume
 * Changed to root 2.23-09. Updated the maT49VertexRoot.hkefiles. Changes in T49CutTrack, T49Dedx, T49Proj1, T49Proj2, TRootDS
 *
 * Revision 1.4  1999/11/25  13:57:52  cblume
 * Introduced gT49KeepEvent in T49Event(MC)Root to make events cloneable
 *
 * Revision 1.3  1999/11/23  12:51:13  cblume
 * Remove files
 *
*/

// ROOT classes
#include "TNamed.h"
#include "TClonesArray.h"

// NA49 classes
#include "T49Event.h"
#include "T49ParticleRoot.h"
#include "T49Vertex.h"
#include "T49BeamRoot.h"
#include "T49RingRoot.h"
#include "T49MonitorTPCRoot.h"
#include "T49VetoRoot.h"

class T49EventRoot : public T49Event {

 public:

    T49EventRoot();
    T49EventRoot(Int_t AllocObjects);
    virtual ~T49EventRoot();

    inline  T49Vertex        *GetPrimaryVertex();
    inline  T49Vertex        *GetFittedVertex();
    inline  T49Vertex        *GetBPDVertex();
    virtual Int_t             GetNParticles(Int_t kSelection = 0);
    virtual void              ClearParticleLists();                 
    virtual Bool_t            ReCreateArrays();

    virtual TClonesArray     *GetPrimaryParticles()                { return fPrimaryParticles;   }
    virtual TClonesArray     *GetSecondaryParticles()              { return fSecondaryParticles; }

    virtual TObjArray        *GetVertices()                        { return fVertexList;    }
    virtual TObjArray        *GetSecondaryVertices()               { return fSecVertexList; }

            void    SetNRun(Int_t i)                               { fNRun         = i; }
            void    SetNEvent(Int_t i)                             { fNEvent       = i; }
            void    SetTriggerMask(Int_t i)                        { fTriggerMask  = i; }
            void    SetDate(Int_t i)                               { fDate         = i; }
            void    SetTime(Int_t i)                               { fTime         = i; }
            void    SetMCFlag(Int_t i)                             { fMCFlag       = i; }
            void    SetMomentum(Float_t x)                         { fMomentum     = x; }
            void    SetProjMass(Float_t x)                         { fProjMass     = x; }
            void    SetTargetMass(Float_t x)                       { fTargetMass   = x; }
            void    SetEveto(Float_t x)                            { fEveto        = x; }
            void    SetVertexX(Float_t x)                          { fVertexX      = x; }
            void    SetVertexY(Float_t x)                          { fVertexY      = x; }
            void    SetVertexZ(Float_t x)                          { fVertexZ      = x; }
            void    SetWfaNbeam(Int_t i)                           { fWfaNbeam     = i; }
            void    SetWfaNinter(Int_t i)                          { fWfaNinter    = i; }
            void    SetWfaBeamTime(Int_t i)                        { fWfaBeamTime  = i; }
            void    SetWfaInterTime(Int_t i)                       { fWfaInterTime = i; }

            void    SetPrimaryParticleList(TClonesArray *list)     { fPrimaryParticles   = list;   }
            void    SetSecondaryParticleList(TClonesArray *list)   { fSecondaryParticles = list;   }
            void    ResetPrimaryParticles()                        { fPrimaryParticles->Clear();   }
            void    ResetSecondaryParticles()                      { fSecondaryParticles->Clear(); }

            Int_t   GetNRun()                                      { return fNRun;         }
            Int_t   GetNEvent()                                    { return fNEvent;       }
            Int_t   GetTriggerMask()                               { return fTriggerMask;  }
            Int_t   GetDate()                                      { return fDate;         }
            Int_t   GetTime()                                      { return fTime;         }
            Int_t   GetMCFlag()                                    { return fMCFlag;       }
            Float_t GetMomentum()                                  { return fMomentum;     }
            Float_t GetProjMass()                                  { return fProjMass;     }
            Float_t GetTargetMass()                                { return fTargetMass;   }
    virtual Float_t GetEveto();
            Float_t GetVertexX()                                   { return fVertexX;      }
            Float_t GetVertexY()                                   { return fVertexY;      }
            Float_t GetVertexZ()                                   { return fVertexZ;      }
            Int_t   GetWfaNbeam()                                  { return fWfaNbeam;     }
            Int_t   GetWfaNinter()                                 { return fWfaNinter;    }
            Int_t   GetWfaBeamTime()                               { return fWfaBeamTime;  } 
            Int_t   GetWfaInterTime()                              { return fWfaInterTime; } 
 
    virtual T49TPC        *GetTPC(Int_t kTpcId)                    { return NULL;  }
    virtual T49MonitorTPC *GetMonitorTPC(Int_t kTpcId);
    virtual T49MonitorTOF *GetMonitorTOF(Int_t kTpcId)             { return NULL;  }
    virtual T49Beam       *GetBeam()                               { return fBeam; }
    virtual T49Ring       *GetRing()                               { return fRing; }
    virtual T49Veto       *GetVeto()                               { return fVeto; }

            Float_t GetTDCalEveto();
            Float_t GetCentrality(Int_t td_cal = 1);
            Int_t   GetCentralityClass(Int_t td_cal = 1);
	    Float_t GetCentrality(Float_t eveto); //new
	    
	    Int_t   GetCentralityClass2(); //centrality class according to A. Laszlo
	    

 protected:

    Int_t              fNRun;               // Run Number
    Int_t              fNEvent;             // Event Number
    Int_t              fTriggerMask;        // Trigger Mask
    Int_t              fMCFlag;             // Monte Carlo Flag
    Int_t              fDate;               // Event date
    Int_t              fTime;               // Event time
    Float_t            fMomentum;           // Beam Momentum
    Float_t            fProjMass;           // Projectile Mass
    Float_t            fTargetMass;         // Target Mass
    Float_t            fEveto;              // Veto Energy
    Float_t            fVertexX;            // Reconstructed main vertex X-Position
    Float_t            fVertexY;            // Reconstructed main vertex Y-Position
    Float_t            fVertexZ;            // Reconstructed main vertex Z-Position
    T49BeamRoot       *fBeam;               // Beam counter information
    T49RingRoot       *fRing;               // Ring calorimeter information
    T49MonitorTPCRoot *fMonitorVT1;         // VT1 Monitor Information
    T49MonitorTPCRoot *fMonitorVT2;         // VT2 Monitor Information
    T49MonitorTPCRoot *fMonitorMTR;         // MTR Monitor Information
    T49MonitorTPCRoot *fMonitorMTL;         // MTL Monitor Information
    TClonesArray      *fPrimaryParticles;   // List of particles connected to primary vertex
    TClonesArray      *fSecondaryParticles; // List of particles connected to secondary vertices
    TObjArray         *fVertexList;         // List of vertices

    // Added for version 5
    TObjArray         *fSecVertexList;      // List of secondary vertices

    // Added for version 7  
    Int_t              fWfaNbeam;           // Number of beam particles from WFA channel 1
    Int_t              fWfaNinter;          // Number of interactions from WFA channel 2
    Int_t              fWfaBeamTime;        // WFA time in nsec
    Int_t              fWfaInterTime;       // WFA time in nsec

    // Added for version 9
    static TClonesArray *fgStaticPrimaryParticles;
    static TClonesArray *fgStaticSecondaryParticles;

    // Added for version 10
    T49VetoRoot       *fVeto;               // Veto calorimeter information
    
   
    

    ClassDef(T49EventRoot,10)               // Event information

};

//______________________________________________________________________________
inline T49Vertex *T49EventRoot::GetPrimaryVertex() 
{
  
  if      (T49Vertex *Vertex = GetFittedVertex())
    return Vertex;
  else if (fVertexList->GetEntries())
    return (T49Vertex *) fVertexList->At(0);
  else
    return 0;

}

//______________________________________________________________________________
inline T49Vertex *T49EventRoot::GetFittedVertex() 
{
  
  if (fVertexList->GetEntries() > 1)
    return (T49Vertex *) fVertexList->At(1);
  else
    return 0;

}

//______________________________________________________________________________
inline T49Vertex *T49EventRoot::GetBPDVertex() 
{
  
  if (fVertexList->GetEntries() > 0)
    return (T49Vertex *) fVertexList->At(0);
  else
    return 0;

}

R__EXTERN Bool_t gT49KeepEvent;

#endif 







