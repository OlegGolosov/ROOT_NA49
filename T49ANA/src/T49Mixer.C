///////////////////////////////////////////////
// *************************
// *  T49MiXeR CLASS V1.0  *
// *************************
// 
// T49Mixer Class is a software tool designed for the Event mixing technique.
// It can generate various types of Object pairs e.g.:
//   1) Random Pair types
//   2) Unique Pair types
// T49Mixer has two Pools (P1 and P2) where objects can be stored.
// Therefore it can generate P1P1, P2P2 and P1P2 pairs.
// In the case of Unique pairs each object is used only once for pair generation.
// T49Mixer can generate also Random Unique Pairs - see e.g. GetRUniquePairP1P1()
//
// Method PutToP1 (the same for P2) stores your objects into the Mixer's Pool 
// via Object->Clone() method. This creates a copy of the object in the Pool.
// If Pool is full, old objects are replacedi (one after another).
// While replacing old object X a simple "delete X" call is used to remove
// old object. Make sure X->Clone() and "delete X" works well for your X !
// (For TObjArray type "delete X" works well if X->SetOwner(1) is used.)
//
// If your objects are not Clonable you can still mix them in T49Mixer:
// Prepare TObjArray* W of pointers to your objects and use SetPool1(W) method.
// In this case you take responsibility for the storage of objects being mixed. 
//  
// Pools of T49Mixer do not have to be full to generate pairs. You can ask for
// the pair any time. For number of pairs in T49Mixer call e.g. NofPairsP1P2().
// 
// T49Mixer remembers which pairs have been generated. 
// Therefore you can ask for not used pairs of a given type e.g.:
//   - GetNuPairP1P2   (first Not used  P1P2 pair )
//   - GetNuRPairP1P2  (random Not used P1P2 pair - requires more CPU time)
//
// T49Mixer has its own Graphical Interface Show() which shows current
// status of the Mixer graphically: 
//   - Number of objects in the pool  ShowP1() 
//   - Number of generated pairs      ShowP1P1()
//   - Status of Unique pairs         ShowUP1P1() 
//
// Typical usage of T49Mixer can look as follows:
//    1) declaration e.g.    T49Mixer* MX = new T49Mixer(100,1030);
//    2) filling of Pool(s)  MX->PutToP1(Obj1) or MX->SetPool1(ObjArray)
//    3) generating pairs    MX->GetRPairP1P1(&Obj1,&Obj2);
//    4) emptying the pools  MX->PurgePool1()  or MX->ClearPool1()
//    5) another filling - 2) and generating pairs - 3)
//    6) deleting mixer if created on Heap (as shown here)
//
// Do not forget: GetRPairP1P1(&Obj1,&Obj2) can repeat same pairs.
//
// Enjoy T49Mixer ! 
// 
// Designed at Begin_Html <a href="http://www.mppmu.mpg.de/">MPI</a>End_Html 19.11.1999. 
// Send comments to Begin_Html <a href="mailto:pfilips.mppmu.mpg.de">PFILIPS</a> End_Html. 
// 
/////////////////////////////////////////////// 

#include "T49Mixer.h"

TObjArray *gT49Mixer = 0;

ClassImp(T49Mixer)

/*
$Log: T49Mixer.C,v $
Revision 1.6  2005/02/08 12:45:26  flierl
Replaced uncorrect assignment of arrays

Revision 1.5  2001/06/13 08:16:20  cblume
Update from Peter Filip

Revision 1.4  2000/02/23 09:42:07  cblume
Update by P.Filip: Speed optimization and Documentation

Revision 1.3  1999/12/20 17:18:33  cblume
Upgrade by P. Filip

 * Revision 1.2  1999/11/24  16:03:46  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 * Revision 1.1  1999/11/23  13:54:39  cblume
 * Add T49Mixer.C
 *

*/

T49Mixer::T49Mixer(Int_t n)
{ if (n<2) n = 2; Init101();
//
//  Constructor to initialize the mixer with 1 Pool.
//  n - number of objects to be stored (default 100)
//  Declare as: T49Mixer TX(1700);  or  T49Mixer TX;
//  Heap declaration: T49Mixer* TX = new T49Mixer(); 
//
  if(gT49Mixer==0) { gT49Mixer = new TObjArray(14,1); }
  fID = gT49Mixer->AddAtFree(this); 
  cout << ".T49MiXeR(" << n << ") ID:" << fID << endl;
  fN1MAX=n; fIdx1MAX=n-1;
  fPool1 = new TObjArray(n);
  fUsed1 = new TArrayC(n); 
  fUniq11= new TArrayI(n); nUsedUniP1P1 = 0;
  fMaskXX = new TArrayI(n*(1+n/32));
  fMaxIdx1 = 0;
  nMaxMXX = n;
  nObjects1=0;
  fP1Full=0;
  fP1Rx = 0;
  nPools = 1;
  fVerbose=0;
  fSeeFlag=1;
  AllUsedResetP1();
  InitRnd(); 
  InitStyle();
}

T49Mixer::T49Mixer(Int_t n1, Int_t n2)
{ if(n1<2)n1=2;if(n2<2)n2=2; Init101();
//
//  Constructor to initialize the mixer with 2 Pools
//  n1, n2 - number of objects to be stored in Pools
//  Declare: T49Mixer DX(100,200); (on Data Segment)
//  or: T49Mixer* HX = new T49Mixer(500,700); (Heap)
//
  if(gT49Mixer==0) { gT49Mixer = new TObjArray(14,1); }
  fID = gT49Mixer->AddAtFree(this); 
  cout << ".T49MiXeR(" << n1 << "," << n2 << ") ID:" << fID << endl; 
  fN1MAX=n1; fN2MAX=n2; fIdx1MAX=n1-1; fIdx2MAX=n2-1;
  fPool1 = new TObjArray(n1); fPool2 = new TObjArray(n2);
  fUsed1 = new TArrayC(n1);   fUsed2 = new TArrayC(n2);
  fUniq11= new TArrayI(n1);   fUniq22= new TArrayI(n2);
  nUsedUniP1P1 = 0;           nUsedUniP2P2 = 0;
  fUniq12= new TArrayI(n1);   fUniq21= new TArrayI(n2);
  nUsedUniP1P2 = 0;           nUsedUniP2P1 = 0;
  if (n1>=n2) { fMaskXX = new TArrayI(n1*(1+n1/32)); nMaxMXX = n1; } 
         else { fMaskXX = new TArrayI(n2*(1+n2/32)); nMaxMXX = n2; }
  if (n1>=n2) { fMaskXY = new TArrayI(n1*(1+n1/32)); nMaxMXY = n1;}
       else   { fMaskXY = new TArrayI(n2*(1+n2/32)); nMaxMXY = n2;}
  fMaxIdx1=0;    fMaxIdx2=0;
  nObjects1=0;   nObjects2=0;
  fP1Full=0; fP2Full=0;
  fP1Rx = 0; fP2Rx = 0;
  nPools = 2; 
  fVerbose=0;
  fSeeFlag=1;
  AllUsedReset();
  InitRnd(); 
  InitStyle();
}

T49Mixer::~T49Mixer()
{
// T49Mixer Destructor  
  if(fVerbose) cout << "T49MiXeR destructor active:" << endl;
  ExitShow();  
  if (nPools == 1)
  { if(fVerbose) cout << "Deleting Pool1 ..." << endl;
    PurgePool1();
    delete fPool1;
    delete fUsed1;
    delete fUniq11;
    delete fMaskXX;
  }
  if (nPools == 2)
  { if(fVerbose) cout << "Deleting Pool1 & Pool2 ..." << endl;
    PurgePool1();   PurgePool2();
    delete fPool1;  delete fPool2;
    delete fUsed1;  delete fUsed2;
    delete fUniq11; delete fUniq22;
    delete fUniq12; delete fUniq21;
    delete fMaskXX; delete fMaskXY;
  }
  (*gT49Mixer)[fID] = 0;
  if (fVerbose) { cout << "T49MiXeR ID was:" << fID << endl; }
  fID = gT49Mixer->GetEntries();
  if (fVerbose) { cout << "T49MiXeR remain:" << fID << endl; }
  if(fID == 0) { delete gT49Mixer; gT49Mixer = 0; }
  gStyle = (TStyle*)gROOT->GetListOfStyles()->Last();
  if(fVerbose) cout << "T49MiXeR destructor finished." << endl;
}
//////////////////////////////////////////////// MIXER Methods
Bool_t T49Mixer::IsFull()
{ return PoolsFull(); }
// Return "true" if Pools of mixer are full
// Identical to PoolsFull()
void  T49Mixer::PutToMixer(TObject *Obj1, TObject *Obj2)
{ PutToP1P2(Obj1,Obj2); }
// Put Obj1 to Pool1 and Obj2 to Pool2
// Identical to PutToP1P2(Obj1,Obj2)
Int_t  T49Mixer::PutOne(TObject *Obj)
{ return PutToP1(Obj); }
// Put object Obj into Pool1 of Mixer
// Identical to PutToP1(Obj)
Int_t  T49Mixer::NofObjects()    
{ return NofObjectsP1(); }
// Get number of objects in Pool1
// Identical to NofObjectsP1()
Int_t  T49Mixer::NofUsed()   
{ return NofUsedP1(); }
// Get number of used objects in Pool1
// Identical to NofUsedP1()
Int_t  T49Mixer::NofFree()   
{ return NofFreeP1(); }
// Get number of not used objects in Pool1
// Identical to NofFreeP1()
TObject*  T49Mixer::GetOne()
{ return GetFromP1(); }
// Get pointer to random object in Pool1 of Mixer
// Identical to GetFromP1()
TObject*  T49Mixer::GetOneAt(Int_t idx)
{ return GetFromP1At(idx); }
// Get pointer to Object in Pool1 of Mixer at position idx
// Identical to GetFromP1At(idx)
TObject*  T49Mixer::GetNuOne()
{ return GetNuOneP1(); }
// Get pointer to Not Used object in Pool1 of Mixer
// Identical to GetNuOneP1()
TObject*  T49Mixer::GetNuROne()
{ return GetNuROneP1(); }
// Get pointer to Not Used Random object in Pool1 of Mixer
// Identical to GetNuROneP1()

Int_t  T49Mixer::NofPairs() 
{ return NofPairsP1P1(); }
// Get number of P1P1 pairs of objects in Pool1 
// Identical to NofPairsP1P1()
Int_t  T49Mixer::NofUsedPairs() 
{ return NofUsedPairsP1P1(); }
// Get number of Used P1P1 pairs of objects in Pool1
// Identical to NofUsedPairsP1P1()
Int_t  T49Mixer::NofFreePairs() 
{ return NofFreePairsP1P1(); }
// Get number of not used P1P1 pairs of objects in Pool1
// Identical to NofFreePairsP1P1()

Bool_t    T49Mixer::GetPairAt(TObject** Obj1, TObject** Obj2, Int_t idx1, Int_t idx2)
{ return GetPairP1P1At(Obj1,Obj2,idx1,idx2); }
// Set pointers Obj1, Obj2 to pair of objects in Pool1 at positions idx1, idx2
// Identical to GetPairP1P1At(Obj1,Obj2,idx1,idx2)
Bool_t    T49Mixer::GetRPair(TObject** Obj1,TObject** Obj2)
{ return GetRPairP1P1(Obj1,Obj2); }
// Set pointers Obj1, Obj2 to pair of objects in Pool1 at random and different positions idx1, idx2 
// Identical to GetRPairP1P1(Obj1,Obj2)
Bool_t    T49Mixer::GetNuPair(TObject** Obj1,TObject** Obj2)
{ return GetNuPairP1P1(Obj1,Obj2); }
// Set pointers Obj1, Obj2 to not used pair of objects in Pool1 
// Identical to GetNuPairP1P1(Obj1,Obj2)
Bool_t    T49Mixer::GetNuRPair(TObject** Obj1,TObject** Obj2)
{ return GetNuRPairP1P1(Obj1,Obj2); }
// Set pointers Obj1, Obj2 to random not used pair of objects in Pool1
// Identical to GetNuRPairP1P1(Obj1,Obj2)

Int_t  T49Mixer::NofUniquePairs() 
{ return NofUniquePairsP1P1(); }
// Get number of Unique P1P1 pairs of objects in Pool1
// Identical to NofUniquePairsP1P1()
Int_t  T49Mixer::NofUsedUniquePairs() 
{ return NofUsedUniquePairsP1P1(); }
// Get number of Used Unique P1P1 pairs of objects in Pool1
// Identical to NofUsedUniquePairsP1P1()
Int_t  T49Mixer::NofFreeUniquePairs() 
{ return NofFreeUniquePairsP1P1(); }
// Get number of not used Unique P1P1 pairs of objects in Pool1 
// Identical to NofFreeUniquePairsP1P1()

Bool_t    T49Mixer::GetUniquePair(TObject** Obj1,TObject** Obj2)
{ return GetUniquePairP1P1(Obj1,Obj2); }
// Set pointers Obj1, Obj2 to Unique pair of objects in Pool1
// Identical to GetUniquePairP1P1(Obj1,Obj2)
Bool_t    T49Mixer::GetRUniquePair(TObject** Obj1,TObject** Obj2)
{ return GetRUniquePairP1P1(Obj1,Obj2); }
// Set pointers Obj1, Obj2 to Random Unique pair of objects in Pool1
// Identical to GetRUniquePairP1P1(Obj1,Obj2)
void  T49Mixer::PurgeMixer()
{ PurgePools(); }
// Delete objects in Pools of Mixer
// Identical to PurgePools()
void  T49Mixer::SetMixer(TObjArray *ObjA1, TObjArray *ObjA2)
{ SetPools(ObjA1,ObjA2); }
// Copy object pointers from TObjArrays into Pools
// Identical to SetPools(ObjA1,ObjA2) 
void  T49Mixer::ClearMixer()
{ ClearPools(); }
// Clear TObject pointers (=0) in Pools of Mixer
// Identical To ClearPools()
void  T49Mixer::ResetMixer()
{ AllUsedReset(); }
// Reset all information about usage of pairs and objects
// Content of Pools remains untouched
// Identical to AllUsedReset()
void  T49Mixer::ShowMixer()
{ Show(); }
// Show Graphic User Interface of Mixer
// Identical to Show()
void  T49Mixer::DumpMixer()
{ DumpP1P1(); if (nPools==2) { DumpP2P2(); DumpP1P2(); } }
// Print ASCII information about pairs in mixer
// Identical to DumpP1P1() and DumpP2P2() + DumpP1P2()

Char_t* T49Mixer::GetLAname()
{ return GetLACName(fLAC); }
// Return string name of last recorded action performed in Mixer
// Identical to GetLACName( GetLACode() )

//////////////////////////////////////////////// STATUS
Bool_t T49Mixer::PoolsFull()
{
// Return "true" if Pools of mixer are full
  if (nPools==1) {return fP1Full;}
  else  { return fP1Full*fP2Full;}
}
Bool_t T49Mixer::Pool1Full()
{
// Return "true" if Pool1 is full 
  return fP1Full; 
}
Bool_t T49Mixer::Pool2Full()
{ 
// Return "true" if Pool2 is full
  return fP2Full; 
}
//////////////////////////////////////////////// PURGE Pool
void  T49Mixer::PurgePools()
{ 
// Delete objects in Pool1 and also in Pool2 if exists 
  PurgePool1(); 
  if (nPools==2) PurgePool2();
}
void  T49Mixer::PurgePool1()
{
// Delete Objects in Pool1
  AllUsedResetP1();
  fLAC = fLAC1 = 54;
  if(fVerbose) cout << "Deleteing objects in Pool 1 ... " << endl;
  Int_t fVerboseStat = fVerbose;
  fVerbose = 0;
  TObject* a;
  for(int i=0;i<nObjects1;i++)
  {
    a = GetFromP1At(i);
    delete a;
  }
  nObjects1 = 0; fMaxIdx1 = 0;
  fP1Full = 0; fP1Rx = 0;
  fVerbose = fVerboseStat;
}
void  T49Mixer::PurgePool2()
{
// Delete Objects in Pool2
  AllUsedResetP2();
  fLAC = fLAC2 = 64;
  if(fVerbose) cout << "Deleteing objects in Pool 2 ... " << endl;
  Int_t fVerboseStat = fVerbose;
  fVerbose = 0;
  TObject* a;
  for(int i=0;i<nObjects2;i++)
  {
    a = GetFromP2At(i);
    delete a;
  }
  nObjects2 = 0; fMaxIdx2 = 0;
  fP2Full = 0; fP2Rx = 0;
  fVerbose = fVerboseStat;
}
void  T49Mixer::ClearPools()
{
// Set TObject pointers = 0 in Pool1 & Pool2
// Use after SetPools() otherwise heap leaks
// This does not reset Used/Notused info. 
  ClearPool1();
  if ( nPools==2 ) ClearPool2();
  fLAC = 77;
}
void  T49Mixer::ClearPool1()
{
// Set all Pool1 TObject pointers = 0
// Use after SetPool1(), otherwise heap leak
// This does not reset Used/Notused info.
  fLAC = fLAC1 = 55;
  fPool1->Clear();
  fP1Full = 0;
  fMaxIdx1 = 0;
  nObjects1 = 0;
}
void  T49Mixer::ClearPool2()
{
// Set all Pool2 TObject pointers = 0
// Use after SetPool2(), otherwise heap leak
// This does not reset Used/Notused info.
  fLAC = fLAC2 = 65;
  fPool2->Clear();
  fP2Full = 0;
  fMaxIdx2 = 0;
  nObjects2 = 0;
}
///////////////////////////////////////////////////////// GUI 
void  T49Mixer::Show()
{
// T49Mixer Graphic User Interface
// Shows status of Pools and generated pairs
  TS.cd();                       // set style
  TText T1;                      // text variable
  Int_t Cb, Ct;                  // color of button & text
  //Char_t nCm[100] = "";          // hidden button command
  Int_t hCb = 36; Int_t hCt = 34;// colors of hidden button & text
//WINDOW
  sprintf(fIDS,"%s%d","ROOT-G-MX",fID);
  cGIF = new TDialogCanvas(fIDS,fIDS,200,300);
  cGIF->SetBorderSize(3);
//NAME
  sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->Copyright(0)");
  TButton *BUTTON = new TButton("T49MiXeR V1.0",fIDS,0.0,0.87,0.98,1.0);
  BUTTON->SetTextColor(2); BUTTON->SetFillStyle(4000); BUTTON->Draw();
//POOL1
  Cb=15; Ct=1;
  sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->ShowP1()");
  TButton *but1 = new TButton("POOL 1",fIDS,.05,.75,.49,.85);
  but1->SetTextColor(Ct); but1->SetFillColor(Cb); but1->Draw();
//POOL2
  Cb=15; Ct=1;
  sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->ShowP2()");
  if (nPools==1) { sprintf(fIDS,"This should be empty"); Cb=hCb; Ct=hCt; }
  TButton *but2 = new TButton("POOL 2",fIDS,.51,.75,.95,.85);
  but2->SetTextColor(Ct); but2->SetFillColor(Cb); but2->Draw();
//PAIRS
  T1.SetTextColor(1); T1.SetTextSize(0.077); T1.DrawText(0.39,0.55,"PAIRS");
//P1P1
  Cb=11; Ct=1;
  sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->ShowP1P1()");
  TButton *but11 = new TButton("P1P1",fIDS,0.09,.61,.49,.69);
  but11->SetTextColor(Ct); but11->SetFillColor(Cb); but11->Draw();
//P2P2
  Cb=11; Ct=1;
  sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->ShowP2P2()");
  if (nPools==1) { sprintf(fIDS,"This should be empty"); Cb = hCb; Ct = hCt; }
  TButton *but22 = new TButton("P2P2",fIDS,0.51,.61,.91,.69);
  but22->SetTextColor(Ct); but22->SetFillColor(Cb); but22->Draw();
//P1P2
  Cb=11; Ct=1;
  sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->ShowP1P2()");
  if (nPools==1) { sprintf(fIDS,"This should be empty"); Cb = hCb; Ct = hCt; }
  TButton *but12 = new TButton("P1P2",fIDS,.25,.45,.75,.53);
  but12->SetTextColor(Ct); but12->SetFillColor(Cb); but12->Draw();
//UPAIRS
  T1.SetTextColor(1); T1.SetTextSize(0.077); T1.DrawText(0.235,0.25,"UNIQUE PAIRS");
//UP1P1
  Cb=17; Ct=1;
  sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->ShowUP1P1()");
  TButton *butU11 = new TButton("UP1P1",fIDS,0.09,.31,.49,.39);
  butU11->SetTextColor(Ct); butU11->SetFillColor(Cb); butU11->Draw();
//UP2P2
  Cb=17; Ct=1;
  sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->ShowUP2P2()");
  if (nPools==1) { sprintf(fIDS,"This should be empty"); Cb = hCb; Ct = hCt; }
  TButton *butU22 = new TButton("UP2P2",fIDS,0.51,.31,.91,.39);
  butU22->SetTextColor(Ct); butU22->SetFillColor(Cb); butU22->Draw();
//UP1P2
  Cb=17; Ct=1;
  sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->ShowUP1P2()");
  if (nPools==1) { sprintf(fIDS,"This should be empty"); Cb = hCb; Ct = hCt; }
  TButton *butU12 = new TButton("UP1P2",fIDS,.25,.15,.75,.23);
  butU12->SetTextColor(Ct); butU12->SetFillColor(Cb); butU12->Draw();
//COPYR
  char TC[14]; ULong_t* W=(ULong_t*)&TC[0];
  W[0]=1179656361;  W[1]=1346980937; W[2]=548282451; W[3]=0;
  sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->Copyright(1)");
  TButton *button = new TButton(TC,fIDS,0.04,0.02,0.96,0.12);
  button->SetTextColor(2); button->SetFillColor(15);
  button->SetFillStyle(4000); button->Draw();
  cGIF->Update();
}
void   T49Mixer::ShowP1()
{
// Show Pool1 status
  if (cP1!=0) { delete cP1; cP1=0; } else {
  TLine LP1, LP1R; TGaxis AxP1; TText T1; TArrow PA; TS.cd();
  Char_t Inf[100];
  sprintf(fIDS,"%s MX%d","POOL1",fID);
  cP1=new TDialogCanvas(fIDS,fIDS,444,144);
  cP1->cd(); cP1->Range(0,0,120,2);
  T1.SetTextColor(1); T1.SetTextSize(0.14);
   sprintf(Inf,"%s%d","Objects: ",nObjects1);
  T1.DrawText(10-3,1.6,Inf);
   sprintf(Inf,"%s%d ","Max: ",fN1MAX);
    T1.SetTextAlign(21); T1.DrawText(60,1.6,Inf);
   sprintf(Inf,"%s%d ","Next Put: ",nObjects1+1);
  Float_t ax = 10+nObjects1*100/fN1MAX;
  LP1.SetLineColor(3); LP1.SetLineWidth(7);
  LP1.DrawLine(10,1.04,ax,1.04);
  ax = 10+(nObjects1+0.5)*100/fN1MAX;
  if(nObjects1==fN1MAX) {
    ax=10+(fP1Rx)*100/fN1MAX;
    LP1R.SetLineColor(2); LP1R.SetLineWidth(3);
    LP1R.DrawLine(10,1.07,ax,1.07);
    ax=10+(fP1Rx+0.5)*100/fN1MAX;
    sprintf(Inf,"%s%d ","Next Put: ",fP1Rx+1);
  }
  PA.SetLineColor(1); PA.DrawArrow(ax,1.5,ax,1.08,0.07,"|>");
  T1.SetTextColor(1); T1.SetTextAlign(31);
  T1.DrawText(114,1.6,Inf); T1.SetTextAlign(11);
  T1.SetTextSize(0.11); T1.DrawText(ax+1,1.34,"Put");
  AxP1.SetLabelSize(0.14); AxP1.SetLineColor(1); AxP1.SetLineWidth(1);
  AxP1.DrawAxis(10,1.0,110,1.0,0,fN1MAX,510,"");

  sprintf(Inf,"%s%s","Last change: ",GetLACName(GetLastP1Call()));
  T1.SetTextColor(1); T1.SetTextSize(0.14); T1.SetTextAlign(21);
  T1.DrawText(60,0.2,Inf); cP1->Update();
  }
}
void   T49Mixer::ShowP2()
{
// Show Pool2 status
  if (cP2!=0) { delete cP2; cP2=0; } else {
    TLine LP2, LP2R; TGaxis AxP2; TText T2; TArrow PA; TS.cd();
    Char_t Inf[100];
    sprintf(fIDS,"%s MX%d","POOL2",fID);
    cP2=new TDialogCanvas(fIDS,fIDS,444,144);
    cP2->cd(); cP2->Range(0,0,120,2);
    T2.SetTextColor(1); T2.SetTextSize(0.14);
     sprintf(Inf,"%s%d","Objects: ",nObjects2);
    T2.DrawText(10-3,1.6,Inf);
     sprintf(Inf,"%s%d ","Max: ",fN2MAX);
    T2.SetTextAlign(21); T2.DrawText(60,1.6,Inf);
     sprintf(Inf,"%s%d ","Next Put: ",nObjects2+1);
    T2.SetTextAlign(11);
    Float_t ax = 10+nObjects2*100/fN2MAX;
    LP2.SetLineColor(3); LP2.SetLineWidth(7);
    LP2.DrawLine(10,1.04,ax,1.04);
    ax = 10+(nObjects2+0.5)*100/fN2MAX;
    if(nObjects2==fN2MAX) {
      ax=10+(fP2Rx)*100/fN2MAX;
      LP2R.SetLineColor(2); LP2R.SetLineWidth(3);
      LP2R.DrawLine(10,1.07,ax,1.07);
      ax=10+(fP2Rx+0.5)*100/fN2MAX;
      sprintf(Inf,"%s%d ","Next Put: ",fP2Rx+1);
    }
    PA.SetLineColor(1); PA.DrawArrow(ax,1.5,ax,1.08,0.07,"|>");
    T2.SetTextColor(1); T2.SetTextAlign(31);
    T2.DrawText(114,1.6,Inf); T2.SetTextAlign(11);
    T2.SetTextSize(0.11); T2.DrawText(ax+1,1.34,"Put");
    AxP2.SetLabelSize(0.14); AxP2.SetLineColor(1); AxP2.SetLineWidth(1);
    AxP2.DrawAxis(10,1.0,110,1.0,0,fN2MAX,510,"");

    sprintf(Inf,"%s%s","Last change: ",GetLACName(GetLastP2Call()));
    T2.SetTextColor(1); T2.SetTextSize(0.14); T2.SetTextAlign(21);
    T2.DrawText(60,0.2,Inf); cP2->Update();
  };
}
void   T49Mixer::ShowP1P1()
{ 
// Show P1P1 pairs status 
  if (cXX!=0) { delete cXX; cXX=0; } else {
  Char_t Inf[100];  TS.cd();
  if(HistXX!=NULL) delete HistXX; 
    sprintf(fIDS,"%s MX%d","P1P1 Pairs",fID);
    HistXX=new TH2C(fIDS,fIDS,
                     fMaxIdx1+1,0,fMaxIdx1+1,fMaxIdx1+1,0,fMaxIdx1+1); 
  Int_t status, sx; if (fMaxIdx1) sx = 16; else sx=0;
  for(int i=0;i<=fMaxIdx1;i++) { 
   for(int j=0;j<=fMaxIdx1;j++) {
     status = (i!=j)? 100*(int)UsedPairP1P1(i,j) : sx;
     HistXX->Fill(i+0.5,j+0.5,status);
   } }
  sprintf(fIDS,"%s MX%d","Used P1P1 Pairs",fID);
  cXX=new TDialogCanvas(fIDS,fIDS,333,333);
  cXX->cd();
  TS.SetOptStat(0); TS.SetOptTitle(0);
  HistXX->Draw("COL");
  Int_t NP=NofPairsP1P1();
  Int_t UP=NofUsedPairsP1P1();
  Int_t FP=NofFreePairsP1P1();
  sprintf(Inf,"%s%d%s%d%s%d%s"," Total:",NP,"   Used:",UP,"   Free:",FP," ");
  TButton *BUTTON = new TButton(Inf,"",0.01,0.91,0.99,0.99);
  BUTTON->SetTextColor(3); BUTTON->SetFillStyle(4000); 
  BUTTON->Draw(); cXX->Update();
  }
}
void   T49Mixer::ShowP2P2()
{ 
// Show P2P2 pairs status 
  if (cYY!=0) { delete cYY; cYY=0; } else {
  Char_t Inf[100]; TS.cd();
  if(HistYY!=NULL) delete HistYY; 
    sprintf(fIDS,"%s MX%d","P2P2 Pairs",fID);
    HistYY=new TH2C(fIDS,fIDS,
                     fMaxIdx2+1,0,fMaxIdx2+1,fMaxIdx2+1,0,fMaxIdx2+1);
  Int_t status,sx; if (fMaxIdx2) sx = 16; else sx = 0;
  for(int i=0;i<=fMaxIdx2;i++) {
   for(int j=0;j<=fMaxIdx2;j++) {
     status = (i!=j)? 100*(int)UsedPairP2P2(i,j) : sx;
     HistYY->Fill(i+0.5,j+0.5,status);
   } }
  sprintf(fIDS,"%s MX%d","Used P2P2 Pairs",fID);
  cYY=new TDialogCanvas(fIDS,fIDS,333,333);
  cYY->cd();
  TS.SetOptStat(0); TS.SetOptTitle(0);
  HistYY->Draw("COL");
  Int_t NP=NofPairsP2P2();
  Int_t UP=NofUsedPairsP2P2();
  Int_t FP=NofFreePairsP2P2();
  sprintf(Inf,"%s%d%s%d%s%d%s"," Total:",NP,"   Used:",UP,"   Free:",FP," ");
  TButton *BUTTON = new TButton(Inf,"",0.01,0.91,0.99,0.99);
  BUTTON->SetTextColor(3); BUTTON->SetFillStyle(4000);
  BUTTON->Draw(); cYY->Update();
  }
}
void   T49Mixer::ShowP1P2()
{ 
// Show P1P2 pairs status 
  if (cXY!=0) { delete cXY; cXY=0; } else {
  Char_t Inf[100];  TS.cd();
  if(HistXY!=NULL) delete HistXY;
    sprintf(fIDS,"%s MX%d","P1P2 Pairs",fID);
    HistXY=new TH2C(fIDS,fIDS,
                fMaxIdx1+1,0,fMaxIdx1+1,fMaxIdx2+1,0,fMaxIdx2+1);
  Int_t status;
  for(int i=0;i<=fMaxIdx1;i++) {
   for(int j=0;j<=fMaxIdx2;j++) {
     status = 100*(int)UsedPairP1P2(i,j); 
     if(i==j && GetP1P2Mode()) status = 16;
     HistXY->Fill(i+0.5,j+0.5,status);
   } }
  sprintf(fIDS,"%s MX%d","Used P1P2 Pairs",fID);
  cXY=new TDialogCanvas(fIDS,fIDS,333,333);
  cXY->cd(); 
  TS.SetOptStat(0); TS.SetOptTitle(0); 
  HistXY->Draw("COL"); 
  Int_t NP=NofPairsP1P2();
  Int_t UP=NofUsedPairsP1P2();
  Int_t FP=NofFreePairsP1P2();
  sprintf(Inf,"%s%d%s%d%s%d%s"," Total:",NP,"   Used:",UP,"   Free:",FP," ");
  TButton *BUTTON = new TButton(Inf,"",0.01,0.91,0.99,0.99);
  BUTTON->SetTextColor(3); BUTTON->SetFillStyle(4000);
  BUTTON->Draw();
  sprintf(Inf,"%s%d"," P1P2 MODE: ",GetP1P2Mode());
  TButton *BUTT = new TButton(Inf,"",0.91,0.1,0.99,0.9);
  BUTT->SetTextColor(1); BUTT->SetFillStyle(4000); BUTT->SetTextAngle(270);
  BUTT->Draw(); cXY->Update();
  }
}
void   T49Mixer::ShowUP1P1()
{ 
// Show UniqueP1P1 pairs status   
  if (cUP1P1!=0) { delete cUP1P1; cUP1P1=0; } else {
  TLine LP1; TGaxis AxP1; TText T1; Char_t Inf[100]; TS.cd();
  sprintf(fIDS,"%s MX%d","Unique P1P1 Pairs",fID);
  cUP1P1=new TDialogCanvas(fIDS,fIDS,444,144);
  cUP1P1->cd(); cUP1P1->Range(0,0,120,2); 
  T1.SetTextColor(1); T1.SetTextSize(0.14);
  T1.DrawText(111,0.4,"P1"); T1.DrawText(111,1.0,"P1");
  T1.SetTextColor(7); T1.SetTextSize(0.14);
  Int_t NP=NofUniquePairsP1P1();
  Int_t UP=NofUsedUniquePairsP1P1();
  Int_t FP=NofFreeUniquePairsP1P1();
  sprintf(Inf,"%s%d%s%d%s%d%s","Total:",NP,"   Used:",UP,"   Free:",FP,"   Dim: 1");  
  T1.DrawText(4,1.64,Inf); 
  LP1.SetLineColor(7);
  for(Int_t idx=0;idx<=fMaxIdx1;idx++) { 
   if (UsedUniqueP1(idx))
    LP1.DrawLine(5+(0.5+idx)*100/fN1MAX,1.1,5+(0.5+GetUsedUniqueIDXP1(idx))*100/fN1MAX,0.5);
  } 
  AxP1.SetLabelSize(0.14); AxP1.SetLineColor(1); AxP1.SetLineWidth(1);
  AxP1.DrawAxis(5,0.5,105,0.5,0,fN1MAX,510,"+");
  AxP1.SetLabelOffset(0.025);
  AxP1.DrawAxis(5,1.1,105,1.1,0,fN1MAX,510,"-");
  cUP1P1->Update();
  }
}

void   T49Mixer::ShowUP2P2()
{ 
// Show UniqueP2P2 pairs status 
  if (cUP2P2!=0) { delete cUP2P2; cUP2P2=0; } else {
  TLine LP1; TGaxis AxP1; TText T1; Char_t Inf[100]; TS.cd();
  sprintf(fIDS,"%s MX%d","Unique P2P2 Pairs",fID);
  cUP2P2=new TDialogCanvas(fIDS,fIDS,444,144);
  cUP2P2->cd(); cUP2P2->Range(0,0,120,2);
  T1.SetTextColor(1); T1.SetTextSize(0.14);
  T1.DrawText(111,1.0,"P2"); T1.DrawText(111,0.4,"P2");
  T1.SetTextColor(7); T1.SetTextSize(0.14);
  Int_t NP=NofUniquePairsP2P2();
  Int_t UP=NofUsedUniquePairsP2P2();
  Int_t FP=NofFreeUniquePairsP2P2();
  sprintf(Inf,"%s%d%s%d%s%d%s","Total:",NP,"   Used:",UP,"   Free:",FP,"   Dim: 1");
  T1.DrawText(4,1.64,Inf);
  LP1.SetLineColor(7);
  for(Int_t idx=0;idx<=fMaxIdx2;idx++) {
   if (UsedUniqueP2(idx)) 
    LP1.DrawLine(5+(0.5+idx)*100/fN2MAX,1.1,5+(0.5+GetUsedUniqueIDXP2(idx))*100/fN2MAX,0.5);
  }
  AxP1.SetLabelSize(0.14); AxP1.SetLineColor(1); AxP1.SetLineWidth(1);
  AxP1.DrawAxis(5,0.5,105,0.5,0,fN2MAX,510,"+");
  AxP1.SetLabelOffset(0.025);
  AxP1.DrawAxis(5,1.1,105,1.1,0,fN2MAX,510,"-");
  cUP2P2->Update();
  }
}

void   T49Mixer::ShowUP1P2()
{ 
// Show UniqueP1P2 pairs status
  if (cUP1P2!=0) { delete cUP1P2; cUP1P2=0; } else {
  TLine LP1; TGaxis AxP1; TText T1; Char_t Inf[100]; TS.cd();
  sprintf(fIDS,"%s MX%d","Unique P1P2 Pairs",fID);
  cUP1P2=new TDialogCanvas(fIDS,fIDS,444,144);
  cUP1P2->cd(); cUP1P2->Range(0,0,120,2);
  T1.SetTextColor(1); T1.SetTextSize(0.14);
  T1.DrawText(111,0.4,"P2"); T1.DrawText(111,1.0,"P1");
  T1.SetTextColor(7); T1.SetTextSize(0.14);
  Int_t NP=NofUniquePairsP1P2();
  Int_t UP=NofUsedUniquePairsP1P2();
  Int_t FP=NofFreeUniquePairsP1P2();
  sprintf(Inf,"%s%d%s%d%s%d%s","Total:",NP,"   Used:",UP,"   Free:",FP,"   Dim: 1");
  T1.DrawText(4,1.64,Inf);
  LP1.SetLineColor(7);
  for(Int_t idx=0;idx<=fMaxIdx1;idx++) {
   if (UsedUniqueP1x2(idx))
    LP1.DrawLine(5+(0.5+idx)*100/fN1MAX,1.1,5+(0.5+GetUsedUniqueIDXP1x2(idx))*100/fN2MAX,0.5);
  }
  AxP1.SetLabelSize(0.14); AxP1.SetLineColor(1); AxP1.SetLineWidth(1);
  AxP1.DrawAxis(5,0.5,105,0.5,0,fN2MAX,510,"+");
  AxP1.SetLabelOffset(0.025);
  AxP1.DrawAxis(5,1.1,105,1.1,0,fN1MAX,510,"-");
  cUP1P2->Update();
  }
}
void  T49Mixer::Copyright(Int_t opt)
{ 
// Copyright window with Button option
// opt=0 for Rights ; opt=1 for Exit GUI 
    TLine LP1; TText T1; TButton *but1; 
    sprintf(fIDS,"%s MX%d","Copyright",fID);
    cEx = new TDialogCanvas(fIDS,fIDS,444,222);
    cEx->cd(); cEx->Range(0,0,100,100); cEx->SetBorderSize(7);
      T1.SetTextAlign(22); T1.SetTextColor(2);
      T1.SetTextSize(0.2); T1.DrawText(50,80,"T49MiXeR CLASS"); 
      T1.SetTextSize(0.07);T1.DrawText(50,66,"Version 1.0 - CERN 1999 ");
      T1.SetTextSize(0.06); 
      T1.DrawText(50,26,"COMMENTS AND SUGGESTIONS ARE APPRECIATED");
      T1.SetTextSize(0.07); T1.DrawText(50,14,"pfilips@mppmu.mpg.de ");
    if (opt==0) {
      sprintf(fIDS,"%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->CopyClose();");
      but1 = new TButton("MPI DESIGN - RIGHTS RESERVED",fIDS,0.1,0.39,0.9,0.53);
      but1->SetFillColor(32);
    } else {
      sprintf(fIDS,"%s%d%s;%s%d%s","((T49Mixer*)(*gT49Mixer)[",fID,"])->CopyClose()",
                                   "((T49Mixer*)(*gT49Mixer)[",fID,"])->CloseWindows()");
      but1 = new TButton("EXIT T49MiXeR GUI",fIDS,0.24,0.39,0.76,0.53);
      but1->SetFillColor(44);
    }
    but1->SetFillStyle(6); but1->Draw();
    cEx->Update();
}
void  T49Mixer::CopyClose()
{ 
// Close Copyright window
  if(gRandom->Rndm()<0.7) {
    if (cEx) { 
      cEx->Close(); 
      cEx->Update(); 
  } }
}
void  T49Mixer::CloseWindows()
{
// Close all GUI objects 
  if(cP1)    { delete cP1; cP1=0; };        // POOL1
  if(cP2)    { delete cP2; cP2=0; };        // POOL2
  if(cXY)    { delete cXY; cXY=0; };        // P1P2
  if(cYY)    { delete cYY; cYY=0; };        // P2P2
  if(cXX)    { delete cXX; cXX=0; };        // P1P1
  if(HistXX) { delete HistXX; HistXX=0; }   // hP1P1
  if(HistYY) { delete HistYY; HistYY=0; }   // hP2P2
  if(HistXY) { delete HistXY; HistXY=0; }   // hP1P2
  if(cUP1P2) { delete cUP1P2; cUP1P2=0; }   // UP1P2
  if(cUP1P1) { delete cUP1P1; cUP1P1=0; }   // UP1P1
  if(cUP2P2) { delete cUP2P2; cUP2P2=0; }   // UP2P2
  if(cGIF)   { delete cGIF; cGIF=0; }       // MAIN WINDOW
  if(cEx)    { cEx->Close(); }       // COPYR ! only close
}
void  T49Mixer::ExitShow()
{
// T49Mixer GUI destructor
  if(fVerbose) { cout << "Exiting T49MiXeR Show." << endl;}
  CloseWindows();                          // Del all +
  if(cEx) { delete cEx;  cEx=0; }          // COPYRIGHT
}
//////////////////////////////////////////////// DUMP
void   T49Mixer::DumpP1P1P2P2()
{
// Print status of P1P1 and P2P2 pairs in ASCII format
  Bool_t status; int k=0;
  cout << "Dump P1P1/P2P2:(" << fMaxIdx1 << "," <<fMaxIdx2 << ")\n";
  for(int i=0;i<fMaxIdx1;i++) { printf("XX%3d: ",i);
   for(int j=0;j<fMaxIdx2;j++) {
     k++; if(k>800) {k=0; cout << endl; }
     status = i<j ? UsedPairP1P1(i,j) : UsedPairP2P2(i,j);
     cout << (int)status << flush;
   }
   cout << endl; k=0;
  }
}
void   T49Mixer::DumpP1P2()
{
// Print status of P1P2 pairs in ASCII format
  Bool_t status; int k=0;
  cout << "Dump P1P2:(" << fMaxIdx1 << "," <<fMaxIdx2 << ")\n";
  for(int i=0;i<fMaxIdx1;i++) { printf("XY%3d: ",i);
   for(int j=0;j<fMaxIdx2;j++) {
     k++; if(k>800) {k=0; cout << endl; }
     status = UsedPairP1P2(i,j);
     cout << (int)status << flush;
   }
   cout << endl; k=0;
  }
}
void   T49Mixer::DumpP1P1()
{
// Print status of P1P1 pairs in ASCII format
  Bool_t status; int k=0;
  cout << "Dump P1P1:(" << fMaxIdx1 << "," <<fMaxIdx1 << ")\n";
  for(int i=0;i<fMaxIdx1;i++) { printf("XX%3d: ",i);
   for(int j=0;j<fMaxIdx1;j++) {
     k++; if(k>800) {k=0; cout << endl; }
     status = UsedPairP1P1(i,j);
     if (i>j) { cout << (int)status << flush; }
        else  { cout <<     " "     << flush; }
   }
   cout << endl; k=0;
  }
}
void   T49Mixer::DumpP2P2()
{
// Print status of P2P2 pairs in ASCII format
  Bool_t status; int k=0;
  cout << "Dump P2P2:(" << fMaxIdx2 << "," <<fMaxIdx2 << ")\n";
  if (nPools==2) {
    for(int i=0;i<fMaxIdx2;i++) { printf("XX%3d: ",i);
     for(int j=0;j<fMaxIdx2;j++) {
       k++; if(k>800) {k=0; cout << endl; }
       status = UsedPairP2P2(i,j);
       if (i<j) { cout << (int)status << flush; }
          else  { cout <<     " "     << flush; }
     }
     cout << endl; k=0;
    }
  }
}
//////////////////////////////////////////////// RESET Used Info
void  T49Mixer::AllUsedReset()
{
// Clear all Used/Nused information in Mixer (all pairs and objects)
  if (nPools == 1)
  {
    ResetUP1();        // Used objects
    ResetUniP1P1();    // Unique pairs
    ResetP1P1_P2P2();  // Faster P1P1 Pair Reset
  }
  if (nPools == 2)
  {
    ResetUP1();    ResetUP2();
    ResetAllPxPx();
    ResetAllUniPxPx();
  }
}

void T49Mixer::AllUsedResetP1()
{
// Clear all Used/Nused information in Pool1 (pairs and objects)
  ResetUP1();
  ResetP1P1();
  ResetUniP1P1();
}
void T49Mixer::AllUsedResetP2()
{
// Clear all Used/Nused information in Pool2 (pairs and objects)
  ResetUP2();
  ResetP2P2();
  ResetUniP2P2();
}

void T49Mixer::ResetUP1() 
{
// Set all TObjects in Pool1 as NotUsed
  fUsed1->Reset(); 
  fL1 = 0;
  fLAC1 = 0;
}

void T49Mixer::ResetUP2()
{
// Set all TObjects in Pool2 as NotUsed
  fUsed2->Reset();  
  fL2 = 0;
  fLAC2 = 0;
}
////////////////////////////////////////// Reset Pairs
void T49Mixer::ResetAllPxPx()
{
// Clear Used/Nused P1P1, P1P2, P2P2 pairs information
  fLACp = 0;
  ResetP1P2();
  ResetP1P1_P2P2();
}
void T49Mixer::ResetP1P1_P2P2()
{ 
// Clear Used/Nused P1P1 and P2P2 pairs information
// Call also for fast P1P1 case if only Pool1 used
  fMaskXX->Reset();                
  for(int i=0; i<=nMaxMXX-1; i++) 
  { SetUsedPairP1P1(i,i); }     // exclude (i,i) pairs
  nUsedP2P2=0;                  // P1P1(i,i)=P2P2(i,i)
  nUsedP1P1=0;
  fL11x1 = 0; fL11x2 = 0;
  fL22x1 = 0; fL22x2 = 0;
}
void T49Mixer::ResetP1P1()
{ 
// Clear Used/Nused P1P1 pairs information
// note: ResetP1P1_P2P2() is faster 
  if (nPools==2)
  {
    for(int i=0; i<=fIdx1MAX; i++)
    { for(int j=0; j<=i; j++)
      { if (i==j)  SetUsedPairP1P1(i,i);  // set P1P1(i,i) Used
            else  SetNUsedPairP1P1(i,j);  // set P1P1(i,j) Not Used
    } }
    nUsedP1P1=0;
    fL11x1 = 0; fL11x2 = 0;
  } else {
    ResetP1P1_P2P2(); 
  }
} 
void T49Mixer::ResetP2P2()
{ 
// Clear Used/Nused P2P2 pairs information
  for(int i=0; i<=fIdx2MAX; i++)
  { for(int j=0; j<=i; j++)
    { if (i==j)  SetUsedPairP2P2(i,i); // set pairs P2P2(i,i) Used
           else  SetNUsedPairP2P2(i,j); // set pairs P2P2(i,j) Not Used
  } }
  nUsedP2P2=0;
  fL22x1 = 0; fL22x2 = 0;
}
void T49Mixer::ResetP1P2()
{ 
// Clear Used/Nused P1P2 pairs information
// (i,i) pairs forbidden if SetP1P2Mode(1)
  fMaskXY->Reset();
  nUsedP1P2=0;
  if(fP1P2Mode==1) {   
    for(int i=0; i<nMaxMXY; i++) SetUsedPairP1P2(i,i);
    nUsedP1P2=0;
  } 
  fL12x1 = 0; fL12x2 = 0;
}
void T49Mixer::SetP1P2Mode(Int_t m)
{
// Set P1P2Mode = 1 to exlude generation of P1P2(i,i) pairs
  if (m<2 && m>-1) fP1P2Mode = m;
  ResetP1P2();  
}
///////////////////////////////// Reset Unique
void T49Mixer::ResetAllUniPxPx()
{
// Clear Used/Nused Unique (P1P1, P1P2, P2P2) pairs information
  fLACu = 0;
  ResetUniP1P1();
  ResetUniP2P2();
  ResetUniP1P2();
}
void T49Mixer::ResetUniP1P1()
{ 
// Clear Used/Nused P1P1 Unique pairs information
  fUniq11->Reset();
  nUsedUniP1P1 = 0;
  fU11x1 = 0; fU11x2 = 0;
}
void T49Mixer::ResetUniP2P2()
{
// Clear Used/Nused P2P2 Unique pairs information
  fUniq22->Reset();
  nUsedUniP2P2 = 0;
  fU22x1 = 0; fU22x2 = 0;
}
void T49Mixer::ResetUniP1P2()
{
// Clear Used/Nused P1P2 Unique pairs information
  fUniq12->Reset(); fUniq21->Reset();
  nUsedUniP1P2 = 0; nUsedUniP2P1 = 0;
  fU12x1 = 0; fU12x2 = 0;
}
////////////////////////////////////////////////////// SetPool1, SetPool2
Bool_t T49Mixer::SetPools(TObjArray *ObjA1, TObjArray *ObjA2)
{
// Copy pointers of TObjects (from TObjArrays) into Pools
// + reset Used/NotUsed status of changed pairs & Objects
// Use only if Pools are empty e.g. after ClearPools()
// Note: This is FASTER then SetPool1() + SetPool2()
  if (nObjects1!=0) { cout << "Pool1 not Empty !" << endl; return false; }
  if (nObjects2!=0) { cout << "Pool2 not Empty !" << endl; return false; }
  Int_t N1 = ObjA1->GetEntries();
  Int_t N2 = ObjA2->GetEntries();
  if ((N1>0 && N1<=fN1MAX) && (N2>0 && N2<=fN2MAX)) {
    for(int i=0;i<N1;i++)  fPool1->AddAt(ObjA1->At(i),i);
    for(int i=0;i<N2;i++)  fPool2->AddAt(ObjA2->At(i),i);
    nObjects1 = N1; nObjects2 = N2;
    fMaxIdx1 = N1 - 1; fMaxIdx2 = N2 - 1;
    fP1Rx=0; fP2Rx=0;
    fP1Full = 1; fP2Full = 1;
    ResetUP1(); ResetUP2();
    ResetAllPxPx();   // This is faster
    ResetAllUniPxPx();
    fLAC2 = 61; fLAC1 = 51; fLAC=76;
    return true;
  }
  else { return false; }
}
Bool_t T49Mixer::SetPool1(TObjArray *ObjA)   
{ 
// Copy pointers of TObjects (from TObjArray) into Pool1
// +reset Used/NotUsed status of changed pairs & Objects
// Use only if Pool1 is empty e.g. after ClearPool1()
  if (nObjects1!=0) 
     { cout << "Can't SetPool1: It is not Empty !" << endl; return false; }
  Int_t N = ObjA->GetEntries();
  if (N>0 && N<=fN1MAX) 
  { fLAC = fLAC1 = 51;
    ResetUP1();         // used Obj. in P1
    ResetUniP1P1();     // Unique P1P1
    if(nPools==2) { 
      ResetUniP1P2();
      ResetP1P2(); 
      ResetP1P1(); // this slow
    } else { ResetP1P1_P2P2(); } 
    for(int i=0;i<N;i++) { fPool1->AddAt(ObjA->At(i),i); }
    nObjects1 = N;
    fMaxIdx1 = nObjects1 - 1;
    fP1Rx=0;
    fP1Full = 1;
    return true;
  } else {
    cout << "Didn't SetPool1: ObjArray size out of range !" << endl;
    return false;
  }
}
Bool_t T49Mixer::SetPool2(TObjArray *ObjA)   
{
// Copy pointers of TObjects (from TObjArray) into Pool2
// +reset Used/NotUsed status of changed pairs & Objects
// Use only if Pool2 is empty e.g. after ClearPool2()
  if (nObjects2!=0) 
     { cout << "Can't SetPool2: It is not Empty !" << endl; return false; }
  if (nPools==1)  
    { cout << "Can't SetPool2: It does'n Exist !" << endl; return false; }
   else 
  {
    Int_t N = ObjA->GetEntries();
    if (N>0 && N<=fN2MAX)
    { fLAC = fLAC2 = 61;
      ResetUP2();        // used Obj. P2
      ResetUniP2P2();    // Unique P2P2
      ResetUniP1P2();
      ResetP1P2(); 
      ResetP2P2();       // this is slow compared tp ResetP1P1_P2P2()
      for(int i=0;i<N;i++) { fPool2->AddAt(ObjA->At(i),i); } 
      nObjects2 = N;
      fMaxIdx2 = nObjects2 - 1;
      fP2Rx=0; 
      fP2Full = 1;
      return true;
    } else { 
      cout << "Didn't SetPool2: ObjArray size out of range !" << endl;
      return false;
    }
  }
}
////////////////////////////////////////////////////// AddOne, AddToP1,2 AddToP1P2
Int_t  T49Mixer::AddOne(TObject *Obj)
{
// Identical to AddToP1(Obj)
  return AddToP1(Obj);
}

Int_t  T49Mixer::AddToP1(TObject *Obj)
{
// Add TObject to Pool1 = create cloned copy of TObject in Pool1 
// returns internal position of TObject in the Pool
  if (nObjects1 < fN1MAX) 
  { fLAC = fLAC1 = 52;
    TObject* a = Obj->Clone(); 
       if (fVerbose) cout << " <I> T49Mixer: Object Cloned !!!" << endl;
    Int_t idx = fPool1->AddAtFree(a);  
    SetNUsedOneP1(idx);
    fMaxIdx1=nObjects1;
    nObjects1++;
    if (nObjects1 == fN1MAX) fP1Full = 1;
       if (fVerbose) printf( " <I> T49Mixer: Object add P1:%d \n",idx);
    return idx;
  }
  else { return -1;}
}

Int_t  T49Mixer::AddToP2(TObject *Obj)
{ 
// Add TObject to Pool2 = create cloned copy of TObject in Pool2
// returns internal position of TObject in the Pool
  if (nObjects2 < fN2MAX)
  { fLAC = fLAC2 = 62;
    TObject* a = Obj->Clone();
       if (fVerbose) cout << " <I> T49Mixer: Object Cloned !!!" << endl;
    Int_t idx = fPool2->AddAtFree(a); 
    SetNUsedOneP2(idx);
    fMaxIdx2 = nObjects2;
    nObjects2++;
    if (nObjects2 == fN2MAX) fP2Full = 1;
       if (fVerbose) printf( " <I> T49Mixer: Object add P2:%d \n",idx);
    return idx; 
  }
  else { return -1;}
}

Bool_t T49Mixer::AddToP1P2(TObject *Obj1, TObject *Obj2)
{
// Add Obj1 to Pool1  &  Add Obj2 to Pool2
  if (AddToP1(Obj1)>=0 && AddToP2(Obj2)>=0) 
     { return true;  }
  else 
     { return false; } 
}
/////////////////////////////////////////////////// ReplaceAt in P1, P2, P1P2
Int_t  T49Mixer::ReplaceOneAt(TObject *Obj, Int_t idx)
{
// Identical to ReplaceInP1At(Obj,idx)
    return ReplaceInP1At(Obj,idx);
}
Int_t  T49Mixer::ReplaceInP1At(TObject *Obj, Int_t idx)
{   
// Replace = delete old TObject + Add new TObject in Pool1 at position idx
    if (CheckBound1(idx)) 
    { 
      TObject* a = GetFromP1At(idx);
      delete a;
       if (fVerbose) cout << " <I> T49Mixer: Object Delete  P1:" << idx << endl;
      a = Obj->Clone();
       if (fVerbose) cout << " <I> T49Mixer: Object Cloned !!!" << endl;
      fPool1->AddAt(a,idx);
       if (fVerbose) printf(" <I> T49Mixer: Object Replace P1:%d \n",idx);
       SetNUsedAfterReplaceP1(idx);  
      return idx;
    } else return -1;
}
Int_t  T49Mixer::ReplaceInP2At(TObject *Obj, Int_t idx)
{
// Replace = delete old TObject + Add new TObject in Pool2 at position idx
    if (CheckBound2(idx))
    { 
      TObject* a = GetFromP2At(idx);
      delete a;
       if (fVerbose) cout << " <I> T49Mixer: Object Delete  P2:" << idx << endl;
      a = Obj->Clone();
       if (fVerbose) cout << " <I> T49Mixer: Object Cloned !!!" << endl;
      fPool2->AddAt(a,idx);
       if (fVerbose) printf(" <I> T49Mixer: Object Replace P2:%d \n",idx);
      SetNUsedAfterReplaceP2(idx);
      return idx;
    } else return -1;
}
Bool_t T49Mixer::ReplaceInP1P2At(TObject *Obj1, TObject *Obj2, Int_t idx1, Int_t idx2)
{   
// Replace (Obj1 in Pool1 ,  Obj2 in Pool2) at positions (idx1 , idx2)
    if(CheckBound1(idx1) && CheckBound2(idx2))
    {
      ReplaceInP1At(Obj1,idx1);
      ReplaceInP2At(Obj2,idx2);
      return true;
    } else return false;
}
///////////////////////////////////////////////////// Replace in P1,P2,P1P1 at fP1Rx++, fP2Rx++
Int_t  T49Mixer::ReplaceOne(TObject *Obj)
{
// Identical to ReplaceInP1(Obj)
    return ReplaceInP1(Obj);
}
Int_t  T49Mixer::ReplaceInP1(TObject *Obj)
{   
// Replace = delete old and Add new TObject in Pool1 
//           at position idx internally increasing
    fLAC = fLAC1 = 53;
    Int_t idx = fP1Rx;
    ReplaceInP1At(Obj,idx);
    if (fP1Rx < fMaxIdx1) { fP1Rx++; } else { fP1Rx = 0; }
    return idx;
}
Int_t  T49Mixer::ReplaceInP2(TObject *Obj)
{   
// Replace = delete old and Add new TObject in Pool1
//           at position idx internally increasing
    fLAC = fLAC2 = 63;
    Int_t idx = fP2Rx;
    ReplaceInP2At(Obj,idx);
    if (fP2Rx < fMaxIdx2) { fP2Rx++; } else { fP2Rx = 0; }
    return idx;
}
Bool_t T49Mixer::ReplaceInP1P2(TObject *Obj1, TObject *Obj2)
{ 
// Replace Obj1 in Pool1 and Obj2 in Pool2 
// at positions idx1 & idx2 internally increasing
    ReplaceInP1(Obj1); 
    ReplaceInP2(Obj2);
    return true;
}
//////////////////////////////////////////////////////////// PUT To P1, P2, P1P2
Bool_t T49Mixer::PutToP1P2(TObject *Obj1, TObject *Obj2)
{
// Put Obj1 to Pool1 and put Obj2 to Pool2
    PutToP1(Obj1);
    PutToP2(Obj2);
    return true;
}
Int_t  T49Mixer::PutToP1(TObject *Obj)  
{
// AddToP1(Obj) or ReplaceInP1(Obj) if Pool1 is full
// returns position of Object in Pool
// Internal increasing counter is used for replacement.
    if (Pool1Full()) { return ReplaceInP1(Obj); } else { return AddToP1(Obj); }
}
Int_t  T49Mixer::PutToP2(TObject *Obj)
{
// AddToP2(Obj) or ReplaceInP2(Obj) if Pool2 is full
// returns position of Object in Pool
// Internal increasing counter is used for replacement.
    if (Pool2Full()) { return ReplaceInP2(Obj); } else { return AddToP2(Obj); }
}
/////////////////////////////////////////////////// GetOne, GetFromP1,P2
TObject*  T49Mixer::GetFromP1()
{ 
// Get pointer to Randomly chosen TObject in Pool1
// TObject is marked as Used
  fLAC = 7;
  Int_t idx = (int)(gRandom->Rndm()*((float)nObjects1));
  SetUsedOneP1(idx);
    if (fVerbose) printf(" <I> T49Mixer: Object out P1:%d \n",idx);
  return fPool1->At(idx);
}
TObject*  T49Mixer::GetFromP2()
{ 
// Get pointer to Randomly chosen TObject in Pool2
// TObject is marked as Used
  fLAC = 8;
  Int_t idx = (int)(gRandom->Rndm()*((float)nObjects2));
  SetUsedOneP2(idx);
    if (fVerbose) printf(" <I> T49Mixer: Object out P2:%d \n",idx);
  return fPool2->At(idx);
}
TObject*  T49Mixer::GetFromPool1(Int_t &idx)
{ 
// Internal routine
  idx = (int)(gRandom->Rndm()*((float)nObjects1));
  return fPool1->At(idx);
}

TObject*  T49Mixer::GetFromPool2(Int_t &idx)
{ 
// Internal routine
  idx = (int)(gRandom->Rndm()*((float)nObjects2));
  return fPool2->At(idx);
}

//////////////////////////////////////////////////////// GetNuOne
TObject*  T49Mixer::GetNuOneP1()
{ 
// Get pointer to first NotUsed TObject in Pool1 
// TObject is marked as Used
  TObject* a = NULL;
  int nF = NofFreeP1();
  if(nF)
  { fLAC = 17;
    int idx = fL1;
    while(UsedOneP1(idx)) { idx++; }
    a = fPool1->At(idx);
    SetUsedOneP1(fL1=idx);
      if (fVerbose) printf(" <I> T49Mixer: Object out P1:%d \n",idx);
  }
  return a;
}
TObject*  T49Mixer::GetNuOneP2()
{
// Get pointer to first NotUsed TObject in Pool2 
// TObject is marked as Used
  TObject* a = NULL;
  int nF = NofFreeP2();
  if(nF)
  { fLAC = 18;
    int idx = fL2;
    while(UsedOneP2(idx)) { idx++; }
    a = fPool2->At(idx);
    SetUsedOneP2(fL2=idx);
      if (fVerbose) printf(" <I> T49Mixer: Object out P2:%d \n",idx);
  }
  return a;
}
//////////////////////////////////////////////// GetNuROne
TObject*  T49Mixer::GetNuROneP1()
{ 
// Get pointer to random NotUsed TObject in Pool1 
// TObject is marked as Used
  TObject* a = NULL;
  int nF = NofFreeP1(); 
  if(nF) { fLAC = 27;
    int idx = 0;
    if(nF>10) { // Use Guessing strategy
      Int_t ix = nObjects1; // Max Number of Attempts
      Bool_t fHit = false;  // Hit Flag
      while((fHit==false) && (--ix != 0)) {
        idx = (int)(gRandom->Rndm()*((float)nObjects1));
        if(!UsedOneP1(idx)) fHit = true;
      } // loop
      if (fHit) {  // Success !!!
        a = fPool1->At(idx);
        SetUsedOneP1(idx);
        if (fVerbose) printf(" <I> T49Mixer: Object out P1:%d \n",idx);
        return a;  // Exit proc !
      }
    }; // End of Guessing, If no success use RndOffset  
    idx = 0;
    int id  = (int) (gRandom->Rndm()*((float)nF));
    while(id >= 0) { if (!UsedOneP1(idx)) id--;  idx++; }
    idx--;
    a = fPool1->At(idx);
    SetUsedOneP1(idx);
    if (fVerbose) printf(" <I> T49Mixer: Object out P1:%d \n",idx);
    return a;
  } // Done. If nF=0 return NULL
  return a;
}
TObject*  T49Mixer::GetNuROneP2()
{ 
// Get pointer to random NotUsed TObject in Pool2 
// TObject is marked as Used
  TObject* a = NULL;
  int nF = NofFreeP2();
  if(nF) { fLAC = 28;
    int idx = 0;
    if(nF>10) { // Use Guessing strategy
      Int_t ix = nObjects2; // Max Number of Attempts
      Bool_t fHit = false;  // Hit Flag
      while((fHit==false) && (--ix != 0)) {
        idx = (int)(gRandom->Rndm()*((float)nObjects2));
        if(!UsedOneP2(idx)) fHit = true;
      } // loop
      if (fHit) {  // Success !!!
        a = fPool2->At(idx);
        SetUsedOneP2(idx);
        if (fVerbose) printf(" <I> T49Mixer: Object out P2:%d \n",idx);
        return a;  // Exit proc !
      }
    }; // End of Guessing, If no success use RndOffset 
    idx = 0;
    int id  = (int) (gRandom->Rndm()*((float)nF));
    while(id >= 0) { if (!UsedOneP2(idx)) id--;  idx++; }
    idx--;
    a = fPool2->At(idx);
    SetUsedOneP2(idx);
    if (fVerbose) printf(" <I> T49Mixer: Object out P2:%d \n",idx);
    return a;
  } // Done. If nF=0 return NULL
  return a;
}
/////////////////////////////////////// GetOneAt, GetFromP1At, GetFromP2At
TObject*  T49Mixer::GetFromP1At(Int_t idx)
{ 
// Get pointer to TObject in Pool1 at position idx
// TObject is marked as Used
  if (!CheckBound1(idx)) {
    printf("<W> T49Mixer::GetFromP1At(%d) index out of range.\n",idx);
    idx = (int) (gRandom->Rndm()*((float)nObjects1));
    printf("    Replacing by random index: %d \n",idx);
  } 
  SetUsedOneP1(idx);
     if (fVerbose) 
       printf(" <I> T49Mixer: Object out P1:%d \n",idx);
  TObject* a = fPool1->At(idx);
  return a;
}

TObject*  T49Mixer::GetFromP2At(Int_t idx)
{
// Get pointer to TObject in Pool2 at position idx
// TObject is marked as Used
  if (!CheckBound2(idx)) {
    printf("<W> T49Mixer::GetFromP1At(%d) index out of range.\n",idx);
    idx = (int) (gRandom->Rndm()*((float)nObjects2));
    printf("    Replacing by random index: %d \n",idx);
  }
  SetUsedOneP2(idx);
     if (fVerbose)
       printf(" <I> T49Mixer: Object out P2:%d \n",idx);
  TObject* a = fPool2->At(idx);
  return a;
}
//////////////////////////////////////// GET PAIRS AT P1P1,P2P2,P1P2
Bool_t    T49Mixer::GetPairP1P1At(TObject** Obj1,  TObject** Obj2, Int_t idx1, Int_t idx2)
{
// Get pair of two Objects from Pool1 at (idx1,idx2)
//          pair is marked as Used
// Call by: GetPairP1P1At(&Obj1, &Obj2,i,j)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  if (CheckBound1(idx1) && CheckBound1(idx2))
  { 
    fSeeFlag = 0;
    *Obj1 = GetFromP1At(idx1); 
    *Obj2 = GetFromP1At(idx2);
    fSeeFlag = 1;
    SetUsedPairP1P1(idx1,idx2); 
    return true;
  } else
  {
    return false;
  }
}
Bool_t    T49Mixer::GetPairP2P2At(TObject** Obj1, TObject** Obj2, Int_t idx1, Int_t idx2)
{ 
// Get pair of two Objects from Pool2 at (idx1,idx2)
//          pair is marked as Used
// Call by: GetPairP2P2At(&Obj1, &Obj2,i,j)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  if (CheckBound2(idx1) && CheckBound2(idx2)) 
  {
    fSeeFlag = 0;
    *Obj1 = GetFromP2At(idx1);
    *Obj2 = GetFromP2At(idx2);
    fSeeFlag = 1;
    SetUsedPairP2P2(idx1,idx2);
    return true;
  } else 
  {
    return false;
  }
}
Bool_t    T49Mixer::GetPairP1P2At(TObject** Obj1, TObject** Obj2, Int_t idx1, Int_t idx2)
{ 
// Get pair of Objects from Pool1 and Pool2 at (idx1,idx2)
//          pair is marked as Used
// Call by: GetPairP1P2At(&Obj1, &Obj2,i,j)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  if (CheckBound1(idx1) && CheckBound2(idx2))
  {
    fSeeFlag = 0;
    *Obj1 = GetFromP1At(idx1);
    *Obj2 = GetFromP2At(idx2);
    fSeeFlag = 1;
    SetUsedPairP1P2(idx1,idx2);
    return true; 
  } else
  {
    return false;
  }
}

/////////////////////////////////////// GET RANDOM PAIRS P1P1, P2P2, P1P2
Bool_t    T49Mixer::GetRPairP1P1(TObject** Obj1, TObject** Obj2)
{
// Get random pair of ANY two Objects from Pool1
//          pair is marked as Used
// Call by: GetRPairP1P1(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t idx1, idx2;
  *Obj1 = GetFromPool1(idx1);      // internal routine, random idx1
  do{*Obj2 = GetFromPool1(idx2);} while(idx1==idx2); // random idx2
  SetUsedPairP1P1(I1=idx1,I2=idx2); // I1, I2 for B.C.M.
  return true;
}
Bool_t    T49Mixer::GetRPairP1P1x(TObject** Obj1, TObject** Obj2)
{
// Get random pair of ANY two Objects from Pool1
//      pair is NOT marked as Used
// Call by: GetRPairP1P1x(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t idx1, idx2;
  *Obj1 = GetFromPool1(idx1);      // internal routine, random idx1
  do{*Obj2 = GetFromPool1(idx2);} while(idx1==idx2); // random idx2
  I1=idx1; I2=idx2;                // I1, I2 for B.C.M.
  return true;
}
Bool_t    T49Mixer::GetRPairP2P2(TObject** Obj1, TObject** Obj2)
{ 
// Get random pair of ANY two Objects from Pool2
//          pair is marked as Used
// Call by: GetRPairP2P2(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t idx1, idx2;
  *Obj1 = GetFromPool2(idx1);         // internal routine, random idx1
  do{*Obj2 = GetFromPool2(idx2);} while(idx1==idx2); // random idx2
  SetUsedPairP2P2(I1=idx1,I2=idx2); // I1, I2 for B.C.M.
  return true;
}
Bool_t    T49Mixer::GetRPairP2P2x(TObject** Obj1, TObject** Obj2)
{ 
// Get random pair of ANY two Objects from Pool2
//      pair is NOT marked as Used
// Call by: GetRPairP2P2x(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t idx1, idx2;
  *Obj1 = GetFromPool2(idx1);         // internal routine, random idx1
  do{*Obj2 = GetFromPool2(idx2);} while(idx1==idx2); // random idx2
  I1=idx1; I2=idx2;                   // I1, I2 for B.C.M.
  return true;
}
Bool_t    T49Mixer::GetRPairP1P2(TObject** Obj1, TObject** Obj2)
{ 
// Get random pair of ANY Objects from Pool1 and Pool2
//          pair is marked as Used
// Call by: GetRPairP1P2(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t idx1, idx2;
  *Obj1 = GetFromPool1(idx1); // internal routine, random idx1
  *Obj2 = GetFromPool2(idx2); // internal routine, random idx2
  SetUsedPairP1P2(I1=idx1,I2=idx2); // I1, I2 for B.C.M.
  return true;
}
Bool_t    T49Mixer::GetRPairP1P2x(TObject** Obj1, TObject** Obj2)
{
// Get random pair of ANY Objects from Pool1 and Pool2
//      pair is NOT marked as Used
// Call by: GetRPairP1P2x(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t idx1, idx2;
  *Obj1 = GetFromPool1(idx1); // internal routine, random idx1
  *Obj2 = GetFromPool2(idx2); // internal routine, random idx2
  I1=idx1; I2=idx2;           // I1, I2 for B.C.M.
  return true;
}
//////////////////////////////////////// GET Not Used Pairs
Bool_t    T49Mixer::GetNuPairP1P1(TObject** Obj1,TObject** Obj2)
{
// Get first Not used pair of two Objects from Pool1
//           and mark pair as Used
// Call by: GetNuPairP1P1(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t idx1 = fL11x1;
  Int_t idx2 = fL11x2;
  Int_t nF = NofFreePairsP1P1();
  if (nF) 
  { fLAC = fLACp = 11;
    while(UsedPairP1P1(idx1,idx2))
    { 
      if (idx1<fMaxIdx1) { idx1++; } else { idx1 = 0; idx2++; }
    }
    SetUsedPairP1P1(idx1,idx2);
    fSeeFlag = 0;
    *Obj1 = GetFromP1At(fL11x1 = idx1);
    *Obj2 = GetFromP1At(fL11x2 = idx2); 
    fSeeFlag = 1; 
    return true;
  } 
  else  return false;   
}  
Bool_t    T49Mixer::GetNuPairP2P2(TObject** Obj1,TObject** Obj2)
{
// Get first Not used pair of two Objects from Pool2
//           and mark pair as Used
// Call by: GetNuPairP2P2(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t nF = NofFreePairsP2P2();
  if (nF) 
  { fLAC = fLACp = 12;
    Int_t idx1 = fL22x1;
    Int_t idx2 = fL22x2;
    while(UsedPairP2P2(idx1,idx2))
    { 
      if (idx1<fMaxIdx2) { idx1++; } else { idx1 = 0; idx2++; }
    }
    SetUsedPairP2P2(idx1,idx2);
    fSeeFlag = 0;
    *Obj1 = GetFromP2At(fL22x1 = idx1);
    *Obj2 = GetFromP2At(fL22x2 = idx2);
    fSeeFlag = 1;
    return true;
  } 
  else  return false;
} 
Bool_t    T49Mixer::GetNuPairP1P2(TObject** Obj1,TObject** Obj2)
{
// Get first Not used pair of Objects from Pool1 and Pool2
//           and mark pair as Used
// Call by: GetNuPairP1P2(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t nF = NofFreePairsP1P2();
  if (nF)
  { fLAC = fLACp = 13;
    Int_t idx1 = fL12x1;
    Int_t idx2 = fL12x2;
    while(UsedPairP1P2(idx1,idx2))
    { 
      if (idx1<fMaxIdx1) { idx1++; } else { idx1 = 0; idx2++; }
    }
    SetUsedPairP1P2(idx1,idx2);
    fSeeFlag = 0;
    *Obj1 = GetFromP1At(fL12x1 = idx1);
    *Obj2 = GetFromP2At(fL12x2 = idx2);
    fSeeFlag = 1;
    return true;
  }
  else return false;
} 
/////////////////////////////////////// Get Not Used Random Pairs
Bool_t    T49Mixer::GetNuRPairP1P1(TObject** Obj1,TObject** Obj2)
{ 
// Get random Not used pair of two Objects from Pool1
//           and mark pair as Used
// Call by: GetNuRPairP1P1(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t nF = NofFreePairsP1P1();
  if (nF) { fLAC = fLACp = 21; 
    Int_t idx1 = 0;   
    Int_t idx2 = 0;
    if (nF>10) { // Use Guessing strategy
      Long_t ix = nObjects1*nObjects1; // Max Number of Attempts
      Bool_t fHit = false;            // Hit Flag
      while((fHit==false) && (--ix != 0)) {
        idx1 = (int)(gRandom->Rndm()*((float)nObjects1));    
        idx2 = (int)(gRandom->Rndm()*((float)nObjects1));   
        if( (idx1 != idx2) && !UsedPairP1P1(idx1,idx2) ) fHit = true; 
      } 
      if (fHit) { SetUsedPairP1P1(idx1,idx2);
         fSeeFlag = 0;
         *Obj1 = GetFromP1At(fR11x1 = idx1);
         *Obj2 = GetFromP1At(fR11x2 = idx2);
         fSeeFlag = 1; 
         return true; }
    }; // End of Guessing. If no Hit continue below:     
      idx1 = 0; // Use RndOffset strategy
      idx2 = 0;
      int id = (int) (gRandom->Rndm()*((float)nF)); // Offset
      while(id >= 0) { 
        if (!UsedPairP1P1(idx1,idx2)) id--;
        if (idx1<fMaxIdx1) {idx1++;} else {idx1=0; idx2++;}
      }
      if (idx1==0) {idx1=fMaxIdx1; idx2--;} else {idx1--;}
      SetUsedPairP1P1(idx1,idx2);
      fSeeFlag = 0;
      *Obj1 = GetFromP1At(fR11x1 = idx1);
      *Obj2 = GetFromP1At(fR11x2 = idx2);
      fSeeFlag = 1;
    return true;
  }
  else return false;
}
Bool_t    T49Mixer::GetNuRPairP2P2(TObject** Obj1,TObject** Obj2)
{
// Get random Not used pair of two Objects from Pool2
//           and mark pair as Used
// Call by: GetNuRPairP2P2(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t nF = NofFreePairsP2P2();
  if (nF) { fLAC = fLACp = 22;         
    Int_t idx1 = 0;          
    Int_t idx2 = 0;
    if (nF>10) {  // Use Guessing strategy
      Long_t ix = nObjects2*nObjects2; // Max Number of Attempts
      Bool_t fHit = false;            // Hit Flag
      while((fHit==false) && (--ix != 0)) {
        idx1 = (int)(gRandom->Rndm()*((float)nObjects2));    
        idx2 = (int)(gRandom->Rndm()*((float)nObjects2)); 
        if( (idx1 != idx2) && !UsedPairP2P2(idx1,idx2) ) fHit = true;
      } 
      if (fHit) { SetUsedPairP2P2(idx1,idx2); 
         fSeeFlag = 0;
         *Obj1 = GetFromP2At(fR22x1 = idx1);
         *Obj2 = GetFromP2At(fR22x2 = idx2);
         fSeeFlag = 1; 
         return true; };
    }; // End of Guessing. If no Hit continue below:
    idx1 = 0; // Use RndOffset strategy
    idx2 = 0;
    int id = (int) (gRandom->Rndm()*((float)nF)); // Offset
    while(id >= 0) {
      if (!UsedPairP2P2(idx1,idx2)) id--;
      if (idx1<fMaxIdx2) {idx1++;} else {idx1=0; idx2++;}
    }
    if (idx1==0) {idx1=fMaxIdx2; idx2--;} else {idx1--;}
    SetUsedPairP2P2(idx1,idx2);
    fSeeFlag = 0;
    *Obj1 = GetFromP2At(fR22x1 = idx1);
    *Obj2 = GetFromP2At(fR22x2 = idx2);
    fSeeFlag = 1;
    return true;
  }
  else return false;
}
Bool_t    T49Mixer::GetNuRPairP1P2(TObject** Obj1,TObject** Obj2)
{
// Get random Not used pair of Objects from Pool1 and Pool2
//           and mark pair as Used
// Call by: GetNuRPairP1P2(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2  as TObject *Obj1,*Obj2

  Int_t nF = NofFreePairsP1P2();
  if (nF) { fLAC = fLACp = 23;         
    Int_t idx1 = 0;            
    Int_t idx2 = 0;
    if (nF>10) {  // Use Guessing strategy
      Long_t ix = nObjects1*nObjects2; // Max Number of Attempts
      Bool_t fHit = false;            // Hit Flag
      while((fHit==false) && (--ix != 0)) {
        idx1 = (int)(gRandom->Rndm()*((float)nObjects1)); 
        idx2 = (int)(gRandom->Rndm()*((float)nObjects2)); 
        if( !UsedPairP1P2(idx1,idx2) ) fHit = true;
      }  
      if (fHit) { SetUsedPairP1P2(idx1,idx2); 
         fSeeFlag = 0;  
         *Obj1 = GetFromP1At(fR12x1 = idx1);
         *Obj2 = GetFromP2At(fR12x2 = idx2);
         fSeeFlag = 1; 
         return true; };
    }             // End of Guessing
    idx1 = 0;     // If no success use RndOffset
    idx2 = 0;
    int id = (int) (gRandom->Rndm()*((float)nF));
    while(id >= 0) {
      if (!UsedPairP1P2(idx1,idx2)) id--;
      if (idx1<fMaxIdx1) {idx1++;} else {idx1=0; idx2++;}
    }
    if (idx1==0) {idx1=fMaxIdx1; idx2--;} else {idx1--;}
    SetUsedPairP1P2(idx1,idx2);
    fSeeFlag = 0;
    *Obj1 = GetFromP1At(fR12x1 = idx1);
    *Obj2 = GetFromP2At(fR12x2 = idx2);
    fSeeFlag = 1;
    return true;
  }
  else return false;
}
//////////////////////////////////////// Get Unique Pairs
Bool_t    T49Mixer::GetUniquePairP1P1(TObject** Obj1,TObject** Obj2)
{
// Get Unique pair of two Objects from Pool1
//           and mark Unique pair as Used
// Call by: GetUniquePairP1P1(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2 as TObject *Obj1,*Obj2

  Int_t nF = NofFreeUniquePairsP1P1();
  if (nF)
  { fLAC = fLACu = 31;
    Int_t idx1 = 0;
    Int_t idx2 = 0;
    while(UsedUniqueP1(idx1)) { idx1++; }
       SetUsedUniqueP1(idx1);
    while(UsedUniqueP1(idx2)) { idx2++; }
       SetUsedUniqueP1(idx2);
    SetUsedUniqueIDXP1(idx1,idx2); // Memory of idx1 <---> idx2 
    fSeeFlag = 0;
    *Obj1 = GetFromP1At(fU11x1 = idx1);
    *Obj2 = GetFromP1At(fU11x2 = idx2);
    fSeeFlag = 1;
    return true;
  }
  else return false;
}
Bool_t    T49Mixer::GetUniquePairP2P2(TObject** Obj1,TObject** Obj2)
{
// Get Unique pair of two Objects from Pool2
//           and mark Unique pair as Used
// Call by: GetUniquePairP2P2(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2 as TObject *Obj1,*Obj2

  Int_t nF = NofFreeUniquePairsP2P2();
  if (nF)
  { fLAC = fLACu = 32;
    Int_t idx1 = 0;
    Int_t idx2 = 0;
    while(UsedUniqueP2(idx1)) { idx1++; }
       SetUsedUniqueP2(idx1);
    while(UsedUniqueP2(idx2)) { idx2++; }
       SetUsedUniqueP2(idx2);
    SetUsedUniqueIDXP2(idx1,idx2); // Memory of idx1 <---> idx2
    fSeeFlag = 0;
    *Obj1 = GetFromP2At(fU22x1 = idx1);
    *Obj2 = GetFromP2At(fU22x2 = idx2);
    fSeeFlag = 1;
    return true;
  }
  else return false;
}
Bool_t    T49Mixer::GetUniquePairP1P2(TObject** Obj1,TObject** Obj2)
{
// Get Unique pair of Objects from Pool1 and Pool2
//           and mark Unique pair as Used
// Call by: GetUniquePairP1P2(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2 as TObject *Obj1,*Obj2

  Int_t nF = NofFreeUniquePairsP1P2();
  if (nF)
  { fLAC = fLACu = 33;
    Int_t idx1 = 0;
    Int_t idx2 = 0;
    while(UsedUniqueP1x2(idx1)) { idx1++; }
       SetUsedUniqueP1x2(idx1);
    while(UsedUniqueP2x1(idx2)) { idx2++; }
       SetUsedUniqueP2x1(idx2);
    SetUsedUniqueIDXP1x2(idx1,idx2);  // Memory of idx1 ---> idx2
    SetUsedUniqueIDXP2x1(idx2,idx1);  // Memory of idx1 <--- idx2
    fSeeFlag = 0;
    *Obj1 = GetFromP1At(fU12x1 = idx1);
    *Obj2 = GetFromP2At(fU12x2 = idx2);
    fSeeFlag = 1;
    return true;
  }
  else return false;
}
/////////////////////////////////////////// Get Random Unique Pair
Bool_t    T49Mixer::GetRUniquePairP1P1(TObject** Obj1,TObject** Obj2)
{
// Get random Unique pair of two Objects from Pool1
//           and mark Unique pair as Used
// Call by: GetRUniquePairP1P1(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2 as TObject *Obj1,*Obj2

  Int_t nF = NofFreeUniquePairsP1P1();
  if (nF)
  { fLAC = fLACu = 41;
    Int_t idx1 = 0;
    Int_t idx2 = 0;
    int id1 = (int) (gRandom->Rndm()*((float)nF*2.0));
    int id2 = (int) (gRandom->Rndm()*((float)(nF*2.0-1))); 
    while(id1 >= 0) { if(!UsedUniqueP1(idx1)) id1--; idx1++; }
       idx1--;
       SetUsedUniqueP1(idx1);
    while(id2 >= 0) { if(!UsedUniqueP1(idx2)) id2--; idx2++; }
       idx2--;
       SetUsedUniqueP1(idx2);
    SetUsedUniqueIDXP1(idx1,idx2); // Memory of idx1 <---> idx2
    fSeeFlag = 0;
    *Obj1 = GetFromP1At(fU11x1 = idx1);
    *Obj2 = GetFromP1At(fU11x2 = idx2);
    fSeeFlag = 1;
    return true;
  }
  else return false;
}
Bool_t    T49Mixer::GetRUniquePairP2P2(TObject** Obj1,TObject** Obj2)
{
// Get random Unique pair of two Objects from Pool2
//           and mark Unique pair as Used
// Call by: GetRUniquePairP2P2(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2 as TObject *Obj1,*Obj2

  Int_t nF = NofFreeUniquePairsP2P2();
  if (nF)
  { fLAC = fLACu = 42;
    Int_t idx1 = 0;
    Int_t idx2 = 0;
    int id1 = (int) (gRandom->Rndm()*((float)nF*2.0));
    int id2 = (int) (gRandom->Rndm()*((float)(nF*2.0-1)));
    while(id1 >= 0) { if(!UsedUniqueP2(idx1)) id1--; idx1++; }
       idx1--;
       SetUsedUniqueP2(idx1);
    while(id2 >= 0) { if(!UsedUniqueP2(idx2)) id2--; idx2++; }
       idx2--;
       SetUsedUniqueP2(idx2);
    SetUsedUniqueIDXP2(idx1,idx2); // Memory of idx1 <---> idx2
    fSeeFlag = 0;
    *Obj1 = GetFromP2At(fU22x1 = idx1);
    *Obj2 = GetFromP2At(fU22x2 = idx2);
    fSeeFlag = 1;
    return true;
  }
  else return false;
}
Bool_t    T49Mixer::GetRUniquePairP1P2(TObject** Obj1,TObject** Obj2)
{
// Get random Unique pair of Objects from Pool1 and Pool2
//           and mark Unique pair as Used
// Call by: GetRUniquePairP1P2(&Obj1, &Obj2)
//          since Obj1, Obj2 are set inside
// Declare Obj1,Obj2 as TObject *Obj1,*Obj2

  Int_t nF = NofFreeUniquePairsP1P2();
  if (nF)
  { fLAC = fLACu = 43;
    Int_t idx1 = 0;
    Int_t idx2 = 0;
    Int_t nF1 = NofFreeP1x2();
    Int_t nF2 = NofFreeP2x1();
    int id1 = (int) (gRandom->Rndm()*((float)nF1));
    int id2 = (int) (gRandom->Rndm()*((float)nF2));
    while(id1 >= 0) { if(!UsedUniqueP1x2(idx1)) id1--; idx1++; }
       idx1--;
       SetUsedUniqueP1x2(idx1);
    while(id2 >= 0) { if(!UsedUniqueP2x1(idx2)) id2--; idx2++; }
       idx2--;
       SetUsedUniqueP2x1(idx2);
    fSeeFlag = 0;
    SetUsedUniqueIDXP1x2(idx1,idx2); // Memory of idx1 <---> idx2
    SetUsedUniqueIDXP2x1(idx2,idx1); // Memory of idx1 <---> idx2
    *Obj1 = GetFromP1At(fU12x1 = idx1);
    *Obj2 = GetFromP2At(fU12x2 = idx2);
    fSeeFlag = 1;
    return true;
  }
  else return false;
}

//////////////////////////////////////////////////////////////// NofObjects
Int_t  T49Mixer::NofObjectsP1()  { return nObjects1; }
// Get number of Objects in Pool1
Int_t  T49Mixer::NofObjectsP2()  { return nObjects2; }
// Get number of Objects in Pool2
  
//////////////////////////////////////////////////////////////// NofUsed
Int_t  T49Mixer::NofUsedP1() { return (int) fUsed1->GetSum(); }
// Get number of Used Objects in Pool1
Int_t  T49Mixer::NofUsedP2() { return (nPools>1)?(int)fUsed2->GetSum():0;}
// Get number of Used Objects in Pool2

//////////////////////////////////////////////////////////////// NofFree
Int_t  T49Mixer::NofFreeP1() { return (nObjects1-NofUsedP1()); } 
// Get number of Free = Not Used Objects in Pool1
Int_t  T49Mixer::NofFreeP2() { return (nObjects2-NofUsedP2()); }
// Get number of Free = Not Used Objects in Pool2

///////////////////////////////////////////////////////////////  NofPairs
Int_t  T49Mixer::NofPairsP1P1() { return nObjects1*(nObjects1-1) / 2; }
// Get number of P1P1 pairs
Int_t  T49Mixer::NofPairsP2P2() { return nObjects2*(nObjects2-1) / 2; }
// Get number of P2P2 pairs
Int_t  T49Mixer::NofPairsP1P2() { return (nObjects1*nObjects2-fP1P2Mode*NofUniquePairsP1P2());}
// Get number of P1P2 pairs

///////////////////////////////////////////////////////////////  NofUsedPairs
Int_t  T49Mixer::NofUsedPairsP1P1() { return nUsedP1P1; }
// Get number of Used P1P1 pairs
Int_t  T49Mixer::NofUsedPairsP2P2() { return nUsedP2P2; }
// Get number of Used P2P2 pairs
Int_t  T49Mixer::NofUsedPairsP1P2() { return nUsedP1P2; }
// Get number of Used P1P2 pairs

///////////////////////////////////////////////////////////////  NofFreePairs
Int_t  T49Mixer::NofFreePairsP1P1() { return NofPairsP1P1()-NofUsedPairsP1P1(); }
// Get number of Free = NotUsed P1P1 pairs
Int_t  T49Mixer::NofFreePairsP2P2() { return NofPairsP2P2()-NofUsedPairsP2P2(); }
// Get number of Free = NotUsed P2P2 pairs
Int_t  T49Mixer::NofFreePairsP1P2() { return NofPairsP1P2()-NofUsedPairsP1P2(); }
// Get number of Free = NotUsed P1P2 pairs

///////////////////////////////////////////////////////////////  NofUniquePairs
Int_t  T49Mixer::NofUniquePairsP1P1() { return nObjects1 / 2; }
// Get number of Unique P1P1 pairs
Int_t  T49Mixer::NofUniquePairsP2P2() { return nObjects2 / 2; }
// Get number of Unique P2P2 pairs
Int_t  T49Mixer::NofUniquePairsP1P2() { return ((nObjects1<nObjects2)? nObjects1 : nObjects2); }
// Get number of Unique P1P2 pairs

///////////////////////////////////////////////////////////////  NofUsedUniquePairs
Int_t  T49Mixer::NofUsedUniquePairsP1P1() { return nUsedUniP1P1 / 2; }
// Get number of Used Unique P1P1 pairs
Int_t  T49Mixer::NofUsedUniquePairsP2P2() { return nUsedUniP2P2 / 2; }
// Get number of Used Unique P2P2 pairs
Int_t  T49Mixer::NofUsedUniquePairsP1P2() { return (nUsedUniP1P2+nUsedUniP2P1) / 2 ; }
// Get number of Used Unique P1P2 pairs

///////////////////////////////////////////////////////////////  NofFreeUniquePairs
Int_t  T49Mixer::NofFreeUniquePairsP1P1() { return NofUniquePairsP1P1()-NofUsedUniquePairsP1P1(); }
// Get number of Free Unique P1P1 pairs
Int_t  T49Mixer::NofFreeUniquePairsP2P2() { return NofUniquePairsP2P2()-NofUsedUniquePairsP2P2(); }
// Get number of Free Unique P2P2 pairs
Int_t  T49Mixer::NofFreeUniquePairsP1P2() { return NofUniquePairsP1P2()-NofUsedUniquePairsP1P2(); }
// Get number of Free Unique P1P2 pairs
Int_t  T49Mixer::NofFreeP1x2() { return nObjects1-nUsedUniP1P2;}
// Internal routine
Int_t  T49Mixer::NofFreeP2x1() { return nObjects2-nUsedUniP2P1;}
// Internal routine

/////////////////////////////////////////////////////////////// Obj:  Used, SetUsed, SetNUsed
Bool_t T49Mixer::UsedOneP1(Int_t idx)  { return fUsed1[0][idx];}
// Return Used/Nused status of Object in Pool1 at position idx
void   T49Mixer::SetUsedOneP1(Int_t idx)  { (*fUsed1)[idx] = fSeeFlag; }
// Mark Object in Pool1 at position idx as Used
void   T49Mixer::SetNUsedOneP1(Int_t idx) { (*fUsed1)[idx] = 0; }
// Mark Object in Pool1 at position idx as NotUsed
Bool_t T49Mixer::UsedOneP2(Int_t idx)  { return (*fUsed2)[idx];}
// Return Used/Nused status of Object in Pool1 at position idx
void   T49Mixer::SetUsedOneP2(Int_t idx)  { (*fUsed2)[idx] = fSeeFlag; }
// Mark Object in Pool1 at position idx as Used
void   T49Mixer::SetNUsedOneP2(Int_t idx) { (*fUsed2)[idx] = 0; }
// Mark Object in Pool1 at position idx as NotUsed

/////////////////////////////////////////////////////////////// Pair: Used, SetUsed, SetNUsed
Bool_t T49Mixer::UsedPairP1P1(Int_t idx1, Int_t idx2)
{ 
// Return Used/Nused status of P1P1 pair (idx1,idx2) 
  Int_t idx;
  if (idx1<=idx2) { idx = idx1*(nMaxMXX)+idx2; }
           else   { idx = idx2*(nMaxMXX)+idx1; }
  return ( ((*fMaskXX)[idx/32] & fx010[idx%32]) == 0 ) ? false : true ; 
}
Bool_t T49Mixer::UsedPairP1P1(Int_t idx)          
{
// Internal routine
  return ( ((*fMaskXX)[idx/32] & fx010[idx%32]) == 0 ) ? false : true ; 
}
void   T49Mixer::SetUsedPairP1P1(Int_t idx1, Int_t idx2)
{ 
// Mark P1P1 pair (idx1,idx2) as Used
  if (fSeeFlag)
  { Int_t idx;
    if (idx1<=idx2) { idx = idx1*(nMaxMXX)+idx2; }
           else     { idx = idx2*(nMaxMXX)+idx1; }
    if (!UsedPairP1P1(idx)) { 
      nUsedP1P1++;
    (*fMaskXX)[idx/32] |= fx010[idx%32]; 
  } }
}
void   T49Mixer::SetNUsedPairP1P1(Int_t idx1, Int_t idx2)
{ 
// Mark P1P1 pair (idx1,idx2) as NotUsed
  Int_t idx;
  if (idx1<=idx2) { idx = idx1*(nMaxMXX)+idx2; }
           else   { idx = idx2*(nMaxMXX)+idx1; }
  if (UsedPairP1P1(idx)) { 
    nUsedP1P1--;
    (*fMaskXX)[idx/32] &= fx101[idx%32]; 
  }
}
Bool_t T49Mixer::UsedPairP2P2(Int_t idx1, Int_t idx2)
{ 
// Return Used/Nused status of P2P2 pair (idx1,idx2)
  Int_t idx;
  if (idx1<=idx2) { idx = idx2*(nMaxMXX)+idx1; }
           else   { idx = idx1*(nMaxMXX)+idx2; }
  return ( ((*fMaskXX)[idx/32] & fx010[idx%32]) == 0 ) ? false : true ;   
}
Bool_t T49Mixer::UsedPairP2P2(Int_t idx)    
{ 
// Internal routine
  return ( ((*fMaskXX)[idx/32] & fx010[idx%32]) == 0 ) ? false : true ; 
}
void   T49Mixer::SetUsedPairP2P2(Int_t idx1, Int_t idx2)
{ 
// Mark P2P2 pair (idx1,idx2) as Used
  if (fSeeFlag)
  { Int_t idx;
    if (idx1<=idx2) { idx = idx2*(nMaxMXX)+idx1; }
           else     { idx = idx1*(nMaxMXX)+idx2; }
    if (!UsedPairP2P2(idx)) {
      nUsedP2P2++;
      (*fMaskXX)[idx/32] |= fx010[idx%32]; 
  } }
}
void   T49Mixer::SetNUsedPairP2P2(Int_t idx1, Int_t idx2)
{ 
// Mark P2P2 pair (idx1,idx2) as NotUsed
  Int_t idx;
  if (idx1<=idx2) { idx = idx2*(nMaxMXX)+idx1; }
           else   { idx = idx1*(nMaxMXX)+idx2; }
  if (UsedPairP2P2(idx)) { 
    nUsedP2P2--;
    (*fMaskXX)[idx/32] &= fx101[idx%32];
  } 
}
Bool_t T49Mixer::UsedPairP1P2(Int_t idx1, Int_t idx2)
{ 
// Return Used/Nused status of P1P2 pair (idx1,idx2)
  Int_t idx;
  idx = idx2*(nMaxMXY)+idx1;
  return ( ((*fMaskXY)[idx/32] & fx010[idx%32]) == 0) ? false : true ;  
}
Bool_t T49Mixer::UsedPairP1P2(Int_t idx)        
{
// Internal routine 
  return ( ((*fMaskXY)[idx/32] & fx010[idx%32]) == 0) ? false : true ; 
}
void   T49Mixer::SetUsedPairP1P2(Int_t idx1, Int_t idx2)
{ 
// Mark P1P2 pair (idx1,idx2) as Used
  if (fSeeFlag)
  { Int_t idx;
    idx = idx2*(nMaxMXY)+idx1;
    if (!UsedPairP1P2(idx)) { 
      nUsedP1P2++;
      (*fMaskXY)[idx/32] |= fx010[idx%32]; 
  } } 
}
void   T49Mixer::SetNUsedPairP1P2(Int_t idx1, Int_t idx2)
{ 
// Mark P1P2 pair (idx1,idx2) as NotUsed
  Int_t idx;
  idx = idx2*(nMaxMXY)+idx1;
  if (UsedPairP1P2(idx)) {
    nUsedP1P2--;
    (*fMaskXY)[idx/32] &= fx101[idx%32]; 
  }
}
/////////////////////////////////////////////////////////////// Unique Pair: Used, SetUsed, SetNUsed
Bool_t T49Mixer::UsedUniqueP1(Int_t idx1)
{
// Return UniqueP1P1 Used/Nused status of TObject in Pool1 at (idx1)
  return (*fUniq11)[idx1];
}
void   T49Mixer::SetUsedUniqueP1(Int_t idx1)
{
// Mark TObject in Pool1 at (idx1) as Used for UniqueP1P1 pairs
  if (!UsedUniqueP1(idx1)) nUsedUniP1P1++;
  (*fUniq11)[idx1] = 1; 
}
void   T49Mixer::SetNUsedUniqueP1(Int_t idx1)
{
// Mark TObject in Pool1 at (idx1) as NotUsed for UniqueP1P1 pairs
  if (UsedUniqueP1(idx1)) nUsedUniP1P1--;
  (*fUniq11)[idx1] = 0; 
}
Bool_t T49Mixer::UsedUniqueP2(Int_t idx1)
{
// Return UniqueP2P2 Used/Nused status of TObject in Pool2 at (idx1)
  return (*fUniq22)[idx1];
}
void   T49Mixer::SetUsedUniqueP2(Int_t idx1)
{
// Mark TObject in Pool2 at (idx1) as Used for Unique P2P2 pairs
  if (!UsedUniqueP2(idx1)) nUsedUniP2P2++;
  (*fUniq22)[idx1] = 1;
}
void   T49Mixer::SetNUsedUniqueP2(Int_t idx1)
{
// Mark TObject in Pool2 at (idx1) as NotUsed for UniqueP2P2 pairs
  if (UsedUniqueP2(idx1)) nUsedUniP2P2--;
  (*fUniq22)[idx1] = 0; 
}
Bool_t T49Mixer::UsedUniqueP1x2(Int_t idx1)
{
// Return UniqueP1P2 Used/Nused status of TObject in Pool1 at (idx1)
  return (*fUniq12)[idx1];
}
void   T49Mixer::SetUsedUniqueP1x2(Int_t idx1)
{
// Mark TObject in Pool1 at (idx1) as Used for Unique P1P2 pairs
  if (!UsedUniqueP1x2(idx1)) nUsedUniP1P2++;
  (*fUniq12)[idx1] = 1;
}
void   T49Mixer::SetNUsedUniqueP1x2(Int_t idx1)
{
// Mark TObject in Pool1 at (idx1) as NotUsed for Unique P1P2 pairs
  if (UsedUniqueP1x2(idx1)) nUsedUniP1P2--;
  (*fUniq12)[idx1] = 0;
}
Bool_t T49Mixer::UsedUniqueP2x1(Int_t idx2)
{
// Return UniqueP1P2 Used/Nused status of TObject in Pool2 at (idx2)
  return (*fUniq21)[idx2];
}
void   T49Mixer::SetUsedUniqueP2x1(Int_t idx2)
{
// Mark TObject in Pool2 at (idx2) as Used for Unique P1P2 pairs
  if (!UsedUniqueP2x1(idx2)) nUsedUniP2P1++;
  (*fUniq21)[idx2] = 1;
}
void   T49Mixer::SetNUsedUniqueP2x1(Int_t idx2)
{
// Mark TObject in Pool2 at (idx2) as NotUsed for Unique P1P2 pairs
  if (UsedUniqueP2x1(idx2)) nUsedUniP2P1--;
  (*fUniq21)[idx2] = 0;
}
void   T49Mixer::SetUsedUniqueIDXP1(Int_t Idx1, Int_t Idx2)
{  
// Internal routine
     (*fUniq11)[Idx2] = Idx1+1;
     (*fUniq11)[Idx1] = Idx2+1; 
}
Int_t  T49Mixer::GetUsedUniqueIDXP1(Int_t idx)
{ 
// Internal routine
  return (*fUniq11)[idx]-1; 
}
void   T49Mixer::SetUsedUniqueIDXP2(Int_t Idx1, Int_t Idx2)
{ 
// Internal routine
     (*fUniq22)[Idx2] = Idx1+1;
     (*fUniq22)[Idx1] = Idx2+1; 
}
Int_t  T49Mixer::GetUsedUniqueIDXP2(Int_t idx)
{ 
// Internal routine
  return (*fUniq22)[idx]-1; 
} 
void   T49Mixer::SetUsedUniqueIDXP1x2(Int_t Idx1, Int_t Idx2)
{ 
// Internal routine 
     (*fUniq12)[Idx1] = Idx2+1;
}
Int_t  T49Mixer::GetUsedUniqueIDXP1x2(Int_t idx)
{ 
// Internal routine
  return (*fUniq12)[idx]-1; } 

void   T49Mixer::SetUsedUniqueIDXP2x1(Int_t Idx1, Int_t Idx2)
{ 
// Internal routine 
     (*fUniq21)[Idx1] = Idx2+1;
}
Int_t  T49Mixer::GetUsedUniqueIDXP2x1(Int_t idx)
{ 
// Internal routine
  return (*fUniq21)[idx]-1;
} 
////////////////////////////////////////////////// SetNUsedAfterReplaceP1 (P2)
void   T49Mixer::SetNUsedAfterReplaceP1(Int_t idx)
{ 
// Internal routine
  SetNUsedOneP1(idx);
  if(nPools==2 && idx<nMaxMXY) {
    for(int i=0; i<nMaxMXY; i++){ if(i!=idx) SetNUsedPairP1P2(idx,i); }
    if (fP1P2Mode==0) SetNUsedPairP1P2(idx,idx);
    if(UsedUniqueP1(idx))   {SetNUsedUniqueP1(GetUsedUniqueIDXP1(idx)); SetNUsedUniqueP1(idx);}
    if(UsedUniqueP1x2(idx)) {SetNUsedUniqueP2x1(GetUsedUniqueIDXP1x2(idx)); SetNUsedUniqueP1x2(idx);}
  }
  for(int i=0; i<nObjects1; i++) { if (idx != i) SetNUsedPairP1P1(idx,i); }
  fL11x1 = 0; fL11x2 = 0;
  fL12x1 = 0; fL12x2 = 0;
  fL1=0;
}
void   T49Mixer::SetNUsedAfterReplaceP2(Int_t idx)
{
// Internal routine
  SetNUsedOneP2(idx);
  if(nPools==2 && idx<nMaxMXY) {
    for(int i=0; i<nMaxMXY; i++) { if(i!=idx) SetNUsedPairP1P2(i,idx); }
    if (fP1P2Mode==0) SetNUsedPairP1P2(idx,idx);
    if(UsedUniqueP2(idx))   {SetNUsedUniqueP2(GetUsedUniqueIDXP2(idx)); SetNUsedUniqueP2(idx);}
    if(UsedUniqueP2x1(idx)) {SetNUsedUniqueP1x2(GetUsedUniqueIDXP2x1(idx)); SetNUsedUniqueP2x1(idx);}
  }
  for(int i=0; i<nObjects2; i++) { if (idx != i) SetNUsedPairP2P2(i,idx); }
  fL22x1 = 0; fL22x2 = 0;
  fL12x1 = 0; fL12x2 = 0;
  fL2=0;
}
////////////////////////////////////////////////// Initialization called from constructors
void T49Mixer::InitRnd()
{
// Initialize and randomize generator
  UInt_t Seed=0;
  gRandom->SetSeed(Seed);
  gRandom->Rndm();
}
void T49Mixer::InitStyle()
{
// Set TStyle for GUI
  Int_t NC; NC = TS.GetNumberOfColors();
  Int_t Cols[256];
  for(int i=0;i<NC;i++) Cols[i]=TS.GetColorPalette(i);
  Cols[NC-1]=3; TS.SetPalette(NC,Cols);
  TS.SetFillColor(11);
}
void T49Mixer::Init101()
{
// Set internal bit masks for AND/OR 32bit operations
  fx010[0] = 0x80000000; fx010[1] = 0x40000000; fx010[2] = 0x20000000; fx010[3] = 0x10000000;
  fx010[4] = 0x8000000;  fx010[5] = 0x4000000;  fx010[6] = 0x2000000;  fx010[7] = 0x1000000;
  fx010[8] = 0x800000;   fx010[9] = 0x400000;   fx010[10]= 0x200000;   fx010[11]= 0x100000;
  fx010[12]= 0x80000;    fx010[13]= 0x40000;    fx010[14]= 0x20000;    fx010[15]= 0x10000;
  fx010[16]= 0x8000;     fx010[17]= 0x4000;     fx010[18]= 0x2000;     fx010[19]= 0x1000;
  fx010[20]= 0x800;      fx010[21]= 0x400;      fx010[22]= 0x200;      fx010[23]= 0x100;
  fx010[24]= 0x80;       fx010[25]= 0x40;       fx010[26]= 0x20;       fx010[27]= 0x10;
  fx010[28]= 0x8;        fx010[29]= 0x4;        fx010[30]= 0x2;        fx010[31]= 0x1;

  fx101[0] = 0x7FFFFFFF; fx101[1] = 0xBFFFFFFF; fx101[2] = 0xDFFFFFFF; fx101[3] = 0xEFFFFFFF;
  fx101[4] = 0xF7FFFFFF; fx101[5] = 0xFBFFFFFF; fx101[6] = 0xFDFFFFFF; fx101[7] = 0xFEFFFFFF;
  fx101[8] = 0xFF7FFFFF; fx101[9] = 0xFFBFFFFF; fx101[10]= 0xFFDFFFFF; fx101[11]= 0xFFEFFFFF;
  fx101[12]= 0xFFF7FFFF; fx101[13]= 0xFFFBFFFF; fx101[14]= 0xFFFDFFFF; fx101[15]= 0xFFFEFFFF;
  fx101[16]= 0xFFFF7FFF; fx101[17]= 0xFFFFBFFF; fx101[18]= 0xFFFFDFFF; fx101[19]= 0xFFFFEFFF;
  fx101[20]= 0xFFFFF7FF; fx101[21]= 0xFFFFFBFF; fx101[22]= 0xFFFFFDFF; fx101[23]= 0xFFFFFEFF;
  fx101[24]= 0xFFFFFF7F; fx101[25]= 0xFFFFFFBF; fx101[26]= 0xFFFFFFDF; fx101[27]= 0xFFFFFFEF;
  fx101[28]= 0xFFFFFFF7; fx101[29]= 0xFFFFFFFB; fx101[30]= 0xFFFFFFFD; fx101[31]= 0xFFFFFFFE;
}
//////////////////////////////////////////////////////////////////////// LAC module
Bool_t  T49Mixer::GetLastUniP1P1(Int_t &Idx1,Int_t &Idx2)
{
// Get (idx1,idx2) of Last generated Unique P1P1 pair
// Call: GetLastUniP1P1(Idx1,Idx2); // Int_t Idx1,Idx2;
  if(fLACu==0) { return false;
  } else {
    Idx1 = fU11x1; 
    Idx2 = fU11x2;
    return true;
  }
}
Bool_t  T49Mixer::GetLastUniP1P2(Int_t &Idx1,Int_t &Idx2)
{
// Get (idx1,idx2) of Last generated Unique P1P2 pair
// Call: GetLastUniP1P2(Idx1,Idx2); // Int_t Idx1,Idx2;
  if(fLACu==0) { return false;
  } else {
    Idx1 = fU12x1; 
    Idx2 = fU12x2; 
    return true; 
  }
}
Bool_t  T49Mixer::GetLastUniP2P2(Int_t &Idx1,Int_t &Idx2)
{
// Get (idx1,idx2) of Last generated Unique P2P2 pair
// Call: GetLastUniP2P2(Idx1,Idx2); // Int_t Idx1,Idx2;
  if(fLACu==0) { return false;
  } else { 
    Idx1 = fU22x1;
    Idx2 = fU22x2;
    return true;
  }
}
Bool_t  T49Mixer::GetLastP1P1(Int_t &Idx1,Int_t &Idx2)
{
// Get (idx1,idx2) of Last generated NotUsed P1P1 pair
// Call: GetLastP1P1(Idx1,Idx2); // Int_t Idx1,Idx2;
  switch(fLACp)
  {
       case 11: Idx1 = fL11x1; Idx2 = fL11x2; break;
       case 21: Idx1 = fR11x1; Idx2 = fR11x2; break;
       default: return false; 
  }
  return true;
}
Bool_t  T49Mixer::GetLastP2P2(Int_t &Idx1,Int_t &Idx2)
{
// Get (idx1,idx2) of Last generated NotUsed P2P2 pair
// Call: GetLastP2P2(Idx1,Idx2); // Int_t Idx1,Idx2;
  switch(fLACp)
  {
       case 12: Idx1 = fL22x1; Idx2 = fL22x2; break;
       case 22: Idx1 = fR22x1; Idx2 = fR22x2; break;
       default: return false;
  }
  return true;
}
Bool_t  T49Mixer::GetLastP1P2(Int_t &Idx1,Int_t &Idx2)
{
// Get (idx1,idx2) of Last generated NotUsed P1P2 pair
// Call: GetLastP1P2(Idx1,Idx2); // Int_t Idx1,Idx2;
  switch(fLACp)
  {
       case 13: Idx1 = fL12x1; Idx2 = fL12x2; break;
       case 23: Idx1 = fR12x1; Idx2 = fR12x2; break;
       default: return false;
  }
  return true;
}

Char_t* T49Mixer::GetLACName(Int_t LA)
{
// Return string name of last operation with code LA
  switch(LA)
  {  
    case  0: sprintf(fLACName," ");                   break;

    case  7: sprintf(fLACName,"GetFromP1");           break;
    case  8: sprintf(fLACName,"GetFromP2");           break;

    case 11: sprintf(fLACName,"GetNuPairP1P1");       break;
    case 12: sprintf(fLACName,"GetNuPairP2P2");       break;
    case 13: sprintf(fLACName,"GetNuPairP1P2");       break;

    case 17: sprintf(fLACName,"GetNuOneP1");          break;
    case 18: sprintf(fLACName,"GetNuOneP2");          break;

    case 21: sprintf(fLACName,"GetNuRPairP1P1");      break;
    case 22: sprintf(fLACName,"GetNuRPairP2P2");      break;
    case 23: sprintf(fLACName,"GetNuRPairP1P2");      break;

    case 27: sprintf(fLACName,"GetNuROneP1");         break;
    case 28: sprintf(fLACName,"GetNuROneP2");         break;

    case 31: sprintf(fLACName,"GetUniquePairP1P1");   break;
    case 32: sprintf(fLACName,"GetUniquePairP2P2");   break;
    case 33: sprintf(fLACName,"GetUniquePairP1P2");   break;

    case 41: sprintf(fLACName,"GetRUniquePairP1P1");  break;
    case 42: sprintf(fLACName,"GetRUniquePairP2P2");  break;
    case 43: sprintf(fLACName,"GetRUniquePairP1P2");  break;

    case 51: sprintf(fLACName,"SetPool1");            break;
    case 52: sprintf(fLACName,"Put/AddToP1");         break;
    case 53: sprintf(fLACName,"Put/ReplaceInP1");     break;
    case 54: sprintf(fLACName,"PurgePool1");          break;
    case 55: sprintf(fLACName,"ClearPool1");          break;

    case 61: sprintf(fLACName,"SetPool2");            break;
    case 62: sprintf(fLACName,"Put/AddToP2");         break;
    case 63: sprintf(fLACName,"Put/ReplaceInP2");     break;
    case 64: sprintf(fLACName,"PurgePool2");          break;
    case 65: sprintf(fLACName,"ClearPool2");          break;

    case 76: sprintf(fLACName,"SetPools");            break;
    case 77: sprintf(fLACName,"ClearPools");          break;

    default: sprintf(fLACName,"Unknown");
  }
  return fLACName;
}
