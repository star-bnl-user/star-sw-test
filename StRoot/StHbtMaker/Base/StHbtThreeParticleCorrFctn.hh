/***************************************************************************
 *
 * Description: part of STAR HBT Framework: StHbtMaker package
 *    base class for a STAR three particle correlation function.  
 *    Users should inherit from this and must implement constructor, 
 *    destructor, Report(), AddMixedTriplet(), AddRealTriplet(), Finish()
 *
 ***************************************************************************/

#ifndef StHbtThreeParticleCorrFctn_hh
#define StHbtThreeParticleCorrFctn_hh

//#include<string>
#include "StHbtMaker/Infrastructure/StHbtTypes.hh"
#include "StHbtMaker/Infrastructure/StHbtEvent.hh"
#include "StHbtMaker/Infrastructure/StHbtTriplet.hh"

class StHbtThreeParticleCorrFctn{

public:
  StHbtThreeParticleCorrFctn(){/* no-op */};
  virtual ~StHbtThreeParticleCorrFctn(){/* no-op */};

  //  virtual string Report() = 0;
  virtual StHbtString Report() = 0;

  virtual void AddRealTriplet(const StHbtTriplet*) = 0;
  virtual void AddMixedTriplet(const StHbtTriplet*) = 0;

  virtual void EventBegin(const StHbtEvent*) { /* no-op */ }
  virtual void EventEnd(const StHbtEvent*) { /* no-op */ }
  virtual void Finish() = 0;

  // the following allows "back-pointing" from the CorrFctn to the "parent" Analysis
  friend class StHbtThreeParticleAnalysis;
  StHbtThreeParticleAnalysis* HbtAnalysis(){return myAnalysis;};

protected:
  StHbtThreeParticleAnalysis* myAnalysis;

private:

};

#endif
