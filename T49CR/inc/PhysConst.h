#ifndef _PhysConst_INCLUDED_
#define _PhysConst_INCLUDED_

/*
$Log: PhysConst.h,v $
 * Revision 1.3  1999/11/23  13:52:07  cblume
 * Remove files
 *
*/

const Float_t kNUCLMASS     = 0.939;
const Float_t kNUCLMASS2    = 0.881721;
const Float_t kPMASS        = 0.938;
const Float_t kKMASS        = 0.493;
const Float_t kPIMASS       = 0.139567;
const Float_t kPIMASS2      = 0.0194769936;
const Float_t kEMASS        = 0.000511;
const Float_t kLAMMASS      = 1.115683;
const Float_t kK0SMASS      = 0.497672;
const Int_t   kNPart        = 6;                                           // Number of particles
const Float_t kMASS[kNPart] = {0.0, kPMASS, kKMASS, kPIMASS, kEMASS, 0.0}; // particle masses

const Float_t kTLAB         = 160.0;
const Float_t kAP           = 208.0;
const Float_t kAT           = 208.0;

const Float_t kStdBeta      = 0.995;
const Float_t kStdGamma     = 9.8;
const Float_t kPi           = 3.14159;

const Int_t   kVT1          = 2;        
const Int_t   kVT2          = 4;        
const Int_t   kMTR          = 8;        
const Int_t   kMTL          = 16;        

#endif
