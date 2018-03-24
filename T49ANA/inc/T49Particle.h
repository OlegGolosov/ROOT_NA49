#ifndef _T49Particle_H_INCLUDED_
#define _T49Particle_H_INCLUDED_

/*
$Log: T49Particle.h,v $
Revision 1.4  2006/08/03 20:04:59  cblume
Remove unneccessary declarations

Revision 1.3  2001/11/12 10:48:14  cblume
Update for ROOT v3.01

 * Revision 1.2  1999/11/23  12:51:19  cblume
 * Remove files
 *
*/

#include "TObject.h"

class T49Particle : public TObject {

public:

    T49Particle() { };
    virtual ~T49Particle() { };

    virtual void    SetIdDet(Int_t i)                           = 0;
    virtual void    SetIflag(Int_t i)                           = 0;

    virtual void    SetNPoint(Int_t i,Int_t Ntpc = 3)           = 0;
    virtual void    SetNFitPoint(Int_t i, Int_t Ntpc = 3)       = 0;
    virtual void    SetNDedxPoint(Int_t i, Int_t Ntpc = 3)      = 0;
    virtual void    SetNMaxPoint(Int_t i, Int_t Ntpc = 3)       = 0;
    virtual void    SetTmeanCharge(Float_t x, Int_t Ntpc = 3)   = 0;

    virtual void    SetCharge(Int_t i)                          = 0;
    virtual void    SetPx(Float_t x)                            = 0;
    virtual void    SetPy(Float_t x)                            = 0;
    virtual void    SetPz(Float_t x)                            = 0;
    virtual void    SetSigPx(Float_t x)                         = 0;
    virtual void    SetSigPy(Float_t x)                         = 0;
    virtual void    SetSigPz(Float_t x)                         = 0;
    virtual void    SetPchi2(Float_t x)                         = 0;

    virtual void    SetBx(Float_t x)                            = 0;
    virtual void    SetBy(Float_t x)                            = 0;
    virtual void    SetXFirst(Float_t x)                        = 0;
    virtual void    SetYFirst(Float_t x)                        = 0;
    virtual void    SetZFirst(Float_t x)                        = 0;
    virtual void    SetXLast(Float_t x)                         = 0;
    virtual void    SetYLast(Float_t x)                         = 0;
    virtual void    SetZLast(Float_t x)                         = 0;

    virtual void    SetWeight(Float_t x)                        = 0;
    virtual void    SetLabel(Int_t Label)                       = 0;
    virtual void    SetRandom()                                 = 0;

    virtual void    SetTofId(Int_t i)                           = 0;
    virtual void    SetTofIflag(Int_t i)                        = 0;
    virtual void    SetTofX(Float_t x)                          = 0;
    virtual void    SetTofY(Float_t x)                          = 0;
    virtual void    SetTofCharge(Float_t x)                     = 0;
    virtual void    SetTofMass2(Float_t x)                      = 0;
    virtual void    SetTofSigMass2(Float_t x)                   = 0;
    
    virtual Int_t   GetIdDet()                                  = 0;         
    virtual Int_t   GetIflag()                                  = 0;         

    virtual Int_t   GetNPoint()                                 = 0;         
    virtual Int_t   GetNFitPoint()                              = 0;         
    virtual Int_t   GetNDedxPoint()                             = 0;         
    virtual Int_t   GetNMaxPoint()                              = 0;         
    virtual Float_t GetTmeanCharge()                            = 0;        

    virtual Int_t   GetNPoint(Int_t Ntpc)                       = 0;         
    virtual Int_t   GetNFitPoint(Int_t Ntpc)                    = 0;         
    virtual Int_t   GetNDedxPoint(Int_t Ntpc)                   = 0;         
    virtual Int_t   GetNMaxPoint(Int_t Ntpc)                    = 0;         
    virtual Float_t GetTmeanCharge(Int_t Ntpc)                  = 0;        

    virtual Int_t   GetCharge()                                 = 0;         
    virtual Float_t GetPx()                                     = 0;        
    virtual Float_t GetPy()                                     = 0;        
    virtual Float_t GetPz()                                     = 0;        
    virtual Float_t GetSigPx()                                  = 0;        
    virtual Float_t GetSigPy()                                  = 0;        
    virtual Float_t GetSigPz()                                  = 0;        
    virtual Float_t GetPchi2()                                  = 0;       

    virtual Float_t GetBx()                                     = 0;       
    virtual Float_t GetBy()                                     = 0;       
    virtual Float_t GetXFirst()                                 = 0;        
    virtual Float_t GetYFirst()                                 = 0;        
    virtual Float_t GetZFirst()                                 = 0;        
    virtual Float_t GetXLast()                                  = 0;        
    virtual Float_t GetYLast()                                  = 0;        
    virtual Float_t GetZLast()                                  = 0;        

    virtual Float_t GetWeight()                                 = 0;       
    virtual Int_t   GetLabel()                                  = 0;        
    virtual Float_t GetRandom()                                 = 0;       

    virtual Float_t GetPt()                                     = 0;       
    virtual Float_t GetP()                                      = 0;       
    virtual Float_t GetE(Float_t mass)                          = 0;       
    virtual Float_t GetPhi()                                    = 0;       
    virtual Float_t GetRap(Float_t mass)                        = 0;       
    virtual Float_t GetMt(Float_t mass)                         = 0;       

    virtual Int_t   GetTofId()                                  = 0;
    virtual Int_t   GetTofIflag()                               = 0;
    virtual Float_t GetTofX()                                   = 0;
    virtual Float_t GetTofY()                                   = 0;
    virtual Float_t GetTofCharge()                              = 0;
    virtual Float_t GetTofMass2()                               = 0;
    virtual Float_t GetTofSigMass2()                            = 0;

    ClassDef(T49Particle,1) // Particle base class
   
};

#endif



  
