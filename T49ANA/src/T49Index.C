#include "T49Index.h"

/*
$Log: T49Index.C,v $
 * Revision 1.1  2000/01/17  12:38:13  cblume
 * Add T49Index.C
 *
*/
 
ClassImp(T49Index)

T49Index::~T49Index()
{
}

T49Index::T49Index()
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
