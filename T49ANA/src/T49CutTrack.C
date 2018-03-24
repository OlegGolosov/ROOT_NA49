//////////////////////////////////////////////////////////
//                                                      //
//              T49CutTrack Class                       //
//                                                      //
//  Cut on tracks in T49ParticleRoot format             //
//                                                      //
//  UpDate: 26.10.99                                    //
//                                                      //
//  c.markert@gsi.de                                    //
//                                                      //
//////////////////////////////////////////////////////////

#include <TClass.h>
#include <TPaveText.h>
#include <TCutG.h>
#include <TArrayI.h>
#include <TObjArray.h>
#include <TF1.h>

#include "T49ParticleRoot.h"

#include "T49CutTrack.h"
#include "T49Prob.h"

ClassImp(T49CutTrack)

/*
$Log: T49CutTrack.C,v $
Revision 1.10  2008/01/04 10:48:58  cblume
64bit modifications by Marek Szuba

Revision 1.9  2005/04/26 18:58:13  cblume
SLC3 and rootv4 compatible version

Revision 1.8  2001/11/12 12:10:29  cblume
Update for ROOT v3.01

 * Revision 1.7  2000/11/09  08:58:39  cblume
 * Introduced dEdx-cut with variable sigma
 *
 * Revision 1.6  1999/12/01  09:39:23  cblume
 * Changed to root 2.23-09. Updated the makefiles. Changes in T49CutTrack, T49Dedx, T49Proj1, T49Proj2, TRootDS
 *
 * Revision 1.5  1999/11/23  13:54:03  cblume
 * Remove files
 *
*/

//______________________________________________________________________________
T49CutTrack::T49CutTrack():T49Cut()
{
  //
  // Default constructor
  //

  Reset();

}

//______________________________________________________________________________
T49CutTrack::T49CutTrack(const Text_t *name, const Text_t *title)
            :T49Cut(name,title)
{

  //
  // Default constructor with name and title
  //

  Reset();
  
}

//______________________________________________________________________________
T49CutTrack::~T49CutTrack()
{

  delete fAcceptedParticleList;
  delete fProbability;
  delete fTofCutPoly;
  delete fFunDedxSigma;

}

//______________________________________________________________________________
void T49CutTrack::Reset()
{

    fnumtrack     = 0;     
    fnumgoodtrack = 0;
 
    fFlagIflag          = kFALSE;
    fFlagCharge         = kFALSE;
    fFlagIdDet          = kFALSE;
    fFlagIdDetList      = kFALSE;
    fFlagBx             = kFALSE;
    fFlagBy             = kFALSE; 
    fFlagBxByEllipse    = kFALSE;  
    fFlagPchi2          = kFALSE;
    fFlagP              = kFALSE;
    fFlagPt             = kFALSE;
    fFlagPx             = kFALSE;
    fFlagPy             = kFALSE;
    fFlagPz             = kFALSE;
    fFlagXFirst         = kFALSE;
    fFlagXLast          = kFALSE;
    fFlagYFirst         = kFALSE;
    fFlagYLast          = kFALSE;
    fFlagZFirst         = kFALSE;
    fFlagZLast          = kFALSE;
    for(Int_t i=0; i< 4; i++)
    {
      fFlagNPoint[i]             = kFALSE;
      fFlagNFitPoint[i]          = kFALSE;
      fFlagNMaxPoint[i]          = kFALSE;
      fFlagNPointToNMaxPoint[i]  = kFALSE;
      fFlagNDedxPoint[i]         = kFALSE;
      fFlagTmeanCharge[i]        = kFALSE;
    }
    fFlagRap            = kFALSE;

    fFlagTofIflag       = kFALSE;
    fFlagTofId          = kFALSE;
    fFlagTofMass2       = kFALSE;
    fFlagTofSigMass2    = kFALSE;
    fFlagTofPathl       = kFALSE;
    fFlagTofX           = kFALSE;
    fFlagTofY           = kFALSE;
    fFlagTofCharge      = kFALSE; 
    fFlagTofCutPoly     = kFALSE;

    fFlagDedx                = kFALSE;
    fFlagProbability         = kFALSE;
    fFlagProtonProbability   = kFALSE; 
    fFlagKaonProbability     = kFALSE; 
    fFlagPionProbability     = kFALSE; 
    fFlagElectronProbability = kFALSE; 
    fFlagDedxSigma           = kFALSE;
    fFlagDedxVarSigma        = kFALSE;

    fnIflag          = 0;
    fnCharge         = 0;
    fnIdDet          = 0;
    fnIdDetList      = 0;
    fnBx             = 0;
    fnBy             = 0; 
    fnBxByEllipse    = 0;
    fnPchi2          = 0;
    fnP              = 0;
    fnPt             = 0;
    fnPx             = 0;
    fnPy             = 0;
    fnPz             = 0;
    fnXFirst         = 0;
    fnXLast          = 0;
    fnYFirst         = 0;
    fnYLast          = 0;
    fnZFirst         = 0;
    fnZLast          = 0;
 
    for(Int_t i=0; i< 4; i++)
    {
      fnNPoint[i]      = 0;
      fnNFitPoint[i]   = 0;
      fnNMaxPoint[i]   = 0;
      fnNDedxPoint[i]  = 0;
      fnTmeanCharge[i] = 0;
    }
    fnRap            = 0;

    fnTofIflag       = 0;
    fnTofId          = 0;
    fnTofMass2       = 0;
    fnTofSigMass2    = 0;
    fnTofPathl       = 0;
    fnTofX           = 0;
    fnTofY           = 0;
    fnTofCharge      = 0; 
    fnTofCutPoly     = 0;
  
    fnDedx                = 0;
    fnProtonProbability   = 0; 
    fnKaonProbability     = 0; 
    fnPionProbability     = 0; 
    fnElectronProbability = 0; 
    fnDedxSigma           = 0;

    fTofCutPoly           = new TObjArray(); 
    fAcceptedParticleList = new TObjArray();
    fProbability          = new T49Prob();
    fFunDedxSigma         = new TF1();

}

//______________________________________________________________________________
void T49CutTrack::SetStandardCutsPP()
{    
  //
  // Sets the standard cuts for p+p events
  //           -5 < Bx < 5
  //           -2 < By < 2
  //           Npoints / Nmax-points (global) > 0.3
  //

  SetBx(-5,5);           
  SetBy(-2,2);   
  SetNPointToNMaxPoint(3,0.3); // 3 = global tracks 0.3 = ratio numpoints/maxpoints 

}

//______________________________________________________________________________
void T49CutTrack::SetStandardCutsPPb()
{
  //
  // Sets the standard cuts for p+Pb events
  //           -5 < Bx < 5
  //           -2 < By < 2
  //           Npoints / Nmax-points (global) > 0.3
    
  SetBx(-5,5);           
  SetBy(-2,2); 
  SetNPointToNMaxPoint(3,0.3); // 3 = global tracks 0.3 = ratio numpoints/maxpoints   

}

//______________________________________________________________________________
void T49CutTrack::SetStandardCutsPbPb()
{    
  // 
  // Sets the standard cuts for Pb+Pb events
  //           -5 < Bx < 5
  //           -2 < By < 2
  //           Npoints / Nmax-points (global) > 0.3
  //

  SetBx(-5,5);           
  SetBy(-2,2); 
  SetNPointToNMaxPoint(3,0.3); // 3 = global tracks 0.3 = ratio numpoints/maxpoints 
}

//______________________________________________________________________________
void T49CutTrack::SetDedxVarSigma(Int_t data, Int_t pid, Int_t tpc
                                , Double_t min, Double_t max
                                , TF1 *sigma, Double_t pmin)
{
  //
  // Set the dedx-cut with variable sigma
  //

  fFlagDedxVarSigma = kTRUE;
  fDataTyp          = data; 
  fParticleId       = pid;
  fDedxTpc          = tpc;
  fMinDedxSigma     = min;
  fMaxDedxSigma     = max; 
  if (fFunDedxSigma) delete fFunDedxSigma;
  fFunDedxSigma     = sigma; 
  fDedxPmin         = pmin;

}

//______________________________________________________________________________
Int_t T49CutTrack::CheckParticle(T49ParticleRoot *track)
{
  //
  // Checks whether a track passes all defined cuts
  //

    fnumtrack++;
    if(fFlagIflag && (track->GetIflag() != fIflag) )
    {
       if(fVerbose)      
         printf("Track not accepted  (Iflag cut). \n");
       fnIflag++;
       return kFALSE;
    }
    if(fFlagCharge && (track->GetCharge() != fCharge) )
    {
       if(fVerbose)      
         printf("Track not accepted  (Charge cut). \n");
       fnCharge++;
       return kFALSE;
    }
    if(fFlagIdDet && (track->GetIdDet() != fIdDet) )
    {
      if(fVerbose)      
        printf("Track not accepted  (IdDet cut). \n");
      fnIdDet++;
      return kFALSE;
    }  

    if(fFlagIdDetList)    
    {
       Int_t z=0;
       for(Int_t i =0;i<fIdDetList.GetSize();i++)
       {       
         if(fIdDetList.At(i) == track->GetIdDet())
           z=1;
       }
       if( z != 1)
       {  
         if(fVerbose)      
           printf("Track not accepted  (IdDet cut). \n");
         fnIdDetList++;
         return kFALSE;
       }
    }
    if(fFlagBx && (track->GetBx() < fMinBx || track->GetBx() > fMaxBx) )
    {
      if(fVerbose)
	printf("Track not accepted  (Bx cut). \n");
      fnBx++;
      return kFALSE;
    }     
    if(fFlagBy && (track->GetBy() < fMinBy || track->GetBy() > fMaxBy) )
    {
      if(fVerbose)
	printf("Track not accepted  (By cut). \n");
      fnBy++;
      return kFALSE;
    }     
    if(fFlagBxByEllipse)
    {
      if( (track->GetBx()/fBxEllipse*track->GetBx()/fBxEllipse)
         +(track->GetBy()/fByEllipse*track->GetBy()/fByEllipse) > 1)
      {
         if(fVerbose)
	   printf("Track not accepted  (BxByEllipse cut) bx = %f, by = %f. \n"
                 ,track->GetBx(),track->GetBy());
         fnBxByEllipse++;
         return kFALSE;
      }
    }      

    for(Int_t i=0; i< 4; i++)
    {
      
      if(fFlagNPoint[i] && (track->GetNPoint(i) < fMinNPoint[i] || 
                            track->GetNPoint(i) > fMaxNPoint[i]) ) 
      {
        if(fVerbose)
	  printf("Track not accepted  ( NPoint[%d]cut). \n",i);
        fnNPoint[i]++;
        return kFALSE;
      } 
      
      if(fFlagNFitPoint[i] && (track->GetNFitPoint(i) < fMinNFitPoint[i] || 
                               track->GetNFitPoint(i) > fMaxNFitPoint[i]) ) 
      {
        if(fVerbose)
	  printf("Track not accepted  ( NFitPoint[%d]cut). \n",i);
        fnNFitPoint[i]++;
        return kFALSE;
      } 
      if(fFlagNMaxPoint[i] && (track->GetNMaxPoint(i) < fMinNMaxPoint[i] || 
                               track->GetNMaxPoint(i) > fMaxNMaxPoint[i]) ) 
      {
        if(fVerbose)
	  printf("Track not accepted  ( NMaxPoint[%d]cut). \n",i);
        fnNMaxPoint[i]++;
        return kFALSE;
      }  
      if(fFlagNPointToNMaxPoint[i] && 
         ((((Float_t) track->GetNPoint(i)) / ((Float_t) track->GetNMaxPoint(i))) 
           < fNPointToNMaxPoint[i])) 
      {
        if(fVerbose)
	  printf("Track not accepted  ( NPointToNMaxPoint[%d]cut). \n",i);
        fnNPointToNMaxPoint[i]++;
        return kFALSE;
      }  
    }
    
    if(fFlagPchi2 && (track->GetPchi2() < fMinPchi2 || track->GetPchi2() > fMaxPchi2) )
    {
      if(fVerbose)
	printf("Track not accepted  (Pchi2 cut). \n");
      fnPchi2++;
      return kFALSE;
    }     
    if(fFlagP && (track->GetP() < fMinP || track->GetP() > fMaxP) )
    {
      if(fVerbose)
	printf("Track not accepted  (P cut). \n");
      fnP++;
      return kFALSE;
    }  
    if(fFlagPt && (track->GetPt() < fMinPt || track->GetPt() > fMaxPt) )
    {
      if(fVerbose)
	printf("Track not accepted  (Pt cut). \n");
      fnPt++;
      return kFALSE;
    }     
    if(fFlagPx && (track->GetPx() < fMinPx || track->GetPx() > fMaxPx) )
    {
      if(fVerbose)
	printf("Track not accepted  (Px cut). \n");
      fnPx++;
      return kFALSE;
    }     
    if(fFlagPy && (track->GetPy() < fMinPy || track->GetPy() > fMaxPy) )
    {
      if(fVerbose)
	printf("Track not accepted  (Py cut). \n");
      fnPy++;
      return kFALSE;
    }     
    if(fFlagPz && (track->GetPz() < fMinPz || track->GetPz() > fMaxPz) )
    {
      if(fVerbose)
	printf("Track not accepted  (Pz cut). \n");
      fnPz++;
      return kFALSE;
    }   
    if(fFlagXFirst && (track->GetXFirst() < fMinXFirst || track->GetXFirst() > fMaxXFirst) )
    {
      if(fVerbose)
	printf("Track not accepted  (XFirst cut). \n");
      fnXFirst++;
      return kFALSE;
    }   
    if(fFlagXLast && (track->GetXLast() < fMinXLast || track->GetXLast() > fMaxXLast) )
    {
      if(fVerbose)
	printf("Track not accepted  (XLast cut). \n");
      fnXLast++;
      return kFALSE;
    } 
    if(fFlagYFirst && (track->GetYFirst() < fMinYFirst || track->GetYFirst() > fMaxYFirst) )
    {
      if(fVerbose)
	printf("Track not accepted  (YFirst cut). \n");
      fnYFirst++;
      return kFALSE;
    }   
    if(fFlagYLast && (track->GetYLast() < fMinYLast || track->GetYLast() > fMaxYLast) )
    {
      if(fVerbose)
	printf("Track not accepted  (YLast cut). \n");
      fnYLast++;
      return kFALSE;
    } 
    if(fFlagZFirst && (track->GetZFirst() < fMinZFirst || track->GetZFirst() > fMaxZFirst) )
    {
      if(fVerbose)
	printf("Track not accepted  (ZFirst cut). \n");
      fnZFirst++;
      return kFALSE;
    }   
    if(fFlagZLast && (track->GetZLast() < fMinZLast || track->GetZLast() > fMaxZLast) )
    {
      if(fVerbose)
	printf("Track not accepted  (ZLast cut). \n");
      fnZLast++;
      return kFALSE;
    } 
 
    for(Int_t i=0; i< 4; i++)
    {
      if(fFlagNDedxPoint[i] && (track->GetNDedxPoint(i) < fMinNDedxPoint[i] || 
                                track->GetNDedxPoint(i) > fMaxNDedxPoint[i]) )
      {
        if(fVerbose)
	  printf("Track not accepted  ( NDedxPoint[%d]cut). \n",i);
        fnNDedxPoint[i]++;
        return kFALSE;
      } 
      if(fFlagTmeanCharge[i] && 
         (track->GetTmeanCharge(i) < fMinTmeanCharge[i] || 
          track->GetTmeanCharge(i) > fMaxTmeanCharge[i]) ) 
      {
        if(fVerbose)
	  printf("Track not accepted  ( TmeanCharge[%d]cut). \n",i);
        fnTmeanCharge[i]++;
        return kFALSE;
      } 
    }
    if(fFlagRap)
    {
      Double_t mass = 0;
      Double_t rap;

      if(fRapParticleId == 0)
        mass = kEMASS;
      if(fRapParticleId == 1)
        mass = kPIMASS;
      if(fRapParticleId == 2)
        mass = kKMASS;
      if(fRapParticleId == 3)
        mass = kPMASS;
 
      rap = GetRap(mass,track);
      if( rap < fMinRap || rap > fMaxRap )
      {
         if(fVerbose)
	  printf("Track not accepted  (Rapidity cut). \n");
         fnRap++;
         return kFALSE;
      }
    } 
   
    // T O F CUTS
    if(fFlagTofIflag && (track->GetTofIflag() < fMinTofIflag || 
                         track->GetTofIflag() > fMaxTofIflag)  )
    {
      if(fVerbose)
	printf("Track not accepted  (TofIflag cut). \n");
      fnTofIflag++;
      return kFALSE;
    } 
    if(fFlagTofId && (track->GetTofId() != fTofId) )
    {
      if(fVerbose)
	printf("Track not accepted  (TofId cut). \n");
      fnTofId++;
      return kFALSE;
    }   
    if(fFlagTofMass2 && (track->GetTofMass2() < fMinTofMass2 || 
                         track->GetTofMass2() > fMaxTofMass2) )
    {
      if(fVerbose)
	printf("Track not accepted  (TofMass2 cut). \n");
      fnTofMass2++;
      return kFALSE;
    } 
    if(fFlagTofSigMass2 && (track->GetTofSigMass2() < fMinTofSigMass2 || 
                            track->GetTofSigMass2() > fMaxTofSigMass2) )
    {
      if(fVerbose)
	printf("Track not accepted  (TofSigMass2 cut). \n");
      fnTofSigMass2++;
      return kFALSE;
    } 
    if(fFlagTofPathl && (track->GetTofPathl() < fMinTofPathl || 
                         track->GetTofPathl() > fMaxTofPathl) )
    {
      if(fVerbose)
	printf("Track not accepted  (TofPathl cut). \n");
      fnTofPathl++;
      return kFALSE;
    } 
    if(fFlagTofX && (track->GetTofX() < fMinTofX || track->GetTofX() > fMaxTofX) )
    {
      if(fVerbose)
	printf("Track not accepted  (TofX cut). \n");
      fnTofX++;
      return kFALSE;
    } 
    if(fFlagTofY && (track->GetTofY() < fMinTofY || track->GetTofY() > fMaxTofY) )
    {
      if(fVerbose)
	printf("Track not accepted  (TofY cut). \n");
      fnTofY++;
      return kFALSE;
    } 
    if(fFlagTofCharge && (track->GetTofCharge() < fMinTofCharge || 
                          track->GetTofCharge() > fMaxTofCharge) )
    {
      if(fVerbose)
	printf("Track not accepted  (TofCharge cut). \n");
      fnTofCharge++;
      return kFALSE;
    } 
    if(fFlagTofCutPoly)
    { 
      Char_t  cname[256];

      TCutG  *cut;
      Double_t  p = track->GetP();
      Double_t  mtpcdedx = track->GetTmeanCharge(2)/1000;
      Double_t  mass2 = track->GetTofMass2(); 

      if(track->GetCharge() == -1)
        sprintf(cname,"CMass2Dedx%s[%d]","N",int(p));
      
      if(track->GetCharge() == 1)
        sprintf(cname,"CMass2Dedx%s[%d]","P",int(p));
    
      cut = NULL;
      cut = (TCutG *) fTofCutPoly->FindObject(cname); 
      
      if (cut == NULL)
      {  
       
         if(fVerbose)
	   printf("Track not accepted p not in this region  (TofCutPoly cut). \n");
         fnTofCutPoly++;
         return kFALSE;
      }
      if( cut->IsInside(mass2,mtpcdedx) == 0)
      {         
         if(fVerbose)
	  printf("Track not accepted  (TofCutPoly cut). \n");
         fnTofCutPoly++;
         return kFALSE;
      }   
  
    }
   
    //DEDX  cuts
    if(fFlagDedx)
    {
      Float_t dedx = track->GetTmeanCharge(fDedxTPC)/1000;
      if( dedx < fMinDedx || dedx > fMaxDedx)
      {
        if(fVerbose)
	  printf("Track not accepted  (Dedx cut). \n");
        fnDedx++;
        return kFALSE;
      }
    } 

    //DEDX  sigma cuts
    if(fFlagDedxSigma)
    {
      fdedx = track->GetTmeanCharge(fDedxTpc)/1000;
      if(fDataTyp == 0)
         fmean =  fProbability->GetParaPP(fParticleId,track->GetCharge(),track->GetP()); 
      if(fDataTyp == 1)
         fmean =  fProbability->GetParaPbPb(fParticleId,track->GetCharge(),track->GetP()); 

      if( fdedx < fMinDedxSigma*fmean || fdedx > fMaxDedxSigma*fmean)
      {
        if(fVerbose)
	  printf("Track not accepted  (DedxSigma cut). \n");
        fnDedxSigma++;
        return kFALSE;
      }
    } 

    // DEDX sigma cuts with ptot-dependent sigma
    if (fFlagDedxVarSigma) {
      fdedx = track->GetTmeanCharge(fDedxTpc) / 1000.0;
      if (fDataTyp == 0)
        fmean = fProbability->GetParaPP(fParticleId,track->GetCharge(),track->GetP()); 
       if (fDataTyp == 1)
        fmean = fProbability->GetParaPbPb(fParticleId,track->GetCharge(),track->GetP()); 
      Float_t sigma = fFunDedxSigma->Eval(track->GetP()) *fmean;
      if (track->GetP() > fDedxPmin) {
        if ((fdedx < (fmean + fMinDedxSigma * sigma)) || 
            (fdedx > (fmean + fMaxDedxSigma * sigma))) {
          if (fVerbose) printf("Track not accepted  (DedxVarSigma cut). \n");
          fnDedxSigma++;
          return kFALSE;
	}
      }
    } 

    //Probability cuts
    if(fFlagProbability)
    {   
       Float_t dedx = track->GetTmeanCharge(fDedxTPC)/1000;
       Float_t p = track->GetP();
       Int_t   charge = track->GetCharge();
       Float_t prob;
    

       prob = fProbability->GetProtonProbability(charge,p,dedx);
       if(fFlagProtonProbability && ( prob< fMinProtonProbability) )
       {
         if(fVerbose)
	   printf("Track not accepted  (ProtonProbability cut). \n");
         fnProtonProbability++;
         return kFALSE;
       } 
       prob = fProbability->GetKaonProbability(charge,p,dedx);
       if(fFlagKaonProbability && (prob < fMinKaonProbability) )
       {
         if(fVerbose)
	   printf("Track not accepted  (KaonProbability cut). \n");
         fnKaonProbability++;
         return kFALSE;
       } 
       prob = fProbability->GetPionProbability(charge,p,dedx);
       if(fFlagPionProbability && (prob < fMinPionProbability) )
       {
         if(fVerbose)
	   printf("Track not accepted  (PionProbability cut). \n");
         fnPionProbability++;
         return kFALSE;
       } 
       prob = fProbability->GetElectronProbability(charge,p,dedx);
       if(fFlagElectronProbability && (prob< fMinElectronProbability) )
       {
         if(fVerbose)
	   printf("Track not accepted  (ElectronProbability cut). \n");
         fnElectronProbability++;
         return kFALSE;
       } 
    }
 
    fnumgoodtrack++;
    return kTRUE;  
}

//______________________________________________________________________________
TObjArray *T49CutTrack::GetAcceptedParticles(TObjArray *ParticleList)
{
  //
  // Returns a list of all tracks that pass the cuts
  //

    T49ParticleRoot *track;
    fAcceptedParticleList->Clear();
   
    TIter NextParticle(ParticleList);
    while( (track = (T49ParticleRoot *) NextParticle()) != 0)
    {
      if( CheckParticle(track) == 1)
	fAcceptedParticleList->Add(track);            
    }
    if(fVerbose)
    {
      printf("Found %d accepted particles. \n", fAcceptedParticleList->GetEntries());
    }
    return fAcceptedParticleList;

}

//______________________________________________________________________________
TPaveText  *T49CutTrack::GetCutList()
{
  //
  // Creates a TPaveText with the defined cuts and the cut-statistics
  //
  
   TPaveText *pave      = new TPaveText(0,0,1,1);

   char          hname[250]; 

   sprintf(hname,"total number of tracks : %d \n",fnumtrack);  
   pave->AddText(hname);

   sprintf(hname,"number of tracks accepted : %d \n",fnumgoodtrack);  
   pave->AddText(hname);

    
   if(fFlagIflag)
   {     
     sprintf(hname,"SetIflag(%d) : %d \n", fIflag,fnIflag);
     pave->AddText(hname);
   }
   if(fFlagCharge)
   {     
     sprintf(hname,"SetCharge(%d) : %d \n", fCharge,fnCharge);
     pave->AddText(hname);
   }
   if(fFlagIdDet)
   {     
     sprintf(hname,"SetIdDet(%d) : %d \n", fIdDet,fnIdDet);
     pave->AddText(hname);
   }
   if(fFlagIdDetList)
   {     
     sprintf(hname,"SetIdDetList : %d \n", fnIdDetList);
     pave->AddText(hname);
   }
   if(fFlagBx)
   {     
     sprintf(hname,"SetBx(%f,%f) : %d \n", fMinBx, fMaxBx,fnBx);
     pave->AddText(hname);
   }
   if(fFlagBy)
   {     
     sprintf(hname,"SetBy(%f,%f) : %d \n", fMinBy, fMaxBy,fnBy);
     pave->AddText(hname);
   }
   if(fFlagBxByEllipse)
   {     
     sprintf(hname,"SetBxByEllipse(%f,%f) : %d \n", fBxEllipse, fByEllipse,fnBxByEllipse);
     pave->AddText(hname);
   }
   for( Int_t i = 0;i<4;i++)
   {
      if(fFlagNPoint[i])
      {     
        sprintf(hname,"SetNPoint(%d,%d,%d) : %d \n",i, fMinNPoint[i]
                                                     , fMaxNPoint[i]
                                                     , fnNPoint[i]);
        pave->AddText(hname);
      }
      if(fFlagNFitPoint[i])
      {     
        sprintf(hname,"SetNFitPoint(%d,%d,%d) : %d \n",i, fMinNFitPoint[i]
                                                        , fMaxNFitPoint[i]
                                                        , fnNFitPoint[i]);
        pave->AddText(hname);
      }
      if(fFlagNMaxPoint[i])
      {     
        sprintf(hname,"SetNMaxPoint(%d,%d,%d) : %d \n",i, fMinNMaxPoint[i]
                                                        , fMaxNMaxPoint[i]
                                                        , fnNMaxPoint[i]);
        pave->AddText(hname);
      }  
      if(fFlagNPointToNMaxPoint[i])
      {     
        sprintf(hname,"SetNPointToNMaxPoint(%d,%f) : %d \n",i, fNPointToNMaxPoint[i]
                                                             , fnNPointToNMaxPoint[i]);
        pave->AddText(hname);
      }  

   }
   if(fFlagPchi2)
   {     
     sprintf(hname,"SetPchi2(%f,%f) : %d \n", fMinPchi2, fMaxPchi2,fnPchi2);
     pave->AddText(hname);
   }
   if(fFlagP)
   {     
     sprintf(hname,"SetP(%f,%f) : %d \n", fMinP, fMaxP,fnP);
     pave->AddText(hname);
   }
   if(fFlagPt)
   {     
     sprintf(hname,"SetPt(%f,%f) : %d \n", fMinPt, fMaxPt,fnPt);
     pave->AddText(hname);
   }
   if(fFlagPx)
   {     
     sprintf(hname,"SetPx(%f,%f) : %d \n", fMinPx, fMaxPx,fnPx);
     pave->AddText(hname);
   }
   if(fFlagPy)
   {     
     sprintf(hname,"SetPy(%f,%f) : %d \n", fMinPy, fMaxPy,fnPy);
     pave->AddText(hname);
   }
    if(fFlagPz)
   {     
     sprintf(hname,"SetPz(%f,%f) : %d \n", fMinPz, fMaxPz,fnPz);
     pave->AddText(hname);
   }
   if(fFlagXFirst)
   {     
     sprintf(hname,"SetXFirst(%f,%f) : %d \n", fMinXFirst, fMaxXFirst,fnXFirst);
     pave->AddText(hname);
   }
   if(fFlagXLast)
   {     
     sprintf(hname,"SetXLast(%f,%f) : %d \n", fMinXLast, fMaxXLast,fnXLast);
     pave->AddText(hname);
   }
   if(fFlagYFirst)
   {     
     sprintf(hname,"SetYFirst(%f,%f) : %d \n", fMinYFirst, fMaxYFirst,fnYFirst);
     pave->AddText(hname);
   }
   if(fFlagYLast)
   {     
     sprintf(hname,"SetYLast(%f,%f) : %d \n", fMinYLast, fMaxYLast,fnYLast);
     pave->AddText(hname);
   }
   if(fFlagZFirst)
   {     
     sprintf(hname,"SetZFirst(%f,%f) : %d \n", fMinZFirst, fMaxZFirst,fnZFirst);
     pave->AddText(hname);
   }
   if(fFlagZLast)
   {     
     sprintf(hname,"SetZLast(%f,%f) : %d \n", fMinZLast, fMaxZLast,fnZLast);
     pave->AddText(hname);
   }
   for( Int_t i = 0;i<4;i++)
   {
      if(fFlagNDedxPoint[i])
      {     
        sprintf(hname,"SetNDedxPoint[%d] (%d,%d) : %d \n",i, fMinNDedxPoint[i]
                                                           , fMaxNDedxPoint[i]
                                                           , fnNDedxPoint[i]);
        pave->AddText(hname);
      }
      if(fFlagTmeanCharge[i])
      {     
        sprintf(hname,"SetTmeanCharge[%d] (%d,%d) : %d \n",i, fMinTmeanCharge[i]
                                                            , fMaxTmeanCharge[i]
                                                            , fnTmeanCharge[i]);
        pave->AddText(hname);
      }
   }
   if(fFlagRap)
   {     
     sprintf(hname,"SetRap(%d,%f,%f) : %d \n", fRapParticleId,fMinRap, fMaxRap,fnRap);
     pave->AddText(hname);
   }


   // T O F 
   if(fFlagTofIflag)
   {     
     sprintf(hname,"SetTofIflag(%d,%d) : %d \n", fMinTofIflag,fMaxTofIflag,fnTofIflag);
     pave->AddText(hname);
   }
   if(fFlagTofMass2)
   {     
     sprintf(hname,"SetTofMass2(%f,%f) : %d \n", fMinTofMass2, fMaxTofMass2,fnTofMass2);
     pave->AddText(hname);
   }
   if(fFlagTofSigMass2)
   {     
     sprintf(hname,"SetTofSigMass2(%f,%f) : %d \n", fMinTofSigMass2
                                                  , fMaxTofSigMass2
                                                  , fnTofSigMass2);
     pave->AddText(hname);
   }
   if(fFlagTofPathl)
   {     
     sprintf(hname,"SetTofPathl(%f,%f) : %d \n", fMinTofPathl, fMaxTofPathl,fnTofPathl);
     pave->AddText(hname);
   }
   if(fFlagTofX)
   {     
     sprintf(hname,"SetTofX(%f,%f) : %d \n", fMinTofX, fMaxTofX,fnTofX);
     pave->AddText(hname);
   }
   if(fFlagTofY)
   {     
     sprintf(hname,"SetTofY(%f,%f) : %d \n", fMinTofY, fMaxTofY,fnTofY);
     pave->AddText(hname);
   }
   if(fFlagTofCharge)
   {     
     sprintf(hname,"SetTofCharge(%f,%f) : %d \n", fMinTofCharge, fMaxTofCharge,fnTofCharge);
     pave->AddText(hname);
   }
   if(fFlagTofCutPoly)
   {     
     sprintf(hname,"SetTofCutPoly() : %d \n", fnTofCutPoly);
     pave->AddText(hname);
   }

   ///dedx
   if(fFlagDedx)
   {     
     sprintf(hname,"SetDedx(%f,%f) : %d \n", fMinDedx, fMaxDedx,fnDedx);
     pave->AddText(hname);
   }
 
   ///dedx sigma
   if(fFlagDedxSigma)
   {     
     sprintf(hname,"SetDedxSigma(%d,%d,%d,%f,%f) : %d \n",fDataTyp,fParticleId
                                                         ,fDedxTpc,fMinDedxSigma
                                                         ,fMaxDedxSigma,fnDedxSigma);
     pave->AddText(hname);
   }
   if (fFlagDedxVarSigma) {     
     sprintf(hname,"SetDedxVarSigma(%d,%d,%d,%f,%f) : %d \n",fDataTyp,fParticleId
                                                            ,fDedxTpc,fMinDedxSigma
                                                            ,fMaxDedxSigma,fnDedxSigma);
     pave->AddText(hname);
   }

   //probability
    if(fFlagProtonProbability)
   {     
     sprintf(hname,"SetProtonProbability(%f) : %d \n", fMinProtonProbability,fnProtonProbability);
     pave->AddText(hname);
   }
   if(fFlagKaonProbability)
   {     
     sprintf(hname,"SetKaonProbability(%f) : %d \n", fMinKaonProbability,fnKaonProbability);
     pave->AddText(hname);
   }
  if(fFlagPionProbability)
   {     
     sprintf(hname,"SetPionProbability(%f) : %d \n", fMinPionProbability,fnPionProbability);
     pave->AddText(hname);
   }
  if(fFlagElectronProbability)
   {     
     sprintf(hname,"SetElectronProbability(%f) : %d \n", fMinElectronProbability
                                                       , fnElectronProbability);
     pave->AddText(hname);
   }

   return pave;

}

//______________________________________________________________________________
void  T49CutTrack::PrintCutList()
{
  //
  // Prints the defined cuts and the cut-statistic
  //

  GetCutList()->Print();

}

//______________________________________________________________________________
void T49CutTrack::ReadTofCut(Char_t fname[256])
{
  //
  // Reads the TOF cut from a file
  //

  FILE  *fp;
  Int_t i;
  Int_t n;
 
  Char_t pname[256];
  Int_t ncut;
  Int_t npoly;
  Float_t XMass2;
  Float_t YDedx;
  
  Float_t XArray[40];
  Float_t YArray[40];
  if (fTofCutPoly) delete fTofCutPoly;
  fTofCutPoly = new TObjArray(); 

  //fTofCutPoly->Clear();

  fp = fopen(fname,"r");

  if(fp==NULL) 
  {
     fprintf(stderr,"Error: open cutfile %s\n",fname);
  }
  
  fscanf(fp,"%d",&npoly);
  printf("npoly %d \n",npoly);
  for(i=0;i<npoly;i++)
  {   
    fscanf(fp,"%s",pname);
    printf("cutname %s \n",pname);

    fscanf(fp,"%d",&ncut);

    for(n=0;n<ncut;n++)
    {  
      fscanf(fp,"%f",&XMass2);
      XArray[n]=XMass2;

      fscanf(fp,"%f",&YDedx);
      YArray[n]=YDedx;
      
      if(fVerbose)
         printf( "mass:%f dedx:%f \n",XMass2,YDedx);
    } 
    TCutG *cut = new TCutG(pname,ncut,XArray,YArray);
    fTofCutPoly->Add(cut);
  }
 
}
  
//______________________________________________________________________________
Double_t T49CutTrack::GetRap(Double_t mass, T49ParticleRoot *track)
{ 
  //
  // Calculate the rapidity of a particleq
  //

  Double_t e;
  Double_t pz;

  e  = sqrt(track->GetP()*track->GetP() + mass*mass);
  pz = track->GetPz(); 

  return 0.5 * log( (e+pz) / (e-pz) );                    
}

//______________________________________________________________________________
void T49CutTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class T49CutTrack.

   if (R__b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
      if (R__v > 1) {
        T49CutTrack::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
        return;
      }
      T49Cut::Streamer(R__b);
      R__b >> fnumtrack;
      R__b >> fnumgoodtrack;
      R__b >> fFlagProbability;
      R__b >> fTofCutPoly;
      R__b >> fFlagIflag;
      R__b >> fnIflag;
      R__b >> fIflag;
      R__b >> fFlagCharge;
      R__b >> fnCharge;
      R__b >> fCharge;
      R__b >> fFlagBx;
      R__b >> fnBx;
      R__b >> fMinBx;
      R__b >> fMaxBx;
      R__b >> fFlagBy;
      R__b >> fnBy;
      R__b >> fMinBy;
      R__b >> fMaxBy;
      R__b >> fFlagBxByEllipse;
      R__b >> fnBxByEllipse;
      R__b >> fBxEllipse;
      R__b >> fByEllipse;
      R__b >> fFlagIdDet;
      R__b >> fnIdDet;
      R__b >> fIdDet;
      R__b >> fFlagIdDetList;
      R__b >> fnIdDetList;
      fIdDetList.Streamer(R__b);
      R__b.ReadStaticArray(fFlagNPoint);
      R__b.ReadStaticArray(fnNPoint);
      R__b.ReadStaticArray(fMinNPoint);
      R__b.ReadStaticArray(fMaxNPoint);
      R__b.ReadStaticArray(fFlagNFitPoint);
      R__b.ReadStaticArray(fnNFitPoint);
      R__b.ReadStaticArray(fMinNFitPoint);
      R__b.ReadStaticArray(fMaxNFitPoint);
      R__b.ReadStaticArray(fFlagNMaxPoint);
      R__b.ReadStaticArray(fnNMaxPoint);
      R__b.ReadStaticArray(fMinNMaxPoint);
      R__b.ReadStaticArray(fMaxNMaxPoint);
      R__b.ReadStaticArray(fFlagNPointToNMaxPoint);
      R__b.ReadStaticArray(fnNPointToNMaxPoint);
      R__b.ReadStaticArray(fNPointToNMaxPoint);
      R__b >> fFlagPchi2;
      R__b >> fnPchi2;
      R__b >> fMinPchi2;
      R__b >> fMaxPchi2;
      R__b >> fFlagP;
      R__b >> fnP;
      R__b >> fMinP;
      R__b >> fMaxP;
      R__b >> fFlagPt;
      R__b >> fnPt;
      R__b >> fMinPt;
      R__b >> fMaxPt;
      R__b >> fFlagPx;
      R__b >> fnPx;
      R__b >> fMinPx;
      R__b >> fMaxPx;
      R__b >> fFlagPy;
      R__b >> fnPy;
      R__b >> fMinPy;
      R__b >> fMaxPy;
      R__b >> fFlagPz;
      R__b >> fnPz;
      R__b >> fMinPz;
      R__b >> fMaxPz;
      R__b >> fFlagXFirst;
      R__b >> fnXFirst;
      R__b >> fMinXFirst;
      R__b >> fMaxXFirst;
      R__b >> fFlagXLast;
      R__b >> fnXLast;
      R__b >> fMinXLast;
      R__b >> fMaxXLast;
      R__b >> fFlagYFirst;
      R__b >> fnYFirst;
      R__b >> fMinYFirst;
      R__b >> fMaxYFirst;
      R__b >> fFlagYLast;
      R__b >> fnYLast;
      R__b >> fMinYLast;
      R__b >> fMaxYLast;
      R__b >> fFlagZFirst;
      R__b >> fnZFirst;
      R__b >> fMinZFirst;
      R__b >> fMaxZFirst;
      R__b >> fFlagZLast;
      R__b >> fnZLast;
      R__b >> fMinZLast;
      R__b >> fMaxZLast;
      R__b.ReadStaticArray(fFlagNDedxPoint);
      R__b.ReadStaticArray(fnNDedxPoint);
      R__b.ReadStaticArray(fMinNDedxPoint);
      R__b.ReadStaticArray(fMaxNDedxPoint);
      R__b.ReadStaticArray(fFlagTmeanCharge);
      R__b.ReadStaticArray(fnTmeanCharge);
      R__b.ReadStaticArray(fMinTmeanCharge);
      R__b.ReadStaticArray(fMaxTmeanCharge);
      R__b >> fFlagRap;
      R__b >> fnRap;
      R__b >> fRapParticleId;
      R__b >> fMinRap;
      R__b >> fMaxRap;
      R__b >> fFlagTofIflag;
      R__b >> fnTofIflag;
      R__b >> fMinTofIflag;
      R__b >> fMaxTofIflag;
      R__b >> fFlagTofId;
      R__b >> fnTofId;
      R__b >> fTofId;
      R__b >> fFlagTofMass2;
      R__b >> fnTofMass2;
      R__b >> fMinTofMass2;
      R__b >> fMaxTofMass2;
      R__b >> fFlagTofSigMass2;
      R__b >> fnTofSigMass2;
      R__b >> fMinTofSigMass2;
      R__b >> fMaxTofSigMass2;
      R__b >> fFlagTofPathl;
      R__b >> fnTofPathl;
      R__b >> fMinTofPathl;
      R__b >> fMaxTofPathl;
      R__b >> fFlagTofX;
      R__b >> fnTofX;
      R__b >> fMinTofX;
      R__b >> fMaxTofX;
      R__b >> fFlagTofY;
      R__b >> fnTofY;
      R__b >> fMinTofY;
      R__b >> fMaxTofY;
      R__b >> fFlagTofCharge;
      R__b >> fnTofCharge;
      R__b >> fMinTofCharge;
      R__b >> fMaxTofCharge;
      R__b >> fFlagTofCutPoly;
      R__b >> fnTofCutPoly;
      R__b >> fFlagDedx;
      R__b >> fnDedx;
      R__b >> fMinDedx;
      R__b >> fMaxDedx;
      R__b >> fDedxTPC;
      R__b >> fDataTyp;
      R__b >> fParticleId;
      R__b >> fDedxTpc;
      R__b >> fFlagDedxSigma;
      R__b >> fnDedxSigma;
      R__b >> fMinDedxSigma;
      R__b >> fMaxDedxSigma;
      R__b >> fFlagDedxVarSigma;
      R__b >> fFunDedxSigma;
      R__b >> fFlagProtonProbability;
      R__b >> fnProtonProbability;
      R__b >> fMinProtonProbability;
      R__b >> fFlagKaonProbability;
      R__b >> fnKaonProbability;
      R__b >> fMinKaonProbability;
      R__b >> fFlagPionProbability;
      R__b >> fnPionProbability;
      R__b >> fMinPionProbability;
      R__b >> fFlagElectronProbability;
      R__b >> fnElectronProbability;
      R__b >> fMinElectronProbability;
      R__b >> fp;
      R__b >> fdedx;
      R__b >> fmean;
   } else {
      T49CutTrack::Class()->WriteBuffer(R__b,this);
//        R__b.WriteVersion(T49CutTrack::IsA());
//        T49Cut::Streamer(R__b);
//        R__b << fnumtrack;
//        R__b << fnumgoodtrack;
//        R__b << fFlagProbability;
//        R__b << fTofCutPoly;
//        R__b << fFlagIflag;
//        R__b << fnIflag;
//        R__b << fIflag;
//        R__b << fFlagCharge;
//        R__b << fnCharge;
//        R__b << fCharge;
//        R__b << fFlagBx;
//        R__b << fnBx;
//        R__b << fMinBx;
//        R__b << fMaxBx;
//        R__b << fFlagBy;
//        R__b << fnBy;
//        R__b << fMinBy;
//        R__b << fMaxBy;
//        R__b << fFlagBxByEllipse;
//        R__b << fnBxByEllipse;
//        R__b << fBxEllipse;
//        R__b << fByEllipse;
//        R__b << fFlagIdDet;
//        R__b << fnIdDet;
//        R__b << fIdDet;
//        R__b << fFlagIdDetList;
//        R__b << fnIdDetList;
//        fIdDetList.Streamer(R__b);
//        R__b.WriteArray(fFlagNPoint, 4);
//        R__b.WriteArray(fnNPoint, 4);
//        R__b.WriteArray(fMinNPoint, 4);
//        R__b.WriteArray(fMaxNPoint, 4);
//        R__b.WriteArray(fFlagNFitPoint, 4);
//        R__b.WriteArray(fnNFitPoint, 4);
//        R__b.WriteArray(fMinNFitPoint, 4);
//        R__b.WriteArray(fMaxNFitPoint, 4);
//        R__b.WriteArray(fFlagNMaxPoint, 4);
//        R__b.WriteArray(fnNMaxPoint, 4);
//        R__b.WriteArray(fMinNMaxPoint, 4);
//        R__b.WriteArray(fMaxNMaxPoint, 4);
//        R__b.WriteArray(fFlagNPointToNMaxPoint, 4);
//        R__b.WriteArray(fnNPointToNMaxPoint, 4);
//        R__b.WriteArray(fNPointToNMaxPoint, 4);
//        R__b << fFlagPchi2;
//        R__b << fnPchi2;
//        R__b << fMinPchi2;
//        R__b << fMaxPchi2;
//        R__b << fFlagP;
//        R__b << fnP;
//        R__b << fMinP;
//        R__b << fMaxP;
//        R__b << fFlagPt;
//        R__b << fnPt;
//        R__b << fMinPt;
//        R__b << fMaxPt;
//        R__b << fFlagPx;
//        R__b << fnPx;
//        R__b << fMinPx;
//        R__b << fMaxPx;
//        R__b << fFlagPy;
//        R__b << fnPy;
//        R__b << fMinPy;
//        R__b << fMaxPy;
//        R__b << fFlagPz;
//        R__b << fnPz;
//        R__b << fMinPz;
//        R__b << fMaxPz;
//        R__b << fFlagXFirst;
//        R__b << fnXFirst;
//        R__b << fMinXFirst;
//        R__b << fMaxXFirst;
//        R__b << fFlagXLast;
//        R__b << fnXLast;
//        R__b << fMinXLast;
//        R__b << fMaxXLast;
//        R__b << fFlagYFirst;
//        R__b << fnYFirst;
//        R__b << fMinYFirst;
//        R__b << fMaxYFirst;
//        R__b << fFlagYLast;
//        R__b << fnYLast;
//        R__b << fMinYLast;
//        R__b << fMaxYLast;
//        R__b << fFlagZFirst;
//        R__b << fnZFirst;
//        R__b << fMinZFirst;
//        R__b << fMaxZFirst;
//        R__b << fFlagZLast;
//        R__b << fnZLast;
//        R__b << fMinZLast;
//        R__b << fMaxZLast;
//        R__b.WriteArray(fFlagNDedxPoint, 4);
//        R__b.WriteArray(fnNDedxPoint, 4);
//        R__b.WriteArray(fMinNDedxPoint, 4);
//        R__b.WriteArray(fMaxNDedxPoint, 4);
//        R__b.WriteArray(fFlagTmeanCharge, 4);
//        R__b.WriteArray(fnTmeanCharge, 4);
//        R__b.WriteArray(fMinTmeanCharge, 4);
//        R__b.WriteArray(fMaxTmeanCharge, 4);
//        R__b << fFlagRap;
//        R__b << fnRap;
//        R__b << fRapParticleId;
//        R__b << fMinRap;
//        R__b << fMaxRap;
//        R__b << fFlagTofIflag;
//        R__b << fnTofIflag;
//        R__b << fMinTofIflag;
//        R__b << fMaxTofIflag;
//        R__b << fFlagTofId;
//        R__b << fnTofId;
//        R__b << fTofId;
//        R__b << fFlagTofMass2;
//        R__b << fnTofMass2;
//        R__b << fMinTofMass2;
//        R__b << fMaxTofMass2;
//        R__b << fFlagTofSigMass2;
//        R__b << fnTofSigMass2;
//        R__b << fMinTofSigMass2;
//        R__b << fMaxTofSigMass2;
//        R__b << fFlagTofPathl;
//        R__b << fnTofPathl;
//        R__b << fMinTofPathl;
//        R__b << fMaxTofPathl;
//        R__b << fFlagTofX;
//        R__b << fnTofX;
//        R__b << fMinTofX;
//        R__b << fMaxTofX;
//        R__b << fFlagTofY;
//        R__b << fnTofY;
//        R__b << fMinTofY;
//        R__b << fMaxTofY;
//        R__b << fFlagTofCharge;
//        R__b << fnTofCharge;
//        R__b << fMinTofCharge;
//        R__b << fMaxTofCharge;
//        R__b << fFlagTofCutPoly;
//        R__b << fnTofCutPoly;
//        R__b << fFlagDedx;
//        R__b << fnDedx;
//        R__b << fMinDedx;
//        R__b << fMaxDedx;
//        R__b << fDedxTPC;
//        R__b << fDataTyp;
//        R__b << fParticleId;
//        R__b << fDedxTpc;
//        R__b << fFlagDedxSigma;
//        R__b << fnDedxSigma;
//        R__b << fMinDedxSigma;
//        R__b << fMaxDedxSigma;
//        R__b << fFlagDedxVarSigma;
//        R__b << fFunDedxSigma;
//        R__b << fFlagProtonProbability;
//        R__b << fnProtonProbability;
//        R__b << fMinProtonProbability;
//        R__b << fFlagKaonProbability;
//        R__b << fnKaonProbability;
//        R__b << fMinKaonProbability;
//        R__b << fFlagPionProbability;
//        R__b << fnPionProbability;
//        R__b << fMinPionProbability;
//        R__b << fFlagElectronProbability;
//        R__b << fnElectronProbability;
//        R__b << fMinElectronProbability;
//        R__b << fp;
//        R__b << fdedx;
//        R__b << fmean;
   }
}










