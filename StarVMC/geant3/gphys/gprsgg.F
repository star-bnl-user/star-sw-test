*
* $Id: gprsgg.F,v 1.1.1.2 2009/02/01 17:10:32 fisyak Exp $
*
* $Log: gprsgg.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:32  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:48:45  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:55  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.2  2003/11/28 11:23:56  brun
* New version of geant321 with all geant3 routines renamed from G to G3
*
* Revision 1.1.1.1  2002/07/24 15:56:25  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:41  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:20  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:21:33  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/04 24/02/95  14.49.53  by  S.Giani
*-- Author :
      FUNCTION G3PRSGG(Z,E)
C.
C.    ******************************************************************
C.    *                                                                *
C.    *  To calculate cross-section for e+e- production                *
C.    *  by photons (in barn/atom)                                     *
C.    *                                                                *
C.    *    ==>Called by : G3PRSGA                                      *
C.    *       Author    L.Urban  *********                             *
C.    *       Modified by: G.Battistoni                                *
C.    *                                                                *
C.    *   constant cross section is assumed above 100 GeV !            *
C.    ******************************************************************
C.
#include "geant321/gconsp.inc"
      DIMENSION C(18),CC(3)
      DATA C/ 0.87842E-3,-0.19625E-2, 0.12949E-2,-0.20028E-3
     +      , 0.12575E-4,-0.28333E-6,-0.10342E-4, 0.17692E-4
     +      ,-0.82391E-5, 0.13063E-5,-0.90815E-7, 0.23586E-8
     +      ,-0.45263E-3, 0.11161E-2,-0.86749E-3, 0.21773E-3
     +      ,-0.20467E-4, 0.65372E-6/
      DATA EUPP/100./
C.
C.    ------------------------------------------------------------------
C.
      SIG=0.
*
      ESAV=E
      IF(E.GT.EUPP) E=EUPP
*
      IF(E.LE.2.*EMASS)GO TO 90
      X=LOG(E/EMASS)
      DO 20 I=1,3
         CC(I)=0.
         D=1.
         DO 10 J=1,6
            JJ=6*I+J-6
            CC(I)=CC(I)+D*C(JJ)
            D=D*X
  10     CONTINUE
  20  CONTINUE
C
      SIG=Z*(Z+1.)*(CC(1)+CC(2)*Z+CC(3)/Z)
C
  90  IF(SIG.LT.0.)SIG=0.
      G3PRSGG=SIG
*
      E=ESAV
*
      END
