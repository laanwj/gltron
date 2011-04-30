#ifndef NEBU_PNG_TEXTURE_H
#define NEBU_PNG_TEXTURE_H

typedef struct {
  int width;
  int height;
  int channels;
  unsigned char *data;
} png_texture;

extern png_texture* load_png_texture(const char *filename);
extern void unload_png_texture(png_texture *tex);
extern png_texture* mipmap_png_texture(png_texture *source, int level, 
				       int clamp_u, int clamp_v);
#endif

