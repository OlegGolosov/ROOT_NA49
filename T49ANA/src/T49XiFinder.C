///////////////////////////////////////////////////////////////////////
//                                                                   //
//  T49XiFinder applies the xifinder algorithm to mini-DST data.     //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include <cstdio>

#include "TObjArray.h"

#include "T49ParticleRoot.h"
#include "T49VertexRoot.h"

#include "T49XiFinder.h"
#include "T49Trkstep.h"

ClassImp(T49XiFinder)

//______________________________________________________________________________
T49XiFinder::T49XiFinder()
{
  //
  // T49XiFinder default constructor
  //

  fNeutralInputArray    = 0;
  fChargedInputArray    = 0;

  fXiArray              = 0;
  fNeutralDaughterArray = 0;
  fChargedDaughterArray = 0;

  fTrkstepCharged       = 0;
  fTrkstepXi            = 0;
  
  fZtarget              = 0;
  fZmin                 = 0;
  fZstepsize            = 0;
  fDcaXcut              = 0;
  fDcaYcut              = 0;
  fDcaRcut              = 0;
  fSideCut              = 0;
  fPtarmCut             = 0;
  fPtMaxCut             = 0;
  fPtotMaxCut           = 0;
  fXtargCut             = 0;
  fYtargCut             = 0;
  fV0MassMin            = 0;
  fV0MassMax            = 0;
  fV0Zmin               = 0;
  fV0ArmPtMax           = 0;
  fChargedXtargMin      = 0;
  fChargedYtargMin      = 0;
  fChargedNPointMin     = 0;

}

//______________________________________________________________________________
T49XiFinder::~T49XiFinder()
{
  //
  // T49XiFinder destructor
  //

  if (fNeutralInputArray) {
    fNeutralInputArray->Delete();
    delete fNeutralInputArray;
  }
  if (fChargedInputArray) {
    fChargedInputArray->Delete();
    delete fChargedInputArray;
  }
  if (fXiArray) {
    fXiArray->Delete();
    delete fXiArray;
  }
  if (fNeutralDaughterArray) {
    fNeutralDaughterArray->Delete();
    delete fNeutralDaughterArray;
  }
  if (fChargedDaughterArray) {
    fChargedDaughterArray->Delete();
    delete fChargedDaughterArray;
  }

}

//______________________________________________________________________________
void T49XiFinder::InitGlobalParameter()
{
  //
  // Initializes the T49XiFinder with the standard parameter for the global 
  // reconstruction and 256tb data.
  //

  fZtarget          = -581.1;
  fZmin             = -560.0; 
  fZstepsize        =    2.0;
  fDcaXcut          =    1.0;  
  fDcaYcut          =    1.0;  
  fDcaRcut          =  999.0;
  fSideCut          =    0.0;
  fPtarmCut         =    0.30;
  fPtMaxCut         =   50.0;
  fPtotMaxCut       =  500.0;
  fXtargCut         =    3.0;
  fYtargCut         =    3.0;
  fV0MassMin        =    1.101;
  fV0MassMax        =    1.131;
  fV0Zmin           = -510.0;
  fV0ArmPtMax       =    0.25;
  fChargedXtargMin  =    1.0;
  fChargedYtargMin  =    0.0;
  fChargedNPointMin =   10; 

  if (fTrkstepCharged) delete fTrkstepCharged;
  fTrkstepCharged = new T49Trkstep();
  fTrkstepCharged->SetKeys("3905","RU00","V00A","COMB/STD+","FULL","R95A");
  fTrkstepCharged->InitDS("trkstep_charged");  

  if (fTrkstepXi)      delete fTrkstepXi;
  fTrkstepXi      = new T49Trkstep();
  fTrkstepXi->SetKeys("3905","RU00","V00A","COMB/STD+","FULL","R95A");
  fTrkstepXi->InitDS("trkstep_xi");  

}

//______________________________________________________________________________
Bool_t T49XiFinder::FindXis()
{
  //
  // Looks for Xi/Omega candidates employing the same procedure than the
  // xifinder of the reconstruction chain.
  //

  Int_t trkstepFlag = 0;

  if ((!fTrkstepXi) || (!fTrkstepCharged)) {
    printf("<T49XiFinder::FindXis> Define the T49Trkstep objects first.\n");
    return kFALSE;
  }    

  // Create output arrays if neccessary
  if (!fXiArray) {
    fXiArray = new TObjArray();
  }
  else {
    fXiArray->Delete();
  }
  if (!fNeutralDaughterArray) {
    fNeutralDaughterArray = new TObjArray();
  }
  else {
    fNeutralDaughterArray->Delete();
  }
  if (!fChargedDaughterArray) {
    fChargedDaughterArray = new TObjArray();
  }
  else {
    fChargedDaughterArray->Delete();
  }

  // Pair combinations possible?
  if ((fChargedInputArray->GetEntries() > 0) &&
      (fNeutralInputArray->GetEntries() > 0)) {

    // Determine the largest vertez z of the lambda
    Float_t zLamMax = -9999.0;
    Float_t zLabMax = -9999.0;
    for (Int_t iNeutral = 0;
	 iNeutral < fNeutralInputArray->GetEntries();
         iNeutral++) {
      T49ParticleRoot *neutral = (T49ParticleRoot *) 
                                 fNeutralInputArray->UncheckedAt(iNeutral);
      if ((neutral->GetIflag() & 1)) {
        if (neutral->GetZFirst() > zLamMax) {
          zLamMax = neutral->GetZFirst();
        }
      }
      if ((neutral->GetIflag() & 2)) {
        if (neutral->GetZFirst() > zLabMax) {
          zLabMax = neutral->GetZFirst();
        }
      }
    }

    for (Int_t iCharged = 0; 
         iCharged < fChargedInputArray->GetEntries(); 
         iCharged++) {

      T49ParticleRoot *charged = (T49ParticleRoot *) 
                                 fChargedInputArray->UncheckedAt(iCharged);

      // Antiparticle ?
      Bool_t anti = kFALSE;
      if (charged->GetCharge() > 0) {
        anti = kTRUE;
      }

      // Reset the found flag and dcas
      for (Int_t iNeutral = 0; 
           iNeutral < fNeutralInputArray->GetEntries(); 
           iNeutral++) {
        T49ParticleRoot *neutral = (T49ParticleRoot *) 
                                   fNeutralInputArray->UncheckedAt(iNeutral);
        neutral->SetIdDet(0);
        neutral->SetTofX(999.0);
        neutral->SetTofY(999.0);
      }

      // Initialize trkstep. Tracking has to start from track position at target 
      // or the start vertex position (stored in the last point coordinates).
      fTrkstepCharged->SetCharge(charged->GetCharge());
      fTrkstepCharged->SetP(charged->GetPx()
                           ,charged->GetPy()
                           ,charged->GetPz());
      fTrkstepCharged->SetPos(charged->GetXLast()
                             ,charged->GetYLast()
                             ,charged->GetZLast());
      trkstepFlag = fTrkstepCharged->TrackTo(charged->GetZFirst()); 
      if (trkstepFlag) {
        printf("<T49XiFinder::FindXis> Trkstep error charged 0 (%d)\n",trkstepFlag);
        break;
      }

      // Determine starting position 
      Float_t zstart = charged->GetZFirst();
      if (anti) {
        if (zstart > zLabMax) {
          zstart = zLabMax;
	}
      }
      else {
        if (zstart > zLamMax) {
          zstart = zLamMax;
        }
      }

      // Step backwards
      for (Float_t zstep = zstart; zstep > fZmin; zstep -= fZstepsize) {

        trkstepFlag = fTrkstepCharged->TrackTo(zstep); 
        if (trkstepFlag) {
          printf("<T49XiFinder::FindXis> Trkstep error charged 1 (%d)\n",trkstepFlag);
          break;
	}

        Float_t x_c = fTrkstepCharged->GetX();
        Float_t y_c = fTrkstepCharged->GetY();

        for (Int_t iNeutral = 0; 
             iNeutral < fNeutralInputArray->GetEntries(); 
             iNeutral++) {

          T49ParticleRoot *neutral = (T49ParticleRoot *) 
                                     fNeutralInputArray->UncheckedAt(iNeutral);

          Float_t x_n     = neutral->GetXFirst();
          Float_t y_n     = neutral->GetYFirst();
          Float_t z_n     = neutral->GetZFirst();
          Float_t px_n    = neutral->GetPx();
          Float_t py_n    = neutral->GetPy();
          Float_t pz_n    = neutral->GetPz();
          Int_t   iflag_n = neutral->GetIflag();
          Int_t   found_n = neutral->GetIdDet();

          // Take only combinations where the (anti)lambda is in the right mass window
          if (( anti &&  (iflag_n & 2) ) ||
              (!anti &&  (iflag_n & 1))) {

            // (Anti)lambda vertex must be downstream
            if (zstep < (z_n + fZstepsize)) {

              if (fVerbose > 2) {
                printf("<T49XiFinder::FindXis>");
                printf(" combination charged %d + neutral %d, step = %f\n"
                      ,iCharged,iNeutral,zstep); 
	      }

	      // Check whether combination was already found
              if (found_n == 0) {

		// Calculate dcas
                Float_t dcaX = TMath::Abs((x_n + (px_n/pz_n * (zstep - z_n))) -  x_c);
                Float_t dcaY = TMath::Abs((y_n + (py_n/pz_n * (zstep - z_n))) -  y_c);
                Float_t dcaR = TMath::Sqrt(dcaX*dcaX + dcaY*dcaY);

                if (fVerbose > 2) {
                  printf("<T49XiFinder::FindXis>");
                  printf(" dcaX = %f, dcaY = %f, dcaR = %f\n",dcaX,dcaY,dcaR);
		}

		// Minimum in dca?
                Float_t vdca1 = neutral->GetTofX();
                Float_t vdca2 = neutral->GetTofY();
                if ((dcaX  <= fDcaXcut) && 
                    (dcaY  <= fDcaYcut) &&
                    (vdca1 <      dcaR) &&
                    (vdca1 <     vdca2) &&
                    (vdca2 <     999.0)) {

                  if (fVerbose > 1) {
                    printf("<T49XiFinder::FindXis>");
                    printf(" minimum in DCA found\n");
		  }

		  // Mark this combination as already found
                  neutral->SetIdDet(1);

		  // Find vertex z
                  Float_t a  = zstep;
                  Float_t b  = zstep +       fZstepsize;
                  Float_t c  = zstep + 2.0 * fZstepsize;
                  Float_t fa = dcaR;
                  Float_t fb = vdca1;
                  Float_t fc = vdca2;
                  Float_t dn = (((b-a)*(fb-fc)) - ((b-c)*(fb-fa))) * 2.0;
                  if (dn == 0) {
                    printf("<T49XiFinder::FindXis> Interpolation error\n");
                    goto failed_r_cut;
		  }
                  Float_t decayz = b - ((((b-a)*(b-a)*(fb-fc)) 
                                       - ((b-c)*(b-c)*(fb-fa))) / dn); 

		  // Track to the found vertex position
                  trkstepFlag = fTrkstepCharged->TrackTo(decayz);
                  if (trkstepFlag) {
                    printf("<T49XiFinder::FindXis> Trkstep error charged 2 (%d)\n",trkstepFlag);
                    goto failed_r_cut;
	          }

                  x_c = fTrkstepCharged->GetX();
                  y_c = fTrkstepCharged->GetY();

		  // Calculate dcas again
                  dcaX = TMath::Abs((x_n + (px_n/pz_n * (zstep - z_n))) -  x_c);
                  dcaY = TMath::Abs((y_n + (py_n/pz_n * (zstep - z_n))) -  y_c);
                  dcaR = TMath::Sqrt(dcaX*dcaX + dcaY*dcaY);

                  // Momentum of charged particle at the Xi vertex
                  Float_t px_c = fTrkstepCharged->GetPx();
                  Float_t py_c = fTrkstepCharged->GetPy();
                  Float_t pz_c = fTrkstepCharged->GetPz();

		  // Good Xi candidate
                  if (dcaR > fDcaRcut) goto failed_r_cut;
                  if (fVerbose > 1) {
                    printf("<T49XiFinder::FindXis>");
                    printf(" candidate passed DCA cut\n");
		  }

                  // Calculate the Xi decay vertex position
                  Float_t x_xi = x_n + (px_n/pz_n * (decayz - z_n));
                  Float_t y_xi = y_n + (py_n/pz_n * (decayz - z_n));
                  Float_t z_xi = decayz;

		  // The Xi invariant mass
                  Float_t e_pion_c = charged->GetE(kPionMass);
                  Float_t e_kaon_c = charged->GetE(kKaonMass);
                  Float_t e_n      = neutral->GetE(kLambdaMass);
                  Float_t p_n_c    = (px_c*px_n) + (py_c*py_n) + (pz_c*pz_n); 
                  Float_t diff_xi  = e_pion_c*e_n - p_n_c; 
                  Float_t diff_om  = e_kaon_c*e_n - p_n_c;
                  Float_t minv_xi  = kPionMass*kPionMass 
                                   + kLambdaMass*kLambdaMass 
                                   + 2.0 * diff_xi;
                  Float_t minv_om  = kKaonMass*kKaonMass 
                                   + kLambdaMass*kLambdaMass 
                                   + 2.0 * diff_om;
                  if (minv_xi > 0.0) {
                    minv_xi = TMath::Sqrt(minv_xi);
		  }
                  else {
                    minv_xi = -999.0;
		  }
                  if (minv_om > 0.0) {
                    minv_om = TMath::Sqrt(minv_om);
		  }
                  else {
                    minv_om = -999.0;
		  }

                  if ((minv_xi > 0.0) && (minv_om > 0.0)) {

  		    // The Xi momentum components at the decay vertex
                    Float_t px_xi   = px_c + px_n;
                    Float_t py_xi   = py_c + py_n;
                    Float_t pz_xi   = pz_c + pz_n;
                    Float_t ptot_xi = TMath::Sqrt(px_xi*px_xi 
                                                + py_xi*py_xi 
                                                + pz_xi*pz_xi);

		    // The armenteros variables
                    Float_t ptotsq_n = px_n*px_n + py_n*py_n + pz_n*pz_n;
                    Float_t ppar_n   = (px_xi*px_n + py_xi*py_n + pz_xi*pz_n)
                                     / ptot_xi;
                    Float_t ptarm    = ptotsq_n - (ppar_n*ppar_n);
                    if (ptarm >= 0.0) {
                      ptarm = TMath::Sqrt(ptarm);
		    }
                    else {
                      ptarm = 9999.0;
		    }

		    // The Xi parameters in the target plane
                    Float_t xtarg_xi = 999.0;
                    Float_t ytarg_xi = 999.0;
                    Float_t ztarg_xi = 999.0;
                    fTrkstepXi->SetCharge(charged->GetCharge());
                    fTrkstepXi->SetPos(x_xi,y_xi,z_xi);
                    fTrkstepXi->SetP(px_xi,py_xi,pz_xi);
                    trkstepFlag = fTrkstepXi->TrackTo(fZtarget);
                    if (trkstepFlag) {
                      printf("<T49XiFinder::FindXis> Trkstep error xi (%d)\n",trkstepFlag);
                      break;
	            }
                    else {
                      xtarg_xi = fTrkstepXi->GetX();
                      ytarg_xi = fTrkstepXi->GetY();
                      ztarg_xi = fTrkstepXi->GetZ();
		    }
                    Float_t pxtarg_xi  = fTrkstepXi->GetPx();
                    Float_t pytarg_xi  = fTrkstepXi->GetPy();
                    Float_t pztarg_xi  = fTrkstepXi->GetPz();
                    Float_t pttarg_xi  = TMath::Sqrt(pxtarg_xi*pxtarg_xi 
                                                   + pytarg_xi*pytarg_xi);
                    Float_t x1minx2_xi = x_n + (px_n/pz_n * (fZtarget - z_n)) 
                                       - charged->GetTofX();

		    if (fVerbose) {
                      printf("<T49XiFinder::FindXis>");
                      printf(" found a candidate before cuts\n");
		    }

		    // Apply the xifinder pair cuts
                    if (ptarm                 > fPtarmCut  ) goto found_one;
		    if (pttarg_xi             > fPtMaxCut  ) goto found_one;
                    if (ptot_xi               > fPtotMaxCut) goto found_one;
                    if (TMath::Abs(xtarg_xi)  > fXtargCut  ) goto found_one;
                    if (TMath::Abs(ytarg_xi)  > fYtargCut  ) goto found_one;
                    if (fSideCut * x_n * x_xi < 0.0        ) goto found_one;

		    if (fVerbose) {
                      printf("<T49XiFinder::FindXis>");
                      printf(" found a candidate after cuts\n");
		    }

		    // Create a new particle structure for the neutral
		    // daughter and add it the to the output list
                    T49ParticleRoot *n_dght = new T49ParticleRoot(neutral);
                    fNeutralDaughterArray->Add(n_dght);
                       
		    // Create a new particle structure for the charged
		    // daughter and add it the to the output list
                    T49ParticleRoot *c_dght = new T49ParticleRoot(charged);
                    c_dght->SetPx(px_c);
                    c_dght->SetPy(py_c);
                    c_dght->SetPz(pz_c);
                    fChargedDaughterArray->Add(c_dght);

                    // Create a new vertex structure for this Xi/Omega 
		    // candidate and add it to the output list
                    T49VertexRoot *xi = new T49VertexRoot();
                    xi->SetX(x_xi);
                    xi->SetY(y_xi);
                    xi->SetZ(z_xi);
                    xi->SetXTarg(xtarg_xi);
                    xi->SetYTarg(ytarg_xi);
                    xi->SetPxXi(pxtarg_xi);
                    xi->SetPyXi(pytarg_xi);
                    xi->SetPzXi(pztarg_xi);
                    xi->SetX1minX2(x1minx2_xi);
                    xi->SetDaughters(c_dght,n_dght);
		    // For testing purposes
                    xi->SetPchi2(minv_xi);
                    fXiArray->Add(xi);

		  } // if: xi and omega mass ok

		} // if: minimum in dca

		// Store last two dcas
	        failed_r_cut:
                neutral->SetTofX(dcaR);
                neutral->SetTofY(vdca1);

	      } // if: lambda already used

	    } // if: lambda downstream

	  } // if: combination (anti)particle <-> (anti)particle

	  found_one:
	  continue;

	} // loop: neutral

      } //  loop: trkstep backwards for charged

    } // loop: charged 

  } // if: pair combinations

  return kTRUE;

}

//______________________________________________________________________________
void T49XiFinder::CreateChargedInput(TObjArray *inputArray, Bool_t primary)
{
  //
  // Defines the charge input array by applying the xifinder cuts to the 
  // pion/kaon candidate.
  //

  if (!fTrkstepCharged) {
    printf("<T49XiFinder::SetChargedInput> Define the T49Trkstep objects first.\n");
  }
  else {

    if (fChargedInputArray) {
      fChargedInputArray->Delete();
    }
    else {
      fChargedInputArray = new TObjArray();
    }
  
    for (Int_t idx = 0; idx < inputArray->GetEntries(); idx++) {

      T49ParticleRoot *charged = (T49ParticleRoot *) inputArray->UncheckedAt(idx);

      // Check whether track is trackable
      fTrkstepCharged->SetCharge(charged->GetCharge());
      if (primary) {
        charged->SetXLast(charged->GetBx());
        charged->SetYLast(charged->GetBy());
        charged->SetZLast(fZtarget);
      } 
      fTrkstepCharged->SetPos(charged->GetXLast()
                             ,charged->GetYLast()
                             ,charged->GetZLast());
      fTrkstepCharged->SetP(charged->GetPx()
                           ,charged->GetPy()
                           ,charged->GetPz());

      if (!(fTrkstepCharged->TrackTo(charged->GetZFirst()))) {

        if (!(fTrkstepCharged->TrackTo(fZtarget))) {

          Float_t xtarg_charged = fTrkstepCharged->GetX();
          Float_t ytarg_charged = fTrkstepCharged->GetY();

          // Check impact parameter and number of points only for primary tracks
          Bool_t accept = kTRUE;
          if (primary) {
            if ((TMath::Abs(ytarg_charged) < fChargedYtargMin ) &&
                (TMath::Abs(xtarg_charged) < fChargedXtargMin ) && 
                (charged->GetNPoint(3)     < fChargedNPointMin)) {
              accept = kFALSE;
	    }
	  }

          if (accept) {

   	    // Add to the charge daughter list
            T49ParticleRoot *chargedPart = new T49ParticleRoot();
            chargedPart->CopyParticle(charged);
            chargedPart->SetTofX(xtarg_charged);
            chargedPart->SetTofY(ytarg_charged);
            fChargedInputArray->Add(chargedPart);

	  }

        }

      }

    }

    if (fVerbose) {
      printf("<T49XiFinder::CreateChargedInput>");
      printf(" accepted %d charged input particles\n"
            ,fChargedInputArray->GetEntries());
    }

  }

}

//______________________________________________________________________________
void T49XiFinder::CreateNeutralInput(TObjArray *inputArray, Bool_t primary)
{
  //
  // Defines the neutral input array by applying the xifinder cuts to the 
  // V0 candidate.
  //

  if (fNeutralInputArray) {
    fNeutralInputArray->Delete();
  }
  else {
    fNeutralInputArray = new TObjArray();
  }
  
  for (Int_t idx = 0; idx < inputArray->GetEntries(); idx++) {

    T49VertexRoot *neutral = (T49VertexRoot *) inputArray->UncheckedAt(idx);

    Int_t flag = 0;

    // Apply V0 cuts only for primary tracks
    Bool_t accept = kTRUE;
    if (primary) {
      if ((neutral->GetZ()            < fV0Zmin    ) &&
          (neutral->GetArmenterosPt() > fV0ArmPtMax)) {
        accept = kFALSE;
      }
    }

    if (accept) {

      // Check for a lambda candidate
      Float_t minvLambda     = neutral->GetInvariantMassLambda();
      if ((minvLambda     >= fV0MassMin) &&
          (minvLambda     <= fV0MassMax)) {
        flag += 1;
      }

      // Check for a antilambda candidate
      Float_t minvAntiLambda = neutral->GetInvariantMassAntiLambda();
      if ((minvAntiLambda >= fV0MassMin) &&
          (minvAntiLambda <= fV0MassMax)) {
        flag += 2;
      }

      // Add to the daughter list if neutral or antineutral
      if (flag) {
        T49ParticleRoot *neutralPart = new T49ParticleRoot();
        neutralPart->SetPx(neutral->GetPx());
        neutralPart->SetPy(neutral->GetPy());
        neutralPart->SetPz(neutral->GetPz());
        neutralPart->SetXFirst(neutral->GetX());
        neutralPart->SetYFirst(neutral->GetY());
        neutralPart->SetZFirst(neutral->GetZ());
        neutralPart->SetIflag(flag);
        neutralPart->SetIdDet(0);
        neutralPart->SetBx(neutral->GetXTarg());
        neutralPart->SetBy(neutral->GetYTarg());
        neutralPart->SetLabel(neutral->GetNTrkFit());
        fNeutralInputArray->Add(neutralPart);
      }

    }

  }

  if (fVerbose) {
    printf("<T49XiFinder::CreateNeutralInput>");
    printf(" accepted %d neutral input particles\n"
          ,fNeutralInputArray->GetEntries());
  }

}

//______________________________________________________________________________
void T49XiFinder::SetTrkstepCharged(T49Trkstep *trkstep)
{
  //
  // Define T49Trkstep for the charged tracks.
  //

  if (fTrkstepCharged) delete fTrkstepCharged;
  fTrkstepCharged = trkstep;

}

//______________________________________________________________________________
void T49XiFinder::SetTrkstepXi(T49Trkstep *trkstep)
{
  //
  // Define T49Trkstep for the Xi/Omega.
  //

  if (fTrkstepXi) delete fTrkstepXi;
  fTrkstepXi = trkstep;

}

//______________________________________________________________________________
void T49XiFinder::DeleteChargedInput()
{
  //
  // Deletes the charged input list
  //

  if (fChargedInputArray) {
    fChargedInputArray->Delete();
    delete fChargedInputArray;
    fChargedInputArray = 0;
  }

}

//______________________________________________________________________________
void T49XiFinder::DeleteNeutralInput()
{
  //
  // Deletes the neutral input list
  //

  if (fNeutralInputArray) {
    fNeutralInputArray->Delete();
    delete fNeutralInputArray;
    fNeutralInputArray = 0;
  }

}






