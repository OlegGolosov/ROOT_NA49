#ifndef _T49ParticleRoot_H_INCLUDED_
#define _T49ParticleRoot_H_INCLUDED_

/*
$Log: T49ParticleRoot.h,v $
Revision 1.14  2008/01/04 10:48:59  cblume
64bit modifications by Marek Szuba

Revision 1.13  2006/08/03 20:05:14  cblume
Remove unneccessary declarations

Revision 1.12  2002/02/04 16:04:24  cblume
Change comment for TOF data

Revision 1.11  2001/12/12 17:34:55  cblume
Update default constructors

Revision 1.10  2001/11/12 10:48:34  cblume
Update for ROOT v3.01

 * Revision 1.9  2001/06/13  08:14:54  cblume
 * Add the possibility to store the index of the stop vertex
 *
 * Revision 1.8  2000/11/24  12:38:45  cblume
 * Moved inline functions to implementation file
 *
 * Revision 1.7  2000/10/31  13:34:16  cblume
 * GetPhi() is not inline any more
 *
 * Revision 1.6  2000/10/09  10:25:05  cblume
 * General update
 *
 * Revision 1.5  2000/04/19  15:21:29  cblume
 * Changed comment for fIflag
 *
 * Revision 1.4  2000/04/18  17:24:05  cblume
 * Part of the member functions are now inline
 *
 * Revision 1.3  1999/11/23  12:51:21  cblume
 * Remove files
 *
*/

#include <cmath>

#include <TBuffer.h>
#include "TObject.h"
#include "TMath.h"

#include "PhysConst.h"

#include "T49Particle.h"

class T49ParticleRoot : public T49Particle {

public:

    T49ParticleRoot();
    T49ParticleRoot(T49ParticleRoot *t);

    virtual ~T49ParticleRoot();

    virtual void    SetNPoint(Int_t i,Int_t Ntpc = 3)         { fNPoint[Ntpc]      =           i; };
    virtual void    SetNFitPoint(Int_t i, Int_t Ntpc = 3)     { fNFitPoint[Ntpc]   =           i; };
    virtual void    SetNDedxPoint(Int_t i, Int_t Ntpc = 3)    { fNDedxPoint[Ntpc]  =           i; };
    virtual void    SetNMaxPoint(Int_t i, Int_t Ntpc = 3)     { fNMaxPoint[Ntpc]   =           i; };
    virtual void    SetTmeanCharge(Float_t x, Int_t Ntpc = 3) { fTmeanCharge[Ntpc] = (Short_t) x; };

    virtual void    SetCharge(Int_t i)                        { fCharge = i;      }
    virtual void    SetIdDet(Int_t i)                         { fIdDet = i;       }
    virtual void    SetIflag(Int_t i)                         { fIflag = i;       }
    virtual void    SetPx(Float_t x)                          { fPx = x;          }
    virtual void    SetPy(Float_t x)                          { fPy = x;          }
    virtual void    SetPz(Float_t x)                          { fPz = x;          }
    virtual void    SetSigPx(Float_t x)                       { fSigPx = x;       }
    virtual void    SetSigPy(Float_t x)                       { fSigPy = x;       }
    virtual void    SetSigPz(Float_t x)                       { fSigPz = x;       }
    virtual void    SetBx(Float_t x)                          { fBx = x;          }
    virtual void    SetBy(Float_t x)                          { fBy = x;          }
    virtual void    SetPchi2(Float_t x)                       { fPchi2 = x;       }
    virtual void    SetXFirst(Float_t x)                      { fXFirst = x;      }
    virtual void    SetYFirst(Float_t x)                      { fYFirst = x;      }
    virtual void    SetZFirst(Float_t x)                      { fZFirst = x;      }
    virtual void    SetXLast(Float_t x)                       { fXLast = x;       }
    virtual void    SetYLast(Float_t x)                       { fYLast = x;       }
    virtual void    SetZLast(Float_t x)                       { fZLast = x;       }
    virtual void    SetWeight(Float_t x)                      { fWeight = x;      }
    virtual void    SetLabel(Int_t Label)                     { fLabel = Label;   }
    virtual void    SetRandom(Float_t n)                      { fRandom = n;      }
    virtual void    SetRandom();
    virtual void    SetStopVertexIndex(Int_t i)               { fVertexIndex = i; }

    virtual void    SetTofIflag(Int_t x)                      { fTofIflag = x;    }
    virtual void    SetTofId(Int_t x)                         { fTofId = x;       }
    virtual void    SetTofX(Float_t x)                        { fTofX = x;        }
    virtual void    SetTofY(Float_t x)                        { fTofY = x;        }
    virtual void    SetTofPathl(Float_t x)                    { fTofPathl = x;    }
    virtual void    SetTofCharge(Float_t x)                   { fTofCharge = x;   }
    virtual void    SetTofMass2(Float_t x)                    { fTofMass2 = x;    }
    virtual void    SetTofSigMass2(Float_t x)                 { fTofSigMass2 = x; }

    virtual void    SetXFirst(Int_t det, Float_t x);
    virtual void    SetYFirst(Int_t det, Float_t x);
    virtual void    SetZFirst(Int_t det, Float_t x);
    virtual void    SetXLast(Int_t det, Float_t x);
    virtual void    SetYLast(Int_t det, Float_t x);
    virtual void    SetZLast(Int_t det, Float_t x);

            Int_t   GetCharge()                               { return fCharge;         }
            Int_t   GetNPoint()                               { return fNPoint[3];      }
            Int_t   GetNFitPoint()                            { return fNFitPoint[3];   }
            Int_t   GetNDedxPoint()                           { return fNDedxPoint[3];  }
            Int_t   GetNMaxPoint()                            { return fNMaxPoint[3];   }
            Float_t GetTmeanCharge()                          { return fTmeanCharge[3]; }

            Int_t   GetNPoint(Int_t Ntpc)                     { return fNPoint[Ntpc];      }
            Int_t   GetNFitPoint(Int_t Ntpc)                  { return fNFitPoint[Ntpc];   }
            Int_t   GetNDedxPoint(Int_t Ntpc)                 { return fNDedxPoint[Ntpc];  }
            Int_t   GetNMaxPoint(Int_t Ntpc)                  { return fNMaxPoint[Ntpc];   }
            Float_t GetTmeanCharge(Int_t Ntpc)                { return fTmeanCharge[Ntpc]; }

            Int_t   GetIdDet()                                { return fIdDet;       }
            Int_t   GetIflag()                                { return fIflag;       }
            Float_t GetPx()                                   { return fPx;          }
            Float_t GetPy()                                   { return fPy;          }
            Float_t GetPz()                                   { return fPz;          }
            Float_t GetSigPx()                                { return fSigPx;       }
            Float_t GetSigPy()                                { return fSigPy;       }
            Float_t GetSigPz()                                { return fSigPz;       }
            Float_t GetBx()                                   { return fBx;          }
            Float_t GetBy()                                   { return fBy;          }
            Float_t GetPchi2()                                { return fPchi2;       }
            Float_t GetXFirst()                               { return fXFirst;      }
            Float_t GetYFirst()                               { return fYFirst;      }
            Float_t GetZFirst()                               { return fZFirst;      }
            Float_t GetXLast()                                { return fXLast;       }
            Float_t GetYLast()                                { return fYLast;       }
            Float_t GetZLast()                                { return fZLast;       }
            Float_t GetWeight()                               { return fWeight;      }
            Int_t   GetLabel()                                { return fLabel;       }
            Float_t GetRandom()                               { return fRandom;      }
            Int_t   GetStopVertexIndex()                      { return fVertexIndex; }

            Int_t   GetTofIflag()                             { return fTofIflag;    }
            Int_t   GetTofId()                                { return fTofId;       }
            Float_t GetTofX()                                 { return fTofX;        }
            Float_t GetTofY()                                 { return fTofY;        }
            Float_t GetTofPathl()                             { return fTofPathl;    }
            Float_t GetTofCharge()                            { return fTofCharge;   }
            Float_t GetTofMass2()                             { return fTofMass2;    }
            Float_t GetTofSigMass2()                          { return fTofSigMass2; }

            Float_t GetX0()                                   { return GetX0(0);     }
            Float_t GetY0()                                   { return GetY0(0);     }
            Float_t GetX0(Float_t z);
            Float_t GetY0(Float_t z);
            Float_t GetXPrime();
            Float_t GetYPrime();

    inline  Float_t GetPt();
            Float_t GetP()                                    { return sqrt(fPx*fPx+fPy*fPy+fPz*fPz); }
    virtual Float_t GetE(Float_t mass)                        { return sqrt(GetP()*GetP()+mass*mass); }
            Float_t GetPhi();
    virtual Float_t GetRap(Float_t mass);
            Float_t GetEta();
    virtual Float_t GetMt(Float_t mass)                       { return sqrt(mass*mass + GetPt()*GetPt()); };

            Float_t GetProbElectron()                         { return 0; };
            Float_t GetProbPion()                             { return 1; };
            Float_t GetProbKaon()                             { return 0; };
            Float_t GetProbProton()                           { return 0; };

            Float_t GetSigElectron()                          { return 0; };
            Float_t GetSigPion()                              { return 0; };
            Float_t GetSigKaon()                              { return 0; };
            Float_t GetSigProton()                            { return 0; };

            Float_t GetXFirst(Int_t det);
            Float_t GetYFirst(Int_t det);
            Float_t GetZFirst(Int_t det);
            Float_t GetXLast(Int_t det);
            Float_t GetYLast(Int_t det);
            Float_t GetZLast(Int_t det);

    virtual void    CopyParticle(T49ParticleRoot *track);

protected:

    Char_t     fIdDet;          // From track_t id_det
    Char_t     fCharge;         // From track_t qpxz
    UChar_t    fNPoint[4];      // From rtrack_t n_point
    UChar_t    fNFitPoint[4];   // From track_t  n_fit_point
    UChar_t    fNDedxPoint[4];  // From rtrack_t n_dedx_point
    UChar_t    fNMaxPoint[4];   // From rtrack_t n_max_point
    Int_t      fIflag;          // From rtrack_t iflag
    UShort_t   fTmeanCharge[4]; // From rtrack_t tmean_charge
    Float_t    fPx;             // From track_t px
    Float_t    fPy;             // From track_t py
    Float_t    fPz;             // From track_t pz
    Float_t    fSigPx;          // From track_t sig_px
    Float_t    fSigPy;          // From track_t sig_py
    Float_t    fSigPz;          // From track_t sig_pz
    Float_t    fBx;             // From track_t bx
    Float_t    fBy;             // From track_t by
    Float_t    fPchi2;          // From track_t pchi2;
    Float_t    fXFirst;         // From rtrack_t x_first
    Float_t    fYFirst;         // From rtrack_t y_first
    Float_t    fZFirst;         // From rtrack_t z_first
    Float_t    fXLast;          // From rtrack_t x_last
    Float_t    fYLast;          // From rtrack_t y_last
    Float_t    fZLast;          // From rtrack_t z_last
    Float_t    fWeight;         // Weight factor for future use
    Float_t    fRandom;         // Random track identifier for sample splitting
    Int_t      fLabel;          // Track label inside T49Event
    Int_t      fVertexIndex;    // Index to the stop vertex
    Int_t      fTofIflag;       // TOF quality flag
    Int_t      fTofId;          // TOF PM number
    Float_t    fTofX;           // TOF hit position in det coord.
    Float_t    fTofY;           // TOF hit position in det coord.
    Float_t    fTofPathl;       // TOF tracklength from target to det
    Float_t    fTofCharge;      // TOF normalized QDC signal
    Float_t    fTofMass2;       // TOF squared mass
    Float_t    fTofSigMass2;    // TOF squared mass

    // Added for version 2
    Float_t    fXLocFirst[3];   // From rtrack_t x_first
    Float_t    fYLocFirst[3];   // From rtrack_t y_first
    Float_t    fZLocFirst[3];   // From rtrack_t z_first
    Float_t    fXLocLast[3];    // From rtrack_t x_last
    Float_t    fYLocLast[3];    // From rtrack_t y_last
    Float_t    fZLocLast[3];    // From rtrack_t z_last

    ClassDef(T49ParticleRoot,3) // Particle information

};

//______________________________________________________________________________
inline Float_t T49ParticleRoot::GetPt()
{
  //
  // The transverse momentum
  //

  return TMath::Sqrt(GetPx()*GetPx()+GetPy()*GetPy());

}

#endif








