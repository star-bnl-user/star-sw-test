// $Id: StFtpcTrackMaker.h,v 1.17 2004/08/10 12:21:42 jcs Exp $
// $Log: StFtpcTrackMaker.h,v $
// Revision 1.17  2004/08/10 12:21:42  jcs
// remove histograms which are also created in St_QA_Maker
//
// Revision 1.16  2004/08/09 15:08:14  jcs
// remove unused histogram
//
// Revision 1.15  2004/06/18 09:07:03  jcs
// add code to write out a root file for calibration
//
// Revision 1.14  2004/06/04 11:04:15  jcs
// replaced StarDb/ftpc/fdepars/fdepar with StarDb/ftpc/ftpcdEdxPars
//
// Revision 1.13  2004/05/24 13:46:39  jcs
// fill StFtpcSoftwareMonitor not dst_mon_soft_ftpc
//
// Revision 1.12  2004/05/07 15:02:18  oldi
// Tracks are written to StEvent directly, now.
// Primary Vertex is read from StEvent.
// MonSoftDst table is filled here now (was filled in the now obsolete StFtpcGlobalMaker before).
//
// Revision 1.11  2004/03/22 16:02:04  oldi
// Moved destruction of the instance of StFtpcTrackingParams from Finish() to FinishRun().
//
// Revision 1.10  2003/09/10 19:47:18  perev
// ansi corrs
//
// Revision 1.9  2002/11/06 13:46:54  oldi
// Vertex handling simplifed.
// Global/primary fit handling simplified.
// Vertex estimations only calculated if a vertex was used for tracking.
// Code clean ups.
//
// Revision 1.8  2002/10/31 13:41:16  oldi
// Histograms for vertex estimation for different sectors added.
//
// Revision 1.7  2002/10/03 10:34:04  oldi
// Usage of gufld removed.
// Magnetic field is read by StMagUtilities, now.
//
// Revision 1.6  2002/04/05 16:51:07  oldi
// Cleanup of MomentumFit (StFtpcMomentumFit is now part of StFtpcTrack).
// Each Track inherits from StHelix, now.
// Therefore it is possible to calculate, now:
//  - residuals
//  - vertex estimations obtained by back extrapolations of FTPC tracks
// Chi2 was fixed.
// Many additional minor (and major) changes.
//
// Revision 1.5  2002/03/01 14:21:21  jcs
// add additional histograms to monitor cluster finding
//
// Revision 1.4  2001/07/12 13:05:01  oldi
// QA histogram of FTPC vertex estimation is generated.
// FTPC vertex estimation is stored as pre vertex (id = 301) in any case, now.
//
// Revision 1.3  2001/02/21 13:14:09  jcs
// Add CVS Id strings in correct place
//
// Revision 1.2  2000/07/03 12:45:23  jcs
// get (pre)Vertex coordinates directly from (pre)Vertex table instead of from
// fptpars
//
// Revision 1.1  2000/05/10 13:39:30  oldi
// Initial version of StFtpcTrackMaker
//

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// StFtpcTrackMaker virtual base class for Maker                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef STAR_StFtpcTrackMaker
#define STAR_StFtpcTrackMaker

#include "StMaker.h"
#include "StFtpcTracker.hh"
#include "StFtpcSoftwareMonitor.h"

// For cluster and laser run analysis
//#define DEBUGFILE
// Select tracker
#define TWOCYCLETRACKING
//#define LASERTRACKING

class TH1F;
class TH2F;
class TProfile;
class StEvent;
class St_ftpcdEdxPars;
 
class StFtpcTrackMaker : public StMaker {

 private:

  protected:
       TH1F          *m_vtx_pos;    //! vertex position
       TH1F          *m_theta;      //! theta
       TH2F          *m_nrec_track; //! # points found per track vs. momentum
       TH2F          *m_padvstime_West; //! padlength vs. timelength
       TH2F          *m_padvstime_East; //! padlength vs. timelength
       TH1F          *m_maxadc_West;
       TH1F          *m_maxadc_East;
       TH1F          *m_charge_West;
       TH1F          *m_charge_East;
       TH1F          *m_xres;       //! x residuals
       TH1F          *m_yres;       //! y residuals
       TH1F          *m_rres;       //! r residuals
       TH1F          *m_phires;     //! phi residuals
       TH2F          *m_rres_vs_r_east;   //! r residuals vs. r east
       TH2F          *m_phires_vs_r_east; //! phi residuals vs. r east
       TH2F          *m_rres_vs_r_west;   //! r residuals vs. r west
       TH2F          *m_phires_vs_r_west; //! phi residuals vs. r west

       TH2F          *m_vertex_east_xy;   //! vertex xy estimation east
       TH1F          *m_vertex_east_z;    //! vertex z estimation east
       TH2F          *m_vertex_east_x_vs_sector; //! vertex x estimation east vs sector
       TH2F          *m_vertex_east_y_vs_sector; //! vertex y estimation east vs sector
       TH2F          *m_vertex_east_z_vs_sector; //! vertex z estimation east vs sector
       TH2F          *m_vertex_west_xy;   //! vertex xy estimation west
       TH1F          *m_vertex_west_z;    //! vertex z estimation west
       TH2F          *m_vertex_west_x_vs_sector; //! vertex x estimation west vs sector
       TH2F          *m_vertex_west_y_vs_sector; //! vertex y estimation west vs sector
       TH2F          *m_vertex_west_z_vs_sector; //! vertex z estimation west vs sector

 public: 
                  StFtpcTrackMaker(const char *name="ftpc_tracks"); // constructor
   virtual       ~StFtpcTrackMaker();                               // destructor
   virtual Int_t  InitRun(Int_t run);                               // Initialisation per run
   virtual Int_t  Init();                                           // Initialisation 
   virtual Int_t  Make();                                           // actual program
   virtual Int_t  Finish();                                         // final cleanup
   virtual Int_t  FinishRun(Int_t run);                             // cleanup after every run
   virtual const char *GetCVS() const
  {static const char cvs[]="Tag $Name:  $ $Id: StFtpcTrackMaker.h,v 1.17 2004/08/10 12:21:42 jcs Exp $ built "__DATE__" "__TIME__ ; return cvs;}
   virtual void   PrintInfo();                                      // prints information
	   void   MakeHistograms(StFtpcTracker *tracker);           // makes histograms
	   void   FillMonSoftFtpc(StEvent *event,StFtpcTracker *tracker,StFtpcSoftwareMonitor *ftpcMon);  // fills StEvent->StSoftwareMonitor->StFtpcSoftwareMonitor


   ClassDef(StFtpcTrackMaker,0)   //StAF chain virtual base class for Makers
};

#endif
