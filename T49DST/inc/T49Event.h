#ifndef _T49Event_included_
#define _T49Event_included_

/*
$Log: T49Event.h,v $
Revision 1.4  2006/08/03 20:04:03  cblume
Remove unneccessary declarations

Revision 1.3  2001/11/12 10:47:09  cblume
Update for ROOT v3.01

 * Revision 1.2  1999/11/23  12:51:12  cblume
 * Remove files
 *
*/

#include "TObject.h"
#include "TObjArray.h"

// NA49 classes
class T49Particle;
class T49Vertex;
class T49MonitorTPC;
class T49MonitorTOF;
class T49TPC;        
class T49Beam;
class T49Veto;
class T49Ring;

class T49Event  : public TObject {

 public:
    T49Event()           {  };
    virtual ~T49Event()  {  };

    virtual T49Vertex       *GetPrimaryVertex()                    = 0;
    virtual void             ClearParticleLists()                  = 0;
 
    virtual Int_t            GetNParticles(Int_t kSelection = 0)   = 0;

    virtual T49MonitorTPC   *GetMonitorTPC(Int_t kTpcId)           = 0;
    virtual T49Beam         *GetBeam()                             = 0; 
    virtual T49Ring         *GetRing()                             = 0; 

    virtual Int_t            GetNRun()                             = 0;
    virtual Int_t            GetNEvent()                           = 0;
    virtual Int_t            GetTriggerMask()                      = 0;
    virtual Int_t            GetDate()                             = 0;
    virtual Int_t            GetTime()                             = 0;
    virtual Int_t            GetMCFlag()                           = 0;
    virtual Float_t          GetMomentum()                         = 0;
    virtual Float_t          GetProjMass()                         = 0;
    virtual Float_t          GetTargetMass()                       = 0;
    virtual Float_t          GetEveto()                            = 0;

    virtual Bool_t           ReCreateArrays()                      = 0;
    virtual void             SetNRun(Int_t i)                      = 0;
    virtual void             SetNEvent(Int_t i)                    = 0;
    virtual void             SetTriggerMask(Int_t i)               = 0;
    virtual void             SetDate(Int_t i)                      = 0;
    virtual void             SetTime(Int_t i)                      = 0;
    virtual void             SetMCFlag(Int_t i)                    = 0;
    virtual void             SetMomentum(Float_t x)                = 0;
    virtual void             SetProjMass(Float_t x)                = 0;
    virtual void             SetTargetMass(Float_t x)              = 0;
    virtual void             SetEveto(Float_t x)                   = 0;

    ClassDef(T49Event,2) // Event base class

};

#endif

