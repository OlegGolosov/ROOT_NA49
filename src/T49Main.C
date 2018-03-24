//*CMZ :  1.03/01 12/08/97  15.37.03  by  Valery Fine(fine@vxcern.cern.ch)
//*CMZ :  1.00/09 20/04/97  20.55.41  by  Valery Fine(fine@vxcern.cern.ch)
//*-- Author :    Fons Rademakers   02/03/95


//*KEEP,CopyRight,T=C.
/************************************************************************
 * Copyright(c) 1995-1997, The ROOT System, All rights reserved.
 * Authors: Rene Brun, Nenad Buncic, Valery Fine, Fons Rademakers.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation for non-commercial purposes is hereby granted without fee,
 * provided that the above copyright notice appears in all copies and
 * that both the copyright notice and this permission notice appear in
 * the supporting documentation. The authors make no claims about the
 * suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ************************************************************************/
//*KEND.

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// T49Main                                                              //
//                                                                      //
// Main program used to create a root49 applicatianalysis/na49/T49/src/on.                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//*KEEP,TROOT.
#include "TROOT.h"
//*KEEP,TRint.
#include "TRint.h"
//*KEND.

#include "T49Run.h"

extern void InitGui();

int Error;  // needed by Motif

VoidFuncPtr_t initfuncs[] = { InitGui, 0 };

TROOT root("Rint","The ROOT Interactive Interface", initfuncs);

//______________________________________________________________________________
int main(int argc, char **argv)
{
#ifndef WIN32
   char appname[] = "Rint";
#else
   char appname[] = "Root_Rint";
#endif
   TRint *theApp = new TRint(appname, &argc, argv, 0, 0);

//   Printf(" ");
//   Printf("  **************************************************");
//   Printf("  *                                                *");
//   Printf("  *     _/_/_/ _/_/_/ _/_/_/ _/_/_/ _/_/_/ _/      *");
//   Printf("  *    _/  _/ _/  _/ _/  _/   _/   _/     _/       *");
//   Printf("  *   _/_/   _/  _/ _/  _/   _/   _/_/_/ _/        *");
//   Printf("  *  _/  _/ _/_/_/ _/_/_/   _/   _/_/_/ _/         *");
//   Printf("  *                                                *");
//   Printf("  *          This is the special N49-ROOT.        *");
//   Printf("  *      For documentation have a look at the      *");
//   Printf("  *         \"ROOT mini-DST Analysis\"-link          *");
//   Printf("  *             from the NA49-webpage.             *");
//   Printf("  *                                                *");
//   Printf("  **************************************************");
//   Printf("  ");

   // Create a global T49Run object
   new T49Run();

   // Set root49 prompt
   theApp->SetPrompt("root49 [%d] ");

   // Init Intrinsics, build all windows, and enter event loop
   theApp->Run();

   delete theApp;

   return(0);

}
