/***************************************************************************
 *
 * $Id: StRandyTopMapMaker.cxx,v 1.3 2000/04/03 20:28:10 rcwells Exp $
 *
 * Author: Torre Wenaus, BNL,
 *         Thomas Ullrich, Nov 1999
 ***************************************************************************
 *
 * Description:  This is an example of a maker to perform analysis
 *               using StEvent.
 *               Use this as a template and customize it for your
 *               studies.
 *
 ***************************************************************************
 *
 * $Log: StRandyTopMapMaker.cxx,v $
 * Revision 1.3  2000/04/03 20:28:10  rcwells
 * Removed remnant Pt tag in code from StAnalysisMaker.
 *
 * Revision 1.2  2000/04/02 20:41:17  rcwells
 * Fixed the FTPC bit check.
 *
 * Revision 2.1  1999/12/30 01:54:57  ogilvie
 * added countPrimaryPions as example how to use PID
 *
 * Revision 2.0  1999/11/04 16:10:03  ullrich
 * Revision for new StEvent
 *
 **************************************************************************/
#include "StRandyTopMapMaker.h"
#include "StChain.h"
#include "StEventTypes.h"
#include "StMessMgr.h"
#include "StEvent.h"
#include "StTrackGeometry.h"
#include "StTrackTopologyMap.h"

static const char rcsid[] = "$Id: StRandyTopMapMaker.cxx,v 1.3 2000/04/03 20:28:10 rcwells Exp $";

ClassImp(StRandyTopMapMaker)

StRandyTopMapMaker::StRandyTopMapMaker(const Char_t *name) : StMaker(name)
{
    drawinit = kFALSE;
}

StRandyTopMapMaker::~StRandyTopMapMaker() { /* noop */ }

Int_t
StRandyTopMapMaker::Init()
{
  return StMaker::Init();
}

void
StRandyTopMapMaker::Clear(Option_t *opt)
{
    StMaker::Clear();
}

Int_t
StRandyTopMapMaker::Finish()
{
    return kStOK;
}

Int_t
StRandyTopMapMaker::Make()
{
    //
    //	This method is called every event. That's the
    //  right place to plug in your analysis to be
    //  done every event.
    //
    StEvent* mEvent;
    mEvent = (StEvent *) GetInputDS("StEvent");
    if (! mEvent) return kStOK; // If no event, we're done
    StEvent& ev = *mEvent;
    
    // OK, we've got the event. Let's fix that Topology Map
    // This only fixes TPC pad rows and the turnaround flag
    StTrack* rTrack;
    int mult = mEvent->trackNodes().size();
    unsigned long mask2 = 2147483648;
    unsigned long temp1;
    for (unsigned long int icount=0; icount<(unsigned long int)mult; icount++){
      rTrack = mEvent->trackNodes()[icount]->track(global);
      // This checks bits that should not be set
      temp1 = rTrack->topologyMap().data(1) & mask2;
      // If FTPC track, don't do anything
      if ( int(temp1) ) continue;
      // End check of bits that should not be set
      int numHits = rTrack->detectorInfo()->hits(kTpcId).size();
      bool padRow[46];
      bool turnAround = false;
      int pad;
      int ipad;
      unsigned long map1 = 0;
      unsigned long map2 = 0;
      StTpcHit* myHit;
      // Zero pad rows
      for (int ipad=0; ipad<46; ipad++) padRow[ipad] = false;
      for (int ihit=0; ihit<numHits; ihit++) {
	myHit = (StTpcHit *)rTrack->detectorInfo()->hits(kTpcId)[ihit];
	pad = myHit->padrow();
	if ( padRow[pad] ) turnAround = true;
	padRow[pad] = true;
      }
      // Fill the map
      for (ipad=1; ipad<=24; ipad++) {
	if ( padRow[ipad] ) {
	  map1 |= 1UL<<(ipad+7);
	}
      }
      for (ipad=25; ipad<=45; ipad++) {
	if ( padRow[ipad] ) {
	  map2 |= 1UL<<(ipad-25);
	}
      }
      if ( turnAround ) map2 |= 1UL<<(30);
      StTrackTopologyMap newMap( map1,map2 );
      rTrack->setTopologyMap( newMap );
    }
    
    return kStOK;
}
