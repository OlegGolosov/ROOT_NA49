////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  File T49Proj2.C                                                           //
//  Version 2.0       R.Ganz  Jan.20  1999                                    //
//                                                                            //
//  The class T49Proj2BASE provides functionality to generate two particle    //
//  projections such as Inv mass spectra, HBT correlation ..                  //
//  What type of projection is used, dependents on the derived class          //
//  which one uses:                                                           //
//  (eg T49Proj2BP for Bertsch Pratt type HBT Qside,Qout,Qlong 3-dimensional) //
//  (eg T49Proj2INVmass for invariant mass spectra 1-dimensional)             //
//  The T49Proj2BASE class allows for analyses eg in kt and y bins by         //
//  booking the arrording and managing the appropriate histogram arrays       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/*
$Log: T49Proj2.C,v $
Revision 1.12  2002/02/04 16:16:05  cblume
Fix bug in SetCMS()

Revision 1.11  2001/12/12 17:43:11  cblume
Caluclate k in the pair cm

Revision 1.10  2001/11/27 16:14:52  cblume
Change dimensions of the character arrays

Revision 1.9  2001/11/21 17:02:24  cblume
Add T49Proj2K

Revision 1.8  2001/11/12 12:12:14  cblume
Update for ROOT v3.01

 * Revision 1.7  2000/07/07  14:13:29  cblume
 * Made ROOT v2.25 compatible
 *
 * Revision 1.6  1999/12/01  09:39:27  cblume
 * Changed to root 2.23-09. Updated the makefiles. Changes in T49CutTrack, T49Dedx, T49Proj1, T49Proj2, TRootDS
 *
 * Revision 1.5  1999/11/24  16:03:50  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 *
 * Revision 1.4  1999/11/23  13:54:13  cblume
 * Remove files
 *
*/

#include "T49Proj2.h"

ClassImp(T49Proj2BP)
ClassImp(T49Proj2YKP)
ClassImp(T49Proj2QtQl)
ClassImp(T49Proj2QINV2)
ClassImp(T49Proj2QINV)
ClassImp(T49Proj2INVmass)
ClassImp(T49Proj2Armenteros)
ClassImp(T49Proj2K)
ClassImp(T49Proj2BASE)

//______________________________________________________________________________
T49Proj2BASE::T49Proj2BASE()
{
  //
  // Constructor creating default histogram array
  //

  fSysID      = 0;
  fBeta       = 0.0;
  fPart1Mass  = 0.0;
  fPart2Mass  = 0.0;
  fVerbose    = kFALSE;

  fNPairs     = 0;
  fQinvFlag   = 0;
  fMirrorFlag = 0;
  fDimArray   = 0;

  fQx         = 0.0;
  fQy         = 0.0;
  fQz         = 0.0;       

  fNXBin      = 1;
  fNYBin      = 1;
  fNZBin      = 1;      

  fXMin       = 0.0;
  fYMin       = 0.0;
  fZMin       = 0.0;

  fXMax       = 0.0;
  fYMax       = 0.0;
  fZMax       = 0.0;

  SetCMS(158.0);

  fBinStat     = NULL;
  fKtAxis      = NULL;   
  fAverKtHist  = NULL;
  fYpAxis      = NULL;   
  fAverYpHist  = NULL; 
  fYAxis       = NULL;   
  fAverYHist   = NULL;
  fPtAxis      = NULL;   
  fAverPtHist  = NULL;
  fPhiAxis     = NULL;   
  fAverPhiHist = NULL;
  fXFAxis      = NULL;   
  fAverXFHist  = NULL;
       
  fHist        = NULL;
  fXHist       = NULL;
  fYHist       = NULL;
  fZHist       = NULL;
  fAHist       = NULL;

}

//______________________________________________________________________________
T49Proj2BASE::~T49Proj2BASE()
{

  if (fKtAxis)  { delete fKtAxis;  };
  if (fYpAxis)  { delete fYpAxis;  };
  if (fYAxis)   { delete fYAxis;   };
  if (fPtAxis)  { delete fPtAxis;  };
  if (fPhiAxis) { delete fPhiAxis; };
  if (fXFAxis)  { delete fXFAxis;  };

  if (fHist) {
    fHist->Delete();
    delete fHist;
  }
  if (fXHist) {
    fXHist->Delete();
    delete fXHist;
  }
  if (fYHist) {
    fYHist->Delete();
    delete fYHist;
  }
  if (fZHist) {
    fZHist->Delete();
    delete fZHist;
  }
  if (fAHist) {
    fAHist->Delete();
    delete fAHist; 
  }
 
}

//______________________________________________________________________________
void T49Proj2BASE::SetCMS(Double_t pLab)
{
  //
  // Set the center of mass system
  //

  fBetaCMS       = pLab / (kNUCLMASS+sqrt(kNUCLMASS2+pLab*pLab));
  Double_t gamma = 1.0 / TMath::Sqrt(1.0 + fBetaCMS*fBetaCMS);
  fYcms          = TMath::Log(gamma+gamma * fBetaCMS);         
  fPprojCMS      = gamma * (pLab - fBetaCMS*TMath::Sqrt(pLab*pLab +kNUCLMASS2));                       

}
 
//______________________________________________________________________________
void T49Proj2BASE::SetParticleMass(Int_t a, Int_t m)
{ 
  //
  // Set the particle masses of the lists (a = 1,2) 
  // to m = 1 proton   
  //        2 kaon  
  //        3 pion  
  //        4 electron     
  //

  Float_t mass = 0.0;

  switch (TMath::Abs(m)) {
  case 1: 
    mass = kPMASS;  
    break;
  case 2: 
    mass = kKMASS;  
    break;
  case 3: 
    mass = kPIMASS; 
    break;
  case 4: 
    mass = kEMASS;  
    break;
  default: 
    printf(" <E> Don't know this particle %d\n",m);
    break;
  };

  SetParticleMass(a,mass);

}

//______________________________________________________________________________
void T49Proj2BASE::SetParticleMass(Int_t a, Float_t mass)
{ 
  //
  // Set the particle masses of the lists (a = 1,2) 
  //

  switch (a) {
  case 1: 
    fPart1Mass = mass; 
    break;
  case 2: 
    fPart2Mass = mass; 
    break;
  default: 
    printf(" <E> Don't know where to put this particle %d\n",a);
    break;
  };

}

//______________________________________________________________________________
void T49Proj2BASE::Book(char* nam, char* tit,
			Int_t nx,Float_t xmin,Float_t xmax, 
			Int_t ny,Float_t ymin,Float_t ymax,
			Int_t nz,Float_t zmin,Float_t zmax)		     
{
  //
  // Books one set of histogram or an array of sets of histogram
  // depending on whether SetKtArray(...) or/and SetYpArray(...) 
  // was activated BEFORE booking
  // It calls BookOne(..) for each set
  //

  Int_t nKt, nYp, nY, nPt, nPhi, nXF ,ID, OF;
  Int_t nKtB, nYpB, nYB, nPtB, nPhiB, nXFB;
  char  TAGname[120]     = "";
  char  TAGAxisName[120] = "";
  char  TAGtitl[120]     = "";
 
  // Further loop for more array types (but keep the order)
  nKtB  = (fKtAxis ? fKtAxis->GetNbins()  : 1);  
  nYpB  = (fYpAxis ? fYpAxis->GetNbins()  : 1);  
  nYB   = (fYAxis  ? fYAxis->GetNbins()   : 1);  
  nPtB  = (fPtAxis ? fPtAxis->GetNbins()  : 1);  
  nPhiB = (fPhiAxis? fPhiAxis->GetNbins() : 1);  
  nXFB  = (fXFAxis ? fXFAxis->GetNbins()  : 1);  
  
  nKt  = 0;
  nYp  = 0;
  nY   = 0;
  nPt  = 0;
  nPhi = 0;
  nXF  = 0;
  OF   = 0;

  fDimArray = nKtB*nYpB*nYB*nPtB*nPhiB*nXFB;

  for (ID = 0; ID < fDimArray; ID++) {

    OF = 1;

    if (fVerbose) {
      printf(" <I> Booking %d %d %d %d %d %d\n" ,nKt, nYp, nY, nPt, nPhi, nXF);
    }

    sprintf(TAGname,"%s%d",nam,ID);
    sprintf(TAGtitl," %s ",tit);

    if (fKtAxis) { 
      sprintf(TAGAxisName,"%sKtAxis",nam);
      fKtAxis->SetName(TAGAxisName);
      char ktChar[40] = "";
      sprintf(ktChar," %.2f < k_{t} < %.2f",fKtAxis->GetBinLowEdge(nKt+1)
                                           ,fKtAxis->GetBinLowEdge(nKt+2)); 
      strcat(TAGtitl,ktChar);
      if (OF == 1) {
	OF = 0;
	if (nKt < nKtB-1) {
          nKt++;
        }
	else {
          nKt = 0;
          OF  = 1;
        }
      }
    }

    if (fYpAxis) { 
      sprintf(TAGAxisName,"%sYpAxis",nam);
      fYpAxis->SetName(TAGAxisName);
      char ypChar[40] = "";
      sprintf(ypChar," %.1f < y < %.1f",fYpAxis->GetBinLowEdge(nYp+1)
                                       ,fYpAxis->GetBinLowEdge(nYp+2)); 
      strcat(TAGtitl,ypChar);
      if (OF == 1) {
	OF = 0;
	if (nYp < nYpB-1) {
          nYp++;
        }
	else {
           nYp = 0;
           OF  = 1;
        }
      }
    }
      
    if (fYAxis) { 
      sprintf(TAGAxisName,"%sYAxis",nam);
      fYAxis->SetName(TAGAxisName);
      char yChar[40] = "";
      sprintf(yChar," %.1f < y < %.1f",fYAxis->GetBinLowEdge(nY+1)
                                      ,fYAxis->GetBinLowEdge(nY+2)); 
      strcat(TAGtitl,yChar);
      if (OF == 1) {
	OF = 0;
	if (nY < nYB-1) { 
          nY++;
        }
	else {
          nY = 0;
          OF = 1;
        }
      }
    }

    if (fPtAxis) { 
      sprintf(TAGAxisName,"%sPtAxis",nam);
      fPtAxis->SetName(TAGAxisName);
      char PtChar[40] = "";
      sprintf(PtChar," %.2f < p_{t} < %.2f",fPtAxis->GetBinLowEdge(nPt+1)
                                           ,fPtAxis->GetBinLowEdge(nPt+2)); 
      strcat(TAGtitl,PtChar);
      if (OF == 1) {
	OF = 0;
	if (nPt < nPtB-1) {
          nPt++;
        }
	else {
          nPt = 0;
          OF  = 1;
        }
      }
    }

    if (fPhiAxis) { 
      sprintf(TAGAxisName,"%sPhiAxis",nam);
      fPhiAxis->SetName(TAGAxisName);
      char PhiChar[40] = "";
      sprintf(PhiChar," %.3f < #phi < %.3f",fPhiAxis->GetBinLowEdge(nPhi+1)
                                           ,fPhiAxis->GetBinLowEdge(nPhi+2)); 
      strcat(TAGtitl,PhiChar);
      if (OF == 1) {
	OF = 0;
	if (nPhi < nPhiB-1) {
          nPhi++;
        }
	else {
          nPhi = 0;
          OF   = 1;
        }
      }
    }

    if (fXFAxis) { 
      sprintf(TAGAxisName,"%sXFAxis",nam);
      fXFAxis->SetName(TAGAxisName);
      char XFChar[40] = "";
      sprintf(XFChar," %.3f < x_{F} < %.3f",fXFAxis->GetBinLowEdge(nXF+1)
                                           ,fXFAxis->GetBinLowEdge(nXF+2)); 
      strcat(TAGtitl,XFChar);
      if (OF == 1) {
	OF = 0;
	if (nXF < nXFB-1) {
          nXF++;
        }
	else {
          nXF = 0;
          OF  = 1;
        }
      }
    }

    BookOne(TAGname,TAGtitl,nx,xmin,xmax,ny,ymin,ymax,nz,zmin,zmax);

  }

  char AverName[40];     
  sprintf(AverName,"%sBinStat",nam);
  fBinStat = new TH1D(AverName,"Number of entries distribution"
                              ,fDimArray,-0.5,1.0*(fDimArray-0.5));

  if (fKtAxis) {
    sprintf(AverName,"%sAverKt",nam);
    fAverKtHist = new TProfile(AverName,"Average K_{t}",fDimArray
                                       ,-0.5,1.0*(fDimArray-0.5),0.0,5.0);
  }
  if (fYpAxis) {
    sprintf(AverName,"%sAverYp",nam);
    fAverYpHist = new TProfile(AverName,"Average y_{p}",fDimArray
                                       ,-0.5,1.0*(fDimArray-0.5),0.0,6.0);
  }
  if (fYAxis) {
    sprintf(AverName,"%sAverY",nam);
    fAverYHist  = new TProfile(AverName,"Average y",fDimArray
                                       ,-0.5,1.0*(fDimArray-0.5),0.0,6.0);
  }
  if (fPtAxis) {
    sprintf(AverName,"%sAverPt",nam);
    fAverPtHist = new TProfile(AverName,"Average p_{t}",fDimArray
                                       ,-0.5,1.0*(fDimArray-0.5),0.0,5.0);
  }
  if (fPhiAxis) {
    sprintf(AverName,"%sAverPhi",nam);
    fAverPhiHist= new TProfile(AverName,"Average #phi",fDimArray
                                       ,-0.5,1.0*(fDimArray-0.5),0.0,360.0);
  }
  if (fXFAxis) {
    sprintf(AverName,"%sAverXF",nam);
    fAverXFHist = new TProfile(AverName,"Average x_{F}",fDimArray
                                       ,-0.5,1.0*(fDimArray-0.5),0.0,1.5);
  }

}	  	  

//______________________________________________________________________________
void T49Proj2BASE::BookOne(char* nam, char* tit,
  			   Int_t nx,Float_t xmin,Float_t xmax, 
			   Int_t ny,Float_t ymin,Float_t ymax,
			   Int_t nz,Float_t zmin,Float_t zmax)		     
{
  //
  //  Books one set of histograms
  //  Each set contains dependent of the dimension of the projection:
  //    H  Histogram for the projected quantiti
  //    X  Histogram accumulating the X component of the projected quantity (eg Qside for BP HBT)
  //    Y  Histogram accumulating the Y component (only for 2-D and 3-D projections)
  //    Z  Histogram accumulating the Z component (only 3-D projections)
  //
  //    If fQinvFlag was set to 1 it also contains a histogram accumulating Qinv
  //
  //    Dividing eg. X by H results in the average X component in each histgram bin
  //    of the projection 
  //
  //  The input parameter <nam> and <tit> will be part of the title and the ROOT name  
  //
              
  fNXBin = nx; 
  fXMin  = xmin; 
  fXMax  = xmax;

  // Create the histogram arrays
  if (!fHist)  fHist  = new TObjArray();
  if (!fXHist) fXHist = new TObjArray();
  if (!fYHist) fYHist = new TObjArray();
  if (!fZHist) fZHist = new TObjArray();
  if (!fAHist) fAHist = new TObjArray();

  if (fDim > 1) { 
    fNYBin = ny; 
    fYMin  = ymin; 
    fYMax  = ymax; 
  } 
  else { 
    fNYBin = 1; 
    fYMin  = 0.0; 
    fYMax  = 0.0; 
  }

  if (fDim > 2) { 
    fNZBin = nz; 
    fZMin  = zmin; 
    fZMax  = zmax; 
  } 
  else { 
    fNZBin = 1; 
    fZMin  = 0.0; 
    fZMax  = 0.0; 
  }

  if (fDim > 3) { 
    printf(" <E> Error defining dimension of projection. \n");
  }

  char title[120] = "";
  char  name[120] = "";

  switch (fDim) {

  case 1:
 
    if (fVerbose) printf(" <I> 1D object %s   %s  \n",nam,fHistTitle); 
    sprintf(name,"H%s",nam);
    sprintf(title,"%s %s",fHistTitle,tit);
    fHist->Add(new TH1D(name,title,fNXBin,fXMin,fXMax));
    ((TH1D*) (fHist->At(fHist->GetLast())))->SetXTitle(fXLabel);
    ((TH1D*) (fHist->At(fHist->GetLast())))->Sumw2();	  
    sprintf(name,"X%s", nam);
    sprintf(title,"<%s> %s %s",fXLabel,fHistTitle,tit);
    fXHist->Add(new TH1D(name,title,fNXBin,fXMin,fXMax));
    if (fQinvFlag) {
      sprintf(name,"A%s",nam);
      sprintf(title,"<Q_{inv}> %s %s",fHistTitle,tit);
      fAHist->Add(new TH1D(name,title,fNXBin,fXMin,fXMax));
    }
    break;

  case 2: 

    if (fVerbose) printf(" <I> 2D object %s   %s  \n",nam,fHistTitle); 
    sprintf(name,"H%s",nam);
    sprintf(title,"%s %s",fHistTitle,tit);
    fHist->Add(new TH2D(name,title,fNXBin,fXMin,fXMax
                                  ,fNYBin,fYMin,fYMax));
    ((TH2D*) (fHist->At(fHist->GetLast())))->SetXTitle(fXLabel);
    ((TH2D*) (fHist->At(fHist->GetLast())))->SetYTitle(fYLabel);
    ((TH2D*) (fHist->At(fHist->GetLast())))->Sumw2();	  
    sprintf(name,"X%s",nam);
    sprintf(title,"<%s> %s %s",fXLabel,fHistTitle,tit);
    fXHist->Add(new TH2D(name,title,fNXBin,fXMin,fXMax
                                   ,fNYBin,fYMin,fYMax));
    sprintf(name,"Y%s",nam);
    sprintf(title,"<%s> %s %s",fYLabel,fHistTitle,tit);
    fYHist->Add(new TH2D(name,title,fNXBin,fXMin,fXMax
                                   ,fNYBin,fYMin,fYMax));
    if (fQinvFlag) {
      sprintf(name,"A%s",nam);
      sprintf(title,"<Q_{inv}> %s %s",fHistTitle,tit);
      fAHist->Add(new TH2D(name,title,fNXBin,fXMin,fXMax
                                     ,fNYBin,fYMin,fYMax));
    }
    break;

  case 3: 

    if (fVerbose) printf(" <I> 3D object %s   %s  \n",nam,fHistTitle);
    sprintf(name,"H%s",nam);
    sprintf(title,"%s %s",fHistTitle,tit);
    fHist->Add(new TH3D(name,title,fNXBin,fXMin,fXMax
                                  ,fNYBin,fYMin,fYMax
                                  ,fNZBin,fZMin,fZMax));
    ((TH3D*) (fHist->At(fHist->GetLast())))->SetXTitle(fXLabel);
    ((TH3D*) (fHist->At(fHist->GetLast())))->SetYTitle(fYLabel);
    ((TH3D*) (fHist->At(fHist->GetLast())))->SetZTitle(fZLabel);
    ((TH3D*) (fHist->At(fHist->GetLast())))->Sumw2();;
    sprintf(name,"X%s",nam);
    sprintf(title,"<%s> %s %s",fXLabel,fHistTitle,tit);
    fXHist->Add(new TH3D(name,title,fNXBin,fXMin,fXMax
                                   ,fNYBin,fYMin,fYMax
                                   ,fNZBin,fZMin,fZMax));
    sprintf(name,"Y%s",nam);
    sprintf(title,"<%s> %s %s",fYLabel,fHistTitle,tit);
    fYHist->Add(new TH3D(name,title,fNXBin,fXMin,fXMax
                                   ,fNYBin,fYMin,fYMax
                                   ,fNZBin,fZMin,fZMax));	  
    sprintf(name,"Z%s",nam);
    sprintf(title,"<%s> %s %s",fZLabel,fHistTitle,tit);
    fZHist->Add(new TH3D(name,title,fNXBin,fXMin,fXMax
                                   ,fNYBin,fYMin,fYMax
                                   ,fNZBin,fZMin,fZMax));	  
    if (fQinvFlag) {
      sprintf(name,"A%s",nam);
      sprintf(title,"<Q_{inv}> %s %s",fHistTitle,tit);
      fAHist->Add(new TH3D(name,title,fNXBin,fXMin,fXMax
                                     ,fNYBin,fYMin,fYMax
                                     ,fNZBin,fZMin,fZMax));
    }
    break;

  };

}

//______________________________________________________________________________
Int_t T49Proj2BASE::CheckMirror(T49ParticleRoot* t1,T49ParticleRoot* t2)
{
  //
  // Checks whether the pair is below or above cms-rapidity
  //  
 
  Double_t y = 0.25 * (TMath::Log((t1->GetE(fPart1Mass)+t1->GetPz())
                                 /(t1->GetE(fPart1Mass)-t1->GetPz()))
		     + TMath::Log((t2->GetE(fPart2Mass)+t2->GetPz())
			         /(t2->GetE(fPart2Mass)-t2->GetPz())));

  return ((y < fYcms) ? 1 : 0);

}

//______________________________________________________________________________
void T49Proj2BASE::Fill(TObjArray* ListA, TObjArray* ListB, T49Cut2Track* T2Cut)
{
  //
  // Filling the projection by combining each particle I
  // from list A with each particle J of list B
  // If the lists are identical only I<J pairs are combined
  // Careful ! If the lists are not identical
  // but contain the same particle double counting will appear !!
  //

  T49ParticleRoot *Part1;
  T49ParticleRoot *Part2;
  
  Int_t SameListFlag = 0;
  Int_t IP1 = 0;
  Int_t IP2 = 0;

  if (ListA == ListB) { 
    SameListFlag = 1; 
  }

  IP1 = 0;
  TIter NextParticle(ListA);
  while ((Part1 = ((T49ParticleRoot *) NextParticle()))) { 
    IP1++; 
    IP2 = 0;
    TIter NextP2(ListB);
    while ((Part2 = ((T49ParticleRoot *) NextP2()))) { 
      if ((SameListFlag == 0) && (Part1 == Part2)) {
        printf(" <E> Identical track in two different lists!\n");
        printf("     Double counting in projection!\n");
      }
      IP2++; 
      if ((SameListFlag == 0) || (IP1 > IP2)) {       
	if ((T2Cut == NULL) || (T2Cut->CheckPair(Part1,Part2))) {
	  if ((Part1->GetPx() == Part2->GetPx()) && 
              (Part1->GetPy() == Part2->GetPy()) && 
              (Part1->GetPz() == Part2->GetPz()) && 
              (SameListFlag   == 0)) {
            printf(" <E> Identical track in two different lists!\n");
            printf("     Double counting in projection!\n");
	  }
	  Fill(Part1,Part2);
	}
      }
    }
  }

}
  
//______________________________________________________________________________
void T49Proj2BASE::Fill(T49ParticleRoot* t1, T49ParticleRoot* t2)
{ 
  //
  // Fill are single pair into projection 
  // (at the proper index of the projection)
  //

  Double_t Qinv       = 0.0;
  Int_t    MirrorThis = 0; 
  Int_t    ID;
   
  if (fMirrorFlag == 1) {
    MirrorThis = CheckMirror(t1,t2);
  }

  // Get the Index for Hist arrays 
  ID = GetArrayIndex(t1,t2,MirrorThis);  

  if ((ID > -1) && (ID < fDimArray)) { 

    // fSysID == 2 is LCMS: beta derived from pair 
    if (fSysID == 2) { 
      fBeta = (Double_t) ((t1->GetPz()          + t2->GetPz())
	                 /(t1->GetE(fPart1Mass) + t2->GetE(fPart2Mass)));
      if (fVerbose == 2) printf(" <I> beta LCMS: %f \n",fBeta);
    }
      
    QCalc(t1,t2,MirrorThis);
      
    // Here for non id part a more general qinv expr. is needed
    if (fQinvFlag) { 
      Qinv = T49Tool::Qinv(t1,t2,fPart1Mass,fPart2Mass);
    }
      
    switch (fDim) {

    case 1:
      if ((fXMin < fQx  ) && 
          (fQx   < fXMax)) { 
        fNPairs++;
	((TH1D*) ( fHist->At(ID)))->Fill(fQx,1.0);
	((TH1D*) (fXHist->At(ID)))->Fill(fQx,fQx);	
	if (fQinvFlag) { 
          ((TH1D*) (fAHist->At(ID)))->Fill(fQx,Qinv); 
        }
      }
      break;

    case 2:
      if ((fXMin < fQx  ) && 
          (fQx   < fXMax) && 
          (fYMin < fQy  ) && 
          (fQy   < fYMax)) { 
        fNPairs++;
        ((TH2D*) ( fHist->At(ID)))->Fill(fQx,fQy,1.0);
        ((TH2D*) (fXHist->At(ID)))->Fill(fQx,fQy,fQx);
        ((TH2D*) (fYHist->At(ID)))->Fill(fQx,fQy,fQy);
        if (fQinvFlag) {
          ((TH2D*) (fAHist->At(ID)))->Fill(fQx,fQy,Qinv); 
        }
      }
      break;

    case 3:
      if ((fXMin < fQx  ) && 
          (fQx   < fXMax) && 
          (fYMin < fQy  ) && 
          (fQy   < fYMax) && 
          (fZMin < fQz  ) && 
          (fQz   < fZMax)) { 
        fNPairs++;
	((TH3D*) ( fHist->At(ID)))->Fill(fQx,fQy,fQz,1.0);
	((TH3D*) (fXHist->At(ID)))->Fill(fQx,fQy,fQz,fQx);
	((TH3D*) (fYHist->At(ID)))->Fill(fQx,fQy,fQz,fQy);
	((TH3D*) (fZHist->At(ID)))->Fill(fQx,fQy,fQz,fQz);
	if (fQinvFlag) {
          ((TH3D*) (fAHist->At(ID)))->Fill(fQx,fQy,fQz,Qinv); 
        }
      }
      break;

    default:
      printf(" <E> Fill - Some error with dimension occured. \n");
      break;

    };

  }

}

//______________________________________________________________________________
Int_t T49Proj2BASE::GetArrayIndex(T49ParticleRoot *t1,T49ParticleRoot *t2
                                 ,Int_t MirrorThis)
{
  //
  // Calculates the histogram index depending on kt or/and y of the pair
  // (also y(P), Pt(P), Phi(P)  with P=p1+p2 are supported)
  // depending on which have been activated (eg. via SetKtArray(...)
  // returns -1 if axis was defined but pairs gives value out of range  
  //

  Int_t   Bin     = 0;
  Int_t   InLimit = 1;

  Float_t Kt  = 0;
  Float_t Yp  = 0;
  Float_t Y   = 0;
  Float_t Pt  = 0;
  Float_t Phi = 0;
  Float_t XF  = 0;

  // Inverse order compared to booking -> slowest changing of index
  if (fXFAxis && InLimit) { 
    XF = (T49Tool::zLorentz(t1,fPart1Mass,fBetaCMS).Z()
        + T49Tool::zLorentz(t2,fPart2Mass,fBetaCMS).Z()) / fPprojCMS;
    if ((fXFAxis->GetXmin() < XF                ) && 
        (XF                 < fXFAxis->GetXmax())) { 
      Bin = Bin * fXFAxis->GetNbins() + (fXFAxis->FindBin(XF)-1); 
      if (fVerbose == 2) {
        printf(" <i>  GetArrayIndex :   %d XF=%f \n",Bin,XF);
      }
    }
    else {
      InLimit = 0; 
      return -1;
    }
  }

  if (fPhiAxis && InLimit) { 
    if ((t1->GetPx() + t2->GetPx()) == 0.0) {
      Phi = ((t1->GetPy() + t2->GetPy()) > 0.0 ? 90.0 :270.0) ;
    }
    else {
      Phi = TMath::ATan((t1->GetPy()+t2->GetPy())
 		      / (t1->GetPx()+t2->GetPx())) * 57.29578;     
      if ((t1->GetPx()+t2->GetPx()) < 0.0) { 
        Phi = Phi + 180.0;
      }
      if (Phi < 0.0) {
        Phi = Phi + 360.0;
      }
    }
    if ((fPhiAxis->GetXmin() < Phi                ) && 
        (Phi                 < fPhiAxis->GetXmax())) { 
      Bin = Bin * fPhiAxis->GetNbins() + (fPhiAxis->FindBin(Phi)-1); 
      if (fVerbose == 2) {
        printf(" <i>  GetArrayIndex :   %d Phi=%f \n",Bin,Phi);
      }
    }
    else {
      InLimit = 0;
      return -1;
    }
  }

  if (fPtAxis && InLimit) { 
    Pt = TMath::Sqrt(T49Tool::Px(t1,t2)*T49Tool::Px(t1,t2)
                   + T49Tool::Py(t1,t2)*T49Tool::Py(t1,t2));
    if ((fPtAxis->GetXmin() < Pt                ) && 
        (Pt                 < fPtAxis->GetXmax())) { 
      Bin = Bin * fPtAxis->GetNbins() + (fPtAxis->FindBin(Pt)-1); 
      if (fVerbose == 2) {
        printf(" <i>  GetArrayIndex :   %d Pt=%f \n",Bin,Pt);
      }
    }
    else {
      InLimit = 0;
    }
  }

  if (fYAxis && InLimit) { 
    Y = 0.5 * TMath::Log((t1->GetE(fPart1Mass)+t2->GetE(fPart2Mass)
                         +t1->GetPz()+t2->GetPz())
	  	        /(t1->GetE(fPart1Mass)+t2->GetE(fPart2Mass)
                         -t1->GetPz()-t2->GetPz()));
    if (MirrorThis) {
      Y = 2.0*fYcms - Y;
    }
    if ((fYAxis->GetXmin() < Y                ) && 
        (Y                 < fYAxis->GetXmax())) { 
      Bin = Bin * fYAxis->GetNbins() + (fYAxis->FindBin(Y)-1); 
      if (fVerbose == 2) {
        printf(" <i>  GetArrayIndex :   %d Y=%f \n",Bin,Y);
      }
    }
    else {
      InLimit = 0;
      return -1;
    }
  }

  if (fYpAxis && InLimit) {
    Yp = 0.25 * (TMath::Log((t1->GetE(fPart1Mass)+t1->GetPz()) 
                           /(t1->GetE(fPart1Mass)-t1->GetPz()))
	        +TMath::Log((t2->GetE(fPart2Mass)+t2->GetPz()) 
                           /(t2->GetE(fPart2Mass)-t2->GetPz())));
    if (MirrorThis) {
      Yp = 2.0*fYcms - Yp;
    }
    if ((fYpAxis->GetXmin() <                 Yp) && 
        (Yp                 < fYpAxis->GetXmax())) { 
      Bin = Bin * fYpAxis->GetNbins() + (fYpAxis->FindBin(Yp)-1); 
      if (fVerbose == 2) {
        printf(" <i>  GetArrayIndex :   %d yp=%f \n",Bin,Yp);
      }
    }
    else {
      InLimit = 0;
      return -1;
    }
  }

  if (fKtAxis && InLimit) { 
    Kt = 0.5*TMath::Sqrt(T49Tool::Px(t1,t2)*T49Tool::Px(t1,t2)
	 	        +T49Tool::Py(t1,t2)*T49Tool::Py(t1,t2));
    if ((fKtAxis->GetXmin() <                 Kt) && 
        (Kt                 < fKtAxis->GetXmax())) { 
      Bin = Bin * fKtAxis->GetNbins() + (fKtAxis->FindBin(Kt)-1); 
      if (fVerbose == 2) {
        printf(" <i>  GetArrayIndex :   %d kt=%f \n",Bin,Kt);
      }
    }
    else {
      InLimit = 0;
      return -1;
    }
  }

  if (Bin > (fDimArray - 1)) {
    printf(" <E> Found Array Index OUT OF LIMIT %d (%d)\n",Bin,fDimArray);
    Bin = -1;
  }

  // Here add further axis but keep reverse order as in Book() 
  if (InLimit == 0) {
    Bin = -1;
    return -1;
  }
  else {
    fBinStat->Fill((Double_t) Bin,1.0);
    if (fKtAxis)  { 
      fAverKtHist->Fill((Double_t) Bin,(Double_t)  Kt); 
    }
    if (fYpAxis)  { 
      fAverYpHist->Fill((Double_t) Bin,(Double_t)  Yp); 
    }
    if (fYAxis)   {  
      fAverYHist->Fill((Double_t) Bin,(Double_t)   Y); 
    } 
    if (fPtAxis)  { 
      fAverPtHist->Fill((Double_t) Bin,(Double_t)  Pt); 
    }
    if (fPhiAxis) {
      fAverPhiHist->Fill((Double_t) Bin,(Double_t) Phi); 
    }
    if (fXFAxis)  { 
      fAverXFHist->Fill((Double_t) Bin,(Double_t)  XF); 
   }
 }
 
 return Bin;

}

//______________________________________________________________________________
void T49Proj2BASE::WriteHist()
{ 
  //
  // Write histgrams to file
  // (which has to be open before using this function)
  //

  fHist->Write();
  fXHist->Write();	      
  if (fYHist) { 
    fYHist->Write(); 
  }
  if (fZHist) { 
    fZHist->Write(); 
  }
  if (fAHist) { 
    fAHist->Write(); 
  }
  fBinStat->Write();
  if (fKtAxis) { 
    fAverKtHist->Write();
    fKtAxis->Write();
  }
  if (fYpAxis) { 
    fAverYpHist->Write();
    fYpAxis->Write();
  }
  if (fYAxis) {  
    fAverYHist->Write();
    fYAxis->Write();
  } 
  if (fPtAxis) { 
    fAverPtHist->Write();
    fPtAxis->Write();
  }
  if (fPhiAxis) {
    fAverPhiHist->Write();
    fPhiAxis->Write();
  }
  if (fXFAxis) { 
    fAverXFHist->Write();
    fXFAxis->Write();
  }

}














