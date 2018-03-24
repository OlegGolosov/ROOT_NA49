{

  ////////////////////////////////////////////////////////////////
  //
  //  Example macro to demonstrate the usage of the T49Proj2 
  //  and the T49Mixing class.
  //  It creates an invariant mass spectrum of positiv and 
  //  negativ tracks, assuming they are kaons, as well as 
  //  the mixed event background.
  //
  ////////////////////////////////////////////////////////////////
   
  gROOT->Reset();
  gROOT->Time();

  // The particle types
  const Int_t kKaon = 2;

  // The maximum number of events to process
  const Int_t nMaxEvent = 1000;

  // The number of events to mix
  const Int_t nMixed = 5;

  // Load libraries if root49 is not used
  if (gClassTable->GetID("T49Run") < 0) {
    printf("Load shared libraries for T49\n");
    gSystem->Load("libT49DST.so");    // Load root DST library
    gSystem->Load("libT49ANA.so");    // Load root analysis library
  }

  // Define the T49Run object
  T49Run *Run;
  if (gT49) {
    Run = gT49;
  }
  else
    Run = new T49Run();         

  // The event object
  T49EventRoot       *Event;

  // The particle lists
  TClonesArray    *AllParticleList;
  TObjArray       *PosParticleList;
  TObjArray       *NegParticleList;
  TObjArray       *MixedPosParticleList;
  TObjArray       *MixedNegParticleList;

  // Define event cuts
  T49CutEvent     *EventCut = new T49CutEvent("EventCut","Event cuts");
  EventCut->SetStandardCutsPbPbMB();

  // Define the track cuts (positiv and negativ tracks)
  T49CutTrack     *TrackCutPos = new T49CutTrack("PosTrackCut","Track cuts positive charge");
  TrackCutPos->SetCharge( 1);
  TrackCutPos->SetStandardCutsPbPb();
  T49CutTrack     *TrackCutNeg = new T49CutTrack("NegTrackCut","Track cuts negative charge");
  TrackCutNeg->SetCharge(-1);
  TrackCutNeg->SetStandardCutsPbPb();

  // The T49Proj2 class for the invariant mass analysis (signal distribution)
  T49Proj2INVmass *Signal = new T49Proj2INVmass();
  Signal->SetParticleMass(1,kKaon);
  Signal->SetParticleMass(2,kKaon);
  Signal->Book("Sig","Signal",300,0.9,1.2);

  // The T49Proj2 class for the invariant mass analysis (mixed event background)
  T49Proj2INVmass *Mixed = new T49Proj2INVmass();
  Mixed->SetParticleMass(1,kKaon);
  Mixed->SetParticleMass(2,kKaon);
  Mixed->Book("Mix","Background",300,0.9,1.2);

  // Define the event-mixer with 100 events in the pool
  T49Mixing       *EventMixer = new T49Mixing(100);

  // Open Root-DST run and event loop    
  Int_t  nEvent      = 0;
  Int_t  nAccEvent   = 0;
  Int_t  nMixedEvent = 0;
  Bool_t firstMix    = kTRUE;
  while ((Event = (T49EventRoot *) Run->GetNextEvent("n-minbias-std+-99D-B")) && 
         (nEvent < nMaxEvent)) {        

    nEvent++;  
    if (!(nEvent % 100)) {
      printf(" Processed %d events\n",nEvent);
      printf(" Run %d event %d (pos %d) (neg %d) tracks\n"
    	    ,Event->GetNRun()
	    ,Event->GetNEvent()
	    ,PosParticleList->GetEntries()
            ,NegParticleList->GetEntries());
    }	      

    // Check wether the event passes the event cut
    if (EventCut->CheckEvent(Event)) {
	      
      nAccEvent++;
	      
      // Get the list of main-vertex tracks
      AllTrackList = Event->GetPrimaryParticles();
                  
      // Create list with positive and negativw particles
      PosParticleList = TrackCutPos->GetAcceptedParticles(AllTrackList);
      NegParticleList = TrackCutNeg->GetAcceptedParticles(AllTrackList);

      // Fill the invariant mass spectrum (signal distribution)
      Signal->Fill(PosParticleList,NegParticleList);
	      
      // Add the particle lists to the event-mixing pool
      EventMixer->AddEvent(PosParticleList,NegParticleList);

      // Start the event mixing when the pool is full
      Int_t iMixed = 0;
      while ((iMixed < nMixed) && (EventMixer->IsReady())) {
        if (firstMix) { 
          printf(" Start event mixing at event %d\n",nEvent);
          firstMix = kFALSE;
	}
	EventMixer->GetEvent(-1,-1);    		  
        MixedPosParticleList = EventMixer->GetPart1List();
        MixedNegParticleList = EventMixer->GetPart2List();
	Mixed->Fill(MixedPosParticleList,MixedNegParticleList);
        MixedPosParticleList->Clear();
	MixedNegParticleList->Clear();
	nMixedEvent++;
	iMixed++;		  
      }

    }

  }   
    
  // Final event-mixing call
  printf(" Start final event mixing call\n");
  while (nMixedEvent < nMixed * nAccEvent) {
    EventMixer->GetEvent(-1,-1);    		  
    MixedPosParticleList = EventMixer->GetPart1List();
    MixedNegParticleList = EventMixer->GetPart2List();
    Mixed->Fill(MixedPosParticleList,MixedNegParticleList);
    MixedPosParticleList->Clear();
    MixedNegParticleList->Clear();
    nMixedEvent++;   
  }

  // Store everything in a ROOT-file
  TFile *OutFile = TFile::Open("minvmacro.root","RECREATE","Minv analysis output");
  EventCut->Write();
  TrackCutPos->Write();
  TrackCutNeg->Write();
  Signal->WriteHist();
  Mixed->WriteHist();
  OutFile->Close();

}





