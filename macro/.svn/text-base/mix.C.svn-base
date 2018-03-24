{ 
// T49 MIXER EXAMPLE MACRO 22.11.1999 //

gROOT->Reset();

  // Load libraries if root49 is not used
  if (gClassTable->GetID("T49Run") < 0) {
    printf("Load shared libraries for T49\n");
    gSystem->Load("libT49DST.so");
    gSystem->Load("libT49ANA.so");
  }

  // Define the T49Run object
  T49Run           *Run;
  if (gT49) {
    Run = gT49;
  }
  else
    Run = new T49Run();         

T49Mixer* MXR = new T49Mixer(100,100);    // MIXER FOR EVENT MIXING
         MXR->SetP1P2Mode(1);             // exclude P1P2(i,i)pairs 
T49Mixer* QXR = new T49Mixer(1500,1500);  // MIXER FOR TRACK MIXING

Char_t  InFile[1000]; sprintf(InFile,"%s","minbias_pbpb_99A");
Char_t  InC[10]; Float_t dPt;

TH1D* HSpm = new TH1D("HSingle+/-","HSingle+/-",100,-2.0,2.0);
TH1D* HXpm = new TH1D("HMixed+/-","HMixed+/-",100,-2.0,2.0);
TH1D* HSmm = new TH1D("HSinglUnique-/-","HSinglUnique-/-",100,-2.0,2.0);
TH1D* HXmm = new TH1D("HMixedUni-/-","HMixedUni-/-",100,-2.0,2.0);


const Int_t       maxEvent = 170;
      Int_t       nEvent = 0;
      Int_t       nPTracks, nNTracks,nPairs;

T49EventRoot     *Event;
T49ParticleRoot  *p1,*p2;
TClonesArray     *PrTracks; 
TObjArray        *PTracks, *NTracks, *PT, *NT;
T49CutTrack      *CuT = new T49CutTrack();
                  CuT->SetIflag(0);

while (Run->OpenNextRun(InFile,0) && (nEvent < maxEvent))
{ while ((Event = (T49EventRoot *) Run->GetNextEvent()) && (nEvent < maxEvent))
 {
   PrTracks = Event->GetPrimaryParticles();       // GET PRIMARY TRACKS

   CuT->SetCharge(+1);
   PTracks = CuT->GetAcceptedParticles(PrTracks); // GET POSITIVE TRACKS
   nPTracks = PTracks->GetEntries();
   CuT->SetCharge(-1);
   NTracks = CuT->GetAcceptedParticles(PrTracks); // GET NEGATIVE TRACKS
   nNTracks = NTracks->GetEntries();
   PrTracks->Clear();

   if(nPTracks*nNTracks > 0) {
     MXR->PutToP1(PTracks);         // STORE POSITIVE TRACKS INTO MXR Pool1
     MXR->PutToP2(NTracks);         // STORE NEGATIVE TRACKS INTO MXR Pool2
     nEvent++; if (!(nEvent%50)) cout << endl << nEvent; 
     if(MXR->IsFull()) cout << "x" << flush; else cout << "." << flush;
//////  THAT WAS TO FILL Mixer MXR ///////////////////
/////  NOW WE USE SECOND MIXER QXR TO MIX SINGLE EVENT AND MIXED EVENT TRACKS ///
//// SINGLE EVENT PAIRS //
     QXR->SetPools(PTracks,NTracks);                  // !! FASTER THEN 2 LINES BELOW !!
//     QXR->SetPool1(PTracks);                        // SET P1 of QXR  (don't ask why ..)
//     QXR->SetPool2(NTracks);                        // SET P2 of QXR

     nPairs = QXR->NofFreePairsP1P2();              // GET NUMBER OF P1P2 PAIRS
     for(int i=0;i<nPairs;i++) {                    // LOOP +/- Pairs
        QXR->GetNuPairP1P2(&p1,&p2);                  // GET PAIR
        dPt = p1->GetPt() - p2->GetPt();              // dPt 
        HSpm->Fill(dPt,1.0);                          // FILL HIST
     }                                              // ENDLOOP 
     nPairs = QXR->NofFreeUniquePairsP1P1();
     for(int i=0;i<nPairs;i++) {                    // LOOP -/- Unique Pairs
        QXR->GetRUniquePairP1P1(&p1,&p2);               // Random Unique
        dPt = p1->GetPt() - p2->GetPt(); 
        HSmm->Fill(dPt,1.0);   
     }                                              // ENDLOOP
     QXR->ClearPools();  // Important
   }
/// MIXED EVENT PAIRS //
   if (MXR->IsFull()) {
     for(int i=0;i<5;i++) {                         // 5x MIXED EVENT

       MXR->GetNuRPairP1P2(&PT,&NT);                  // GET MIXED P/N TRACKLISTS FROM MXR
         QXR->SetPools(PT,NT);                        // FASTER THEN TWO LINES BELOW
//       QXR->SetPool1(PT);
//       QXR->SetPool2(NT);
       nPairs = QXR->NofFreePairsP1P2();
       for(int j=0;j<nPairs;j++) {                    // LOOP +/- Pairs
          QXR->GetNuPairP1P2(&p1,&p2);
          dPt = p1->GetPt() - p2->GetPt();  
          HXpm->Fill(dPt,1.0);             
       }                                              // ENDLOOP
       nPairs = QXR->NofFreeUniquePairsP1P1();
       for(int j=0;j<nPairs;j++) {                    // LOOP -/- Unique Pairs
          QXR->GetRUniquePairP1P1(&p1,&p2);              // Random Unique
          dPt = p1->GetPt() - p2->GetPt(); 
          HXmm->Fill(dPt,1.0);
       }                                              // ENDLOOP
       QXR->ClearPools();
     }
////////// END OF MIXED EVENT //////// 
   }
} } ////// END OF RUN (2xwhile) LOOP

MXR->ShowP1P2(); 
cout << "\nPress Enter to delete Mixer & show Histograms:" << flush;
cin.getline(InC,5);

delete QXR;
MXR->SetVerbose(1);
delete MXR;

TCanvas* HSX = new TCanvas("HSX","T49MIXER EXAMPLE",600,600);
HSX->Divide(2,2);

HSX->cd(1); HSpm->Draw();
HSX->cd(2); HXpm->Draw();
HSX->cd(3); HSmm->Draw();
HSX->cd(4); HXmm->Draw();


} // END OF MACRO 
