///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Cut base class                                                 //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "T49Cut.h"

ClassImp(T49Cut)

/*
$Log: T49Cut.C,v $
 * Revision 1.2  2000/11/09  08:55:42  cblume
 * Removed header files
 *
 * Revision 1.1  1999/11/23  13:54:42  cblume
 * Add T49Cut.C
 *
*/

//______________________________________________________________________________
T49Cut::T49Cut():TNamed() 
{
  //
  // T49Cut default constructor
  //

  fVerbose = 0;

}

//______________________________________________________________________________
T49Cut::T49Cut(const Text_t *name, const Text_t *title)
       :TNamed(name, title) 
{
  //
  // T49Cut constructor with name and title
  //

  fVerbose = 0;

}




