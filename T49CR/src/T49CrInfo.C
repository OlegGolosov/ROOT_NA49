#include "stdlib.h"
#include "TObject.h"
#include "TRandom.h"
#include "T49CrInfo.h"


#include "math.h"

ClassImp(T49CrInfo)

T49CrInfo::~T49CrInfo()
{
}

T49CrInfo::T49CrInfo()
{ 
  for(Int_t i = 0;i<4;i++)
    {
      fPosition[i]  = 0.0;    
      fAmplitude[i] = 0.0;      
    }

  fReso   = 0.0;
  fChisq  = 0.0;
}
