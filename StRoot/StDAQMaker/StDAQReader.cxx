/***************************************************************************
 *
 * $Id: StDAQReader.cxx,v 1.4 1999/08/01 00:14:49 perev Exp $
 *
 * Author: Victor Perev
 ***************************************************************************
 *
 * Description: Offline Wrapper for DAQ reader classes
 *
 ***************************************************************************
 *
 * $Log: StDAQReader.cxx,v $
 * Revision 1.4  1999/08/01 00:14:49  perev
 * leak removed author added
 *
 * Revision 1.3  1999/08/01 00:09:07  perev
 * leak removed author added
 *
 **************************************************************************/#include "StDAQMaker/StDAQReader.h"
#include "StDaqLib/GENERIC/EventReader.hh"

//	non standard open,close,read
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Stypes.h"
//


typedef EventInfo DAQEventInfo;
//_____________________________________________________________________________
StDAQReader::StDAQReader(const char *file)
{
  fFd = -1;
  fEventReader = 0;
  fTPCReader = 0;
  fOffset = 0;
  fFile = 0;
  if (!file || !file[0]) return;
  fEventInfo = new DAQEventInfo;
  memset(fEventInfo,0,sizeof(DAQEventInfo));
  assert(sizeof(DAQEventInfo)==sizeof(EventInfo));

  assert(!open(file));
  setTPCVersion();
}

//_____________________________________________________________________________
int StDAQReader::open(const char *file)
{
  delete [] fFile;
  fFile = new char[strlen(file)+1];
  strcpy(fFile,file);

  close();
  fFd = ::open(file,O_RDONLY);
   if (fFd==-1) { 
     printf("<StDAQReader::open>  %s %s ",file, strerror( errno ) );
     return 1;
  }
   
  return 0;  
}
//_____________________________________________________________________________
int StDAQReader::close()
{
  delete fEventReader; fEventReader = 0;
  if (fFd != (-1)) ::close(fFd);
  fFd = -1;
  return 0;
}
//_____________________________________________________________________________
StDAQReader::~StDAQReader()
{
  if (fFd != (-1)) ::close(fFd);
  delete fEventReader;
}
//_____________________________________________________________________________
int StDAQReader::readEvent()
{  
  delete fEventReader;
  if (fOffset == -1) return 1;
  fEventReader = new EventReader();
  fEventReader->InitEventReader(fFd, fOffset, 0);
  fOffset = fEventReader->NextEventOffset();
  *fEventInfo = fEventReader->getEventInfo();

  if (fTPCReader) fTPCReader->Update();

  return 0;
}
//_____________________________________________________________________________
int StDAQReader::skipEvent(int nskip)
{
  for (int isk=0; isk<nskip; isk++) 
  {
    delete fEventReader;
    if (fOffset == -1) return kStEOF;
    fEventReader = new EventReader();
    fEventReader->InitEventReader(fFd, fOffset, 0);
  }
  return 0;
}


//_____________________________________________________________________________
void StDAQReader::setTPCVersion(const char* vers)
{strcpy(fTPCVersion,vers);} 

//_____________________________________________________________________________
  int StDAQReader::getRunNumber()   const { return fEventReader->runno();}
//_____________________________________________________________________________
  int StDAQReader::getEventNumber() const {return fEventInfo->EventSeqNo;}
//_____________________________________________________________________________
  unsigned int StDAQReader::getUnixTime() const {return fEventInfo->UnixTime;}
//_____________________________________________________________________________
  unsigned int StDAQReader::getTrigWord() const {return fEventInfo->TrigWord;}
//_____________________________________________________________________________
  unsigned int StDAQReader::getTrigInputWord() const {return fEventInfo->TrigInputWord;}
//_____________________________________________________________________________
   int StDAQReader::TPCPresent()  const {return  fEventInfo->TPCPresent;}
//_____________________________________________________________________________
   int StDAQReader::SVTPresent()  const {return  fEventInfo->SVTPresent;}
//_____________________________________________________________________________
   int StDAQReader::TOFPresent()  const {return  fEventInfo->TOFPresent;}
//_____________________________________________________________________________
   int StDAQReader::EMCPresent()  const {return  fEventInfo->EMCPresent;}
//_____________________________________________________________________________
   int StDAQReader::SMDPresent()  const {return  fEventInfo->SMDPresent;}
//_____________________________________________________________________________
   int StDAQReader::FTPCPresent() const {return  fEventInfo->FTPCPresent;}
//_____________________________________________________________________________
   int StDAQReader::RICHPresent() const {return  fEventInfo->RICHPresent;}
//_____________________________________________________________________________
   int StDAQReader::TRGDetectorsPresent() const{return  fEventInfo->TRGDetectorsPresent;}
//_____________________________________________________________________________
   int StDAQReader::L3Present()   const {return  fEventInfo->L3Present;}
//_____________________________________________________________________________
StTPCReader *StDAQReader::getTPCReader() 
{
  if (!fTPCReader) {
    fTPCReader = new StTPCReader(this);
  }
  return fTPCReader;
}
//_____________________________________________________________________________
void StDAQReader::printEventInfo()
{fEventReader->printEventInfo();}
//_____________________________________________________________________________
StTPCReader::StTPCReader(StDAQReader *daqr)
{
  fDAQReader = daqr;
  fSector = -1999;
  fTPCImpReader 	= 0;
  fZeroSuppressedReader = 0;
  fADCRawReader 	= 0;
  fPedestalReader 	= 0;
  fPedestalRMSReader 	= 0;
  fGainReader 		= 0;
  fCPPReader 		= 0;
  fBadChannelReader 	= 0;
  Update();
}
//_____________________________________________________________________________
void StTPCReader::Update()
{
  setSector(-1);
}
//_____________________________________________________________________________
StTPCReader::~StTPCReader()
{
  delete fTPCImpReader;
}
//_____________________________________________________________________________
void StTPCReader::setSector(int sector)
{
  if (sector == fSector) return;
  if (sector == -1) {
   delete fTPCImpReader;
   fTPCImpReader = ::getDetectorReader(fDAQReader->fEventReader,fDAQReader->fTPCVersion);
   fSector = -1999;
  }
  delete fZeroSuppressedReader;
  delete fADCRawReader ;
  delete fPedestalReader ;
  delete fPedestalRMSReader;
  delete fGainReader;
  delete fCPPReader;
  delete fBadChannelReader;

  fZeroSuppressedReader = 0;
  fADCRawReader 	= 0;
  fPedestalReader 	= 0;
  fPedestalRMSReader 	= 0;
  fGainReader 		= 0;
  fCPPReader 		= 0;
  fBadChannelReader 	= 0;
  if (sector == -1) return;

  fSector = sector;

  fZeroSuppressedReader = fTPCImpReader->getZeroSuppressedReader(fSector);
  fADCRawReader 	= fTPCImpReader->getADCRawReader(fSector);
  fPedestalReader 	= fTPCImpReader->getPedestalReader(fSector);
  fPedestalRMSReader 	= fTPCImpReader->getPedestalRMSReader(fSector);
  fGainReader 		= fTPCImpReader->getGainReader(fSector);
  fCPPReader 		= fTPCImpReader->getCPPReader(fSector);
  fBadChannelReader 	= fTPCImpReader->getBadChannelReader(fSector);
}
//_____________________________________________________________________________

int StTPCReader::getMaxPad(int padrow) const
{
  const unsigned char PADS[45] = {
   88, 96,104,112,118,126,134,142,150,
  158,166,174,182, 98,100,102,104,106,
  106,108,110,112,112,114,116,118,120,
  122,122,124,126,128,128,130,132,134,
  136,138,138,140,142,144,144,144,144};
  
  assert(padrow>0 && padrow <=45);   
  return PADS[padrow-1];
}
  
//_____________________________________________________________________________
int StTPCReader::getPadList(int Sector, int PadRow, unsigned char *&padList)
{
  setSector(Sector);  
  if (!fZeroSuppressedReader) return -1;
  return fZeroSuppressedReader->getPadList(PadRow, &padList);  
} 
//_____________________________________________________________________________
  int StTPCReader::getSequences(int Sector, int PadRow, int Pad, int &nSeq,
			   TPCSequence *&SeqData) 
{
  setSector(Sector);
  nSeq = 0; SeqData = 0;
  if (!fZeroSuppressedReader) return -1;
  Sequence *seq;
  int iret = fZeroSuppressedReader->getSequences(PadRow,Pad,&nSeq,&seq);
  assert (sizeof(TPCSequence)==sizeof(Sequence));
  SeqData = (TPCSequence*)seq;
  return iret;
}

//_____________________________________________________________________________
int StTPCReader::getRawADC(int Sector,int PadRow, int Pad, int &nArray,
                        unsigned char *&Array)
{
  setSector(Sector);
  nArray = 0; Array=0;
  if (!fADCRawReader) return -1;
  return fADCRawReader->getSequences(PadRow,Pad,&nArray,&Array);
}  
//_____________________________________________________________________________
int StTPCReader::getPedestals(int Sector,int PadRow, int Pad, int &nArray,
                           unsigned char *&Array)
{  
  setSector(Sector);
  nArray = 0; Array=0;
  if (!fPedestalReader) return -1;
  return fPedestalReader->getSequences(PadRow,Pad,&nArray,&Array);
}  
//_____________________________________________________________________________
int StTPCReader::getRMSPedestals(int Sector,int PadRow, int Pad, int &nArray,
                           unsigned char *&Array)
{  
  setSector(Sector);
  nArray = 0; Array=0;
  if (!fPedestalRMSReader) return -1;
  return fPedestalRMSReader->getSequences(PadRow,Pad,&nArray,&Array);
}  
//_____________________________________________________________________________
int StTPCReader::getGain(int Sector, int PadRow, int Pad, TPCGain *&gain)
{  
  setSector(Sector);
  gain = 0;
  if (!fGainReader) return -1; 
  struct Gain *gainqq;
  int iret = fGainReader->getGain(PadRow,Pad,&gainqq);
  assert(sizeof(TPCGain)==sizeof(struct Gain));
  gain = (TPCGain*)gainqq;
  return iret;
}
//_____________________________________________________________________________
int StTPCReader::getClusters(int Sector, int PadRow, int Pad, int &nClusters, 
			     TPCCluster *&clusters)
{  
  setSector(Sector);
  nClusters=0; clusters=0;
  if (!fCPPReader) return -1;
  struct ASIC_Cluster *clustersqq;
  int iret = fCPPReader->getClusters(PadRow,Pad,&nClusters,&clustersqq);
  assert(sizeof(TPCCluster)==sizeof(struct ASIC_Cluster));
  clusters = (TPCCluster *)clustersqq;
  return iret;
}  
//_____________________________________________________________________________
int StTPCReader::IsBad(int Sector, int PadRow, int Pad)
{
  setSector(Sector);
  if (!fBadChannelReader) return 1;
  return fBadChannelReader->IsBad(PadRow,Pad);
}
  
  
