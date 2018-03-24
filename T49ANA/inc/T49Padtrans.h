#ifndef _T49Padtrans_INCLUDED_
#define _T49Padtrans_INCLUDED_

#include "TObject.h"

class T49Padtrans : public TObject {

 public:

  T49Padtrans();
  virtual ~T49Padtrans() { };

  Int_t    InitDS(Char_t *dspack_server = "");

  void     SetKeys(Char_t *key1, Char_t *key3, Char_t *key4
                 , Char_t *key5, Char_t *key6, Char_t *key7);
  void     SetInitFile(Char_t *filename);
  void     SetVerbose(Int_t v);

  Int_t    Pad2int(Int_t detector, Int_t sector
                 , Int_t row, Double_t pad, Double_t time
                 , Double_t *xint, Double_t *yint, Double_t *zint);
  Int_t    Int2pad(Int_t detector, Double_t xint, Double_t yint, Double_t zint
                 , Int_t *sector, Int_t *row, Double_t *pad, Double_t *time);
  Int_t    Int2ext(Int_t detector, Double_t xint, Double_t yint, Double_t zint
                 , Double_t *xext, Double_t *yext, Double_t *zext);
  Int_t    Ext2int(Int_t detector, Double_t xext, Double_t yext, Double_t zext
                 , Double_t *xint, Double_t *yint, Double_t *zint);
  Int_t    Int2sr(Int_t detector, Double_t xint, Double_t yint, Double_t zint
                , Int_t *sector, Int_t *row);
  Int_t    Int2drow(Int_t detector, Double_t xint, Double_t yint, Double_t zint
                  , Int_t *sector, Double_t *row, Double_t *pad, Double_t *time);
  Int_t    Drow2int(Int_t detector, Int_t sector
                  , Double_t row, Double_t pad, Double_t time
                  , Double_t *xint, Double_t *yint, Double_t *zint);
  Double_t Pad2channel(Int_t detector, Int_t sector, Int_t row, Double_t pad);
  Double_t Channel2pad(Int_t detector, Int_t sector, Int_t row, Double_t channel);
  Int_t    Ext2beam(Double_t xext, Double_t yext, Double_t zext
                  , Double_t *xbeam, Double_t *ybeam, Double_t *zbeam);
  Int_t    Beam2ext(Double_t xbeam, Double_t ybeam, Double_t zbeam
                  , Double_t *xext, Double_t *yext, Double_t *zext);

 private:
  
  ClassDef(T49Padtrans,0) // Padtrans wrapper class

};

#endif



