/****************************************************
 *
 * $Id: StPmdAbsClustering.cxx,v 1.4 2003/09/02 17:58:48 perev Exp $
 *
 * Author: Subhasis Chattopadhyay
 *
 ******************************************************
 *
 * Description: This is for Abstract PMD cluster Finder
 *
 ******************************************************
 *
 * $Log: StPmdAbsClustering.cxx,v $
 * Revision 1.4  2003/09/02 17:58:48  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 1.3  2003/05/14 10:49:04  subhasis
 * CPV clustering added
 *
 *
 ******************************************************/

#include<Stiostream.h>
#include<assert.h>
#include<math.h>
#include"TROOT.h"
#include<TRandom.h>
#include<TBrowser.h>
#include<TPad.h>
#include<StMessMgr.h>
#include<TFile.h>


#include <TTableSorter.h>

#include "StBFChain.h"
#include "StPmdUtil/StPmdHit.h"
#include "StPmdAbsClustering.h"
#include "StPmdClusterMaker.h"
//#include "StPmdUtil/StPmdClusterCollection.h"
//#include "StPmdUtil/StPmdCluster.h"
//#include "StPmdUtil/StPmdModule.h"
//#include "StPmdUtil/StPmdDetector.h"



ClassImp(StPmdAbsClustering)

//-----------------------------------
StPmdAbsClustering::StPmdAbsClustering(StPmdDetector *pmd_det, StPmdDetector *cpv_det){
 m_pmd_det=pmd_det;
 m_cpv_det=cpv_det;
}
//------------------------------
StPmdAbsClustering::~StPmdAbsClustering()
{
}
//---------------------------------

