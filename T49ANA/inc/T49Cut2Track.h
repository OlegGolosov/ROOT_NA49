#ifndef _T49Cut2Track_INCLUDED_
#define _T49Cut2Track_INCLUDED_

/*
$Log: T49Cut2Track.h,v $
 * Revision 1.3  1999/11/23  13:52:09  cblume
 * Remove files
 *
*/

#include "T49Cut.h"
#include "T49Tool.h"

class T49Cut2Track : public T49Cut
{

public:

  T49Cut2Track();
  T49Cut2Track(const Text_t *name, const Text_t *title);
  ~T49Cut2Track();

  void      Reset();

  void      SetStandardCuts();
  Int_t     CheckPair(T49ParticleRoot *track1, T49ParticleRoot *track2);

  TString  *GetCutList();
  void      PrintCutList();

  static void     SetParticleMass(Int_t a, Int_t m); 
  static Float_t  GetPartMass(Int_t i)                  { return (i==1 ? fPart1Mass : fPart2Mass);} 
  
  void      SetMinDist(Double_t x)                      { fFlagDist = kTRUE; 
                                                          fMinDist = x;}
  void      SetArmenterosAlpha(Double_t min, Double_t max)   
                                                        { fFlagArmenterosAlpha = kTRUE; 
                                                          fMinArmenterosAlpha = min; 
                                                          fMaxArmenterosAlpha = max ;}
  void      SetArmenterosPt(Double_t min, Double_t max) { fFlagArmenterosPt = kTRUE; 
                                                          fMinArmenterosPt = min; 
                                                          fMaxArmenterosPt = max ;}
  void      SetPsiLCMS(Double_t min, Double_t max)      { fFlagPsiLCMS  = kTRUE; 
                                                          fMinPsiLCMS = min; 
                                                          fMaxPsiLCMS = max ;}
  void      SetMinQ(Double_t xmin,Double_t ymin,Double_t zmin) 
                                                        { fFlagMinQ = kTRUE; 
                                                          fMinQx = xmin;
                                                          fMinQy = ymin;
                                                          fMinQz = zmin;}
  void      SetQinv(Double_t min, Double_t max)         { fFlagQinv = kTRUE; 
                                                          fMinQinv = min; 
                                                          fMaxQinv = max ;}
  void      SetMinv(Double_t min, Double_t max)         { fFlagMinv = kTRUE; 
                                                          fMinMinv = min; 
                                                          fMaxMinv = max ;}

  void      SetMinDistCutOff()                          { fFlagDist = kFALSE; }
  void      SetArmenterosAlphaCutOff()                  { fFlagArmenterosAlpha = kFALSE; }
  void      SetArmenterosPtCutOff()                     { fFlagArmenterosPt = kFALSE; }
  void      SetPsiLCMSCutOff()                          { fFlagPsiLCMS = kFALSE; }
  void      SetMinQOff()                                { fFlagMinQ    = kFALSE; }
  void      SetQinvOff()                                { fFlagQinv    = kFALSE; }
  void      SetMinvOff()                                { fFlagMinv    = kFALSE; }

protected:

  static Float_t fPart1Mass ;                           //  Mass of first particle
  static Float_t fPart2Mass ;                           //  Mass of second particle

  Int_t       fnum2track;                               //  number of tracks
  Int_t       fnumgood2track;                           //  number of accepted tracks

  Int_t       fFlagDist; 
  Int_t       fnDist;                                
  Double_t    fMinDist;                                 //  Minimum distance of two tracks
 
  Int_t       fFlagArmenterosAlpha;
  Int_t       fnArmenterosAlpha;
  Double_t    fMinArmenterosAlpha;
  Double_t    fMaxArmenterosAlpha;                      //  Armenteros Alpha cut range

  Int_t       fFlagArmenterosPt;
  Int_t       fnArmenterosPt;
  Double_t    fMinArmenterosPt;
  Double_t    fMaxArmenterosPt;                         //  Armenteros Pt cut range

  Int_t       fFlagPsiLCMS;  
  Int_t       fnPsiLCMS;          
  Double_t    fMinPsiLCMS;
  Double_t    fMaxPsiLCMS;                              //  Sign of angle between Q in restfram of pair
                                                        //  and P (in LCMS) 
  Int_t       fFlagMinQ;
  Int_t       fnMinQ;
  Double_t    fMinQx;                                   //  p1x-p2x in LAB frame
  Double_t    fMinQy;                                   //  p1y-p2y in LAB frame
  Double_t    fMinQz;                                   //  p1z-p2z in LAB frame

  Int_t       fFlagQinv;
  Int_t       fnQinv;
  Double_t    fMinQinv;
  Double_t    fMaxQinv;                                 //  Qinvariant cut

  Int_t       fFlagMinv;
  Int_t       fnMinv;
  Double_t    fMinMinv;
  Double_t    fMaxMinv;                                 //  Minvariant cut
 
  ClassDef(T49Cut2Track,1)                              //  Track pair cuts
 			
}; 

#endif


