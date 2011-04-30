#ifndef GLTRON_H
#define GLTRON_H

/*
  gltron
  Copyright (C) 1999 by Andreas Umbach <marvin@dataway.ch>
*/

/* Win32 port maintained by Andreas Umbach <marvin@dataway.ch> */
#ifdef WIN32
#include <windows.h>
#endif /* WIN32 */

/* MacOS port maintained by Darrell Walisser <walisser@mac.com> */

#ifdef macintosh  /* os9/classic */
#include <string.h>
#undef RC_NAME
#define RC_NAME "gltronPrefs.txt"
#endif 

#ifdef __APPLE__ /* osx */
#undef RC_NAME
#define RC_NAME "gltronPrefs.txt"
#endif

#endif /* GLTRON_H */
