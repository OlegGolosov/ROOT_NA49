#ifndef _T49MonitorTPCRoot_H_INCLUDED_
#define _T49MonitorTPCRoot_H_INCLUDED_

/*
$Log: T49MonitorTPCRoot.h,v $
Revision 1.5  2006/08/03 20:04:41  cblume
Remove unneccessary declarations

Revision 1.4  2001/11/12 10:48:04  cblume
Update for ROOT v3.01

 * Revision 1.3  2000/10/09  10:25:04  cblume
 * General update
 *
 * Revision 1.2  1999/11/23  12:51:15  cblume
 * Remove files
 *
*/

#include <TBuffer.h>
#include "T49MonitorTPC.h"

class T49MonitorTPCRoot : public T49MonitorTPC {

public:

    T49MonitorTPCRoot();
    virtual ~T49MonitorTPCRoot() { };

    void     SetNRun(Int_t n)          { fNRun = n;         }
    void     SetNEvent(Int_t n)        { fNEvent = n;       }
    void     SetDate(Int_t n)          { fDate = n;         }
    void     SetTime(Int_t n)          { fTime = n;         }
    void     SetNPoint(Int_t n)        { fNPoint = n;       }
    void     SetNItrack(Int_t n)       { fNItrack = n;      }
    void     SetNRtrack(Int_t n)       { fNRtrack = n;      }
    void     SetNRtrackKpass(Int_t n)  { fNRtrackKpass = n; }
    void     SetNTrack(Int_t n)        { fNTrack = n;       }
    void     SetNVertex(Int_t n)       { fNVertex = n;      }
    void     SetSumAdc(Int_t n)        { fSumAdc = n;       }
    void     SetIflagVdMod(Int_t n)    { fIflagVdMod = n;   }
    void     SetTmeanCharge(Float_t x) { fTmeanCharge = x;  }
    void     SetAvgResX(Float_t x)     { fAvgResX = x;      }
    void     SetAvgResY(Float_t x)     { fAvgResY = x;      }
    void     SetCurVdrift(Float_t x)   { fCurVdrift = x;    }
    void     SetAveVdrift(Float_t x)   { fAveVdrift = x;    }
    void     SetRawVdrift(Float_t x)   { fRawVdrift = x;    }
    void     SetMonVdrift(Float_t x)   { fMonVdrift = x;    }
    void     SetGasGain(Float_t x)     { fGasGain = x;      }
    void     SetEvtGain(Float_t x)     { fEvtGain = x;      }
    void     SetTemperature(Float_t x) { fTemperature = x;  }
    void     SetPressure(Float_t x)    { fPressure = x;     }

    Int_t    GetNRun()                 { return fNRun;         }
    Int_t    GetNEvent()               { return fNEvent;       }
    Int_t    GetDate()                 { return fDate;         }
    Int_t    GetTime()                 { return fTime;         }
    Int_t    GetNPoint()               { return fNPoint;       }
    Int_t    GetNItrack()              { return fNItrack;      }
    Int_t    GetNRtrack()              { return fNRtrack;      }
    Int_t    GetNRtrackKpass()         { return fNRtrackKpass; }
    Int_t    GetNTrack()               { return fNTrack;       }
    Int_t    GetNVertex()              { return fNVertex;      }
    Int_t    GetSumAdc()               { return fSumAdc;       }
    Int_t    GetIflagVdMod()           { return fIflagVdMod;   }
    Float_t  GetTmeanCharge()          { return fTmeanCharge;  }
    Float_t  GetAvgResX()              { return fAvgResX;      }
    Float_t  GetAvgResY()              { return fAvgResY;      }
    Float_t  GetCurVdrift()            { return fCurVdrift;    }
    Float_t  GetAveVdrift()            { return fAveVdrift;    }
    Float_t  GetRawVdrift()            { return fRawVdrift;    }
    Float_t  GetMonVdrift()            { return fMonVdrift;    }
    Float_t  GetGasGain()              { return fGasGain;      }
    Float_t  GetEvtGain()              { return fEvtGain;      }
    Float_t  GetTemperature()          { return fTemperature;  }
    Float_t  GetPressure()             { return fPressure;     }

protected:

    Int_t   fNRun;                  // Run number
    Int_t   fNEvent;                // Event number
    Int_t   fDate;                  // Date from event structure
    Int_t   fTime;                  // Time from event structure
    Int_t   fNPoint;                // Number of points
    Int_t   fNItrack;               // Number of itracks
    Int_t   fNRtrack;               // Number of rtracks
    Int_t   fNRtrackKpass;          // Number of rtracks found in kink finder passes
    Int_t   fNTrack;                // Number of tracks
    Int_t   fNVertex;               // Number of vertices found
    Int_t   fSumAdc;                // Sum of all rawdata ADC counts
    Float_t fTmeanCharge;           // Mean of the truncated mean charge
    Float_t fAvgResX;               // Mean abs x residuum of all i/rtracks
    Float_t fAvgResY;               // Mean abs y residuum of all i/rtracks
    Float_t fCurVdrift;             // Vdrift for this event from charge step
    Float_t fAveVdrift;             // The running average vdrift from charge step
    Float_t fRawVdrift;             // Vdrift from raw data
    Float_t fMonVdrift;             // Process time dependent drift velocity (S)
    Int_t   fIflagVdMod;            // The success flag of the vdrift module
    Float_t fGasGain;               // Gain factor from raw data
    Float_t fEvtGain;               // Eventwise gain factor
    Float_t fTemperature;           // Temperature from raw data
    Float_t fPressure;              // Pressure from raw data

    ClassDef(T49MonitorTPCRoot,2)   // Monitor information

};

#endif
