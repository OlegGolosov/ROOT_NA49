#ifndef _T49VERTEX_INCLUDED_
#define _T49VERTEX_INCLUDED_

/*
$Log: T49Vertex.h,v $
Revision 1.5  2006/08/03 20:05:31  cblume
Remove unneccessary declarations

Revision 1.4  2005/04/26 18:58:13  cblume
SLC3 and rootv4 compatible version

Revision 1.3  2001/11/12 10:49:29  cblume
Update for ROOT v3.01

 * Revision 1.2  1999/11/23  12:51:32  cblume
 * Remove files
 *
*/

#include "TObjArray.h"
#include "TArrayF.h"

class T49Vertex : public TObject {

public:
    T49Vertex() { };
    T49Vertex(T49Vertex& Vertex):TObject(Vertex) { };

    virtual ~T49Vertex() { };

    virtual void    SetIflag(Int_t Flag)                        = 0;
    virtual void    SetIdDet(UChar_t DetectorId)                = 0;
    virtual void    SetIdVtx(UChar_t VertexId)                  = 0;
    virtual void    SetNTrkFit(Int_t n)                         = 0;
    virtual void    SetNTrkOut(Int_t n)                         = 0;
    virtual void    SetX(Float_t X)                             = 0;
    virtual void    SetY(Float_t Y)                             = 0;
    virtual void    SetZ(Float_t Z)                             = 0;
    virtual void    SetSigX(Float_t X)                          = 0;
    virtual void    SetSigY(Float_t Y)                          = 0; 
    virtual void    SetSigZ(Float_t Z)                          = 0;
    virtual void    SetPchi2(Float_t FitProb)                   = 0;
  
    virtual Int_t   GetIflag()                                  = 0;
    virtual UChar_t GetIdDet()                                  = 0;
    virtual UChar_t GetIdVtx()                                  = 0;
    virtual Int_t   GetNTrkFit()                                = 0;
    virtual Int_t   GetNTrkOut()                                = 0;
    virtual Float_t GetX()                                      = 0;
    virtual Float_t GetY()                                      = 0;
    virtual Float_t GetZ()                                      = 0;
    virtual Float_t GetSigX()                                   = 0;
    virtual Float_t GetSigY()                                   = 0;
    virtual Float_t GetSigZ()                                   = 0;
    virtual Float_t GetPchi2()                                  = 0;

    ClassDef(T49Vertex,2)   // Vertex base class
  
};

#endif



