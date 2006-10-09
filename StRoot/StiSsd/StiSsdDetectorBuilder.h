// $Id: StiSsdDetectorBuilder.h,v 1.11 2006/10/09 15:47:59 fisyak Exp $
// 
// $Log: StiSsdDetectorBuilder.h,v $
// Revision 1.11  2006/10/09 15:47:59  fisyak
// use Normal represantation, remove StiDedxCalculator
//
// Revision 1.10  2006/06/28 18:51:46  fisyak
// Add loading of tracking and hit error parameters from DB
//
// Revision 1.9  2005/06/21 16:35:01  lmartin
// DetectorBuilder updated with the correct methods from StSsdUtil
//
// Revision 1.8  2005/06/21 15:31:47  lmartin
// CVS tags added
//
/*!
 * \class StiSsdDetectorBuilder
 * \author Christelle Roy
 * \date 02/27/04

This class is the description of the StiSsdDetectorBuilder
*/
#ifndef StiSsdDetectorBuilder_H
#define StiSsdDetectorBuilder_H
#include "Sti/StiDetectorBuilder.h"
#include "Sti/StiHitErrorCalculator.h"
#include "StThreeVector.hh"
class ssdWafersPosition_st;
class St_ssdWafersPosition;
class StiSsdDetectorBuilder : public StiDetectorBuilder
{

 public:
        StiSsdDetectorBuilder(bool active,const string & inputFile);
	virtual ~StiSsdDetectorBuilder(); 
	virtual void buildDetectors(StMaker& source);
	virtual void loadDS(TDataSet&);
	virtual void setDefaults();
	virtual void useVMCGeometry();		
	void         setSiMat(StiMaterial     *m) {_siMat = m;}
	void         setHybridMat(StiMaterial *m) {_hybridMat = m;}
	StiMaterial *getSiMat()    {return _siMat;}
	StiMaterial *getHybridMat(){return _hybridMat;}
 
 protected:
	StiMaterial *_siMat;
	StiMaterial *_hybridMat;
	StiDefaultHitErrorCalculator _hitCalculator;
	ssdWafersPosition_st *ssdWafersPosition(Int_t Id, St_ssdWafersPosition *wafers);
};
#endif 
