#ifndef FONTTEX_H
#define FONTTEX_H

#include "video/nebu_font.h"

#if 1
typedef nebu_Font FontTex;
#else
typedef struct {
  int nTextures;
  
  int texwidth; /* texture width */
  int width; /* character width */

  int lower; /* lowest ascii character (normally: 32) */
  int upper; /* highest ascii character (normally: 126) */

  int *texID;
} FontTex;
#endif

#endif
