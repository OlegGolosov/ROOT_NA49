{  

  /////////////////////////////////////////////////////////////////////////
  //
  //  Example macro to demonstrate how to read DSPACK data with root49
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

  Int_t handle;

  event_t       *Event;
  g_track_ref_t *TrackRef;
  g_ptr_list_t  *PtrList;
  rtrack_t      *RTrack;
  track_t       *Track;

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
    Event = (event_t *) RootDS->IdsgetDS("event",&handle,&nEvent);
    cout << " Run = "   << Event->n_run
         << " event = " << Event->n_event << endl;     

    // Get the pointer to the global tracks
    Int_t nTrackRef;
    TrackRef = (g_track_ref_t *) RootDS->IdsgetDS("g_track_ref",&handle,&nTrackRef);

    // Loop through the global tracks
    for (Int_t iTrackRef = 0; iTrackRef < nTrackRef; iTrackRef++) {

      if ((PtrList = (g_ptr_list_t *) TrackRef[iTrackRef].g_ptr_list_p) &&
          (RTrack  = (rtrack_t *)     PtrList->rtrack_glb_p)            &&
          (Track   = (track_t *)      RTrack->first_track_p)) {

        hPt->Fill(TMath::Sqrt(Track->px*Track->px
                            + Track->py*Track->py));

      }

    }

  }

  // Close DSPACK file
  RootDS->CloseDS();

  cout << "Analyzed " << iEvt << " events" << endl;

  hPt->Draw(); 

}







