/***************************************************************************
 *
 * $Id: StFTPCReader.h,v 1.1 2000/01/24 14:39:33 perev Exp $
 *
 * Author: Holm Huemmler
 ***************************************************************************
 *
 * Description: Offline Wrapper for DAQ FTPC reader classes
 *
 ***************************************************************************
 *
 * $Log: StFTPCReader.h,v $
 * Revision 1.1  2000/01/24 14:39:33  perev
 * FTPC (HolmMade) is added
 *
 *
 *
 **************************************************************************/
#ifndef _StFTPCReader_
#define _StFTPCReader_

#ifndef __CINT__
#include "StDaqLib/GENERIC/EventReader.hh"
#endif /*__CINT__*/
#include "StDAQReader.h"

//		Forward declarations
struct  EventInfo;
typedef  EventInfo DAQEventInfo;
class  EventReader;
class  DetectorReader;
class  ZeroSuppressedReader;
class  ADCRawReader;
class  PedestalReader;
class  PedestalRMSReader;
class  GainReader;
class  CPPReader;
class  BadChannelReader;
class  RICH_Reader;
typedef RICH_Reader StRICHReader;
//


struct TPCSequence;

struct TPCCluster;

struct TPCGain;

class StTPCReader;
class StDAQReader;


#include "St_fss_Maker/StFssSectorReader.hh"

class  StFTPCReader 
{
  public:
  friend class StDAQReader;

  StFTPCReader(StDAQReader *rd);
  StFTPCReader(unsigned short *fcl_ftpcsqndx, int nSeq,
	       char *fcl_ftpcadc, int nAdc);
  virtual ~StFTPCReader();

  int getMaxPad(int PadRow) const {return 160;};	//Number of pads in padrow

  virtual int getPadList(int Sector, int PadRow, unsigned char *&padList);
      // Fills (*padList[]) with the list of pad numbers containing hits
      // returns number of pads in (*padList)[]
      // or negative if call fails
  
  virtual int getSequences(int Sector, int PadRow, int Pad, int *nSeq,
			   TPCSequence *&SeqData);
      	//  Fills (*SeqData)[] along with the ADC
      	// buffers pointed to by (*SeqData)[]
      	// Set nSeq to the # of elements in the (*SeqData)[] array
      	// returns 0 if OK.
      	// or negative if call fails

  virtual int getRawADC(int Sector,int PadRow, int Pad, int &nArray,
                        unsigned char *&Array);
	// Fills (*Array)[] with Raw data
	// Fills nArray with the # of elements in (*Array)[] (512 bytes / TPC)
	// returns 0 if OK.
	// returns negative if call fails


  virtual int getPedestals(int Sector,int PadRow, int Pad, int &nArray,
                           unsigned char *&Array);

	// Fills (*Array)[] with Pedestal data
	// Fills nArray with the # of elements in Array (512 bytes for TPC)
	// returns 0 if OK.
	// returns negative if call fails


  virtual int getRMSPedestals(int Sector,int PadRow, int Pad, int &nArray,
                              unsigned char *&Array);

	// Fills (*Array)[] with Pedestal RMS data * 16
	// Fills nArray with the # of elements in (*Array)[] (512 bytes / TPC)
	// returns 0 if OK.
	// returns negative if call fails


  virtual int getGain(int Sector, int PadRow, int Pad, TPCGain *&gain);
	// sets (*gain) to a valid gain structure pointer
	// returns 0 if OK
	// returns negative if call fails


//  virtual int getMeanGain();
      // returns mean gain

//  virtual int getGainEvents();
	// returns the number of events the calculation is based upon

  virtual int getClusters(int Sector, int PadRow, int Pad, int &nClusters, 
			  TPCCluster *&clusters);
	// sets (*clusters) to beginning of array of clusters
	// sets nClusters to the length of the array
	// returns 0 if OK
	// returns negative if call fails

  virtual int IsBad(int Sector, int PadRow, int Pad);
	// returns true if the pad is bad.  
	// returns false if the pad is not bad.

protected:
  virtual void Update();
  virtual void setSector(int sector);

  StDAQReader 		*fDAQReader;
  DetectorReader 	*fFTPCImpReader;
  ZeroSuppressedReader 	*fZeroSuppressedReader;
  ADCRawReader 		*fADCRawReader;
  PedestalReader 	*fPedestalReader;
  PedestalRMSReader 	*fPedestalRMSReader;
  GainReader 		*fGainReader;
  CPPReader 		*fCPPReader;
  BadChannelReader 	*fBadChannelReader;

  int fSector;

  int simu;
  unsigned short *m_ftpcsqndx;
  int m_numSqndx;
  char *m_ftpcadc;
  int m_numAdc;
  StFssSectorReader *mSecReader;
};
#endif
