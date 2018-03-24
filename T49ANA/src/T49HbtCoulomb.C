/***************************************************************************
 * Author: Dominik Flierl, flierl@ikf.uni-frankfurt.de
 ***************************************************************************/

using namespace std ;

#include <iostream>
#include <cstdlib>
#include "T49HbtCoulomb.h"
#include "TMath.h"

ClassImp(T49HbtCoulomb)

//--------------------------------
T49HbtCoulomb::T49HbtCoulomb()
{
  // constants of nature
  f_hbc = 0.197327 ; // [GeV fm]
  fBohrRadius = 388.0 ; // [fm] for like sign pions only !
  fBohrRadiusUSP = (-1.0) * fBohrRadius ;  // [fm] for UNlike sign pions !

  // constants used in the paper
  f_d2 = 3*TMath::Pi()/8.0 ; // d2 in equation 8 in the paper

  // default MeanSeparation = 10 fm 
  fMeanSeparation = 10.0 ;
  CalculateConstants() ;
  CalculateConstantsUSP() ;
}
//--------------------------------
T49HbtCoulomb::~T49HbtCoulomb()
{
  // empty
}
//--------------------------------
void T49HbtCoulomb::SetMeanSeparation(Double_t MeanSeparation)
{
  fMeanSeparation = MeanSeparation ;
  CalculateConstants() ;
  CalculateConstantsUSP() ;
}
//--------------------------------
void T49HbtCoulomb::CalculateConstants()
{
  // in equation 8 in the paper
  fConstant1 = 1 + (2*fMeanSeparation/fBohrRadius) * ( 1+f_d2*fMeanSeparation/fBohrRadius) ;
  
  // equation 9 in 'the paper (attention k = q/2)
  fCriticalMomentum = f_hbc * 2.0 * TMath::Pi()/(4.0*fMeanSeparation) * fConstant1 ; // crossover momentum in [GeV/c]
  fCriticalMomentum2 = fCriticalMomentum * fCriticalMomentum ; // square it

  // equation 10 in the paper
  fConstant2 = fBohrRadius * fMeanSeparation * fCriticalMomentum2 * ( 1.0 - Ac(fCriticalMomentum)  * fConstant1 ) / ( f_hbc * f_hbc );
}
//--------------------------------
Double_t T49HbtCoulomb::Ac(Double_t qinv)
{
  // The gamov function 
  // in the paper: 2 pi eta = (2 * pi) / (k*a) = (2 * pi) / ( a * ( qinv/2 ) ) = 4*pi*hbc/(a*qinv)
  Double_t TwoPiEta =  4.0*TMath::Pi()*f_hbc/(fBohrRadius*qinv) ;
  return TwoPiEta/(exp(TwoPiEta)-1.0) ;
}
//--------------------------------
Double_t T49HbtCoulomb::Weight(Double_t qinv)
{
  // depending on qinv use quantum mechanical or classical solution: equation 8 in the paper
  // qinv = 2 * k : k is used in the paper, here we use qinv 
   if(qinv<=fCriticalMomentum)
    {
      return (Ac(qinv) * fConstant1) ;
    }
  else
    {
      return (1.0 - fConstant2 / (fBohrRadius * fMeanSeparation * pow(qinv,2) / ( f_hbc * f_hbc ) ) ) ;
    }
}
///////////////////////////////////////////////////////////////////////////////
// Unlike sign pairs
///////////////////////////////////////////////////////////////////////////////
Double_t T49HbtCoulomb::WeightUnlikeSignPairs(Double_t qinv)
{
  // depending on qinv use quantum mechanical or classical solution: equation 8 in the paper
  // now for unlike sign pairs (USP), i.e. : negative Bohrradius ! (see page 250 in the paper)
  // qinv = 2 * k : k is used in the paper, here we use qinv 
   if(qinv <= fCriticalMomentumUSP)
    {
      return (AcUSP(qinv) * fConstant1USP) ;
    }
  else
    {
      return (1.0 - (fConstant2USP * f_hbc * f_hbc) / (fBohrRadiusUSP * fMeanSeparation * pow(qinv,2))) ;
    }
}
//--------------------------------
Double_t T49HbtCoulomb::AcUSP(Double_t qinv)
{
  // The gamov function, now for unlike sign pairs
  // in the paper: 2 pi eta = (2 * pi) / (k*a) = (2 * pi) / ( a * ( qinv/2 ) ) = 4*pi*hbc/(a*qinv)
  Double_t TwoPiEta =  4.0*TMath::Pi()*f_hbc/(fBohrRadiusUSP*qinv) ;
  return TwoPiEta/(exp(TwoPiEta)-1.0) ;
}
//--------------------------------
void T49HbtCoulomb::CalculateConstantsUSP()
{
  // equation 8 in the paper for unlike sign pairs 
  fConstant1USP = 1 + (2*fMeanSeparation/fBohrRadiusUSP) * ( 1+f_d2*fMeanSeparation/fBohrRadiusUSP) ;
  
  // equation 9 in 'the paper (attention k = q/2)
  fCriticalMomentumUSP = f_hbc * 2.0 * TMath::Pi()/(4.0*fMeanSeparation) * fConstant1USP ; // crossover momentum in [GeV/c]
  fCriticalMomentumUSP2 = fCriticalMomentumUSP * fCriticalMomentumUSP ; // square it
  //cout << "critical momentum : " << fCriticalMomentumUSP << endl ;

  // equation 10 in the paper for unlike sign pairs
  fConstant2USP = fBohrRadiusUSP * fMeanSeparation * fCriticalMomentumUSP2 * ( 1.0 - AcUSP(fCriticalMomentumUSP)  * fConstant1USP ) / ( f_hbc * f_hbc );
}
////////////////////////////////////////////////////////////////////////////////
// Classical approximation according to PBM in Nucl. Phys. A610 (1996) 286c-296c
/////////////////////////////////////////////////////////////////////////////////
Double_t T49HbtCoulomb::ClassicalWeight(Double_t qinv)
{
  // the coulomb correction derived in a simple toy model derived in
  // G. Baym & PBM, Nucl. Phys. A610 (1996) 286c-296c
  // electron charge in natural units =  0.0854245
  // reduced mass m1*m2+(m1+m2) with pion mass (m1=m2=0.139 GeV) m(reduced) = 0.069 [GeV]
  // fMeanSeparation must be positive !
  Double_t arg =  1.0 - 2.0 * 0.069785 * 0.0854245 * 0.0854245 * f_hbc / ( fabs(fMeanSeparation) * qinv * qinv ) ;
  if(arg<0.0) 
    {
      cout << "In T49HbtCoulomb::ClassicalWeight(Double_t qinv)                                      qinv = " << qinv << " too small !" << endl ;
      return 0.1 ;
    }
  //cout << "hat geklapt" << endl;
  Double_t weight = TMath::Sqrt(arg) ;
  return weight ;
}
//-----------------------------------
Double_t T49HbtCoulomb::ClassicalWeightUnlikeSignPairs(Double_t qinv)
{
  // the coulomb correction derived in a simple toy model derived in
  // G. Baym & PBM, Nucl. Phys. A610 (1996) 286c-296c 
  // electron charge in natural units =  0.0854245
  // reduced mass m1*m2+(m1+m2) with pion mass (m1=m2=0.139 GeV) m(reduced) = 0.069 [GeV]
  // fMeanSeparation must be positive !
  Double_t arg =  1.0 + 2.0 * 0.069785 * 0.0854245 * 0.0854245 * f_hbc / ( fabs(fMeanSeparation) * qinv * qinv ) ;
  Double_t weight = TMath::Sqrt(arg) ;
  
  return weight ;
}
