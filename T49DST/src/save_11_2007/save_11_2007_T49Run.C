///////////////////////////////////////////////////////////////////////
//
//  The top class of the NA49 mini-DST ROOT-tree.
//
///////////////////////////////////////////////////////////////////////

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TKey.h"
#include "TClonesArray.h"
#include "TList.h"
#include "TSystem.h"

#include "T49Run.h"
#include "T49EventRoot.h"
#include "T49EventMCRoot.h"
#include "PhysConst.h"

T49Run *gT49;

ClassImp(T49Run)

/*
$Log: T49Run.C,v $
Revision 1.26  2006/11/28 17:37:59  cblume
Add eveto calibration by Benjamin

Revision 1.25  2003/04/23 15:34:58  cblume
Add function OpenNextRunTD() GetNextEventID()

Revision 1.24  2003/01/20 16:56:06  cblume
Skip run on RFIO error as provided by Marco

Revision 1.23  2002/10/07 09:57:00  cblume
Update for CASTOR

Revision 1.22  2002/08/29 12:57:41  cblume
Change to RFIO as default

Revision 1.21  2001/12/17 16:23:52  cblume
Bug fix in Open()

Revision 1.20  2001/12/12 17:38:51  cblume
Update default constructors

Revision 1.19  2001/11/26 12:17:01  cblume
Update

Revision 1.18  2001/11/21 16:57:44  cblume
Update

Revision 1.17  2001/11/12 10:52:21  cblume
Update for ROOT v3.01

 * Revision 1.16  2001/07/06  15:21:06  cblume
 * Don't set branch status of fPointIndex
 *
 * Revision 1.15  2001/06/13  08:13:09  cblume
 * Modifications from Marco to avoid memory leaks
 *
 * Revision 1.14  2000/11/09  09:09:02  cblume
 * Change back to NFS as default
 *
 * Revision 1.13  2000/10/25  09:41:17  cblume
 * Set basker size to 8000
 *
 * Revision 1.12  2000/10/09  10:25:08  cblume
 * General update
 *
 * Revision 1.11  2000/08/28  08:30:48  cblume
 * Introduce RFIO
 *
 * Revision 1.10  2000/06/05  12:57:15  cblume
 * V0ParticleOnly added
 *
 * Revision 1.9  2000/04/14  14:23:34  cblume
 * Added SmallParticleOnly()
 *
 * Revision 1.8  2000/04/13  15:12:10  cblume
 * Introduced SetMainVertexOnly() and GetCurrentEvent()
 *
 * Revision 1.7  1999/12/14  10:36:11  cblume
 * Remove constructors with arguments. Safety check on run-type in GetNextEvent()
 *
 * Revision 1.6  1999/12/01  09:39:32  cblume
 * Changed to root 2.23-09. Updated the makefiles. Changes in T49CutTrack, T49Dedx, T49Proj1, T49Proj2, TRootDS
 *
 * Revision 1.5  1999/11/23  12:59:05  cblume
 * Remove files
 *
*/

//______________________________________________________________________________
T49Run::T49Run() 
{
  //
  // T49Run default constructor
  //

  Int_t i;

  fVerbose           = 0;
  for (i = 0; i < 256; i++) {
    fFileName[i] = 0;
    fHostName[i] = 0;
    fRunType[i]  = 0;
  }
  fLocalHost         = NULL; 
  fRunID             = -1;
  fCurrentRunID      = 0;
  fRunNumber         = 0;
  fNumEvent          = 0;
  fMaxEvents         = 0;
  fEventAddress      = NULL;
  fRootFile          = NULL; 
  fTree              = NULL;
  fEventBranch       = NULL;
  fMainVertexOnly    = kFALSE;
  fSmallParticleOnly = kFALSE;
  fV0ParticleOnly    = kFALSE;
#ifdef NODS
  fRFIO              = kFALSE;
#else
  fRFIO              = kTRUE;
#endif
  fIOstat            = 0;

  fXsec              = 0;
  for (Int_t i = 0; i < 5; i++) {
    fCentrClasses[i] = 0;
  }
  for (Int_t i = 0; i < 140; i++) {
    fCentrBins[i]    = 0;
    fCentrBinX [i]   = 0;
  }
  fNrCentrBins       = 0;
  fTdRefRun          = 0;
  fTdA               = 0;
  fTdB               = 0;
  fTdAref            = 0;
  fTdBref            = 0;
  fAutoInit          = 0;
  fTdCal             = 0;
  fThisRunInit       = 0;
  fInitRun           = 0;

  gT49 = this;

  ResetCurrentRunID(); 

}

//______________________________________________________________________________
Bool_t T49Run::Open(Int_t RunNumber,Int_t MaxEvents) 
{
  //  
  // Opens a mini-DST file for run <RunNumber>.
  // The filename is taken from the database file .root49dsts 
  // whose location has to be defined by the environment 
  // variable ROOT49. The files are opened via RFIO per default.
  //

  Int_t   n;
  Int_t   num;
  Char_t  DataBaseFile[256];
  FILE   *fp;

  fRunNumber = RunNumber;

  const Char_t *DirName = gSystem->Getenv("ROOT49"); 
  if (!DirName) {
    fprintf(stderr,"T49Run::Open(): ROOT49 environment variable not defined\n");
    fprintf(stderr,"T49Run::Open(): Can't translate run number %d to filename\n"
                  ,RunNumber);
    return kFALSE;
  }

  sprintf(DataBaseFile,"%s/.root49dsts",DirName);
  fp = fopen(DataBaseFile,"r");
  if (!fp) {
    fprintf(stderr,"T49Run::Open(): Can't find ROOT49 run database %s\n"
                  ,DataBaseFile);
    fprintf(stderr,"T49Run::Open(): Can't translate run number %d to filename\n"
                  ,RunNumber);
    return kFALSE;
  }

  if (fVerbose) printf("T49Run::Open(): Scan database %s\n",DataBaseFile);
  while ((n = fscanf(fp,"%d %d %s %s %s",&fRunID,&num, (char *) &fFileName
                                                     , (char *) &fHostName
                                                     , (char *) &fRunType)) > 0) {
    if (fVerbose > 1) printf("T49Run::Open(): Found %d: %s %s %s\n"
                            ,num,fFileName,fHostName,fRunType);
    if (num == RunNumber) break;
  }
  if (fVerbose) printf("T49Run::Open(): Found %d: %s %s %s\n"
                      ,num,fFileName,fHostName,fRunType);

  if (n > 0) {
    if (fVerbose) printf("T49Run::Open(): Selected file %s\n",fFileName);
    if (fRFIO) {
      Char_t tmpname[256];
      sprintf(tmpname,"rfio:%s",fFileName);
      strcpy(fFileName,tmpname);
    }
    return Open(fFileName,MaxEvents);
  }
  else {
    fprintf(stderr,"T49Run::Open(): Can't translate run number %d to filename\n"
                  ,RunNumber);
    return kFALSE;
  }

}

//______________________________________________________________________________
Bool_t T49Run::Open(Char_t *FileName,Int_t MaxEvents) 
{
  //
  // Opens the mini-DST file <FileName>.
  //

  fNumEvent  = 0;
  fMaxEvents = MaxEvents;
  fIOstat    = 0;

  if (fVerbose) printf("T49Run::Open(): Open ROOT49 Run, file %s\n",FileName);
  fRootFile = TFile::Open(FileName);
  if (fRootFile && fRootFile->IsOpen()) {

    fRootFile->cd();
    fTree = (TTree *) gDirectory->Get("T49");

    // Detect Monte Carlo runs automatically
    Bool_t isMC = kFALSE;
    if (!fTree) {
      fTree = (TTree *) gDirectory->Get("T49MC");
      if (fTree) isMC = kTRUE;
    }          
    if (!fTree) {
      printf("T49Run::Open(): No tree found\n");
      fIOstat = kIONoTree;
      return kFALSE;
    }

    if (fVerbose) {
      printf("T49Run::Open(): Found fTree at %x\n",(Int_t) fTree);
      if (isMC) printf("T49Run::Open(): Tree contains Monte-Carlo data\n");
      if (fTree && (fVerbose > 1)) fTree->Print();
    }

    if (fTree) {

      fTree->SetBasketSize("*",8000);

      if (fVerbose) {
        if (isMC) {
          printf("T49Run::Open(): %d events in T49MC\n"
                ,(Int_t) fTree->GetEntries());
	}
        else {
          printf("T49Run::Open(): %d events in T49\n"  
                ,(Int_t) fTree->GetEntries());
	}
        printf("T49Run::Open(): Switch off bad branches\n");
        fTree->Print();
      }

      fTree->SetBranchStatus("*",1);
      // Switch off processing the secondary particles if requested
      if (fMainVertexOnly)    MainVertexOnly();
      // Switch off processing the full T49ParticleRoot if requested
      if (fSmallParticleOnly) SmallParticleOnly();
      // Switch off processing data members of T49ParticleRoot unneccessary 
      // for the V0 analysis
      if (fV0ParticleOnly)    V0ParticleOnly();

      if (fVerbose) printf("T49Run::Open(): Get Branch 'Event'\n");
      if (!fEventAddress) {
        if (isMC) {
          fEventAddress = new T49EventMCRoot(kTRUE);
        }
        else { 
          fEventAddress = new T49EventRoot(kTRUE);
        }   
      }
      fTree->SetBranchAddress("Event",&fEventAddress);
      fEventBranch = fTree->GetBranch("Event");
      if (!fEventBranch) printf("T49Run::Open(): Couldn't find branch 'Event'\n");

      if (fMaxEvents > fTree->GetEntries()) {
        fMaxEvents = (Int_t) fTree->GetEntries();
      }

    }

  }
  else {

    fprintf(stderr,"T49Run::Open(): fRootFile == NULL\n");
    //exit(0);
    return kFALSE;

  }

  if (fVerbose) printf("T49Run::Open(): Success\n");

  return kTRUE;

}

//______________________________________________________________________________
Bool_t T49Run::Close() 
{
  //
  // Closes a mini-DST file.
  //

  //if (fEventAddress) {
  //  if (fVerbose) {
  //    printf("T49Run::Close(): Call ReCreateArrays\n");
  //  }
  //  fEventAddress->ReCreateArrays();
  //}
  //else {
  //  printf("T49Run::Close(): fEventAddress == NULL\n");
  //}

  if (fRootFile) {
    delete fTree;
    fRootFile->Close();
    delete fRootFile;
    fRootFile = NULL;  
  }
  else {
    fprintf(stderr,"T49Run::Close(): fRootFile == NULL\n");
    return kFALSE;
  }

  return kTRUE;

}

//______________________________________________________________________________
void T49Run::SetRunType(Char_t *RunType)
{
  //
  // Define the run type for eveto calibration
  //

  sprintf(fRunType,"%s",RunType);

}

//______________________________________________________________________________
T49Event *T49Run::GetNextEvent(Int_t skip) 
{
  //
  // Reads in the next event of a mini-DST file.
  // 

  fIOstat    = 0;
  fNumEvent += skip;

  if (fEventAddress) {
    if (fVerbose) {
      printf("T49Run::GetNextEvent(): Clear old ClonesArray\n");
    }
    fEventAddress->ClearParticleLists();
  }

  if (fNumEvent == fMaxEvents) {
    return NULL;
    if (fVerbose) printf("T49Run::GetNextEvent(): Reached %d events\n"
                        ,fMaxEvents);
  }

  if (fVerbose) printf("T49Run::GetNextEvent(): Get event %d\n",fNumEvent);
  if (!(fTree->GetEvent(fNumEvent))) {
    printf("T49Run::GetNextEvent(): I/O error for event %d\n",fNumEvent);
    fIOstat = kIOEventFailed;
    exit(0);
  }
  fNumEvent++;
  if (fEventAddress == NULL) {
    printf("T49Run::GetNextEvent(): fEventAddress is NULL\n");
    return NULL;
  }
  if (fVerbose) printf("T49Run::GetNextEvent(): Found event at %p, I/O status = %d\n"
                      ,fEventAddress,fIOstat);

  return fEventAddress;

}

//______________________________________________________________________________
T49Event *T49Run::GetNextEvent(Char_t *RunType,Bool_t UseLocalData)
{
  //
  // Read in the next event for the given run-type specifier <RunType>.
  // The file are opend autimatically and the filename is taken from the 
  // database file .root49dsts whose location has to be defined by the 
  // environment variable ROOT49.
  // If <UseLocalData> is switched on, only data from the local
  // disk is used.
  //

  return GetNextEvent(RunType,0,0,UseLocalData);

}

//______________________________________________________________________________
T49Event *T49Run::GetNextEvent(Char_t *RunType,Int_t MinNum,Int_t MaxNum
                              ,Bool_t UseLocalData)
{
  //
  // Read in the next event for the given run-type specifier <RunType> with
  // the run number in the range <MinRun> - <MaxRun>.
  // The file are opend automatically and the filename is taken from the 
  // database file .root49dsts whose location has to be defined by the 
  // environment variable ROOT49.
  // If <UseLocalData> is switched on, only data from the local
  // disk is used.
  //

  T49Event *Event;

  // Run already open?
  if (fRootFile) {
    // Check whether the opened file belong to the correct run-type
    if (strcmp(fRunType,RunType) != 0) {
      printf(" T49Run::GetNextEvent(): Run type mismatch: %s, %s\n"
            ,fRunType,RunType);
      return NULL;
    }
    // Look for the next event
    do {
      Event = GetNextEvent(0);
    } 
    while ((Event) && (fIOstat));
    if (Event) {
      return Event;
    }
    // Open the next run when there is no event found
    else {
      Close();
      if (OpenNextRun(RunType,MinNum,MaxNum,UseLocalData)) {
        do {
          Event = GetNextEvent(0);
        } 
        while ((Event) && (fIOstat));
        if (Event) {
          return Event;
	}
        else {
          return NULL;
	}
      }
      else {
        return NULL;
      }
    }
  }
  else {
    if (OpenNextRun(RunType,MinNum,MaxNum,UseLocalData)) {
      do {
        Event = GetNextEvent(0);
      } 
      while ((Event) && (fIOstat));
      if (Event) {
        return Event;
      }
      else {
        return NULL;
      }
    }
    else {
      return NULL;
    }
  }

}

//______________________________________________________________________________
T49Event *T49Run::GetNextEventID(Char_t *RunType,Int_t MinID,Int_t MaxID
                                ,Bool_t UseLocalData)
{
  //
  // Read in the next event for the given run-type specifier <RunType> with
  // the run ID number, corresponding to the physical file number
  //  in the range <MinID> - <MaxID>.
  // The file are opend automatically and the filename is taken from the 
  // database file .root49dsts whose location has to be defined by the 
  // environment variable ROOT49.
  // If <UseLocalData> is switched on, only data from the local
  // disk is used.
  //

  T49Event *Event;

  // Run already open?
  if (fRootFile) {
    // Check whether the opened file belong to the correct run-type
    if (strcmp(fRunType,RunType) != 0) {
      printf(" T49Run::GetNextEvent(): Run type mismatch: %s, %s\n"
            ,fRunType,RunType);
      return NULL;
    }
    // Look for the next event
    do {
      Event = GetNextEvent(0);
    } 
    while ((Event) && (fIOstat));
    if (Event) {
      return Event;
    }
    // Open the next run when there is no event found
    else {
      Close();
      if (OpenNextRunID(RunType,MinID,MaxID,UseLocalData)) {
        do {
          Event = GetNextEvent(0);
        } 
        while ((Event) && (fIOstat));
        if (Event) {
          return Event;
	}
        else {
          return NULL;
	}
      }
      else {
        return NULL;
      }
    }
  }
  else {
    if (OpenNextRunID(RunType,MinID,MaxID,UseLocalData)) {
      do {
        Event = GetNextEvent(0);
      } 
      while ((Event) && (fIOstat));
      if (Event) {
        return Event;
      }
      else {
        return NULL;
      }
    }
    else {
      return NULL;
    }
  }

}

//______________________________________________________________________________
void T49Run::Reset() 
{
  //
  // Resets the event counter.
  //

  fNumEvent = 0;

}

//______________________________________________________________________________
void T49Run::SetMaxEvents(Int_t n) 
{
  //
  // Defines the maximum number of events.
  //

  if (fTree) {
    if (n > fTree->GetEntries()) {
      fMaxEvents = (Int_t) fTree->GetEntries();
    }
    else {
      fMaxEvents = n;
    }
  }

}

//______________________________________________________________________________
Bool_t T49Run::OpenNextRun(Char_t *RunType,Bool_t UseLocalData) 
{
  //
  // Opens the next run for the given run-type specifier <RunType>.
  // The filename is taken from the database file .root49dsts 
  // whose location has to be defined by the environment 
  // variable ROOT49.
  // If <UseLocalData> is switched on, only data from the local
  // disk is used.
  //

  return OpenNextRun(RunType,0,0,UseLocalData);

}

//______________________________________________________________________________
Bool_t T49Run::OpenNextRun(Char_t *RunType,Int_t MinRun, Int_t MaxRun
                         , Bool_t UseLocalData) 
{
  //
  // Opens the next run for the given run-type specifier <RunType> with
  // the run number in the range <MinRun> - <MaxRun>.
  // The filename is taken from the database file .root49dsts 
  // whose location has to be defined by the environment 
  // variable ROOT49.
  // If <UseLocalData> is switched on, only data from the local
  // disk is used.
  //
  if (fRootFile) Close();
  Int_t   n;
  Int_t   num;
  Char_t  DataBaseFile[256];
  FILE   *fp;

  const Char_t *LocalHostName = gSystem->Getenv("HOST"); 
  if (!LocalHostName) {
    fprintf(stderr
       ,"T49Run::OpenNextRun(): HOST environment variable not defined\n");
    fprintf(stderr
       ,"T49Run::OpenNextRun(): Can't select local data files\n");
    UseLocalData = kFALSE;
  }
  else {
    if (fVerbose) printf("T49Run::OpenNextRun(): LocalHostName = '%s'\n"
                        ,LocalHostName);
  }

  const Char_t *DirName = gSystem->Getenv("ROOT49"); 
  if (!DirName) {
    fprintf(stderr
           ,"T49Run::OpenNextRun(): ROOT49 environment variable not defined\n");
    fprintf(stderr
           ,"T49Run::OpenNextRun(): Can't find runs for type '%s'\n",RunType);
    return kFALSE;
  }

  sprintf(DataBaseFile,"%s/.root49dsts",DirName);
  fp = fopen(DataBaseFile,"r");
  if (!fp) {
    fprintf(stderr
           ,"T49Run::OpenNextRun(): Can't find ROOT49 run database \n%s\n"
           ,DataBaseFile);
    fprintf(stderr
           ,"T49Run::OpenNextRun(): Can't find runs for type '%s'\n"
           ,RunType);
    return kFALSE;
  }

  if (fVerbose) {
    if (UseLocalData)
      printf("T49Run::OpenNextRun(): Scan database %s for '%s' && '%s'\n"
            ,DataBaseFile,RunType,LocalHostName);
    else
      printf("T49Run::OpenNextRun(): Scan database %s for '%s'\n"
            ,DataBaseFile,RunType);
  }
  Bool_t is_open = 0;
  while ((n = fscanf(fp,"%d %d %s %s %s",&fRunID,&num, (char *) &fFileName
                                                     , (char *) &fHostName
                                                     , (char *) &fRunType)) > 0) {
    if (fVerbose > 1) printf("T49Run::OpenNextRun(): Found %d: %s %s %s\n"
                            ,num,fFileName,fHostName,fRunType);
    Bool_t run_found = 0;
    if (MaxRun) {
      if ((strcmp(fRunType,RunType) == 0)                            &&
          (!UseLocalData || (strcmp(fHostName,LocalHostName) == 0))  &&
          (fRunID > fCurrentRunID && num >= MinRun && num <= MaxRun)) {
        fCurrentRunID = fRunID;
        fRunNumber    = num;
	run_found=1;
        //break;
      }
    }
    else {
      if ((strcmp(fRunType,RunType) == 0)                            &&
          (!UseLocalData || (strcmp(fHostName,LocalHostName) == 0))  &&
          (fRunID > fCurrentRunID)) {
        fCurrentRunID = fRunID;
        fRunNumber    = num;
	run_found=1;
        //break;
      }
    }
    if (run_found) {
      if (fVerbose) {
	printf("T49Run::OpenNextRun(): Accept %d: %d %s %s %s\n"
	       ,num,fCurrentRunID,fFileName,fHostName,fRunType);
	printf("T49Run::OpenNextRun(): Selected file %s\n",fFileName);
      }
      if (fRFIO) {
	Char_t tmpname[256];
	sprintf(tmpname,"rfio:%s",fFileName);
	strcpy(fFileName,tmpname);
      }
      if (is_open = Open(fFileName))
	break;
    }
  }
  fclose(fp);
 
  if (!is_open) {
    fprintf(stderr,"T49Run::OpenNextRun(): Can't find run with type '%s'\n"
                  ,RunType);
    return kFALSE;
  }

  CheckEvetoCal();

  return is_open;

}

//______________________________________________________________________________
Bool_t T49Run::OpenNextRunID(Char_t *RunType,Int_t MinID, Int_t MaxID
                           , Bool_t UseLocalData) 
{
  //
  // Opens the next run for the given run-type specifier <RunType> with
  // the run ID number, corresponding to the physical file number,
  // in the range <MinID> - <MaxID>.
  // The filename is taken from the database file .root49dsts 
  // whose location has to be defined by the environment 
  // variable ROOT49.
  // If <UseLocalData> is switched on, only data from the local
  // disk is used.
  //

  Int_t   n;
  Int_t   num;
  Char_t  DataBaseFile[256];
  FILE   *fp;

  const Char_t *LocalHostName = gSystem->Getenv("HOST"); 
  if (!LocalHostName) {
    fprintf(stderr
       ,"T49Run::OpenNextRun(): HOST environment variable not defined\n");
    fprintf(stderr
       ,"T49Run::OpenNextRun(): Can't select local data files\n");
    UseLocalData = kFALSE;
  }
  else {
    if (fVerbose) printf("T49Run::OpenNextRun(): LocalHostName = '%s'\n"
                        ,LocalHostName);
  }

  const Char_t *DirName = gSystem->Getenv("ROOT49"); 
  if (!DirName) {
    fprintf(stderr
           ,"T49Run::OpenNextRun(): ROOT49 environment variable not defined\n");
    fprintf(stderr
           ,"T49Run::OpenNextRun(): Can't find runs for type '%s'\n",RunType);
    return kFALSE;
  }

  sprintf(DataBaseFile,"%s/.root49dsts",DirName);
  fp = fopen(DataBaseFile,"r");
  if (!fp) {
    fprintf(stderr
           ,"T49Run::OpenNextRun(): Can't find ROOT49 run database \n%s\n"
           ,DataBaseFile);
    fprintf(stderr
           ,"T49Run::OpenNextRun(): Can't find runs for type '%s'\n"
           ,RunType);
    return kFALSE;
  }

  if (fVerbose) {
    if (UseLocalData)
      printf("T49Run::OpenNextRun(): Scan database %s for '%s' && '%s'\n"
            ,DataBaseFile,RunType,LocalHostName);
    else
      printf("T49Run::OpenNextRun(): Scan database %s for '%s'\n"
            ,DataBaseFile,RunType);
  }
  Bool_t is_open = 0;
  while ((n = fscanf(fp,"%d %d %s %s %s",&fRunID,&num, (char *) &fFileName
                                                     , (char *) &fHostName
                                                     , (char *) &fRunType)) > 0) {
    if (fVerbose > 1) printf("T49Run::OpenNextRun(): Found %d: %s %s %s\n"
                            ,fRunID,fFileName,fHostName,fRunType);
    Bool_t run_found = 0;
    if (MaxID) {
      if ((strcmp(fRunType,RunType) == 0)                            &&
          (!UseLocalData || (strcmp(fHostName,LocalHostName) == 0))  &&
          (fRunID > fCurrentRunID &&  fRunID >= MinID && fRunID <= MaxID)) {
        fCurrentRunID = fRunID;
        fRunNumber    = num;
	run_found=1;
        //break;
      }
    }
    else {
      if ((strcmp(fRunType,RunType) == 0)                            &&
          (!UseLocalData || (strcmp(fHostName,LocalHostName) == 0))  &&
          (fRunID > fCurrentRunID)) {
        fCurrentRunID = fRunID;
        fRunNumber    = num;
	run_found=1;
        //break;
      }
    }
    if (run_found) {
      if (fVerbose) {
	printf("T49Run::OpenNextRun(): Accept %d: %d %s %s %s\n"
	       ,num,fCurrentRunID,fFileName,fHostName,fRunType);
	printf("T49Run::OpenNextRun(): Selected file %s\n",fFileName);
      }
      if (fRFIO) {
	Char_t tmpname[256];
	sprintf(tmpname,"rfio:%s",fFileName);
	strcpy(fFileName,tmpname);
      }
      if (is_open = Open(fFileName))
	break;
    }
  }
  fclose(fp);
 
  if (!is_open) {
    fprintf(stderr,"T49Run::OpenNextRun(): Can't find run with type '%s'\n"
                  ,RunType);
    return kFALSE;
  }

  CheckEvetoCal();

  return is_open;

}

//______________________________________________________________________________
Bool_t T49Run::ListRuns(Char_t *RunType,Bool_t UseLocalData,Bool_t LongList) 
{
  //
  // Lists all runs for the given run-type specifier <RunType> 
  // The filenames are taken from the database file .root49dsts 
  // whose location has to be defined by the environment 
  // variable ROOT49.
  // If <UseLocalData> is switched on, only data from the local
  // disk is used.
  // For <LongList> is TRUE also filename and data host are listed.
  //

  Int_t   n;
  Int_t   num;
  Char_t  DataBaseFile[256];
  FILE   *fp;

  const Char_t *LocalHostName = gSystem->Getenv("HOST"); 
  if (!LocalHostName) {
    fprintf(stderr
           ,"T49Run::ListRuns(): HOST environment variable not defined\n");
    fprintf(stderr,"T49Run::ListRuns(): Can't select local data files\n");
    UseLocalData = kFALSE;
  }

  const Char_t *DirName = gSystem->Getenv("ROOT49"); 
  if (!DirName) {
    fprintf(stderr
           ,"T49Run::ListRuns():  ROOT49 environment variable not defined\n");
    fprintf(stderr
           ,"T49Run::ListRuns(): Can't find runs for type '%s'\n",RunType);
    return kFALSE;
  }

  sprintf(DataBaseFile,"%s/.root49dsts",DirName);
  fp = fopen(DataBaseFile,"r");
  if (!fp) {
    fprintf(stderr
           ,"T49Run::ListRuns(): Can't find ROOT49 run database %s\n"
           ,DataBaseFile);
    fprintf(stderr
           ,"T49Run::ListRuns(): Can't find runs for type '%s'\n"
           ,RunType);
    return kFALSE;
  }

  if (RunType) {
    if (UseLocalData) {
      printf("T49Run::ListRuns(): Scan database %s for '%s' && '%s'\n"
            ,DataBaseFile,RunType,LocalHostName);
    }
    else {
      printf("T49Run::ListRuns(): Scan database %s for '%s'\n"
            ,DataBaseFile,RunType);
    }
  }

  Int_t RunID;
  Char_t FileName[256];
  Char_t HostName[256];
  Char_t RType[256];
  while ((n = fscanf(fp,"%d %d %s %s %s",&RunID,&num, (char *) &FileName
                                                    , (char *) &HostName
                                                    , (char *) &RType)) > 0) {
    if (RunType) { 
      if ((strcmp(RType,RunType) == 0)                             && 
          (!UseLocalData || (strcmp(HostName,LocalHostName) == 0))) {
        if (LongList) {
          printf("T49Run::ListRuns(): Found: %d %s     %s %s\n"
                ,num,RType,FileName,HostName);
	}
        else {
          printf("T49Run::ListRuns(): Found: %d %s \n",num,RType);
	}
      }
    }
    else {
      if (!UseLocalData || (strcmp(HostName,LocalHostName)==0)) {
        if (LongList) {
          printf("T49Run::ListRuns(): Found: %d %s     %s %s\n"
                ,num,RType,FileName,HostName);
	}
        else {
          printf("T49Run::ListRuns(): Found: %d %s \n",num,RType);
	}
      }
    }
  }

  return kTRUE;

}

//______________________________________________________________________________
void T49Run::SmallParticleOnly() 
{

  fTree->SetBranchStatus("fPrimaryParticles.fXLoc*"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fYLoc*"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fZLoc*"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fWeight"        ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fRandom"        ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fLabel"         ,0);
  //fTree->SetBranchStatus("fPrimaryParticles.fPointIndex"    ,0);
 
  fTree->SetBranchStatus("fSecondaryParticles.fXLoc*"       ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fYLoc*"       ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fZLoc*"       ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fNDedxPoint*" ,0);
  //fTree->SetBranchStatus("fSecondaryParticles.fTmeanCharge*",0);
  fTree->SetBranchStatus("fSecondaryParticles.fWeight"      ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fRandom"      ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fLabel"       ,0);
  //fTree->SetBranchStatus("fSecondaryParticles.fPointIndex"  ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofIflag"    ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofId"       ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofX"        ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofY"        ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofPathl"    ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofCharge"   ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofMass2"    ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofSigMass2" ,0);
  
}

//______________________________________________________________________________
void T49Run::V0ParticleOnly() 
{

  fTree->SetBranchStatus("fPrimaryParticles.fIdDet"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fCharge"        ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fNFitPoint*"    ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fNDedxPoint*"   ,0);
  //fTree->SetBranchStatus("fPrimaryParticles.fTmeanCharge*"  ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fPx"            ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fPy"            ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fPz"            ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fSigPx"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fSigPy"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fSigPz"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fBx"            ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fBy"            ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fPchi2"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fWeight"        ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fRandom"        ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fLabel"         ,0);
  //fTree->SetBranchStatus("fPrimaryParticles.fPointIndex"    ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fTofIflag"      ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fTofId"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fTofX"          ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fTofY"          ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fTofPathl"      ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fTofCharge"     ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fTofMass2"      ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fTofSigMass2"   ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fXLoc*"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fYLoc*"         ,0);
  fTree->SetBranchStatus("fPrimaryParticles.fZLoc*"         ,0);
 
  fTree->SetBranchStatus("fSecondaryParticles.fNFitPoint*"  ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fNDedxPoint*" ,0);
  //fTree->SetBranchStatus("fSecondaryParticles.fTmeanCharge*",0);
  fTree->SetBranchStatus("fSecondaryParticles.fWeight"      ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fRandom"      ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fLabel"       ,0);
  //fTree->SetBranchStatus("fSecondaryParticles.fPointIndex"  ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofIflag"    ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofId"       ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofX"        ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofY"        ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofPathl"    ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofCharge"   ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofMass2"    ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fTofSigMass2" ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fXLoc*"       ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fYLoc*"       ,0);
  fTree->SetBranchStatus("fSecondaryParticles.fZLoc*"       ,0);
  
}

//______________________________________________________________________________
void T49Run::MainVertexOnly()
{

  fTree->SetBranchStatus("fSecondaryParticles*",0);
  fTree->SetBranchStatus("fSecVertexList*"     ,0);
      
}

//______________________________________________________________________________
void T49Run::SetEvetoCal(Bool_t tdcal) 
{

  fAutoInit    = 1;
  fTdCal       = tdcal;
  //fThisRunInit = 0;
  fInitRun     = 0;

}

//______________________________________________________________________________
void T49Run::CheckEvetoCal()
{

  // Auto init on and this run not initialised ?
  if ((fAutoInit) && 
      (fInitRun != fRunNumber)) {

    if (fTdCal) {
      InitEvetoCalibration(1,0);
    }
    else {
      InitEvetoCalibration(1,-1);
    }

    fInitRun = fRunNumber;

  } 

}

//______________________________________________________________________________
void T49Run::InitEvetoCalibration(Int_t centOn, Int_t refrun)
{
  //
  // Initialises eveto calibration.
  // <refrun>: reference run for time dependent eveto calibration (A. Laszlo),
  // 0 for first run, -1 for no time dependent calibration
  //

  fTdRefRun = refrun;

  // Correction constants
  const Char_t *dirName = gSystem->Getenv("T49");
        Char_t  dumb[100];
        Char_t  fileName[256];

  TString *rts = new TString(fRunType);
  rts->ReplaceAll("/",1,"_",1);

  for (Int_t j = 0; j < 140; j++) {
    fCentrBins[j]    = 0.0;
    fCentrBinX[j]    = 0.0;
  }
  for (Int_t j = 0; j < 5; j++) {
    fCentrClasses[j] = 0.0;
  }

  //
  // Time dependent calibration of eveto
  //

  fTdA    = 1;
  fTdB    = 0;
  fTdAref = 1;
  fTdBref = 0;

  Int_t  runFound     = 0;
  Int_t  refFound     = 0;
  Bool_t CorFileFound = 0;

  if (fTdRefRun >= 0) {

    sprintf(fileName,"%s/data/%s.corr",dirName,rts->Data());

    ifstream fin3(fileName);

    if (fin3) {

      CorFileFound = 1;
      Float_t av       = 0.0;
      Float_t bv       = 0.0;
      Float_t ave      = 0.0;
      Float_t bve      = 0.0;
      Int_t   runv     = 0;
      Int_t   firstrun = 1;

      fin3 >> dumb >> dumb >> dumb >> dumb >> dumb >> dumb;

      while (fin3 >> runv >> av >> ave >> bv >> bve) {

        if ((fTdRefRun == 0) && (firstrun > 0)) {
          fTdAref   = av;
          fTdBref   = bv;
          firstrun  = 0;
          fTdRefRun = runv;
          refFound  = 1;
        } //if: first run as ref run
        else {
          if ((fTdRefRun > 0) && (runv == fTdRefRun)) {
            // reference run
            fTdAref  = av;
            fTdBref  = bv;
            refFound = 1;
          } //if: ref run
        }
        if (runv == fRunNumber) {
          // actual run
          fTdA     = av;
          fTdB     = bv;
          runFound = 1;
        } //if: actual run

      } // while: read file

      fin3.close();

    } //if: file found
    else {

      printf("TD calibration file not found\n");
      CorFileFound = 0;

    }

    if (!runFound && CorFileFound) {
      printf("No calibration values found for the current run! Time dependent calibration off for this run.\n");
      fTdA    = 1;
      fTdB    = 0;
      fTdAref = 1;
      fTdBref = 0;
    }
    if (!refFound && CorFileFound) {
      printf("No reference run found! Time dependent calibration off for this run.\n");
      fTdA    = 1;
      fTdB    = 0;
      fTdAref = 1;
      fTdBref = 0;
    }

  } //if: time dep. veto cal.

  //
  // Centrality
  //

  if (centOn) {

    if ((refrun < 0) || (!CorFileFound)) {
      sprintf(fileName,"%s/data/%s.centrality"      ,dirName,rts->Data());
    }
    else {
      sprintf(fileName,"%s/data/%s.centrality_tdcal",dirName,rts->Data());
    }

    // Centrality classes
    ifstream fin2(fileName);

    if (fin2) {

      Float_t cent     = 0.0;
      Float_t evetocut = 0.0;
      Int_t   i        = 0;

      fin2 >> dumb >> fXsec;

      while (fin2 >> cent >> evetocut) {

        fCentrBins[i] = evetocut;
        fCentrBinX[i] = cent;
        i++;

        if ((cent >=  5.0) && (fCentrClasses[0] == 0)) {
          fCentrClasses[0] = evetocut;
        }
        if ((cent >= 12.5) && (fCentrClasses[1] == 0)) {
          fCentrClasses[1] = evetocut;
        }
        if ((cent >= 23.5) && (fCentrClasses[2] == 0)) {
          fCentrClasses[2] = evetocut;
        }
        if ((cent >= 33.5) && (fCentrClasses[3] == 0)) {
          fCentrClasses[3] = evetocut;
        }
        if ((cent >= 43.5) && (fCentrClasses[4] == 0)) {
          fCentrClasses[4] = evetocut;
        }

      }

      fin2.close();

      fNrCentrBins = i;

    }
    else {

      printf("centrality table not found\n");

    }

  }

  delete rts;

}

//______________________________________________________________________________
void T49Run::Streamer(TBuffer &R__b)
{
  //
  // Stream an object of class T49Run.
  //

  UInt_t R__s, R__c;
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v > 2) {
      T49Run::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
    }
    TObject::Streamer(R__b);
    R__b >> fVerbose;
    R__b.ReadStaticArray(fFileName);
    R__b.ReadStaticArray(fHostName);
    R__b.ReadStaticArray(fRunType);
    R__b >> fRunID;
    R__b >> fCurrentRunID;
    R__b >> fRunNumber;
    R__b >> fNumEvent;
    R__b >> fMaxEvents;
    R__b >> fEventAddress;
    R__b >> fRootFile;
    R__b >> fTree;
    R__b >> fEventBranch;
    R__b >> fMainVertexOnly;
    R__b >> fSmallParticleOnly;
    R__b >> fV0ParticleOnly;
    R__b >> fRFIO;
    R__b.CheckByteCount(R__s, R__c, T49Run::IsA());
  }
  else {
    T49Run::Class()->WriteBuffer(R__b,this);
  }

}




