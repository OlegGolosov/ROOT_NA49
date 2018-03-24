///////////////////////////////////////////////////////////////////////
//                                                                   //
//    T49Trkstep provides an interface to NA49-trkstep.              //
//    It allows to extrapolate a track defined by its momentum       //
//    components by swimming through the magnetic field.             //
//    The field is read from the NA49 database, which requires that  //
//    the appropriate keys are set.                                  //
//                                                                   //
//    An example application might look like this:                   //
//                                                                   //
//       T49Trkstep *T = new T49Trkstep();                           //
//       T->SetKeys("1458","RU96","V96C","COMB/STD+","FULL","R95A"); //
//       T->InitDS("trkstep");                                       //
//       T->SetCharge(1);                                            //
//       T->SetPos(0,0,-580);                                        //
//       T->SetP(0.1,0.05,10.0);                                     //
//       T->TrackTo(-570);                                           //
//       T->GetZ();                                                  //
//                                                                   //
//    Based on TMvlTracker by Marco van Leeuwen                      //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include <cstdio>

#include <TSystem.h>

#ifndef NODS
#include "dspack_rootds.h"
#endif

#include "T49Trkstep.h"

ClassImp(T49Trkstep)

//______________________________________________________________________________
T49Trkstep::T49Trkstep()
{
  //
  // T49Trkstep default constructor
  // Initializes T49Trkstep with the adaptive step size algorithm,
  // field boundary checking, and track charge = +1.
  //

  fFixStep  = 0;
  fStepSize = 0;
  fCheck    = 0;
  fCharge   = 1;

}

//______________________________________________________________________________
void T49Trkstep::SetKeys(Char_t *key1, Char_t *key3, Char_t *key4
                       , Char_t *key5, Char_t *key6, Char_t *key7)
{ 
  //
  // Sets the keys 
  //

  gSystem->Setenv("KEY1",key1);
  gSystem->Setenv("KEY3",key3);
  gSystem->Setenv("KEY4",key4);
  gSystem->Setenv("KEY5",key5);
  gSystem->Setenv("KEY6",key6);
  gSystem->Setenv("KEY7",key7);

}

//______________________________________________________________________________
void T49Trkstep::SetKeys96STDp()
{ 
  //
  // Sets the default keys for the 96 std+ central Pb+Pb data
  //

  gSystem->Setenv("KEY1","1458");
  gSystem->Setenv("KEY3","RU96");
  gSystem->Setenv("KEY4","V96C");
  gSystem->Setenv("KEY5","COMB/STD+");
  gSystem->Setenv("KEY6","FULL");
  gSystem->Setenv("KEY7","R95A");

}

//______________________________________________________________________________
Int_t T49Trkstep::InitDS(Char_t *dspack_server)
{
  //
  // Initializes the database
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Trkstep::InitDS> Sorry, DSPACK library not available.\n");

#else

  Char_t *argv[1] = {"tracker"};

  printf("<T49Trkstep::InitDS> Trying to initialize %s\n",dspack_server);
  if (init_na49db (dspack_server, argv, &ierr)) {
    printf ("<T49Trkstep::InitDS> init_na49db returned %d\n",ierr);
    return ierr;
  }

#endif

  return ierr;

}

//______________________________________________________________________________
Int_t T49Trkstep::TrackTo(Float_t z)
{
  //
  // Tracks a given track to the chosen z-position.
  // Return values:  0 = successful
  //                 1 = track left magnetic field map
  //                 2 = too many steps ( maximum 500 )
  //                 3 = track changed direction ( spiral track )
  //                 4 = could not get bmap bounds from HEPDB
  //                 5 = zero input momentum
  //

  Int_t ierr = 0;

#ifdef NODS

  printf("<T49Trkstep::TrackTo> Sorry, DSPACK library not available.\n");

#else

  fXstop[2] = z;
  trkstep_(&fCharge, fXstart, fPstart, fXstop, fPstop
         , &fFixStep, &fStepSize, &fCheck, &ierr);

  fPstart[0] = fPstop[0];
  fPstart[1] = fPstop[1];
  fPstart[2] = fPstop[2];
  fXstart[0] = fXstop[0];
  fXstart[1] = fXstop[1];
  fXstart[2] = fXstop[2];

#endif

  return ierr;

}

//______________________________________________________________________________
void T49Trkstep::SetP(Float_t px, Float_t py, Float_t pz)
{
  //
  // Sets the momentum components of the track
  //

  fPstart[0] = px;
  fPstart[1] = py;
  fPstart[2] = pz;
  fPstop[0]  = fPstart[0];
  fPstop[1]  = fPstart[1];
  fPstop[2]  = fPstart[2];

}

//______________________________________________________________________________
void T49Trkstep::SetPos(Float_t x, Float_t y, Float_t z)
{
  //
  // Sets the starting position of the track
  //

  fXstart[0] = x;
  fXstart[1] = y;
  fXstart[2] = z;
  fXstop[0]  = fXstart[0];
  fXstop[1]  = fXstart[1];
  fXstop[2]  = fXstart[2];

}






