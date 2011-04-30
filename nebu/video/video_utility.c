#include "video/nebu_renderer_gl.h"

#include "base/nebu_surface.h"
#include "video/nebu_video_system.h"
#include "video/nebu_video_utility.h"

void nebu_Video_rasonly(int x, int y, int width, int height)
{
	/* do rasterising only (in local display d) */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) width, 0.0f, (GLfloat) height, 0.0f, 1.0f);
	nebu_Video_CheckErrors("rasonly");
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(x, y, width, height);
}

nebu_Surface* nebu_Video_GrabScreen(int width, int height) { 
  nebu_Surface *pSurface = nebu_Surface_Create(width, height, NEBU_SURFACE_FMT_RGB);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pSurface->data);
  return pSurface;
}
