{{
// Macro to fit a single bin of an already filled container. Allows
// user to see the result of the initialisation of the
// T49DedxFunction, as well as the fit result.

  gROOT->Reset();

  // Open input file

  TFile fin("contain_mtpc.root");
  T49Container* mtpc_cont = (T49Container*) fin.Get("mtpc_cont");

  // Declare functions. Four gaussians are needed; one for each
  // particle species.

  T49DedxFunction *DedxFunc = new T49DedxFunction("fitfunc");
  Char_t name[256];
  TF1* tmpF;
  TObjArray Funcs(4);
  for (Int_t p=0;p<4;p++) {
    sprintf(name,"func%d",p);
    tmpF=new TF1(name,"gaus");
    Funcs[p]=tmpF;
    tmpF->SetLineColor(p+1);
    tmpF->SetRange(0.5,2.0);
  }

  TCanvas *c1 = new TCanvas("c1","Canvas #1",10,10,400,500);
  TPad *pad1 = new TPad("pad1","top pad",0,0.2,1,1);
  TPad *pad2 = new TPad("pad2","bottom pad",0,0,1,0.2);
  pad1.Draw();
  pad2.Draw();
  TCanvas *c2=new TCanvas("c2","Canvas #2",450,10,400,400);
  TCanvas *c3=new TCanvas("c3","Canvas #3",450,50,400,400);

  TF1 p1("p1","pol2");
  TF1 g1("g1","[0]*exp(-0.5*(x-[1])*(x-[1])/[2])");
  TF1 g2("g2","[0]*exp(-0.5*(x-[1])*(x-[1])/[2])");
  TF1 *g3=0;
  TH1F *h2 = new TH1F("h2","scaled differences",150,0.5,2);
  TH1F *h3 = new TH1F("h3","differences",150,0.5,2);
  TH1F *h4 = new TH1F("h4","smoothed",150,0.5,2);
  TH1F *subH = new TH1F("subH","after pion subtraction",150,0.5,2);

  T49Index I;
  Int_t Bin=0;
  printf("Please enter ChargeBin: ");
  scanf("%d",&Bin);
  I.SetChargeBin(Bin);
  printf("Please enter PtotBin: ");
  scanf("%d",&Bin);
  I.SetPtotBin(Bin);
  printf("Please enter PtBin: ");
  scanf("%d",&Bin);
  I.SetPtBin(Bin);
  printf("Please enter PhiBin: ");
  scanf("%d",&Bin);
  I.SetPhiBin(Bin);

  TH1F *tmpH = mtpc_cont->GetHist(&I);
  Float_t Ptot=0;
  pad1->cd();
  gStyle->SetOptStat(1);
  tmpH->Draw("hist");
  c1->Update();
  gStyle->SetOptStat(0);
  if (tmpH->GetEntries()>1000) {
    Float_t run_sum=0;
 
   // Create and draw smoothed histogram, as used internally in
   // T49DedxFunction::Init()

    for (Int_t i=3;i<149;i++) {
      Float_t smooth=(tmpH->GetBinContent(i-1)+tmpH->GetBinContent(i)+
		      tmpH->GetBinContent(i+1))/3;
      h4->SetBinContent(i,smooth);
    }
    c2->cd();
    h4->SetMarkerStyle(2);
    h4->Draw("");
    c2->Update();
    pad1->cd();     
    TAxis *xax = tmpH->GetXaxis();
    mtpc_cont->FindPhase(&I);
    Float_t ptot = I.GetPtot();

    // Initialise fitfunction

    DedxFunc->SetPtot(ptot);
    DedxFunc->Init(tmpH,"V");
    for (Int_t p=0;p<4;p++){
      tmpF=(TF1*) Funcs[p];
      tmpF->SetParameter(0,DedxFunc->GetAmplitude(p));
      Float_t pos=DedxFunc->GetPosition(p);
      tmpF->SetParameter(1,pos);
      Float_t res=pos*DedxFunc->GetResolution();
      tmpF->SetParameter(2,res);
      printf("particletype %d, pos %f, amp %f, res %f, posbb %f \n",
	     i,pos,DedxFunc->GetAmplitude(p),res,DedxFunc->GetRelRise(p));
      tmpF->Draw("same");
    }
    DedxFunc->Draw("same");
    c1->Update();

    // Fill and draw histogram with subtracted pion peak, as used in
    // T49DedxFunction::Init().

    c3->cd();
    for (Int_t j=1;j<151;j++) {
      tmpF=(TF1*) Funcs[1];
      subH->SetBinContent(j,tmpH->GetBinContent(j)-tmpF->Eval(tmpH->GetBinCenter(j)));
    }
    subH->Draw();
    c3->Update();
    printf("press <CR> \n");
    Char_t s[255];
    gets(&s);
    gets(&s);

    // Fit function, using fixed relative electron, proton and kaon positions 

    tmpH->Fit("fitfunc","NR");
    pad1.cd();
    for (Int_t p=0;p<4;p++){
      tmpF=(TF1*) Funcs[p];
      tmpF->SetParameter(0,DedxFunc->GetAmplitude(p));
      tmpF->SetParameter(1,DedxFunc->GetPosition(p));
      tmpF->SetParameter(2,DedxFunc->GetPosition(p)*
			 DedxFunc->GetResolution());
      tmpF->Draw("same");
    }
    DedxFunc->Draw("same");

    // Calculate and plot the scaled (to standard deviation=sqrt(N))
    // difference between measurement and fit.

    c1->Update();
    Float_t err=0,diff=0;
    for (Int_t i=1;i<=xax->GetNbins();i++) {
      err = sqrt(tmpH->GetBinContent(i));
      tmpH->SetBinError(i,err);
      diff = tmpH->GetBinContent(i)-DedxFunc->Eval(xax->GetBinCenter(i));
      h3->SetBinContent(i,diff);
      if (err != 0) h2->SetBinContent(i,diff/err);
    }
    pad2.cd();
    h2->SetMinimum(-3);
    h2->SetMaximum(3);
    h2->Draw();
    c1->Update();
    printf("press <CR> \n");
    Char_t s[255];
    gets(&s);

    // Fit again, using free relative positions

    DedxFunc->SetFree();
    tmpH->Fit("fitfunc","NR");
    pad1.cd();
    for (Int_t p=0;p<4;p++){
      tmpF=(TF1*) Funcs[p];
      tmpF->SetParameter(0,DedxFunc->GetAmplitude(p));
      tmpF->SetParameter(1,DedxFunc->GetPosition(p));
      tmpF->SetParameter(2,DedxFunc->GetPosition(p)*
			 DedxFunc->GetResolution());
      tmpF->Draw("same");
    }
    DedxFunc->Draw("same");
    for (Int_t i=1;i<=xax->GetNbins();i++) {
      err = sqrt(tmpH->GetBinContent(i));
      tmpH->SetBinError(i,err);
      diff = tmpH->GetBinContent(i)-DedxFunc->Eval(xax->GetBinCenter(i));
      h3->SetBinContent(i,diff);
      if (err != 0) h2->SetBinContent(i,diff/err);
    }
    pad2.cd();
    h2->Draw();
    c1->Update();
  }
}}










