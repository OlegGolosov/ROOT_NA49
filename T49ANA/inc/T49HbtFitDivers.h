/*******************************************************************************************
 * Author: Dominik Flierl, flierl@ikf.uni-frankfurt.de
 *******************************************************************************************
 * these classes inherit from T49HbtFit_Base : they provide the implementation
 * of the virtual functions of the base class
 * a simple example would look like
 *
 * TFile f("Histos.root","read") ;
 * TH3D* num = (TH3D*) f.Get("numerator") ;
 * TH3D* den = (TH3D*) f.Get("denominator") ;
 * TH3D* Meanqinv = (TH3D*) f.Get("meanqinv") ;
 * T49HbtCoulomb* cc = new T49HbtCoulomb() ;
 * TMinuit* gMinuit = new TMinuit(2) ;
 * ***** if you want to use your own start values, add the following lines *****
 * Int_t ierflg = 0 ;
 * gMinuit->mnparm(0, "Normalization", 0.1, 0.01, 0,0,ierflg);
 * gMinuit->mnparm(1, "lambda",        0.5, 0.1, 0,0,ierflg);
 * gMinuit->mnparm(2, "Rside",         5.0,  0.1, 0,0,ierflg);
 * gMinuit->mnparm(3, "Rout",          5.0,  0.1, 0,0,ierflg);
 * gMinuit->mnparm(4, "Rlong",         5.0,  0.1, 0,0,ierflg);
 * gMinuit->mnparm(5, "Routlong",      5.0,  0.1, 0,0,ierflg);
 * *****************************************************************************
 * T49HbtFit_BP_ULS_CHI2* dFitter = new T49HbtFit_BP_ULS_CHI2(gMinuit, num, den) ;
 * dFitter->SetCoulombCalculator(Meanqinv,cc,0.8) ;
 * dFitter->FillInternalArrays() ;
 * dFitter->doFit() ;
 * projector = new T49FitProjector(dFitter) ;
 * dqs = (TH1D*) (projector->get1dProjection("x",0.0,proRange,-intRange,intRange,-intRange,intRange))->Clone("dqs") ;
 * dqs->Draw()
 *
 *******************************************************************************************/

// revision 2004/10/13 calt
// in TheoreticalValue(..): TVector3 objects are now handled consistently as pointers
// added function 'InitMinuit()' to initialize TMinuit with standard start values

#ifndef T49HbtFit_Divers_hh
#define T49HbtFit_Divers_hh

#include "T49HbtFitBase.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class T49HbtFit_BP_LS_CHI2 : public T49HbtFitBase
{
  public :
    // constructors
    T49HbtFit_BP_LS_CHI2(TMinuit* gMinuit, TH3D* numerator, TH3D* denominator ) 
    : T49HbtFitBase(gMinuit,numerator,denominator)  {};
    ~T49HbtFit_BP_LS_CHI2() {} ;
  
    // implementation of virtual functions from the base class
    void InitMinuit() ;   // set default start values
    void mfcn(Int_t &nParamters, Double_t *gin, Double_t &finalChi2, Double_t *parameterSet, Int_t iflag) ; // the general fcn 
    Double_t  TheoreticalValue(TVector3* position, double* parameterSet ) ; // return calculated value 
    // root dictionary
    ClassDef(T49HbtFit_BP_LS_CHI2, 0) 
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class T49HbtFit_BP_LS_MML : public T49HbtFitBase
{
  public :
    // constructors
    T49HbtFit_BP_LS_MML(TMinuit* gMinuit, TH3D* numerator, TH3D* denominator ) 
    : T49HbtFitBase(gMinuit,numerator,denominator)  {};
    ~T49HbtFit_BP_LS_MML() {} ;
  
    // implementation of virtual functions from the base class
    void InitMinuit() ;   // set default start values
    void mfcn(Int_t &nParamters, Double_t *gin, Double_t &finalMML, Double_t *parameterSet, Int_t iflag) ; // the general fcn 
    Double_t  TheoreticalValue(TVector3* position, double* parameterSet ) ; // return calculated value 
	
    // root dictionary
    ClassDef(T49HbtFit_BP_LS_MML, 0) 
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class T49HbtFit_BP_ULS_CHI2 : public T49HbtFitBase
{
  public :
    // constructors
    T49HbtFit_BP_ULS_CHI2(TMinuit* gMinuit, TH3D* numerator, TH3D* denominator )  
    : T49HbtFitBase(gMinuit,numerator,denominator)  {};
    ~T49HbtFit_BP_ULS_CHI2() {} ;
  
    // implementation of virtual functions from the base class
    void InitMinuit() ;   // set default start values
    void mfcn(Int_t &nParamters, Double_t *gin, Double_t &finalChi2, Double_t *parameterSet, Int_t iflag) ; // the general fcn 
    Double_t  TheoreticalValue(TVector3* position, double* parameterSet ) ; // return calculated value 
	
    // root dictionary
    ClassDef(T49HbtFit_BP_ULS_CHI2, 0) 
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class T49HbtFit_YKP_LS_CHI2 : public T49HbtFitBase
{
  public :
    // constructors
    T49HbtFit_YKP_LS_CHI2(TMinuit* gMinuit, TH3D* numerator, TH3D* denominator )  
    : T49HbtFitBase(gMinuit,numerator,denominator)  {};
     ~T49HbtFit_YKP_LS_CHI2() {} ;
  
    // implementation of virtual functions from the base class
    void InitMinuit();   // set default start values
    void mfcn(Int_t &nParamters, Double_t *gin, Double_t &finalChi2, Double_t *parameterSet, Int_t iflag) ; // the general fcn 
    Double_t  TheoreticalValue(TVector3* position, double* parameterSet ) ; // return calculated value 
	
    // root dictionary
    ClassDef(T49HbtFit_YKP_LS_CHI2, 0) 
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ifndef T49HbtFit_Divers_Base_hh
#endif
