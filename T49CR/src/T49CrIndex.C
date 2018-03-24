#include "stdlib.h"
#include "TObject.h"
#include "TRandom.h"
#include "T49CrIndex.h"


#include "math.h"

ClassImp(T49CrIndex)

T49CrIndex::~T49CrIndex()
{
}

T49CrIndex::T49CrIndex()
{ 
  fCharge = 0;
  fPtot = 0;    
  fPt = 0;    
  fPhi = 0;

  fChargeBin = 0;
  fPtotBin = 0;    
  fPtBin = 0;    
  fPhiBin = 0;
}
