/********************************************************************************************************
 * Author: Dominik Flierl, flierl@ikf.uni-frankfurt.de
 ********************************************************************************************************
 * This class returns the coulomb weight for a pair with a given momentum difference qinv
 * Analytical formula are based on Phys. Lett. B 432(1998)248-257 and Nucl. Phys. A610 (1996) 286c-296c
 ********************************************************************************************************/

#ifndef T49HbtCoulomb_hh
#define T49HbtCoulomb_hh

#include "TObject.h"
class T49HbtCoulomb: public TObject
{
  public :
    // Constructor & destructor
    T49HbtCoulomb() ;
    ~T49HbtCoulomb() ;
    
    // Like sign specific member functions
    // return the coulomb weight factor according to equation 8 
    // in Sinyukov et al., Phys. Lett. B 432(1998)248-257 for like sign pairs
    Double_t Weight(Double_t qinv) ;
    Double_t Ac(Double_t qinv) ;   // gamov function
    void   CalculateConstants() ;  // init constants
    Double_t GetConstant1()        { return fConstant1 ; } ;
    Double_t GetCriticalMomentum() { return fCriticalMomentum ; } ;

    // UNlike sign specific member functions
    // return the coulomb weight factor according to equation 8 
    Double_t WeightUnlikeSignPairs(Double_t qinv) ;
    Double_t AcUSP(Double_t qinv) ;   // gamov function
    void   CalculateConstantsUSP() ;  // init constants
    Double_t GetConstant1USP()        { return fConstant1USP ; } ;
    Double_t GetCriticalMomentumUSP() { return fCriticalMomentumUSP ; } ;

    // Some general setters & getters
    Double_t GetMeanSeparation()   { return fMeanSeparation ; } ; 
    void SetMeanSeparation(Double_t MeanSeparation) ;

    // The coulomb correction derived in a simple toy model given in
    // G. Baym & PBM, Nucl. Phys. A610 (1996) 286c-296c
    Double_t ClassicalWeight(Double_t qinv) ;
    Double_t ClassicalWeightUnlikeSignPairs(Double_t qinv) ;

  private :
    
    // Constants
    Double_t f_hbc ;        // hbc = 0.2 [Gev][fm] adjust natural units: hb=c=1 
    Double_t fMeanSeparation ;   // <r*> in the paper =~ radius of the source
    Double_t f_d2 ;         // d2 in the paper = constant = 3pi/8
    
    // Like sign specific constants
    Double_t fBohrRadius ;  // a in the paper = bohr Radius = 388 fm for like sign pion-pion system
    Double_t fConstant1 ;   // equation 9 in the paper
    Double_t fConstant2 ;   // equation 10 in the paper
    Double_t fCriticalMomentum ;  // crossover momentum k~ in the paper equation 9
    Double_t fCriticalMomentum2 ; // square of the crossovermomentum
    
    // Unlike sign specific constants
    Double_t fBohrRadiusUSP ;  // a in the paper = bohr Radius = -388 fm for UNlike sign pion-pion system
    Double_t fConstant1USP ;   // equation 9 in the paper
    Double_t fConstant2USP ;   // equation 10 in the paper
    Double_t fCriticalMomentumUSP ;  // crossover momentum k~ in the paper equation 9
    Double_t fCriticalMomentumUSP2 ; // square of the crossovermomentum
    
    ClassDef(T49HbtCoulomb,1) ;
} ;
#endif
