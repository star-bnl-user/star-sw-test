/**********************************************************
 *
 * Here are defined the input/output stream operators for
 *  StHbtEvent and associated classes
 *
 *********************************************************/

#ifndef StHbtIO_cc
#define StHbtIO_cc

#include "StHbtMaker/Infrastructure/StHbtV0.hh"
#include "StHbtMaker/Infrastructure/StHbtTrack.hh"
#include "StHbtMaker/Infrastructure/StHbtEvent.hh"

#include <float.h>    // these tell maximum values of types so we
#include <limits.h>   // don't write the characters "inf" to our microDST


//------------------------- StHbtTrack -----------------------------------
ostream& operator<<(ostream& out, StHbtTrack& trk){
  // sometimes ChiSqXY is infinity, so output largest number instead
  float ChiSqXYtemp = (fabs(trk.mChiSqXY)>FLT_MAX) ? FLT_MAX : trk.mChiSqXY;

  return (out
	  << trk.mCharge     << " " << trk.mNHits        << " "
	  << trk.mNHitsPoss  << " " << trk.mNSigmaPion   << " "
	  << trk.mNSigmaKaon << " " << trk.mNSigmaProton << " "
	  << trk.mdEdx       << " " << trk.mDCAxy        << " "
	  << trk.mDCAz       << " " << ChiSqXYtemp       << " "
	  << trk.mChiSqZ     << " " << trk.mP.x()        << " "
	  << trk.mP.y()      << " " << trk.mP.z()        << " "
	  // now for the StPhysicalHelixD...
	  << trk.mHelix.curvature() << " " << trk.mHelix.dipAngle() << " "
	  << trk.mHelix.phase()     << " " 
	  << trk.mHelix.origin().x() << " " << trk.mHelix.origin().y() << " " << trk.mHelix.origin().z() << " "
	  << trk.mHelix.h());
}

istream& operator>>(istream& in,  StHbtTrack& trk){
  double x,y,z;
  double curvature, dipAngle, phase;  // for PhysicalHelix
  double xorigin,yorigin,zorigin;     // for PhysicalHelix
  int h;                              // for PhysicalHelix
  in
    >> trk.mCharge     >> trk.mNHits       
    >> trk.mNHitsPoss  >> trk.mNSigmaPion  
    >> trk.mNSigmaKaon >> trk.mNSigmaProton
    >> trk.mdEdx       >> trk.mDCAxy       
    >> trk.mDCAz       >> trk.mChiSqXY     
    >> trk.mChiSqZ     >> x
    >> y               >> z
    // now for the StPhysicalHelixD...
    >> curvature       >> dipAngle
    >> phase           >> xorigin
    >> yorigin         >> zorigin
    >> h;
  // set up P
  trk.mP.setX(x); 
  trk.mP.setY(y); 
  trk.mP.setZ(z); 
  // Pt is derived
  trk.mPt = sqrt(x*x+y*y);
  // now set up the StPhysicalHelixD
  StThreeVectorD origin(xorigin,yorigin,zorigin);
  trk.mHelix.setParameters(curvature,dipAngle,phase,origin,h);
  return in;
}

//------------------------- StHbtV0 -----------------------------------
ostream& operator<<(ostream& out, StHbtV0& v0){

  return (out
	  << v0.mdecayLengthV0    << " " << v0.mdecayVertexV0.x()  << " "
	  << v0.mdecayVertexV0.y()<< " " << v0.mdecayVertexV0.z()  << " "
	  << v0.mdcaV0Daughters   << " " << v0.mdcaV0ToPrimVertex  << " "
	  << v0.mdcaPosToPrimVertex <<" "<< v0.mdcaNegToPrimVertex << " "
	  << v0.mmomPos.x()       << " " << v0.mmomPos.y()         << " "
          << v0.mmomPos.z()       << " " << v0.mmomNeg.x()         << " "
	  << v0.mmomNeg.y()       << " " << v0.mmomNeg.z()         << " " 
	  << v0.mtpcHitsPos       << " " << v0.mtpcHitsNeg         << " "    
	  << v0.mrapLambda        << " " << v0.mrapK0Short         << " "
          << v0.mcTauLambda       << " " << v0.mcTauK0Short        << " "
          << v0.midNeg            << " " << v0.midPos     );
}

//------------------------- StHbtV0 -----------------------------------
  istream& operator>>(istream& in, StHbtV0& v0){
 

   in 
	  >> v0.mdecayLengthV0      >>  v0.mdecayVertexV0   
	  >> v0.mdcaV0Daughters     >>  v0.mdcaV0ToPrimVertex 
	  >> v0.mdcaPosToPrimVertex >>  v0.mdcaNegToPrimVertex
	  >> v0.mmomPos             >>  v0.mmomNeg      
	  >> v0.mtpcHitsPos         >>  v0.mtpcHitsNeg                
	  >> v0.mrapLambda          >>  v0.mrapK0Short 
          >> v0.mcTauLambda         >>  v0.mcTauK0Short 
          >> v0.midNeg              >>  v0.midPos;
   v0.UpdateV0();
   return (in) ;
}

//------------------------- StHbtEvent -----------------------------------
ostream& operator<<(ostream& out, StHbtEvent& ev){
  out
    << ev.mEventNumber         << " " << ev.mCtbMultiplicity  << " "
    << ev.mZdcAdc[0]           << " " << ev.mZdcAdc[1]         << " "
    << ev.mTpcNhits            << " " << ev.mNumberOfTracks   << " "
    << ev.mNumberOfGoodTracks  << " " << ev.mReactionPlane[0] << " "
    << ev.mReactionPlane[1]    << " " << ev.mPrimVertPos.x()   << " " 
    << ev.mPrimVertPos.y()      << " " << ev.mPrimVertPos.z()   << endl;
  out << ev.mTrackCollection->size() << endl;;
  StHbtTrack trk;
  for (StHbtTrackIterator iter=ev.mTrackCollection->begin();
       iter != ev.mTrackCollection->end(); iter++){
    trk= **iter;              // correct???
    out << trk << endl;   // don't forget the endl to seperate them...
  } 
  // now we do the v0 collection...
  out << ev.mV0Collection->size() << endl;;
  StHbtV0 v0;
  for (StHbtV0Iterator iterv0=ev.mV0Collection->begin();
       iterv0 != ev.mV0Collection->end(); iterv0++){
    v0 = **iterv0;              // correct???
    out << v0 << endl;   // don't forget the endl to seperate them...
  } 

  out << endl; // blank-line delimiter between events
  return out;
}

istream& operator>>(istream& in,  StHbtEvent& ev){ 

  double x,y,z; 
  in >> ev.mEventNumber;
  if (in.eof()) {
    cout << "Hit end of file " << endl;
    return in;
  }
  in >> ev.mCtbMultiplicity 
    >> ev.mZdcAdc[0]           >> ev.mZdcAdc[1]         
    >> ev.mTpcNhits            >> ev.mNumberOfTracks  
    >> ev.mNumberOfGoodTracks  >> ev.mReactionPlane[0]
    >> ev.mReactionPlane[1]    >> x
    >> y                       >> z;
  ev.mPrimVertPos.setX(x);
  ev.mPrimVertPos.setY(y);
  ev.mPrimVertPos.setZ(z);
  // 
  //  OK, time to read in Track and V0 collections
  // 
  long NtracksInCollection;
  in >> NtracksInCollection;
  if (!(in.good())){
    cout << "StHbtEvent input operator finds stream in bad state ! " << endl;
    return in;
  }
  //  ev.mTrackCollection = new StHbtTrackCollection; <-- NO!
  //  the TrackCollection is instantiated by constructor!!
  //
  // since this should *overwrite* any StHbtTracks in the
  // StHbtTrackCollection, let's erase any that might be there
  //
  StHbtTrackIterator iter;
  for (iter=ev.mTrackCollection->begin();iter!=ev.mTrackCollection->end();iter++){
    delete *iter;
  }
  // ok, now we have gotten rid of the tracks themselves.  Let's lose the pointers to those deleted tracks
  ev.mTrackCollection->clear();  // if this doesn't work then just delete the collection and make a new one.

  for (int itrk=0; itrk<NtracksInCollection; itrk++){
    StHbtTrack* trk = new StHbtTrack;
    if ( !(in >> (*trk))){
      cout << "StHbtEvent input operator finds stream in bad state during track read ! ";
      cout << itrk << " of " << NtracksInCollection << " intended" << endl;
      return in;
    }
    ev.mTrackCollection->push_back(trk);  // ?ok?
    //cout << " " << itrk;
  }
  // now we should do the v0 collection...
 
 long NV0sInCollection;
  in >> NV0sInCollection;
  if (!(in.good())){
    cout << "StHbtEvent input operator finds stream in bad state ! " << endl;
    return in;
  }
  // since this should *overwrite* any StHbtV0s in the
  // StHbtV0Collection, let's erase any that might be there
  //
  StHbtV0Iterator iterv0;
  for (iterv0=ev.mV0Collection->begin();iterv0!=ev.mV0Collection->end();iterv0++){
    delete *iterv0;
  }
  // ok, now we have gotten rid of the v0s themselves.  Let's lose the pointers to those deleted v0ss
  ev.mV0Collection->clear();  // if this doesn't work then just delete the collection and make a new one.

  for (int iv0=0; iv0<NV0sInCollection; iv0++){
    StHbtV0* v0 = new StHbtV0;
    if ( !(in >> (*v0))){
      cout << "StHbtEvent input operator finds stream in bad state during v0 read ! ";
      cout << iv0 << " of " << NV0sInCollection << " intended" << endl;
      return in;
    }
    ev.mV0Collection->push_back(v0);  // ?ok?
    //cout << " " << iv0;
  }

  return in;
}

#endif
