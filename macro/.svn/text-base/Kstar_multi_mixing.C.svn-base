//         K*->K+pi-
void Kstar_multi_mixing(Int_t StartRun=3905,Int_t StopRun =3919,Int_t MaxEvent =1000,const TString id="test", Int_t type = 1)
{
  /*Int_t StartRun = 3905;
  Int_t StopRun =3919;
  Int_t MaxEvent =1000;
  const TString id="01";*/
  
  //gROOT->Reset();
  //gROOT->Time();
  
  TString outpath="/home/hirg/slodkow/pater/na49/Resonance/Kstar/PbPb/universal_multi_mixing/";
  
  gROOT->LoadMacro("/home/hirg/slodkow/pater/na49/Resonance/Kstar/PbPb/universal_multi_mixing/T49MultiMixer.cxx");
  gROOT->LoadMacro("/data/ppstor001a/users/slodkow/na49/Resonance/Kstar/PbPb/pid.cxx"); 
  gROOT->LoadMacro("/data/ppstor001a/users/slodkow/na49/Resonance/Kstar/PbPb/phi.cxx");
  //gROOT->LoadMacro("/data/ppstor001a/users/slodkow/na49/Resonance/Kstar/PbPb/append.C");
  //gROOT->LoadMacro("/data/ppstor001a/users/slodkow/na49/Resonance/Kstar/PbPb/remove_temp_file.C");
  


  Bool_t phicut = kFALSE; // kFALSE - without azimutal angle cuts, kTRUE - with azimutal angle cuts

  Int_t debug = 0;
  Int_t bench = 0;
  Float_t Pimass = 0.1395700;
  Float_t Kmass = 0.493677;
  Float_t Prmass = 0.93827231;


  
  
  ///////////////////////
  // START
  //////////////////////
  
  
  // The particle types
  const Int_t pPion = 1;
  const Int_t pKaon = 2;
  const Int_t pProton = 3;
  const Int_t nPion = 4;
  const Int_t nKaon = 5;
  
  //const Int_t nMaxEvent = 494992;
  const Int_t nMaxEvent = MaxEvent;
  const Int_t nMixed = 100;

  // For computing in multiplicity intervals
  if(type == 1)
  {
      const Int_t mult_min = 400;
      const Int_t mult_max = 1600;
      const Int_t mult_interval = 20;
  }
  else
  {
      const Int_t mult_min = 0;
      const Int_t mult_max = 24000;
      const Int_t mult_interval = 400;
  }
  
    // Load libraries if root49 is not used
  if (gClassTable->GetID("T49Run") < 0) {
    printf("Load shared libraries for T49\n");
    gSystem->Load("libT49DST.so");    // Load root DST library
    gSystem->Load("libT49ANA.so");    // Load root analysis library
  }
  
  T49Run *Run;
  if (gT49) {
    Run = gT49;
  }
  else {
    Run = new T49Run();         
  }

      Char_t *RunType="std+-160GeV-central-256TB-01I";
      Int_t StartAtRun = StartRun;
      Int_t EndAtRun = StopRun;

  // The event object
  T49EventRoot       *Event;
  
  // The particle lists
  TObjArray       *AllTrackList;
  TObjArray       *CutTrackList;
  TObjArray       *PosParticleList;
  TObjArray       *NegParticleList;
  TObjArray       *MixedPosParticleList;
  TObjArray       *MixedNegParticleList;
  TObjArray       *PhicutParticleList;
  
  // Define event cuts
  T49CutEvent     *EventCut = new T49CutEvent("EventCut","Event cuts");
  
  EventCut->SetVertexIflag(0);
  EventCut->SetVertexX(-0.3,0.3);
  EventCut->SetVertexY(-0.3,0.3);
  EventCut->SetVertexZ(-582,-580);
  //EventCut->SetMult(mult,mult+mult_interval);
  EventCut->SetMult(mult_min,mult_max);


  // Define the track cuts (positiv and negativ tracks)
  PID     *TrackCutPos = new PID("PosTrackCut","Track cuts positive charge");
  PID     *TrackCutNeg = new PID("NegTrackCut","Track cuts negative charge");
  PID     *cuttrack = new PID("Cut1","Track cuts");
  PHI     *cutphi = new PHI("Cut2","Azimutal angle cuts");
  
  cuttrack->SetBx(-5,5);
  cuttrack->SetBy(-3,3);
  cuttrack->SetP(3.0,180.0);
  cuttrack->SetNPoint(3,25,234);
  cuttrack->SetNPointToNMaxPoint(3,0.5);

  // Define the 2D histograms for checking of PID
  TH2F *dedx_all_pos = new TH2F("dEdx_p_all_pos","dEdx_p_all_pos",50,-1,3, 50, 0.9, 1.9); 
  TH2F *dedx_all_neg = new TH2F("dEdx_p_all_neg","dEdx_p_all_neg",50,-1,3, 50, 0.9, 1.9); 
  TH2F *dedx_pos = new TH2F("dEdx_p_pos","dEdx_p_pos",50,-1,3, 50, 0.9, 1.9); 
  TH2F *dedx_neg = new TH2F("dEdx_p_neg","dEdx_p_neg",50,-1,3, 50, 0.9, 1.9); 
  TH2F *acc_all_pos = new TH2F("acc_all_pos","acc_all_pos",50,0, 6, 50, 0, 2); 
  TH2F *acc_all_neg = new TH2F("acc_all_neg","acc_all_neg",50,0, 6, 50, 0, 2); 
  TH2F *acc_pos = new TH2F("acc_pos","acc_all",50,0, 6, 50, 0, 2); 
  TH2F *acc_neg = new TH2F("acc_neg","acc_all",50,0, 6, 50, 0, 2); 
  TH2F *rap_pt = new TH2F("rap_pt","rapidity vs. pt",50,0, 6, 50, 0, 2);
  TH2F *rap_pt_pos = new TH2F("rap_pt_pos","rapidity vs. pt",50,0, 6, 50, 0, 2);
  TH2F *rap_pt_neg = new TH2F("rap_pt_neg","rapidity vs. pt",50,0, 6, 50, 0, 2);
  
  
  // Define the event-mixer with 100 events in the pool
  T49Mixing       *EventMixer = new T49Mixing(100);
  
  // Open Root-DST run and event loop    
  Int_t  nEvent      = 0;
  Int_t  nAccEvent   = 0;
  Int_t  nMixedEvent = 0;
  Bool_t firstMix    = kTRUE;
  
  Int_t ptcl = 0;
  
  TH1F *hdet = new TH1F("hdet","hdet",25,1,25); 
  TH1F *hdet_pos = new TH1F("hdet_pos","hdet_pos",25,1,25); 
  TH1F *hdet_neg = new TH1F("hdet_neg","hdet_neg",25,1,25); 
  TH1F *hpx_pos = new TH1F("hpx_pos","hpx_pos",100,-10,10); 
  TH1F *hpx_neg = new TH1F("hpx_neg","hpx_neg",100,-10,10); 
  
  TH1F *hpos = new TH1F("hpos","hpos",100,0,100); 
  TH1F *hneg = new TH1F("hneg","hneg",100,0,100);


  // Open Root-DST run and event loop    
  nEvent      = 0;
  nAccEvent   = 0;
  nMixedEvent = 0;
  firstMix    = kTRUE;


  // The T49Proj2 class for the invariant mass analysis (signal distribution)

        T49Proj2INVmass *Signal = new T49Proj2INVmass();
	Signal->SetParticleMass(1,Kmass);
	Signal->SetParticleMass(2,Pimass);
        Signal->System(0.0);
        Signal->SetArrayY(7,2.61,4.71);
        Signal->Book("Sig","Signal",150,0.5,2.0);
  
	// The T49Proj2 class for the invariant mass analysis (mixed event background)
	T49Proj2INVmass *Mixed = new T49Proj2INVmass();
	Mixed->SetParticleMass(1,Kmass);
	Mixed->SetParticleMass(2,Pimass);
        Mixed->System(0.0);
        Mixed->SetArrayY(7,2.61,4.71);
	Mixed->Book("Mix","Background",150,0.5,2.0);




  // Reset anaysis for mulitpicity intervals

  //Run->SetCurrentRunID(0);
  Run->Reset();
  Run->ResetCurrentRunID();
  Run->ResetRootFile();
  printf("fCurrentRunID = %d\n",Run->GetCurrentRunID());

  Int_t mult;
  TString min_mult;
  TString max_mult;
  Int_t ranges;;


  // Define the event-multi-mixer with muli minimum and maximum common varable;

  T49MultiMixer *MultiMixer = new T49MultiMixer(mult_min,mult_max,mult_interval,nMaxEvent,type);

  /*ranges = (mult_max-mult_min)/mult_interval;

  TObjArray **MultiPosParticleList = new TObjArray * [ranges];
  TObjArray **MultiNegParticleList = new TObjArray * [ranges];
  TObjArray **MultiMixedPosParticleList = new TObjArray * [ranges];
  TObjArray **MultiMixedNegParticleList = new TObjArray * [ranges];*/


  while ((Event = (T49EventRoot *) Run->GetNextEvent(RunType,StartAtRun,EndAtRun)) && (nEvent < nMaxEvent))
  { 
    
    nEvent++;  
//    printf("number of event: %d  \n",nEvent);
    if(bench) gBenchmark->Start("event");    
    if (!(nEvent % 100)) {
	printf(" Processed %d events\n",nEvent);
	printf(" Run %d event %d\n"
		 ,Event->GetNRun()
		 ,Event->GetNEvent());
    }	      


    // Check wether the event passes the event cut
    if (EventCut->CheckEvent(Event) == 1)
        { // Event Loop
  
        //printf("Accept event in (%d,%d) multiplicity intervals\n",mult,mult+mult_interval);
	nAccEvent++;
	// Get the list of main-vertex tracks
	AllTrackList = Event->GetPrimaryParticles();
	for(ptcl=0;ptcl< AllTrackList->GetEntries();ptcl++)	  { // all particle loop
	  atrack = ((T49ParticleRoot *)AllTrackList->At(ptcl));
	  if(atrack ->GetCharge() == 1) {
	    acc_all_pos -> Fill(atrack->GetRap(Prmass),atrack->GetPt());
	    dedx_all_pos -> Fill(log10(atrack->GetP()), atrack->GetTmeanCharge()/1000);
	  }
	  if(atrack ->GetCharge() == -1) {
	    acc_all_neg -> Fill(atrack->GetRap(Kmass),atrack->GetPt());
	    dedx_all_neg -> Fill(log10(atrack->GetP()), atrack->GetTmeanCharge()/1000);
	  }
	  hdet -> Fill(atrack->GetIdDet());
	}

	CutTrackList = cuttrack->GetAcceptedParticles(AllTrackList);


	if(phicut) PhicutParticleList = cutphi->azimuthal_angle(CutTrackList,TMath::Pi()/6);
        else PhicutParticleList = CutTrackList;

        PosParticleList = TrackCutPos->dEdX(PhicutParticleList, pKaon, 4.0, 4.0, 1, 0);
	hpos ->Fill(PosParticleList->GetEntries());



        for(ptcl=0;ptcl< PosParticleList->GetEntries();ptcl++)  { // pos particle loop
	  track = ((T49ParticleRoot *)PosParticleList->At(ptcl));
	  acc_pos -> Fill(track->GetRap(Kmass),track->GetPt());
	  dedx_pos -> Fill(log10(track->GetP()), track->GetTmeanCharge()/1000);
	  hdet_pos -> Fill(track->GetIdDet());
	  hpx_pos -> Fill(track->GetPx());
	}
	

        NegParticleList = TrackCutNeg->dEdX(PhicutParticleList, nPion, 20.0, 8.0, 1, 0);
        hneg ->Fill(NegParticleList->GetEntries());

	
	// Check PID with selected particles
	for(ptcl=0;ptcl< NegParticleList->GetEntries();ptcl++)  { // neg particle loop
	  track = ((T49ParticleRoot *)NegParticleList->At(ptcl));
	  acc_neg -> Fill(track->GetRap(Pimass),track->GetPt());
	  dedx_neg -> Fill(log10(track->GetP()), track->GetTmeanCharge()/1000);
	  hdet_neg -> Fill(track->GetIdDet());
	  hpx_neg -> Fill(track->GetPx());
	}


	/*********************************************************************************/
        /* Fill plot rapidity vs. pt in mass invaraint intervals for each paritcles pair */

	T49ParticleRoot *Part1;
	T49ParticleRoot *Part2;
        Int_t IP1 = 0;
	Int_t IP2 = 0;
        Double_t minv;

        IP1 = 0;
	TIter NextParticle(PosParticleList);
	while ((Part1 = ((T49ParticleRoot *) NextParticle())))
	{
	    IP1++;
	    IP2 = 0;
	    TIter NextP2(NegParticleList);
	    while ((Part2 = ((T49ParticleRoot *) NextP2())))
	    {
		IP2++;
		if (IP1 > IP2)
		    {
			minv = T49Tool::Minv(Part1,Part2,Kmass,Pimass);
			if(minv>0.85 && minv<0.95)
			{
			    rap_pt->Fill(Part1->GetRap(Kmass),Part1->GetPt());
			    rap_pt->Fill(Part2->GetRap(Pimass),Part2->GetPt());
			    rap_pt_pos->Fill(Part1->GetRap(Kmass),Part1->GetPt());
			    rap_pt_neg->Fill(Part2->GetRap(Pimass),Part2->GetPt());
			}

		    }
	    }
	}

	/*********************************************************************************/


        MultiMixer->AddEvent(Event,PosParticleList,NegParticleList);

        MultiMixer->GetEvents(Signal,Mixed);



     } // endif of eventcut

  } // end event loop reader
  

  if(type == 1)
      TFile *OutFile = TFile::Open(outpath+"Kstar_multi_"+id+".root", "RECREATE", "Minv analysis output");
  else
      TFile *OutFile = TFile::Open(outpath+"Kstar_eveto_"+id+".root", "RECREATE", "Minv analysis output");

  OutFile->cd();


  MultiMixer->WriteEvents(OutFile,Signal,Mixed);

  dedx_all_pos->Write();
  acc_all_pos->Write();
  dedx_all_neg->Write();
  acc_all_neg->Write();
  dedx_neg->Write();
  
  dedx_pos->Write();
  acc_pos->Write();
  acc_neg->Write();
  
  hdet->Write();
  hdet_pos->Write();
  hdet_neg->Write();
  hpx_pos->Write();
  hpx_neg->Write();
  
  hpos->Write();
  hneg->Write();

  rap_pt->Write();
  rap_pt_pos->Write();
  rap_pt_neg->Write();


  OutFile->Close();

} 
