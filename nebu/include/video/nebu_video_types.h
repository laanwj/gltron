#ifndef NEBU_VIDEO_TYPES_H
#define NEBU_VIDEO_TYPES_H

typedef struct {
  int win_id;     /* only the root visual has an id */
  int h, w;       /* window dimension */
  int vp_x, vp_y; /* viewport position */
  int vp_h, vp_w; /* viewport dimension */

  int onScreen;

  int *ridTextures;
  unsigned int *textures; /* texture IDs */
} Visual;

#endif
