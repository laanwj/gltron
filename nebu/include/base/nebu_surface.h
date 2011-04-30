#ifndef NEBU_SURFACE_H
#define NEBU_SURFACE_H

#include "base/nebu_png_texture.h"
#include "base/nebu_types.h"

typedef struct {
	int w, h;
	int format;
	unsigned char* data;
} nebu_Surface;

enum { 
	NEBU_SURFACE_FMT_UNKNOWN,
	NEBU_SURFACE_FMT_RGB,
	NEBU_SURFACE_FMT_RGBA,
	NEBU_SURFACE_FMT_ALPHA,
	NEBU_SURFACE_FMT_MAX,
};

extern int nebu_Surface_FormatSize[NEBU_SURFACE_FMT_MAX];

nebu_Surface* nebu_Surface_Create(int width, int height, int format);
nebu_Surface* nebu_Surface_LoadPNG(const char *filename);
int nebu_Surface_SavePNG(nebu_Surface* pSurface, const char *filename);
int nebu_Surface_SaveBMP(nebu_Surface* pSurface, const char *filename);
void nebu_Surface_Blit(nebu_Surface *pSrcSurface, nebu_Rect *pSrcRect, nebu_Surface* pDstSurface, nebu_Rect *pDstRect);
void nebu_Surface_Free(nebu_Surface* pSurface);
#endif
