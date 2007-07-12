#include "Stiostream.h"
#include <stdexcept>
#include <cmath>
#include <stdio.h>
#include "StEvent.h"
#include "StEventTypes.h"
#include "StDbUtilities/StTpcCoordinateTransform.hh"
#include "StDbUtilities/StTpcLocalSectorCoordinate.hh"
#include "StDbUtilities/StGlobalCoordinate.hh"
#include "StTpcDb/StTpcDb.h"
#include "Sti/Base/Factory.h"
#include "Sti/StiHit.h"
#include "Sti/StiHitContainer.h"
#include "Sti/StiDetector.h"
#include "Sti/StiDetectorBuilder.h"
#include "StiTpcHitLoader.h"
#include "Sti/StiHitTest.h"
//________________________________________________________________________________
StiTpcHitLoader::StiTpcHitLoader(): StiHitLoader<StEvent,StiDetectorBuilder>("TpcHitLoader"), 
				    _minRow(1), _maxRow(45), _minSector(1), _maxSector(24) {}
//________________________________________________________________________________
StiTpcHitLoader::StiTpcHitLoader(StiHitContainer* hitContainer,
                                 Factory<StiHit>*hitFactory,
                                 StiDetectorBuilder * detector)
: StiHitLoader<StEvent,StiDetectorBuilder>("TpcHitLoader",hitContainer,hitFactory,detector), 
				    _minRow(1), _maxRow(45), _minSector(1), _maxSector(24) {}
//________________________________________________________________________________
void StiTpcHitLoader::loadHits(StEvent* source,
                               Filter<StiTrack> * trackFilter,
                               Filter<StiHit> * hitFilter)
{
  cout << "StiTpcHitLoader::loadHits(StEvent*) -I- Started" << endl;
  if (!_detector)
    throw runtime_error("StiTpcHitLoader::loadHits(StEvent*) - FATAL - _detector==0");
  if(!_hitContainer)
    throw runtime_error("StiTpcHitLoader::loadHits(StEvent*) - FATAL - _hitContainer==0");

  StiDetector * detector;
  StiHit* stiHit;
  const StTpcHitCollection* tpcHits = source->tpcHitCollection();
  if (!tpcHits) return;
  unsigned int stiSector;
  for (unsigned int sector=_minSector-1; sector<_maxSector; sector++)    {
#if 0
    stiSector = sector;
#else
    if (sector<12)      stiSector = sector;
    else                stiSector = 11 - (sector-11)%12;
#endif
    const StTpcSectorHitCollection* secHits = tpcHits->sector(sector);
    if (!secHits) {
      cout << "StiTpcHitLoader::loadHits(StEvent* source) -W- no hits for sector:"<<sector<<endl;
      break;
    }
    //    for (unsigned int row=0; row<45; row++)
    Float_t driftvel = 1e-6*gStTpcDb->DriftVelocity(sector+1); // cm/mkmsec
    for (unsigned int row=_minRow-1; row<_maxRow; row++) {
      //cout << "StiTpcHitLoader:loadHits() -I- Loading row:"<<row<<" sector:"<<sector<<endl;
      const StTpcPadrowHitCollection* padrowHits = secHits->padrow(row);
      if (!padrowHits) break;
      const StSPtrVecTpcHit& hitvec = padrowHits->hits();
      detector = _detector->getDetector(row,stiSector);

      if (!detector) throw runtime_error("StiTpcHitLoader::loadHits(StEvent*) -E- Detector element not found");
      const_StTpcHitIterator iter;
      StiHitTest hitTest;
      for (iter = hitvec.begin();iter != hitvec.end();++iter)        {
        StTpcHit*hit=*iter;
        if(!_hitFactory) throw runtime_error("StiTpcHitLoader::loadHits(StEvent*) -E- _hitFactory==0");
        stiHit = _hitFactory->getInstance();
        if(!stiHit)   throw runtime_error("StiTpcHitLoader::loadHits(StEvent*) -E- stiHit==0");
        stiHit->reset();
        stiHit->setGlobal(detector,hit,hit->position().x(),hit->position().y(), hit->position().z(),hit->charge());
        hitTest.add(hit->position().x(),hit->position().y(), hit->position().z());
	if (hit->sector() <= 12) stiHit->setVz( driftvel);
	else                     stiHit->setVz(-driftvel);
        _hitContainer->add( stiHit );
	
      }
      if (hitTest.width()>0.1) {
	printf("**** TPC hits too wide (%g) sector=%d row%d\n"
	       ,hitTest.width(),sector,row);
      }
      
    }
  }
  cout << "StiTpcHitLoader::loadHits(StEvent*) -I- Done" << endl;
}
//________________________________________________________________________________
