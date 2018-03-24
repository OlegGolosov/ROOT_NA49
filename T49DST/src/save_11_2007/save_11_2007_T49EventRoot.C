///////////////////////////////////////////////////////////////////////
//
//  Contains the event information and gives access to the rest of
//  the data.
//
///////////////////////////////////////////////////////////////////////

/*
$Log: T49EventRoot.C,v $
Revision 1.20  2006/11/28 17:38:08  cblume
Add eveto calibration by Benjamin

Revision 1.19  2006/08/17 13:24:11  cblume
*** empty log message ***

Revision 1.18  2002/10/07 09:56:17  cblume
Update for CASTOR

Revision 1.17  2002/03/27 14:02:39  cblume
Make GetEveto() run dependent

Revision 1.16  2002/02/04 16:07:15  cblume
Add T49Veto

Revision 1.15  2001/12/12 17:37:22  cblume
Update default constructors

Revision 1.14  2001/11/26 12:15:36  cblume
Cleanup

Revision 1.13  2001/11/21 16:57:28  cblume
Update

Revision 1.12  2001/11/12 10:51:25  cblume
Update for ROOT v3.01

 * Revision 1.11  2001/06/13  08:11:48  cblume
 * Modifications from Marco to avoid memory leaks
 *
 * Revision 1.10  2000/11/09  09:07:38  cblume
 * Introduce the byte count in the streamer
 *
 * Revision 1.9  2000/10/25  09:37:31  cblume
 * Include WFA information
 *
 * Revision 1.8  2000/04/13  15:10:08  cblume
 * Introduced SetMainVertexOnly() and GetCurrentEvent()
 *
 * Revision 1.7  1999/12/01  09:39:31  cblume
 * Changed to root 2.23-09. Updated the makefiles. Changes in T49CutTrack, T49Dedx, T49Proj1, T49Proj2, TRootDS
 *
 * Revision 1.6  1999/11/25  13:57:54  cblume
 * Introduced gT49KeepEvent in T49Event(MC)Root to make events cloneable
 *
 * Revision 1.5  1999/11/24  16:03:54  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 *
 * Revision 1.4  1999/11/23  12:59:00  cblume
 * Remove files
 *
*/

#include "TMemberInspector.h"
#include "TClass.h"

#include "T49Run.h"
#include "T49EventRoot.h"
#include "T49Particle.h"
#include "T49VetoRoot.h"

Bool_t gT49KeepEvent = kFALSE;

TClonesArray *T49EventRoot::fgStaticPrimaryParticles   = NULL; 
TClonesArray *T49EventRoot::fgStaticSecondaryParticles = NULL;

ClassImp(T49EventRoot)

//______________________________________________________________________________
T49EventRoot::T49EventRoot() 
{
  //
  // T49EventRoot constructor
  //

  fNRun          = 0;
  fNEvent        = 0;
  fTriggerMask   = 0;
  fMCFlag        = 0;
  fDate          = 0;
  fTime          = 0;
  fMomentum      = 0;
  fProjMass      = 0;
  fTargetMass    = 0;
  fEveto         = 0;
  fVertexX       = 0;
  fVertexY       = 0;
  fVertexZ       = 0;
  fWfaNbeam      = 0;
  fWfaNinter     = 0;
  fWfaBeamTime   = 0;
  fWfaInterTime  = 0;
  fBeam          = NULL;
  fRing          = NULL;
  fVeto          = NULL;
  fMonitorVT1    = NULL;
  fMonitorVT2    = NULL;
  fMonitorMTR    = NULL;
  fMonitorMTL    = NULL;
  fVertexList    = NULL;
  fSecVertexList = NULL;

  fPrimaryParticles   = NULL;
  fSecondaryParticles = NULL;

}

//______________________________________________________________________________
T49EventRoot::T49EventRoot(Int_t AllocObjects) 
{
  //
  // T49EventRoot constructor
  //

  static Int_t FirstCall = kTRUE;

  fNRun         = 0;
  fNEvent       = 0;
  fTriggerMask  = 0;
  fMCFlag       = 0;
  fDate         = 0;
  fTime         = 0;
  fMomentum     = 0;
  fProjMass     = 0;
  fTargetMass   = 0;
  fEveto        = 0;
  fVertexX      = 0;
  fVertexY      = 0;
  fVertexZ      = 0;
  fWfaNbeam     = 0;
  fWfaNinter    = 0;
  fWfaBeamTime  = 0;
  fWfaInterTime = 0;

  if (AllocObjects) {
    fBeam          = new T49BeamRoot();
    fRing          = new T49RingRoot();
    fVeto          = new T49VetoRoot();
    fMonitorVT1    = new T49MonitorTPCRoot();
    fMonitorVT2    = new T49MonitorTPCRoot();
    fMonitorMTR    = new T49MonitorTPCRoot();
    fMonitorMTL    = new T49MonitorTPCRoot();
    fVertexList    = new TObjArray(10);
    fSecVertexList = new TObjArray(800);
  }
  else {
    fBeam          = NULL;
    fRing          = NULL;
    fVeto          = NULL;
    fMonitorVT1    = NULL;
    fMonitorVT2    = NULL;
    fMonitorMTR    = NULL;
    fMonitorMTL    = NULL;
    fVertexList    = NULL;
    fSecVertexList = NULL;
  }

  if (gT49KeepEvent) {

    fPrimaryParticles   = new TClonesArray("T49ParticleRoot",1000);
    fSecondaryParticles = new TClonesArray("T49ParticleRoot",1000);

  }
  else {

    if (FirstCall) {
      fgStaticPrimaryParticles   = new TClonesArray("T49ParticleRoot",1000);
      fgStaticSecondaryParticles = new TClonesArray("T49ParticleRoot",1000);
      FirstCall = kFALSE;
    }

    if (fgStaticPrimaryParticles) {
      fPrimaryParticles   = fgStaticPrimaryParticles;
      fPrimaryParticles->Clear();
    }
    else {
      fPrimaryParticles   = NULL;
    }
    if (fgStaticSecondaryParticles) {
      fSecondaryParticles = fgStaticSecondaryParticles;
      fSecondaryParticles->Clear();
    }
    else {
      fSecondaryParticles = NULL;
    }

  }

}

//______________________________________________________________________________
T49EventRoot::~T49EventRoot() 
{
  //
  // T49EventRoot destructor
  //

  if (fBeam)       delete fBeam;
  if (fRing)       delete fRing;
  if (fVeto)       delete fVeto;
  if (fMonitorVT1) delete fMonitorVT1;
  if (fMonitorVT2) delete fMonitorVT2;
  if (fMonitorMTR) delete fMonitorMTR;
  if (fMonitorMTL) delete fMonitorMTL;

  if (fVertexList) {
    fVertexList->Delete();	
    delete fVertexList;
  }
  if (fSecVertexList) {
    fSecVertexList->Delete();	
    delete fSecVertexList;
  }

}

//______________________________________________________________________________
Int_t T49EventRoot::GetNParticles(Int_t kSelection) 
{
  //
  // Returns the number of particles.
  //    kSelection = 0  -- primary particles
  //    kSelection = 1  -- seconary particles
  //

  if      (kSelection == 0) {
    if (fPrimaryParticles)   
      return fPrimaryParticles->GetEntries();
    else                     
      return 0;
  }
  else if (kSelection == 1) {
    if (fSecondaryParticles) 
      return fSecondaryParticles->GetEntries();
    else                     
      return 0;
  }
  else {
    return 0;
  }

}

//______________________________________________________________________________
T49MonitorTPC *T49EventRoot::GetMonitorTPC(Int_t ID) 
{
  //
  // Get the monitor information for one TPC.
  //    ID =  2 -- VT1
  //    ID =  4 -- VT2
  //    ID =  8 -- MTR
  //    ID = 16 -- MTL
  //

  if      (ID ==  2)
    return fMonitorVT1;
  else if (ID ==  4)
    return fMonitorVT2;
  else if (ID ==  8)
    return fMonitorMTR;
  else if (ID == 16)
    return fMonitorMTL;
  else {
    fprintf(stderr,"T49EventRoot::GetMonitorTPC(%d): No such TPC\n",ID);
    return NULL;
  }

}

//______________________________________________________________________________
void T49EventRoot::ClearParticleLists() 
{
  //
  // Does a Clear() on the particle list and a Delete() on
  // the vertex list.
  //

  if (fPrimaryParticles) {
    fPrimaryParticles->Clear();
  }
  if (fSecondaryParticles) {
    fSecondaryParticles->Clear();
  }
  if (fVertexList) {
    fVertexList->Delete();
    delete fVertexList;
    fVertexList = NULL;
  }
  if (fSecVertexList) {
    fSecVertexList->Delete();
    delete fSecVertexList;
    fSecVertexList = NULL;
  }

}

//______________________________________________________________________________
Bool_t T49EventRoot::ReCreateArrays() 
{
  //
  // Creates new particle lists.
  //

  if (fgStaticPrimaryParticles) {
    fgStaticPrimaryParticles->Dump();
    delete fgStaticPrimaryParticles;
  }
  if (fgStaticSecondaryParticles) {
    delete fgStaticSecondaryParticles;
  }

  fgStaticPrimaryParticles   = new TClonesArray("T49ParticleRoot",1000);
  fgStaticSecondaryParticles = new TClonesArray("T49ParticleRoot",1000);

  if ((fgStaticPrimaryParticles   != NULL) & 
      (fgStaticSecondaryParticles != NULL)) {
    fPrimaryParticles   = fgStaticPrimaryParticles;
    fSecondaryParticles = fgStaticSecondaryParticles;
    return kTRUE;
  }
  else {
    return kFALSE;
  }

}

//______________________________________________________________________________
Float_t T49EventRoot::GetEveto()
{
  //
  // Returns the veto energy.
  // For the 40GeV min. bias data the recalibrated eveto from T49VetoRoot is
  // returned.
  //

  if ((fNRun >= 3003) && (fNRun <= 3166)) {
    if (fVeto) {
      return fVeto->GetEveto();
    }
    else {
      return 0.0;
    }
  } 
  else {
    return fEveto; 
  }

}


//______________________________________________________________________________
Float_t T49EventRoot::GetTDCalEveto()
{
  //
  // Returns the time dependent calibrated veto energy
  //

  Float_t eveto  = GetEveto();

  Float_t tdA    = gT49->GetTDEvetoCalA();
  Float_t tdB    = gT49->GetTDEvetoCalB();
  Float_t tdAref = gT49->GetTDEvetoCalRefA();
  Float_t tdBref = gT49->GetTDEvetoCalRefB();
  if (tdAref == 0) {
    tdAref = 1.0;
  }

  Float_t tDCalEveto = tdA / tdAref * eveto - (tdBref - tdB) / tdAref;

  return tDCalEveto;

}


//______________________________________________________________________________
Float_t T49EventRoot::GetCentrality(Int_t td_cal)
{
  //
  //  Returns the centrality in percentage to total inelastic cross section.
  //

  Float_t eveto = 0;

  if ((gT49->GetCentrBins(0)  != 0) &&
      (gT49->GetNrCentrBins()  > 0)) {

    if (td_cal > 0) {
      eveto = GetTDCalEveto();
    }
    else {
      eveto = GetEveto();
    }

    Int_t i = 0;
    while ((eveto > gT49->GetCentrBins(i)) &&
           (i     < gT49->GetNrCentrBins())) {
      i++;
    }

    Float_t upCentr  = gT49->GetCentrBinX(i);
    Float_t lowCentr = 0.0;

    if (i > 0) {
      lowCentr = gT49->GetCentrBinX(i-1);
    }

    Float_t upEveto  = gT49->GetCentrBins(i);
    Float_t lowEveto = 0.0;

    if (i > 0) {
      lowEveto = gT49->GetCentrBins(i-1);
    }

    Float_t slope = (upCentr - lowCentr) / (upEveto - lowEveto);
    Float_t centr = lowCentr + slope * (eveto - lowEveto);

    if (gT49->GetCentrBinX(i) > 0) {
      return centr;
    }
    else {
      return gT49->GetDatasetCrosssection();
    }

  }
  else {

    return -1.0;

  }

}

//______________________________________________________________________________
Float_t T49EventRoot::GetCentrality(Float_t eveto)
{
  //
  //  Returns the centrality in percentage to total inelastic cross section.
  //

  if ((gT49->GetCentrBins(0)  != 0) &&
      (gT49->GetNrCentrBins()  > 0)) {

    Int_t i = 0;
    while ((eveto > gT49->GetCentrBins(i)) &&
           (i     < gT49->GetNrCentrBins())) {
      i++;
    }

    Float_t upCentr  = gT49->GetCentrBinX(i);
    Float_t lowCentr = 0.0;

    if (i > 0) {
      lowCentr = gT49->GetCentrBinX(i-1);
    }

    Float_t upEveto  = gT49->GetCentrBins(i);
    Float_t lowEveto = 0.0;

    if (i > 0) {
      lowEveto = gT49->GetCentrBins(i-1);
    }

    Float_t slope = (upCentr - lowCentr) / (upEveto - lowEveto);
    Float_t centr = lowCentr + slope * (eveto - lowEveto);

    if (gT49->GetCentrBinX(i) > 0) {
      return centr;
    }
    else {
      return gT49->GetDatasetCrosssection();
    }

  }
  else {

    return -1.0;

  }

}

//______________________________________________________________________________
Int_t T49EventRoot::GetCentralityClass(Int_t td_cal)
{
  //
  // Returns the centrality class
  //

  Float_t eveto = 0;

  if (gT49->GetCentrClasses(0) != 0) {

    if (td_cal > 0) {
      eveto = GetTDCalEveto();
    }
    else {
      eveto = GetEveto();
    }

    Int_t i = 0;
    while ((eveto > gT49->GetCentrClasses(i)) &&
           (gT49->GetCentrClasses(i) > 0)     &&
           (i < 5)) {
      i++;
    }
    i++;

    return i;

  }
  else {

    return -1;

  }

}

//______________________________________________________________________________
void T49EventRoot::Streamer(TBuffer &R__b) 
{
  //
  // Stream an object of class T49EventRoot.
  //

  UInt_t R__s, R__c;  

  T49BeamRoot *DummyRing;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v > 7) {
      printf("T49EventRoot::Streamer -- R__v > 7\n");
      T49EventRoot::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
    }
    printf("T49EventRoot::Streamer -- R__v = %d\n",R__v);
    T49Event::Streamer(R__b);
    R__b >> fNRun;
    R__b >> fNEvent;
    R__b >> fTriggerMask;
    R__b >> fMCFlag;
    R__b >> fDate;
    R__b >> fTime;
    R__b >> fMomentum;
    R__b >> fProjMass;
    R__b >> fTargetMass;
    R__b >> fEveto;
    R__b >> fVertexX;
    R__b >> fVertexY;
    R__b >> fVertexZ;
    R__b >> fBeam;
    if (R__v < 6) {
      printf("T49EventRoot::Streamer -- R__v < 6\n");
      R__b >> DummyRing;
    }
    else {
      R__b >> fRing;
    }
    R__b >> fMonitorVT1;
    R__b >> fMonitorVT2;
    R__b >> fMonitorMTR;
    R__b >> fMonitorMTL;
    fPrimaryParticles->Streamer(R__b);
    fSecondaryParticles->Streamer(R__b);
    R__b >> fVertexList;
    if (R__v > 4) {
      R__b >> fSecVertexList;
    }
    if (R__v > 6) {
      R__b >> fWfaNbeam;
      R__b >> fWfaNinter;
      R__b >> fWfaBeamTime;
      R__b >> fWfaInterTime;
    } 
    R__b.CheckByteCount(R__s, R__c, T49EventRoot::IsA());
  } 
  else {
    T49EventRoot::Class()->WriteBuffer(R__b,this);
  }

}




