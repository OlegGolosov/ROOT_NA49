#include "T49InMuDST.h"

ClassImp(T49InMuDST)

/*
$Log: T49InMuDST.C,v $
Revision 1.3  2005/04/26 18:58:13  cblume
SLC3 and rootv4 compatible version

Revision 1.2  1999/11/23 13:54:07  cblume
Remove files

*/

//////////////////////////////////////////////////////////////

Int_t T49InMuDST::OpenFile(Text_t *fN)
{
  fName=fN;
  fTestEOF=OpenFile(); 
  return fTestEOF;
}

Int_t T49InMuDST::OpenFile()
{ 
  printf(" <I> Open muDST-file: %s \n",fName);
  fp_in = fopen(fName,"r");
  if (fp_in==NULL)
	{printf(" <E> Could NOT open muDST-file %s \n",fName);return 0;};
  return 1;
}

TObjArray* T49InMuDST::ReadEvt()
{
  char InStr[96];
  Int_t Num1,Num2,Num3,Num4,Num5,Num6,Num7,Num8;
  Float_t fPx,fPy,fPz,fCharge,fdEdx,fM2,fFlag,fSigmadEdx;
  Int_t fNumVTPCPoints,fNumMTPCPoints,fITrck,fNRawTrck;
  
  TrackList->Delete();
  
  if (fgets(InStr,96,fp_in)==NULL) 
    {
      printf(" <W> Error when reading event \n");return 0;
    }
  else
    { 
      fEvtCount++;
      if (fEvtCount==(1000*(fEvtCount/1000))) {printf(" <I> uDST Event %d processed!\n",fEvtCount);};

	  /// This for pp:        
      sscanf(InStr,"%d %d %d",&fNRun,&fNEvt,&fNRawTrck);   // pp uDST has no CMD
      fEvtType=0;

	  // p Pb   uDST only,  but comment out the line with fEvtType++	  
	  //   sscanf(InStr,"%d %d %d %d",&fNRun,&fNEvt,&fNRawTrck,&fEvtType);
	 
      fNRun=fNRun+1000; 
      
      if (fVerbose)
	{
	  printf ("Run %d  Event %d  Tracks: %d \n",fNRun,fNEvt,fNRawTrck);
	};
      
      fNTrck=0; 
      for (fITrck=0;fITrck<fNRawTrck;fITrck++)
	{ 
	  if(fgets(InStr,96,fp_in)==NULL)
	    {
	      printf(" <E> Expected %d tracks but found %d \n",fNTrck,fITrck);return 0;
	    }
	  else
	    { 
	      sscanf(InStr,"%d %d %d %d %d %d %d %d",&Num1,&Num2,&Num3,&Num4,&Num5,&Num6,&Num7,&Num8);
	      fPx        = 0.001 * Num1 ;
	      fPy        = 0.001 * Num2 ;
	      fPz        = 0.001 * Num3 ;
	      fCharge    = 1.0 * Num4/abs(Num4);
	      //fdEdx      = 0.001 * fCharge*Num4;
	      fdEdx      = fCharge*Num4;
	      fM2        = 0.001 * Num5 ;
	      fFlag      = 1.0 * Num6/abs(Num6);
	      fNumVTPCPoints = (int) (1.0 * fFlag*Num6);
	      fNumMTPCPoints = (int) (1.0 * fFlag*Num7);
	      //fSigmadEdx     = 0.001*Num8;
	      fSigmadEdx     = 1.0*Num8;
	      
		  // comment out for p Pb  
	      if (fFlag>0) {fEvtType++;};                    // Charge multiplicity
	      
	      if (fFlag>0.0)
		{
		  T49ParticleRoot *Part = new T49ParticleRoot();
		  Part->SetPx(fPx);
		  Part->SetPy(fPy);
		  Part->SetPz(fPz);
                  Int_t chargeI = ((Int_t) fCharge);
		  Part->SetCharge(chargeI); 
		  Part->SetNDedxPoint(fNumVTPCPoints, 1);
		  Part->SetNDedxPoint(fNumMTPCPoints, 2);
		  Part->SetNDedxPoint(fNumMTPCPoints+fNumVTPCPoints,3);
                  Part->SetTmeanCharge(fdEdx,3);
                  Part->SetTmeanCharge(fSigmadEdx,1);
		  
		  TrackList->Add(Part);
		  fNTrck++;
		};
	      if (fVerbose)
		{
		  printf("Track: %d %f %f %f %f %f %f %f %d %d\n",
			 fITrck,fPx,fPy,fPz,fCharge,fdEdx,fM2,fFlag,fNumVTPCPoints,fNumMTPCPoints);
		};
	    };	  
	};
      return TrackList;
    };
}

