*
* $Id: eexlvl.F,v 1.1.1.2 2009/02/01 17:10:17 fisyak Exp $
*
* $Log: eexlvl.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:17  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:47  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:23  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:24  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:36  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:15  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:19:55  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.42  by  S.Giani
*-- Author :
*$ CREATE EEXLVL.FOR
*COPY EEXLVL
*
*=== eexlvl ===========================================================*
*
      SUBROUTINE EEXLVL ( JA, JZ, EEX1ST, EEX2ND, EEXCON )
 
#include "geant321/dblprc.inc"
#include "geant321/dimpar.inc"
#include "geant321/iounit.inc"
*
*----------------------------------------------------------------------*
*                                                                      *
*     Created on 06 december 1991  by    Alfredo Ferrari & Paola Sala  *
*                                                   Infn - Milan       *
*                                                                      *
*     Last change on 28-apr-92     by    Alfredo Ferrari               *
*                                                                      *
*                                                                      *
*----------------------------------------------------------------------*
*
#include "geant321/eva0.inc"
*
      SAVE SQATAR, JAOLD
      DATA JAOLD / 0 /
*
      IF ( JA .EQ. JZ ) THEN
         EEXCON = 0.D+00
         EEX1ST = 0.D+00
         EEX2ND = 0.D+00
         RETURN
      END IF
      IF ( JZ .EQ. 0 .OR. JA .EQ. JZ ) THEN
         EEXCON = 0.D+00
      ELSE
         EEXCON = 1.D-03 * ( CAM4 (JZ) + CAM5 (JA-JZ) )
      END IF
*  **** Very tentative selection of Eex1st, Eex2nd, based on pairing
*       energies according to delta = 12 MeV / A^1/2 ****
      IZODD  = 1 - MOD ( JZ, 2 )
      INODD  = 1 - MOD ( JA-JZ, 2 )
      IODD   = IZODD + INODD
*  +-------------------------------------------------------------------*
*  |  Even-even nucleus
      IF ( IODD .GE. 2 ) THEN
*  |  +----------------------------------------------------------------*
*  |  |
         IF ( JA .NE. JAOLD ) THEN
            JAOLD  = JA
            DJA = JA
            SQATAR = SQRT ( DJA )
         END IF
*  |  |
*  |  +----------------------------------------------------------------*
         EEX1ST = 12.0D-03 / SQATAR
         EEX2ND = 2.D+00 * EEX1ST
*  |
*  +-------------------------------------------------------------------*
*  |  even-odd nucleus
      ELSE IF ( IODD .GT. 0 ) THEN
*  |  +----------------------------------------------------------------*
*  |  |
         IF ( JA .NE. JAOLD ) THEN
            JAOLD  = JA
            DJA    = JA
            SQATAR = SQRT ( DJA )
         END IF
*  |  |
*  |  +----------------------------------------------------------------*
         EEX1ST = 12.0D-03 / SQATAR
         EEX2ND = EEX1ST
*  |
*  +-------------------------------------------------------------------*
*  |  odd-odd nucleus
      ELSE
         EEX2ND = 0.D+00
         EEX1ST = 0.D+00
      END IF
*  |
*  +-------------------------------------------------------------------*
      RETURN
*=== End of subroutine eexlvl =========================================*
      END
