#ifndef ASU_MALLOC_OFF

#include <stdio.h>
#include <stdlib.h>
//#include <new.h>
#include "asuAlloc.h"
#ifdef new
#undef new 
#endif

void* operator new   (size_t sz) { return asuMalloc(sz,"NEW:",-1);}
void* operator new   (size_t sz,const char *file,int line) {return asuMalloc(sz,file,-line);}
void  operator delete(void *ptr) { asuFree (ptr,"DEL:",-1);}

#if  defined(__GNUC__) || defined(__ACC)
void* operator new []  (size_t sz)  { return operator new (sz);}
void* operator new []  (size_t sz,const char *file,int line) {return operator new (sz,file,line);}
void  operator delete [] (void *ptr) {operator delete (ptr) ;}
#endif /*__GNUC__*/


#endif /* ASU_MALLOC_OFF */

