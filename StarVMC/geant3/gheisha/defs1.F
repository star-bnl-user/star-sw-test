*
* $Id: defs1.F,v 1.1.1.2 2009/02/01 17:10:27 fisyak Exp $
*
* $Log: defs1.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:27  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:48:05  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:37  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:27  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:39  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:18  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:20:58  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.38  by  S.Giani
*-- Author :
      SUBROUTINE DEFS1(I,J,K)
C
C *** NVE 16-MAR-1988 CERN GENEVA ***
C
C ORIGIN : H.FESEFELDT (15-JAN-1984)
C
#include "geant321/mxgkgh.inc"
#include "geant321/s_blank.inc"
C
      DATA PI/3.141592653589793/
      PX=PV(1,I)
      PY=PV(2,I)
      PZ=PV(3,I)
      PT2 = PV(1,J)**2+PV(2,J)**2
      IF(PT2.GT.0.) THEN
      CALL LENGTX(J,P)
      COST=PV(3,J)/P
      SINT1=SQRT(ABS((1.-COST)*(1.+COST)))
      SINT2=SQRT(PT2)/P
      SINT=0.5*(SINT1+SINT2)
      PH=PI*0.5
      IF(PV(2,J).LT.0.) PH=PI*1.5
      IF(ABS(PV(1,J)).GT.1.E-6) PH=ATAN2(PV(2,J),PV(1,J))
      COSP=COS(PH)
      SINP=SIN(PH)
      PV(1,K)= COST*COSP*PX-     SINP*PY+SINT*COSP*PZ
      PV(2,K)= COST*SINP*PX+     COSP*PY+SINT*SINP*PZ
      PV(3,K)=-SINT     *PX             +COST     *PZ
      ELSE
      PV(1,K)=PX
      PV(2,K)=PY
      PV(3,K)=PZ
C --- Take the case of theta=pi into account (MR/NVE 27-sep-1990) ---
      IF (PV(3,J) .LT. 0.) PV(3,K)=-PZ
      ENDIF
      END
