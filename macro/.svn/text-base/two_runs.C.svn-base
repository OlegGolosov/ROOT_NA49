{

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
//   Example macro to demonstrate how to process two different runs at the same time.     //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

  gROOT->Reset();
  gROOT->Time();
 
  // Load libraries if root49 is not used
  if (gClassTable->GetID("T49Run") < 0) {
    printf("Load shared libraries for T49\n");
    gSystem->Load("libT49DST.so");    // Load root DST library
    gSystem->Load("libT49ANA.so");    // Load root analysis library
  }

  const Int_t maxEvent1 = 10;
  const Int_t maxEvent2 = 5;
   
  // !!!!!!!!!!!!!
  // Important to allow for different TClonesArrays in different Events
  // at the same time !!!!
  gT49KeepEvent = kTRUE;
  // !!!!!!!!!!!!!
   
  // Define the first T49Run object
  T49Run *Run1 = new T49Run();

  // Define the second T49Run object
  T49Run *Run2 = new T49Run();

  T49EventRoot    *Event1, *Event2;  
  TClonesArray    *TrackList1, *TrackList2;  
  TObjArray       *SecVertexList1, *SecVertexList2;
     
  Char_t *inFile1 = "/d/alice/na49/DST/00B/t49run1389.0.root";
  Char_t *inFile2 = "/d/alice/na49/DST/00B/t49run1444.0.root";

  // Open Root-DST run 1
  Run1->Open(inFile1);

  // Open Root-DST run 2
  Run2->Open(inFile2);

  Int_t nEvent1 = 0;
  Int_t nEvent2 = 0;

  // Loop over the events of the first run
  while ((Event1 = (T49EventRoot *) Run1->GetNextEvent()) && 
         (nEvent1 < maxEvent1)) {        

    nEvent1++;  

    // Get main-vertex track list of the first event
    TrackList1 = Event1->GetPrimaryParticles();

    // Get the list with the secondary vertices of the first event
    SecVertexList1 = Event1->GetSecondaryVertices();

    T49ParticleRoot *Track1 = (T49ParticleRoot *) TrackList1->At(0);
    T49VertexRoot   *V01    = (T49VertexRoot *)   SecVertexList1->At(0);

    // Loop over the events of the second run
    while ((Event2 = (T49EventRoot *) Run2->GetNextEvent()) &&
           (nEvent2 < maxEvent2)) {

      nEvent2++;

      // Get main-vertex track list of the second event
      TrackList2 = Event2->GetPrimaryParticles();

      // Get the list with the secondary vertices of the second event
      SecVertexList2 = Event2->GetSecondaryVertices();
      
      T49ParticleRoot *Track2 = (T49ParticleRoot *) TrackList2->At(0);
      T49VertexRoot   *V02    = (T49VertexRoot *)   SecVertexList2->At(0);

      printf("Run1 %d event %d %d tracks\n",Event1->GetNRun()
                                           ,Event1->GetNEvent()
                                           ,TrackList1->GetEntries());

      // Neccessary, because T49VertexRoot accesses the V0 daughter tracks 
      // via the global pointer to the corresponding T49Run object
      gT49 = Run1;
      printf("V01: minv(lambda) = %g\n",V01->GetInvariantMassLambda());

      printf("Run2 %d event %d %d tracks\n",Event2->GetNRun()
                                           ,Event2->GetNEvent()
                                           ,TrackList2->GetEntries());  

      // Neccessary, because T49VertexRoot accesses the V0 daughter tracks 
      // via the global pointer to the corresponding T49Run object
      gT49 = Run2;
      printf("V02: minv(lambda) = %g\n",V02->GetInvariantMassLambda());

    }

    nEvent2 = 0;

  }                   

}








































































