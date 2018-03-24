#ifdef __CINT__

/*
$Log: T49ANALinkDef.h,v $
Revision 1.17  2006/07/28 09:52:47  cblume
Add new T49MultiMixer class

Revision 1.16  2004/11/09 16:04:08  flierl
add T49HbtMomentumResolutionCorrection

Revision 1.15  2004/09/15 14:34:12  flierl
Add the new HBT classes for fitting

Revision 1.14  2002/11/22 09:31:15  cblume
Add T49Padtrans

Revision 1.13  2002/10/07 09:55:29  cblume
Update for CASTOR

Revision 1.12  2001/11/21 17:01:05  cblume
Add T49Proj2K

Revision 1.11  2001/11/12 11:36:35  cblume
Update for ROOT v3.01

 * Revision 1.10  2000/11/09  09:10:08  cblume
 * Add T49Trkstep
 *
 * Revision 1.9  2000/10/09  10:24:51  cblume
 * General update
 *
 * Revision 1.8  2000/07/07  14:16:03  cblume
 * Add T49OnePeak and T49SumGaus, Streamer for T49DedxInfo
 *
 * Revision 1.7  2000/06/09  14:56:05  cblume
 * Customized streamer for T49Container
 *
 * Revision 1.6  2000/01/17  12:40:01  cblume
 * Added T49Container T49CutPid T49DedxFunction T49DedxInfo T49Index
 *
 * Revision 1.5  1999/11/24  16:03:32  cblume
 * Fixes in gteval_nt_root.h and T49Mixer. Addrd T49Dedx again in T49ANA. Some cosmetics.
 *
 * Revision 1.4  1999/11/23  13:52:08  cblume
 * Remove files
 *
*/

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ global gT49Mixer;

#pragma link C++ class T49InMuDST+;
#pragma link C++ class T49Cut+;
#pragma link C++ class T49CutEvent+;
#pragma link C++ class T49CutTrack-;
#pragma link C++ class T49Cut2Track+;
#pragma link C++ class T49CutV0-;
#pragma link C++ class T49CutPid+;
#pragma link C++ class T49Container-;
#pragma link C++ class T49Dedx+;
#pragma link C++ class T49DedxFunction+;
#pragma link C++ class T49DedxInfo-;
#pragma link C++ class T49Prob+;
#pragma link C++ class T49Tool+;
#pragma link C++ class T49Mixing+;
#pragma link C++ class T49Mixer+;
#pragma link C++ class T49MultiMixer+;
#pragma link C++ class T49Proj1BASE+;
#pragma link C++ class T49Proj1Dedx+;
#pragma link C++ class T49Proj1Prob+;
#pragma link C++ class T49Proj2BASE+;
#pragma link C++ class T49Proj2BP+;
#pragma link C++ class T49Proj2YKP+;
#pragma link C++ class T49Proj2QtQl+;
#pragma link C++ class T49Proj2QINV+;
#pragma link C++ class T49Proj2QINV2+;
#pragma link C++ class T49Proj2INVmass+;
#pragma link C++ class T49Proj2Armenteros+;
#pragma link C++ class T49Proj2K+;
#pragma link C++ class T49Histogram+;
#pragma link C++ class T49Histogram1D+;
#pragma link C++ class T49Histogram2D+;
#pragma link C++ class T49Histogram3D+;
#pragma link C++ class T49Index+;
#pragma link C++ class T49OnePeak+;
#pragma link C++ class T49SumGaus+;
#pragma link C++ class T49Trkstep+;
#pragma link C++ class T49Padtrans+;
#pragma link C++ class T49XiFinder+;
#pragma link C++ class T49HbtMomentumResolutionCalculator+;
#pragma link C++ class T49HbtMomentumResolutionProvider+;
#pragma link C++ class T49HbtMomentumResolutionCorrection+;
#pragma link C++ class T49HbtCoulomb+;
#pragma link C++ class T49HbtFitBase+;
#pragma link C++ class T49FitProjector+;
#pragma link C++ class T49HbtFit_BP_LS_CHI2+;
#pragma link C++ class T49HbtFit_BP_LS_MML+;
#pragma link C++ class T49HbtFit_BP_ULS_CHI2+;
#pragma link C++ class T49HbtFit_YKP_LS_CHI2+;
#pragma link C++ class T49dEdxFunctions;

#endif
