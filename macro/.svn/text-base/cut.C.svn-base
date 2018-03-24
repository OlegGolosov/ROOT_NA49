{

    ////////////////////////////////////////////////////////////////
    //
    //  Analysis example macro.
    //  Demomstrates the access the ROOT mini-DST data and
    //  the use of the cut-classes.
    //  Origin: C.Markert
    //
    ////////////////////////////////////////////////////////////////
   
    gROOT->Reset();
    gROOT->Time();
 
    // Load libraries if root49 is not used
    if (gClassTable->GetID("T49Run") < 0) {
      printf("Load shared libraries for T49\n");
      gSystem->Load("libT49DST.so");    // Load root DST library
      gSystem->Load("libT49ANA.so");    // Load root analysis library
    }

    Int_t i;
    Int_t nevent = 0;
    Int_t nmaxevent = 10;
         
    // Define the T49Run object
    T49Run *run;
    if (gT49) {
      run = gT49;
    }
    else
      run = new T49Run();         
      
    T49CutEvent     *cutevent  = new T49CutEvent("eventcut","Event cuts");
    T49EventRoot    *event;  

    TClonesArray    *AllTrackList;  

    T49CutTrack     *cuttrack = new T49CutTrack("trackcut","Track cuts");
    TObjArray       *CutTrackList;
    T49ParticleRoot *track; 

    // Create 1d histogram
    TH1F *pt = new TH1F("pt","pt",100,0,2);
     
    // Set event cuts   
    cutevent->SetVertexIflag(0); // good events VertexIflag = 0 
    cutevent->SetMult(800,1650);     
    cutevent->SetVerbose(1);

    // Set track cuts 
    cuttrack->SetCharge(-1);
    cuttrack->SetBx(-5,5);         
    cuttrack->SetBy(-2,2); 
    cuttrack->SetNPoint(3,20, 235);
    cuttrack->SetP(3,100); 
     
    // Open Root-DST run and event loop    
    while ((event = (T49EventRoot *) run->GetNextEvent("n-minbias-std+-99D-B")) && 
           (nevent < nmaxevent)) {        

      nevent++;  
      printf("number of event: %d  \n",nevent);

      // Get main-vertex track list 
      AllTrackList = event->GetPrimaryParticles();
      printf("Run %d event %d %d tracks\n",event->GetNRun(),
                                           event->GetNEvent(),
                                           AllTrackList->GetEntries());  
                            
      // Check events with given event cuts
      if(cutevent->CheckEvent(event) == 1)   // == 1 event is OK
      { 	     
        // Get accepted track list
        CutTrackList = cuttrack->GetAcceptedParticles(AllTrackList);           
                                                        
        printf( "number of accepted tracks = %d \n",CutTrackList->GetEntries());

        // Loop over all accepted tracks
        for(i=0;i< CutTrackList->GetEntries();i++)                  
        {
          track = ((T49ParticleRoot *)CutTrackList->At(i));
          pt->Fill(track->GetPt());  // Fill histogram                                 
        }
      }

      cout << "Next event" << endl;

    }
                   
    // Draw the histogram
    pt->Draw();

    // Create outputfile
    printf("Write to file ! \n");
    TFile *hfile = TFile::Open("cutmacro.root", "RECREATE", "file with histograms");
    pt->Write();       // Write histogram to file    
    cutevent->Write(); // Write the event cuts
    cuttrack->Write(); // Write the track cuts
    hfile->Close();    // Close file

    printf("Program END ! \n");

}








































































