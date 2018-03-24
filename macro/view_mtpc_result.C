{{
//  Macro to inspect the result of the mtpc fits. Uses a filled
//  T49Conatiner for histograms and the fit-result.

  gROOT->Reset();

  // Open data file

  TFile f("contain_mtpc_fitted.root");
  T49Container *mtpcC=(T49Container*) f->Get("mtpc_cont");

  TH1F* tmpH;
  TF1* tmpF;
  TAxis* PtotAxis;
  T49DedxInfo* Info;
  T49Index I;

  // Initialise array to hold fits to each particle species
 
  Char_t name[255];
  TObjArray Funcs(4);
  for (Int_t p=0;p<4;p++) {
    sprintf(name,"func%d",p);
    Funcs[p]=new TF1(name,"gaus");
    ((TF1*) Funcs[p])->SetLineColor(p+1);
  }
  TF1 SumFunc("sumf","gaus(0)+gaus(3)+gaus(6)+gaus(9)",0.5,2);

  Int_t dum;
  Int_t plotnr=0;
  Int_t X[4]={1,12,2,4};
  Int_t Inp=1;
  Float_t ptot;
  Float_t npr,npi,nka,nel,nall;
  TCanvas c1("c1","Canvas #1",500,600);
  
  // Program loops until user enters a negative number

  while (Inp>=0) {

    // Set index fields

    I.SetChargeBin(X[0]);
    I.SetPtotBin(X[1]);
    I.SetPtBin(X[2]);
    I.SetPhiBin(X[3]);

    // Retrieve Histo
    tmpH = mtpcC.GetHist(&I);
    tmpH->Draw();
    c1.Update();
    mtpcC->FindPhase(&I);
    printf("mtpc: charge %d, ptot %f, pt %f, phi %f \n",
	   I.GetCharge(),I.GetPtot(), I.GetPt(), I.GetPhi());
    Int_t Integr = tmpH->Integral();
    printf("mtpc: number of entries : %d \n",Integr);
    if (tmpH->Integral()>1000) {
      ptot = I.GetPtot();
      Info = mtpcC->GetInfo(&I);
      printf("Pipos: %5.2f, rel. res. %7.3e \n",Info->GetPosition(1),
	     Info->GetReso());
      for (Int_t p=0;p<4;p++){
	tmpF=(TF1*) Funcs[p];
	tmpF->SetRange(0.5,2.0);
	tmpF->SetParameter(0,Info->GetAmplitude(p));
	SumFunc->SetParameter(3*p,Info->GetAmplitude(p));
	Float_t pos=Info->GetPosition(p);
	tmpF->SetParameter(1,pos);
	SumFunc->SetParameter(3*p+1,pos);
	Float_t res=Info->GetReso();
	res=res*pos;
	tmpF->SetParameter(2,res);
	SumFunc->SetParameter(3*p+2,res);
	tmpF->Draw("same");
      }
      SumFunc->Draw("same");
      c1.Update();
    }

    // Ask user for new bin

    printf("Charge: %d, Ptot: %d, Pt: %d, Phi: %d \n",X[0],X[1],X[2],X[3]);
    printf("Change (index num): ");
    scanf("%d",&Inp);
    if (Inp>=0) {
      printf("Old value of X[%d]: %f, enter new value ",Inp,X[Inp] );
      scanf("%d",&dum);
      X[Inp]=dum;
    }
  }
}}






