// $Id: StMaker.cxx,v 1.54 1999/07/13 02:19:34 perev Exp $
// $Log: StMaker.cxx,v $
// Revision 1.54  1999/07/13 02:19:34  perev
// GetCVS,StEvtHddr,etc...
//
// Revision 1.53  1999/07/12 02:33:09  perev
// Add SetMode
//
// Revision 1.52  1999/07/11 21:04:06  fisyak
// Clash resolion
//
// Revision 1.51  1999/07/11 20:40:35  perev
// Move Clear from StChain to StMaker
//
// Revision 1.50  1999/07/11 01:59:04  perev
// add GetCVSTag again
//
// Revision 1.49  1999/07/11 01:33:33  fine
// makedoc some corrections for MakeDoc
//
// Revision 1.48  1999/07/09 22:00:22  perev
// GetCVS into StMaker
//
// Revision 1.47  1999/06/11 23:45:31  perev
// cleanup
//
// Revision 1.46  1999/06/11 22:56:03  perev
// Merge 2 updates
//
// Revision 1.45  1999/06/11 21:50:47  perev
// garb->Delete()
//
// Revision 1.44  1999/06/11 17:45:57  perev
// assert StMaker::Streamer to forbid to write it
//
// Revision 1.43  1999/05/23 04:05:02  fine
// The lost since 1.35 Wed Mar 10 20:23:58 timer functions have been re-introduced
//
// Revision 1.42  1999/05/23 03:25:07  perev
// Start & Stop Timer instead of benchmark
//
// Revision 1.41  1999/05/22 17:50:18  perev
// StMaker::EndMaker ps added
//
// Revision 1.40  1999/05/13 20:56:50  perev
// Supress too much warnings
//
// Revision 1.39  1999/05/10 17:16:44  perev
// AddHist typo
//
// Revision 1.38  1999/05/10 15:37:51  perev
// Save of hisogramm in StMaker::Init
//
// Revision 1.37  1999/05/07 20:51:31  perev
// AddData bug fix
//
// Revision 1.36  1999/05/07 15:46:10  perev
// Added test for the same object into AddObj
//
// Revision 1.35  1999/05/06 22:15:32  perev
// fix objLast should not be included in StMaker::Init
//
// Revision 1.34  1999/05/06 21:27:10  perev
// StMaker remove his from hdirectory
//
// Revision 1.33  1999/05/06 00:47:43  fine
// maker's histogram is removed from the ROOT system gDirectory
//
// Revision 1.32  1999/05/06 00:23:45  fine
// StMaker::MakeDoc some extra comments have been introduced
//
// Revision 1.31  1999/05/06 00:19:04  fine
// StMaker::MakeDoc method has been re-introduced for the 3d time
//
// Revision 1.30  1999/05/05 16:23:07  perev
// add recreation of m_DataSet to keep old codes
//
// Revision 1.29  1999/05/03 22:29:28  perev
// Bug in GetDataSet fix. Thanks to Bill Love
//
// Revision 1.28  1999/05/01 00:53:38  perev
// GetDataSet bug fix NAME == NAME/.data
//
// Revision 1.26  1999/04/30 14:58:41  perev
//  cd() added to StMaker class
//
// Revision 1.25  1999/04/16 14:22:00  fisyak
// replace break in Makers loop from ==kStErr to >kStWarn to account EOF
//
// Revision 1.24  1999/03/28 02:57:51  perev
// Add .const in searching path in GetDataSet
//
// Revision 1.23  1999/03/20 20:57:35  perev
// add StEvtHddr.h and fix Get/SetNumber in maker
//
// Revision 1.22  1999/03/19 20:30:49  perev
// GetCVSTag introduced
//
// Revision 1.21  1999/03/11 01:23:59  perev
// new schema StChain
//
// Revision 1.14  1998/12/21 19:42:51  fisyak
// Move ROOT includes to non system
//
// Revision 1.13  1998/11/19 01:23:57  fine
// StChain::MakeDoc has been introduced, StChain::MakeDoc has been fixed (see macros/bfc_doc.C macro
//
// Revision 1.12  1998/11/18 22:46:09  fine
// The lost MakeDoc method has been re-introduced
//
// Revision 1.9  1998/09/23 20:22:52  fisyak
// Prerelease SL98h
//
// Revision 1.10  1998/10/06 18:00:27  perev
// cleanup
// Revision 1.8  1998/09/22 01:39:07  fine
// Some make up
//
// Revision 1.6  1998/08/18 14:05:02  fisyak
// Add to bfc dst
//
// Revision 1.5  1998/07/20 15:08:09  fisyak
// Add tcl and tpt
//
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// StChain virtual base class for StMaker                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TSystem.h"
#include "TClass.h"
#include "TROOT.h"
#include "THtml.h"
#include "TH1.h"

#include "TChain.h"
#include "TTree.h"
#include "TList.h"
#include "TClonesArray.h"
#include "TBrowser.h"

#include "StMaker.h"
#include "StChain.h"
#include "St_Table.h"

StMaker *StMaker::fgStChain = 0;
Int_t MaxWarnings = 26;

ClassImp(StMaker)

static void doPs(const char *who,const char *where);

//_____________________________________________________________________________
StMaker::StMaker()
{
::doPs(GetName(),"constructor");
  SetMode();
}

//_____________________________________________________________________________
StMaker::StMaker(const char *name,const char *):St_DataSet(name,".maker")
{
   SetMode();
   m_Inputs = 0;
   if (!fgStChain) {	// it is first maker, it is chain
     fgStChain = this;
     AddData(0,".make");
   } else         {	// add this maker to chain  
     fgStChain->AddData(this,".make");
   }
   m_DataSet  = new St_ObjectSet(".data") ;Add(m_DataSet);
   m_ConstSet = new St_ObjectSet(".const");Add(m_ConstSet);
   m_GarbSet  = new St_ObjectSet(".garb" );Add(m_GarbSet);
   m_Inputs  =  new St_ObjectSet(".aliases" );Add(m_Inputs);
   AddHist(0); m_Histograms = GetHistList();
   gStChain = this; //?????????????????????????????????????????????????????
::doPs(GetName(),"constructor");
}

//_____________________________________________________________________________
StMaker::~StMaker()
{
  Finish();
}
//______________________________________________________________________________
void StMaker::SetNumber(Int_t number)
{
 StMaker *par = GetParentMaker();
 if (par) par->SetNumber(number);
 m_Number = number;
}
//______________________________________________________________________________
Int_t StMaker::GetNumber() const
{
 StMaker *par = GetParentMaker();
 if (par) return par->GetNumber();
 return m_Number;
}
//______________________________________________________________________________
StMaker *StMaker::GetParentMaker() const
{ 
  St_DataSet *par = GetParent(); if (!par) return 0;
  return (StMaker*)par->GetParent();
}
//______________________________________________________________________________
StMaker *StMaker::GetMaker(const char *mkname) 
{ 
  TString path(".make/"); path+=mkname;
  return (StMaker*)GetDataSet((const char*)path);
}
//______________________________________________________________________________
TObject *StMaker::GetDirObj(const char *dir) const
{
  St_ObjectSet *h = (St_ObjectSet*)Find(dir);
  if (!h) return 0;
  return h->GetObject();
}
//______________________________________________________________________________
void StMaker::SetDirObj(TObject *obj,const char *dir)
{ 
  St_ObjectSet *set = (St_ObjectSet *)Find(dir);
  if (!set) { // No dir, make it
    set = new St_ObjectSet(dir); Add(set);}
  set->SetObject(obj);
}
//______________________________________________________________________________
St_ObjectSet *StMaker::AddObj(TObject *obj,const char *dir)
{ 
  assert (dir[0]=='.');
  St_ObjectSet *set = (St_ObjectSet*)Find(dir);
  if (!set) { // No dir, make it
    set = new St_ObjectSet(dir); Add(set);}

  TList *list = (TList *)set->GetObject();
  if (!list) {// No list, make it
    list = new TList();
    set->SetObject((TObject*)list);}
  if (!obj) return set;
  if(!list->FindObject(obj)) list->Add(obj);
  return set;
}
//______________________________________________________________________________
 void StMaker::AddHist(TH1 *h,const char *dir)
{  
  if (dir){/*unused*/}
  if (!h) {AddObj(0,".hist");return;}
  if (h->InheritsFrom(TH1::Class())) h->SetDirectory(0);
  AddObj(h,".hist");
}    




//______________________________________________________________________________
St_DataSet *StMaker::AddData(St_DataSet *ds, const char* dir)
{ 
  assert (dir); assert(dir[0]=='.');
  St_DataSet *set = Find(dir);
  if (!set) { // No dir, make it
    set = new St_ObjectSet(dir); Add(set);}
  if (!ds) return set;
  TList *tl = set->GetList();
  if (!tl || !tl->FindObject(ds)) set->Add(ds);
  return set;
}
//______________________________________________________________________________
St_DataSet  *StMaker::GetData(const char *name, const char* dir) const
{ 
  St_DataSet *set = Find(dir);
  if (!set) return 0;
  return set->Find(name);
}
//______________________________________________________________________________
void StMaker::AddAlias(const char* log, const char* act,const char* dir)
{
  St_DataSet *ali = new St_DataSet(log); 
  ali->SetTitle(act);
  AddData(ali,dir);
}
//______________________________________________________________________________
void StMaker::SetAlias(const char* log, const char* act,const char* dir)
{ 
  St_DataSet *ali = GetData(log,dir);
  if (ali) {
    if (!strcmp(act,ali->GetTitle())) return;
  } else {
    ali = new St_DataSet(log); AddData(ali,dir);
  }
  ali->SetTitle(act);

  if (GetDebug()) 
    printf("<%s(%s)::SetAlias> %s = %s\n",ClassName(),GetName(),log,act);
}
//______________________________________________________________________________
void StMaker::SetOutput(const char* log,St_DataSet *ds)
{
  int idx;
  const char* logname = log;
  if (!logname || !logname[0]) logname = ds->GetName();
  TString act = ds->Path();
  while ((idx=act.Index(".make/"))>=0) act.Replace(0,idx+6,"");  
  SetOutput(logname,act); 
}

//______________________________________________________________________________
void StMaker::SetOutputAll(St_DataSet *ds)
{
  St_DataSet *set;
  St_DataSetIter next(ds);
  while ((set = next())) SetOutput(set);
}

//______________________________________________________________________________
TList *StMaker::GetMakeList() const
{ St_DataSet *ds = Find(".make");
  if (!ds) return 0;
  return ds->GetList();
}
//______________________________________________________________________________
TString StMaker::GetAlias(const char* log,const char* dir) const
{
  TString act;
  int nspn = strcspn(log," /");
  act.Prepend(log,nspn);
  St_DataSet *in = GetData(act,dir);
  act ="";
  if (in) {act = in->GetTitle(); act += log+nspn;}
  return act;
}
//______________________________________________________________________________
St_DataSet *StMaker::GetDataSet(const char* logInput,
                                const StMaker *uppMk,
                                const StMaker *dowMk) const
{
St_DataSetIter nextMk(0);
TString actInput,findString,tmp;
St_DataSet *dataset,*dir;
StMaker    *parent,*mk;
int icol,islas;
  
  actInput = GetInput(logInput);
  if (actInput.IsNull()) actInput = logInput;
  

//		Direct try
  dataset = 0;
  if (actInput.Contains("."))  dataset = Find(actInput);
  if (dataset) goto FOUND;
  
  if (actInput==GetName()) dataset = m_DataSet;
  if (dataset) goto FOUND;

//		Not so evident, do some editing
  
  
  icol = actInput.Index(":");
  if (icol>=0) {//there is maker name is hidden
    tmp = actInput; 
    tmp.Replace(0,0,".make/"); icol +=6;
    tmp.Replace(icol,1,"/.data/");
    dataset = Find((const char*)tmp);  		// .make/MAKER/.data/...
    if (dataset) goto FOUND;
    dataset = Find((const char*)tmp+6);		//       MAKER/.data/...
    if (dataset) goto FOUND;
    tmp.Replace(icol,7,"/.const/");
    dataset = Find((const char*)tmp);		// .make/MAKER/.const/...
    if (dataset) goto FOUND;
    dataset = Find((const char*)tmp+6);		//       MAKER/.const/...
    if (dataset) goto FOUND;
    goto DOWN;
  }

  assert(m_DataSet);
  islas = actInput.Index("/");
  if (islas>0) {
    tmp.Replace(0,999,actInput,islas);
    if (tmp == GetName()) { // 
      tmp = actInput;
      tmp.Replace(0,islas+1,"");
      dataset = m_DataSet->Find(tmp);
      if (dataset) goto FOUND;
      dataset = m_ConstSet->Find(tmp);
      if (dataset) goto FOUND;
    }
  }

  dataset = m_DataSet->Find(actInput);
  if (dataset) goto FOUND;
  dataset = m_ConstSet->Find(actInput);
  if (dataset) goto FOUND;


//	Try to search DOWN
DOWN: if (!(dir = Find(".make"))) goto UP;

  nextMk.Reset(dir);
  while ((mk = (StMaker*)nextMk()))
  {
    if (mk==dowMk) continue;
    dataset = mk->GetDataSet(actInput,this,0);
    if (dataset) goto FOUND;
  }

//     Try to search UP
UP: if (uppMk) return 0;

  parent = GetMaker(this); if (!parent) goto NOTFOUND;
  dataset = parent->GetDataSet(actInput,0,this);
  if (dataset) goto FOUND;

//		Not FOUND
NOTFOUND:
  if (!dowMk && GetDebug()) //Print Warning message
    if ((MaxWarnings--) > 0) Warning("GetDataSet"," \"%s\" Not Found ***\n",(const char*)actInput);
  return 0;

//		DataSet FOUND
FOUND: if (uppMk || dowMk) 	return dataset;
       if (GetDebug()<2) 	return dataset;
  printf("Remark: <%s::%s> DataSet %s FOUND in %s\n"
  ,ClassName(),"GetDataSet",logInput,(const char*)dataset->Path());

  return dataset;

}
//______________________________________________________________________________
St_DataSet *StMaker::GetDataBase(const char* logInput)
{
  St_DataSet *ds;
  StMaker *mk;
  ds = GetInputDS(logInput);
  if (!ds) return 0;
  mk = GetMaker(ds); if (!mk) return 0;
  return mk->UpdateDB(ds);
}
//_____________________________________________________________________________
void StMaker::Clear(Option_t *option)
{
  if(option){};
  if (m_DataSet) m_DataSet->Delete();
//    Reset lists of event objects
   
   TIter next(GetMakeList());
   StMaker *maker;
   while ((maker = (StMaker*)next())) {
      maker->Clear(option);
   }
   return;

}
//_____________________________________________________________________________
Int_t StMaker::Init()
{   
   TObject  *objLast,*objHist;
   TList *tl = GetMakeList();
   if (!tl) return kStOK;
   
   TIter nextMaker(tl);
   StMaker *maker;

   while ((maker = (StMaker*)nextMaker())) {

     // save last created histogram in current Root directory
      gROOT->cd();
      objLast = gDirectory->GetList()->Last();

// 		Initialise maker
//VP      gBenchmark->Start((const char *)maker->GetName());
      maker->StartTimer();
      if (GetDebug()) printf("\n*** Call %s::Init() ***\n\n",maker->ClassName());
      if ( maker->Init()) return kStErr;
      maker->StopTimer();
//VP      gBenchmark->Stop((const char *) maker->GetName());

// 		Add the Maker histograms in the Maker histograms list
// 		and remove it from the ROOT system directory
      gROOT->cd();
      TIter nextHist(gDirectory->GetList());
      int ready = !objLast;
      while((objHist=nextHist())) {// loop over gDirectory
        if (!ready && objHist!=objLast)		continue;
        ready = 1999;
        if (objHist==objLast)			continue;
        if (!objHist->InheritsFrom("TH1")) 	continue;

// 		Move the histogram from the ROOT list into the "maker's" list
        ((TH1*)objHist)->SetDirectory(0);
        maker->AddHist((TH1*)objHist);
      }
    ::doPs(maker->GetName(),"Init");
    }
  return kStOK; 
}
void StMaker::StartMaker()
{
  if (!m_DataSet) {//Keep legacy code
    m_DataSet = Find(".data");
    if (!m_DataSet) {m_DataSet = new St_ObjectSet(".data"); Add(m_DataSet);}
  }
  if (GetDebug()) printf("\n*** Call %s::Make() ***\n\n", ClassName());
//VP  gBenchmark->Start(GetName());
  StartTimer();}
//_____________________________________________________________________________
void StMaker::EndMaker(int ierr)
{
  if (ierr){};
  St_DataSet *dat = Find(".data");
  if (dat) dat->Pass(ClearDS,0);
  St_DataSet *gar = Find(".garb");
  if (gar) gar->Delete();
  ::doPs(GetName(),"EndMaker");
  
//VP  gBenchmark->Stop(GetName());
  StopTimer();
}

//_____________________________________________________________________________
Int_t StMaker::Finish()
{
//    Terminate a run
//   place to make operations on histograms, normalization,etc.
   int nerr = 0;

   TIter next(GetMakeList());
   StMaker *maker;
   Double_t totalCpuTime = 0;
   Double_t totalRealTime = 0;   
   while ((maker = (StMaker*)next())) 
   {
      if ( maker->Finish() ) nerr++;
      maker->PrintTimer();
      totalCpuTime  += maker->CpuTime();
      totalRealTime += maker->RealTime();
   
//VP      gBenchmark->Print((char *) maker->GetName());
   }

   // Print relative time
   if (totalCpuTime && totalRealTime) {
     Printf("------------------------------------------------------------------");
     Printf("%-10s: Real Time = %6.2f seconds Cpu Time = %6.2f seconds"
                               ,GetName(),totalRealTime,totalCpuTime);
     Printf("------------------------------------------------------------------");
     next.Reset();
     while ((maker = (StMaker*)next())) {
        Printf("%-10s: Real Time = %5.1f %%        Cpu Time = %5.1f %% "
               ,maker->GetName()
               ,100*maker->RealTime()/totalRealTime
               ,100*maker->CpuTime()/totalCpuTime);
     }
     Printf("------------------------------------------------------------------");
   }
   
   Clear();
   return nerr;
}

//_____________________________________________________________________________
Int_t StMaker::Make()
{
//   Loop on all makers
   Int_t ret;
   TList *tl = GetMakeList();
   if (!tl) return kStOK;
   
   TIter nextMaker(tl);
   StMaker *maker;
   while ((maker = (StMaker*)nextMaker())) {
  // Call Maker
     maker->StartMaker();
     ret = maker->Make();
     maker->EndMaker(ret);
     
     if (Debug()) printf("*** %s::Make() == %d ***\n",maker->ClassName(),ret);

     if (ret>kStWarn) { if (Debug()) maker->ls(3); return ret;}
   }
   return kStOK;
}
//_____________________________________________________________________________
void StMaker::Fatal(int Ierr, const char *com)
{
   printf("%s::Fatal: Error %d %s\n",GetName(),Ierr,com);
   StMaker *parent = (StMaker *)GetParent();
   if (parent) ((StMaker*)parent)->Fatal(Ierr,com);
   fflush(stdout);
}
//_____________________________________________________________________________
StMaker *StMaker::GetMaker(const St_DataSet *ds) 
{ 
  const St_DataSet *par = ds;
  while (par && (par = par->GetParent()) && strncmp(".maker",par->GetTitle(),6)) {}
  return (StMaker*)par;
}
//_____________________________________________________________________________
EDataSetPass StMaker::ClearDS (St_DataSet* ds,void * )
{
  static TClass *tabClass = 0;
  if (!tabClass) tabClass  = gROOT->GetClass("St_Table");

  if (ds->InheritsFrom(tabClass)) ds->Clear("Garbage");
  return kContinue; 
}
//_____________________________________________________________________________
void StMaker::PrintInfo() const
{
   const char *cvs = GetCVS();
   const char *built = strstr(cvs,"built");
   printf("\n%.*s\n",built-cvs,cvs);
   printf("%s\n\n",built);

//     Print info for all defined Makers
   TIter next(GetMakeList());
   StMaker *maker;
   while ((maker = (StMaker*)next())) {
      maker->PrintInfo();
   }
}
//_____________________________________________________________________________
Int_t        StMaker::GetEventNumber() const 
{
   StEvtHddr *hd = (StEvtHddr*)GetDataSet("EvtHddr");
   if (!hd) return -1;
   return hd->GetEventNumber();
}
//_____________________________________________________________________________
Int_t        StMaker::GetRunNumber() const 
{
   StEvtHddr *hd = (StEvtHddr*)GetDataSet("EvtHddr");
   if (!hd) return -1;
   return hd->GetRunNumber();
}
//_____________________________________________________________________________
TDatime  StMaker::GetDateTime() const 
{
    
   TDatime td; return td;   
   StEvtHddr *hd = (StEvtHddr*)GetDataSet("EvtHddr");
   if (!hd) return td;
   return hd->GetDateTime();
}
//_____________________________________________________________________________
Int_t    StMaker::GetDate()  const {return GetDateTime().GetDate();}
//_____________________________________________________________________________
Int_t    StMaker::GetTime()  const {return GetDateTime().GetTime();}
//_____________________________________________________________________________
const Char_t *StMaker::GetEventType() const
{
   StEvtHddr *hd = (StEvtHddr*)GetDataSet("EvtHddr");
   if (!hd) return 0;
   return hd->GetEventType();
}

//_____________________________________________________________________________
void StMaker::PrintTimer(Option_t *option) 
{
   if(option){};
   Printf("%-10s: Real Time = %6.2f seconds Cpu Time = %6.2f seconds",GetName()
                                         ,m_Timer.RealTime(),m_Timer.CpuTime());
}

//_____________________________________________________________________________
void StMaker::MakeDoc(const TString &stardir,const TString &outdir, Bool_t baseClasses)
{
 //
 // MakeDoc - creates the HTML doc for this class and for the base classes:
 //           (if baseClasses == kTRUE)
 //         *  St_XDFFile  St_Module      St_Table       *
 //         *  St_DataSet  St_DataSetIter St_FileSet     *
 //         *  StMaker     StChain                       *
 //
 // stardir - the "root" directory to lookup the subdirectories as follows.
 //           = "$(STAR)"             by default
 // outdir  - directory to write the generated HTML and Postscript files into
 //           = "$(STAR)/StRoot/html" by default
 //
 //            The following subdirectories are used to look it up:
 //            $(stardir) + "StRoot/St_base"
 //            $(stardir) + "StRoot/StChain"
 //            $(stardir) + "StRoot/xdf2root"
 //            $(stardir) + ".share/tables"
 //            $(stardir) + "StRoot/StUtilities"
 //            $(stardir) + "inc",
 //            $(stardir) + "StRoot/<this class name>",
 //
 //   where $(stardir) is the input parameter (by default = "$(afs)/rhic/star/packages/dev/")
 //
 // baseClasses - flag to mark whether the base classes HTML docs will be created as well
 //               = kTRUE by default

 // Define the type of the OS
  TString STAR= stardir;
  TString delim = ":";
  Bool_t NT=kFALSE;

  if (strcmp(gSystem->GetName(),"WinNT") == 0 ) {
     NT=kTRUE;
     delim = ";";
     STAR.ReplaceAll("$(afs)","//sol/afs");
  }
  else 
     STAR.ReplaceAll("$(afs)","/afs");

  TString classname = ClassName();

  if (!gHtml) gHtml = new THtml;

  // Define the set of the subdirectories with the STAR class sources
  //                       | ----------------------  | ------------  | ------------------ |
  //                       | Directory name             Class name     Share library name |
  //                       | ----------------------  | ------------  | ------------------ |
  const Char_t *source[] = {"StRoot/St_base"         , "St_DataSet"  ,    "St_base"
                           ,"StRoot/StChain"         , "StMaker"     ,    "StChain"
                           ,"StRoot/xdf2root"        , "St_XDFFile"  ,    "xdf2root"
//                           ,"StRoot/StUtilities"     , "StMessage"   ,    "StUtilities"
                           ,"StRoot/StarClassLibrary", ""            ,    ""
                           ,"StRoot/StEvent"         , "StEvent"     ,    "StEvent"
                           ,"StRoot/St_TLA_Maker"    , "St_TLA_Maker",    "St_TLA_Maker"
                           ,".share/tables"          , ""            ,     ""
                           ,"inc"                    , ""            ,     ""
                           };

  const Int_t lsource = sizeof(source)/sizeof(const Char_t *);
 
  TString lookup = STAR;
  lookup += "/StRoot/";
  lookup += classname;

  // Add class name base
  
  Int_t i = 0;
  for (i=0;i<lsource-3;i+=3) {
    lookup += delim;
    lookup += STAR;
    lookup += "/";
    lookup += source[i];
    // Load extra share library if any
    const Char_t *cl = source[i+1];
    const Char_t *so = source[i+2];
    if (cl && cl[0] && so && so[0] && !gROOT->GetClass(cl)) 
    {
       if (gSystem->Load(so)) 
           printf(" Failed to load the share library %s for class %s\n",so,cl);
    }
  }
  
  const Char_t *c = ClassName();  // This trick has to be done since a bug within ROOT

  lookup += delim;
  lookup += STAR;
  lookup += "/StRoot/";
  lookup += c;

  gSystem->ExpandPathName(lookup);
  lookup.ReplaceAll("//StRoot/","/StRoot/");
  gHtml->SetSourceDir(lookup);

  TString odir = outdir;
//  odir.ReplaceAll("$(STAR)",STAR);
  gSystem->ExpandPathName(odir);
  gHtml->SetOutputDir(odir);

  // Create the list of the classes defined with the loaded DLL's to be documented

  Char_t *classes[] = { "St_XDFFile",  "St_Module",      "St_Table"
                       ,"St_DataSet",  "St_DataSetIter", "St_FileSet"
                       ,"StMaker",     "StChain"
                       ,"table_head_st"
                      };
  Int_t nclass = sizeof(classes)/4;
  // Create the definitions of the classes not derived from TObjects
  TString header = STAR;
  header += "/inc/table_header.h";

  gSystem->ExpandPathName(header);
  header.ReplaceAll("//inc/","/inc/");
  gROOT->LoadMacro(header);

  TClass header1("table_head_st",1,"table_header.h","table_header.h");

  // Update the docs of the base classes
  static Bool_t makeAllAtOnce = kTRUE;
  if (makeAllAtOnce && baseClasses) { 
     makeAllAtOnce = kFALSE;
     gHtml->MakeAll(); 
     for (i=0;i<nclass;i++) gHtml->MakeClass(classes[i]);
  }

  // Create the doc for this class
  printf(" Making html for <%s>\n",c);
  gHtml->MakeClass((Char_t *)c);
//   Loop on all makers
   TList *tl = GetMakeList();
   if (tl) {
     TIter nextMaker(tl);
     StMaker *maker;
     while ((maker = (StMaker*)nextMaker())) 
         maker->MakeDoc(stardir,outdir,kFALSE);
   }
}

//_____________________________________________________________________________
static void doPs(const char *who, const char *where)
{
  static const char *ps =0;
  if (!ps) {
//		execute shell      
    ps = gSystem->Getenv("StarEndMakerShell"); 
    if (ps) {
      TString *ts = new TString(ps);
      char buf[12]; sprintf(buf,"%d",gSystem->GetPid());
      ts->ReplaceAll("$$",buf);
      ps = ts->Data();
    } else { ps ="";}
  }
  if (ps[0]) { //Execute shell
    printf("\ndoPs for <%s.%s> shell=%s\n",who,where,ps);
    fflush(stdout);
    if (gSystem->Exec(ps)) ps="";
  }
}
//_____________________________________________________________________________
void StMaker::Streamer(TBuffer &b)
{ Error("Streamer"," attempt to write %s\n ",GetName());
  assert(0);
}
