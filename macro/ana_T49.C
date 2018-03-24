{  

  /////////////////////////////////////////////////////////////////////////
  //
  //  Example macro to demonstrate how to read DSPACK data and directly
  //  convert them into T49DST data structures with root49
  //  using the TRootDS class.
  //  It loops through all global main-vertex tracks and fills a
  //  pt-spectrum.
  //
  /////////////////////////////////////////////////////////////////////////

  // Reset ROOT environment and switch on timer
  gROOT.Reset();
  gROOT.Time();

  // DSPACK file
  Char_t  *fRun = "//CERN/NA49/DST/R96B/1464/AUG99/PRO/R1464_01-01.DS";
 
  // Create a new TRootDS object. This provides the DSPACK interface
  TRootDS *RootDS = new TRootDS();

  // Number of events to be analyzed
  Int_t maxEvt = 100;

  // Create the histograms
  TH1F *hPt = new TH1F("hPt","Transverse momentum all tracks",20,0.0,2.0);

  // Open the DSPACK file
  cout << "Trying to open: " << fRun << endl;
  RootDS->OpenDS(fRun);

  // Loop through the events
  Int_t iEvt = 0;
  while ((RootDS->ReadEventT49()) && (iEvt < maxEvt)) {

    iEvt++;
    cout << " Analyzing event no. " << iEvt << endl;

    // Get the event
    T49EventRoot *Event = RootDS->GetEvent();

    // Get the list of main-vertex tracks
    TClonesArray *TrackList = Event->GetPrimaryParticles();

    // Loop through the tracks
    for (Int_t iTrack = 0; iTrack < TrackList->GetEntries(); iTrack++) {

      // Get one track
      T49ParticleRoot *Track = (T49ParticleRoot *) TrackList->At(iTrack);
      
      // Fill the spectrum
      hPt->Fill(Track->GetPt());

    }

  }

  // Close DSPACK file
  RootDS->CloseDS();

  cout << "Analyzed " << iEvt << " events" << endl;

  hPt->Draw(); 

}







