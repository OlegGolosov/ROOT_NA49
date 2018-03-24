{

  /////////////////////////////////////////////////////////////////////////
  //
  //  Copy example macro. Use root49 to copy a DSPACK file into an
  //  ROOT mini-DST file.
  //
  /////////////////////////////////////////////////////////////////////////

  gROOT->Reset();

  // Filenames
  Char_t *DSFile   = "//CERN/NA49/DST/R96B/1464/AUG99/PRO/R1464_01-01.DS";
  Char_t *ROOTFile = "/shift/na49i04/data1/christoph/test/test_1464.root";

  // Create a TRootDS object
  TRootDS *RootDS = new TRootDS();

  // Verbose mode
  RootDS->SetVerbose(1);
  // Use normalized dE/dx values
  RootDS->SetNormalizedDedx(1);
  // Copy also V0 tracks and vertices
  RootDS->SetFillSecondary(1);

  // Copy the file
  printf("Copy File %s\n",DSFile);
  RootDS->DS2Tree(DSFile,ROOTFile);

  // Close the files
  RootDS->Close();
  delete RootDS;

}

