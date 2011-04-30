#ifndef NEBU_RENDERER_GL_H
#define NEBU_RENDERER_GL_H

// #define OPENGL_ES

#ifdef OPENGL_ES
#include <OpenGLES/ES1/gl.h>
#else // OPENGL_ES
#define GLEW_STATIC
#include "../../glew/include/GL/glew.h"
#endif // !OPENGL_ES

#include <stdio.h>

#define POLY_COUNT

#define RENDERER_TYPE_ALL 255
#define RENDERER_TYPE_COLOR 1
#define RENDERER_TYPE_NORMAL 2
#define RENDERER_TYPE_TEXTURE_COORD 4
#define RENDERER_TYPE_TEXTURE 8
#define RENDERER_TYPE_TEXTURE_MODE 16

extern void initRenderer();
extern void printRendererInfo();
extern void clearState();

typedef struct GLstate {
  int tex_id; /* current texture */
  int tex_env_mode; /* current texture env mode */
  int binds; /* texture bind changes - used for statistics only */
  int mod_changes;   /* mode changes - used for statistics only */
  int type_mask; /* which modes to change */
} GLstate;

typedef struct Renderer {
  /* shows capabilities of the renderer */
  int ext_filter_anisotropic;
} Renderer;

extern Renderer renderer;
extern GLstate *state;

#endif
