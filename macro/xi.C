void xi()
{

  ////////////////////////////////////////////////////////////////////////////////
  //                                                                            //
  //  Example macro for a xi analysis. It demonstates how to read the           //
  //  xi vertices from the ROOT mini-DSTs and how to access the information     //
  //  connected to it:                                                          //
  //                                                                            //
  //   Xi-vertex                                     (T49VertexRoot object)     //
  //    |                                                                       //
  //    |-> Charged daughter track                   (T49ParticleRoot object)   //
  //    |                                                                       //
  //    |-> Neutral daughter track (lambda)          (T49ParticleRoot object)   //
  //           |                                                                //
  //           |-> Lambda vertex                     (T49VertexRoot object)     //
  //                  |                                                         //
  //                  |-> Positive daughter track    (T49ParticleRoot object)   //
  //                  |                                                         //
  //                  |-> Negative daughter track    (T49ParticleRoot object)   //
  //                                                                            //
  ////////////////////////////////////////////////////////////////////////////////

  // Reset the ROOT environment
  gROOT.Reset();

  // Load libraries if root49 is not used
  if (gClassTable->GetID("T49Run") < 0) {
    printf("Load shared libraries for T49\n");
    gSystem->Load("libT49DST.so");
    gSystem->Load("libT49ANA.so");
  }

  T49EventRoot     *Event;
  TObjArray        *SecVertexList;

  // Define the T49Run object
  T49Run           *Run;
  if (gT49) {
    Run = gT49;
  }
  else {
    Run = new T49Run();         
  }

  // maximum number of events to process
  const Int_t       maxEvent = 100000;

  TH1F *hXiMinv   = new TH1F("hXiMinv"  ,"Invariant mass Xi"            ,120,1.20,1.44);
  TH1F *hXiMinvC  = new TH1F("hXiMinvC" ,"Invariant mass Xi (cut)"      ,120,1.20,1.44);
  TH1F *hLamMinv  = new TH1F("hLamMinv" ,"Invariant mass lambda"        , 60,1.08,1.20); 
  TH1F *hLamMinvC = new TH1F("hLamMinvC","Invariant mass lambda (cut)"  , 60,1.08,1.20); 

  // Event loop
  Int_t iEvent = 0;
  while ((Event = (T49EventRoot *) Run->GetNextEvent("Xi-test")) &&
         (iEvent < maxEvent)) {                                                 
  
    // Get the list with the secondary vertices 
    SecVertexList = Event->GetSecondaryVertices();

    if (!(iEvent % 100)) {
      cout << " Run "                << Event->GetNRun()
           << " event "              << Event->GetNEvent()
           << " with "               << SecVertexList->GetEntries()
           << " secondary vertices " << endl;
    }

    // Loop through all secondary vertices
    for (Int_t idx = 0; idx < SecVertexList->GetEntries(); idx++) {

      T49VertexRoot *Xi = (T49VertexRoot *) SecVertexList->At(idx);

      // Check for Xi vertex id
      if (Xi->GetIdVtx() == 41) {

	// The daughter tracks of the Xi (pion/kaon and lambda)
        T49ParticleRoot *Charged = Xi->GetFullChargedXiTrack();
        T49ParticleRoot *Neutral = Xi->GetFullNeutralXiTrack();

	// The daughter lambda vertex
        Int_t index = Neutral->GetStopVertexIndex();
        T49VertexRoot *V0 = (T49VertexRoot *) SecVertexList->At(index);

	// The daughter tracks of the lambda
        T49ParticleRoot *Positive = V0->GetFullPositiveTrack();
        T49ParticleRoot *Negative = V0->GetFullNegativeTrack();

	// Fill the invariant mass spectra
        hXiMinv->Fill(Xi->GetInvariantMassXi());
        hLamMinv->Fill(V0->GetInvariantMassLambda());

	// Apply some cuts to reduce the background
        if ((Xi->GetXTarg()   <    1.0) &&
            (Xi->GetYTarg()   <    3.0) &&
            (Xi->GetZ()       > -500.0) &&
            (Xi->GetX1minX2() >    2.5) &&
            (V0->GetZ()       > -400.0)) {
          hXiMinvC->Fill(Xi->GetInvariantMassXi());
          hLamMinvC->Fill(V0->GetInvariantMassLambda());
	}         

      }

    }

    // Count the number of events
    iEvent++;

  }

  TCanvas *cXi = new TCanvas("cXi","Xi",50,50,600,600);
  cXi->Divide(2,2);
  cXi->cd(1);
  hXiMinv->Draw();
  cXi->cd(2);
  hLamMinv->Draw();
  cXi->cd(3);
  hXiMinvC->Draw();
  cXi->cd(4);
  hLamMinvC->Draw();

}
