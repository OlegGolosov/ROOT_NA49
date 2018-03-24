#include "T49HbtMomentumResolutionCalculation.h"
ClassImp(T49HbtMomentumResolutionCalculator)
ClassImp(T49HbtMomentumResolutionProvider)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The T49HbtMomentumResolutionCalculator class is intended to produce for each qSide,qLong,qOut bin in each kt-Y bin 
// the momentum resolution in qSide,qOut,qLong and a possible shift in qSide,qOut,qLong. To obtain these values 3 vectors 
// for each cell are filled holding the difference between a Monte Carlo pair (MC) and the reconstructed pair (RC) 
// in qSide, qOut and qLong. A gaus fit to each vector gives the RMS (=resolution) and the shift, which will is needed  
// by the smear procedure in T49HbtMomentumResolutionCorrection.
//
// The constructor wants to know the dimension of the qside,qout,qlong 3d object and the kt-Y binning.
// The mixing macro calls the Use function, enertering lists of MC and the according RC tracks.
// The result can be stored in a file which will be accessed by T49HbtMomentumResolutionCorrection 
// via T49HbtMomentumResolutionProvider
//
// Structure of T49HbtMomentumResolutionCalculator:
//
//             each kt-Y cell -> ListofDisplacedMomenta[#qside * #qout * #qlong] holding TVector3s of MomDiffs
//                            -> TH3D* ResolutionMeanQside (out,long)  results
//                            -> TH3D* ResolutionRMSQside  (out,long)  results
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

T49HbtMomentumResolutionCalculator::T49HbtMomentumResolutionCalculator(TH3D* MomentumDifferenceOfMCTracks,
Int_t nKtBins, Double_t minKt, Double_t maxKt, Int_t nYBins, Double_t minY, Double_t maxY)
{
  // Init master 3d histo
  mMomentumDifferenceOfMCTracks = MomentumDifferenceOfMCTracks ;
  mMomentumDifferenceOfMCTracks->SetName("Master3d") ;

  // Number of cells in the master 3d histogramm 
  mNumberOfCells = (mMomentumDifferenceOfMCTracks->GetNbinsX()+2) * 
                   (mMomentumDifferenceOfMCTracks->GetNbinsY()+2) *
                   (mMomentumDifferenceOfMCTracks->GetNbinsZ()+2) ;

  // Create TObjArrays holding MC and Reconstructed tracks
  mSelectedMCTracks = new TObjArray(1000) ;
  mSelectedRETracks = new TObjArray(1000) ;

  // Fix pion mass
  mPionMass = 0.139 ;
 
  // Calculate number of kt-Y bins
  mNKtBins = nKtBins ;
  mNYBins = nYBins ;
  mMaxKtY = mNKtBins * mNYBins ;
  
  // Calculate kt-Y bin intervalls
  // Kt
  mKtBins = new Double_t[mNKtBins+1] ;
  for(Int_t index = 0 ; index < (mNKtBins+1) ; index++)
    {
      mKtBins[index] = minKt + ( ((Double_t)(maxKt-minKt)/(Double_t)mNKtBins ) * (Double_t) index) ;
    }
  // Y
  mYBins  = new Double_t[mNYBins+1] ;
  for(Int_t index = 0 ; index < (mNYBins+1) ; index++)
    {
      mYBins[index] = minY + ( (maxY-minY)/mNYBins * index) ;
    }

  // Create set of objects for each kt-Y bin
  mListOfListOfDisplacedMomenta = new TObjArray(mMaxKtY) ;
  // Number of histor RMS in qSide,qOut,qLong and shift for of them = 6
  mNumberOfResolutionHistos = 6 ; 
  mResolution = new TObjArray(mMaxKtY*mNumberOfResolutionHistos) ;
  mResolution->SetName("resultArray") ;
  
  // Loop over kt-Y bins
  for(Int_t indexKtY = 0 ; indexKtY < mMaxKtY ; indexKtY++)
    {
      // Create list of displaced momenta for each kt-Y bin
      TObjArray* lListOfListOfDisplacedMomenta = new TObjArray(mNumberOfCells) ;
      mListOfListOfDisplacedMomenta->AddAt(lListOfListOfDisplacedMomenta,indexKtY) ;

      // Create resolution 3d histos for each kt-Y bin
      for(Int_t index_MRMS_osl = 0 ; index_MRMS_osl < mNumberOfResolutionHistos ; index_MRMS_osl++)
	{
	  // Compile name of the 6 histos (Mean and RMS for qSide, qOut and qLong) for each ktY bin
	  TString name;
	  switch (index_MRMS_osl)
	    {
	    case 0:
	      name.Append("ResolutionMeanQSide") ; break ;
	    case 1:
	      name.Append("ResolutionMeanQOut") ; break ;
	    case 2:
	      name.Append("ResolutionMeanQLong") ; break ;
	    case 3:
	      name.Append("ResolutionRMSQSide") ; break ;
	    case 4:
	      name.Append("ResolutionRMSQOut") ; break ;
	    case 5:
	      name.Append("ResolutionRMSQLong") ; break ;
	    default: 
	      cerr << "Error in T49HbtMomentumResolutionCalculator Constructor, should never happen ! "<< endl ; break;
	    };
	  
	  // Add kt-Y index to histo's name 
	  name += indexKtY ;
	  
	  // Create histo as a clone from the master
	  TH3D* lResolution =  (TH3D*) mMomentumDifferenceOfMCTracks->Clone(name.Data()) ;
	  lResolution->Reset() ;
      
	  // Store pointer to histo in result TObjArray
	  mResolution->AddAt(lResolution,indexKtY*mNumberOfResolutionHistos+index_MRMS_osl) ;
	}
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
T49HbtMomentumResolutionCalculator::~T49HbtMomentumResolutionCalculator()
{
  mListOfListOfDisplacedMomenta->Clear() ;
  mResolution->Clear() ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCalculator::Use(TObjArray*  MCTrackList, TObjArray*  RETrackList)
{
  // Empty TObjArrays holding MC and Reconstructed tracks
  mSelectedMCTracks->Clear() ;
  mSelectedRETracks->Clear() ;

  // Select MC tracks and find best associated reconstructed tracks  
  SelectedAssocicatedTracks(MCTrackList, RETrackList) ;
  
  // After previous step, mSelectedMCTracks and mSelectedRETracks should exist and hold same number of tracks
  if( mSelectedMCTracks->GetEntries() == mSelectedRETracks->GetEntries() && mSelectedMCTracks->GetEntries() > 0 )
    {
      // Create pairs and fill reconstructed momentum differences in appropriate TClonesArrays 
      FillDisplacedMomenta() ;
    }
  else
    {
      // This should not happen
      cerr << "Serious error: Wrong number of MC or RC pairs " << endl ;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCalculator::SelectedAssocicatedTracks(TObjArray*  MCTrackList, TObjArray*  RETrackList)
{
  //
  // Select reconstructed tracks which are best associated with a given MC track -> fill lists with selected tracks
  //
  Int_t MCtrackcounter = 0 ;
  for(Int_t indexMCTracks = 0; indexMCTracks < MCTrackList->GetEntries(); indexMCTracks++) 
    {
      // Get MC track
      T49ParticleMCRoot *MCTrack = (T49ParticleMCRoot *) MCTrackList->At(indexMCTracks) ;
      if(MCTrack->GetPid() == 9 && MCTrack->GetStartVertex() == 0 && MCTrack->GetNPoint()>10)
	{
	  // Search best matched primary track
	  T49ParticleRoot* BestRecTrack = NULL ;
	  Float_t BestRatio = 0.0 ;
	  Float_t BestQinv = 0.0 ;

	  // Loop over matched RC tracks
	  for (Int_t iMatch = 0 ; iMatch < MCTrack->GetNPriMatched() ; iMatch++) 
	    { 
	      // Get RC track
	      Int_t iRec = MCTrack->GetPriMatched(iMatch) ;
	      T49ParticleRoot *RecTrack = (T49ParticleRoot *) RETrackList->At(iRec) ;
	      
	      // Ratio #Matched/#PointsOnTrack
	      Float_t Ratio = (Float_t) MCTrack->GetNPriMatchPoint(iMatch)/ (Float_t) RecTrack->GetNPoint() ;
	      
	      // Qinv
	      Float_t qx  =  MCTrack->GetPx() - RecTrack->GetPx() ;
	      Float_t qy  =  MCTrack->GetPy() - RecTrack->GetPy() ;
	      Float_t qz  =  MCTrack->GetPz() - RecTrack->GetPz() ;
	      Float_t qE  =  MCTrack->GetE() - RecTrack->GetE(0.139) ;
	      Float_t Qinv = sqrt(-(qE*qE-(qx*qx+qy*qy+qz*qz))) ;
	      
	      if (iMatch == 0)
		{
		  // start values
		  BestRecTrack = RecTrack ;
		  BestRatio = Ratio ;
		  BestQinv = Qinv ;
		}
	      else 
		{
		  // check ratio and qinv to choose the only one
		  if(Ratio>BestRatio && Qinv>BestQinv)
		    {
		      BestRecTrack = RecTrack ;
		    }
		}
	    } // loop matched tracks
	     
	  // Ratio > 0.8 is required as additional criterion for a track match
	  if(BestRatio>0.8 && BestRecTrack) 
	    {
	      mSelectedMCTracks->AddAtAndExpand(MCTrack,MCtrackcounter) ;
	      mSelectedRETracks->AddAtAndExpand(BestRecTrack,MCtrackcounter) ;
	      MCtrackcounter++ ;
	    }
	} // if good mc track
    } // loop mc tracks
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCalculator::FillDisplacedMomenta()
{
  //
  // Construct all possible pairs of MC tracks
  //
  T49ParticleMCRoot *Part1 ;
  T49ParticleMCRoot *Part2 ;
    
  Int_t IP1 = 0 ;
  Int_t IP2 = 0 ;
  
  TIter NextParticle(mSelectedMCTracks) ;
  while ((Part1 = ((T49ParticleMCRoot *) NextParticle())))
    { 
      IP1++ ; 
      IP2 = 0 ;
      TIter NextP2(mSelectedMCTracks) ;
      while ((Part2 = ((T49ParticleMCRoot *) NextP2()))) 
	{ 
	  IP2++ ;
	  if(IP1 > IP2) 
	    {
	      FillDisplacedMomenta(Part1,Part2);
	    }
	}
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCalculator::FillDisplacedMomenta(T49ParticleMCRoot* MC1, T49ParticleMCRoot* MC2)
{
  //
  // Calculate MC and RE momentum difference, add difference MC-RE to  
  // vector with index according to the MCqout,MCside,MClong position in master TH3D 
  //

  // Calculate MC momentum difference
  Double_t MCBeta = (Double_t) ((MC1->GetPz() + MC2->GetPz())/(MC1->GetE() + MC2->GetE()));
  TLorentzVector M1 = T49Tool::zLorentz(MC1,MC1->GetMass(),MCBeta) ;
  TLorentzVector M2 = T49Tool::zLorentz(MC2,MC2->GetMass(),MCBeta) ;
  
  Double_t ktPAIR = TMath::Sqrt((M1.X()+M2.X())*(M1.X()+M2.X())
				+(M1.Y()+M2.Y())*(M1.Y()+M2.Y())) ;
  Double_t MCqside = ((M1.X()-M2.X())*(M1.Y()+M2.Y())
		      -(M1.Y()-M2.Y())*(M1.X()+M2.X())) / ktPAIR ; // Qside              
  Double_t MCqout  = (M1.X()*M1.X()-M2.X()*M2.X() 
		      +M1.Y()*M1.Y()-M2.Y()*M2.Y())     / ktPAIR ; // Qout
  Double_t MCqlong =  M1.Z() - M2.Z() ;                            // Qlong

  Double_t YPAIR   = 0.5 * TMath::Log( (MC1->GetE()+MC1->GetPz()+MC2->GetE()+MC2->GetPz()) / 
				       (MC1->GetE()-MC1->GetPz()+MC2->GetE()-MC2->GetPz()) ); 
  
  Int_t mReverseOrder = 0 ;
  if(MCqside < 0.0)
    {
      MCqside = MCqside * (-1.0) ;
      MCqout  = MCqout  * (-1.0) ;
      MCqlong = MCqlong * (-1.0) ;
      mReverseOrder = 1 ;
    }
  
  // Fill only if MCqside, MCqout & MCqlong are in the range of 3d MomentumDifferenceOfMCTracks
  if( MCqside > mMomentumDifferenceOfMCTracks->GetXaxis()->GetXmin() && MCqside < mMomentumDifferenceOfMCTracks->GetXaxis()->GetXmax() &&
      MCqout > mMomentumDifferenceOfMCTracks->GetYaxis()->GetXmin() && MCqout < mMomentumDifferenceOfMCTracks->GetYaxis()->GetXmax() &&
      MCqlong > mMomentumDifferenceOfMCTracks->GetZaxis()->GetXmin() && MCqlong < mMomentumDifferenceOfMCTracks->GetZaxis()->GetXmax() )
    {
      
      // Get RC tracks
      T49ParticleRoot* RE1 = (T49ParticleRoot*) mSelectedRETracks->At(mSelectedMCTracks->IndexOf(MC1)) ; 
      T49ParticleRoot* RE2 = (T49ParticleRoot*) mSelectedRETracks->At(mSelectedMCTracks->IndexOf(MC2)) ; 
      // Calculate RC momentum difference
      Double_t REBeta = (Double_t) ((RE1->GetPz() + RE2->GetPz())/(RE1->GetE(mPionMass) + RE2->GetE(mPionMass)));
      TLorentzVector R1 = T49Tool::zLorentz(RE1,mPionMass,REBeta) ;
      TLorentzVector R2 = T49Tool::zLorentz(RE2,mPionMass,REBeta) ;
      
      Double_t REktPAIR = TMath::Sqrt((R1.X()+R2.X())*(R1.X()+R2.X())
				      +(R1.Y()+R2.Y())*(R1.Y()+R2.Y())) ;
      Double_t REqside = ((R1.X()-R2.X())*(R1.Y()+R2.Y())
			  -(R1.Y()-R2.Y())*(R1.X()+R2.X())) / REktPAIR ; // Qside              
      Double_t REqout  = (R1.X()*R1.X()-R2.X()*R2.X() 
			  +R1.Y()*R1.Y()-R2.Y()*R2.Y())     / REktPAIR ; // Qout
      Double_t REqlong =  R1.Z() - R2.Z() ;                            // Qlong
      
      if(mReverseOrder == 1) 
	{
	  REqside = REqside  * (-1.0) ;
	  REqout  = REqout   * (-1.0) ;
	  REqlong = REqlong  * (-1.0) ;
	} 

      //
      // Fill in Clonesarray at the appropriate position
      //
      
      // Get appropriate position
      Int_t GeneralIndex = mMomentumDifferenceOfMCTracks->FindBin(MCqside,MCqout,MCqlong) ;
      
      // Create TVector3 with the momentum difference
      TVector3* MomDiffReconstructed = new TVector3((REqside-MCqside),(REqout-MCqout),(REqlong-MCqlong)) ;
      
      // If we do not have a TObjArray for this position yet, create it
      Int_t lKtYIndex = GetKtYIndex(ktPAIR,YPAIR) ;
      if(lKtYIndex>=0)
	{
	  if(!((TObjArray*)mListOfListOfDisplacedMomenta->At(lKtYIndex))->At(GeneralIndex))
	    {
	      TObjArray* aNewObjArray = new TObjArray(10) ;
	      ((TObjArray*)mListOfListOfDisplacedMomenta->At(lKtYIndex))->AddAt(aNewObjArray, GeneralIndex) ;
	    }
	  
	  // Add it
	  ((TObjArray*)((TObjArray*) mListOfListOfDisplacedMomenta->At(lKtYIndex))->At(GeneralIndex))->Add(MomDiffReconstructed) ;
	}
    } // if in range of 3d histo
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int_t T49HbtMomentumResolutionCalculator::GetKtYIndex(Double_t lKtPair, Double_t lYPair)
{
  // Kt
  Double_t ktIndex = -1.0 ;
  for(Int_t index = 0 ; index < mNKtBins ; index++) 
    {
      if(lKtPair>mKtBins[index] && lKtPair<mKtBins[index+1])
	{
	  ktIndex = index ;
	}
    }
  // Y
  Double_t yIndex = -1.0 ;
  for(Int_t index = 0 ; index < mNYBins ; index++) 
    {
      if(lYPair>mYBins[index] && lYPair<mYBins[index+1])
	{
	  yIndex = index ;
	}
    }
  // return kty bin, if out of bonds negative value
  return ((Int_t) (ktIndex+mNKtBins*yIndex)) ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TObjArray* T49HbtMomentumResolutionCalculator::GetResolution3dHisots()
{
  // Calculate and return resulting resolution histograms

  // Loop over kt-Y bins
  for(Int_t lKtYIndex = 0 ; lKtYIndex < mMaxKtY ; lKtYIndex++ )
    {
      // Loop over all cells in the master 3d histo
      for(Int_t GeneralIndex = 0 ; GeneralIndex < mNumberOfCells ; GeneralIndex++)
	{
	  // If a TObjArray exists
	  if(((TObjArray*)mListOfListOfDisplacedMomenta->At(lKtYIndex))->At(GeneralIndex))
	    {
	      // Get TObjArray of displaxed momenta
	      TObjArray* currentObjArray = (TObjArray*) ((TObjArray*) mListOfListOfDisplacedMomenta->At(lKtYIndex))->At(GeneralIndex) ;

	      // Where are we in the master 3d histo ?
	      /*
		Int_t nbinsX = mMomentumDifferenceOfMCTracks->GetNbinsX()+2 ;
		Int_t nbinsY = mMomentumDifferenceOfMCTracks->GetNbinsY()+2 ;
		Int_t nbinsZ = mMomentumDifferenceOfMCTracks->GetNbinsZ()+2 ;
		Int_t binx = GeneralIndex%nbinsX;
		Int_t biny = ((GeneralIndex-binx)/nbinsX)%nbinsY ;
		Int_t binz = (((GeneralIndex-binx)/nbinsX)-biny)/nbinsZ ;
		cout << "working on bin:  qside: " << mMomentumDifferenceOfMCTracks->GetXaxis()->GetBinCenter(binx) << "\t" ;
		cout << "qout: " << mMomentumDifferenceOfMCTracks->GetYaxis()->GetBinCenter(biny) << "\t" ;
		cout << "qlong: " << mMomentumDifferenceOfMCTracks->GetZaxis()->GetBinCenter(binz) << "\t" << endl;
	      */

	      // If vector for this bin has more than 3 entries 
	      if(currentObjArray->GetEntries() > 3)
		{
		  // Loop over vector to calculate mean shift
		  Double_t MeanQSide     = 0.0 ;
		  Double_t MeanQOut      = 0.0 ;
		  Double_t MeanQLong     = 0.0 ;
		  Double_t MeanCounter   = 0.0 ;
		  for(Int_t VectorIndex = 0 ; VectorIndex < currentObjArray->GetEntries() ; VectorIndex++)
		    {
		      MeanCounter++ ;
		      MeanQSide += ((TVector3*) (currentObjArray->At(VectorIndex)))->Px() ;
		      MeanQOut  += ((TVector3*) (currentObjArray->At(VectorIndex)))->Py() ;
		      MeanQLong += ((TVector3*) (currentObjArray->At(VectorIndex)))->Pz() ;
		      
		      // Some output, for debugging only
		      /*
			Double_t a = (Double_t) ((TVector3*) (currentObjArray->At(VectorIndex)))->Px() ;
			TVector3* c = ((TVector3*) (currentObjArray->At(VectorIndex))) ;
			Int_t b = currentObjArray->GetEntries() ;
			cout << "qsideshift "  << ((TVector3*) (currentObjArray->At(VectorIndex)))->Px() << "\t" ; 
			cout << "qoutshift  " << ((TVector3*) (currentObjArray->At(VectorIndex)))->Py() << "\t";
			cout << "qlongshift " << ((TVector3*) (currentObjArray->At(VectorIndex)))->Pz()<< endl  ;
		      */
		    } 
		  MeanQSide = MeanQSide / MeanCounter ;
		  MeanQOut  = MeanQOut  / MeanCounter ;
		  MeanQLong = MeanQLong / MeanCounter ;
		  
		  // Fill mean shift into 3d histo
		  ((TH3D*) mResolution->At(lKtYIndex*6))->SetBinContent(GeneralIndex,MeanQSide) ;
		  ((TH3D*) mResolution->At(lKtYIndex*6+1))->SetBinContent(GeneralIndex,MeanQOut) ;
		  ((TH3D*) mResolution->At(lKtYIndex*6+2))->SetBinContent(GeneralIndex,MeanQLong) ;
		  		  
		  // Loop over vector to calculate RMS
		  Double_t RMSQSide     = 0.0 ;
		  Double_t RMSQOut      = 0.0 ;
		  Double_t RMSQLong     = 0.0 ;
		  Double_t RMSCounter   = 0.0 ;
		  for(Int_t VectorIndex = 0 ; VectorIndex < currentObjArray->GetEntries() ; VectorIndex++)
		    {
		      RMSQSide += pow((((TVector3*) (currentObjArray->At(VectorIndex)))->Px() -  MeanQSide),2) ;
		      RMSQOut  += pow((((TVector3*) (currentObjArray->At(VectorIndex)))->Py() -  MeanQOut),2)  ; 
		      RMSQLong += pow((((TVector3*) (currentObjArray->At(VectorIndex)))->Pz() -  MeanQLong),2) ;
		      RMSCounter++ ;
		      // Some output, for debugging only
		      /*
			Double_t a = (Double_t) ((TVector3*) (currentObjArray->At(VectorIndex)))->Px() ;
			TVector3* c = ((TVector3*) (currentObjArray->At(VectorIndex))) ;
			Int_t b = currentObjArray->GetEntries() ;
			cout << "qsideRMS "  << pow((((TVector3*) (currentObjArray->At(VectorIndex)))->Px() -  MeanQSide),2)   << "\t" ; 
			cout << "qoutRMS  " << pow((((TVector3*) (currentObjArray->At(VectorIndex)))->Py() -  MeanQOut),2)  << "\t";
			cout << "qlongRMS " << pow((((TVector3*) (currentObjArray->At(VectorIndex)))->Pz() -  MeanQLong),2)  << endl  ;
		      */
		} 
		  RMSQSide = sqrt(RMSQSide / RMSCounter) ;
		  RMSQOut  = sqrt(RMSQOut  / RMSCounter) ;
		  RMSQLong = sqrt(RMSQLong / RMSCounter) ;
		  
		  // Fill mean into 3d histo
		  ((TH3D*) mResolution->At(lKtYIndex*6+3))->SetBinContent(GeneralIndex,RMSQSide) ;
		  ((TH3D*) mResolution->At(lKtYIndex*6+4))->SetBinContent(GeneralIndex,RMSQOut) ;
		  ((TH3D*) mResolution->At(lKtYIndex*6+5))->SetBinContent(GeneralIndex,RMSQLong) ;
		  
		} // if Entries > 0
	    } // if TObjArray
	} // loop over 3d master histo
    } // loop over kty index
  // Return ObjArray with the Result 3d histos
  return mResolution ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCalculator::PrintResolution3dHisots(Int_t lKtYIndex)
{
  // This member function plots for a given kty bin the mean and rms of each cell 
  // along with the cells position in the master TH3D.
  // If commented out, also the vector is plotted from which rms and mean are derived

  // Make sure the resolution histos are filled
  this->GetResolution3dHisots() ;
  
  // Loop over all cells in the master 3d histo
  for(Int_t GeneralIndex = 0 ; GeneralIndex < mNumberOfCells ; GeneralIndex++)
    {
      // If a TObjArray exists
      if(((TObjArray*) mListOfListOfDisplacedMomenta->At(lKtYIndex))->At(GeneralIndex))
	{
	  TObjArray* currentObjArray = (TObjArray*) ((TObjArray*)mListOfListOfDisplacedMomenta->At(lKtYIndex))->At(GeneralIndex) ;
	  if(currentObjArray->GetEntries()>3)
	    {
	      // Find the qout,qside,qlong value in the master 3d histo
	      Int_t nbinsX = mMomentumDifferenceOfMCTracks->GetNbinsX()+2 ;
	      Int_t nbinsY = mMomentumDifferenceOfMCTracks->GetNbinsY()+2 ;
	      Int_t nbinsZ = mMomentumDifferenceOfMCTracks->GetNbinsZ()+2 ;
	      Int_t binx = GeneralIndex%nbinsX;
	      Int_t biny = ((GeneralIndex-binx)/nbinsX)%nbinsY ;
	      Int_t binz = (((GeneralIndex-binx)/nbinsX)-biny)/nbinsZ ;
	      
	      cout << "General index : " << "\t" <<  GeneralIndex 
		   << "\t must equal \t" << mMomentumDifferenceOfMCTracks->GetBin(binx,biny,binz) << "\t" ;
	      cout << "\t bin x,y,z: \t " << binx  << "\t" << biny  << "\t" << binz  << "\t"  << endl;
	      cout << "qside: " << mMomentumDifferenceOfMCTracks->GetXaxis()->GetBinCenter(binx) << "\t" ;
	      cout << "qout: " << mMomentumDifferenceOfMCTracks->GetYaxis()->GetBinCenter(biny) << "\t" ;
	      cout << "qlong: " << mMomentumDifferenceOfMCTracks->GetZaxis()->GetBinCenter(binz) << "\t" << endl;
	     	     
	      cout << "Mean qside : " << ((TH3D*) mResolution->At(lKtYIndex*6))->GetBinContent(GeneralIndex) << "\t"; 
	      cout << "Mean qOut : "  << ((TH3D*) mResolution->At(lKtYIndex*6+1))->GetBinContent(GeneralIndex) << "\t"; 
	      cout << "Mean qLong : " << ((TH3D*) mResolution->At(lKtYIndex*6+2))->GetBinContent(GeneralIndex) << "\t"; 
	      cout << "RMS qside : "  << ((TH3D*) mResolution->At(lKtYIndex*6+3))->GetBinContent(GeneralIndex) << "\t";
	      cout << "RMS qOut : "   << ((TH3D*) mResolution->At(lKtYIndex*6+4))->GetBinContent(GeneralIndex) << "\t";
	      cout << "RMS qLong : "  << ((TH3D*) mResolution->At(lKtYIndex*6+5))->GetBinContent(GeneralIndex) << "\t" << endl ;
	     
	      // Show vector, which was used to calculate mean and rms
	      cout << "Number of entries : " << currentObjArray->GetEntries() << endl ;
	      for(Int_t VectorIndex = 0 ; VectorIndex < currentObjArray->GetEntries() ; VectorIndex++)
		{
		  //Double_t a = (Double_t) ((TVector3*) (currentObjArray->At(VectorIndex)))->Px() ;
		  //TVector3* c = ((TVector3*) (currentObjArray->At(VectorIndex))) ;
		  //Int_t b = currentObjArray->GetEntries() ;
		  cout << "\t qside "  << ((TVector3*) (currentObjArray->At(VectorIndex)))->Px() << "\t" ; 
		  cout << "qout  " << ((TVector3*) (currentObjArray->At(VectorIndex)))->Py() << "\t";
		  cout <<"qlong " << ((TVector3*) (currentObjArray->At(VectorIndex)))->Pz()<< endl  ;
		}
	    }
	}
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TObjArray* T49HbtMomentumResolutionCalculator::GetResolutionDistributions(Int_t lKtYIndex)
{
  // This member function returns 6 TH1D's filled with the mean and the rms values for qSide, qOut & qLong of all cells 
  // of a given kty bin
  
  // Make sure the resolution histos are filled
  this->GetResolution3dHisots() ;

  // Create object array holding 6 distribution histograms
  TObjArray* mResolutionDistribution = new TObjArray(6) ;
  TH1D* MeanRsideDistribution = new TH1D("MeanRsideDistribution","MeanRsideDistribution",1000,-0.05,0.05) ;
  mResolutionDistribution->AddAt(MeanRsideDistribution,0) ;
  TH1D* MeanRoutDistribution = new TH1D("MeanRoutDistribution","MeanRoutDistribution",1000,-0.05,0.05) ;
  mResolutionDistribution->AddAt(MeanRoutDistribution,1) ;
  TH1D* MeanRlongDistribution = new TH1D("MeanRlongDistribution","MeanRlongDistribution",1000,-0.05,0.05) ;
  mResolutionDistribution->AddAt(MeanRlongDistribution,2) ;
  TH1D* RMSRsideDistribution = new TH1D("RMSRsideDistribution","RMSRsideDistribution",1000,0.0,0.05) ;
  mResolutionDistribution->AddAt(RMSRsideDistribution,3) ;
  TH1D* RMSRoutDistribution = new TH1D("RMSRoutDistribution","RMSRoutDistribution",1000,0.0,0.05) ;
  mResolutionDistribution->AddAt(RMSRoutDistribution,4) ;
  TH1D* RMSRlongDistribution = new TH1D("RMSRlongDistribution","RMSRlongDistribution",1000,0.0,0.05) ;
  mResolutionDistribution->AddAt(RMSRlongDistribution,5) ;
  
  // Loop over all cells in the master 3d histo and fill the 6 distribution histograms
  for(Int_t GeneralIndex = 0 ; GeneralIndex < mNumberOfCells ; GeneralIndex++)
    {
      MeanRsideDistribution->Fill(((TH3D*) mResolution->At(lKtYIndex*6))->GetBinContent(GeneralIndex)) ;
      MeanRoutDistribution->Fill(((TH3D*) mResolution->At(lKtYIndex*6+1))->GetBinContent(GeneralIndex)) ;
      MeanRlongDistribution->Fill(((TH3D*) mResolution->At(lKtYIndex*6+2))->GetBinContent(GeneralIndex)) ;
      
      RMSRsideDistribution->Fill(((TH3D*) mResolution->At(lKtYIndex*6+3))->GetBinContent(GeneralIndex)) ;
      RMSRoutDistribution->Fill(((TH3D*) mResolution->At(lKtYIndex*6+4))->GetBinContent(GeneralIndex)) ;
      RMSRlongDistribution->Fill(((TH3D*) mResolution->At(lKtYIndex*6+5))->GetBinContent(GeneralIndex)) ;
    }

  // Return Obj array
  return mResolutionDistribution ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionCalculator::SaveResolution(Char_t* Name)
{
  //
  // Store the calculated resolution TH3Ds in a root file, 
  // to have easy access using class T49HbtMomentumResolutionProvider
  //

  // Create TGraphs holding information about kt-Y bins
  Double_t lDummy[mNKtBins+mNYBins] ;
  TGraph* lKtBins = new TGraph(mNKtBins+1,mKtBins,lDummy) ;
  lKtBins->SetName("KtBins");
  TGraph* lYBins =  new TGraph(mNYBins+1,mYBins,lDummy) ;
  lYBins->SetName("YBins");

  // Open resuls to file
  TFile mResultFile(Name,"recreate") ;
  lKtBins->Write() ;
  lYBins->Write() ;
  mMomentumDifferenceOfMCTracks->Write() ;
  mResolution->Write("resultArray",1) ;
  mResultFile.Close() ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
T49HbtMomentumResolutionProvider::T49HbtMomentumResolutionProvider(Char_t* Name)
{
  // Open result file
  TFile* mResultFile = new TFile(Name,"read") ;
  
  // Get number and size of kt-Y bins 
  // Kt
  TGraph* lgKtBins = (TGraph*) mResultFile->Get("KtBins") ;
  mNKtBins = lgKtBins->GetN()-1 ;
  mKtBins = new Double_t[mNKtBins+1] ; 
  Double_t* lKtBins = lgKtBins->GetX() ;
  for(Int_t index = 0 ; index < mNKtBins+1 ; index++)  mKtBins[index] = lKtBins[index] ;
  // Y
  TGraph* lgYBins = (TGraph*) mResultFile->Get("YBins") ;
  mNYBins = lgYBins->GetN()-1 ;
  mYBins = new Double_t[mNYBins+1] ; 
  Double_t* lYBins = lgYBins->GetX() ;
  for(Int_t index = 0 ; index < mNYBins+1 ; index++)  mYBins[index] = lYBins[index] ;
 
  // Get TObjArray with resolution histos 
  mResolution = (TObjArray*) mResultFile->Get("resultArray") ;

  // Get master 3d Histo
  mMaster3d = (TH3D*) mResultFile->Get("Master3d") ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int_t T49HbtMomentumResolutionProvider::GetKtYIndex(Double_t lKtPair, Double_t lYPair)
{
  // Kt
  Double_t ktIndex = -1.0 ;
  for(Int_t index = 0 ; index < mNKtBins ; index++) 
    {
      if(lKtPair>=mKtBins[index] && lKtPair<mKtBins[index+1])
	{
	  ktIndex = index ;
	}
    }
  // Y
  Double_t yIndex = -1.0 ;
  for(Int_t index = 0 ; index < mNYBins ; index++) 
    {
      if(lYPair>=mYBins[index] && lYPair<mYBins[index+1])
	{
	  yIndex = index ;
	}
    }
  // return kty bin, if out of bonds negative value
  if((ktIndex+mNKtBins*yIndex)>24)
    {
      ktIndex = -1.0 ;
      cerr <<"This should not happen !" << endl ; 
    }

  return ((Int_t) (ktIndex+mNKtBins*yIndex)) ;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void T49HbtMomentumResolutionProvider::GetMeanAndRMS(Double_t ktPair, Double_t YPair, 
						     Double_t qside, Double_t qout, Double_t qlong,
						     Double_t& MeanQside, Double_t& MeanQout, Double_t& MeanQlong,
						     Double_t& RMSQside , Double_t& RMSQout , Double_t& RMSQlong ) 
  
{
  // Get index of this qout-qside-qlong bin
  Int_t lGeneralIndex = mMaster3d->FindBin(qside,qout,qlong) ;
  // Get kt-Y index of the pair
  Int_t lKtYIndex = this->GetKtYIndex(ktPair,YPair) ;
  if(lKtYIndex>=0)
    {
      // Fill the requested Mean and RMS values
      MeanQside = ((TH3D*) mResolution->At(lKtYIndex*6))->GetBinContent(lGeneralIndex) ;
      MeanQout  = ((TH3D*) mResolution->At(lKtYIndex*6+1))->GetBinContent(lGeneralIndex) ;
      MeanQlong = ((TH3D*) mResolution->At(lKtYIndex*6+2))->GetBinContent(lGeneralIndex) ;
      RMSQside  = ((TH3D*) mResolution->At(lKtYIndex*6+3))->GetBinContent(lGeneralIndex) ;
      RMSQout   = ((TH3D*) mResolution->At(lKtYIndex*6+4))->GetBinContent(lGeneralIndex) ;
      RMSQlong  = ((TH3D*) mResolution->At(lKtYIndex*6+5))->GetBinContent(lGeneralIndex) ;
      if(RMSQside<0 || RMSQout<0 || RMSQlong<0)
	{
	  cerr << "This should never happen ! "<< endl ;
	}
    }
  else
    {
      MeanQside = 0.0 ;
      MeanQout  = 0.0 ;
      MeanQlong = 0.0 ;
      RMSQside  = 0.0 ;
      RMSQout   = 0.0 ;
      RMSQlong  = 0.0 ;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

