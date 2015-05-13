// \class StFpsRawHitMaker
// \author Akio Ogawa
//
//  $Id: StFpsRawHitMaker.cxx,v 1.1 2015/03/23 11:33:20 jeromel Exp $
//  $Log: StFpsRawHitMaker.cxx,v $
//  Revision 1.1  2015/03/23 11:33:20  jeromel
//  Peer review closed 2015/03/20 - content added
//

#include "StFpsRawHitMaker.h"

#include "St_base/StMessMgr.h"
#include "St_base/Stypes.h"
#include "RTS/src/DAQ_FPS/daq_fps.h"
#include "RTS/src/DAQ_READER/daq_dta.h"
#include "StChain/StRtsTable.h"
#include "StEvent/StEvent.h"
#include "StEvent/StFmsCollection.h"
#include "StEvent/StFmsHit.h"
#include "StFmsDbMaker/StFmsDbMaker.h"

StFpsRawHitMaker::StFpsRawHitMaker(const Char_t* name) :
  StRTSBaseMaker( "adc", name ), mPrePost(0), mFmsCollection(0){}

StFpsRawHitMaker::~StFpsRawHitMaker(){}

/// Check if global pointer for StFmsDbMaker is available
Int_t StFpsRawHitMaker::InitRun(Int_t runNumber){
  LOG_DEBUG << "StFpsRawHitMaker::InitRun with run = "  << runNumber << endm;
  if(!gStFmsDbMaker){
    LOG_ERROR  << "StFpsRawHitMaker::InitRun Failed to get gStFmsDbMaker" << endm;
    return kStFatal;
  }
  return kStOK;
}

/// Searches for StEvent and StFmsCollection, and create StFmsCollection if not found
Int_t StFpsRawHitMaker::prepareEnvironment(){
  StEvent* eventPtr=0;
  eventPtr= (StEvent*)StRTSBaseMaker::GetInputDS("StEvent");
  mFmsCollection=0;
  if(eventPtr){
    LOG_DEBUG <<"StFpsRawHitMaker::prepareEnvironment found StEvent" <<endm;
    mFmsCollection=eventPtr->fmsCollection();
  }else{
    LOG_ERROR <<"StFpsRawHitMaker::prepareEnvironment found no StEvent"<<endm;
    return kStFatal;
  }
  if(!mFmsCollection){
    LOG_DEBUG <<"StFpsRawHitMaker::prepareEnvironment creating new StFmsCollection" <<endm;
    mFmsCollection=new StFmsCollection;
    if(!mFmsCollection){
      LOG_ERROR <<"StFpsRawHitMaker::prepareEnvironment could not create StFmsCollection" <<endm;
      return kStFatal;
    }
    LOG_DEBUG <<"StFpsRawHitMaker::prepareEnvironment setting new StFmsCollection to StEvent" <<endm;
    eventPtr->setFmsCollection(mFmsCollection);
  }  
  return kStOK;
}

/// Read FPS data from daq file, and add to StFmsCollection as a StFmsHit
Int_t StFpsRawHitMaker::Make(){
  StRtsTable* rts_tbl=0;
  int n=0, ngood=0;
  if(int ret=prepareEnvironment()!=kStOK ) return ret;

  // loop over DaqElements of FPS data in daq file
  // A DaqElement contains data from one QT baord and one bunch crossings
  while((rts_tbl = GetNextDaqElement("fps/adc"))){    
    int xing=rts_tbl->Sector(); 
    if(xing>=128) xing-=256;
    int qt=rts_tbl->Rdo();
    int ndata=rts_tbl->GetNRows();
    LOG_DEBUG << Form("FPS: xing=%4d QT=%02d NData=%d",xing,qt,ndata)<<endm;
    if(mPrePost==0 && xing!=0) continue;  
    for(StRtsTable::iterator it=rts_tbl->begin(); it!=rts_tbl->end(); it++){
      fps_adc_t *a=(fps_adc_t *)*it;
      //fps_adc_t *a=static_cast<fps_adc_t *>(*it);  //would not work! use "reinterpret_cast".
      int ch=a->ch;
      int adc=a->adc;
      int tdc=a->tdc;  
      int slatid = gStFmsDbMaker->fpsSlatidFromQT(qt,ch); //Get SlatId from QT address and channel
      int q,l,s;
      gStFmsDbMaker->fpsQLSfromSlatId(slatid,&q,&l,&s); //Get Quad/Layer/Slat#s from SlatId
      LOG_DEBUG << Form("FPS: xing=%4d QT%02d ch%02d Slaiid=%3d Q%1dL%1dS%02d ADC=%4d TDC=%2d",xing,qt,ch,slatid,q,l,s,adc,tdc)<<endm;
      n++;
      int flag=0;	
      if(slatid<0)          { /* LOG_WARN << "Invalid SlatId = "<<slatid<<endm;*/      flag=1; }
      if(q<0 || l<1 || s<1) { /* LOG_WARN << Form("Invalid Q/L/S = %d/%d/%d",q,l,s);*/ flag=1; }
      if(flag==0){
	StFmsHit* hit = new StFmsHit();
	hit->setDetectorId(kFpsDetId);	 
	hit->setChannel(slatid);
	hit->setQtCrate(kFpsQtCrate);
	hit->setQtSlot(qt);
	hit->setQtChannel(ch);
	hit->setAdc(adc);
	hit->setTdc(tdc);
	hit->setEnergy(0.0);
	mFmsCollection->addHit(hit);
	if(Debug()) hit->print();	 
	ngood++;
      }
    }
  }
  LOG_DEBUG << Form("StFpsRawHitMaker:: found total %d hits and %d good",n,ngood) << endm;
  return kStOK;
}

ClassImp(StFpsRawHitMaker)