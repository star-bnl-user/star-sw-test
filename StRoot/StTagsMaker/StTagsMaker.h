// $Id: StTagsMaker.h,v 1.5 2004/07/29 22:56:59 fisyak Exp $
// $Log: StTagsMaker.h,v $
// Revision 1.5  2004/07/29 22:56:59  fisyak
// Add Global tags
//
// Revision 1.4  2003/09/10 19:47:37  perev
// ansi corrs
//
// Revision 1.3  2000/03/28 19:09:37  fine
// Adjuested to ROOT 2.24
//
// Revision 1.2  2000/02/02 21:20:56  fisyak
// Remove user parametes from GetTags
//
// Revision 1.1.1.1  2000/01/27 18:54:00  fisyak
// Initial revision of Tags Maker
//
#ifndef StTagsMaker_H
#define StTagsMaker_H

#include "StMaker.h"
#include "St_DataSet.h"

class TTree;
class EvtHddr_st;
class HighPtTag_st; 
class FlowTag_st;   
class HbtTag_st;    
class PCollTag_st;  
class ScaTag_st;    
class StrangeTag_st;
class TClass;
class StTagsMaker : public StMaker {
 private:
// static Char_t  m_VersionCVS = "$Id: StTagsMaker.h,v 1.5 2004/07/29 22:56:59 fisyak Exp $";
 protected:
 public: 
                  StTagsMaker(const char *name="tags");
   virtual       ~StTagsMaker();
   virtual Int_t Init();
   virtual Int_t InitTags();
   virtual Int_t Make();
   static EDataSetPass  GetTags (St_DataSet* ds);
   virtual const char *GetCVS() const
  {static const char cvs[]="Tag $Name:  $ $Id: StTagsMaker.h,v 1.5 2004/07/29 22:56:59 fisyak Exp $ built "__DATE__" "__TIME__ ; return cvs;}

   ClassDef(StTagsMaker,0)   //StAF chain virtual base class for Makers
};

#endif
