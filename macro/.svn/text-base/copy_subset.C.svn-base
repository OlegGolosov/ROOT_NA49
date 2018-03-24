void copy_subset() {

  ///////////////////////////////////////////////////////////////////////
  //
  // This macro allows to copy a subset of a mini-DST root-tree into
  // a new one.
  //
  ///////////////////////////////////////////////////////////////////////

  if (!gT49) {
    gSystem->Load("libT49DST.so");
    gSystem->Load("libT49ANA.so");
  }

  gROOT->Reset();

  // Name of the old file
  Char_t *cfold = "/shift/na49i04/data1/root49-mdst/newrun1400.0.root";
  // Name of the new file
  Char_t *cfnew = "/shift/na49i04/data1/christoph/test1400.0.root";

  // Get the old file
  TFile *fold = TFile::Open(cfold);

  // Get the old tree
  TTree *told = (TTree *) fold->Get("T49");

  // Set the top branch address
  T49EventRoot *event = new T49EventRoot();
  told->SetBranchAddress("Event",&event);

  // Switch off all branches by default
  told->SetBranchStatus("*",0);

  // Now switch on all those branches that should go into the new tree.
  // The following list contains all branches in T49. So, if some of them
  // should be omitted, simply switch them off by commenting out the
  // corresponding line.

  // The event information
  told->SetBranchStatus("Event"                              ,1);
  told->SetBranchStatus("fNRun"                              ,1);
  told->SetBranchStatus("fNEvent"                            ,1);
  told->SetBranchStatus("fTriggerMask"                       ,1);
  told->SetBranchStatus("fMCFlag"                            ,1);
  told->SetBranchStatus("fDate"                              ,1);
  told->SetBranchStatus("fTime"                              ,1);
  told->SetBranchStatus("fMomentum"                          ,1);
  told->SetBranchStatus("fProjMass"                          ,1);
  told->SetBranchStatus("fTargetMass"                        ,1);
  told->SetBranchStatus("fEveto"                             ,1);
  told->SetBranchStatus("fVertexX"                           ,1);
  told->SetBranchStatus("fVertexY"                           ,1);
  told->SetBranchStatus("fVertexZ"                           ,1);
  told->SetBranchStatus("fBeam"                              ,1);
  told->SetBranchStatus("fRing"                              ,1);
  told->SetBranchStatus("fMonitorVT1"                        ,1);
  told->SetBranchStatus("fMonitorVT2"                        ,1);
  told->SetBranchStatus("fMonitorMTR"                        ,1);
  told->SetBranchStatus("fMonitorMTL"                        ,1);
  told->SetBranchStatus("fUniqueID"                          ,1);
  told->SetBranchStatus("fBits"                              ,1);

  // The list of primary vertices
  told->SetBranchStatus("fVertexList"                        ,1);

  // The list of secondary vertices
  told->SetBranchStatus("fSecVertexList"                     ,1);

  // The data of a primary track
  //told->SetBranchStatus("fPrimaryParticles.fXLocFirst*"      ,1);
  //told->SetBranchStatus("fPrimaryParticles.fYLocFirst*"      ,1);
  //told->SetBranchStatus("fPrimaryParticles.fZLocFirst*"      ,1);
  //told->SetBranchStatus("fPrimaryParticles.fXLocLast*"       ,1);
  //told->SetBranchStatus("fPrimaryParticles.fYLocLast*"       ,1);
  //told->SetBranchStatus("fPrimaryParticles.fZLocLast*"       ,1);
  told->SetBranchStatus("fPrimaryParticles.fIdDet"           ,1);
  told->SetBranchStatus("fPrimaryParticles.fCharge"          ,1);
  told->SetBranchStatus("fPrimaryParticles.fNPoint*"         ,1);
  told->SetBranchStatus("fPrimaryParticles.fNFitPoint*"      ,1);
  told->SetBranchStatus("fPrimaryParticles.fNDedxPoint*"     ,1);
  told->SetBranchStatus("fPrimaryParticles.fNMaxPoint*"      ,1);
  told->SetBranchStatus("fPrimaryParticles.fIflag"           ,1);
  told->SetBranchStatus("fPrimaryParticles.fTmeanCharge*"    ,1);
  told->SetBranchStatus("fPrimaryParticles.fPx"              ,1);
  told->SetBranchStatus("fPrimaryParticles.fPy"              ,1);
  told->SetBranchStatus("fPrimaryParticles.fPz"              ,1);
  told->SetBranchStatus("fPrimaryParticles.fSigPx"           ,1);
  told->SetBranchStatus("fPrimaryParticles.fSigPy"           ,1);
  told->SetBranchStatus("fPrimaryParticles.fSigPz"           ,1);
  told->SetBranchStatus("fPrimaryParticles.fBx"              ,1);
  told->SetBranchStatus("fPrimaryParticles.fBy"              ,1);
  told->SetBranchStatus("fPrimaryParticles.fPchi2"           ,1);
  told->SetBranchStatus("fPrimaryParticles.fXFirst"          ,1);
  told->SetBranchStatus("fPrimaryParticles.fYFirst"          ,1);
  told->SetBranchStatus("fPrimaryParticles.fZFirst"          ,1);
  told->SetBranchStatus("fPrimaryParticles.fXLast"           ,1);
  told->SetBranchStatus("fPrimaryParticles.fYLast"           ,1);
  told->SetBranchStatus("fPrimaryParticles.fZLast"           ,1);
  told->SetBranchStatus("fPrimaryParticles.fWeight"          ,1);
  told->SetBranchStatus("fPrimaryParticles.fRandom"          ,1);
  told->SetBranchStatus("fPrimaryParticles.fLabel"           ,1);
  told->SetBranchStatus("fPrimaryParticles.fPointIndex"      ,1);
  //told->SetBranchStatus("fPrimaryParticles.fTofIflag"        ,1);
  //told->SetBranchStatus("fPrimaryParticles.fTofId"           ,1);
  //told->SetBranchStatus("fPrimaryParticles.fTofX"            ,1);
  //told->SetBranchStatus("fPrimaryParticles.fTofY"            ,1);
  //told->SetBranchStatus("fPrimaryParticles.fTofPathl"        ,1);
  //told->SetBranchStatus("fPrimaryParticles.fTofCharge"       ,1);
  //told->SetBranchStatus("fPrimaryParticles.fTofMass2"        ,1);
  //told->SetBranchStatus("fPrimaryParticles.fTofSigMass2"     ,1);
  told->SetBranchStatus("fPrimaryParticles.fUniqueID"        ,1);
  told->SetBranchStatus("fPrimaryParticles.fBits"            ,1);

  // The data of a secondary track
  //told->SetBranchStatus("fSecondaryParticles.fXLocFirst*"    ,1);
  //told->SetBranchStatus("fSecondaryParticles.fYLocFirst*"    ,1);
  //told->SetBranchStatus("fSecondaryParticles.fZLocFirst*"    ,1);
  //told->SetBranchStatus("fSecondaryParticles.fXLocLast*"     ,1);
  //told->SetBranchStatus("fSecondaryParticles.fYLocLast*"     ,1);
  //told->SetBranchStatus("fSecondaryParticles.fZLocLast*"     ,1);
  told->SetBranchStatus("fSecondaryParticles.fIdDet"         ,1);
  told->SetBranchStatus("fSecondaryParticles.fCharge"        ,1);
  told->SetBranchStatus("fSecondaryParticles.fNPoint*"       ,1);
  told->SetBranchStatus("fSecondaryParticles.fNFitPoint*"    ,1);
  told->SetBranchStatus("fSecondaryParticles.fNDedxPoint*"   ,1);
  told->SetBranchStatus("fSecondaryParticles.fNMaxPoint*"    ,1);
  told->SetBranchStatus("fSecondaryParticles.fIflag"         ,1);
  told->SetBranchStatus("fSecondaryParticles.fTmeanCharge*"  ,1);
  told->SetBranchStatus("fSecondaryParticles.fPx"            ,1);
  told->SetBranchStatus("fSecondaryParticles.fPy"            ,1);
  told->SetBranchStatus("fSecondaryParticles.fPz"            ,1);
  told->SetBranchStatus("fSecondaryParticles.fSigPx"         ,1);
  told->SetBranchStatus("fSecondaryParticles.fSigPy"         ,1);
  told->SetBranchStatus("fSecondaryParticles.fSigPz"         ,1);
  told->SetBranchStatus("fSecondaryParticles.fBx"            ,1);
  told->SetBranchStatus("fSecondaryParticles.fBy"            ,1);
  told->SetBranchStatus("fSecondaryParticles.fPchi2"         ,1);
  told->SetBranchStatus("fSecondaryParticles.fXFirst"        ,1);
  told->SetBranchStatus("fSecondaryParticles.fYFirst"        ,1);
  told->SetBranchStatus("fSecondaryParticles.fZFirst"        ,1);
  told->SetBranchStatus("fSecondaryParticles.fXLast"         ,1);
  told->SetBranchStatus("fSecondaryParticles.fYLast"         ,1);
  told->SetBranchStatus("fSecondaryParticles.fZLast"         ,1);
  told->SetBranchStatus("fSecondaryParticles.fWeight"        ,1);
  told->SetBranchStatus("fSecondaryParticles.fRandom"        ,1);
  told->SetBranchStatus("fSecondaryParticles.fLabel"         ,1);
  told->SetBranchStatus("fSecondaryParticles.fPointIndex"    ,1);
  //told->SetBranchStatus("fSecondaryParticles.fTofIflag"      ,1);
  //told->SetBranchStatus("fSecondaryParticles.fTofId"         ,1);
  //told->SetBranchStatus("fSecondaryParticles.fTofX"          ,1);
  //told->SetBranchStatus("fSecondaryParticles.fTofY"          ,1);
  //told->SetBranchStatus("fSecondaryParticles.fTofPathl"      ,1);
  //told->SetBranchStatus("fSecondaryParticles.fTofCharge"     ,1);
  //told->SetBranchStatus("fSecondaryParticles.fTofMass2"      ,1);
  //told->SetBranchStatus("fSecondaryParticles.fTofSigMass2"   ,1);
  told->SetBranchStatus("fSecondaryParticles.fUniqueID"      ,1);
  told->SetBranchStatus("fSecondaryParticles.fBits"          ,1);

  // Create a new file 
  TFile *fnew = TFile::Open(cfnew, "RECREATE");

  // Create a clone of the old tree in the new file
  TTree *tnew = told->CloneTree();

  // Write out the new tree
  tnew->Print();
  tnew->Write();

  delete fold;
  delete fnew;

}
