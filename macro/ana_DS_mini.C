{  

  /////////////////////////////////////////////////////////////////////////
  //
  //  Example macro to demonstrate how to read DSPACK mini-DST data with 
  //  root49 using the TRootDS class.
  //
  /////////////////////////////////////////////////////////////////////////

  // Reset ROOT environment and switch on timer
  gROOT.Reset();
  gROOT.Time();

  // DSPACK mini-DST file
  Char_t  *fRun = "mini://CERN/NA49/DST/R96B/1470/AUG99/PRO";
 
  // Create a new TRootDS object. This provides the DSPACK interface
  TRootDS *RootDS = new TRootDS();
   
  // Number of events to be analyzed
  Int_t maxEvt = 100;

  Int_t handle;

  mini_event_t       *Event;
  mini_rtrack_t      *RTrack;
  mini_track_t       *Track;

  // Create the histograms
  TH1F *hPt = new TH1F("hPt","Transverse momentum all tracks",20,0.0,2.0);

  // Open the DSPACK file
  cout << "Trying to open: " << fRun << endl;
  RootDS->OpenDS(fRun);

  // Loop through the events
  Int_t iEvt = 0;
  while ((RootDS->ReadEventDS()) && (iEvt < maxEvt)) {

    iEvt++;

    // Get the pointer to the event_t structure
    Int_t nEvent = 0;
    Event = (mini_event_t *) RootDS->IdsgetDS("mini_event",&handle,&nEvent);
    cout << " Run = "   << Event->n_run
         << " event = " << Event->n_event << endl;     

    // Get the pointer to the rtracks
    Int_t nRTrack = 0;
    RTrack = (mini_rtrack_t *) RootDS->IdsgetDS("mini_rtrack",&handle,&nRTrack);

    // Loop through the rtracks
    for (Int_t iRTrack = 0; iRTrack < nRTrack; iRTrack++) {

      // Get the main vertex-fit of that rtrack
      Track = (mini_track_t *) RTrack->first_track_p;

      // Fill a pt-spectrum with all tracks
      hPt->Fill(TMath::Sqrt(Track->px*Track->px
                          + Track->py*Track->py));

      RTrack++;

    }

  }

  // Close DSPACK file
  RootDS->CloseDS();

  cout << "Analyzed " << iEvt << " events" << endl;

  hPt->Draw(); 

}







