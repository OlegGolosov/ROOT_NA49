/////////////////////////////////////////////////////////
//  Mixing tool 
/////////////////////////  Author R.Ganz April 99 ///////
//
//  You can store lists of Particles (TObjArrays) in this
//  mixer. It will organize the memory, and tell you
//  when it has collected enough statisic to produce
//  mixed events.
//  Optional you can have one or two lists(default) of
//  particles stored
/////////////////////////////////////////////////////////  

#include "T49Mixing.h"

ClassImp(T49Mixing)

/*
$Log: T49Mixing.C,v $
Revision 1.5  2002/03/27 14:07:22  cblume
Change deletion of T49ParticleRoot

Revision 1.4  2001/11/12 12:11:46  cblume
Update for ROOT v3.01

 * Revision 1.3  1999/11/23  13:54:08  cblume
 * Remove files
 *
*/

T49Mixing::T49Mixing()
{
  Reset();
}

T49Mixing::T49Mixing(Int_t n, Int_t nl)
{
  ////////////////////////////////////////
  //  use this constructor to initialize the mixer
  //   n    number of events to be stored
  //   nl   number of lists to be kept (default 2)
  ////////////////////////////////////////////
  Reset();
  nMaxEvents = n;
  fNumList =nl;
  fPart1Pool = new TObjArray(n);
  fPart1List = new TObjArray();
  fPart2List = new TObjArray();
  if (fNumList>1)
    {
      fPart2Pool = new TObjArray(n);
    };
}


void T49Mixing::Reset()
{
  nEvent=0;
  nCurrent=0;
  fIsReadyFlag=0;
  fVerbose=0;
  nMaxEvents = 0;
  fPart1Pool = NULL;
  fPart2Pool = NULL;
  fPart1List = NULL;
  fPart2List = NULL;
  fNumList =2;
}

void T49Mixing::AddEvent(TObjArray *P1list, TObjArray *P2list)
{
  ///////////////////////////////////////////
  // Add one (2nd left empty) or two lists to the mixer
  // If initialize number of events is eceeded
  // ReadyFlag is set and first(oldest) list(event)
  // will be removed
  ///////////////////////////////////////////// 
  

  Int_t I;
  T49ParticleRoot *Part,*PartHelp;;
 
// delete first part lists
  if (fIsReadyFlag==1)  
    {
      TObject *PartH =  (TObject*)fPart1Pool->At(0);
      ((TObjArray*)PartH)->Delete();
      fPart1Pool->RemoveAt(0);
      delete PartH; 
      fPart1Pool->Compress();
      if (fVerbose) {printf(" <I> T49Mixing: Deleted Part 1 list \n");};
    };

  if (fIsReadyFlag==1 && fNumList==2)  
    {
      TObject *PartH =  (TObject*)fPart2Pool->At(0);
      ((TObjArray*)PartH)->Delete();
      fPart2Pool->RemoveAt(0);
      delete PartH; 
      fPart2Pool->Compress();
      if (fVerbose) {printf(" <I> T49Mixing: Deleted Part 2 list \n");};
    };

// add new  part lists at the end

  TObjArray* a = new TObjArray();
  for (I=0; I< P1list->GetEntries(); I++)
    {
      Part    = new T49ParticleRoot();
      PartHelp =  (T49ParticleRoot*)P1list->At(I);
      Part->CopyParticle(PartHelp);
      a->Add(Part);
    }
  fPart1Pool->Add(a);  

  if (fNumList==2)
    { 
      TObjArray* b = new TObjArray();
      for (I=0; I< P2list->GetEntries() ;I++)
	{
	  Part    = new T49ParticleRoot();
	  PartHelp =  (T49ParticleRoot*)P2list->At(I);
	  Part->CopyParticle(PartHelp);
	  b->Add(Part);
	}
      fPart2Pool->Add(b);
    };
  nEvent++;

  if (nEvent == nMaxEvents)  
    { 
      fIsReadyFlag=1;
      if (fVerbose) 
	{printf(" <I> T49Mixing is ready now \n");};
    };
  if (nCurrent<nMaxEvents)  { nCurrent++;}; 
  if (fVerbose) 
    {printf(" <I> T49Mixing: Event %d   Current %d added  \n",nEvent,nCurrent);};
}


  void T49Mixing::GetEvent(Int_t Evt1, Int_t Evt2)
{ 
  ////////////////////////////////////////////////////
  // Sets particle List 1 and 2 to lists of
  // Evt1 and Evt2 respectivly
  //
  // Eventmixing mode:
  // If Evt1<0 Event Number will be assigned randomly to Evt1
  // and Part1List is assigned accordingly    
  // If Evt2<0 Event Number will be assigned randomly to Evt2
  // and Part2List is assigned accordingly    
  /////////////////////////////////////////////////////////

  fEvent1 = Evt1;
  fEvent2 = Evt2;

  if (fEvent1 < 0) {
    while  (!CheckBound(fEvent1))   
      {fEvent1 = (int)(gRandom->Rndm(nEvent)*((float)nCurrent));};
  }

  if (fEvent2 < 0) {
    while  (!CheckBound(fEvent2) || (fEvent1==fEvent2))
      {fEvent2 = (int)(gRandom->Rndm(nEvent)*((float)nCurrent));};
  }

  if (fVerbose) 
    {printf(" <I> T49Mixing: Mixed Event %d  and %d   \n",fEvent1,fEvent2);};   

}

TObjArray* T49Mixing::GetPart1List()
{ ////////////////////////////////////////////////
  // Retrieve list 1 from event #Evt1 assigned in GetEvent
  ////////////////////////////////////////////////
  T49ParticleRoot *track;
  if (fPart1List) fPart1List->Clear();
  
  TIter NextParticle((TObjArray*)fPart1Pool->At(fEvent1));
  while( (track = (T49ParticleRoot *) NextParticle()) != 0)
    {
      fPart1List->Add(track);            
    }
  
  if(fVerbose)
    {
      printf(" <I> Mixing: Returns Part1List with %d particles. \n", fPart1List->GetEntries());
    }
  
  return fPart1List;
}

TObjArray* T49Mixing::GetPart2List()
{ ////////////////////////////////////////////////
  // Retrieve list 2  from event#Evt2 assigned in GetEvent
  ////////////////////////////////////////////////

  T49ParticleRoot *track;
  if (fPart2List) fPart2List->Clear();
  
  if (fNumList==2)
    {
      TIter NextParticle((TObjArray*)fPart2Pool->At(fEvent2));
      while( (track = (T49ParticleRoot *) NextParticle()) != 0)
	{
	  fPart2List->Add(track);            
	}
    }
  else
    {
      TIter NextParticle((TObjArray*)fPart1Pool->At(fEvent2));      
      while( (track = (T49ParticleRoot *) NextParticle()) != 0)
	{
	  fPart2List->Add(track);            
	}
    }
  if(fVerbose)
    {
      printf(" <I> Mixing: Returns Part2List with %d particles. \n", fPart2List->GetEntries());
    }
  return fPart2List;
}


