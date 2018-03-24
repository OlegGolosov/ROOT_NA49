//////////////////////////////////////////////////////////
//                                                      //
//  Cut on track pairs                                  //
//                                                      //
//////////////////////////////////////////////////////////

#include "T49Cut2Track.h"

ClassImp(T49Cut2Track)

/*
$Log: T49Cut2Track.C,v $
 * Revision 1.3  1999/11/23  13:54:00  cblume
 * Remove files
 *
*/

Float_t  T49Cut2Track::fPart1Mass =0.0;
Float_t  T49Cut2Track::fPart2Mass =0.0;

void T49Cut2Track::SetParticleMass(Int_t a, Int_t m)
{ 
  //////////////////////////////////////////////////////////////////////
  // Set the particle masses of the lists (a=1,2) 
  // to m =1  Proton   = 2 Kaon   = 3 Pion   = 4 Electron     
  //////////////////////////////////////////////////////////////////////
  Float_t Mass=0.0;
  switch (TMath::Abs(m))
    {
    case 1: Mass = kPMASS;  break;
    case 2: Mass = kKMASS;  break;
    case 3: Mass = kPIMASS; break;
    case 4: Mass = kEMASS;  break;
    default: printf(" <E> Don't know this particle\n");break;
    };
  switch (a)
    {
    case 1: fPart1Mass = Mass; break;
    case 2: fPart2Mass = Mass; break;
    default: printf(" <E> Don't know where to put this particle\n");break;
    };
}

T49Cut2Track::T49Cut2Track():T49Cut()
{
  //
  // Default constructor
  //

  Reset();

}

T49Cut2Track::T49Cut2Track(const Text_t *name, const Text_t *title)
             :T49Cut(name,title)
{
  //
  // Default constructor with name and title
  //

  Reset();

}

T49Cut2Track::~T49Cut2Track()
{
  
}

void T49Cut2Track::Reset()
{

    fFlagDist            = kFALSE;
    fFlagArmenterosAlpha = kFALSE;
    fFlagArmenterosPt    = kFALSE;
    fFlagPsiLCMS         = kFALSE;
    fFlagMinQ            = kFALSE;
    fFlagQinv            = kFALSE;
    fFlagMinv            = kFALSE;

    fnDist            = 0;
    fnArmenterosAlpha = 0;
    fnArmenterosPt    = 0;
    fnPsiLCMS         = 0;
    fnMinQ            = 0;
    fnQinv            = 0;
    fnMinv            = 0;

}

void T49Cut2Track::SetStandardCuts()
{

  //
  // Sets the standard cuts
  //    min. Distance > 2.0 cm
  //

  SetMinDist(2.0); 

}

Int_t T49Cut2Track::CheckPair(T49ParticleRoot *t1, T49ParticleRoot *t2)
{ 

  //
  // Checks whether a track pair passes all defined cuts
  //

   fnum2track++;

   Float_t Dist = T49Tool::TwoTrackDist(t1,t2);
   if(fFlagDist && (Dist < fMinDist) )
   {           
      if(fVerbose)
	 printf("Pair removed due to Distance %f < %f \n",Dist,fMinDist);
      fnDist++;
      return kFALSE;     	
   } 

   Double_t ArmenterosAlpha = T49Tool::ArmAlpha(t1,t2,fPart1Mass,fPart2Mass);
   if(fFlagArmenterosAlpha && (ArmenterosAlpha < fMinArmenterosAlpha || 
                               ArmenterosAlpha > fMaxArmenterosAlpha)  )
   {      
      if(fVerbose)
	  printf("Pair removed due to Armenteros Alpha Cut %f \n"
                ,ArmenterosAlpha);
      fnArmenterosAlpha++;
      return kFALSE;	 
   } 

   Double_t ArmenterosPt = T49Tool::ArmPt(t1,t2,fPart1Mass,fPart2Mass);
   if(fFlagArmenterosPt && (ArmenterosPt < fMinArmenterosPt || 
                            ArmenterosPt > fMaxArmenterosPt)  )
   {          	  
      if(fVerbose)
	 printf("Pair removed due to Armenteros Pt Cut %f \n",ArmenterosPt);
      fnArmenterosPt++;
      return kFALSE;	  
   }

   Double_t PSI = T49Tool::PsiLCMS(t1,t2,fPart1Mass,fPart2Mass);
   if (fFlagPsiLCMS && ( PSI < fMinPsiLCMS || PSI > fMaxPsiLCMS) )
   {
      if(fVerbose)
	 printf("Pair removed due to Psi LCMS Cut %f  \n",PSI);
      fnPsiLCMS++;    
      return kFALSE;	
   }

   Double_t MinQx = TMath::Abs(T49Tool::Qx(t1,t2));
   Double_t MinQy = TMath::Abs(T49Tool::Qy(t1,t2));
   Double_t MinQz = TMath::Abs(T49Tool::Qz(t1,t2));
   if (fFlagMinQ && ( MinQx < fMinQx) && ( MinQy < fMinQy) && ( MinQz < fMinQz) )
   {
      if(fVerbose)
	 printf("Pair removed due to MinQ Cut %f  %f  %f \n",MinQx,MinQy,MinQz);
      fnMinQ++;    
      return kFALSE;	
   }


   Double_t Qinv = T49Tool::Qinv(t1,t2,fPart1Mass,fPart2Mass);
   if (fFlagQinv && ( Qinv < fMinQinv || Qinv > fMaxQinv) )
   {
      if(fVerbose)
	 printf("Pair removed due to Q inv Cut %f  \n",Qinv);
      fnQinv++;    
      return kFALSE;	
   }

   Double_t Minv = T49Tool::Minv(t1,t2,fPart1Mass,fPart2Mass);
   if (fFlagMinv && ( Minv < fMinMinv || Minv > fMaxMinv) )
   {
      if(fVerbose)
	 printf("Pair removed due to  M inv Cut %f  \n",Minv);
      fnMinv++;    
      return kFALSE;	
   }

   fnumgood2track++;	
   return kTRUE; 
}

TString  *T49Cut2Track::GetCutList()
{
  //
  // Creates a TString with the defined cuts and the cut-statistics
  //

   TString *string  = new TString();

   char          hname[256]; 

   sprintf(hname,"total number of tracks : %d \n",fnum2track);  
   string->Append(hname);

   sprintf(hname,"number of tracks accepted : %d \n",fnumgood2track);  
   string->Append(hname);

   //if(fFlagIflag)
   //{     
   // sprintf(hname,"SetIflag(%d) : %d \n", fIflag,fnIflag);
   //  string->Append(hname);
   //  }  
   if(fFlagDist)
   {     
      sprintf(hname,"SetMinDist(%f) : %d \n", fMinDist,fnDist);
      string->Append(hname);
   }
   if(fFlagArmenterosAlpha)
   {     
      sprintf(hname," SetArmenterosAlpha(%f,%f): %d \n", fMinArmenterosAlpha
                                                       , fMaxArmenterosAlpha
                                                       , fnArmenterosAlpha);
      string->Append(hname);
   }
   if(fFlagArmenterosPt)
   {     
      sprintf(hname," SetArmenterosPt(%f,%f): %d \n", fMinArmenterosPt
                                                    , fMaxArmenterosPt
                                                    , fnArmenterosPt);
      string->Append(hname);
   }
   if(fFlagPsiLCMS)
   {     
      sprintf(hname," SetPsiLCMS(%f,%f): %d \n", fMinPsiLCMS, fMaxPsiLCMS, fnPsiLCMS);
      string->Append(hname);
   }
 
   if(fFlagMinQ)
   {     
      sprintf(hname," SetMinQ(%f,%f,%f): %d \n", fMinQx, fMinQy,fMinQz, fnMinQ);
      string->Append(hname);
   }
   if(fFlagQinv)
   {     
      sprintf(hname," SetQinv(%f,%f): %d \n", fMinQinv, fMaxQinv, fnQinv);
      string->Append(hname);
   }

   if(fFlagMinv)
   {     
      sprintf(hname," SetMinv(%f,%f): %d \n", fMinMinv, fMaxMinv, fnMinv);
      string->Append(hname);
   }
 
    //printf("cuts: %s \n",string->Data());
    return string;

}

void  T49Cut2Track::PrintCutList()
{
  //
  // Prints the defined cuts
  //

   printf("Trackcuts: \n %s \n",GetCutList()->Data());

}





