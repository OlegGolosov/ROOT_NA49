#include "T49CutPid.h"

#include "T49EventRoot.h"
#include "T49ParticleRoot.h"
#include "T49Run.h"
#include "T49Container.h"

//*-*-* ROOT includes

#include "TObjArray.h"

/*
$Log: T49CutPid.C,v $
Revision 1.3  2001/11/21 17:01:48  cblume
New version from Marco

 * Revision 1.2  2000/01/30  14:43:59  cblume
 * New versions from Marco with updated comments
 *
 * Revision 1.1  2000/01/17  12:38:06  cblume
 * Add T49CutPid.C
 *
*/

//______________________________________________________________________
// Cut class analoguous to T49CutTrack. Only provides PID cuts. Care
// should be taken to use the same quality cuts for filling the
// T49Container and selecting identified particles.

ClassImp(T49CutPid)


T49CutPid::T49CutPid():T49Cut()
{
  fMinProb=0.1;
  fMinDelta=-3;
  fMaxDelta=+3;
  fList = new TObjArray();
}

T49CutPid::~T49CutPid()
{
  delete fList;
}

TObjArray *T49CutPid::GetParticleType(TCollection *tracks, T49Container* Cont, Int_t type, Int_t charge)
{
//  Selects particles of specified type and charge and stores them in
//  a TObjArray. A pointer to the list is returned. Specify charge==0
//  to select both charges. Particle types: 0: electron, 1: pion, 2:
//  kaon, 3:proton. The selection uses the probability and the minumum
//  value set using SetMinProb().  

  if (tracks==fList) {
    printf("ERROR in T49CutPid, repeated cuts impossible!!\n");
    return fList;
  }
  fList->Clear();
   Float_t prob;
   TIter NextTrack(tracks);
   T49ParticleRoot* track;
   while ((track = (T49ParticleRoot*) NextTrack())) {
     if (track->GetCharge()*charge>=0 && 
	 (prob=Cont->GetProbability(track,type)) && 
	 prob>=fMinProb) fList->Add(track);
   }
   return fList;
}

TObjArray *T49CutPid::GetDeltaDedxParticleType(TCollection *tracks, T49Container* Cont, Int_t type, Int_t charge, Int_t flag)
{
//  Selects particles of specified type and charge and stores them in
//  a TObjArray. A pointer to the list is returned. Specify charge==0
//  to select both charges. Particle types: 0: electron, 1: pion, 2:
//  kaon, 3:proton. The selection uses the difference between the 
//  dE/dx expected for the particle-type (retrieved from the T49Container) 
//  and the actual dE/dx of a track. The selection window is set using 
//  SetDeltaDedx() (In units of standard deviation).
  //
  // If flag!=0 bins without fit return all tracks.

  if (tracks==fList) {
    printf("ERROR in T49CutPid, repeated cuts impossible!!\n");
    return fList;
  }
  fList->Clear();
  Float_t delta;
  TIter NextTrack(tracks);
  T49ParticleRoot* track;
  Int_t ret;
  while ((track = (T49ParticleRoot*) NextTrack())) {
    ret=Cont->GetDeltaDedx(track,type,delta);
    if (track->GetCharge()*charge>=0 &&
	(ret || flag) && 
	delta<=fMaxDelta && delta>=fMinDelta) fList->Add(track);
  }
  return fList;
}


void T49CutPid::ListCuts()
{
//  Prints out the values of the dE/dx cut parameters. Note that
//  which values are used depends on the cut method
//  (GetDeltaDedxParticleType() or GetParticleType())

    printf("Probability for PID   : %g\n",fMinProb);
    printf("Delta dE/dx for PID   : %g  -  %g\n",fMinDelta,fMaxDelta);
}









