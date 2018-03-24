{
// EXAMPLE MACRO FOR T49Mixer //

gROOT->Reset();

  // Load libraries if root49 is not used
  if (gClassTable->GetID("T49Run") < 0) {
    printf("Load shared libraries for T49\n");
    gSystem->Load("libT49DST.so");
    gSystem->Load("libT49ANA.so");
  }

T49Mixer MX;           // Default 1 Pool, 100 Objects
TObject  Obj[100];     // 100 = (0..99) TObjetcs
TObject *PT,*PT1,*PT2; // Pointers to TObject

for(int i=0;i<100;i++) Obj[i]->SetUniqueID(i); // Set Obj. ID

for(int i=0;i<100;i++) {              // Fill MX Pool  
  PT = &Obj[i];                       // To be clear
  MX.PutOne(PT);
}
cout << "N of Objects in MX: " << MX.NofObjects()     << endl;
cout << "N of Object  Pairs: " << MX.NofPairs()       << endl;
cout << "N of Unique  Pairs: " << MX.NofUniquePairs() << endl;

cout << "\nPress Enter for Random Objects:" << flush;
Char_t  InC[10]; cin.getline(InC,5);

while(MX.NofFree()) {
 PT = MX.GetNuROne();               // Get Not-used-random Objects
 cout << PT->GetUniqueID() << " " << flush;
} 

cout << "\nPress Enter for Random Pairs:" << flush;
cin.getline(InC,5);
while(MX.NofFreePairs()) {          // to get random not used pairs
 MX.GetNuRPair(&PT1,&PT2); 	    // "&" since PT1/2 changed
 cout << PT1->GetUniqueID() << "-" 
      << PT2->GetUniqueID() << " " << flush; 
}

cout << "\nPress Enter for Random Unique Pairs:" << flush;
cin.getline(InC,5);

while(MX.NofFreeUniquePairs()) {    // to get random uniquepairs
 MX.GetRUniquePair(&PT1,&PT2);      // "&" since PT1/2 changed
 cout << PT1->GetUniqueID() << "-" 
      << PT2->GetUniqueID() << " " << flush;
}
cout << "\n\nThat's it.  Have FUN with T49MiXeR !" << endl;
MX.Show();
}
