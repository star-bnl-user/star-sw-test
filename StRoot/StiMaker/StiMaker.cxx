// $Id $
/// \File StiMaker.cxx
/// \author M.L. Miller 5/00
/// \author C Pruneau 3/02
// $Log: StiMaker.cxx,v $
// Revision 1.173  2006/12/18 01:29:00  perev
// +noTreeSearch flag & pulls
//
// Revision 1.172  2006/10/16 20:30:42  fisyak
// Clean dependencies from Sti useless classes
//
// Revision 1.171  2006/10/15 05:10:10  fisyak
// Add Hpd
//
// Revision 1.170  2006/10/09 15:51:28  fisyak
// Remove Ftpc
//
// Revision 1.169  2006/08/01 03:51:00  perev
// Return from Make() for too many hits
//
// Revision 1.168  2006/06/16 21:27:52  perev
// Minimal errors of vertex 1 micron
//
// Revision 1.167  2006/05/31 03:59:04  fisyak
// Add Victor's dca track parameters, clean up
//
// Revision 1.166  2006/04/14 22:51:26  perev
// Option useFakeVertex added
//
// Revision 1.161  2006/02/14 18:53:58  perev
// Sub makerFunctionality added.
//
// Revision 1.160  2006/02/08 20:56:39  fisyak
// use kHftId and kIstId for StiDetector groups instead of hadr coded numbers 9999 and 9998
//
// Revision 1.159  2006/01/19 20:21:52  perev
// Ist added
//
// Revision 1.158  2005/12/31 01:34:02  perev
// Degug histos added
//
// Revision 1.157  2005/12/07 23:55:02  perev
// control is changed using StMaker::SetAttr
//
// Revision 1.156  2005/11/22 23:15:27  fisyak
// Clean up parameters setting
//
// Revision 1.155  2005/10/26 21:54:10  fisyak
// Remove dead classes, gid rid off dependencies from StMcEvent and StiGui
//
// Revision 1.154  2005/10/06 20:38:46  fisyak
// Clean up
//
// Revision 1.153  2005/09/28 21:46:36  fisyak
// Persistent StMcEvent
//
// Revision 1.152  2005/08/09 15:23:18  perev
// Add new factory for Node extention
//
// Revision 1.151  2005/08/04 04:03:19  perev
// Cleanup
//
// Revision 1.150  2005/07/21 01:20:12  perev
// clearmem is default now
//
// Revision 1.149  2005/07/20 17:33:25  perev
// MultiVertex
//
// Revision 1.148  2005/02/25 17:41:01  perev
// Time count added
//
// Revision 1.147  2005/01/25 17:23:48  pruneau
// removed references to html package
//
// Revision 1.146  2005/01/21 03:13:37  pruneau
// turned off StiHistograms
//
// Revision 1.145  2005/01/17 03:56:47  pruneau
// change track container to vector
//
// Revision 1.144  2005/01/17 01:32:13  perev
// parameters protected
//
// Revision 1.143  2004/08/04 21:06:17  pruneau
// Added an "if" statement predicated on m_Mode to clear the memory used by the
// factories at the end of StiMaker::Make().
//
// Revision 1.142  2004/04/15 00:43:22  pruneau
// Added Ssd to the list of possible detectors...
//
// Revision 1.141  2004/03/26 15:30:06  andrewar
// bug in field reset
//
// Revision 1.140  2004/03/26 14:52:43  calderon
// Print out the magnetic field read from StEvent::eventSummary()
//
// Revision 1.139  2004/03/25 22:42:44  andrewar
// temp mag field fix; cache filed value and reset if it goes to zero. This
// protects against corrupt event headers...
//
// Revision 1.138  2004/02/24 01:59:46  jeromel
// Commented out include of disappeared .h
//
// Revision 1.137  2004/02/21 18:28:31  pruneau
// Updates to comply to changes in interfaces
//
// Revision 1.136  2004/02/19 22:18:07  pruneau
// Modified call to StMcEventMaker structure
//
// Revision 1.135  2004/02/13 17:36:24  andrewar
// Changed name of StMcEventMaker to StMcEvent... this allows me to run
// simulation. It doesn't seem like this follows the Maker name scheme, though...
//
// Revision 1.134  2004/02/03 18:10:10  pruneau
// Changed name of StMcEventMaker to McEvent in GetMaker call
//
// Revision 1.133  2004/01/30 21:47:23  pruneau
// Changed organization so detector geometris are loaded and build in InitRun
// rather than Make.
// Added accesses to db
//
// Revision 1.132  2003/10/28 16:01:15  andrewar
// Passing tracking parameter file to detector Builders.
//
// Revision 1.131  2003/09/02 17:59:59  perev
// gcc 3.2 updates + WarnOff
//
// Revision 1.130  2003/08/05 18:20:33  andrewar
// Changed default parameters to apply eta filters.
//
// Revision 1.129  2003/07/30 20:12:31  pruneau
// Added new histo group
//
// Revision 1.128  2003/06/10 18:47:28  andrewar
// Changed StiResiduaCalc calls to conform to modified class.
//
// Revision 1.127  2003/05/07 03:06:34  pruneau
// *** empty log message ***
//
// Revision 1.126  2003/05/06 16:48:10  mmiller
// Incorporated StiPixel.  usePixel==false by default.
//
// Revision 1.125  2003/05/06 15:36:36  mmiller
// Committing changes to turn on multiple regions (StiPlacement::StiRegion -> kMidRapidity, kForwardRapidity, etc).
// Also added a point to StiToolkit for StiMaker.  This allows for the req. GetDataSet calls in the FTPC code.
// Not so elegant...
//
// Revision 1.124  2003/04/30 15:39:33  pruneau
// Integrating StiResidual in main stream Sti
//
// Revision 1.123  2003/04/29 18:48:50  pruneau
// *** empty log message ***
//
// Revision 1.122  2003/04/13 02:16:13  pruneau
// *** empty log message ***
//
// Revision 1.121  2003/04/11 18:56:14  pruneau
// Pulling the B field from StEventSummary
//
// Revision 1.120  2003/04/11 16:51:57  pruneau
// various fixes
//
// Revision 1.119  2003/04/10 14:53:06  pruneau
// removing obsolete files and classes
//
// Revision 1.118  2003/04/10 12:10:09  pruneau
// Changed StiMaker and Default Toolkit to accomodate the new Event Display
//
// Revision 1.117  2003/03/31 17:19:27  pruneau
// various
//
// Revision 1.116  2003/03/17 17:44:49  pruneau
// *** empty log message ***
//
// Revision 1.115  2003/03/13 18:59:42  pruneau
// various updates
//
// Revision 1.114  2003/03/13 16:30:59  andrewar
// Added plotting package
//
// Revision 1.113  2003/03/13 15:15:51  pruneau
// various
//
// Revision 1.112  2003/03/12 17:58:04  pruneau
// fixing stuff
//
// Revision 1.111  2003/02/25 14:21:06  pruneau
// *** empty log message ***
//
// Revision 1.110  2003/01/24 06:12:28  pruneau
// removing centralized io
//
// Revision 1.109  2003/01/22 20:06:26  andrewar
// Changed includes to point to new libraries (StiTpc, StiSvt, etc)
//
// Revision 1.108  2002/12/19 19:29:42  pruneau
// *** empty log message ***
//
// Revision 1.106  2002/10/04 01:54:48  pruneau
// DefaultToolkit now uses the StiHitLoader scheme rahter than the StiHitFiller.
//
// Revision 1.105  2002/09/27 19:19:01  mmiller
// Changed program flow to once again allow for track by track gui.
//
// Revision 1.104  2002/09/10 18:42:40  pruneau
// Fixed bug in the call sequence of the association maker
// introduced in the previous release.
//
// Revision 1.103  2002/09/05 21:27:10  pruneau
// Fixed problem with StiRootSimpleTrackFilter::makeNewObject
//
// Revision 1.102  2002/09/05 05:47:30  pruneau
// Adding Editable Parameters and dynamic StiOptionFrame
//
// Revision 1.101  2002/08/28 17:14:18  pruneau
// Simplified the interface of StiKalmanTrackFinder and the calls
// required in StiMaker.
//
// Revision 1.100  2002/08/23 18:16:50  pruneau
// Added StiSimpleTrackFilter to StiMaker to enable simple and
// fast track finding diagnostics.
//
// Revision 1.99  2002/08/19 19:32:59  pruneau
// eliminated cout when unnecessary, made helix member of the EventFiller
//
// Revision 1.98  2002/06/26 23:05:31  pruneau
// changed macro
//
// Revision 1.97  2002/06/18 18:08:34  pruneau
// some cout statements removed/added
//
// Revision 1.96  2002/06/04 19:45:31  pruneau
// including changes for inside out tracking
//
/*!

\class StiMaker 

\author M.L. Miller 5/00
\author C Pruneau 3/02
\author V Perev 2005

A maker StiMaker is a steering maker for Sti package.
<br>
Main tasks:				
<ul>
<li> Create StiHits;			
<li> Make tracks;				
<li> Make Primary vertices;		
<li> Make Primary tracks;			
<li> Save produced data into StEvent.	
</ul>
More detailed: 				<br>
<ul>
<li>On Init:				
<ul>
<li> Detectors initialization. 
     SetAttr("useTpc"  ,1) && SetAttr("activeTpc"  ,1) 	// default
     SetAttr("useSvt",  1) && SetAttr("activeSvt"  ,0) 	// default
     SetAttr("useSsd"  ,0) && SetAttr("activeSsd"  ,0)	// default Off
     SetAttr("usePixel",0) && SetAttr("activePixel",0)	// default Off
     SetAttr("useIst"  ,0) && SetAttr("activeIst"  ,0)	// default Off
     SetAttr("useHpd"  ,0) && SetAttr("activeHpd"  ,0)	// default Off

     SetAttr("useEventFiller"      ,kTRUE);		// default On
     SetAttr("useTracker"          ,kTRUE);		// default On
     SetAttr("useVertexFinder"     ,kTRUE);		// default On
     SetAttr("makePulls"           ,kFALSE);		// default Off

     SetAttr("noTreeSearch",kFALSE);	// treeSearch default ON
     SetAttr("svtSelf",,kFALSE);	// Svt self align default OFF
</ul>
 
<li>On InitRun:				
<ul>
 <li> Build detectors;			
 <li> Init seed finder;			
 <li> Init hit loader;			
 <li> Init tracker;  			
 <li> Init StEvent filler;			 
 <li> Init vertex finder;			 
</ul>
<li>In Make:				
<ul>
 <li> Load hits; 
 <li> Find seeds; 
 <li> Create global tracks; 
 <li> Save tracks into StEvent;
 <li> Find vertecies; 
 <li> Create and assign primaries tracks; 
 <li> Save primary tracks into StEvent;

*/
#include <Stiostream.h>
#include <math.h>
#include <string>
#include "TSystem.h"
#include "TTree.h"
#include "TCL.h"
#include "StChain.h"
#include "TDataSet.h"
#include "TDataSetIter.h"
#include "StMessMgr.h"
#include "StBFChain.h"
#include "SystemOfUnits.h"
#include "TMemStat.h"
#include "PhysicalConstants.h"
#include "StDetectorId.h"
#include "StEventTypes.h"
#include "Sti/Base/EditableFilter.h"
#include "Sti/StiKalmanTrackFinder.h"
#include "Sti/StiTrackContainer.h"
#include "Sti/StiDefaultTrackFilter.h"
#include "Sti/StiMasterDetectorBuilder.h"
#include "Sti/Star/StiStarDetectorGroup.h"
#include "StiTpc/StiTpcDetectorGroup.h"
#include "StiTpc/StiTpcHitLoader.h"
#include "StiSvt/StiSvtDetectorGroup.h"
#include "StiSsd/StiSsdDetectorGroup.h"
#include "StiPixel/StiPixelDetectorGroup.h"
#include "Sti/StiKalmanTrackNode.h"
#include "Sti/StiKalmanTrack.h"
#include "Sti/StiHitLoader.h"
#include "Sti/StiVertexFinder.h"
#include "Sti/StiDetectorContainer.h"
#include "StiMaker/StiStEventFiller.h"
#include "StiDefaultToolkit.h"
#include "StiMaker.h"
#include "TFile.h"
#include "TCanvas.h"
#include "Sti/StiTrackingParameters.h"
#include "Sti/StiKalmanTrackFinderParameters.h"
#include "Sti/StiKalmanTrackFitterParameters.h"
#include "StiTpc/StiTpcDetectorBuilder.h"
#include "StiSvt/StiSvtDetectorBuilder.h"
#include "Sti/StiHitErrorCalculator.h"
#include "StiPixel/StiIstDetectorGroup.h"
#include "StiPixel/StiHpdDetectorGroup.h"  //added for the Y-Pixel detector
#include "StiUtilities/StiDebug.h"
#include "StiUtilities/StiPullEvent.h"
#include "TDataSet.h"
#include "tables/St_TrackingParameters_Table.h"
#include "tables/St_KalmanTrackFinderParameters_Table.h"
#include "tables/St_KalmanTrackFitterParameters_Table.h"
#include "tables/St_HitError_Table.h"

#include "Sti/StiTimer.h"

/// Definion of minimal primary vertex errors.
/// Typical case,vertex got from simulations with zero errors.
/// But zero errors could to unpredicted problems
/// Now minimal possible error is 1 micron
static const float MIN_VTX_ERR2 = 1e-4*1e-4;

ClassImp(StiMaker)
  
//_____________________________________________________________________________
StiMaker::StiMaker(const Char_t *name) : 
    StMaker(name),
    _initialized(false),
    _toolkit(0),
    _hitLoader(0),
    _seedFinder(0),
    _tracker(0),
    _fitter(0),
    _eventFiller(0),
    _trackContainer(0),
    _vertexFinder(0),
    _loaderTrackFilter(0),
    _loaderHitFilter(0)

{
  cout <<"StiMaker::StiMaker() -I- Starting"<<endl;
  mPullFile=0; mPullEvent=0;mPullTTree=0;
  memset(mPullHits,0,sizeof(mPullHits));

  if (!StiToolkit::instance()) new StiDefaultToolkit;
  _toolkit = StiToolkit::instance();
  SetAttr("useTpc"		,kTRUE);
  SetAttr("activeTpc"		,kTRUE);
  SetAttr("useSvt"		,kTRUE); 
//SetAttr("activeSvt"		,kTRUE);
//SetAttr("useAux"		,kTRUE); // Auxiliary info added to output for evaluation
  SetAttr("useEventFiller"      ,kTRUE);
  SetAttr("useTracker"          ,kTRUE);
  SetAttr("useVertexFinder"     ,kTRUE);

  if (strstr(gSystem->Getenv("STAR"),".DEV"))
     SetAttr("useAux",kTRUE); // Auxiliary info added to output for evaluation
}

//_____________________________________________________________________________
StiMaker::~StiMaker() 
{
  cout <<"StiMaker::~StiMaker() -I- Started/Done"<<endl;
}

//_____________________________________________________________________________
void StiMaker::Clear(const char*)
{
  if (_tracker  ) _tracker->clear();
  if (mPullEvent) mPullEvent->Clear();
  StMaker::Clear();
}

//_____________________________________________________________________________
Int_t StiMaker::Finish()
{
  StiDebug::Finish();
//	Finish Pull
  if (mPullTTree) {
    if  (Debug()) mPullTTree->Print();
    if (mPullFile) {
      TFile *tfile = mPullTTree->GetCurrentFile(); //just in case we switched to a new file
      tfile->Write();
      tfile->Close();
      mPullFile  = 0;
      mPullTTree = 0;
    }
  }
  StiTimer::Print();
  StiTimer::Clear();




  return StMaker::Finish();
}

//_____________________________________________________________________________
Int_t StiMaker::Init()
{

  StiDebug::Init();
  runField =0.;
  StiTimer::Init("StiTrackFinder::find() TIMING"
	        ,StiTimer::fgFindTimer,StiTimer::fgFindTally);
  
  _loaderHitFilter = 0; // not using this yet.

  if (IAttr("useSvtSelf")) {
    SetAttr("useTpc"		,0);
    SetAttr("activeTpc"		,0);
    SetAttr("useSvt"		,kTRUE); 
    SetAttr("activeSvt"		,kTRUE);
//    SetAttr("useSsd"		,kTRUE); 
//    SetAttr("activeSsd"		,kTRUE);
    SetAttr("useEventFiller"    ,0);
    SetAttr("useTracker"        ,0);
    SetAttr("useVertexFinder"   ,0);
    gSystem->Load("StSvtSelfMaker");
    StMaker *selfMk = StMaker::New("StSvtSelfMaker",0,0);
    AddMaker(selfMk);
  }
  InitDetectors();
  return StMaker::Init();
}

//_____________________________________________________________________________
Int_t StiMaker::InitDetectors()
{
  StiDetectorGroup<StEvent> * group;
  cout<<"StiMaker::InitDetectors() -I- Adding detector group:Star"<<endl;
  _toolkit->add(new StiStarDetectorGroup(false,"none"));
  if (IAttr("useTpc"))
    {
      cout<<"StiMaker::InitDetectors() -I- Adding detector group:TPC"<<endl;
      _toolkit->add(group = new StiTpcDetectorGroup(IAttr("activeTpc"),SAttr("tpcInputFile")));
      group->setGroupId(kTpcId);
      if (IAttr("activeSvt") || IAttr("activeSsd") || IAttr("skip1row")) {// skip 1 row 
	((StiTpcHitLoader*)group->hitLoader())->setMinRow(2);
	cout << "StiMaker::InitDetectors() -I- Ignore hits in the first pad row"<<endl;
      }
    }
  if (IAttr("useSvt"))
    {
    cout<<"StiMaker::Init() -I- Adding detector group:SVT"<<endl;
    _toolkit->add(group = new StiSvtDetectorGroup(IAttr("activeSvt"),SAttr("svtInputFile")));
    group->setGroupId(kSvtId);
    }
  if (IAttr("useSsd"))
      {
	  cout<<"StiMaker::Init() -I- Adding detector group:Ssd"<<endl;
	  _toolkit->add(group = new StiSsdDetectorGroup(IAttr("activeSsd"),SAttr("ssdInputFile")));
	  group->setGroupId(kSsdId);
      }
  if (IAttr("usePixel"))
    {
      cout<<"StiMaker::Init() -I- Adding detector group:PIXEL"<<endl;
      _toolkit->add(group = new StiPixelDetectorGroup(IAttr("activePixel"),SAttr("pixelInputFile")));
      group->setGroupId(kHftId);
    }
 if (IAttr("useIst"))
    {
      cout<<"StiMaker::Init() -I- Adding detector group:Ist"<<endl;  
      _toolkit->add(group = new StiIstDetectorGroup(IAttr("activeIst"),SAttr("istInputFile")));
      group->setGroupId(kIstId);  
    }
  if (IAttr("useHpd"))
    {
      cout<<"StiMaker::Init() -I- Adding detector group:Hpd"<<endl;
      _toolkit->add(group = new StiHpdDetectorGroup(IAttr("activeHpd"),SAttr("hpdInputFile")));
      group->setGroupId(kHpdId);
    }  //Added but removed later to make the IST hits the tracking...
  return kStOk;
}

//_____________________________________________________________________________
Int_t StiMaker::InitRun(int run)
{
  if (!_initialized)
    {
      cout <<"StiMaker::InitRun() -I- Initialization Segment Started"<<endl;


			// Load Detector related parameters
			StiMasterDetectorBuilder * masterBuilder = _toolkit->getDetectorBuilder();
			masterBuilder->build(*this);
      StiDetectorContainer * detectorContainer = _toolkit->getDetectorContainer(); 
      detectorContainer->initialize();//build(masterBuilder);
      detectorContainer->reset();
       _seedFinder = _toolkit->getTrackSeedFinder();
      _seedFinder->initialize();
      _hitLoader  = _toolkit->getHitLoader();
      _tracker=0;
      if (IAttr("useTracker")) {
        _tracker = dynamic_cast<StiKalmanTrackFinder *>(_toolkit->getTrackFinder());
        _fitter  = dynamic_cast<StiKalmanTrackFitter *>(_toolkit->getTrackFitter());
	_tracker->load("trackFinderPars.dat",*this);
	_fitter->load("trackFitterPars.dat",*this);
        if (IAttr("noTreeSearch")) _tracker->setComb(0);

      }
      _eventFiller=0;
      if (IAttr("useEventFiller")) {
        _eventFiller =  new StiStEventFiller();
        _eventFiller->setUseAux(IAttr("useAux"));
        InitPulls();
      }
      _trackContainer = _toolkit->getTrackContainer();
      _vertexFinder   = 0;
      
      _vertexFinder   = _toolkit->getVertexFinder();
      if (_tracker) {
        _tracker->initialize();
        _tracker->clear();
      }
      _initialized=true;
      cout <<"StiMaker::InitRun() -I- Initialization Segment Completed"<<endl;
    }
  
  return StMaker::InitRun(run);
}

//_____________________________________________________________________________
Int_t StiMaker::Make()
{
  cout <<"StiMaker::Make() -I- Starting on new event"<<endl;

  eventIsFinished = false;
  StEvent   * event = dynamic_cast<StEvent*>( GetInputDS("StEvent") );

  if (!event) return kStWarn;

  // Retrieve bfield in Tesla
  double field = event->summary()->magneticField()/10.;

  if (runField==0) runField=field;
  if (field==0 && field != runField) field=runField;

  cout << "StiMaker::Make() -I- Reading eventSummary()->magneticField() " << field << endl; 
  if (_tracker) {
    static_cast<StiKalmanTrackFinderParameters&>(_tracker->getParameters()).setField(field);
  _tracker->clear();
  }
  try {
    _hitLoader->loadEvent(event,_loaderTrackFilter,_loaderHitFilter);
  }
  catch (runtime_error &rte)
  {
    cout << "StiMaker::Make() - loadEvent(..) failed :" << rte.what() << endl;
    MyClear();
    return kStWarn;
  }

  _seedFinder->reset();
  if (_tracker) {
      _tracker->findTracks();
      const std::vector<StiHit*> *vertexes=0;
      try
	{
	  if (_eventFiller)
	    _eventFiller->fillEvent(event, _trackContainer);
	}
      catch (runtime_error & rte)
	{
	  cout << "StiMaker::Make() - Run Time Error :" << rte.what() << endl;
	}
      if (_vertexFinder)
	{
	  //cout << "StiMaker::Maker() -I- Will Find Vertex"<<endl;
	  _vertexFinder->fit(event);
	  vertexes = _vertexFinder->result();


	  if (vertexes && vertexes->size())
	    {

              //Set minimal errors
	      for (size_t i=0;i<vertexes->size();i++) {
	        StiHit *vtx=(*vertexes)[i];
                float vtxErr[6];
		memcpy(vtxErr,vtx->errMtx(),sizeof(vtxErr));
                if (vtxErr[0]>MIN_VTX_ERR2
                &&  vtxErr[2]>MIN_VTX_ERR2
                &&  vtxErr[5]>MIN_VTX_ERR2) continue;
                memset(vtxErr,0,sizeof(vtxErr));
                vtxErr[0]=MIN_VTX_ERR2;
                vtxErr[2]=MIN_VTX_ERR2;
                vtxErr[5]=MIN_VTX_ERR2;
                vtx->setError(vtxErr);
              }
	      //cout << "StiMaker::Make() -I- Got Vertex; extend Tracks"<<endl;
	      _tracker->extendTracksToVertices(*vertexes);
	      //cout << "StiMaker::Make() -I- Primary Filling"<<endl; 
		  if (_eventFiller) _eventFiller->fillEventPrimaries();
	    }
	}
    }
  int iAns=kStOK,iAnz;
  if (mPullTTree) {iAns = FillPulls();}
  cout<< "StiMaker::Make() -I- Done"<<endl;
  iAnz = StMaker::Make();

  MyClear();
  if (iAns) return iAns;
  if (iAnz) return iAnz;
  return kStOK;
}
//_____________________________________________________________________________
void StiMaker::MyClear()
{
//    cout << "StiMaker -I- Perform Yuri's clear... ;-)" << endl;
//      TMemStat::PrintMem("Before StiFactory clear()");
      _toolkit->getHitFactory()->clear();
      _toolkit->getTrackNodeFactory()->clear();
      _toolkit->getTrackNodeExtFactory()->clear();
      _toolkit->getTrackNodeInfFactory()->clear();
      _toolkit->getTrackFactory()->clear();
//      TMemStat::PrintMem("After  StiFactory clear()");
}
//_____________________________________________________________________________
Int_t StiMaker::InitPulls()
{
  if (!IAttr("makePulls")) 	return 0;
  
  StBFChain *bfc = dynamic_cast<StBFChain*>(GetChain());
  assert(bfc);
  TFile *tfile  = bfc->GetTFile();
  if (!tfile) {
    TString ts  = bfc->GetFileIn();
    ts= gSystem->BaseName(ts);
    int ext = ts.Index(".");
    if (ext>0) ts.Replace(ext,999,"");
    ts +=".stipull.root";
    tfile = mPullFile = new TFile(ts,"RECREATE","TTree Sti Pulls ROOT file");
  }
  tfile->cd();
  mPullTTree = new TTree("StiPulls","TTree Sti pulls");
  mPullTTree->SetAutoSave(100000000);  // autosave when 0.1 Gbyte written
  mPullEvent = new StiPullEvent;
  TBranch *branch = mPullTTree->Branch("event", mPullEvent->ClassName(),&mPullEvent, 16000,99);
  branch->SetAutoDelete(kFALSE);
  _eventFiller->setPullEvent(mPullEvent);
  return 0;
}
//_____________________________________________________________________________
Int_t StiMaker::FillPulls()
{
  StEvtHddr   *hddr = GetEvtHddr();
  mPullEvent->mRun  = hddr->GetRunNumber();
  mPullEvent->mEvt  = hddr->GetEventNumber();
  mPullEvent->mDate = hddr->GetDateTime();	//DAQ time (GMT)
  StiHit *vertex   = _vertexFinder->getVertex(0);
  if (!vertex) return 0;
  mPullEvent->mChi2 = 0;	
  
  mPullEvent->mVtx[0] = vertex->x_g();
  mPullEvent->mVtx[1] = vertex->y_g();
  mPullEvent->mVtx[2] = vertex->z_g();
  TCL::ucopy(vertex->errMtx(),mPullEvent->mEtx,6);
  mPullTTree->Fill();
  for (int i=0; i<3; i++) {mPullHits[i]+=mPullEvent->mNHits[i];}
  if (! IAttr(".Privilege")) return kStOK;

  int k;for (k=2; k>=0; k--) {if (mPullHits[k]) break;}
  if (k<0) return kStOK;
  k = mPullHits[k]<<(k*3);
  if (k>1000000) return kStSTOP;
  return kStOK;  
}  
