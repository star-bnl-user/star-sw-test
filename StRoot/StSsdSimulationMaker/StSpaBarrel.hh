// $Id: StSpaBarrel.hh,v 1.4 2006/09/15 21:09:52 bouchet Exp $
//
// $Log: StSpaBarrel.hh,v $
// Revision 1.4  2006/09/15 21:09:52  bouchet
// read the noise and pedestal from ssdStripCalib
//
// Revision 1.3  2005/11/22 03:56:45  bouchet
// id_mctrack is using for setIdTruth
//
// Revision 1.2  2005/05/13 08:39:32  lmartin
// CVS tags added
//

#ifndef STSPABARREL_HH
#define STSPABARREL_HH
#include <stdlib.h>
#include <math.h>
#include "tables/St_ssdDimensions_Table.h"
#include "tables/St_slsCtrl_Table.h"
#include "tables/St_sdm_calib_par_Table.h"
#include "tables/St_ssdStripCalib_Table.h"

#include "Random.hh"
#include "RanGauss.hh"
#include "StSpaWafer.hh"


class St_sls_strip;
class St_sdm_calib_db;
class St_sdm_condition_db;
class St_spa_strip;

class St_ssdStripCalib;

class StSpaBarrel
{
 public:
  StSpaBarrel(ssdDimensions_st  *geom_par, sdm_calib_par_st *cal_par);
  ~StSpaBarrel();

  void  setSpaParameters(ssdDimensions_st  *geom_par);
  int   readStripFromTable(St_sls_strip *sls_strip);
  int   readNoiseFromTable(St_ssdStripCalib *strip_calib);
  int   readConditionDbFromTable(St_sdm_condition_db *sdm_condition);
  int   writeStripToTable(St_spa_strip *spa_strip);
  int   writeStripToTable(St_spa_strip *spa_strip,St_sls_strip *sls_strip);
  void  addNoiseToStrip(slsCtrl_st *ctrl);
  void  doDaqSimulation(slsCtrl_st *ctrl);

  StSpaWafer** mWafers;
  
 private:
  int       mSsdLayer;
  int       mNLadder;
  int       mNWaferPerLadder;
  int       mNStripPerSide;
  RanGauss *mGaussDistribution;

  int       idWaferToWaferNumb(int idWafer);
  int       waferNumbToIdWafer(int waferNumb);
  
};
#endif
