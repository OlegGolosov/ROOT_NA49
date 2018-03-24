///////////////////////////////////////////////////////////////////////
//
//  Contains the veto calorimeter information
//
///////////////////////////////////////////////////////////////////////

/*
$Log: T49VetoRoot.C,v $
Revision 1.2  2002/03/27 14:03:00  cblume
Include GetEveto() with calibration for 40GeV min bias

Revision 1.1  2002/02/04 16:06:54  cblume
Add T49VetoRoot.C

*/

#include "TObject.h"
#include "TClass.h"

#include "T49VetoRoot.h"

ClassImp(T49VetoRoot)

//______________________________________________________________________________
T49VetoRoot::T49VetoRoot() 
{
  //
  // T49VetoRoot constructor
  //

  fVersion = 0;
  for (Int_t iVetoADC = 0; iVetoADC < kVetoADC; iVetoADC++) {
    fADCphoton[iVetoADC] = 0;
    fADChadron[iVetoADC] = 0;
  }

}

//______________________________________________________________________________
Float_t T49VetoRoot::GetEveto()
{
  //
  // Returns the calibrated veto energy.
  // Only available for the 40GeV minimum bias data.
  //

  Float_t phveto = 0;
  Float_t hdveto = 0;

  for (Int_t iVetoADC = 0; iVetoADC < kVetoADC; iVetoADC++) {
    phveto += fADCphoton[iVetoADC];
    hdveto += fADChadron[iVetoADC];
  }

  return (1.4*phveto + 2.0*hdveto) * 1.4675;

}
