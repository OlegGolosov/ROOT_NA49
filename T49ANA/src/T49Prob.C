//////////////////////////////////////////////////////
///////////   0 =  pion
///////////   1 =  kaon
///////////   2 =  proton
///////////   3 =  electron
/////////////////////////////////////////////////////
#include <cmath>
#include <cstdlib>

#include "T49Prob.h"
#include "T49EventRoot.h"
#include "T49ParticleRoot.h"
#include "T49Run.h"

#include "PhysConst.h"

//*-*-* ROOT includes

#include "TROOT.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TNtuple.h"
#include "TBranch.h"
#include "TObjArray.h"
#include "TObject.h"
#include "TArrayF.h" 
#include "TMatrix.h"

ClassImp(T49Prob)

/*
$Log: T49Prob.C,v $
Revision 1.6  2008/01/04 10:48:58  cblume
64bit modifications by Marek Szuba

Revision 1.5  2001/11/12 12:12:03  cblume
Update for ROOT v3.01

 * Revision 1.4  1999/11/23  13:54:10  cblume
 * Remove files
 *
*/

T49Prob::T49Prob()
{     
    fDedxMatrix       = new TMatrix(100,20);
    fDedxFitPosMatrix = new TMatrix(12,5);
    fDedxFitNegMatrix = new TMatrix(12,5); 
}

  
void T49Prob::ReadDedxPara(Char_t fname[256])
{  
  FILE  *fp; 
  Int_t i;
  Int_t ii;
  Int_t row;
  Int_t column;
  Double_t x;

  fp = fopen(fname,"r");

  if(fp==NULL) 
  {
     fprintf(stderr,"Error: open cutfile %s\n",fname);
  }
  printf("read matrix: %s ",fname);
  fscanf(fp,"%d",&row);
  printf("row: %d ,",row);
  fscanf(fp,"%d",&column);
  printf("column: %d \n",column);

  if (fDedxMatrix) delete fDedxMatrix;
  fDedxMatrix   = new TMatrix(row,column);

  for(i=0;i<row;i++)
  {   
     for(ii=0;ii<column;ii++)
     {              
       fscanf(fp,"%le",&x); 
       fDedxMatrix->operator()(i,ii) = x;
     }
  } 
}

TArrayD T49Prob::GetAmplitude(Int_t charge,Float_t p)
{  
   Int_t i;
   Int_t ii;
   Int_t a;  

   Double_t  A[4];  
   Double_t  amplitude;
   TArrayD   ampl(4); 
   
 

   for(i=0;i<4;i++) 
   {
      A[i] = 0;    
      if(charge == 1)
      {
         for(ii=0;ii<fDedxFitPosMatrix->GetNcols();ii++)
         {
            a= i*3 ;
            A[i] = A[i] + fDedxFitPosMatrix->operator()(a,ii)*pow(p,ii);                   
         }
      }   
      if(charge == -1)
      {
         for(ii=0;ii<fDedxFitNegMatrix->GetNcols();ii++)
         {
            a= i*3 ;
            A[i] = A[i] + fDedxFitNegMatrix->operator()(a,ii)*pow(p,ii);                  
         }
      }  
      amplitude = A[i];
      ampl.AddAt(amplitude,i);
       
   }
   
   return ampl;
} 

TArrayD T49Prob::GetMean(Int_t charge,Float_t p)
{  
   Int_t i;
   Int_t ii;
   Int_t a;  
  
   Double_t  M[4]; 
   Double_t  meanvalue;
   TArrayD   mean(4); 
   
 

   for(i=0;i<4;i++) 
   {
      M[i] = 0;
      if(charge == 1)
      {
         for(ii=0;ii<fDedxFitPosMatrix->GetNcols();ii++)
         {
            a= i*3 ;          
            M[i] = M[i] + fDedxFitPosMatrix->operator()(a+1,ii)*pow(p,ii);                     
         }
      }   
      if(charge == -1)
      {
         for(ii=0;ii<fDedxFitNegMatrix->GetNcols();ii++)
         {
            a= i*3 ;           
            M[i] = M[i] + fDedxFitNegMatrix->operator()(a+1,ii)*pow(p,ii);                    
         }
      }  
      meanvalue = M[i];
      mean.AddAt(meanvalue,i);
       
   }
   
   return mean;
} 

TArrayD T49Prob::GetSigma(Int_t charge,Float_t p)
{  
   Int_t i;
   Int_t ii;
   Int_t a;  

   Double_t  S[4];
   Double_t  sigma;
   TArrayD   sig(4); 
   
   for(i=0;i<4;i++) 
   {
     
      S[i] = 0;
      if(charge == 1)
      {
         for(ii=0;ii<fDedxFitPosMatrix->GetNcols();ii++)
         {
            a= i*3 ;         
            S[i] = S[i] + fDedxFitPosMatrix->operator()(a+2,ii)*pow(p,ii);           
         }
      }   
      if(charge == -1)
      {
         for(ii=0;ii<fDedxFitNegMatrix->GetNcols();ii++)
         {
            a= i*3 ;          
            S[i] = S[i] + fDedxFitNegMatrix->operator()(a+2,ii)*pow(p,ii);           
         }
      }  
      sigma = S[i];
      sig.AddAt(sigma,i);
       
   }
   
   return sig;
} 


TArrayD T49Prob::GetDedxAmplitude(Int_t charge,Float_t p,Float_t dedx)
{  
   Int_t i;   

   TArrayD  A(4);
   TArrayD  M(4);
   TArrayD  S(4);
   Double_t  amplitude;
   TArrayD   ampl(4); 
   
   A = GetAmplitude(charge,p);
   M = GetMean(charge,p);
   S = GetSigma(charge,p);

   for(i=0;i<4;i++) 
   {
     
      amplitude = A.At(i)*exp(-1*((dedx-M.At(i))*(dedx-M.At(i)))/(2*S.At(i)*S.At(i)));
      ampl.AddAt(amplitude,i);
       
   }
   
   return ampl;
} 


TArrayD T49Prob::GetDedxProbability(Int_t charge,Float_t p,Float_t dedx)
{  
   Int_t i;
   
   TArrayD   ampl;
   Double_t  asum;
   Double_t  prob; 
   TArrayD   DedxProb(5); 

   ampl  = GetDedxAmplitude(charge,p,dedx);
 
   asum = ampl.At(0)+ ampl.At(1)+ ampl.At(2)+ ampl.At(3);
    
   for(i=0;i<4;i++)
   {
      prob = ampl.At(i)/asum;
      DedxProb.AddAt(prob,i);
     
   }
   DedxProb.AddAt(asum,4);
   
   return DedxProb;
} 

Double_t T49Prob::GetProtonAmplitude(Int_t charge,Float_t p)
{
   TArrayD  ampl;
   ampl = GetAmplitude(charge,p);

   return ampl.At(2);
}

Double_t T49Prob::GetKaonAmplitude(Int_t charge,Float_t p)
{
   TArrayD  ampl;
   ampl = GetAmplitude(charge,p);

   return ampl.At(1);
}

Double_t T49Prob::GetPionAmplitude(Int_t charge,Float_t p)
{
   TArrayD  ampl;
   ampl = GetAmplitude(charge,p);

   return ampl.At(0);
}

Double_t T49Prob::GetElectronAmplitude(Int_t charge,Float_t p)
{
   TArrayD  ampl;
   ampl = GetAmplitude(charge,p);

   return ampl.At(3);
}

Double_t T49Prob::GetProtonAmplitude(Int_t charge,Float_t p,Float_t dedx)
{
   TArrayD  ampl;
   ampl = GetDedxAmplitude(charge,p,dedx);

   return ampl.At(2);
}

Double_t T49Prob::GetKaonAmplitude(Int_t charge,Float_t p,Float_t dedx)
{
   TArrayD  ampl;
   ampl = GetDedxAmplitude(charge,p,dedx);

   return ampl.At(1);
}

Double_t T49Prob::GetPionAmplitude(Int_t charge,Float_t p,Float_t dedx)
{
   TArrayD  ampl;
   ampl = GetDedxAmplitude(charge,p,dedx);

   return ampl.At(0);
}

Double_t T49Prob::GetElectronAmplitude(Int_t charge,Float_t p,Float_t dedx)
{
   TArrayD  ampl;
   ampl = GetDedxAmplitude(charge,p,dedx);

   return ampl.At(3);
}

Double_t T49Prob::GetProtonProbability(Int_t charge,Float_t p,Float_t dedx)
{
   TArrayD  prob;
   prob = GetDedxProbability(charge,p,dedx);

   return prob.At(2);
}

Double_t T49Prob::GetKaonProbability(Int_t charge,Float_t p,Float_t dedx)
{
   TArrayD  prob;
   prob = GetDedxProbability(charge,p,dedx);

   return prob.At(1);
}

Double_t T49Prob::GetPionProbability(Int_t charge,Float_t p,Float_t dedx)
{
   TArrayD  prob;
   prob = GetDedxProbability(charge,p,dedx);

   return prob.At(0);
}
 
Double_t T49Prob::GetElectronProbability(Int_t charge,Float_t p,Float_t dedx)
{
   TArrayD  prob;
   prob = GetDedxProbability(charge,p,dedx);

   return prob.At(3);
}



void T49Prob::ReadDedxFit(Char_t fname[256])
{  
  FILE  *fp; 
  Int_t i;
  Int_t ii;
  Int_t row;
  Int_t column;
  Double_t x;

  
 

  fp = fopen(fname,"r");

  if(fp==NULL) 
  {
     fprintf(stderr,"Error: open cutfile %s\n",fname);
  }
 
  printf("read matrix: %s ",fname);
  fscanf(fp,"%d",&row);
  printf("row: %d ,",row);
  fscanf(fp,"%d",&column);
  printf("column: %d \n",column);

  //make two lists pos and neg
  row = row/2;

  if (fDedxFitPosMatrix) delete fDedxFitPosMatrix;
  if (fDedxFitNegMatrix) delete fDedxFitNegMatrix;
  fDedxFitPosMatrix = new TMatrix(row,column);
  fDedxFitNegMatrix = new TMatrix(row,column); 

  for(i=0;i<(row);i++)
  {   
     for(ii=0;ii<column;ii++)
     {              
       fscanf(fp,"%le",&x); 
       fDedxFitPosMatrix->operator()(i,ii) = x;      
     }    
  } 
  for(i=0;i<row;i++)
  {   
     for(ii=0;ii<column;ii++)
     {              
       fscanf(fp,"%le",&x); 
       fDedxFitNegMatrix->operator()(i,ii) = x;
     }    
  } 

}


Double_t T49Prob::GetParaPP(Int_t id, Int_t charge, Float_t p)
{    
  //  id = 0 electron
  //  id = 1 pion
  //  id = 2 kaon
  //  id = 3 proton

   
         Float_t Par1 = 3.72;
         Float_t Par2 = 1.37;
         Float_t Par3 = 4.0;
         Float_t Par4 = 1.51;
         Float_t Par5 = -2.0;

         Double_t mass[4];
         Double_t bg;
         Int_t part;  
	                         
         Float_t xx, d, beta, gamma, ff;
         Float_t BMin, a, b, m;
      
   	 mass[0] = 0.00000026112;
      	 mass[1] = 0.019488;
      	 mass[2] = 0.24374;
      	 mass[3] = 0.879844;

         part = id;             
         bg = p / sqrt (mass[part]);   


  
            BMin = Par1/sqrt(1.0+Par1*Par1);
            a    = -Par5/2.0*(1.0-BMin*BMin)/TMath::Power(BMin,(Par5+4.0));
            b    = Par1*Par1*(1.-BMin*BMin*(1.+2./Par5))+log(1.-BMin*BMin);
            m    = (Par3-Par2)/((Par4/a-b+1.)/2./log(10.)-Par2);
            xx    = log10(bg);
            d    = 0.0;
            if(xx > Par2)
            {              
               d  = xx-Par2+(Par2-Par3)/m;
               if(xx < Par3)
               {                      
                   d = d+TMath::Power((Par3-xx),m)/(m*TMath::Power((Par3-Par2),(m-1.))); 
               }
            }
            d     = d*2.*log(10.);
            beta  = bg/sqrt(1.+bg*bg);
            gamma = bg/beta;

            ff     = a*TMath::Power(beta,Par5)*(b+2.*log(gamma)-beta*beta-d);

            // now we calculate the truncated mean from the dE/dx
            ff     = ff*(1.+0.1800*log(ff)-0.0155*log(ff)*log(ff));
          	                                                       
  
            return ff;
}


Double_t T49Prob::GetParaPbPb(Int_t id, Int_t charge, Float_t p)
{ 
 //  id = 0 electron
  //  id = 1 pion
  //  id = 2 kaon
  //  id = 3 proton


         Double_t mass[4]; 
         Double_t x1, x2,p0,bg,ff,d1,d2,d3,dfq;
         Int_t part;
         
         Float_t c_par1 = 1.597546;
         Float_t d_par1 = 9.8;
   	 Float_t e_par1 = 2.38; 
  	 Float_t f_par1 = 0.2;

   	 mass[0] = 0.00000026112;
      	 mass[1] = 0.019488;
      	 mass[2] = 0.24374;
      	 mass[3] = 0.879844;


    
             part = id;             
             bg = p / sqrt (mass[part]);        
             x1 = pow(10,(e_par1 - 1.0/3.0 * sqrt(2.0*log(10.0)/(3.0 * f_par1))));
             x2 = pow(10,(e_par1 + 2.0/3.0 * sqrt(2.0*log(10.0)/(3.0 * f_par1))));
          
             p0 = c_par1/(d_par1 + 2.0*log(10.0)*e_par1 - 1.0);
          
             if (bg<x1)
               dfq = 0;
             else
             {
                 dfq = -2.0 * log(bg) + 2.0 * log(10.0) * e_par1;
                 if(bg<x2)
                 {
                     d1 = 2.0/3.0*sqrt(2.0*log(10.0)/(3.0 * f_par1));
                     d2 = log(bg)/log(10.0);
                     d3 = pow((e_par1 + d1 - d2),3);
                     dfq -= f_par1*d3;
                 }
             }
       
             ff = p0*( (1+ bg*bg)/(bg*bg) * ( d_par1 + log(1+(bg*bg)) + dfq)-1.0 + 0.01 * log10(bg) );
       return ff;  
}          
           
