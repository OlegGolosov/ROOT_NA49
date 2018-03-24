///////////////////////////////////////////////////////////////////////
//                                                                   //
//  T49 wrapper class to padtrans.                                   //
//  It uses the drift velocities from as default                     //
//      $T49/inc/padtrans_init.d                                     //
//  To change to another init file use SetInitFile().                //
//                                                                   //
//  Example:                                                         //
//                                                                   //
//  T49Padtrans *pt = new T49Padtrans();                             //
//  pt->SetKeys("1458","RU96","V96C","COMB/STD+","FULL","R95A");     //
//  pt->InitDS("padtrans");                                          //
//  Double_t xext;                                                   //
//  Double_t yext;                                                   //
//  Double_t zext;                                                   //
//  pt->Int2ext(1,10,-10,10,&xext,&yext,&zext);                      //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include <cstdio>

#include <TSystem.h>

#ifndef NODS
#include "dspack_rootds.h"
#endif

#include "T49Padtrans.h"

ClassImp(T49Padtrans)

//______________________________________________________________________________
T49Padtrans::T49Padtrans()
{
  //
  // T49Padtrans default constructor
  //

  // Use default init file
  Char_t filename[255];
  sprintf(filename,"%s/inc/padtrans_init.d",gSystem->Getenv("T49"));
  gSystem->Setenv("PADTRANS_INIT",filename);

}

//______________________________________________________________________________
void T49Padtrans::SetKeys(Char_t *key1, Char_t *key3, Char_t *key4
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
void T49Padtrans::SetInitFile(Char_t *filename)
{
  //
  // Set the name of the init file for the drift velocities
  //

  gSystem->Setenv("PADTRANS_INIT",filename);

}

//______________________________________________________________________________
void T49Padtrans::SetVerbose(Int_t v)
{
  //
  // Switch to verbose mode
  //

#ifdef NODS

  printf("<T49Padtrans::SetVerbose> Sorry, DSPACK library not available.\n");

#else

  set_padtrans_verbose(v); 

#endif

}

//______________________________________________________________________________
Int_t T49Padtrans::InitDS(Char_t *dspack_server)
{
  //
  // Initializes the database
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Padtrans::InitDS> Sorry, DSPACK library not available.\n");

#else

  Char_t *argv[1] = {"tracker"};

  printf("<T49Padtrans::InitDS> Trying to initialize %s\n",dspack_server);
  if (init_na49db (dspack_server, argv, &ierr)) {
    printf ("<T49Padtrans::InitDS> init_na49db returned %d\n",ierr);
    return ierr;
  }

#endif

  return ierr;

}

//______________________________________________________________________________
Int_t T49Padtrans::Pad2int(Int_t detector, Int_t sector
                         , Int_t row, Double_t pad, Double_t time
                         , Double_t *xint, Double_t *yint, Double_t *zint)
{
  //
  // pad/time -> internal
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Padtrans::Pad2int> Sorry, DSPACK library not available.\n");

#else

  ierr = pad2int(detector,sector,row,pad,time,xint,yint,zint);

#endif

  return ierr;

}

//______________________________________________________________________________
Int_t T49Padtrans::Int2pad(Int_t detector
                         , Double_t xint, Double_t yint, Double_t zint
                         , Int_t *sector
                         , Int_t *row, Double_t *pad, Double_t *time)
{
  //
  // internal -> pad/time
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Padtrans::Int2pad> Sorry, DSPACK library not available.\n");

#else

  ierr = int2pad(detector,xint,yint,zint,sector,row,pad,time);

#endif

  return ierr;

}

//______________________________________________________________________________
Int_t T49Padtrans::Int2ext(Int_t detector
                         , Double_t xint, Double_t yint, Double_t zint
                         , Double_t *xext, Double_t *yext, Double_t *zext)
{
  //
  // internal -> external
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Padtrans::Int2ext> Sorry, DSPACK library not available.\n");

#else

  ierr = int2ext(detector,xint,yint,zint,xext,yext,zext);  

#endif

  return ierr;

}

//______________________________________________________________________________
Int_t T49Padtrans::Ext2int(Int_t detector
                         , Double_t xext, Double_t yext, Double_t zext
                         , Double_t *xint, Double_t *yint, Double_t *zint)
{
  //
  // extrernal -> internal
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Padtrans::Ext2int> Sorry, DSPACK library not available.\n");

#else

  ierr = ext2int(detector,xext,yext,zext,xint,yint,zint);    

#endif

  return ierr;

}

//______________________________________________________________________________
Int_t T49Padtrans::Int2sr(Int_t detector
                        , Double_t xint, Double_t yint, Double_t zint
                        , Int_t *sector, Int_t *row)
{
  //
  // internal -> sector/row 
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Padtrans::Int2sr> Sorry, DSPACK library not available.\n");

#else

  ierr = int2sr(detector,xint,yint,zint,sector,row);     

#endif

  return ierr;

}

//______________________________________________________________________________
Int_t T49Padtrans::Int2drow(Int_t detector
                          , Double_t xint, Double_t yint, Double_t zint
                          , Int_t *sector
                          , Double_t *row, Double_t *pad, Double_t *time)
{
  //
  // internal -> detector pad/time
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Padtrans::Int2drow> Sorry, DSPACK library not available.\n");

#else

  ierr = int2drow(detector,xint,yint,zint,sector,row,pad,time);  

#endif

  return ierr;

}

//______________________________________________________________________________
Int_t T49Padtrans::Drow2int(Int_t detector, Int_t sector
                          , Double_t row, Double_t pad, Double_t time
                          , Double_t *xint, Double_t *yint, Double_t *zint)
{
  //
  // detector pad/time -> internal
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Padtrans::Drow2int> Sorry, DSPACK library not available.\n");

#else

  ierr = drow2int(detector,sector,row,pad,time,xint,yint,zint);   

#endif

  return ierr;

}

//______________________________________________________________________________
Double_t T49Padtrans::Pad2channel(Int_t detector, Int_t sector
                                , Int_t row, Double_t pad)
{
  //
  // pad -> adc channel
  //

#ifdef NODS

  printf("<T49Padtrans::Pad2channel> Sorry, DSPACK library not available.\n");
  return 0.0;

#else

  return pad2channel(detector,sector,row,pad); 

#endif

}

//______________________________________________________________________________
Double_t T49Padtrans::Channel2pad(Int_t detector, Int_t sector
                                , Int_t row, Double_t channel)
{
  //
  // adc channel -> pad
  //

#ifdef NODS

  printf("<T49Padtrans::Channel2pad> Sorry, DSPACK library not available.\n");
  return 0.0;

#else

  return channel2pad(detector,sector,row,channel); 

#endif

}

//______________________________________________________________________________
Int_t T49Padtrans::Ext2beam(Double_t xext, Double_t yext, Double_t zext
                          , Double_t *xbeam, Double_t *ybeam, Double_t *zbeam)
{
  //
  // external -> beam
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Padtrans::Ext2beam> Sorry, DSPACK library not available.\n");

#else

  ierr = ext2beam(xext,yext,zext,xbeam,ybeam,zbeam);  

#endif

  return ierr;

}

//______________________________________________________________________________
Int_t T49Padtrans::Beam2ext(Double_t xbeam, Double_t ybeam, Double_t zbeam
                          , Double_t *xext, Double_t *yext, Double_t *zext)
{
  //
  // beam -> external 
  //

  Int_t   ierr = 0;

#ifdef NODS

  printf("<T49Padtrans::Beam2ext> Sorry, DSPACK library not available.\n");

#else

  ierr = beam2ext(xbeam,ybeam,zbeam,xext,yext,zext); 

#endif

  return ierr;

}




