#ifndef NEBU_VIDEO_UTILITY_H
#define NEBU_VIDEO_UTILITY_H

#include "base/nebu_surface.h"

void nebu_Video_rasonly(int x, int y, int width, int height);
nebu_Surface* nebu_Video_GrabScreen(int width, int height);

#endif
