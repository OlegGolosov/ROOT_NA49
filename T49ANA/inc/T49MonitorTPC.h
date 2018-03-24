#ifndef _T49MonitorTPC_H_INCLUDED_
#define _T49MonitorTPC_H_INCLUDED_

/*
$Log: T49MonitorTPC.h,v $
Revision 1.4  2006/08/03 20:04:34  cblume
Remove unneccessary declarations

Revision 1.3  2001/11/12 10:47:56  cblume
Update for ROOT v3.01

 * Revision 1.2  1999/11/23  12:51:15  cblume
 * Remove files
 *
*/

#include "TObject.h"

class T49MonitorTPC : public TObject {

public:
    T49MonitorTPC() { };
    virtual ~T49MonitorTPC() { };
 
    virtual    void     SetNRun(Int_t n)          = 0;
    virtual    void     SetNEvent(Int_t n)        = 0;
    virtual    void     SetDate(Int_t n)          = 0;
    virtual    void     SetTime(Int_t n)          = 0;
    virtual    void     SetNPoint(Int_t n)        = 0;
    virtual    void     SetNItrack(Int_t n)       = 0;
    virtual    void     SetNRtrack(Int_t n)       = 0;
    virtual    void     SetNRtrackKpass(Int_t n)  = 0;
    virtual    void     SetNTrack(Int_t n)        = 0;
    virtual    void     SetNVertex(Int_t n)       = 0;
    virtual    void     SetSumAdc(Int_t n)        = 0;
    virtual    void     SetIflagVdMod(Int_t n)    = 0;
    virtual    void     SetTmeanCharge(Float_t x) = 0;
    virtual    void     SetAvgResX(Float_t x)     = 0;
    virtual    void     SetAvgResY(Float_t x)     = 0;
    virtual    void     SetCurVdrift(Float_t x)   = 0;
    virtual    void     SetAveVdrift(Float_t x)   = 0;
    virtual    void     SetRawVdrift(Float_t x)   = 0;
    virtual    void     SetMonVdrift(Float_t x)   = 0;
    virtual    void     SetGasGain(Float_t x)     = 0;
    virtual    void     SetEvtGain(Float_t x)     = 0;
    virtual    void     SetTemperature(Float_t x) = 0;
    virtual    void     SetPressure(Float_t x)    = 0;

    virtual    Int_t    GetNRun()          = 0;
    virtual    Int_t    GetNEvent()        = 0;
    virtual    Int_t    GetDate()          = 0;
    virtual    Int_t    GetTime()          = 0;
    virtual    Int_t    GetNPoint()        = 0;
    virtual    Int_t    GetNItrack()       = 0;
    virtual    Int_t    GetNRtrack()       = 0;
    virtual    Int_t    GetNRtrackKpass()  = 0;
    virtual    Int_t    GetNTrack()        = 0;
    virtual    Int_t    GetNVertex()       = 0;
    virtual    Int_t    GetSumAdc()        = 0;
    virtual    Int_t    GetIflagVdMod()    = 0;
    virtual    Float_t  GetTmeanCharge()   = 0;
    virtual    Float_t  GetAvgResX()       = 0;
    virtual    Float_t  GetAvgResY()       = 0;
    virtual    Float_t  GetCurVdrift()     = 0;
    virtual    Float_t  GetAveVdrift()     = 0;
    virtual    Float_t  GetRawVdrift()     = 0;
    virtual    Float_t  GetMonVdrift()     = 0;
    virtual    Float_t  GetGasGain()       = 0;
    virtual    Float_t  GetEvtGain()       = 0;
    virtual    Float_t  GetTemperature()   = 0;
    virtual    Float_t  GetPressure()      = 0;

    ClassDef(T49MonitorTPC,2) // Monitor base class
   
};

#endif
