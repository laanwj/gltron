#ifndef NEBU_2D_H
#define NEBU_2D_H

#include "base/nebu_surface.h"

typedef struct {
	int w, h;
	int tex_w, tex_h;
	unsigned int tex_id;
} nebu_2d;

nebu_2d* nebu_2d_Create(nebu_Surface* pSurface, int flags);
nebu_2d* nebu_2d_LoadPNG(const char *path, int flags);
void nebu_2d_Free(nebu_2d *p2d);

void nebu_2d_Draw(const nebu_2d *p2d);

#endif
