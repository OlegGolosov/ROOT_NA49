#ifndef _T49CrCut_INCLUDED_
#define _T49CrCut_INCLUDED_

#include "TObject.h"
#include "TFile.h"
#include "TList.h"
#include "TObjArray.h"
#include "TTree.h"
#include "TF1.h"
#include "T49ParticleRoot.h"
#include "T49EventRoot.h"
#include "T49Vertex.h"

class T49CrCut : public TObject {
  
public: 
  T49CrCut();
  virtual ~T49CrCut(){ };
  
  Int_t Init();
  Double_t  EventCut(T49EventRoot *e);
  Int_t     EventMultiplicity(T49EventRoot *e);
  Double_t  IsGoodTrack(T49ParticleRoot *p);
  //  Double_t  IsEbETrack(T49ParticleRoot *p);
  Double_t  IsPrimary(T49ParticleRoot *p);

  ClassDef(T49CrCut,1)

    protected:
  
};

#endif
