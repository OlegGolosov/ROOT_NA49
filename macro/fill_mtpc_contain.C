{{
//  This macro fills a T49Container with dE/dx data, using MTPC dE/dx 
//  values. The macro is written to a file, and can subsequentlt be fitted, 
//  using e.g. fit_mtpc.C

  gROOT->Reset();

  // Specify number of events to be read
  Int_t max_event=10000;

  // Specify runtype
  Char_t* RunType="central-full-chain-std+-98h-tcal";

  // Event cut
  T49CutEvent *cutevent = new T49CutEvent();
  cutevent->SetStandardCutsPbPb();

  // Track quality cuts 
  T49CutTrack *cuttrack = new T49CutTrack();
  cuttrack->SetStandardCutsPbPb();
  cuttrack->SetNDedxPoint(2,60,150);
  cuttrack->SetNPointToNMaxPoint(3,0.3);

  // Open outputfile
  TFile fout("contain_mtpc.root","RECREATE");
  
  T49Container *mtpcC = new T49Container();
  mtpcCont->Init(40,1,120,10,0,2,8);  
  mtpcCont->SetTPC(2);
  
  T49Run *run = new T49Run();
  run->SetVerbose(0);

  T49EventRoot *event;
  T49ParticleRoot *particle;
  TClonesArray *Particles;
  TObjArray *AccPart;
  
  Int_t num = 0;
  Int_t event_cnt = 0;
  Int_t mult;
  Int_t mtpc_cnt=0;

  // Loop over events and fill container

  while((event = (T49EventRoot *) run->GetNextEvent(RunType))  
	&& (event_cnt <= max_event)) {
    if (cutevent->CheckEvent(event)) {
      Particles = event->GetPrimaryParticles();
      AccPart = cuttrack->GetAcceptedParticles(Particles);
      event_cnt++;
      TIter NextTrack(AccPart);
      while(particle = (T49ParticleRoot *) NextTrack()) {
	mtpcC->FillContainer(particle);
	mtpc_cnt++;
	mult++;
      }
    }
    if(num%50==0)
      printf("Event %d %d(%d) (collected Parts: %d)\n",event->GetNRun(), event->GetNEvent(),num,mtpc_cnt);
    num++;
  }

  // Write results to file

  run->Close();
  fout.cd();  
  printf("Write mtpc_cont\n");
  mtpcC->Write("mtpc_cont",1);
  printf("mtpc written \n");
  fout.Close();
}}





