//:>------------------------------------------------------------------
//: FILE:       FtfFinder.h
//: HISTORY:
//:             28oct1996 version 1.00
//:             23aug1999 ppy printVols and printRows deleted
//:             26jan2000 ppy destructor added
//:             27jan2000 ppy VOLUME, ROW and AREA classes replaced by
//:                           FtfContainer
//:<------------------------------------------------------------------

#ifndef FTFFINDER
#define FTFFINDER
#include <memory.h>

#include "FtfGeneral.h"
#include "FtfPara.h"
#include "FtfHit.h"
#include "FtfTrack.h"
#include "FtfMcTrack.h"

#ifdef SL3ROOT
#include "Rtypes.h"
#else
#define ClassDef(a,b)
#endif


class FtfFinder {

public:
   FtfFinder( ) ;
   ~FtfFinder( ) ;
   friend class FtfTrack ;
	
   void    dEdx                    ( ) ;
   int     getTracks               ( ) ;
   void    mergePrimaryTracks      ( ) ;
   double  process ( ) ;
   int     reset                   ( ) ;
   int     setConformalCoordinates ( ) ;
   int     setPointers             ( ) ;
   double  CpuTime                 ( ) ;
   double  RealTime                ( ) ;
//
//
   int           nHits      ;  
   int           nHitsOutOfRange ;
   int           maxHits    ;  
   FtfHit        *hit       ;  
   int           nTracks    ; 
   FtfTrack      *track     ;  
   FtfPara       para       ;
   int           maxTracks  ;
   int           nMcTracks  ;
   FtfMcTrack    *mcTrack    ;
   FtfContainer  *volumeC ;
   FtfContainer  *rowC    ;
   FtfContainer  *trackC  ;
   double        cpuTime ;
   double        realTime ;
private: 

   FtfTrack      *currentTrack ;

   ClassDef(FtfFinder,1)

    
} ;
#endif

