{{
  gROOT->Reset();
  Char_t *RunType = "central-full-chain-std+-98h-tcal";

  // Retrieve dE/dx container
  TFile fin("contain_mtpc_fitted.root");
  T49Container *mtpcC = (T49Container*) fin->Get("mtpc_cont"); 

  // Event cut
  T49CutEvent *cutevent = new T49CutEvent();
  cutevent->SetStandardCutsPbPb();

  // Track quality cuts 
  T49CutTrack *cuttrack = new T49CutTrack();
  cuttrack->SetStandardCutsPbPb();
  cuttrack->SetNDedxPoint(2,60,150);
  cuttrack->SetNPointToNMaxPoint(3,0.3);

  // More initialisations...

  T49CutTrack *cutposka = (T49CutTrack*) cuttrack->Clone();
  cutposka->SetCharge(1);
  cutposka->SetDedxSigma(1,2,2,0.94,1.06);
  T49CutTrack *cutnegka = (T49CutTrack*) cuttrack->Clone();
  cutnegka->SetCharge(-1);
  cutnegka->SetDedxSigma(1,2,2,0.94,1.06);

  T49CutPid* cutpid = new T49CutPid();
  cutpid->SetDeltaDedx(-2,2);

  TCanvas *c1 = new TCanvas("c1","Canvas #1",500,600);
  gStyle->SetOptStat(0);

  TH2F *dedxH = new TH2F("dedxH","delta dE/dx vs. kaprob",100,-2.5,2.5,
			 100,0,1);

  TClonesArray *AllTracks;
  TObjArray    *SelTracks;
  TObjArray    *PosKa;
  TObjArray    *NegKa;
  TObjArray    *PidL;
  T49EventRoot *event;
  T49ParticleRoot* track;

  Int_t mult_poska;
  Int_t mult_negka;

  Int_t num      = 0;
  Int_t nevent = 500;

  // Start eventloop

  T49Run   *run = new T49Run();  
  run->SetVerbose(0);
  while(event = (T49EventRoot *) run->GetNextEvent(RunType) && 
	num < nevent) {
    num++;
    if (cutevent->CheckEvent(event)) {
      AllTracks = event->GetPrimaryParticles();

      // Select particles using T49CutTrack
      PosKa=cutposka->GetAcceptedParticles(AllTracks);
      NegKa=cutnegka->GetAcceptedParticles(AllTracks);
      printf("Selection using T49CutTrack: Ka+ %d, Ka- %d \n",
	     PosKa->GetEntries(),NegKa->GetEntries());

      // For slected kaons, determine delta dE/dx and kaon-probability
      // using the dE/dx container, and fill histogram
      Float_t delta;
      Float_t prob[4];
      TIter NextTrack(PosKa);
      while (track=(T49ParticleRoot*) NextTrack()) {
	if (mtpcC->GetDeltaDedx(track,2,delta) && 
	    mtpcC->GetProbability(track,prob))
	  dedxH->Fill(delta,prob[2],1);
      }
      TIter NextTrack2(NegKa);
      while (track=(T49ParticleRoot*) NextTrack2()) {
	if (mtpcC->GetDeltaDedx(track,2,delta) && 
	    mtpcC->GetProbability(track,prob))
	  dedxH->Fill(delta,prob[2],1);
      }
      dedxH->Draw();
      c1->Update();

      // Select kaons using T49CutPid::GetDeltaDedxParticles()
      SelTracks=cuttrack->GetAcceptedParticles(AllTracks);
      PidL=cutpid->GetDeltaDedxParticleType(SelTracks,mtpcC,2,1);
      Int_t mult_poska=PidL->GetEntries();
      PidL=cutpid->GetDeltaDedxParticleType(SelTracks,mtpcC,2,-1);
      Int_t mult_negka=PidL->GetEntries();
      printf("Selection using T49CutPid: Ka+ %d, Ka- %d \n",
	     mult_poska,mult_negka);
    }
  }
  run->Close();
}} 


