#ifndef _T49Run_INCLUDED_
#define _T49Run_INCLUDED_

/*
$Log: T49Run.h,v $
Revision 1.15  2007/11/05 18:30:31  cblume
Update by Banjamin

Revision 1.13  2006/11/28 17:37:39  cblume
Add eveto calibration by Benjamin

Revision 1.12  2003/04/23 15:34:35  cblume
Add function OpenNextRunTD() GetNextEventID()

Revision 1.11  2001/12/12 17:35:39  cblume
Update default constructors

Revision 1.10  2001/11/12 10:49:08  cblume
Update for ROOT v3.01

 * Revision 1.9  2000/08/28  08:33:22  cblume
 * Introduce RFIO
 *
 * Revision 1.8  2000/06/05  12:56:50  cblume
 * V0ParticleOnly added
 *
 * Revision 1.7  2000/04/14  14:21:58  cblume
 * Added SetSmallParticleOnly()
 *
 * Revision 1.6  2000/04/13  14:53:45  cblume
 * Remove files
 *
 * Revision 1.5  1999/12/14  10:36:28  cblume
 * Remove constructors with arguments.
 *
 * Revision 1.4  1999/12/01  09:39:29  cblume
 * Changed to root 2.23-09. Updated the makefiles. Changes in T49CutTrack, T49Dedx, T49Proj1, T49Proj2, TRootDS
 *
 * Revision 1.3  1999/11/23  12:51:29  cblume
 * Remove files
 *
*/

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TObjArray.h"

#include "T49Event.h"

// I/O flags
const Bool_t kIONoTree      = 1;
const Bool_t kIOEventFailed = 2;

class T49Run : public TObject {
    
  public: 

    T49Run();
    virtual ~T49Run() {}
    
    virtual Bool_t     Open(Int_t  RunNumber,Int_t MaxEvents = 10000000);
    virtual Bool_t     Open(Char_t *FileName,Int_t MaxEvents = 10000000);
    virtual Bool_t     OpenNextRun(Char_t *RunType,Bool_t UseLocalData = kFALSE);
    virtual Bool_t     OpenNextRun(Char_t *RunType,Int_t MinNum,Int_t MaxNum,Bool_t UseLocalData = kFALSE);
    virtual Bool_t     OpenNextRunID(Char_t *RunType,Int_t MinID,Int_t MaxID,Bool_t UseLocalData = kFALSE);

    static  Bool_t     ListRuns(Char_t *RunType = NULL,Bool_t UseLocalData = kFALSE,Bool_t LongList = kFALSE);
    virtual Bool_t     Close();
    virtual void       Reset();

    virtual void       SetMaxEvents(Int_t n);
    virtual void       SetVerbose(Bool_t n)      { fVerbose           = n;     }
    virtual void       SetMainVertexOnly()       { fMainVertexOnly    = kTRUE; }
    virtual void       SetSmallParticleOnly()    { fSmallParticleOnly = kTRUE; }
    virtual void       SetV0ParticleOnly()       { fV0ParticleOnly    = kTRUE; }
    virtual void       SetRFIO(Bool_t n)         { fRFIO              = n;     }
    virtual void       SetRunType(Char_t *RunType);
    virtual void       SetRunNumber(Int_t runno) { fRunNumber         = runno; }

    virtual T49Event  *GetNextEvent(Int_t skip = 0);
    virtual T49Event  *GetNextEvent(Char_t *RunType,Bool_t UseLocalData = kFALSE);
    virtual T49Event  *GetNextEvent(Char_t *RunType,Int_t MinNum,Int_t MaxNum,Bool_t UseLocalData = kFALSE);
    virtual T49Event  *GetNextEventID(Char_t *RunType,Int_t MinID,Int_t MaxID,Bool_t UseLocalData = kFALSE);
    virtual T49Event  *GetEvent()                { return fEventAddress;       }

    virtual Int_t      GetMaxEvents()            { return fMaxEvents;          }
    virtual Int_t      GetRunID()                { return fRunID;              }
    virtual Int_t      GetCurrentRunID()         { return fCurrentRunID;       }
    virtual Int_t      GetRunNumber()            { return fRunNumber;          }
    virtual TFile     *GetFile()                 { return fRootFile;           }
    virtual Char_t    *GetRunType()              { return fRunType;            }
    virtual Char_t    *GetLocalHost()            { return fLocalHost;          }
    virtual TTree     *GetTree()                 { return fTree;               }
    virtual T49Event  *GetCurrentEvent()         { return fEventAddress;       }
    virtual Bool_t     GetMainVertexOnly()       { return fMainVertexOnly;     }
    virtual Bool_t     GetSmallParticleOnly()    { return fSmallParticleOnly;  }
    virtual Bool_t     GetV0ParticleOnly()       { return fV0ParticleOnly;     }
    virtual Bool_t     GetRFIO()                 { return fRFIO;               }
    virtual Int_t      GetIOstat()               { return fIOstat;             } 

    virtual void       ResetCurrentRunID()       { fCurrentRunID = -1;         }

            void       InitEvetoCalibration(Int_t centOn = 1, Int_t refrun = 0);

            void       SetEvetoCal(Int_t tdcal = 1);// 0: no time dependent calibration
            										// 1: with time dependent calibration
            										// 2: Centrality classes of A. Laszlo for 158A GeV (event->GetCentralityClass2() !)
            										// 3: Centrality classes of A. Laszlo for 40A GeV
            void       CheckEvetoCal();

            Float_t    GetTDEvetoCalA()          { return fTdA;                }
            Float_t    GetTDEvetoCalB()          { return fTdB;                }
            Float_t    GetTDEvetoCalRefA()       { return fTdAref;             }
            Float_t    GetTDEvetoCalRefB()       { return fTdBref;             }
            Float_t    GetDatasetCrosssection()  { return fXsec;               }
            Float_t    GetCentrBins(Int_t i)     { return fCentrBins[i];       }
            Float_t    GetCentrBinX(Int_t i)     { return fCentrBinX[i];       }
            Float_t    GetCentrClasses(Int_t i)  { return fCentrClasses[i];    }
            Float_t    GetNrCentrBins()          { return fNrCentrBins;        }
	    Bool_t     IsTDCal()		 { return fTDCInitSuccess;     } //returns if TDCal is successful for that run
	    Int_t      GetTDCalRefRun()          { return fTdRefRun;           }
	    

  protected:

    void MainVertexOnly();
    void SmallParticleOnly();
    void V0ParticleOnly();
 
    Bool_t      fVerbose;           // Output control flag      
    Char_t      fFileName[256];     // File name on ROOT cluster
    Char_t      fHostName[256];     // Host name of machine on which data file is stored
    Char_t     *fLocalHost;         //!Pointer to host name of machine on which program is running
    Char_t      fRunType[256];      // Run type identifier
    Int_t       fRunID;             // Sequential run identifier in database
    Int_t       fCurrentRunID;      // Current run ID in database
    Int_t       fRunNumber;         // NA49 Run Number
    Int_t       fNumEvent;          // Event counter for event selection
    Int_t       fMaxEvents;         // User limit for event loop        
    T49Event   *fEventAddress;      // Event data pointer
    TFile      *fRootFile;          // Run File with Example events
    TTree      *fTree;              // Tree containing events
    TBranch    *fEventBranch;       // Tree branch containing event data
    Bool_t      fMainVertexOnly;    // Switches off processing the secondary tracks
    Bool_t      fSmallParticleOnly; // Switches off processing the full T49ParticleRoot
    Bool_t      fV0ParticleOnly;    // Switches off processing the full T49ParticleRoot (only V0 data)
    Bool_t      fRFIO;              // Switches on RFIO
    Int_t       fIOstat;            // I/O status flag

    Float_t     fXsec;              // Total cross section of the dataset
    Float_t     fCentrClasses[5];
    Float_t     fCentrBins[200];
    Float_t     fCentrBinX[200];
    Int_t       fNrCentrBins;
    Int_t       fTdRefRun;          // Reference run
    Float_t     fTdA;
    Float_t     fTdB;
    Float_t     fTdAref;
    Float_t     fTdBref;

    Bool_t      fAutoInit;          // Automatic initialisation
    Int_t       fTdCal;             // Time dependent calibration on ?
    Bool_t      fThisRunInit;       // Is this run initialised ?
    Bool_t 	fTDCInitSuccess;
    Int_t       fInitRun;
    

    ClassDef(T49Run,5)              // T49Run top class    

};

R__EXTERN  T49Run *gT49;

#endif

