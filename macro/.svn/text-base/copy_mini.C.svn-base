{

  /////////////////////////////////////////////////////////////////////////
  //
  //  Copy example macro. Use root49 to copy a DSPACK mini-DST file 
  //  into an ROOT mini-DST file.
  //
  /////////////////////////////////////////////////////////////////////////

  gROOT->Reset();

  // Filenames
  Char_t *DSFile   = "mini://CERN/NA49/DST/R96B/1470/AUG99/PRO";
  Char_t *ROOTFile = "/shift/na49i04/data1/christoph/test/test_dsmini_1470.root";

  // Create a TRootDS object
  TRootDSmini *RootDSmini = new TRootDSmini();

  // Verbose mode
  RootDSmini->SetVerbose(1);
  // Use normalized dE/dx values
  RootDSmini->SetNormalizedDedx(1);
  // Copy also V0 tracks and vertices
  RootDSmini->SetFillSecondary(1);

  // Copy the file
  printf("Copy File %s\n",DSFile);
  RootDSmini->DS2Tree(DSFile,ROOTFile);

  // Close the files
  RootDSmini->Close();
  delete RootDSmini;

}

