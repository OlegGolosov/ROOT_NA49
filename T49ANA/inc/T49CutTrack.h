#ifndef _T49CutTrack_INCLUDED_
#define _T49CutTrack_INCLUDED_

/*
$Log: T49CutTrack.h,v $
Revision 1.7  2001/11/12 12:05:16  cblume
Update for ROOT v3.01

 * Revision 1.6  2000/11/09  08:58:24  cblume
 * Replace header files by forward declarations. Introduced dEdx-cut with variable sigma
 *
 * Revision 1.5  1999/11/23  13:52:11  cblume
 * Remove files
 *
*/

#include "TArrayI.h"

#include "T49Cut.h"

class TPaveText;
class TCutG;
class TObjArray;
class TF1;

class T49ParticleRoot;

class T49Prob;

class T49CutTrack : public T49Cut
{

public:

    T49CutTrack();
    T49CutTrack(const Text_t *name, const Text_t *title);
    ~T49CutTrack();

    void      Reset();
   
    void      SetStandardCutsPP();   
    void      SetStandardCutsPPb();   
    void      SetStandardCutsPbPb();   

    Int_t     CheckParticle(T49ParticleRoot *track);    
    TObjArray *GetAcceptedParticles(TObjArray *ParticleList);
    void      ReadTofCut(Char_t fname[256]);

    TPaveText *GetCutList();
    void      PrintCutList();

    Double_t  GetRap(Double_t mass, T49ParticleRoot *track);
    
    void      SetIflag(Int_t n)                        { fFlagIflag = kTRUE; 
                                                         fIflag = n; }
    void      SetCharge(Int_t n)                       { fFlagCharge = kTRUE; 
                                                         fCharge = n; }
    void      SetIdDet(Int_t n)                        { fFlagIdDet = kTRUE; 
                                                         fIdDet = n; }
    void      SetIdDetList(TArrayI array)              { fFlagIdDetList = kTRUE; 
                                                         fIdDetList = array; } 
    void      SetBx(Double_t x, Double_t y)            { fFlagBx = kTRUE; 
                                                         fMinBx = x; 
                                                         fMaxBx = y; }
    void      SetBy(Double_t x, Double_t y)            { fFlagBy = kTRUE; 
                                                         fMinBy = x; 
                                                         fMaxBy = y; }
    void      SetBxByEllipse(Double_t x, Double_t y)   { fFlagBxByEllipse = kTRUE; 
                                                         fBxEllipse = x; 
                                                         fByEllipse = y; }
    void      SetNPoint(Int_t i, Int_t x, Int_t y)     { fFlagNPoint[i] = kTRUE; 
                                                         fMinNPoint[i] = x; 
                                                         fMaxNPoint[i] = y; }
    void      SetNFitPoint(Int_t i, Int_t x, Int_t y)  { fFlagNFitPoint[i] = kTRUE; 
                                                         fMinNFitPoint[i] = x; 
                                                         fMaxNFitPoint[i] = y; }
    void      SetNMaxPoint(Int_t i, Int_t x, Int_t y)  { fFlagNMaxPoint[i] = kTRUE; 
                                                         fMinNMaxPoint[i] = x; 
                                                         fMaxNMaxPoint[i] = y; }
    void      SetNPointToNMaxPoint(Int_t i,Double_t x) { fFlagNPointToNMaxPoint[i] = kTRUE; 
                                                         fNPointToNMaxPoint[i] = x; }

    void      SetPchi2(Double_t x, Double_t y)         { fFlagPchi2 = kTRUE; 
                                                         fMinPchi2 = x; 
                                                         fMaxPchi2 = y; }
    void      SetP(Double_t x, Double_t y)             { fFlagP = kTRUE; 
                                                         fMinP = x; 
                                                         fMaxP = y; }
    void      SetPt(Double_t x, Double_t y)            { fFlagPt = kTRUE; 
                                                         fMinPt = x; 
                                                         fMaxPt = y; }
    void      SetPx(Double_t x, Double_t y)            { fFlagPx = kTRUE; 
                                                         fMinPx = x; 
                                                         fMaxPx = y; }
    void      SetPy(Double_t x, Double_t y)            { fFlagPy = kTRUE; 
                                                         fMinPy = x; 
                                                         fMaxPy = y; }
    void      SetPz(Double_t x, Double_t y)            { fFlagPz = kTRUE; 
                                                         fMinPz = x; 
                                                         fMaxPz = y; }
    void      SetXFirst(Double_t x, Double_t y)        { fFlagXFirst = kTRUE; 
                                                         fMinXFirst = x; 
                                                         fMaxXFirst = y; }
    void      SetXLast(Double_t x, Double_t y)         { fFlagXLast = kTRUE; 
                                                         fMinXLast = x; 
                                                         fMaxXLast = y; }
    void      SetYFirst(Double_t x, Double_t y)        { fFlagYFirst = kTRUE; 
                                                         fMinYFirst = x; 
                                                         fMaxYFirst = y; }
    void      SetYLast(Double_t x, Double_t y)         { fFlagYLast = kTRUE; 
                                                         fMinYLast = x; 
                                                         fMaxYLast = y; }
    void      SetZFirst(Double_t x, Double_t y)        { fFlagZFirst = kTRUE; 
                                                         fMinZFirst = x; 
                                                         fMaxZFirst = y; }
    void      SetZLast(Double_t x, Double_t y)         { fFlagZLast = kTRUE; 
                                                         fMinZLast = x; 
                                                         fMaxZLast = y; }
    void      SetNDedxPoint(Int_t i, Int_t x, Int_t y) { fFlagNDedxPoint[i] = kTRUE; 
                                                         fMinNDedxPoint[i] = x; 
                                                         fMaxNDedxPoint[i] = y; }
    void      SetTmeanCharge(Int_t i, Int_t x, Int_t y) { fFlagTmeanCharge[i] = kTRUE; 
                                                          fMinTmeanCharge[i] = x; 
                                                          fMaxTmeanCharge[i] = y; }
    void      SetRap(Int_t i, Double_t x, Double_t y)   { fFlagRap = kTRUE; 
                                                          fRapParticleId = i; 
                                                          fMinRap = x; 
                                                          fMaxRap = y; }

    void      SetTofIflag(Int_t x, Int_t y)            { fFlagTofIflag = kTRUE; 
                                                         fMinTofIflag = x; 
                                                         fMaxTofIflag = y; }
    void      SetTofId(Int_t n)                        { fFlagTofId = kTRUE; 
                                                         fTofId = n; }
    void      SetTofMass2(Double_t x, Double_t y)      { fFlagTofMass2 = kTRUE; 
                                                         fMinTofMass2 = x; 
                                                         fMaxTofMass2 = y; }
    void      SetTofSigMass2(Double_t x, Double_t y)   { fFlagTofSigMass2 = kTRUE; 
                                                         fMinTofSigMass2 = x; 
                                                         fMaxTofSigMass2 = y; }
    void      SetTofPathl(Double_t x, Double_t y)      { fFlagTofPathl = kTRUE; 
                                                         fMinTofPathl = x; 
                                                         fMaxTofPathl = y; }
    void      SetTofX(Double_t x, Double_t y)          { fFlagTofX = kTRUE; 
                                                         fMinTofX = x; 
                                                         fMaxTofX = y; }
    void      SetTofY(Double_t x, Double_t y)          { fFlagTofY = kTRUE; 
                                                         fMinTofY = x; 
                                                         fMaxTofY = y; }
    void      SetTofCharge(Double_t x, Double_t y)     { fFlagTofCharge = kTRUE; 
                                                         fMinTofCharge = x; 
                                                         fMaxTofCharge = y; }
    void      SetTofCutPoly(Char_t fname[256])         { fFlagTofCutPoly = kTRUE; 
                                                         ReadTofCut(fname); }

    void      SetDedx(Double_t x, Double_t y)          { fFlagDedx = kTRUE; 
                                                         fMinDedx = x; 
                                                         fMaxDedx = y; }
    void      SetDedxSigma(Int_t data, Int_t pid, Int_t tpc, Double_t min, Double_t max)   
                                                       { fFlagDedxSigma = kTRUE;
                                                         fDataTyp       = data; 
                                                         fParticleId    = pid;
                                                         fDedxTpc       = tpc;
                                                         fMinDedxSigma  = min;
                                                         fMaxDedxSigma  = max; }
    void      SetDedxVarSigma(Int_t data, Int_t pid, Int_t tpc, Double_t min, Double_t max
                            , TF1 *sigma, Double_t pmin = 0.0);
    void      SetProbability(T49Prob *x)               { fFlagProbability = kTRUE; 
                                                         fProbability = x;}
    void      SetProtonProbability(Double_t x)         { fFlagProtonProbability = kTRUE; 
                                                         fMinProtonProbability = x;
                                                         fDedxTPC = 2; }
    void      SetKaonProbability(Double_t x)           { fFlagKaonProbability = kTRUE; 
                                                         fMinKaonProbability = x;
                                                         fDedxTPC = 2; }
    void      SetPionProbability(Double_t x)           { fFlagPionProbability = kTRUE; 
                                                         fMinPionProbability = x; 
                                                         fDedxTPC = 2; }
    void      SetElectronProbability(Double_t x)       { fFlagElectronProbability = kTRUE; 
                                                         fMinElectronProbability = x;
                                                         fDedxTPC = 2; }
    void      SetProtonProbability(Double_t x,Int_t y) { fFlagProtonProbability = kTRUE; 
                                                         fMinProtonProbability = x;
                                                         fDedxTPC = y; }
    void      SetKaonProbability(Double_t x,Int_t y)   { fFlagKaonProbability = kTRUE; 
                                                         fMinKaonProbability = x;
                                                         fDedxTPC=y; }
    void      SetPionProbability(Double_t x,Int_t y)   { fFlagPionProbability = kTRUE; 
                                                         fMinPionProbability = x;
                                                         fDedxTPC = y; }
    void      SetElectronProbability(Double_t x,Int_t y)   
                                                       { fFlagElectronProbability = kTRUE; 
                                                         fMinElectronProbability = x;
                                                         fDedxTPC = y; }
 
    void      SetIflagCutOff()              { fFlagIflag = kFALSE; }
    void      SetChargeCutOff()             { fFlagCharge = kFALSE; }
    void      SetIdDetCutOff()              { fFlagIdDet = kFALSE; }
    void      SetIdDetListCutOff()          { fFlagIdDetList = kFALSE; } 
    void      SetBxCutOff()                 { fFlagBx = kFALSE; }
    void      SetByCutOff()                 { fFlagBy = kFALSE; }
    void      SetBxByEllipseCutOff()        { fFlagBxByEllipse = kFALSE; }
    void      SetNPointCutOff(Int_t i)      { fFlagNPoint[i] = kFALSE; }
    void      SetNFitPointCutOff(Int_t i)   { fFlagNFitPoint[i] = kFALSE; }
    void      SetNMaxPointCutOff(Int_t i)   { fFlagNMaxPoint[i] = kFALSE; }
    void      SetNPointToNMaxPointCutOff(Int_t i) { fFlagNPointToNMaxPoint[i] = kFALSE;}

    void      SetPchi2CutOff()              { fFlagPchi2 = kFALSE; }
    void      SetPCutOff()                  { fFlagP = kFALSE; }
    void      SetPtCutOff()                 { fFlagPt = kFALSE; }
    void      SetPxCutOff()                 { fFlagPx = kFALSE; }
    void      SetPyCutOff()                 { fFlagPy = kFALSE; }
    void      SetPzCutOff()                 { fFlagPz = kFALSE; }
    void      SetXFirstCutOff()             { fFlagXFirst = kFALSE; }
    void      SetXLastCutOff()              { fFlagXLast = kFALSE; }
    void      SetYFirstCutOff()             { fFlagYFirst = kFALSE; }
    void      SetYLastCutOff()              { fFlagYLast = kFALSE; }
    void      SetZFirstCutOff()             { fFlagZFirst = kFALSE; }
    void      SetZLastCutOff()              { fFlagZLast = kFALSE; }
    void      SetNDedxPointCutOff(Int_t i)  { fFlagNDedxPoint[i] = kFALSE; }
    void      SetTmeanChargeCutOff(Int_t i) { fFlagTmeanCharge[i] = kFALSE; }

    void      SetTofIflagCutOff()           { fFlagTofIflag = kFALSE; }
    void      SetTofIdCutOff()              { fFlagTofId = kFALSE; }
    void      SetTofMass2CutOff()           { fFlagTofMass2 = kFALSE; }
    void      SetTofSigMass2CutOff()        { fFlagTofSigMass2 = kFALSE; }
    void      SetTofPathlCutOff()           { fFlagTofPathl = kFALSE; }
    void      SetTofXCutOff()               { fFlagTofX = kFALSE; }
    void      SetTofYCutOff()               { fFlagTofY = kFALSE; }
    void      SetTofChargeCutOff()          { fFlagTofCharge = kFALSE; }

    void      SetDedxOff()                         { fFlagDedx = kFALSE; }
    void      SetProtonProbabilityCutOff()         { fFlagProtonProbability = kFALSE; }
    void      SetKaonProbabilityCutOff()           { fFlagKaonProbability = kFALSE; }
    void      SetPionProbabilityCutOff()           { fFlagPionProbability = kFALSE; }
    void      SetElectronProbabilityCutOff()       { fFlagElectronProbability = kFALSE; }

    TF1      *GetFunDedxSigma()             { return fFunDedxSigma; }

  protected:
     
    Int_t       fnumtrack;                     //  number of tracks
    Int_t       fnumgoodtrack;                 //  number of accepted tracks

    TObjArray   *fAcceptedParticleList;        //! List of accepted particles after quality cuts  
    T49Prob     *fProbability;                 //!
    Int_t       fFlagProbability;

    TObjArray   *fTofCutPoly;                  // tof dedx/mass2 polygon

    //Track cuts
    Int_t       fFlagIflag;           //  flag of cut
    Int_t       fnIflag;              //  number of not accepted tracks dou to this cut 
    Int_t       fIflag;               //  track iflag to cut on (usually fIflag == 0)

    Int_t       fFlagCharge;          //  flag of cut
    Int_t       fnCharge;             //  number of not accepted tracks dou to this cut 
    Int_t       fCharge;              //  -1 for neg particles +1 pos particles
 
    Int_t       fFlagBx;              //  flag of cut
    Int_t       fnBx;                 //  number of not accepted tracks dou to this cut 
    Double_t    fMinBx;               //  Minumum bx at vertex
    Double_t    fMaxBx;               //  Maximum bx at vertex

    Int_t       fFlagBy;              //  flag of cut
    Int_t       fnBy;                 //  number of not accepted tracks dou to this cut 
    Double_t    fMinBy;               //  Minumum by at vertex
    Double_t    fMaxBy;               //  Maximum by at vertex

    Int_t       fFlagBxByEllipse;     //  flag of bxby elliptic cut
    Int_t       fnBxByEllipse;        //  number of not accepted tracks dou to this cut 
    Double_t    fBxEllipse;           //  radius of bx
    Double_t    fByEllipse;           //  radius of by

    Int_t       fFlagIdDet;           //  flag of cut
    Int_t       fnIdDet;              //  number of not accepted tracks dou to this cut 
    Int_t       fIdDet;               //  

    Int_t       fFlagIdDetList;       //  flag of cut
    Int_t       fnIdDetList;          //
    TArrayI     fIdDetList;           //

    Int_t       fFlagNPoint[4];       //  flag of cut
    Int_t       fnNPoint[4];          //  number of not accepted tracks dou to this cut 
    Int_t       fMinNPoint[4];        //  Minumum number of points for all tpcs fMinNPoint[3]=global
    Int_t       fMaxNPoint[4];        //  Maxumum number of points for all tpcs fManNPoint[3]=global

    Int_t       fFlagNFitPoint[4];    //  flag of cut
    Int_t       fnNFitPoint[4];       //  number of not accepted tracks dou to this cut 
    Int_t       fMinNFitPoint[4];     //  Minumum number of fitted points for all fMinNFitPoint[3]=global
    Int_t       fMaxNFitPoint[4];     //  Maxumum number of fitted points for all fMaxNFitPoint[3]=global

    Int_t       fFlagNMaxPoint[4];    //  flag of cut
    Int_t       fnNMaxPoint[4];       //  number of not accepted tracks dou to this cut 
    Int_t       fMinNMaxPoint[4];     //  Minumum number of fitted points for all fMinNFitPoint[3]=global
    Int_t       fMaxNMaxPoint[4];     //  Maxumum number of fitted points for all fMaxNFitPoint[3]=global

    Int_t       fFlagNPointToNMaxPoint[4];    //  flag of cut
    Int_t       fnNPointToNMaxPoint[4];       //  number of not accepted tracks dou to this cut 
    Double_t    fNPointToNMaxPoint[4];        // ratio number of points to max number of points 
 

    Int_t       fFlagPchi2;           //  flag of cut
    Int_t       fnPchi2;              //  number of not accepted tracks due to this cut 
    Double_t    fMinPchi2;            //  Minumum of chi2 from momentum fit
    Double_t    fMaxPchi2;            //  Maximum of chi2 from momentum fit

    Int_t       fFlagP;               //  flag of cut
    Int_t       fnP;                  //  number of not accepted tracks due to this cut 
    Double_t    fMinP;                //  Minumum p 
    Double_t    fMaxP;                //  Maximum px

    Int_t       fFlagPt;              //  flag of cut
    Int_t       fnPt;                 //  number of not accepted tracks due to this cut 
    Double_t    fMinPt;               //  Minumum pt 
    Double_t    fMaxPt;               //  Maximum pt 

    Int_t       fFlagPx;              //  flag of cut
    Int_t       fnPx;                 //  number of not accepted tracks due to this cut 
    Double_t    fMinPx;               //  Minumum px 
    Double_t    fMaxPx;               //  Maximum px 

    Int_t       fFlagPy;              //  flag of cut
    Int_t       fnPy;                 //  number of not accepted tracks due to this cut 
    Double_t    fMinPy;               //  Minumum px 
    Double_t    fMaxPy;               //  Maximum px 

    Int_t       fFlagPz;              //  flag of cut
    Int_t       fnPz;                 //  number of not accepted tracks due to this cut 
    Double_t    fMinPz;               //  Minumum px 
    Double_t    fMaxPz;               //  Maximum px 

    Int_t       fFlagXFirst;          //  flag of cut
    Int_t       fnXFirst;             //  number of not accepted tracks due to this cut 
    Double_t    fMinXFirst;           //  Minumum XFirst
    Double_t    fMaxXFirst;           //  Maximum XFirst 

    Int_t       fFlagXLast;           //  flag of cut
    Int_t       fnXLast;              //  number of not accepted tracks due to this cut 
    Double_t    fMinXLast;            //  Minumum XLast
    Double_t    fMaxXLast;            //  Maximum XLast 

    Int_t       fFlagYFirst;          //  flag of cut
    Int_t       fnYFirst;             //  number of not accepted tracks due to this cut 
    Double_t    fMinYFirst;           //  Minumum YFirst
    Double_t    fMaxYFirst;           //  Maximum YFirst 

    Int_t       fFlagYLast;           //  flag of cut
    Int_t       fnYLast;              //  number of not accepted tracks due to this cut 
    Double_t    fMinYLast;            //  Minumum YLast
    Double_t    fMaxYLast;            //  Maximum YLast 

    Int_t       fFlagZFirst;          //  flag of cut
    Int_t       fnZFirst;             //  number of not accepted tracks due to this cut 
    Double_t    fMinZFirst;           //  Minumum ZFirst
    Double_t    fMaxZFirst;           //  Maximum ZFirst 

    Int_t       fFlagZLast;           //  flag of cut
    Int_t       fnZLast;              //  number of not accepted tracks due to this cut 
    Double_t    fMinZLast;            //  Minumum ZLast
    Double_t    fMaxZLast;            //  Maximum ZLast 

    Int_t       fFlagNDedxPoint[4];   //  flag of cut
    Int_t       fnNDedxPoint[4];      //  number of not accepted tracks due to this cut 
    Int_t       fMinNDedxPoint[4];    //  Minumum of number of dedx points
    Int_t       fMaxNDedxPoint[4];    //  Maximum of number of dedx points

    Int_t       fFlagTmeanCharge[4];  //  flag of cut
    Int_t       fnTmeanCharge[4];     //  number of not accepted tracks due to this cut 
    Int_t       fMinTmeanCharge[4];   //  Minumum of truncated mean charge
    Int_t       fMaxTmeanCharge[4];   //  Maximum of truncated mean charge

    Int_t       fFlagRap;             //  flag of cut
    Int_t       fnRap;                //  number of not accepted tracks due to this cut 
    Int_t       fRapParticleId;       //  el=0, pi=1, ka=2, pr=3
    Double_t    fMinRap;              //  min rapidity
    Double_t    fMaxRap;              //  max rapidity

    //TOF Cuts
    Int_t       fFlagTofIflag;        //  flag of cut
    Int_t       fnTofIflag;           //  number of not accepted tracks due to this cut 
    Int_t       fMinTofIflag;         //  tof iflag to cut on (usually TofIflag > 0)
    Int_t       fMaxTofIflag;         //  tof iflag to cut on (usually TofIflag > 0)

    Int_t       fFlagTofId;           //  flag of cut
    Int_t       fnTofId;              //  number of not accepted tracks due to this cut 
    Int_t       fTofId;               //  32 = tofl, 128 tofr
 
    Int_t       fFlagTofMass2;        //  flag of cut
    Int_t       fnTofMass2;           //  number of not accepted tracks due to this cut 
    Double_t    fMinTofMass2;         //  Minumum Mass2
    Double_t    fMaxTofMass2;         //  Maximum Mass2 

    Int_t       fFlagTofSigMass2;     //  flag of cut
    Int_t       fnTofSigMass2;        //  number of not accepted tracks due to this cut 
    Double_t    fMinTofSigMass2;      //  Minumum of sig of Mass2
    Double_t    fMaxTofSigMass2;      //  Maximum of sig of Mass2

    Int_t       fFlagTofPathl;        //  flag of cut
    Int_t       fnTofPathl;           //  number of not accepted tracks due to this cut 
    Double_t    fMinTofPathl;         //  Minumum pathlength
    Double_t    fMaxTofPathl;         //  Maximum pathlength

    Int_t       fFlagTofX;            //  flag of cut
    Int_t       fnTofX;               //  number of not accepted tracks due to this cut 
    Double_t    fMinTofX;             //  Minumum x tof coordinate 
    Double_t    fMaxTofX;             //  Maximum x tof coordinate 

    Int_t       fFlagTofY;            //  flag of cut
    Int_t       fnTofY;               //  number of not accepted tracks due to this cut 
    Double_t    fMinTofY;             //  Minumum y tof coordinate 
    Double_t    fMaxTofY;             //  Maximum y tof coordinate 

    Int_t       fFlagTofCharge;       //  flag of cut
    Int_t       fnTofCharge;          //  number of not accepted tracks due to this cut 
    Double_t    fMinTofCharge;        //  Minumum QDC value
    Double_t    fMaxTofCharge;        //  Maximum QDC value
 
    //Tof dedx/mass2 polygon cut
    Int_t       fFlagTofCutPoly;      //  flag of cut
    Int_t       fnTofCutPoly;         //  number of not accepted tracks due to this cut 


    Int_t       fFlagDedx;            //  flag of cut
    Int_t       fnDedx;               //  number of not accepted tracks due to this cut 
    Double_t    fMinDedx;             //  Minumum x dedx coordinate 
    Double_t    fMaxDedx;             //  Maximum x dedx coordinate 

    //probability cuts
    Int_t       fDedxTPC; 
  
    Int_t       fDataTyp;                 // p+p = 0; pb+pb = 1;
    Int_t       fParticleId;              // el=0, pi=1, ka=2, pr=3
    Int_t       fDedxTpc;                 // vt1=0, vt2=1, mtpc=2, global=3

    Int_t       fFlagDedxSigma;           //  flag of cut
    Int_t       fnDedxSigma;              //  number of not accepted tracks due to this cut 
    Double_t    fMinDedxSigma;            //  sigma * mean cut
    Double_t    fMaxDedxSigma;            //  sigma * mean cut

    Int_t       fFlagDedxVarSigma;        //  flag of cut
    TF1        *fFunDedxSigma;            //  Function containing sigma(ptot)
    Double_t    fDedxPmin;                //  Minimum momentum for which cut is applied
 
    Int_t       fFlagProtonProbability;   //  flag of cut
    Int_t       fnProtonProbability;      //  number of not accepted tracks due to this cut 
    Double_t    fMinProtonProbability;    //  Maximum of proton probability cut
    
    Int_t       fFlagKaonProbability;     //  flag of cut
    Int_t       fnKaonProbability;        //  number of not accepted tracks due to this cut 
    Double_t    fMinKaonProbability;      //  Maximum of Kaon probability cut
 
    Int_t       fFlagPionProbability;     //  flag of cut
    Int_t       fnPionProbability;        //  number of not accepted tracks due to this cut 
    Double_t    fMinPionProbability;      //  Maximum of Pion probability cut
      
    Int_t       fFlagElectronProbability; //  flag of cut
    Int_t       fnElectronProbability;    //  number of not accepted tracks due to this cut 
    Double_t    fMinElectronProbability;  //  Maximum of Electron probability cut
      
    Double_t    fp;
    Double_t    fdedx;
    Double_t    fmean;

    ClassDef(T49CutTrack,3)               // Single track cuts
    
}; 
#endif









