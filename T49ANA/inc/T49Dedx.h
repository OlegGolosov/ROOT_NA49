#ifndef _T49Dedx_INCLUDED_
#define _T49Dedx_INCLUDED_

#include "TObject.h"
#include "TFile.h"
#include "TList.h"
#include "TObjArray.h"
#include "TTree.h"
#include "TF2.h"

#include "T49EventRoot.h"
#include "T49Particle.h"

/*
$Log: T49Dedx.h,v $
 * Revision 1.2  1999/11/24  16:03:35  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 *
 * Revision 1.1  1999/11/24  15:35:30  cblume
 * Add T49Dedx.h
 *
*/

class T49Dedx : public TObject {
    
  public: 
    T49Dedx();
    virtual ~T49Dedx(){ };

    Int_t Init();
    void    EventFit(T49EventRoot *event, Double_t *p2pi, Double_t *k2pi);
    void    EventFit(Int_t Num);
    void    FitPRatio(Double_t Pmin = 7, Double_t Pmax = 20);

    void    ReadCalib(Int_t RunNumber);
    void    DoEventCalib(T49EventRoot *event, Bool_t DoMultCorr=kTRUE);

    void    SetMinIonNeg(Float_t x) { fMinIonNeg = x; }
    void    SetMinIonPos(Float_t x) { fMinIonPos = x; }
    void    SetRes0(Float_t x)      { fRes0 = x;      }
    void    SetRes1(Float_t x)      { fRes1 = x;      }
    void    GetDedxPred(T49Particle *track,Float_t mass, Float_t *pos, Float_t *sigma);
    

    TF2  *GetFunc();

    void     SetResolution(Double_t x);
    void     SetCalibHome(Char_t *Dir) { sprintf(fCalibHome,"%s",Dir); }
    void     SetMinIon(Double_t x);
    Double_t GetResolution(); 
    Float_t  GetRelRise(Float_t p, Float_t mass);
    Float_t  GetMultCorr(Int_t Mult, Float_t x);
    Double_t GetMinIon();    


    ClassDef(T49Dedx,1)  // T49Dedx top class    


  protected:

    Char_t fCalibHome[256];        //! Home directory for run-by-run calibration files
  
    Float_t fCalibPos0;            // Run dE/dx calibration factor
    Float_t fCalibPos1;            // Run dE/dx calibration factor
    Float_t fCalibNeg0;            // Run dE/dx calibration factor
    Float_t fCalibNeg1;            // Run dE/dx calibration factor

    Float_t fMinIonNeg;            // Minimum Ionisation for neg particles
    Float_t fMinIonPos;            // Minimum Ionisation for pos particles
    Float_t fRes0;                 // Resolution for particle at p=0      
    Float_t fRes1;                 // Momentum dependenc of resolution    

};

#endif
