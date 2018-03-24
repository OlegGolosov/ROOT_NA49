#ifndef _T49CutEvent_INCLUDED_
#define _T49CutEvent_INCLUDED_

/*
$Log: T49CutEvent.h,v $
 * Revision 1.5  2000/11/09  09:02:15  cblume
 * Replaced header files by forward declarations
 *
 * Revision 1.4  1999/11/23  13:52:10  cblume
 * Remove files
 *
*/

#include "T49Cut.h"

class TPaveText;

class T49EventRoot;

class T49CutEvent : public T49Cut
{

public:

    T49CutEvent();
    T49CutEvent(const Text_t *name, const Text_t *title);

    virtual   ~T49CutEvent() {};

    void      Reset();

    void      SetStandardCutsPP();
    void      SetStandardCutsPPb();
    void      SetStandardCutsPbPb();
    void      SetStandardCutsPbPbMB();

    Int_t     CheckEvent(T49EventRoot *Event); 

    TPaveText *GetCutList();
    void      PrintCutList();
  
    void      SetVertexIflag(Int_t n1,Int_t n2=99,Int_t n3=99) 
                                                   { fFlagVertexIflag = kTRUE; 
                                                     fN1VertexIflag   = n1; 
                                                     fN2VertexIflag   = n2;
                                                     fN3VertexIflag   = n3; }

    void      SetVertexX(Double_t x, Double_t y)   { fFlagVertexX = kTRUE; 
                                                     fMinVertexX = x; 
                                                     fMaxVertexX = y; }
    void      SetVertexY(Double_t x, Double_t y)   { fFlagVertexY = kTRUE; 
                                                     fMinVertexY = x; 
                                                     fMaxVertexY = y; }
    void      SetVertexZ(Double_t x, Double_t y)   { fFlagVertexZ = kTRUE; 
                                                     fMinVertexZ = x; 
                                                     fMaxVertexZ = y; }
    void      SetEVeto(Double_t x, Double_t y)     { fFlagEVeto = kTRUE; 
                                                     fMinEVeto = x; 
                                                     fMaxEVeto = y; }
    void      SetMult(Int_t x, Int_t y)            { fFlagMult = kTRUE; 
                                                     fMinMult = x; 
                                                     fMaxMult = y; }

    void      SetVertexIflagCutOff()               { fFlagVertexIflag = kFALSE; }
    void      SetVertexXCutOff()                   { fFlagVertexX = kFALSE; }
    void      SetVertexYCutOff()                   { fFlagVertexY = kFALSE; }
    void      SetVertexZCutOff()                   { fFlagVertexZ = kFALSE; }
    void      SetEVetoCutOff()                     { fFlagEVeto = kFALSE; }
    void      SetMultCutOff()                      { fFlagMult = kFALSE; }

protected:

    Int_t       fnumevent;                     //  number of events
    Int_t       fnumgoodevent;                 //  number of accepted events

    Int_t       fFlagVertexIflag;              //  flag of cut
    Int_t       fnVertexIflag;                 //  number of not accepted events due to this cut 
    Int_t       fMinVertexIflag;               //  vertex iflag to cut Pb (usually fVertexIflag == 0)
    Int_t       fMaxVertexIflag;               //  in pp (usually fVertexIflag == 0,1)

    Int_t       fN1VertexIflag;                //  accepted vertex flag
    Int_t       fN2VertexIflag;                //  accepted vertex flag
    Int_t       fN3VertexIflag;                //  accepted vertex flag

    Int_t       fFlagVertexX;                  //  flag of cut 
    Int_t       fnVertexX;                     //  number of not accepted events due to this cut 
    Double_t    fMinVertexX;                   //  Minimum X vertex position
    Double_t    fMaxVertexX;                   //  Maximum X vertex position

    Int_t       fFlagVertexY;                  //  flag of cut  
    Int_t       fnVertexY;                     //  number of not accepted events due to this cut 
    Double_t    fMinVertexY;                   //  Minimum Y vertex position
    Double_t    fMaxVertexY;                   //  Maximum Y vertex position

    Int_t       fFlagVertexZ;                  //  flag of cut 
    Int_t       fnVertexZ;                     //  number of not accepted events due to this cut 
    Double_t    fMinVertexZ;                   //  Minimum Z vertex position
    Double_t    fMaxVertexZ;                   //  Maximum Z vertex position

    Int_t       fFlagEVeto;                    //  flag of cut   
    Int_t       fnEVeto;                       //  number of not accepted events due to this cut 
    Double_t    fMinEVeto;                     //  Minimum veto calorimeter energy
    Double_t    fMaxEVeto;                     //  Maximum veto calorimeter energy

    Int_t       fFlagMult;                     //  flag of cut 
    Int_t       fnMult;                        //  number of not accepted events due to this cut 
    Int_t       fMinMult;                      //  Minimum multiplicity
    Int_t       fMaxMult;                      //  Maximum multiplicity

    ClassDef(T49CutEvent,1)                    //  Event cuts

}; 

#endif

