#ifndef _T49Cut_INCLUDED_
#define _T49Cut_INCLUDED_

/*
$Log: T49Cut.h,v $
 * Revision 1.3  2000/11/09  08:55:06  cblume
 * Removed header files
 *
 * Revision 1.2  2000/10/09  10:24:52  cblume
 * General update
 *
 * Revision 1.1  1999/11/23  13:53:00  cblume
 * Add T49Cut.h
 *
*/

#include "TNamed.h"

class T49Cut : public TNamed {

public:

  T49Cut();
  T49Cut(const Text_t *name, const Text_t *title);
  virtual ~T49Cut() {};

  virtual void     Reset()                 {};

  virtual void     PrintCutList()          {};
  virtual void     PrintStatistics()       {};
  virtual void     SetStandardCuts()       {};
  virtual void     SetVerbose(Int_t i = 1) { fVerbose = i; };

protected:

  Int_t   fVerbose;                       // Verbose flag

  ClassDef(T49Cut,1)                      // Cut base class

};

#endif
