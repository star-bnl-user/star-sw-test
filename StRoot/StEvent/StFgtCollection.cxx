/***************************************************************************
 *
 * $Id: StFgtCollection.cxx,v 2.1 2012/04/16 20:20:49 ullrich Exp $
 * Author: S. Gliske, Oct 2011
 *
 ***************************************************************************
 *
 * Description: FGT data collection for StEvent.
 *
 ***************************************************************************
 *
 * $Log: StFgtCollection.cxx,v $
 * Revision 2.1  2012/04/16 20:20:49  ullrich
 * Initial Revision
 *
 *
 **************************************************************************/

#include "StFgtCollection.h"

// constructor
StFgtCollection::StFgtCollection() : StObject() {
    // set the disc field for some of the collections
    for( int i=0; i<kFgtNumDiscs; ++i ){
        mStripCollection[i].setDisc( i );
        mHitCollection[i].setDisc( i );
    }
}

// deconstructor
StFgtCollection::~StFgtCollection(){
    // nothing to do
}

void StFgtCollection::Clear( Option_t *opt ){
    for( int i=0; i<kFgtNumDiscs; ++i ){
        mStripCollection[i].Clear( opt );
        mHitCollection[i].Clear( opt );
    }
    mPointCollection.Clear( opt );
}

ClassImp(StFgtCollection);
