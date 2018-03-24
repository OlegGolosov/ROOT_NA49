#ifndef _T49Mixer_INCLUDED_
#define _T49Mixer_INCLUDED_

/*
$Log: T49Mixer.h,v $
Revision 1.8  2008/01/04 10:48:58  cblume
64bit modifications by Marek Szuba

Revision 1.7  2005/05/20 11:28:05  cblume
Get rid of deprecated warnings

Revision 1.6  2001/06/13 08:16:54  cblume
Update from Peter Filip

Revision 1.5  2000/02/23 09:42:24  cblume
Update by P.Filip: Speed optimization and Documentation

Revision 1.4  1999/12/20 17:18:46  cblume
Upgrade by P. Filip

 * Revision 1.3  1999/12/01  09:39:20  cblume
 * Changed to root 2.23-09. Updated the makefiles. Changes in T49CutTrack, T49Dedx, T49Proj1, T49Proj2, TRootDS
 *
 * Revision 1.2  1999/11/24  16:03:36  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 *
*/

#include <iostream>
using namespace std;

/* ROOT includes */
#include "TClass.h"
#include "TRandom.h"
#include "TObject.h"
#include "TArrayI.h"
#include "TObjArray.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TDialogCanvas.h"
#include "TButton.h"
#include "TH2.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TArrow.h"
#include "TText.h"
#include "TROOT.h"


class T49Mixer : public TObject 
{
protected:
  Int_t nPools;                        // Number of active Pools (1 or 2)
  TObjArray *fPool1, *fPool2 ;         // Pool1 and Pool2  
  Int_t fN1MAX, fN2MAX;                // Original n1,n2 called in Constructor 
  Int_t fIdx1MAX, fIdx2MAX;            // Global max. value of Idx1,Idx2 = fNxMAX-1
  Int_t fMaxIdx1, fMaxIdx2;            // Current max. value of Idx1,Idx2 = nObj-1 
  Int_t nMaxMXX, nMaxMXY;              // x-size of fMaskXX and fMaskXY
  Int_t nObjects1,nObjects2;           // Number of objects in Pool1, Pool2 
  TArrayC   *fUsed1, *fUsed2;          // Mask of Used Objects in Pool1 and Pool2
  Int_t fP1Full, fP2Full;              // Flags to indicate nObjects == nMaxObjects
  Int_t fP1P2Mode;                     // Mode for P1P2 pairs (1 -> excludes (i,i) )
  Int_t fP1Rx, fP2Rx;                  // Replacement counters for P1, P2 (PUT)

  Int_t fL1, fL2;                      // Last idx1, idx2 for Not Used P1 and P2 Objects
  Int_t fL11x1,fL11x2;                 // Last idx1 and idx2 for P1P1Nused pairs
  Int_t fL22x1,fL22x2;                 // Last idx1 and idx2 for P2P2Nused pairs 
  Int_t fL12x1,fL12x2;                 // Last idx1 and idx2 for P1P2Nused pairs
  Int_t fR11x1,fR11x2;                 // Last idx1 and idx2 for RP1P1Nused pairs
  Int_t fR22x1,fR22x2;                 // Last idx1 and idx2 for RP2P2Nused pairs
  Int_t fR12x1,fR12x2;                 // Last idx1 and idx2 for RP1P2Nused pairs
  Int_t fU11x1,fU11x2;                 // Last idx1 and idx2 for P1P1Unique pairs
  Int_t fU22x1,fU22x2;                 // Last idx1 and idx2 for P2P2Unique pairs
  Int_t fU12x1,fU12x2;                 // Last idx1 and idx2 for P1P2Unique pairs
  Int_t fLAC,fLACp,fLACu,fLAC1,fLAC2;  // Last Action Codes
  Char_t fLACName[100];                // Last Action Name

  Int_t fID;                           // ID if this Mixer in GLOBAL gT49Mixer TObjArray
  Char_t fIDS[100];                    // Name+ID to identify Hist, Canvas

  Int_t fVerbose;                      // Flag to print out info for debugging
  Int_t fSeeFlag;                      // to make Pool1 & Pool2 blind during Get action
   
  Int_t     nUsedP1P1, nUsedP2P2;      // counter of Used P1P1 and P2P2 Pairs
  Int_t     nUsedP1P2;                 // counter of Used P1P2 Pairs
  Int_t     nUsedUniP1P1, nUsedUniP2P2;// counter of Used Unique P1P1 and P2P2 Pairs
  Int_t     nUsedUniP1P2, nUsedUniP2P1;// counter of Used Unique P1P2-P2P1 Pairs
  TArrayI   *fMaskXX,*fMaskXY;         // Mask of Used PairsP1P1, P2P2 and P1P2;
  TArrayI   *fUniq11, *fUniq22;        // Mask & Memory of Used Unique P1-P1 and P2-P2 Pairs 
  TArrayI   *fUniq12, *fUniq21;        // Mask & Memory of Used Unique P1-P2 Pairs 
  UInt_t     fx010[32], fx101[32];     // Bit Masks for OR, AND operations

  TObject   *O1,*O2;                   // Two TObject pointers used in B.C. module
  Int_t      I1, I2;                   // Last indexes used in B.C. module
// GUI variables
  TH2C      *HistXX,*HistYY,*HistXY;   // Histo for P1P1, P2P2 and P1P2 pairs
  TDialogCanvas *cXX,*cYY,*cXY;        
  TDialogCanvas *cP1,*cP2,*cUP1P1;     // canvas for P1P1, P2P2, P1P2, P1, P2
  TDialogCanvas *cUP2P2, *cUP1P2;      // canvas for Unique P1P1, P2P2, P1P2
  TDialogCanvas *cGIF, *cEx;           // canvas for Graph.Int.Face & for Exit
  TStyle    TS;                        // TStyle for GUI

//PROTECTED INTERNAL ROUTINES//

  void      SetUsedUniqueIDXP1(Int_t Idx1, Int_t Idx2);    // Internal routine
  Int_t     GetUsedUniqueIDXP1(Int_t Idx);                 // Internal routine
  void      SetUsedUniqueIDXP2(Int_t Idx1, Int_t Idx2);    // Internal routine
  Int_t     GetUsedUniqueIDXP2(Int_t Idx);                 // Internal routine
  void      SetUsedUniqueIDXP1x2(Int_t Idx1, Int_t Idx2);  // Internal routine
  Int_t     GetUsedUniqueIDXP1x2(Int_t Idx);               // Internal routine
  void      SetUsedUniqueIDXP2x1(Int_t Idx1, Int_t Idx2);  // Internal routine
  Int_t     GetUsedUniqueIDXP2x1(Int_t Idx);               // Internal routine
  Int_t     NofFreeP1x2();                                 // Internal for UniqueRndP1P2
  Int_t     NofFreeP2x1();                                 // Internal for UniqueRndP1P2
  Bool_t    UsedUniqueP1(Int_t idx1);                      //
  Bool_t    UsedUniqueP2(Int_t idx1);                      //
  void      SetUsedUniqueP1(Int_t idx1);                   //
  void      SetUsedUniqueP2(Int_t idx1);                   //
  void      SetNUsedUniqueP1(Int_t idx1);                  //
  void      SetNUsedUniqueP2(Int_t idx1);                  //
  Bool_t    UsedUniqueP1x2(Int_t idx1);                    //
  Bool_t    UsedUniqueP2x1(Int_t idx2);                    //
  void      SetUsedUniqueP1x2(Int_t idx1);                 //
  void      SetUsedUniqueP2x1(Int_t idx2);                 // 
  void      SetNUsedUniqueP1x2(Int_t idx1);                //  
  void      SetNUsedUniqueP2x1(Int_t idx2);                //
  Bool_t    UsedPairP1P1(Int_t idx);                       // Internal Fast Routine
  Bool_t    UsedPairP2P2(Int_t idx);                       // Internal Fast Routine
  Bool_t    UsedPairP1P2(Int_t idx);                       // Internal Fast Routine

  void      SetNUsedAfterReplaceP1(Int_t idx);             // Internal routine
  void      SetNUsedAfterReplaceP2(Int_t idx);             // Internal routine

  void      InitRnd();                                     // Init Random Generator
  void      Init101();                                     // Init BITMASKS

  Bool_t    CheckBound1(Int_t m) { return (( 0<=m && m < nObjects1) ? 1 : 0 );}
  Bool_t    CheckBound2(Int_t m) { return (( 0<=m && m < nObjects2) ? 1 : 0 );}

public:
            // T49Mixer(); defined by the line below //
            T49Mixer(Int_t n = 100);
            T49Mixer(Int_t n1, Int_t n2);
           ~T49Mixer();

//MIXER//
  Bool_t    IsFull();                         // Pool(s) are Full
  void      PurgeMixer();                          // Purge Pools
  void      ClearMixer();                          // Clear Pools
  void      SetMixer(TObjArray *A1,TObjArray *A2); // Set Pools
  void      PutToMixer(TObject *T1, TObject *T2);  // Add Object T1 to Pool1 and T2 to Pool2
  void      ResetMixer();                          // Reset All used info Pairs + Objects
  void      ShowMixer();                           // Show GUI of Mixer
  void      DumpMixer();                           // Dump Pair Info in Mixer
  void      SetVerbose(Int_t n=1) {fVerbose=n;};   // turn ON/OFF debug info
  void      SetP1P2Mode(Int_t m=0);                // turn ON/OFF "identical" (i,i) P1P2 pairs
  Int_t     GetP1P2Mode() { return fP1P2Mode; };   // get current setting of P1P2Mode

  void      AllUsedReset();                        // Reset all Used Info. (Pairs+Objects)

//POOL//
//-------------------PURGE/CLEAR/Re/SET-POOL1/POOL2---------------------------//
  Bool_t    Pool1Full();                // Pool1 is Full
  Bool_t    Pool2Full();                // Pool2 is Full
  Bool_t    PoolsFull();                // Mixer is Full

  void      PurgePool1();               // Empty Pool1 (delete Objects)
  void      PurgePool2();               // Empty Pool2 (delete Objects)
  void      PurgePools();               // Empty Pool1 & Pool2 if exists 

  void      ClearPool1();               // Clear Pool1 (do Not delete Obj.)
  void      ClearPool2();               // Clear Pool2 (do Not delete Obj.)
  void      ClearPools();               // Clear Pool1 & Pool2 if exists

  Bool_t    SetPool1(TObjArray *ObjArray); // Fast way to fill Pool1
  Bool_t    SetPool2(TObjArray *ObjArray); // Fast way to fill Pool2
  Bool_t    SetPools(TObjArray *ObjA1, TObjArray *ObjA2);  // Set P1&P2
 
  void      AllUsedResetP1();          // Reset Used P1 and P1P1/UniP1P1
  void      AllUsedResetP2();          // Reset Used P2 and P2P2/UniP2P2

//OBJECT//
//------------------------------PUT=ADD/REPLACE-OBJECT---------------------------//      
  Int_t     AddOne(TObject *Obj);      // Add object, (Pool1) return (idx) 
  Int_t     AddToP1(TObject *Obj);     // Add to Pool1, return (idx) 
  Int_t     AddToP2(TObject *Obj);     // Add to Pool2, return (idx)
  Bool_t    AddToP1P2(TObject *Obj1, TObject *Obj2);    // return success

  Int_t     ReplaceOneAt(TObject *Obj, Int_t idx);  // replace Obj(idx), delete old in (P1)
  Int_t     ReplaceInP1At(TObject *Obj, Int_t idx); // replace Obj(idx) in P1, delete old 
  Int_t     ReplaceInP2At(TObject *Obj, Int_t idx); // replace Obj(idx) in P2, delete old 
  Bool_t    ReplaceInP1P2At(TObject *Obj1, TObject *Obj2, Int_t idx1, Int_t idx2); 

  Int_t     ReplaceOne(TObject *Obj);  // replace Obj(fP1Rx), delete old Obj (Pool1)
  Int_t     ReplaceInP1(TObject *Obj); // replace Obj(fP1Rx) in P1, delete old Obj
  Int_t     ReplaceInP2(TObject *Obj); // replace Obj(fP2Rx) in P2, delete old Obj
  Bool_t    ReplaceInP1P2(TObject *Obj1, TObject *Obj2); // Replace Pair

  Int_t     PutOne(TObject *Obj);      // Add or Replace (P1)
  Int_t     PutToP1(TObject *Obj);     // Add or Replace according to Pool1Full()
  Int_t     PutToP2(TObject *Obj);     // Add or Replace according to Pool2Full()
  Bool_t    PutToP1P2(TObject *Obj1, TObject *Obj2);  // Add or Replace Pair
//---------------------------GET-OBJECT-----------------------------------------//
  TObject*  GetOne();                  // get * to random object  (Pool1) 
  TObject*  GetOneAt(Int_t idx);       // get * to object at idx  (Pool1)
  TObject*  GetFromP1();               // get * to random object from Pool1 
  TObject*  GetFromP2();               // get * to random object from Pool2
  TObject*  GetFromPool1(Int_t &idx);  // get * to random object from Pool1 set *idx 
  TObject*  GetFromPool2(Int_t &idx);  // get * to random object from Pool2 set *idx
  TObject*  GetFromP1At(Int_t idx);    // get * to object in Pool1 at idx 
  TObject*  GetFromP2At(Int_t idx);    // get * to object in Pool2 at idx
//NOTUSED
  TObject*  GetNuOne();                // get * to First Not Used object (Pool1)
  TObject*  GetNuROne();               // get * to Randm Not Used object (Pool1)
  TObject*  GetNuOneP1();              // get * to First Not Used object in Pool1
  TObject*  GetNuOneP2();              // get * to First Not Used object in Pool2
  TObject*  GetNuROneP1();             // get * to Randm Not Used object in Pool1
  TObject*  GetNuROneP2();             // get * to Randm Not Used object in Pool2
//------------------------USED/SET-USED/SET-NUSED-OBJECT-------------------------//
  Bool_t    UsedOneP1(Int_t idx);
  Bool_t    UsedOneP2(Int_t idx);

  void      SetUsedOneP1(Int_t idx);
  void      SetUsedOneP2(Int_t idx);

  void      SetNUsedOneP1(Int_t idx);
  void      SetNUsedOneP2(Int_t idx);

  void      ResetUP1();                   // Reset used P1 Objects info
  void      ResetUP2();                   // Reset used P2 Objects info
//--------------------------NUMBER-OF-OBJECTS--------------------------------//
  Int_t     NofUsed();                  // number of Used objects (Pool1)
  Int_t     NofFree();                  // number of Unused objects (Pool1)
  Int_t     NofObjects();               // number of objects in (Pool1)

  Int_t     NofUsedP1();                // number of Used objects in Pool1
  Int_t     NofFreeP1();                // number of Unused objects in Pool1
  Int_t     NofObjectsP1();             // number of objects in Pool1

  Int_t     NofUsedP2();                // number of Used objects in Pool2
  Int_t     NofFreeP2();                // number of Unused objects in Pool2
  Int_t     NofObjectsP2();             // number of objects in Pool2


//PAIR//
//--------------------------RESET-PAIRS--------------------------------------------//
  void      ResetAllPxPx();             // Reset P1P1-P2P2-P1P2 Pairs
  void      ResetP1P1_P2P2();           // Reset P1P1 & P2P2 !FAST!
  void      ResetP1P1();                // Reset P1P1 Pairs
  void      ResetP2P2();                // Reset P2P2 Pairs
  void      ResetP1P2();                // Reset P1P2 Pairs
  void      ResetAllUniPxPx();          // Reset P1P1 P2P2 P1P2 Unique Pairs
  void      ResetUniP1P1();             // Reset P1P1 Unique Pairs
  void      ResetUniP2P2();             // Reset P2P2 Unique Pairs
  void      ResetUniP1P2();             // Reset P1P2 Unique Pairs
//----------------------------GET-PAIRS--------------------------------------------//
  Bool_t    GetPairAt(TObject** Obj1, TObject** Obj2, Int_t idx1, Int_t idx2);
  Bool_t    GetPairP1P1At(TObject** Obj1, TObject** Obj2, Int_t idx1, Int_t idx2);
  Bool_t    GetPairP2P2At(TObject** Obj1, TObject** Obj2, Int_t idx1, Int_t idx2);
  Bool_t    GetPairP1P2At(TObject** Obj1, TObject** Obj2, Int_t idx1, Int_t idx2);
//RANDOM
  Bool_t    GetRPair(TObject** Obj1,TObject** Obj2);       // i<>j
  Bool_t    GetRPairP1P1(TObject** Obj1,TObject** Obj2);   // i<>j
  Bool_t    GetRPairP2P2(TObject** Obj1,TObject** Obj2);   // i<>j
  Bool_t    GetRPairP1P2(TObject** Obj1,TObject** Obj2);   // i<>=j
  Bool_t    GetRPairP1P1x(TObject** Obj1,TObject** Obj2);   // i<>j
  Bool_t    GetRPairP2P2x(TObject** Obj1,TObject** Obj2);   // i<>j
  Bool_t    GetRPairP1P2x(TObject** Obj1,TObject** Obj2);   // i<>=j
//NOT-USED
  Bool_t    GetNuPair(TObject** Obj1,TObject** Obj2);      // Not Used Pair i<>j
  Bool_t    GetNuPairP1P1(TObject** Obj1,TObject** Obj2);  // Not Used Pair i<>j
  Bool_t    GetNuPairP2P2(TObject** Obj1,TObject** Obj2);  // Not Used Pair i<>=j
  Bool_t    GetNuPairP1P2(TObject** Obj1,TObject** Obj2);
//NOT-USED-RANDOM
  Bool_t    GetNuRPair(TObject** Obj1,TObject** Obj2);     // Not Used Random Pair i<>j
  Bool_t    GetNuRPairP1P1(TObject** Obj1,TObject** Obj2); // Not Used Random Pair i<>j
  Bool_t    GetNuRPairP2P2(TObject** Obj1,TObject** Obj2); // Not Used Random Pair i<>j
  Bool_t    GetNuRPairP1P2(TObject** Obj1,TObject** Obj2); // Not Used Random Pair i<>=j
//UNIQUE
  Bool_t    GetUniquePair(TObject** Obj1,TObject** Obj2);     
  Bool_t    GetUniquePairP1P1(TObject** Obj1,TObject** Obj2);
  Bool_t    GetUniquePairP2P2(TObject** Obj1,TObject** Obj2);
  Bool_t    GetUniquePairP1P2(TObject** Obj1,TObject** Obj2);
//UNIQUE-RANDOM 
  Bool_t    GetRUniquePair(TObject** Obj1,TObject** Obj2);    
  Bool_t    GetRUniquePairP1P1(TObject** Obj1,TObject** Obj2);
  Bool_t    GetRUniquePairP2P2(TObject** Obj1,TObject** Obj2);
  Bool_t    GetRUniquePairP1P2(TObject** Obj1,TObject** Obj2);
//-------------------------?USED-SETUSED-SETNUSED-PAIR----------------//
  Bool_t    UsedPairP1P1(Int_t idx1, Int_t idx2);
  Bool_t    UsedPairP2P2(Int_t idx1, Int_t idx2);
  Bool_t    UsedPairP1P2(Int_t idx1, Int_t idx2);

  void      SetUsedPairP1P1(Int_t idx1, Int_t idx2);
  void      SetUsedPairP2P2(Int_t idx1, Int_t idx2);
  void      SetUsedPairP1P2(Int_t idx1, Int_t idx2);

  void      SetNUsedPairP1P1(Int_t idx1, Int_t idx2);
  void      SetNUsedPairP2P2(Int_t idx1, Int_t idx2);
  void      SetNUsedPairP1P2(Int_t idx1, Int_t idx2);
//------------------------NUMBER-OF-PAIRS-----------------------------//
  Int_t     NofPairs();                 // number of non-identical pairs (P1P1)
  Int_t     NofUsedPairs();             // number of Used  pairs (P1P1) 
  Int_t     NofFreePairs();             // number of Unused  pairs (P1P1)

  Int_t     NofPairsP1P1();             // number of non-identical P1P1 pairs
  Int_t     NofUsedPairsP1P1();         // number of Used P1P1 pairs
  Int_t     NofFreePairsP1P1();         // number of Unused P1P1 pairs

  Int_t     NofPairsP2P2();             // number of non-identical P2P2 pairs
  Int_t     NofUsedPairsP2P2();         // number of Used P2P2 pairs
  Int_t     NofFreePairsP2P2();         // number of Unused P2P2 pairs

  Int_t     NofPairsP1P2();             // number of P1P2 pairs
  Int_t     NofUsedPairsP1P2();         // number of Used P1P2 pairs
  Int_t     NofFreePairsP1P2();         // number of Unused P1P2 pairs
//UNIQUE//
  Int_t     NofUniquePairs();           // number of Unique pairs (P1P1)
  Int_t     NofUsedUniquePairs();       // number of Used Unique pairs (P1P1)
  Int_t     NofFreeUniquePairs();       // number of Unused Unique pairs (P1P1)

  Int_t     NofUniquePairsP1P1();       // number of Unique P1P1 pairs
  Int_t     NofUsedUniquePairsP1P1();   // number of Used Unique P1P1 pairs
  Int_t     NofFreeUniquePairsP1P1();   // number of Unused Unique P1P1 pairs

  Int_t     NofUniquePairsP2P2();       // number of Unique P2P2 pairs
  Int_t     NofUsedUniquePairsP2P2();   // number of Used Unique P2P2 pairs
  Int_t     NofFreeUniquePairsP2P2();   // number of Unused Unique P2P2 pairs

  Int_t     NofUniquePairsP1P2();       // number of Unique P1P2 pairs
  Int_t     NofUsedUniquePairsP1P2();   // number of Used Unique P1P2 pairs
  Int_t     NofFreeUniquePairsP1P2();   // number of Unused Unique P1P2 pairs

//--------------------------------GRAPHICS INTERFACE MODULE------------------------------//
  void      Show();
  void      ShowP1();
  void      ShowP2();
  void      ShowP1P1();
  void      ShowP2P2();
  void      ShowP1P2();
  void      ShowUP1P1();
  void      ShowUP2P2();
  void      ShowUP1P2();
  void      ExitShow();
// internal GUI routines //
  void      InitStyle();
  void      Copyright(Int_t opt = 0); // opt = 1 for Exit button 
  void      CopyClose();
  void      CloseWindows();
//------------------------------------ASCII STATUS--------------------------------------//
  void      DumpP1P1();
  void      DumpP2P2();
  void      DumpP1P1P2P2();
  void      DumpP1P2();

//-----------------------------------LAST ACTION MODULE---------------------------------//
  Int_t     GetLACode() { return fLAC; };               // Get Last Action Code
  void      ClrLACode() { fLAC = 0; };                  // Clear Las Action Code
  Int_t     GetLastP1Call() { return fLAC1; }           // Last Pool1 call
  Int_t     GetLastP2Call() { return fLAC2; }           // Last Pool2 call
  Int_t     GetLACPair()    { return fLACp; }           // Last GetPair call
  Int_t     GetLACUniPair() { return fLACu; }           // Last Unique Pair call
  Char_t*   GetLACName(Int_t LA);                       // Get name of LA code
  Char_t*   GetLAname();                                // Get Last Action name
  Bool_t    GetLastUniP1P1(Int_t &Idx1,Int_t &Idx2);    // Idx1, Idx2 of Last Unique P1P1
  Bool_t    GetLastUniP2P2(Int_t &Idx1,Int_t &Idx2);    // Idx1, Idx2 of Last Unique P2P2
  Bool_t    GetLastUniP1P2(Int_t &Idx1,Int_t &Idx2);    // Idx1, Idx2 of Last Unique P1P2
  Bool_t    GetLastP1P1(Int_t &Idx1,Int_t &Idx2);       // Idx1, Idx2 of Last P1P1
  Bool_t    GetLastP2P2(Int_t &Idx1,Int_t &Idx2);       // Idx1, Idx2 of Last P2P2
  Bool_t    GetLastP1P2(Int_t &Idx1,Int_t &Idx2);       // Idx1, Idx2 of Last P1P2

//----------------------BACKWARD COMPATIBILITY MODULE---(for T49Mixing class)----//
  Bool_t    IsReady() { return IsFull(); }
  Bool_t    CH1(Int_t i, Int_t j) { return (CheckBound1(i) && CheckBound1(j)); }
  Bool_t    CH2(Int_t i, Int_t j) { return (CheckBound2(i) && CheckBound2(j)); }
  Bool_t    CH3(Int_t i, Int_t j) { return (CheckBound1(i) && CheckBound2(j)); }
  void      AddEvent(TObject* Obj) { PutToP1(Obj); }      // PUT  = Add or Replace
  void      AddEvent(TObject* Obj1,TObject* Obj2){PutToP1(Obj1);PutToP2(Obj2);}
  void      GetEvent() { GetEvent(-1,-1); }     // Get random Obj. pair from Pool1 + Mark
  void      GetEvent(int i,int j) { if(CH1(i,j)) { I1=i; I2=j; } else GetRPair(&O1,&O2);}
  void      GetEventX() { GetEventX(-1,-1); }   // Get random Obj. pair from Pool1 - Mark
  void      GetEventX(int i,int j){if(CH1(i,j)){I1=i;I2=j;} else GetRPairP1P1x(&O1,&O2);}
  TObject*  GetPart1List() { return GetFromP1At(I1); }    // get Obj1* of P1P1
  TObject*  GetPart2List() { return GetFromP1At(I2); }    // get Obj2* of P1P1
  void      GetEventP1() { GetEventP1(-1,-1); }
  void      GetEventP1(Int_t i, Int_t j) { GetEvent(i,j); }
  void      GetEventP1X() { GetEventP1X(-1,-1); }
  void      GetEventP1X(Int_t i, Int_t j) { GetEventX(i,j); }
  TObject*  GetPart1ListP1() { return GetPart1List(); }   // P1 is default
  TObject*  GetPart2ListP1() { return GetPart2List(); }   // P1 is default
  void      GetEventP2() { GetEventP2(-1,-1); }
  void      GetEventP2X() { GetEventP2X(-1,-1); }
  void      GetEventP2(int i,int j){if(CH2(i,j)){I1=i;I2=j;} else GetRPairP2P2(&O1,&O2);}
  void      GetEventP2X(int i,int j){if(CH2(i,j)){I1=i;I2=j;} else GetRPairP2P2x(&O1,&O2);}
  TObject*  GetPart1ListP2() { return GetFromP2At(I1); }  // get Obj1* of P2P2
  TObject*  GetPart2ListP2() { return GetFromP2At(I2); }  // get Obj2* of P2P2
  void      GetEventP12() { GetEventP12(-1,-1); }
  void      GetEventP12X() { GetEventP12X(-1,-1); }
  void      GetEventP12(int i,int j){if(CH3(i,j)){I1=i;I2=j;}else GetRPairP1P2(&O1,&O2);}
  void      GetEventP12X(int i,int j){if(CH3(i,j)){I1=i;I2=j;}else GetRPairP1P2x(&O1,&O2);}
  TObject*  GetPart1ListP12() { return GetFromP1At(I1); } // get Obj1* of P1P2
  TObject*  GetPart2ListP12() { return GetFromP2At(I2); } // get Obj2* of P1P2
//----------------------------END OF B.C.M.---------------(for T49Mixing)---------//
  
  ClassDef(T49Mixer,1)  // New universal ROOT49 mixer 

}; 

R__EXTERN TObjArray *gT49Mixer;

#endif

