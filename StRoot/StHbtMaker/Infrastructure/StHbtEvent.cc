/***************************************************************************
 *
 * $Id: StHbtEvent.cc,v 1.3 1999/07/27 10:47:04 lisa Exp $
 *
 * Author: Mike Lisa, Ohio State, lisa@mps.ohio-state.edu
 ***************************************************************************
 *
 * Description: part of STAR HBT Framework: StHbtMaker package
 *   HbtEvent is the "transient microDST"  Objects of this class are
 *   generated from the input data by a Reader, and then presented to
 *   the Cuts of the various active Analyses.
 *
 ***************************************************************************
 *
 * $Log: StHbtEvent.cc,v $
 * Revision 1.3  1999/07/27 10:47:04  lisa
 * now works in dev on linux and solaris - mistake in deleting picoEvents fixed
 *
 * Revision 1.2  1999/07/19 14:24:05  hardtke
 * modifications to implement uDST
 *
 * Revision 1.1.1.1  1999/06/29 16:02:57  lisa
 * Installation of StHbtMaker
 *
 **************************************************************************/

#include "StHbtMaker/Infrastructure/StHbtEvent.hh"
//___________________
StHbtEvent::StHbtEvent(){
  mPrimVertPos[0]=-999.0;
  mPrimVertPos[1]=-999.0;
  mPrimVertPos[2]=-999.0;
  mTrackCollection = new StHbtTrackCollection;
  mV0TrackCollection = new StHbtV0TrackCollection;
}
//___________________
StHbtEvent::~StHbtEvent(){
  StHbtTrackIterator iter;
  for (iter=mTrackCollection->begin();iter!=mTrackCollection->end();iter++){
    delete *iter;
  }
  delete mTrackCollection;


  //must do the same for the V0 track collection
  StHbtV0TrackIterator V0iter;
  for (V0iter=mV0TrackCollection->begin();V0iter!=mV0TrackCollection->end();V0iter++){
    delete *V0iter;
  }
  delete mV0TrackCollection;
}
//___________________

