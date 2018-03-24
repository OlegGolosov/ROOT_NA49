
/////////////////////////////////////////////////////////////////////////////////////
//                                                                                 //
//  T49MultiMixer class                                                            //
//                                                                                 //
//  Author: Marcin Slodkowski                                                      //
//          slodkow@if.pw.edu.pl                                                   //
//                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////

#include <TObjArray.h>
#include <TFile.h>

#include <T49EventRoot.h>

#include <T49MultiMixer.h>
#include <T49Mixing.h>
#include <T49Proj2.h>

ClassImp(T49MultiMixer)

/*
$Log: T49MultiMixer.C,v $
Revision 1.1  2006/07/28 09:53:01  cblume
Add T49MultiMixer.C

*/

//______________________________________________________________________________
T49MultiMixer::T49MultiMixer()
{
    Reset();
}

//______________________________________________________________________________
T49MultiMixer::T49MultiMixer(Int_t Multi_min, Int_t Multi_max, 
                             Int_t Multi_interval, Int_t Events, Int_t character)
{
  ////////////////////////////////////////
  //  use this constructor to initialize the multimixer
  //   multi_min    minimal value custom varable;
  //   multi_max    maximum value custom varable;
  //   multi_interval multi intervals;
  ////////////////////////////////////////////

    Int_t i;
    Reset();

    // Output control flag (kTRUE,kFALSE)
    fVerbose = kTRUE;

    // Type of bin selection
    fType = character;


    fPimass = 0.1395700;
    fKmass = 0.493677;
    fPrmass = 0.93827231;

    fFirstMix = kTRUE;
    fNMixed = 100;

    fMulti_min = Multi_min;
    fMulti_max = Multi_max;
    fMulti_interval = Multi_interval;
    fEvents=Events;
    fEvent_counter=0;


    fRanges = (fMulti_max-fMulti_min)/fMulti_interval;

    fMultiPosParticleList = new TObjArray();
    fMultiNegParticleList = new TObjArray();
    fMultiMixedPosParticleList = new TObjArray();
    fMultiMixedNegParticleList = new TObjArray();

    fEventMixer = new T49Mixing * [fRanges];


//     Char_t *chart_name_sig = new Char_t [20];
//     Char_t *signal_name = new Char_t [20];
//     Char_t *chart_name_mix = new Char_t [20];
//     Char_t *background_name = new Char_t [20];

    for(i=0;i<fRanges;i++)
    {
        // Define standard event mixer for bins
        fEventMixer[i] = new T49Mixing(10);
    }

}

//______________________________________________________________________________
void T49MultiMixer::Reset()
{
}

//______________________________________________________________________________
Int_t T49MultiMixer::MultiEvetoBins(T49EventRoot *Event, Int_t type)
{
    switch(type)
    {
    case 1:
        return Event->GetNParticles();
    case 2:
        return (Int_t)Event->GetEveto();
    default:
        return 0;
    }

}

//______________________________________________________________________________
void T49MultiMixer::AddEvent(T49EventRoot *Event, TObjArray *P1list, TObjArray *P2list)
{
    Int_t counter;
    Int_t multi;
    Int_t iMixed = 0;
    counter=0;
    for(multi=fMulti_min;(multi+fMulti_interval)<=fMulti_max;multi+=fMulti_interval)
    {
        if(MultiEvetoBins(Event,fType) > multi && MultiEvetoBins(Event,fType) < (multi+fMulti_interval))
        {
            if(fVerbose)
            {
                if(fType == 1)
                    printf("T49MultiMixer: Accept event in (%d,%d) multiplicity intervals\n",multi,multi+fMulti_interval);
                else
                    printf("T49MultiMixer: Accept event in (%d,%d) eveto intervals\n",multi,multi+fMulti_interval);

            }
            fEvent_counter = counter;

            fMultiPosParticleList = P1list;
            fMultiNegParticleList = P2list;

            // Add the particle lists to the event-mixing pool
            fEventMixer[counter]->AddEvent(P1list,P2list);
	
            // Start the event mixing when the pool is full
            iMixed = 0;
            //cout<<"counter="<<counter<<endl;
            while ((iMixed < fNMixed) && (fEventMixer[counter]->IsReady()))
            { // mixer Loop
                if (fFirstMix)
                {
                    printf("T49MultiMixer: Start event mixing at event %d\n",Event->GetNEvent());
                    fFirstMix = kFALSE;
                }
                fEventMixer[counter]->GetEvent(-1,-1);

                fMultiMixedPosParticleList = fEventMixer[counter]->GetPart1List();
                fMultiMixedNegParticleList = fEventMixer[counter]->GetPart2List();

                iMixed++;
            }
        }
        counter++;
    }

}

//______________________________________________________________________________
void T49MultiMixer::GetEvents(T49Proj2BASE *MultiSignal,T49Proj2BASE *MultiMixed)
{
  //Int_t multi;
  //Int_t i,j;
  //Int_t nevent;


    if(fVerbose)
        printf("T49MultiMixer: Fill for bin =  %d\n",fEvent_counter);

    if(fMultiPosParticleList->GetEntries() !=0 && fMultiNegParticleList->GetEntries() !=0)
        MultiSignal->Fill(fMultiPosParticleList,fMultiNegParticleList);
    if(fMultiMixedPosParticleList->GetEntries() !=0 && fMultiMixedNegParticleList->GetEntries() !=0)
        MultiMixed->Fill(fMultiMixedPosParticleList,fMultiMixedNegParticleList);


    fMultiPosParticleList->Clear();
    fMultiNegParticleList->Clear();
    fMultiMixedPosParticleList->Clear();
    fMultiMixedNegParticleList->Clear();

}

//______________________________________________________________________________
void T49MultiMixer::WriteEvents(TFile *OutFile,T49Proj2BASE *MultiSignal,T49Proj2BASE *MultiMixed)
{
  //Int_t multi;
  //Int_t i;

    MultiSignal->WriteHist();
    MultiMixed->WriteHist();

}
