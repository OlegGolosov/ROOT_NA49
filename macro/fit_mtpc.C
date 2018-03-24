{{
//  Macro to fit an already filled T49Container. The procedure is
//  optimised for the MTPC dE/dx data, but no care hgas been taken
//  to fit the electron-peak correctly. The procedure runs per
//  momentum-bin, and does the following: 
//
//  1. For every Charge, Pt and Phi-bin having more than 1000
//  entries, initialise T49DedxFunction and fit, using constant
//  relative proton, kaon en electron-peakposition.
//
//  2. Use the histograms with a large proton peak to determine the
//  relative proton peakpos. And the same for the kaons.
//
//  3. Refit all bins, using a free proton/kaon position, and check
//  whether the result agrees with the found positions. If so, keep
//  fit, if not, try again, fixing proton or kaon pos.
//
//  The results are stored in the container, which is written to a new
//  file. Also a list of fit-parameters is written to a separate file
//  as a TTree.

  gROOT->Reset();

  // Read input container

  TFile fin("contain_mtpc.root");
  T49Container* mtpcC = (T49Container*) fin.Get("mtpc_cont");

  gMinuit->SetMaxIterations(2500);

  TH1F* tmpH;
  T49DedxFunction* tmpF;
  TAxis* PtotAxis;
  T49DedxInfo* Info;

  // Initialise TTree for fitresults.

  TTree *NT = new TTree("NT","fit results");
  Int_t ChargeBin;
  Int_t PtotBin;
  Int_t PtBin;
  Int_t PhiBin;
  Int_t NEnt;
  Float_t ptot;   
  NT->Branch("ChargeBin",&ChargeBin,"ChargeBin/I");
  NT->Branch("PtotBin",&PtotBin,"PtotBin/I");
  NT->Branch("PtBin",&PtBin,"PtBin/I");
  NT->Branch("PhiBin",&PhiBin,"PhiBin/I");
  NT->Branch("NEnt",&NEnt,"NEnt/I");
  NT->Branch("Ptot",&ptot,"Ptot");

  Float_t FixPoKa;
  Float_t FreeRes,FreePAmp,FreePPos,FreeKaAmp,FreeKaPos,FreePiAmp;
  Float_t FreePiPos,FreeElAmp,FreeElPos,FreeChiSq;

  NT->Branch("FixPoKa",&FixPoKa,"FixPoKa");
  NT->Branch("FreeRes",&FreeRes,"FreeRes");
  NT->Branch("FreePPos",&FreePPos,"FreePPos");
  NT->Branch("FreePAmp",&FreePAmp,"FreePAmp");
  NT->Branch("FreeKaPos",&FreeKaPos,"FreeKaPos");
  NT->Branch("FreeKaAmp",&FreeKaAmp,"FreeKaAmp");
  NT->Branch("FreePiPos",&FreePiPos,"FreePiPos");
  NT->Branch("FreePiAmp",&FreePiAmp,"FreePiAmp");
  NT->Branch("FreeElPos",&FreeElPos,"FreeElPos");
  NT->Branch("FreeElAmp",&FreeElAmp,"FreeElAmp");
  NT->Branch("FreeChiSq",&FreeChiSq,"FreeChiSq");

  Float_t NP,NKa,NPi,NEl,NAll;
  TCanvas *c1 = new TCanvas("c1","Canvas #1",500,600);

  Char_t name[255];
  TObjArray Indices(2*10*8);
  TObjArray FitFuncs(2*10*8);
  Float_t PoverKa[2*10*8];
  Int_t Index[2*10*8];
  T49Index *myI= new T49Index();
  T49Index *I;

  // PtotBin is the outer for-loop.

  for (PtotBin=0;PtotBin<40;PtotBin++) {
    myI->SetPtotBin(PtotBin);
    Int_t NFits=0;
    for (ChargeBin=0;ChargeBin<2;ChargeBin++) {
      myI->SetChargeBin(ChargeBin);
      for (PtBin=0;PtBin<10;PtBin++) {
	myI->SetPtBin(PtBin); 
	for (PhiBin=0;PhiBin<8;PhiBin++) {
	  myI->SetPhiBin(PhiBin);
	  
	  // Get and draw histgram for current bin

	  tmpH = mtpcC->GetHist(myI);
	  tmpH->Draw();
	  c1->Update();
	  mtpcC->FindPhase(myI);
	  NEnt=tmpH->Integral();

	  // Start fitting procedure if more than 1000 entries

	  if (NEnt>1000) {
	    // Store Index and function for later retrieval

	    delete Indices[NFits];
	    Indices[NFits]=new T49Index(*myI);
	    delete FitFuncs[NFits];
	    sprintf(name,"fitfunc%d",NFits);
	    tmpF= new T49DedxFunction(name);
	    FitFuncs[NFits]=tmpF;
	    ptot = myI->GetPtot();
	    tmpF->SetPtot(ptot);
	    printf("Fitting MTPC, ChargeBin: %d, PtotBin: %d, PtBin: %d, PhiBin: %d, fixed parameters. \n",ChargeBin,PtotBin,PtBin,PhiBin);

	    // Initialise and fit.

	    tmpF->Init(tmpH);
	    if (ptot>20) {
	      printf("Setting electron amplitude to 0 (Ptot>20 GeV)\n");
	      tmpF->SetFixedAmplitude(0,0);
	    }
	    tmpH->Fit(tmpF->GetName(),"QBNR");
	    if (!gMinuit->fCstatu->Contains("CONVERGED")) {
	      printf("No convergence at fitting, routine Fit \n");
	      printf("Minuit return string: %s\n",gMinuit->fCstatu->Data());
	    }
	    tmpF->Draw("same");
	    c1->Update();

	    // Store ratio of proton and kaon peak 
 
	    PoverKa[NFits]=tmpF->GetAmplitude(3)/tmpF->GetAmplitude(2);
	    tmpF->SetFree();
	    if (tmpF->GetAmplitude(3)>0.7*tmpF->GetAmplitude(1)) {
	      tmpF->SetFixedAmplitude(2,0);
	      PoverKa[NFits]=tmpF->GetAmplitude(3);
	    }
	    Float_t MaxAmp=TMath::Max(tmpF->GetAmplitude(1),tmpF->GetAmplitude(3));
	    for (Int_t p=0;p<4;p++)
	      if ((MaxAmp>100&&tmpF->GetAmplitude(p)<10)
		  || (tmpF->GetAmplitude(p)<0.01*MaxAmp)) {
		tmpF->SetFixedAmplitude(p,0);
		if (p==1) printf("WARNING: low pion amplitude \n");
		if (p==2) PoverKa[NFits]=tmpF->GetAmplitude(3);
	      }
	    tmpF->StoreParameters();
	    NFits++;
	  }
	}
      }
    }

    // All histograms in the Ptot-bin have been fitted for a first time.
    // Now it really starts!!

    Float_t MeanPRelPos=0;
    Float_t SumSqPRelPos=0;
    Float_t RelPos[10*8];
    Int_t NFreeFits=0;
    
    if (NFits>0&&NFits<=3) {
      printf("WARNING: only three or less fits in this PtotBin\n");
    }
    
    if (NFits>0) {
      TMath::Sort(NFits,PoverKa,Index);   // Sorts in descending order
      if (NFits>3) {

	// Find relative proton position, by doing free fits to the 
	// histos with the larger P-to-Ka-ratio.

	Float_t MaxPoverKa=PoverKa[Index[0]];
	printf("Determining proton peak pos...\n");
	Int_t i=0;
	while (i<=NFits/3 && PoverKa[Index[i]]>0.75) {
	  Int_t Idx=Index[i];
	  I=(T49Index*)Indices[Idx];
	  tmpH = mtpcC->GetHist(I);
	  tmpH.Draw();
	  tmpF=(T49DedxFunction*) FitFuncs[Idx];
	  tmpF->SetFixedPosition(2);
	  if (!tmpF->IsAmplitudeFixed(3)){
	    printf("Fitting MTPC, ChargeBin: %d, PtotBin: %d, PtBin: %d, PhiBin: %d, fixed kaon position. \n",I->GetChargeBin(),I->GetPtotBin(),I->GetPtBin(),I->GetPhiBin());
	    tmpH->Fit(tmpF->GetName(),"QBNR");
	    if (!gMinuit->fCstatu->Contains("CONVERGED")) {
	      printf("No convergence at fitting, routine Fit \n");
	      printf("Minuit return string: %s\n",gMinuit->fCstatu->Data());
	    }
	    RelPos[NFreeFits]=tmpF->GetPosition(3)/tmpF->GetPosition(1);
	    MeanPRelPos+=RelPos[NFreeFits];
	    SumSqPRelPos+=RelPos[NFreeFits]*RelPos[NFreeFits];
	    NFreeFits++;
	  }
	  tmpF->Draw("same");
	  c1->Update();
	  i++;
	}
      }
      Float_t StDevPRelPos;
      Bool_t Done=0;
      Int_t N=NFreeFits;
      while (!Done) {
	Done=1;
	if (N>2) {
	  MeanPRelPos /= N;
	}
	else {
	  printf("No proton peak position could be determined. Using default value\n");
	  MeanPRelPos=tmpF->GetRelRise(3)/tmpF->GetRelRise(1);
	}
	if (N>3) {
	  StDevPRelPos=sqrt((SumSqPRelPos-N*MeanPRelPos*MeanPRelPos)/N);
	  
	}
	else {
	  printf("No stddev could be determined. Using default value\n");
	  StDevPRelPos=0.01*MeanPRelPos;
	}
	if (N>5) {
	  SumSqPRelPos=0;
	  Float_t Mean=0;
	  Int_t RejCount=0;
	  for (Int_t i=0;i<NFreeFits;i++) {
	    if (TMath::Abs(RelPos[i]-MeanPRelPos)<3*StDevPRelPos) {
	      Mean+=RelPos[i];
	      SumSqPRelPos+=RelPos[i]*RelPos[i];
	    }
	    else RejCount++;
	  }
	  if (RejCount>NFreeFits-N) {
	    printf("# proton positions rejected: %d \n",RejCount);
	    printf("Old mean proton pos: %f\n",MeanPRelPos);
	    printf("Old standard dev:  %f \n",StDevPRelPos);
	    Done=0; // Once again....
	    N=NFreeFits-RejCount;
	    MeanPRelPos=Mean;
	  }
	}
      }
      printf("Number of free fits: %d \n",N);
      printf("Average proton position: %f \n",MeanPRelPos);
      printf("Standard dev of proton pos: %f\n",StDevPRelPos);
      
      // Done with proton position. Same procedure for kaon postion.

      Float_t MeanKaRelPos=0;
      Float_t SumSqKaRelPos=0;
      Int_t NFreeFits=0;
      if (NFits>3) {
	printf("Determining kaon position...\n");
	Int_t i=NFits-1;
	while (i>=2*NFits/3&&PoverKa[Index[i]]<1.25) {
	  Int_t Idx=Index[i];
	  // if (PoverKa[Idx]>0.5*MaxPoverKa) printf("PoverKa unexpectedly high\n");
	  I=(T49Index*)Indices[Idx];
	  tmpH = mtpcC->GetHist(I);
	  tmpH.Draw();
	  tmpF=(T49DedxFunction*) FitFuncs[Idx];
	  //tmpF->SetFreePosition(3);
	  tmpF->RestoreParameters();
	  if (!tmpF->IsAmplitudeFixed(2)){
	    printf("Fitting MTPC, ChargeBin: %d, PtotBin: %d, PtBin: %d, PhiBin: %d, free parameters. \n",I->GetChargeBin(),I->GetPtotBin(),I->GetPtBin(),I->GetPhiBin());
	    tmpH->Fit(tmpF->GetName(),"QBNR");
	    if (!gMinuit->fCstatu->Contains("CONVERGED")) {
	      printf("No convergence at fitting, routine Fit \n");
	      printf("Minuit return string: %s\n",gMinuit->fCstatu->Data());
	    }
	    tmpF->Draw("same");
	    c1->Update();
	    RelPos[NFreeFits]=tmpF->GetRelPosition(2);
	    if (TMath::Abs(tmpF->GetRelPosition(3)-MeanPRelPos)>3*StDevPRelPos){
	      printf("Refitting MTPC, ChargeBin: %d, PtotBin: %d, PtBin: %d, PhiBin: %d, fixed proton position. \n",I->GetChargeBin(),I->GetPtotBin(),I->GetPtBin(),I->GetPhiBin());
	      tmpF->RestoreParameters();
	      tmpF->SetFixedPosition(3,MeanPRelPos*tmpF->GetPosition(1));
	      tmpF->ResetFreePosition(2);
	      tmpH->Fit(tmpF->GetName(),"QBNR");
	      if (!gMinuit->fCstatu->Contains("CONVERGED")) {
		printf("No convergence at fitting, routine Fit \n");
		printf("Minuit return string: %s\n",gMinuit->fCstatu->Data());
	      }
	      tmpF->Draw("same");
	      c1->Update();
	      RelPos[NFreeFits]=tmpF->GetRelPosition(2);
	    }
	    MeanKaRelPos+=RelPos[NFreeFits];
	    SumSqKaRelPos+=RelPos[NFreeFits]*RelPos[NFreeFits];
	    
      	    NFreeFits++;
	  }
	  i--;
	}
      }
      Float_t StDevKaRelPos;
      Bool_t Done=0;
      Int_t N=NFreeFits;
      while (!Done) {
	Done=1;
	if (N>2) {
	  MeanKaRelPos /= N;
	}
	else {
	  printf("No kaon peak position could be determined. Using default value\n");
	  MeanKaRelPos=tmpF->GetRelRise(2)/tmpF->GetRelRise(1);

	}
	if (N>3) {
	  StDevKaRelPos=sqrt((SumSqKaRelPos-N*MeanKaRelPos*MeanKaRelPos)/N);
	}
	else {
	  printf("No stddev could be determined. Using default value\n");
	  StDevKaRelPos=0.01*MeanKaRelPos;
	}
	if (N>5) {
	  SumSqKaRelPos=0;
	  Float_t Mean=0;
	  Int_t RejCount=0;
	  for (Int_t i=0;i<NFreeFits;i++) {
	    if (TMath::Abs(RelPos[i]-MeanKaRelPos)<3*StDevKaRelPos) {
	      Mean+=RelPos[i];
	      SumSqKaRelPos+=RelPos[i]*RelPos[i];
	    }
	    else RejCount++;
	  }
	  if (RejCount>NFreeFits-N) {
	    printf("# kaon positions rejected: %d \n",RejCount);
	    printf("Old mean kaon pos: %f\n",MeanPRelPos);
	    printf("Old standard dev:  %f \n",StDevPRelPos);
	    Done=0; // Once again....
	    N=NFreeFits-RejCount;
	    MeanKaRelPos=Mean;
	  }
	}
      }
      printf("Number of free fits: %d \n",N);
      printf("Average kaon position: %f \n",MeanKaRelPos);
      printf("Standard dev of kaon pos: %f\n",StDevKaRelPos);

      // Done with kaon position.

      // Loop over all fits and see whether proton and kaon position 
      // come out right. 

      for (Int_t i=0;i<NFits;i++) {
	Int_t Idx=Index[i];
	I=(T49Index*)Indices[Idx];
	tmpH = mtpcC->GetHist(I);
	tmpH.Draw();
	tmpF=(T49DedxFunction*) FitFuncs[Idx];
	printf("Fitting MTPC, ChargeBin: %d, PtotBin: %d, PtBin: %d, PhiBin: %d, free parameters. \n",I->GetChargeBin(),I->GetPtotBin(),I->GetPtBin(),I->GetPhiBin());
	tmpF->RestoreParameters();
	tmpH->Fit(tmpF->GetName(),"QBNR");
	if (!gMinuit->fCstatu->Contains("CONVERGED")) {
	  printf("No convergence at fitting, routine Fit \n");
	  printf("Minuit return string: %s\n",gMinuit->fCstatu->Data());
	}
	tmpF->Draw("same");
	c1->Update();
       	if (TMath::Abs(tmpF->GetRelPosition(3)-MeanPRelPos)>3*StDevPRelPos){
	  printf("Refitting MTPC, ChargeBin: %d, PtotBin: %d, PtBin: %d, PhiBin: %d, fixed proton position. \n",I->GetChargeBin(),I->GetPtotBin(),I->GetPtBin(),I->GetPhiBin());
	  tmpF->RestoreParameters();
	  tmpF->SetFixedPosition(3,MeanPRelPos*tmpF->GetPosition(1));
	  tmpF->SetFreePosition(2,MeanKaRelPos*tmpF->GetPosition(1));
	  tmpH->Fit(tmpF->GetName(),"QBNR");
	  if (!gMinuit->fCstatu->Contains("CONVERGED")) {
	    printf("No convergence at fitting, routine Fit \n");
	    printf("Minuit return string: %s\n",gMinuit->fCstatu->Data());
	  }
	  tmpF->Draw("same");
	  c1->Update();
	}
	if (TMath::Abs(tmpF->GetRelPosition(2)-MeanKaRelPos)>3*StDevKaRelPos){
	  printf("Refitting MTPC, ChargeBin: %d, PtotBin: %d, PtBin: %d, PhiBin: %d, fixed kaon position. \n",I->GetChargeBin(),I->GetPtotBin(),I->GetPtBin(),I->GetPhiBin());
	  tmpF->RestoreParameters();
	  tmpF->SetFreePosition(3,MeanPRelPos*tmpF->GetPosition(1));
	  tmpF->SetFixedPosition(2,MeanKaRelPos*tmpF->GetPosition(1));
	  tmpH->Fit(tmpF->GetName(),"QBNR");
	  if (!gMinuit->fCstatu->Contains("CONVERGED")) {
	    printf("No convergence at fitting, routine Fit \n");
	    printf("Minuit return string: %s\n",gMinuit->fCstatu->Data());
	  }
	  tmpF->Draw("same");
	  c1->Update();
	}
	if (TMath::Abs(tmpF->GetRelPosition(3)-MeanPRelPos)>3*StDevPRelPos){
	  printf("Refitting MTPC, ChargeBin: %d, PtotBin: %d, PtBin: %d, PhiBin: %d, fixed proton and kaon position. \n",I->GetChargeBin(),I->GetPtotBin(),I->GetPtBin(),I->GetPhiBin());
	  tmpF->RestoreParameters();	  
	  tmpF->SetFixedPosition(3,MeanPRelPos*tmpF->GetPosition(1));
	  tmpF->SetFixedPosition(2,MeanKaRelPos*tmpF->GetPosition(1));
	  tmpH->Fit(tmpF->GetName(),"QBNR");
	  if (!gMinuit->fCstatu->Contains("CONVERGED")) {
	    printf("No convergence at fitting, routine Fit \n");
	    printf("Minuit return string: %s\n",gMinuit->fCstatu->Data());
	  }
	  tmpF->Draw("same");
	  c1->Update();
	}

	// Store all parameters for TTree

	FreeChiSq = tmpF->GetChisquare()/tmpF->GetNDF();
	FreeElAmp = tmpF->GetAmplitude(0);
	FreeElPos = tmpF->GetPosition(0);
	FreePiAmp = tmpF->GetAmplitude(1);
	FreePiPos = tmpF->GetPosition(1);
	FreeKaAmp = tmpF->GetAmplitude(2);
	FreeKaPos = tmpF->GetPosition(2);
	FreePAmp = tmpF->GetAmplitude(3);
	FreePPos = tmpF->GetPosition(3);
	FreeRes   = tmpF->GetResolution();
	FixPoKa=PoverKa[Idx];
	NEnt=tmpH->GetEntries();

	// Store parameters in info-records of the container

	Info=mtpcC->GetInfo(I);
	Info->SetPosition(FreeElPos,0);
	Info->SetPosition(FreePiPos,1);
	Info->SetPosition(FreeKaPos,2);
	Info->SetPosition(FreePPos,3);
	Info->SetAmplitude(FreeElAmp,0);
	Info->SetAmplitude(FreePiAmp,1);
	Info->SetAmplitude(FreeKaAmp,2);
	Info->SetAmplitude(FreePAmp,3);
	Info->SetReso(FreeRes);
	Info->SetChiSq(FreeChiSq);

	ChargeBin=I->GetChargeBin();
	PtBin=I->GetPtBin();
	PhiBin=I->GetPhiBin();
	NT.Fill();
      }
    }
    // All refits done (1 momentum bin)
  }

  // All mometum bins done

  // Write result to files

  TFile fout("fit_mtpc_result.root","RECREATE");
  fout.cd();
  NT.Write();
  fout.Close();
  TFile fout2("contain_mtpc_fitted.root","RECREATE");
  fout2.cd();
  mtpcC->Write("mtpc_cont");
  fout2.Close();
}}












