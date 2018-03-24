{

  /////////////////////////////////////////////////////////////////////////
  //
  //  Example macro for the analysis of MC data. 
  //
  /////////////////////////////////////////////////////////////////////////

  // Reset the ROOT environment
  gROOT.Reset();

  // Load libraries if root49 is not used
  if (gClassTable->GetID("T49Run") < 0) {
    printf("Load shared libraries for T49\n");
    gSystem->Load("libT49DST.so");
    gSystem->Load("libT49ANA.so");
  }

  T49EventMCRoot *MCEvent;

  TClonesArray   *PriTrackList;
  TClonesArray   *MCTrackList;
  TObjArray      *MCVertexList;

  // Define the T49Run object
  T49Run           *Run;
  if (gT49) {
    Run = gT49;
  }
  else
    Run = new T49Run();         

  // maximum number of events to process
  const Int_t       maxEvent = 100;

  // Book the histograms
  TH1F *hPid = new TH1F("hPid","MC GEANT PID",31,-0.5,30.5);
  TH1F *hRap = new TH1F("hRap","MC rapidity",60,0.0,6.0);
  TH1F *hVtx = new TH1F("hVtx","MC start vertex x",100,-50.0,50.0);
  TH1F *hPtd = new TH1F("hPtd","Pt-difference MC - matched",100,-0.05,0.05);

  // Open the ROOT MC mini-DST file
  Run->Open("/shift/na49i04/data1/christoph/test/test_1458.root");

  // Loop through the events
  Int_t iEvent = 0; 
  while ((MCEvent = (T49EventMCRoot *) Run->GetNextEvent()) && 
         (iEvent < maxEvent)) {        

    // Count the number of events
    iEvent++;

    // Get the list of all main-vertex tracks
    PriTrackList = MCEvent->GetPrimaryParticles();
    // Get the list of all simulated tracks
    MCTrackList  = MCEvent->GetMCParticles();
    // Get the list with the simulated vertices
    MCVertexList = MCEvent->GetMCVertices();

    cout << " Run "              << MCEvent->GetNRun()
         << " event "            << MCEvent->GetNEvent()
         << " with "             << MCTrackList->GetEntries()
         << " simulated tracks " << endl;

    // Loop through all simulated tracks
    for (Int_t iMCTrack = 0; iMCTrack < MCTrackList->GetEntries(); iMCTrack++) {
       
      // Get the MC track
      T49ParticleMCRoot *MCTrack = (T49ParticleMCRoot *) MCTrackList->At(iMCTrack);
      hPid->Fill(MCTrack->GetPid());
      hRap->Fill(MCTrack->GetRap());

      // Get the MC start vertex
      Int_t iStart = MCTrack->GetStartVertex();
      if (iStart >= 0) {
        T49VertexMCRoot *MCStart = (T49VertexMCRoot *) MCVertexList->At(iStart);
        if (MCTrack->GetPid() == 6) {
          hVtx->Fill(MCStart->GetX());
	}
      }

      // Get the matched primary tracks
      for (Int_t iMatch = 0; iMatch < MCTrack->GetNPriMatched(); iMatch++) {
        Int_t iRec = MCTrack->GetPriMatched(iMatch);
        T49ParticleRoot *RecTrack = (T49ParticleRoot *) PriTrackList->At(iRec);
        if (MCTrack->GetPid() == 9) {
          hPtd->Fill(MCTrack->GetPt() - RecTrack->GetPt());
	}
      }

    }

  }

  TCanvas *c1 = new TCanvas("c1","mcmacro",50,50,600,600);
  c1->Divide(2,2);
  c1->cd(1);
  hPid->Draw();
  c1->cd(2);
  hRap->Draw();
  c1->cd(3);
  hVtx->Draw();
  c1->cd(4);
  hPtd->Draw();

}
