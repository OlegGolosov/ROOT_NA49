#include "stdlib.h"
#include <math.h>
#include "T49ParticleRoot.h"
#include "T49CrMixer.h"

#include "TROOT.h"
#include "TObjArray.h"
#include "TRandom.h"

ClassImp(T49CrMixer)

//////////////////////////////////////////////////////////////////
//
//
//   T49CrMixer is a tool to generate reference samples that are
//   needed for several types of analysis (e.g. HBT) by mixing
//   tracks from several events. T49CrMixer provides a simple interface
//   for adding tracks to a track-pool and getting mixed events of
//   arbitrayry size from this pool.
//
///////////////////////////////////////////////////////////////////


extern "C" float ran3(Long_t *x);

Int_t T49CrMixer::Rndm(Float_t n)
{
     Int_t j  = (int) (n*ran3(&fIdum));
     return j;
}

void  T49CrMixer::SetSeed(Int_t n)
{
     fIdum = n;
}

T49CrMixer::T49CrMixer(Int_t MaxPoolSize, Int_t MaxStoredEvents)
{
//
// Constructor for T49CrMixer. The argument 'max' specifies the
// maximum size of the track-pool. This should be much larger
// than the typical multiplicity for one event, to avoid residual
// correlations.

// Store as many different events as maximum multiplicity of 
// mixed events - rule out multiple tracks of the same event
// in a mixed event

    Int_t i;
    fRandom = new TRandom();
    fMaxStoredEvents  = MaxStoredEvents;
    fCurrentEvent     = 0;
    fNStoredEvents    = 0;
    fMaxPoolSize      = MaxPoolSize;
    fMaxTracksPerEvent  = MaxPoolSize/fMaxStoredEvents;
    fNTracksPerEvent    = new int[fMaxStoredEvents];
    fNOrgTracksPerEvent = new int[fMaxStoredEvents];
    fEventUsed          = new int[fMaxStoredEvents];
    fEventStat          = new int[fMaxStoredEvents];
    fIndexStat          = new int[fMaxPoolSize];
    fTotalCalls         = 0;
    fMeanPt             = 0;
    fTotalEventCalls    = 0;
    fMaxOrgTracks       = 0;
    fPoolSize    = 0;
    fVerbose     = kFALSE;
    fParticlePool   = new T49ParticleRoot[fMaxPoolSize];
    for(i=0;i<fMaxStoredEvents;i++)
    {
       fNTracksPerEvent[i]    = 0;
       fNOrgTracksPerEvent[i] = 0;
       fEventUsed[i]          = 0;
       fEventStat[i]      = 0;
    }
    for(i=0;i<fMaxPoolSize;i++)
    {
       fIndexStat[i]       = 0;
    }
    
    if(!fParticlePool)
    {
        printf("Couldn't alloc fParticlePool, exit\n");
        exit(0);
    }
    else
    {
         printf("Created track pool of size %d (%d)\n",
                  fMaxPoolSize,fMaxTracksPerEvent);
    }
}

Int_t T49CrMixer::PickEvent(Int_t CheckMult)
{
   Int_t n;
   Int_t m;

   n =  Rndm(fNStoredEvents);

   if(CheckMult)
   {
       m =  Rndm(fMaxOrgTracks);
       while(m > fNOrgTracksPerEvent[n])
       {
           n =  Rndm(fNStoredEvents-1);
           m =  Rndm(fMaxOrgTracks);
       }
       fEventStat[n]++;
       fTotalEventCalls++;
   }

   if(fVerbose)
       printf("PickEvent(): Selected event %d (%d, %d, %d, %d)\n",
                         n,fNStoredEvents,fMaxStoredEvents,fNOrgTracksPerEvent[n],fMaxOrgTracks);
   return n;
}

void T49CrMixer::AddParticleList(TObjArray *ParticleList)
{
// This routine adds a list of tracks to the trackpool

     Int_t max;
     Int_t n;
     Int_t i;
     Int_t index;
     Int_t NEvent;
     Char_t used[3000];

     for(i=0;i<3000;i++)
     {
         used[i] = 0;
     }

     T49ParticleRoot *track;

     NEvent = fCurrentEvent;
     if(fCurrentEvent < (fMaxStoredEvents-1))
     {
         fCurrentEvent++;
     }
     else
     {
         fCurrentEvent = 0;
     }
     if(fNStoredEvents < (fMaxStoredEvents-1))
     {
         fNStoredEvents++;
     }
     if(fVerbose)
     {
         printf("%d stored events (%d,%d), add %d tracks to event %d (%d)\n",
                 fNStoredEvents,fMaxStoredEvents,fCurrentEvent,
                 ParticleList->GetEntries(),NEvent,fPoolSize);
     }
     index = NEvent * fMaxTracksPerEvent;

     max = ParticleList->GetEntries();
     fNOrgTracksPerEvent[NEvent] = max;
     if(max > fMaxOrgTracks)
         fMaxOrgTracks = max;
     if(max > 3000)
     {
         printf("Warning: %d particles in list - exceeding max. of 3000\n",max);
     }
     if(max >= fMaxTracksPerEvent)
     {
         for(i=0;i<fMaxTracksPerEvent;i++)
         {
             n = Rndm(max);
             while(used[n])
             {
                 n = Rndm(max);
             }
             used[n] = 1;
             track  = (T49ParticleRoot *) ParticleList->At(n);
             if(index < fMaxPoolSize)
             {
                 fParticlePool[index].CopyParticle(track);
                 fPoolSize++;
                 index++;
             }
         }
         fNTracksPerEvent[NEvent] = fMaxTracksPerEvent;
     }
     else
     {
         for(i=0;i<max;i++)
         {
             track  = (T49ParticleRoot *) ParticleList->At(i);
             fParticlePool[index].CopyParticle(track);
             fPoolSize++;
             index++;
         }
         fNTracksPerEvent[NEvent] = max;
     }
     
     if(fVerbose)
         printf("T49CrMixer.Size = %d\n",fPoolSize);
}

T49ParticleRoot * T49CrMixer::GetParticle(Int_t NEvent)
{
    Int_t index;
    Int_t max;
    T49ParticleRoot *track;

    max   = fNTracksPerEvent[NEvent];
    index = NEvent * fMaxTracksPerEvent + Rndm(max);
    fIndexStat[index]++;
    fTotalCalls++;
/*
    Float_t  mean = (Float_t ) fTotalCalls / (Float_t) fPoolSize;
    if((fIndexStat[index] > mean + 10*sqrt(mean)) && (mean > 1))
    {
        printf("fTotalCalls, fPoolSize,mean   : %d %d %g\n",
               fTotalCalls, fPoolSize,mean);
        printf("index,fIndexStat[index],NEvent: %d %d %d\n",
               index,fIndexStat[index],NEvent);
        printf("fNTracksPerEvent[NEvent],fNOrgTracksPerEvent[NEvent]: %d %d\n",
                    fNTracksPerEvent[NEvent],fNOrgTracksPerEvent[NEvent]);
    }
*/
    track = (T49ParticleRoot *) &fParticlePool[index];
    fMeanPt += track->GetPt();
    if(fVerbose > 1)
    {
         printf("pick track in event %d (%d): %d : %d\n",
                          NEvent,fNStoredEvents,max,index);
         printf("pt,y =  %g,%g\n",track->GetPt(),track->GetRap(0.14));
    }
    return track;
}

void T49CrMixer::ShowStat()
{
    Int_t i;
    Double_t mean;
    Int_t min = 100000000;
    Int_t max = 0;
    Int_t n   = 0;
    Int_t sum = 0;
    Int_t MinIndex = 0;
    Int_t MaxIndex = 0;


     printf("fNStoredEvents, fMaxStoredEvents: %d %d\n",fNStoredEvents, fMaxStoredEvents);
     printf("fMaxPoolSize, fPoolSize         : %d %d\n",fMaxPoolSize, fPoolSize );
     printf("fMaxTracksPerEvent              : %d \n",fMaxTracksPerEvent);        

    for(i=0;i<fMaxPoolSize;i++)
    {
        if(fIndexStat[i] > 0)
        {
            sum += fIndexStat[i];
            n++;
            if(fIndexStat[i] < min)
            {
               min = fIndexStat[i];
               MinIndex = i;
            }
            if(fIndexStat[i] > max)
            {
               max = fIndexStat[i];
               MaxIndex = i;
            }
        }
    }

    if(n>0)
        mean = (Float_t) sum/n;
    else
        mean = 0;
    printf("fTotalCalls, sum, n, min, max, mean: %d %d %d %d (%d)%d (%d) %g\n",
            fTotalCalls, sum, n, min, MinIndex, max, MaxIndex, mean);
    n   = 0;
    sum = 0;
    mean = 0;
    min = 1000000;
    max = 0;

    for(i=0;i<fNStoredEvents;i++)
    {
        if(fEventStat[i] > 0)
        {
            sum += fEventStat[i];
            n++;
            if(fEventStat[i] < min)
            {
               min = fEventStat[i];
               MinIndex = i;
            }
            if(fEventStat[i] > max)
            {
               max = fEventStat[i];
               MaxIndex = i;
            }
        }
    }
    if(n>0)
        mean = (Float_t) sum/n;
    else
        mean = 0;

    printf("fTotalEventCalls, sum, n, min, max, mean: %d %d %d %d (%d, %d) %d (%d, %d) %g\n",
            fTotalEventCalls, sum, n, min, MinIndex,fNOrgTracksPerEvent[MinIndex], 
            max, MaxIndex,fNOrgTracksPerEvent[MaxIndex], mean);
    if(fTotalCalls > 0)
        mean = fMeanPt/fTotalCalls;
    else
        mean = 0;
    printf("Mean pt: %g\n",mean);

}

Bool_t T49CrMixer::IsReady(Int_t mult)
{
    if(mult == 0)
    {
        if(fNStoredEvents < (fMaxStoredEvents-1))
        {
            return kFALSE;
        }
        else
        {
            return kTRUE;
        }
    }
    else
    {
        if(fNStoredEvents < mult * 2)
        {
            if(fVerbose)
                printf("mult %d, fNStoredEvents %d\n",mult,fNStoredEvents);
            return kFALSE;
        }
        else
        {
            return kTRUE;
        }
    }
    
}

TObjArray *T49CrMixer::GetParticleList(Int_t mult)
{

// This routine returns a list of mixed tracks, with the
// multiplicity requested in 'mult'

    Int_t i;
    Int_t n;
    Int_t cnt;
    TObjArray *a;

    a = new TObjArray();
    if(!IsReady(mult))
    {
        printf("Number of stored events %d, need %d\n",
               fNStoredEvents,fMaxStoredEvents);
        printf("event mixing not possible yet - add more events to track pool\n");
        return NULL;
    }
    for(i=0;i<fNStoredEvents;i++)
    {
        fEventUsed[i] = 0;
    }
    cnt = 0;
    while(cnt < mult)  
    {
        n = PickEvent(kTRUE);
        if(!fEventUsed[n])
        {
            a->Add(GetParticle(n));
            fEventUsed[n] = kTRUE;
            cnt++;
        }
    }
    return a;

#ifdef MUELL
    else
    {
        cnt = 0;
        while(cnt < (fNStoredEvents - mult))
        {
            n = PickEvent(kTRUE);
            if(!fEventUsed[n])
            {
                fEventUsed[n] = kTRUE;
                cnt++;
            }
        }
        for(i=0;i<fNStoredEvents;i++)
        {
            if(!fEventUsed[i])
            {
                a->Add(GetParticle(i));
            }
        }
    }
#endif
}

void T49CrMixer::GetPair(T49ParticleRoot **p1, T49ParticleRoot **p2)
{
    Int_t n1 = 0;
    Int_t n2 = 0;

    n1 = PickEvent(kTRUE);
    n2 = PickEvent(kTRUE);
    while(n1 == n2)
        n2 = PickEvent(kTRUE);
    *p1 = GetParticle(n1);
    *p2 = GetParticle(n2);
}

TObjArray *T49CrMixer::GetEvent(Int_t NEvent)
{
    TObjArray *a = new TObjArray();

    Int_t index = NEvent * fMaxTracksPerEvent;
    for(Int_t i=0;i<fNTracksPerEvent[NEvent];i++)
    {
        T49ParticleRoot *track  = (T49ParticleRoot *) &fParticlePool[index];
        a->Add(track);
        index++;
    }
return a;
}
#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)

/* According to Knuth, any large MBIG, and any smaller  */
/* (but still large) MSEED can be substituted for the above values. */

float ran3(long *idum)
/* Returns a uniform random deviate between 0.0 and 1.0. Set idum to  */
/* any negative value to initialize or reinitialize the sequence. */

{
  static int inext, inextp;
  static long ma[56];       /* The value 56 is special (range ma[1..55]) */
  static int iff = 0;       /* and should not be modified */
  long mj, mk;
  int i, ii, k;

  if (*idum < 0 || iff == 0){     /* ----- initialization ----- */
    iff = 1;
    mj = MSEED - (*idum < 0 ? -*idum : *idum);  /* Initialize ma[55] using the seed, */
    mj %= MBIG;                                 /* idum, and the large number MSEED */
    ma[55] = mj;
    mk = 1;
    for (i=1; i<=54; i++){                /* Now initialize the rest of the table, */
      ii = (21*i) % 55;                   /* in a slightly random order, with */
      ma[ii] = mk;                        /* numbers that are not especially random. */
      mk = mj-mk;
      if (mk < MZ) mk += MBIG;
      mj = ma[ii];
    }
    for (k=1; k<=4; k++){        /* We randomize them by 'warming up the generator'. */
      for (i=1; i<=55; i++){
        ma[i] -= ma[1 + (i+30) % 55];
        if (ma[i] < MZ) ma[i] += MBIG;
      }
    }
    inext = 0;
    inextp = 31;
    *idum = 1;
  }
  /* ----- Here is where we start, except on initialization. ----- */
  if (++inext == 56) inext = 1;      /* increment inext and inextp, */
  if (++inextp == 56) inextp = 1;    /* wrapping around at 56 vto 1. */
  mj = ma[inext] - ma[inextp];       /* Generate a new random number subtractively. */
  if (mj < MZ) mj += MBIG;           /* Be sure that it is in the range. */
  ma[inext] = mj;                    /* Store it, */
  return mj*FAC;                     /* and output the derived uniform deviate. */
}
/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */


