// $Id: StdEdxY2Maker.h,v 1.12 2004/07/01 13:38:23 fisyak Exp $
#ifndef STAR_StdEdxY2Maker
#define STAR_StdEdxY2Maker

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// StdEdxY2Maker virtual base class for Maker                            //
//                                                                      //
//  Submit any problem with this code via begin_html <A HREF="http://www.rhic.bnl.gov/STAR/html/comp_l/sofi/bugs/send-pr.html"><B><I>"STAR Problem Report Form"</I></B></A> end_html
//
//////////////////////////////////////////////////////////////////////////
#ifndef StMaker_H
#include "StMaker.h"
#endif
#include "StTpcDb/StTpcdEdxCorrection.h" 
#include "StThreeVectorF.hh"
#include "StThreeVectorD.hh" 
class Bichsel;
class StGlobalTrack;
class TMinuit; 
class StEvent;
class StGlobalCoordinate;
class TH2D;
class St_trigDetSums;
class trigDetSums;
class StdEdxY2Maker : public StMaker {
 public: 
  enum  EMode {kOldClusterFinder       ,
	       kCalibration            ,
	       kDoNotCorrectdEdx       ,
	       kPadSelection           ,
	       kMip                    ,
	       kAdcHistos              ,
	       kXYZcheck               ,
	       kSpaceChargeStudy       ,
	       kGASHISTOGRAMS          ,
	       kProbabilityPlot        ,
	       kMakeTree               ,
	       kCORRELATION            ,
	       kAlignment              , 
	       kZBGX };
  StdEdxY2Maker(const char *name="dEdxY2");
  virtual       ~StdEdxY2Maker() {}
  virtual Int_t Init();
  virtual Int_t InitRun(Int_t RunNumber);
  virtual Int_t Finish();
  virtual Int_t FinishRun(Int_t OldRunNumber);
  virtual Int_t Make();
  virtual void  SetMask(Int_t mask) {m_Mask = mask;}
  static  Int_t SortdEdx(Int_t N, dEdx_t *dE, dEdx_t *dES);
  Double_t LikeliHood(Double_t Xlog10bg, Int_t NdEdx, dEdx_t *dEdx);
  void    Histogramming(StGlobalTrack* gTrack=0);
  void    TrigHistos(Int_t iok = 0);
  void    SpaceCharge(Int_t iok = 0, StEvent * pEvent=0, StGlobalCoordinate *global=0, dEdx_t *CdEdx=0);
  void    XyzCheck(StGlobalCoordinate *global=0, Int_t iokCheck=0);
  void    QAPlots(StGlobalTrack* gTrack = 0);
  void    BadHit(const StThreeVectorF &xyz);
  void    DoFitZ(Double_t &chisq, Double_t &fitZ, Double_t &fitdZ);
  void    PrintdEdx(Int_t iop = 0);
  static  void Landau(Double_t x, Double_t *val);
  static  void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
  virtual const char *GetCVS() const {
    static const char cvs[]=
      "Tag $Name:  $ $Id: StdEdxY2Maker.h,v 1.12 2004/07/01 13:38:23 fisyak Exp $ built "__DATE__" "__TIME__ ; 
    return cvs;
  }
 private:
  Bichsel             *m_Bichsel;       //!
  TMinuit             *m_Minuit;        //!
  StTpcdEdxCorrection *m_TpcdEdxCorrection; // !
  Int_t                m_Mask; //!
  static Int_t  NdEdx;
  static dEdx_t *CdEdx; // corrected
  static dEdx_t *FdEdx; // fit
  static dEdx_t *dEdxS; // dEdx sorted
  StThreeVectorD      *mNormal[24][45];     //!
  StThreeVectorD      *mRowPosition[24][45][3]; //!
  St_trigDetSums      *m_trigDetSums;//!
  trigDetSums_st      *m_trig;//!
  TH2D                *mHitsUsage;//!
  
  ClassDef(StdEdxY2Maker,0)   //StAF chain virtual base class for Makers
};

#endif

