// $Id: WeventDisplay.h,v 1.1 2011/02/10 20:33:27 balewski Exp $
//
//*-- Author : Jan Balewski, MIT


//----------------------------
//------- W-event Display container
//----------------------------
#ifndef W_EVENT_Diplay_HH
#define W_EVENT_Display_HH

#include <TVector3.h>

#include "Wevent2011.h"

class TH2F;
class TH1F;
class St2011WMaker;
class TLine;
class TBox;

//---------------
class WeventDisplay {
 public:
  int maxEve;
  TH2F *hTpcET;
  TH2F *hEmcET;
  TH2F *hBsmdAdc[mxBSmd];
  TH1F *hEsmdShower[mxEsmdPlane];
  TH2F *hEsmdXpt;
  TLine *etaBL_ln, *etaBR_ln; // barrel
  TLine *etaEL_ln, *etaER_ln; // endcap
  TBox  *bxT, *bxE,*bxS; // tpc, emc, bsmd
  St2011WMaker* wMK;

  WeventDisplay( St2011WMaker* mk, int mxEv);
  void clear();
  void exportEvent(  const char *tit,WeveVertex myV, WeveEleTrack myTr);
  void export2sketchup(  const char *tit, WeveVertex myV, WeveEleTrack myTr);
  void getPrimTracks(  int vertID);
  void draw( const char *tit,int eveID, int daqSeq, int runNo,  WeveVertex myV, WeveEleTrack myTr);

};

#endif


// $Log: WeventDisplay.h,v $
// Revision 1.1  2011/02/10 20:33:27  balewski
// start
//
