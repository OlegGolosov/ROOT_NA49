#include <cmath>
#include "T49DedxFunction.h"
#include "TAxis.h"
#include "TMatrix.h"
#include "TH1.h"
#include "TMinuit.h"
#include "TVector.h"
/*
$Log: T49DedxFunction.C,v $
Revision 1.6  2008/01/04 10:48:58  cblume
64bit modifications by Marek Szuba

Revision 1.5  2005/02/08 10:22:20  flierl
TVector.h included

Revision 1.4  2000/07/07 14:12:11  cblume
Made ROOT v2.25 compatibel by Marco

 * Revision 1.3  2000/04/13  15:21:09  cblume
 * Bug fixes from Marco
 *
 * Revision 1.2  2000/01/30  14:44:12  cblume
 * New versions from Marco with updated comments
 *
 * Revision 1.1  2000/01/17  12:38:08  cblume
 * Add T49DedxFunction.C
 *
*/

//____________________________________________________________________
//  dE/dx fit function class
//  
//  This is an interface to a TF1, used to fit a dE/dx spectra.
//  The function consists of four gaussians, with identical relative
//  widths. Different methods are provide to fix or release the
//  positions of the different peaks. Internally, the pion peak position
//  is absolute, whereas the other peak positions are relative
//  to it.

ClassImp(T49DedxFunction)

T49DedxFunction::T49DedxFunction(Char_t* name): 
     TF1(name,
     	"[6]*exp(-0.5*(x-[2])*(x-[2])/([2]*[2]*[0]))+[5]*exp(-0.5*(x-[1]*[2])*(x-[1]*[2])/([1]*[2]*[1]*[2]*[0]))+[7]*exp(-0.5*(x-[3]*[2])*(x-[3]*[2])/([3]*[2]*[3]*[2]*[0]))+[8]*exp(-0.5*(x-[4]*[2])*(x-[4]*[2])/([4]*[2]*[4]*[2]*[0]))")
{
  SetParNames("SqRelRes","ElRelPos","PiPos","KaRelPos","PRelPos","ElAmp","PiAmp","KaAmp","PAmp");
  SetParLimits(1,1,-1);
  SetParLimits(3,1,-1);
  SetParLimits(4,1,-1);

  fAmpFix[0]=0;
  fAmpFix[1]=0;
  fAmpFix[2]=0;
  fAmpFix[3]=0;

  fPtot = -1;

  // Parameters for Bethe Bloch-parametrization     
  fParaC =  1.597546;// 1.5624;
  fParaD =  9.8;
  fParaE =  2.38;
  fParaF =  0.2;
  
  fStored=0;

}

void T49DedxFunction::SetFree()
{
  //  Release bounds on relative positions (N.B. the pion position 
  //  is not affected)

  if (!fAmpFix[0] || GetAmplitude(0) > 0) SetParLimits(1,0,0);
  if (!fAmpFix[2] || GetAmplitude(2) > 0) SetParLimits(3,0,0);
  if (!fAmpFix[3] || GetAmplitude(3) > 0) SetParLimits(4,0,0);
}

void T49DedxFunction::SetFreePosition(Int_t i,Float_t x)
{
  //  Release bound on peak position i, and set position to x.

  SetPosition(i,x);
  SetParLimits(1+i,0,0);
}

void T49DedxFunction::SetFixedPosition(Int_t i,Float_t x)
{
//  Set fixed value for peak position i

  SetPosition(i,x);
  SetParLimits(1+i,1,-1);
}

void T49DedxFunction::SetFreePosition(Int_t i)
{
  //  Release bounds on position of peak i

  if (!fAmpFix[i] || GetAmplitude(i)>0)
    SetParLimits(1+i,0,0);
}

void T49DedxFunction::SetFixedPosition(Int_t i)
{
  //  Fix position of peak i

  SetParLimits(1+i,1,-1);
}

void T49DedxFunction::ResetFreePosition(Int_t i)
{
  //  Set position to Bethe Bloch value, and 
 
  if (i==1)
    SetParameter(2,GetRelRise(1));
  else
    SetParameter(1+i,GetRelRise(i)/GetRelRise(1));
  if (!fAmpFix[i] || GetAmplitude(i)>0)    
    SetParLimits(1+i,0,0);
}


void T49DedxFunction::SetPosition(Int_t i, Float_t x, Float_t lowlim, Float_t uplim)
{
  //  Set position and bounds. (N.B.: the bounds of non-pion peaks 
  //  change with the pion peak position, since they are relative positions)

  if (i==1) {
    for (Int_t p=0;p<4;p++) {
      if (p!=1) SetParameter(1+p,GetPosition(p)/x);
    }
    SetParameter(2,x);
    if (!fAmpFix[i] || GetAmplitude(i)>0)
      SetParLimits(2,lowlim,uplim);
  }
  else if (GetParameter(2)!=0) {
    SetParameter(1+i,x/GetParameter(2));
    if (!fAmpFix[i] || GetAmplitude(i)>0)
      SetParLimits(1+i,lowlim/GetParameter(2),uplim/GetParameter(2));
  }
  else printf("First set Pion Position!! \n");
}

void T49DedxFunction::InitPositions(Float_t pipos)
{
  //  Sets pion position to pipos, and relative positions of other peaks 
  //  acoording to Bethe-Bloch parametrization

  SetPosition(1,pipos);
  for (Int_t i=0;i<4;i++) if (i!=1) SetParameter(1+i,GetRelRise(i)/GetRelRise(1));
  SetParLimits(1,1,-1);
  SetParLimits(3,1,-1);
  SetParLimits(4,1,-1);
}

void T49DedxFunction::SetAmplitude(Int_t i, Float_t x,Float_t lowlim, Float_t uplim)
{
  SetParameter(5+i,x);
  SetParLimits(5+i,lowlim,uplim);
}

void T49DedxFunction::SetAmplitude(Int_t i, Float_t x)
{
  SetParameter(5+i,x);
}

void T49DedxFunction::SetFixedAmplitude(Int_t i, Float_t x)
{
  //  Facility to fix the amplitude. Mainly used to fix amplitudes at zero.

  SetParameter(5+i,x);
  SetParLimits(5+i,1,-1);
  fAmpFix[i]=1;
  if (x==0) SetFixedPosition(i);
}

void T49DedxFunction::SetFixedAmplitude(Int_t i)
{
  SetParLimits(5+i,1,-1);
  fAmpFix[i]=1;
}

void T49DedxFunction::SetResolution(Float_t x)
{
  SetParameter(0,x*x);
}

Float_t T49DedxFunction::GetPosition(Int_t i)
{
  if (i==1) 
    return GetParameter(2);
  else
    return GetParameter(1+i)*GetParameter(2);
}

Float_t T49DedxFunction::GetRelPosition(Int_t i)
{
  if (i!=1) return GetParameter(1+i);
  printf("This is probably a mistake, Relative Pion Position == 1\n");
  return 1;
}

Float_t T49DedxFunction::GetAmplitude(Int_t i)
{
  return GetParameter(5+i);
}

Float_t T49DedxFunction::GetResolution()
{
  return sqrt(GetParameter(0));
}

void T49DedxFunction::GetRatio(Int_t part, Float_t &val, Float_t &err)
{
  Float_t sum=0, err_sq=0, p_cnt=0, err_cnt_sq=0;
  Float_t Der[9];
  Der[0]=0;
  for (Int_t p=0; p<4; p++) {
    sum+=GetResolution()*GetPosition(p)*GetAmplitude(p);
    Der[0]+=GetPosition(p)*GetAmplitude(p);
    Der[1+2*p]=GetResolution()*GetPosition(p);
    Der[2+2*p]=GetResolution()*GetAmplitude(p);
  }
  for (Int_t i=0; i<9; i++)
    for (Int_t j=0; j<9; j++)
      err_sq+=Der[i]*fCovMat[i][j]*Der[j];

  p_cnt=GetResolution()*GetPosition(part)*GetAmplitude(part);
  for (Int_t i=0; i<9; i++) 
    Der[i]=0;

  Der[0]=GetPosition(part)*GetAmplitude(part);
  Der[1+2*part]=GetResolution()*GetPosition(part);
  Der[2+2*part]=GetResolution()*GetAmplitude(part);
  for (Int_t i=0; i<9; i++)
    for (Int_t j=0; j<9; j++)
      err_cnt_sq+=Der[i]*fCovMat[i][j]*Der[j];

  val=p_cnt/sum;
  err=sqrt(err_sq/sum/sum+err_cnt_sq/p_cnt/p_cnt);

}

void T49DedxFunction::GetRatio2(Int_t part, Float_t &val, Float_t &err)
{
  Float_t denom=0;
  Float_t num=0;
  Float_t Der[9];
  num=GetPosition(part)*GetAmplitude(part);  
  for (Int_t p=0; p<4; p++) 
    denom+=GetPosition(p)*GetAmplitude(p);
  val=num/denom;

  Float_t denomsq=denom*denom;
  Der[0]=0;
  for (Int_t p=0; p<4; p++) 
    if (p==part) {
      Der[1+2*p]=GetPosition(p)/denom-num/denomsq*GetPosition(p);
      Der[2+2*p]=GetAmplitude(p)/denom-num/denomsq*GetAmplitude(p);
    }
    else {
      Der[1+2*p]=-num/denomsq*GetPosition(p);
      Der[2+2*p]=-num/denomsq*GetAmplitude(p);
    }

  err=0;
  for (Int_t i=1; i<9; i++)
    for (Int_t j=1; j<9; j++)
      err+=Der[i]*fCovMat[i][j]*Der[j];
  err=sqrt(err);
}

void T49DedxFunction::StoreCovMat()
{
  Double_t *pCovMat=(Double_t *) fCovMatMinuit;
  gMinuit->mnemat(pCovMat,9);
  if (gMinuit->GetNumFixedPars()!=0) {
    Double_t val,err;
    // Remap matrix, taking care of fixed parameters
    for (Int_t i=8;i>=0;i--) {
      Int_t k=gMinuit->GetParameter(i,val,err);
      for (Int_t j=8;j>=0;j--) {
	Int_t l=gMinuit->GetParameter(j,val,err);
	if (k==0||l==0) 
	  fCovMatMinuit[i][j]=0;
	else
	  fCovMatMinuit[i][j]=fCovMatMinuit[k-1][l-1];
      }
    }
  }
  // Project onto new parameters: res, elamp,elpos,piamp,pipos etc...
  Float_t DerMat[9][9];
  for (Int_t i=0;i<9;i++)
    for (Int_t j=0;j<9;j++)
      DerMat[i][j]=0;
  //res
  DerMat[0][0]=0.5/sqrt(GetParameter(0));
  //elamp and pos
  DerMat[1][5]=1.;
  DerMat[2][1]=GetParameter(2);
  DerMat[2][2]=GetParameter(1);
  //piamp and pos
  DerMat[3][6]=1.;
  DerMat[4][2]=1.;
  //kaamp and pos
  DerMat[5][7]=1.;
  DerMat[6][3]=GetParameter(2);
  DerMat[6][2]=GetParameter(3);
  //pramp and pos
  DerMat[7][8]=1.;
  DerMat[8][4]=GetParameter(2);
  DerMat[8][2]=GetParameter(4);
  for (Int_t i=0;i<9;i++)
    for (Int_t j=0;j<9;j++) {
      fCovMat[i][j]=0;
      for (Int_t k=0;k<9;k++)
	for (Int_t l=0;l<9;l++) 
	  fCovMat[i][j]+=DerMat[i][k]*fCovMatMinuit[k][l]*DerMat[j][l];
    }
}

void T49DedxFunction::StoreParameters()
{
  //  The class may hold one alternative set of parameters. This method 
  //  stores the parameters internally.

  GetParameters(fStoredPars);
  for (Int_t i=0; i<9; i++) 
    GetParLimits(i,fStoredLowLim[i],fStoredUpLim[i]);
  fStored=1;
}

void T49DedxFunction::RestoreParameters()
{
  //  Sets the parameters to the values stored using StoreParameters().

  if (fStored) {
    SetParameters(fStoredPars);
    for (Int_t i=0; i<9; i++) { 
      SetParLimits(i,fStoredLowLim[i],fStoredUpLim[i]);
      if (i>5) {
	if (fStoredLowLim[i]>fStoredUpLim[i]) 
	fAmpFix[i-5]=1;
      else
	fAmpFix[i-5]=0;
      }
    }
  }
  else printf("No parameters stored for this function.\n");
}

Float_t T49DedxFunction::GetRelRise(Int_t part)
{
  //  Returns the value of dE/dx from the Bethe-Bloch-parametrization. First 
  //  call SetPtot()

  if (fPtot>0)
    return RelRise(fPtot,part);
  else {
    printf("First call SetPtot()!!! \n");
    return 0;
  }
}

void T49DedxFunction::Init(TH1F* Hist,Char_t* Opt)
{
//  This method uses the histogram to provide an estimate for the peak
//  positions, amplitudes and widths. Relies heavily on the Bethe-Bloch
//  parametrization, so first call SetPtot(). Option "V" triggers
//  verbose behaviour.

  Float_t dedx[4];
  Float_t pos[4];
  Bool_t Verbose=0;
  if (strspn("V",Opt)) Verbose=1;
  
  dedx[0] = GetRelRise(0);
  dedx[1] = GetRelRise(1);
  dedx[2] = GetRelRise(2);
  dedx[3] = GetRelRise(3);

  pos[0] = dedx[0]/dedx[1];
  pos[1] = dedx[1]/dedx[1];  
  pos[2] = dedx[2]/dedx[1];
  pos[3] = dedx[3]/dedx[1];

// Set up peak search window.

  TAxis *Xax=Hist->GetXaxis();
  Int_t Bin = (Xax->FindBin(dedx[1])+Xax->FindBin(dedx[3]))/2;
  const Int_t Window=35;
  Float_t Smooth[2*Window+1];
  Int_t LeftBin = Bin-Window;
  Int_t RightBin = Bin+Window;
  if (LeftBin<1) LeftBin=1;
  if (RightBin>Xax->GetNbins()) RightBin = Xax->GetNbins();

// Smooth data

  if (Verbose) printf("Smoothing histogram, bin %d to %d, X= %f to %f \n",
                      Bin-Window,Bin+Window,Hist->GetBinCenter(Bin-Window),Hist->GetBinCenter(Bin+Window));
  for (Int_t i = Bin-Window;i <= Bin+Window;i++)
     if (i<1 || i>Xax->GetNbins()) 
        Smooth[i-Bin+Window]=0;
     else
        Smooth[i-Bin+Window]=(Hist->GetBinContent(i-1)+Hist->GetBinContent(i)+Hist->GetBinContent(i+1))/3;

// Find maxima
  
  // Maximum number of peaks is quite high, since some histos are ragged
  const Int_t MaxPeakCount = 30;
  Int_t PeakIndex[MaxPeakCount];
  Float_t Peaks[MaxPeakCount];
  Int_t PeakQual[MaxPeakCount];
  Int_t PeakCount=0;
  if (Verbose) printf("Searching for maximum, trying: \n");
  for (Int_t i=1; i < 2*Window; i++) {
    if ((Smooth[i]>Smooth[i-1] && Smooth[i]>=Smooth[i+1]) ||
	(Smooth[i]>=Smooth[i-1] && Smooth[i]>Smooth[i+1])) {
      PeakCount++;
      if (PeakCount-1<MaxPeakCount) {
	PeakIndex[PeakCount-1] = i;
	Peaks[PeakCount-1] = Smooth[i];
	if ((i>1 && i<2*Window-1) &&
	    (Smooth[i]>=Smooth[i-1] && Smooth[i]>Smooth[i-2]) &&
	    (Smooth[i]>=Smooth[i+1] && Smooth[i]>Smooth[i+2])) {
	  PeakQual[PeakCount-1] = 2; 
	} else PeakQual[PeakCount-1] = 1;

	if (PeakCount > 1 && i-PeakIndex[PeakCount-2] < 5) { 
	  //if peak very close to former peak
	  Int_t l = PeakIndex[PeakCount-2];
	  if ((l>1 && i<2*Window-1) && 
	      (Smooth[l]>Smooth[l-1] && Smooth[l]>Smooth[l-2]) &&
	      (Smooth[i]>Smooth[i+1] && Smooth[i]>Smooth[i+2])) {
	    //and peaks seem to be part of one peak, try to merge peaks
	    PeakQual[PeakCount-2]=2;
	    PeakQual[PeakCount-1]=2;
	  } 
	}

	if (Verbose) printf("Maximum #%d found, at bin %d, smoothed value: %f, Quality: %d\n",PeakCount,i+Bin-Window,Smooth[i],PeakQual[PeakCount-1]);
      }
      else printf("WARNING: too many maxima found in T49DedxFunction::Init. \n");
    }
  }

  // Initialize xvals, yvals, etc for PolyFit:

  Float_t xvals[2*Window+1];
  Float_t yvals[2*Window+1];
  Float_t errs[2*Window+1];
  Int_t Idx;
  for (Int_t i=LeftBin;i<=RightBin;i++) {
    Idx = i-Bin+Window;
    xvals[Idx] = Xax->GetBinCenter(i);
    yvals[Idx] = Hist->GetBinContent(i);
    errs[Idx] = sqrt(yvals[Idx]); 
  }
  Float_t pars[3];

  Int_t Quality=2;
  const Int_t MaxFits=10;
  Int_t NFits=0;
  Int_t LeftBound[MaxFits], RightBound[MaxFits];
  Float_t Pos[MaxFits], SigSq[MaxFits], Amp[MaxFits];  

  while (Quality>0 && NFits==0) {
    for (Int_t i=0; i<PeakCount; i++) {
      if (PeakQual[i] == Quality && Smooth[PeakIndex[i]]>7) {
	Bool_t InRange=0;
	for (Int_t j=0; j<NFits; j++) 
	  if (PeakIndex[i]>LeftBound[j] && PeakIndex[i]<RightBound[j]) InRange=1;
	if (!InRange && NFits < MaxFits) {
	  LeftBound[NFits]=0;
	  RightBound[NFits]=0;
	  // Find region > 0.7*Max, or stop where the smoothed function starts to
	  // rise again...
	  if (Verbose) printf("Probing left boundary... Trying bin ");
	  for (Int_t k = PeakIndex[i];k>1;k--) {
	    if (Verbose) printf("%d, ",k+Bin-Window);
	    if ((Smooth[k]<0.7*Peaks[i])||
		((Smooth[k-1]>Smooth[k]) && (Smooth[k-2]>Smooth[k-1]))) {
	      LeftBound[NFits]=k;
	      if (Verbose) printf("found!! \n");
	      break;
	    }
	  }
	  if (Verbose) printf("Probing right boundary... Trying bin ");
	  for (Int_t k = PeakIndex[i];k<2*Window-1;k++) {
	    if (Verbose) printf("%d, ",k+Bin-Window);
	    if ((Smooth[k]<0.7*Peaks[i])||
		((Smooth[k+1]>Smooth[k]) && (Smooth[k+2]>Smooth[k+1]))) {
	      RightBound[NFits]=k;
	      if (Verbose) printf("found!! \n");
	      break;
	    }
	  }
	  if (LeftBound[NFits]==0) {
	    printf("WARNING: Peak search goes out of window, T49DedxFunction::Init(), letfb \n");
	  }
	  if (RightBound[NFits]==0) {
	    printf("WARNING: Peak search goes out of window, T49DedxFunction::Init(), rigthb \n");
	    RightBound[NFits]=2*Window;
	  }
	  if (Verbose) printf("Boundaries set at: left %d, %f; right %d, %f \n",LeftBound[NFits]+Bin-Window,xvals[LeftBound[NFits]],RightBound[NFits]+Bin-Window,xvals[RightBound[NFits]]);
	  if (RightBound[NFits]-LeftBound[NFits]>3) {
	    PolyFit(RightBound[NFits]-LeftBound[NFits]+1,&xvals[LeftBound[NFits]],&yvals[LeftBound[NFits]],&errs[LeftBound[NFits]],2,pars);
	    //  PolyFit(rightb-leftb+1,&xvals[Idx],&yvals[Idx],2,pars);
	    Pos[NFits] = -pars[1]/2/pars[2];
	    SigSq[NFits] = -0.5/pars[2]*(pars[0]-0.25*pars[1]*pars[1]/pars[2]);
	    Amp[NFits] = -2*pars[2]*SigSq[NFits];
	    if (Verbose) printf("Fit # %d, Pos: %f, Width: %f, Amp: %f, ",NFits+1,Pos[NFits],SigSq[NFits],Amp[NFits]);
	    if (SigSq[NFits]>0 &&
		Pos[NFits]>xvals[LeftBound[NFits]] && 
		Pos[NFits]<xvals[RightBound[NFits]]) {
	      NFits++;
	      if (Verbose) printf("kept. \n");
	    }
	    else if (Verbose) printf("discarded. \n");
	    if (Verbose && NFits==MaxFits) printf("Maximum number of fits (%d) reached, no more peak will be fitted\n",MaxFits);
	  }
	}
      }
    }
    Quality--;
    if (NFits==0) printf("WARNING: Poor peak quality in T49DedxFunction::Init. \n");
  }
  Float_t PiPos,PiAmp,Res,PrAmp;
  Float_t KaAmp = 0;
  if (NFits==0) {
    printf("WARNING: No peaks found in T49DedxFunction::Init. \n");
      PiPos=dedx[1];
      PiAmp=100;
      PrAmp=10;
      Res=0.001;
  }
  else {
    Int_t IdxPi=0;
    Float_t DistPi=fabs(Pos[0]-dedx[1]);
    Int_t IdxPr=-1;
    Float_t DistPr=fabs(Pos[0]-dedx[3])+0.1;
    Int_t IdxKa=-1;
    Float_t DistKa=fabs(Pos[0]-dedx[2])+0.1;
    for (Int_t i=0; i<NFits; i++) {
      if (fabs(Pos[i]-dedx[1])<DistPi) {
	DistPi=fabs(Pos[i]-dedx[1]);
	IdxPi=i;
      } 
      if (fabs(Pos[i]-dedx[3]) < DistPr && 
	  fabs(Pos[i]-dedx[1]) > fabs(Pos[i]-dedx[3])) {
	DistPr=fabs(Pos[i]-dedx[3]);
	IdxPr=i;
      }
      if (fabs(Pos[i]-dedx[2])<DistKa &&
	  fabs(Pos[i]-dedx[1]) > fabs(Pos[i]-dedx[2])) {
	DistKa=fabs(Pos[i]-dedx[2]);
	IdxKa=i;
      }
    }
    if (Verbose) printf("Proton peak: fit# %d, Kaon peak: fit# %d, Pion peak: fit# %d \n",IdxPr+1,IdxKa+1,IdxPi+1);
    if (Verbose) printf("Amp[IdxPi]= %f, Amp[IdxPr]=%f \n",
			IdxPi>=0?Amp[IdxPi]:0,IdxPr>=0?Amp[IdxPr]:0);
    // Initialise Pion peak
    PiPos=Pos[IdxPi];
    PiAmp=Amp[IdxPi];
    Res=SigSq[IdxPi];

    // Possibly override, due to large proton peak 
    // (also valid if largest peak is proton peak)

    if (IdxPr >= 0 && Amp[IdxPi]<=Amp[IdxPr]) {
      if (DistKa < DistPr) {
	PiPos=Pos[IdxKa]/pos[2];
	KaAmp=Amp[IdxKa];
	Res=SigSq[IdxKa]/pos[2]/pos[2];
	PrAmp=0.05*KaAmp;
      }
      else {
	PiPos=Pos[IdxPr]/pos[3];
	PrAmp=Amp[IdxPr];
	Res=SigSq[IdxPr]/pos[3]/pos[3];
	KaAmp=0.05*PrAmp;
      }
      if (IdxPr==IdxPi) 
	PiAmp=0.05*PrAmp;
      else { 
	PiAmp=Amp[IdxPi]-PrAmp*exp(-0.5*(pos[3]-1)*PiPos*(pos[3]-1)*PiPos/Res)-
	  KaAmp*exp(-0.5*(pos[2]-1)*PiPos*(pos[2]-1)*PiPos/Res);
	if (PiAmp<0)
	  PiAmp=0.05*PrAmp;
      }
    }
    else {
      //if (IdxKa==IdxPi || ( DistKa > fabs(PiPos-dedx[2]) && DistPr > fabs(PiPos-dedx[3]))) {
      //printf("WARNING: Second peak not reliable in T49DedxFunction::Init.\n");
      // If no second peak, ready
      if (IdxKa < 0 && IdxPr < 0) {
	PrAmp=0.05*PiAmp;
	KaAmp=0.05*PiAmp;
      }
      else {
	// Else, recalculate proton/kaon peak
	// Start addition 991215
	//if (Verbose) printf("Subtracting pion peak, new points: \n");
	for (Int_t i=0; i<2*Window+1; i++) {
	  yvals[i]=yvals[i]-PiAmp*exp(-0.5*(xvals[i]-PiPos)*(xvals[i]-PiPos)/Res);
	  //if (Verbose) printf("%f, %f \n",xvals[i],yvals[i]);
	}
	// refit kaon
	if (IdxKa >= 0) {
	  PolyFit(RightBound[IdxKa]-LeftBound[IdxKa]+1,&xvals[LeftBound[IdxKa]],&yvals[LeftBound[IdxKa]],&errs[LeftBound[IdxKa]],2,pars);
	  Pos[IdxKa] = -pars[1]/2/pars[2];
	  SigSq[IdxKa] = -0.5/pars[2]*(pars[0]-0.25*pars[1]*pars[1]/pars[2]);
	  Amp[IdxKa] = -2*pars[2]*SigSq[IdxKa];
	  if (Verbose) printf("Refitted kaon (#%d), Pos: %f, Width: %f, Amp: %f\n",IdxKa+1,Pos[IdxKa],SigSq[IdxKa],Amp[IdxKa]);
	  if (SigSq[IdxKa]<0 ||
	      Pos[IdxKa]<xvals[LeftBound[IdxKa]] || 
	      Pos[IdxKa]>xvals[RightBound[IdxKa]]) 
	    printf("WARNING in T49DedxFunction::Init: error for refitted kaon peak. \n");
	}
	// refit proton
	if (IdxPr >=0 && IdxPr != IdxKa) {
	  PolyFit(RightBound[IdxPr]-LeftBound[IdxPr]+1,&xvals[LeftBound[IdxPr]],&yvals[LeftBound[IdxPr]],&errs[LeftBound[IdxPr]],2,pars);
	  Pos[IdxPr] = -pars[1]/2/pars[2];
	  SigSq[IdxPr] = -0.5/pars[2]*(pars[0]-0.25*pars[1]*pars[1]/pars[2]);
	  Amp[IdxPr] = -2*pars[2]*SigSq[IdxPr];
	  if (Verbose) printf("Refitted proton (#%d), Pos: %f, Width: %f, Amp: %f\n",IdxPr+1,Pos[IdxPr],SigSq[IdxPr],Amp[IdxPr]);
	  if (SigSq[IdxPr]<0 ||
	      Pos[IdxPr]<xvals[LeftBound[IdxPr]] || 
	      Pos[IdxPr]>xvals[RightBound[IdxPr]]) 
	    printf("WARNING in T49DedxFunction::Init: error for refitted proton peak. \n");
	}
	if (IdxPr == 0 ||
	    fabs(Pos[IdxKa]-pos[2]*PiPos) < fabs(Pos[IdxKa]-pos[3]*PiPos)) {
	  KaAmp=Amp[IdxKa];
	  //-PiAmp*exp(-0.5*(pos[2]-1)*PiPos*(pos[2]-1)*PiPos/Res);
	  PrAmp=0.01*PiAmp;
	}
	else {
	  PrAmp=Amp[IdxPr];
	  //-PiAmp*exp(-0.5*(pos[3]-1)*PiPos*(pos[3]-1)*PiPos/Res);
	  KaAmp=0.01*PiAmp;
	}
      }
    }
  }
  if (sqrt(Res)>(1-pos[3]) && (PrAmp>0.5*PiAmp && PrAmp<1.5*PiAmp)) {
    // For severely overlapping peaks:
    Res*=0.5;
    KaAmp/=2;
    PrAmp/=2;
    PiAmp/=2;
  }
  Float_t MaxAmp=PiAmp;
  if (KaAmp>MaxAmp) MaxAmp=KaAmp;
  if (PrAmp>MaxAmp) MaxAmp=PrAmp;
  InitPositions(PiPos);
  SetResolution(sqrt(Res)/PiPos);
  SetAmplitude(0,0.05*MaxAmp,0,5*MaxAmp);
  SetAmplitude(1,PiAmp,0,5*MaxAmp);
  SetAmplitude(2,KaAmp,0,5*MaxAmp);
  SetAmplitude(3,PrAmp,0,5*MaxAmp);
  SetRange(0.5,2.0);
}

Float_t T49DedxFunction::RelRise(Float_t Ptot, Int_t Part)
{
//  Returns expected dE/dx values, according to Bethe-Bloch parametrization
//  (Called by GetRelRise())
//  ** Code provided by Christof Roland **
 
  Float_t bg, dedx;
  Float_t c, d, e, f, x1,x2, p0, dfq, d1, d2, d3;
  Double_t mass[4] = {MASS_E_2, MASS_PI_2, MASS_K_2, MASS_P_2};
  // char *names [4] = {"elec","Pions", "Kaons", "Protons"};
  
  bg = Ptot / sqrt (mass[Part]);
  
  c =  fParaC;
  d =  fParaD;
  e =  fParaE;
  f =  fParaF;

  x1 = pow(10,(e - 1.0/3.0 * sqrt(2.0*log(10.0)/(3.0 * f))));
  x2 = pow(10,(e + 2.0/3.0 * sqrt(2.0*log(10.0)/(3.0 * f))));
  
  p0 = c/(d + 2.0*log(10.0)*e - 1.0);
  
  if (bg<x1)
    dfq = 0;
  else
    {
      dfq = -2.0 * log(bg) + 2.0 * log(10.0) * e;
      if(bg<x2)
	{
	  d1 = 2.0/3.0*sqrt(2.0*log(10.0)/(3.0 * f));
	  d2 = log(bg)/log(10.0);
	  d3 = pow(( e + d1 - d2),3);
	  dfq -= f*d3;
	}
    }
  
  dedx = p0*( (1+ bg*bg)/(bg*bg) * ( d + log(1+(bg*bg)) + dfq)-1.0 );
  
  return dedx;
}

void T49DedxFunction::PolyFit(const Int_t NPoints, Float_t* x, Float_t* y, Float_t* err, Int_t Order, Float_t* Result)
{
//  Fits polynomial of form par[0] + par[1]*x +par[2]*x^2, etc
//  Uses the provided errors as weights.
//  (Called internally by Init())

   Float_t u[NPoints];
   Float_t v[NPoints];

   Float_t MinX=x[0];
   Float_t MaxX=x[0];
   Float_t MinY=y[0];
   Float_t MaxY=y[0];
   for (Int_t i=0; i<NPoints; i++) {
      if (x[i]>MaxX) MaxX = x[i];
      if (x[i]<MinX) MinX = x[i];
      if (y[i]>MaxY) MaxY = y[i];
      if (y[i]<MinY) MinY = y[i];
   }
   Float_t XScale=1/(MaxX-MinX);
   Float_t YScale=1/(MaxY-MinY);
   for (Int_t i=0; i<NPoints; i++) {
      u[i]=XScale*(x[i]-MinX);
      v[i]=YScale*(y[i]-MinY);
   }

   TMatrix upow(NPoints,2*Order+1);
   for (Int_t i=0; i<=2*Order; i++)
     for (Int_t j=0; j<NPoints; j++){
       if (i==0) upow(j,i)=1/(err[j]*err[j]*YScale*YScale);
          else upow(j,i)=upow(j,i-1)*u[j]; }

   TVector B(Order+1);
   for (Int_t i=0; i<=Order; i++) {
     B(i)=0;
     for (Int_t j=0; j<NPoints; j++){
       B(i)+=upow(j,i)*v[j];
     }
   }
   TMatrix A(Order+1,Order+1);
   for (Int_t i=0; i<=Order; i++)
     for (Int_t j=0; j<=Order; j++) {
       if (j>=i) {
          A(i,j)=0;
          for (Int_t k=0; k<NPoints; k++) {
             A(i,j) += upow(k,j+i);
          }; 
       } else A(i,j)=A(j,i);
     }
   TMatrix Ainv(TMatrix::kInverted,A);
   B *= Ainv;
   Ainv *= A;
   if (Ainv(Order,1)>1e-4) printf("ERROR: numerical accuracy too low in T49DedxFunction::PolyFit\n");
   Float_t XScalePow=1.0;
   Float_t MinXPow=1.0;
   for (Int_t k=0;k<=Order;k++) {
      Result[k]=0;
      for (Int_t j=0;j<=Order;j++) {
        if (j==0)
           XScalePow=1.0;
        else XScalePow*=XScale;
        if (j>=k) {
          if (j-k==0) 
             MinXPow=1.0;
          else MinXPow*=-MinX;
          Result[k]+=B(j)*XScalePow*MinXPow*Binom(j,k);
        }
      }
      Result[k]=1/YScale*Result[k];
      if (k==0) Result[k]+=MinY;
   }
}

Int_t T49DedxFunction::Binom(Int_t n,Int_t k)
{
//  Calculates binomial coefficient n over k.
//  Called by PolyFit()

  if (n<k) {
    printf("ERROR in Binom: n<k, n=%d k=%d\n",n,k);
    return 0;
  }
  else {
    Int_t Num=1;
    for (Int_t i=n;i>k;i--) Num*=i;
    Int_t Denom=1;
    for (Int_t i=n-k;i>1;i--) Denom*=i;
    return Num/Denom;
  }
}

















