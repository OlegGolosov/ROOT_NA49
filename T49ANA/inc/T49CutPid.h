#ifndef _T49CutPid_INCLUDED_
#define _T49CutPid_INCLUDED_

/*
$Log: T49CutPid.h,v $
Revision 1.3  2001/11/21 16:59:16  cblume
New version from Marco

 * Revision 1.1  2000/01/17  12:37:42  cblume
 * Add T49CutPid.h
 *
*/

#include "TObject.h"
#include "TObjArray.h"
#include "T49ParticleRoot.h"
#include "T49Container.h"
#include "T49Cut.h"

class T49CutPid : public T49Cut {
    
  public: 
    T49CutPid();
    ~T49CutPid();

    TObjArray *GetParticleType(TCollection *tracks, T49Container* Cont, Int_t type, Int_t charge=0);
    TObjArray *GetDeltaDedxParticleType(TCollection *tracks, T49Container* Cont, Int_t type, Int_t charge, Int_t flag=0);
 
    void    SetMinProb(Float_t p)            { fMinProb=p; }
    void    SetDeltaDedx(Float_t a, Float_t b) { fMinDelta= a; fMaxDelta=b; }
    void    ListCuts();

    ClassDef(T49CutPid,1)  // T49CutPid class    


  protected:
 
    Float_t     fMinProb;            // Min probability for PID-cuts
    Float_t     fMaxDelta;           // Maximum Delta for dE/dx PID cut 
    Float_t     fMinDelta;           // Maximum Delta for dE/dx PID cut 
    TObjArray   *fList;
};

#endif












