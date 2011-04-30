#ifndef GRAPHICS_UTILITY_H
#define GRAPHICS_UTILITY_H

#include "video/nebu_font.h"
#include "video/nebu_video_types.h"

void rasonly(Visual *d);
void doPerspective(float fov, float ratio, float znear, float zfar);
void doLookAt(float *cam, float *target, float *up);
void drawText(nebu_Font* ftx, float x, float y, float size, const char *text);

#endif
