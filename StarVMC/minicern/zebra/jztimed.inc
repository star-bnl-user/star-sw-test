*
* $Id: jztimed.inc,v 1.1.1.1 2004/07/17 20:01:57 perev Exp $
*
* $Log: jztimed.inc,v $
* Revision 1.1.1.1  2004/07/17 20:01:57  perev
* STAR version of Geant321 TGeant3 etc
*
* Revision 1.1.1.1  2002/07/24 15:56:28  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:49  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:27  fca
* AliRoot sources
*
* Revision 1.1.1.1  1996/03/06 10:46:55  mclareni
* Zebra
*
*
#ifndef CERNLIB_ZEBRA_JZTIMED_INC
#define CERNLIB_ZEBRA_JZTIMED_INC
*
*
* jztimed.inc
*
#if defined(CERNLIB_JZTIME)
      IF (JQTIME.NE.0)  CALL TIMED (DELTAT)
#endif

#endif
