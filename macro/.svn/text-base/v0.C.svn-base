#ifndef __CINT__

// C++ 
#include <stdio.h>
#include <math.h>

// ROOT
#include <TROOT.h>
#include <TSystem.h>
#include <TClassTable.h>
#include <TFile.h>
#include <TObject.h>
#include <TObjArray.h>
#include <TClonesArray.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>

// T49DST
#include <T49Run.h>
#include <T49EventRoot.h>
#include <T49VertexRoot.h>
#include <T49ParticleRoot.h>

// T49ANA
#include <T49CutV0.h>
#include <T49Histogram2D.H>

//extern void InitGui();
//VoidFuncPtr_t initfuncs[] = {InitGui, 0};
TROOT root("Rint","Root class");

int main (int argc, char **argv)
#else
void v0()
#endif
{

#ifdef __CINT__

  // Reset the ROOT environment
  gROOT.Reset();

  // Load libraries if root49 is not used
  if (gClassTable->GetID("T49Run") < 0) {
    printf("Load shared libraries for T49\n");
    gSystem->Load("libT49DST.so");
    gSystem->Load("libT49ANA.so");
  }

#endif

  T49EventRoot     *Event;
  T49VertexRoot    *V0;
  TObjArray        *SecVertexList;
  TObjArray        *V0List;

  T49CutV0         *V0Cut = new T49CutV0("cuts","V0 cuts");

  // Define the T49Run object
  T49Run           *Run;
  if (gT49) {
    Run = gT49;
  }
  else
    Run = new T49Run();        

  // Read only reduced particle information 
  Run->SetSmallParticleOnly();

  // maximum number of events to process
  const Int_t       maxEvent = 1000;

  TH2F *hArmenteros = new TH2F("hArmenteros","Armenteros Plot"
                                            ,60,-1.1,1.1,25,0.0,0.25);
  TH1F *hCosThe     = new TH1F("hCosThe"    ,"Cos(theta) lambda",100,-1.5,1.5);
  TH1F *hLifetime   = new TH1F("hLifetime"  ,"Lifetime lambda (10^-10! s)",100,0,10);
  TH1F *hPosPt      = new TH1F("hPosPt"     ,"positive daughter pt (non-vtx)" ,50,0,5);
  TH1F *hNegPt      = new TH1F("hNegPt"     ,"negative daughter pt (non-vtx)" ,50,0,5);

  // The minv-distributions for different y-pt-bins
  T49Histogram2D *Lambda = new T49Histogram2D("Lambda","Lambda Minv vs y-pt");
  Lambda->SetHistogram("Minv","M?inv! Lambda",120,1.08,1.20);
  Lambda->SetDimensions(4,0.9,4.9,4,0.0,2.0);
  Lambda->SetXYTitle("M?inv! (GeV)","Entries");
  Lambda->CreateHistograms();

  // Define the V0-cuts
  V0Cut->SetStandardCutsGSI();
  V0Cut->SetVerbose(0);

  // Loop through the events
  Int_t iEvent = 0; 
  while ((Event = (T49EventRoot *) Run->GetNextEvent("n-std+-00B")) && 
         (iEvent < maxEvent)) {        

    // Get the list with the secondary vertices 
    SecVertexList = Event->GetSecondaryVertices();

    cout << " Run "         << Event->GetNRun()
         << " event "       << Event->GetNEvent()
         << " with "        << SecVertexList->GetEntries()
         << " V0-vertices " << endl;

    // Get the list of accepted V0s
    V0List = V0Cut->GetAcceptedV0s(SecVertexList);

    // Loop through all accepted V0s
    for (Int_t idxV0 = 0; idxV0 < V0List->GetEntries(); idxV0++) {

      V0 = (T49VertexRoot *) V0List->At(idxV0);

      // Fill histograms
      hArmenteros->Fill(V0->GetArmenterosAlpha(),V0->GetArmenterosPt());
      hCosThe->Fill(V0->GetCosThetaLambda());
      hLifetime->Fill(V0->GetLifetimeLambda());

      // The daughter tracks
      hPosPt->Fill(V0->GetPositiveTrack()->GetPt());
      hNegPt->Fill(V0->GetNegativeTrack()->GetPt());

      // Fill the minv-distributions
      Lambda->Fill(V0->GetRapidityLambda()
                  ,V0->GetPt()
                  ,V0->GetInvariantMassLambda());

    }

    // Count the number of events
    iEvent++;

  }

  cout << endl << " Analyzed " << iEvent << " events" << endl;

  // Print the cut statistics
  V0Cut->PrintCutList();
  V0Cut->PrintStatistics();

  // Print the definition of the minv-spectra
  Lambda->Print();

  // Draw the min-spectra
  Lambda->Draw();

  // Draw the other histograms 
  TCanvas *c1 = new TCanvas("c1","V0Example",10,10,600,900);
  c1->Divide(1,3);

  c1->cd(1);
  hArmenteros->SetXTitle("Alpha");
  hArmenteros->SetYTitle("Pt (GeV/c)");
  hArmenteros->Draw();

  c1->cd(2);
  gPad->Divide(2,1);
  gPad->cd(1);
  gPad->SetLogy();
  hPosPt->SetLineColor(38);
  hPosPt->SetXTitle("pt (GeV/c)");
  hPosPt->Draw();

  c1->cd(2);
  gPad->cd(2);
  gPad->SetLogy();
  hNegPt->SetLineColor(38);
  hNegPt->SetXTitle("pt (GeV/c)");
  hNegPt->Draw();

  c1->cd(3);
  gPad->Divide(2,1);
  gPad->cd(1);
  hCosThe->SetLineColor(46);
  hCosThe->SetXTitle("cos(theta)");
  hCosThe->Draw();

  c1->cd(3);
  gPad->cd(2);
  gPad->SetLogy();
  hLifetime->SetLineColor(46);
  hLifetime->SetXTitle("lifetime (10^10! s)");
  hLifetime->Draw();

  // Save the V0Minv-class
  TFile *fout = TFile::Open("lambda.root", "RECREATE");
  Lambda->WriteHistograms();
  fout->Close();

#ifndef __CINT__
  return 0;
#endif

}
