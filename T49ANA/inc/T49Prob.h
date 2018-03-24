#ifndef _T49Prob_INCLUDED_
#define _T49Prob_INCLUDED_

/*
$Log: T49Prob.h,v $
 * Revision 1.5  1999/11/25  13:57:49  cblume
 * Introduced gT49KeepEvent in T49Event(MC)Root to make events cloneable
 *
 * Revision 1.4  1999/11/23  13:52:16  cblume
 * Remove files
 *
*/

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TCutG.h"
#include "TObject.h"
#include "TArrayF.h"
#include "TMatrix.h"

class T49Stat;
class T49Dedx;
class T49ParticleRoot;
class T49EventRoot;

class T49Prob : public TObject {
    
  public: 
    T49Prob();
    virtual ~T49Prob() {};
 
    TArrayD    GetAmplitude(Int_t charge,Float_t p);
    TArrayD    GetMean(Int_t charge,Float_t p);
    TArrayD    GetSigma(Int_t charge,Float_t p);
    TArrayD    GetDedxAmplitude(Int_t charge,Float_t p,Float_t dedx);
    TArrayD    GetDedxProbability(Int_t charge,Float_t p,Float_t dedx);


    Double_t   GetProtonAmplitude(Int_t charge,Float_t p);
    Double_t   GetKaonAmplitude(Int_t charge,Float_t p);
    Double_t   GetPionAmplitude(Int_t charge,Float_t p);
    Double_t   GetElectronAmplitude(Int_t charge,Float_t p);

    Double_t   GetProtonAmplitude(Int_t charge,Float_t p,Float_t dedx);
    Double_t   GetKaonAmplitude(Int_t charge,Float_t p,Float_t dedx);
    Double_t   GetPionAmplitude(Int_t charge,Float_t p,Float_t dedx);
    Double_t   GetElectronAmplitude(Int_t charge,Float_t p,Float_t dedx);

    Double_t   GetProtonProbability(Int_t charge,Float_t p,Float_t dedx); 
    Double_t   GetKaonProbability(Int_t charge,Float_t p,Float_t dedx); 
    Double_t   GetPionProbability(Int_t charge,Float_t p,Float_t dedx); 
    Double_t   GetElectronProbability(Int_t charge,Float_t p,Float_t dedx); 

    Double_t   GetParaPP(Int_t id, Int_t charge, Float_t p); 
    Double_t   GetParaPbPb(Int_t id, Int_t charge, Float_t p);


    void       ReadDedxPara(Char_t fname[256]);
    void       ReadDedxFit(Char_t fname[256]);

    TMatrix   *GetDedxMatrix()               { return fDedxMatrix; }
    TMatrix   *GetDedxFitPosMatrix()         { return fDedxFitPosMatrix; }
    TMatrix   *GetDedxFitNegMatrix()         { return fDedxFitNegMatrix; }

    ClassDef(T49Prob,1) // Pid-propability class

  protected:
 
 
    TMatrix   *fDedxMatrix;    //! list of dedx fits for cuts
    TMatrix   *fDedxFitPosMatrix; //! list of p fits for cuts
    TMatrix   *fDedxFitNegMatrix; //! list of p fits for cuts
  

   
}; 

#endif

