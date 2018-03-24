#ifndef _T49ParticleIdRoot_H_INCLUDED_
#define _T49ParticleIdRoot_H_INCLUDED_

/*
$Log: T49ParticleIdRoot.h,v $
Revision 1.3  2001/11/12 10:48:21  cblume
Update for ROOT v3.01

 * Revision 1.2  2000/10/09  10:25:04  cblume
 * General update
 *
 * Revision 1.1  1999/11/23  12:55:45  cblume
 * Add T49ParticleIdRoot.h
 *
*/

#include <TBuffer.h>
#include "T49ParticleRoot.h"

class T49ParticleIdRoot : public T49ParticleRoot {

public:

    T49ParticleIdRoot();
    T49ParticleIdRoot(T49ParticleIdRoot *Track);

    virtual ~T49ParticleIdRoot() { };

    virtual void    SetProbElectron(Float_t prob) { fProbElectron = prob; }
    virtual void    SetProbPion(Float_t prob)     { fProbPion     = prob; }
    virtual void    SetProbKaon(Float_t prob)     { fProbKaon     = prob; }
    virtual void    SetProbProton(Float_t prob)   { fProbProton   = prob; }

    virtual void    SetSigElectron(Float_t sig)   { fSigElectron  = sig; }
    virtual void    SetSigPion(Float_t sig)       { fSigPion      = sig; }
    virtual void    SetSigKaon(Float_t sig)       { fSigKaon      = sig; }
    virtual void    SetSigProton(Float_t sig)     { fSigProton    = sig; }

    virtual Float_t GetProbElectron()             { return fProbElectron; };
    virtual Float_t GetProbPion()                 { return fProbPion;     };
    virtual Float_t GetProbKaon()                 { return fProbKaon;     };
    virtual Float_t GetProbProton()               { return fProbProton;   };

    virtual Float_t GetSigElectron()              { return fSigElectron;  };
    virtual Float_t GetSigPion()                  { return fSigPion;      };
    virtual Float_t GetSigKaon()                  { return fSigKaon;      };
    virtual Float_t GetSigProton()                { return fSigProton;    };

protected:

    Float_t    fProbElectron;                  // Probability for an electron
    Float_t    fProbPion;                      // Probability for a pion
    Float_t    fProbKaon;                      // Probability for a kaon
    Float_t    fProbProton;                    // Probability for a proton

    Float_t    fSigElectron;                   // Sigma of the electron dEdx distribution
    Float_t    fSigPion;                       // Sigma of the pion dEdx distribution
    Float_t    fSigKaon;                       // Sigma of the kaon dEdx distribution
    Float_t    fSigProton;                     // Sigma of the proton dEdx distribution

    ClassDef(T49ParticleIdRoot,2)              // Identified particle information

};

#endif












