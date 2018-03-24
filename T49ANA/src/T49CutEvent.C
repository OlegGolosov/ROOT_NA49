//////////////////////////////////////////////////////////
//                                                      //
//              T49CutEvent Class                       //
//                                                      //
//  Cut on events in T49EventRoot format                //
//                                                      //
//  UpDate: 04.02.2000                                  //
//                                                      //
//  c.markert@gsi.de                                    //
//                                                      //
//////////////////////////////////////////////////////////

#include <TPaveText.h>

#include "T49EventRoot.h"
#include "T49VertexRoot.h"

#include "T49CutEvent.h"

ClassImp(T49CutEvent)

/*
$Log: T49CutEvent.C,v $
 * Revision 1.5  2000/11/09  09:05:17  cblume
 * Inlcuded necessary header files
 *
 * Revision 1.4  2000/02/04  10:56:28  cblume
 * Change in standard vertex-z cut for PbPb
 *
 * Revision 1.3  1999/11/23  13:54:01  cblume
 * Remove files
 *
*/

//______________________________________________________________________________
T49CutEvent::T49CutEvent():T49Cut()
{ 
  //
  // Default constructor
  //

  Reset();

}

//______________________________________________________________________________
T49CutEvent::T49CutEvent(const Text_t *name, const Text_t *title)
            :T49Cut(name,title)
{ 
  //
  // Default constructor with name and title
  //

  Reset();

}

//______________________________________________________________________________
void T49CutEvent::Reset()
{
  //
  // Resets the cut definition and the counters
  //

    fnumevent     = 0;     
    fnumgoodevent = 0; 

    fFlagVertexIflag = kFALSE;
    fFlagVertexX     = kFALSE;
    fFlagVertexY     = kFALSE;
    fFlagVertexZ     = kFALSE;
    fFlagEVeto       = kFALSE;
    fFlagMult        = kFALSE;

    fnVertexIflag = 0;
    fnVertexX     = 0;
    fnVertexY     = 0;
    fnVertexZ     = 0;
    fnEVeto       = 0;
    fnMult        = 0;

}

//______________________________________________________________________________
void T49CutEvent::SetStandardCutsPP()
{   
  //
  // Sets the standard cuts for p+p events: 
  //          Iflag = 0 or 4 
  //           -1 < vertex-x <    1
  //           -1 < vertex-y <    1
  //         -585 < vertex-z < -574
  //

    SetVertexIflag(0,4);
    SetVertexX(-1,1);
    SetVertexY(-1,1);
    SetVertexZ(-585,-574);  
}

//______________________________________________________________________________
void T49CutEvent::SetStandardCutsPPb()
{   
  //
  // Sets the standard cuts for p+Pb events:
  //          Iflag = 0, 1 or 4
  //           -1 < vertex-x <    1
  //           -1 < vertex-y <    1
  //         -583 < vertex-z < -577
  //

    SetVertexIflag(0,1,4);
    SetVertexX(-1,1);
    SetVertexY(-1,1);
    SetVertexZ(-583,-577);  
}

//______________________________________________________________________________
void T49CutEvent::SetStandardCutsPbPb()
{   
  //
  // Sets the standard cuts for central Pb+Pb events:
  //          Iflag = 0
  //           -1 < vertex-x <    1
  //           -1 < vertex-y <    1
  //         -580 < vertex-z < -578   04.02.2000 (mvl) changed from -581 < vertex-z < -579 
  //
 
    SetVertexIflag(0);
    SetVertexX(-1,1);
    SetVertexY(-1,1);
    SetVertexZ(-580,-578);  
}

//______________________________________________________________________________
void T49CutEvent::SetStandardCutsPbPbMB()
{   
  //
  // Sets the standard cuts for minimum bias Pb+Pb events:
  //          Iflag = 0
  //           -1 < vertex-x <    1
  //           -1 < vertex-y <    1
  //         -585 < vertex-z < -575
  //

    SetVertexIflag(0);
    SetVertexX(-1,1);
    SetVertexY(-1,1);
    SetVertexZ(-585,-575);  
}

//______________________________________________________________________________
Int_t T49CutEvent::CheckEvent(T49EventRoot *Event)
{
  //
  // Checks whether a given event passes the defined cuts  
  //

    TObjArray     *VtxList;
    T49VertexRoot *vtx;

    fnumevent++;

    if(fFlagMult && (Event->GetNParticles() < fMinMult || Event->GetNParticles() > fMaxMult) )
    {
      if(fVerbose)
	printf("Event not accepted  Mult %d. \n",Event->GetNParticles());
      fnMult++;
      return kFALSE;
    } 
  
    if(fFlagVertexIflag)
    {
       VtxList = Event->GetVertices();
       if(VtxList->GetEntries() > 0)  // number of vertices > 0
       {
         vtx = (T49VertexRoot *) VtxList->First();                 
         if(vtx->GetIflag() != fN1VertexIflag && 
            vtx->GetIflag() != fN2VertexIflag && 
            vtx->GetIflag() != fN3VertexIflag)
         {
           if(fVerbose)
             printf("Event not accepted VertexIflag = %d \n",vtx->GetIflag());
           fnVertexIflag++;
           return kFALSE;
         }
      }
    }
    if(fFlagVertexX && (Event->GetVertexX() < fMinVertexX || Event->GetVertexX() > fMaxVertexX) )
    {
      if(fVerbose)
	printf("Event not accepted  VertexX = %f \n",Event->GetVertexX());
      fnVertexX++;
      return kFALSE;
    }
    if(fFlagVertexY && (Event->GetVertexY() < fMinVertexY || Event->GetVertexY() > fMaxVertexY) )
    {
      if(fVerbose)
	printf("Event not accepted  VertexY = %f \n",Event->GetVertexY());
      fnVertexY++;
      return kFALSE;
    }
    if(fFlagVertexZ && (Event->GetVertexZ() < fMinVertexZ || Event->GetVertexZ() > fMaxVertexZ) )
    {
      if(fVerbose)
	printf("Event not accepted  VertexZ = %f \n",Event->GetVertexZ());
      fnVertexZ++;
      return kFALSE;
    }
    if(fFlagEVeto && (Event->GetEveto() < fMinEVeto || Event->GetEveto() > fMaxEVeto) )
    {
      if(fVerbose)
	printf("Event not accepted  EVeto = %f \n",Event->GetEveto());
      fnEVeto++;
      return kFALSE;
    }
   
    fnumgoodevent++;
    return kTRUE;

}

//______________________________________________________________________________
TPaveText  *T49CutEvent::GetCutList()
{
  //
  // Creates a TPaveText with the defined cuts and the cut-statistics
  //

  TPaveText *pave      = new TPaveText(0,0,1,1);
 
   Char_t         hname[256]; 

   sprintf(hname,"total number of events : %d \n",fnumevent);  
   pave->AddText(hname);

   sprintf(hname,"number of events accepted : %d \n",fnumgoodevent);  
   pave->AddText(hname);

  if(fFlagMult)
   {     
      sprintf(hname,"SetMult(%d,%d) : %d \n", fMinMult, fMaxMult,fnMult);
      pave->AddText(hname);
   }
   if(fFlagVertexIflag)
   { 
      if(fN2VertexIflag!= 99 && fN3VertexIflag != 99)      
         sprintf(hname,"SetVertexIflag(%d,%d,%d) : %d \n",fN1VertexIflag
                                                         ,fN2VertexIflag
                                                         ,fN3VertexIflag
                                                         ,fnVertexIflag);  
      if(fN2VertexIflag!= 99 && fN3VertexIflag == 99)      
         sprintf(hname,"SetVertexIflag(%d,%d) : %d \n",fN1VertexIflag
                                                      ,fN2VertexIflag
                                                      ,fnVertexIflag);  
      if(fN2VertexIflag== 99 && fN3VertexIflag == 99)      
         sprintf(hname,"SetVertexIflag(%d) : %d \n",fN1VertexIflag
                                                   ,fnVertexIflag);  
      
      pave->AddText(hname);
   }
   if(fFlagVertexX)
   {
      sprintf(hname,"SetVertexX(%f,%f) : %d \n", fMinVertexX, fMaxVertexX,fnVertexX);
      pave->AddText(hname);
   }
   if(fFlagVertexY)
   {
      sprintf(hname,"SetVertexY(%f,%f) : %d \n", fMinVertexY, fMaxVertexY,fnVertexY);
      pave->AddText(hname);
   }
   if(fFlagVertexZ)
   {
      sprintf(hname,"SetVertexZ(%f,%f) : %d \n", fMinVertexZ, fMaxVertexZ, fnVertexZ);
      pave->AddText(hname);
   }
   if(fFlagEVeto)
   {
      sprintf(hname,"SetEVeto(%f,%f) : %d \n", fMinEVeto, fMaxEVeto,fnEVeto);
      pave->AddText(hname);
   }
    return pave;
}

//______________________________________________________________________________
void  T49CutEvent::PrintCutList()
{
  //
  // Prints the defined cuts and the cut-statistic
  //

    GetCutList()->Print();
}












