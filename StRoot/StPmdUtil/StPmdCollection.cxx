 /**********************************************
 *
 * $Id: StPmdCollection.cxx,v 1.3 2003/10/14 10:16:31 subhasis Exp $
 *
 * Author: Subhasis Chattopadhyay, July 2002.
 ***********************************************
 *
 * Description: Base class for PMD collection
 *
 ***********************************************
 * $Log: StPmdCollection.cxx,v $
 * Revision 1.3  2003/10/14 10:16:31  subhasis
 * zeroed before delete
 *
 * Revision 1.2  2003/05/12 12:07:13  subhasis
 * Mapping added
 *
 ***********************************************/
#include "StPmdCollection.h"
#include "StPmdDetector.h"

ClassImp(StPmdCollection)

StPmdCollection::StPmdCollection(Char_t * name):TDataSet(name) {
  for(int i=0; i<2; i++){
   if(mDetector[i]) mDetector[i]=0;
   if(mDetector[i]) delete mDetector[i];
       StPmdDetector * det = new StPmdDetector(i,12);
      this->setDetector(det,i);
  }
}

StPmdCollection::~StPmdCollection(){
  for(int i=0; i<2; i++){
    if(mDetector[i]) delete mDetector[i];
  }
}
    
StPmdDetector*
StPmdCollection::detector(Int_t id)
{
    if(id >= 0 && id <= 1)
        return mDetector[id];
    else
        return 0;
}

void
StPmdCollection::setDetector(StPmdDetector* val, Int_t id)
{
    if (val) {
        if (id >= 0 && id <= 1) {

	    if (mDetector[id]) mDetector[id]=0;
            if (mDetector[id]) delete mDetector[id];
            mDetector[id] = val;
        }
    }
}



