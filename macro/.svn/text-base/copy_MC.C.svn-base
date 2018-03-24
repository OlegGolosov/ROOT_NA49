void copy_MC(const Char_t *DSFile   = "/shift/shd09/data17/vp/users/cblume/160gev/embed_1458_pi-_10_100.ds",
             const Char_t *ROOTFile = "/shift/na49i04/data1/christoph/test/test_1458.root") {


  /////////////////////////////////////////////////////////////////////////
  //
  //  Copy example macro for MC data. Use root49 to copy a DSPACK file 
  //  into an ROOT mini-DST file.
  //
  /////////////////////////////////////////////////////////////////////////

  gROOT->Reset();

  // Create a TrootDSMC obkect
  TRootDSMC *RootDSMC = new TRootDSMC();

  // Verbose mode
  RootDSMC->SetVerbose(1);
  // Use normalized dE/dx values
  RootDSMC->SetNormalizedDedx(1);
  // Copy also V0 tracks and vertices
  RootDSMC->SetFillSecondary(1);
  // Copy the also the MC data
  RootDSMC->SetFillMCTracks(1);

  // Copy the MC run into a root tree
  printf("Copy File %s\n",DSFile);
  RootDSMC->DS2Tree(DSFile,ROOTFile);

  // Close the files
  RootDSMC->Close();
  delete RootDSMC;

}

