#include "video/nebu_png_texture.h"

#include <stdio.h>
#include <stdlib.h>

#if !defined(WIN32) && !defined(macintosh)
#include <unistd.h>
#endif

#define ERR_PREFIX "[load_png_texture] "

#include <png.h>

FILE *f;

void user_read_data(png_structp png_ptr,
		    png_bytep data, png_size_t length) {
  fread(data, 1, length, f);
}

png_texture* load_png_texture(const char *filename) {
  unsigned int i;
  int bpc, color_type, zsize;

  png_texture *tex;
  /* the following is defined in <png.h> */
  png_uint_32 x, y;
  png_structp png_ptr;
  png_infop info_ptr;
  png_byte **row_pointers;
  
  f = fopen(filename, "rb");
  if(f == NULL) {
    fprintf(stderr, ERR_PREFIX "can't open file %s\n", filename);
    return NULL;
  }

  png_ptr = png_create_read_struct
    (PNG_LIBPNG_VER_STRING, 0, 0, 0);

  if (!png_ptr)
    return NULL;

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr){
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return NULL;
  }

  /*  png_init_io(png_ptr, f);
      fprintf(stderr, "png, after init io \n");  */

  /* I need to do this, otherwise it crashes on win32 */
  png_set_read_fn(png_ptr, 0, user_read_data);
  /* fprintf(stderr, "png, after init user_read_data\n"); */

  png_read_info(png_ptr, info_ptr);
  /* fprintf(stderr, "png, after read info \n"); */
  png_get_IHDR(png_ptr, info_ptr, &x, &y, &bpc, &color_type, 0, 0, 0);

  if(color_type != PNG_COLOR_TYPE_RGB &&
     color_type != PNG_COLOR_TYPE_RGB_ALPHA) {
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    fprintf(stderr, ERR_PREFIX "wrong png_color_type\n");
    fclose(f);
    return NULL;
  }

  if(bpc != 8) {
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    fprintf(stderr, ERR_PREFIX "wrong bitdepth: %d\n", bpc);
    fclose(f);
    return NULL;
  }
 
  switch(color_type) {
  case PNG_COLOR_TYPE_RGB: zsize = 3; break;
  case PNG_COLOR_TYPE_RGB_ALPHA: zsize = 4; break;
  default: 
    fprintf(stderr, "unknown png color type\n");
    return NULL;
  }

  tex = (png_texture*) malloc(sizeof(png_texture));
  tex->data = malloc(x * y * zsize);
  tex->width = x;
  tex->height = y;
  tex->channels = zsize;
  
    /* get pointers */
  row_pointers = (png_byte**) malloc(y * sizeof(png_byte*));
  for(i = 0; i < y; i++)
    row_pointers[i] = tex->data + (y - i - 1) 
      * zsize * x;

  png_read_image(png_ptr, row_pointers);
  png_destroy_read_struct(&png_ptr, &info_ptr, 0);

  free(row_pointers);
  fclose(f);
  return tex;
}

void unload_png_texture(png_texture *tex) {
  free(tex->data);
  free(tex);
}

#define TARGET(X, Y, I) (mip->data + I + mip->channels * (X + Y * mip->width))
#define SOURCE(X, Y, I) (source->data + I + \
			 source->channels * (X + Y * source->width))

png_texture* mipmap_png_texture(png_texture *source, int level, 
		    int clamp_u, int clamp_v) {
  png_texture *mip;
  int i;
  int x, y;
  int fx, fy;

  if(level != 1) return NULL;
  mip = (png_texture*) malloc(sizeof(png_texture));
  mip->channels = source->channels;
  fx = (source->width > 1) ? 2 : 1;
  fy = (source->height > 1) ? 2 : 1;
			  
  mip->width = source->width / fx;
  mip->height = source->height / fy;
  mip->data = (unsigned char*) malloc(mip->width * mip->height *
				      mip->channels);

  /* simple linear filtering */
  for(i = 0; i < mip->channels; i++) {
    for(y = 0; y < mip->height; y++) {
      for(x = 0; x < mip->width; x++) {
	/*
	*( TARGET(x, y, i) ) = ( *( SOURCE(x * fx, y * fy, i) )  +
				 *( SOURCE((x + 1) * fx, (y + 1) * fy, i) ) +
				 *( SOURCE((x + 1) * fx, y * fy, i) ) + 
				 *( SOURCE(x * fx, (y + 1) * fy, i) ) ) / 4;
				 */
	int sx, sy, xoff, yoff;
	sx = fx * x;
	sy = fy * y;
	xoff = fx - 1;
	yoff = fy - 1;

	*( mip->data + i + mip->channels * ( x + y * mip->width ) ) =
	  ( *( source->data + i + source->channels * ( sx + sy * source->width) ) +
	    *( source->data + i + source->channels * ( sx + xoff + sy * source->width) ) +
	    *( source->data + i + source->channels * ( sx + xoff + (sy + yoff) * source->width) ) +
	    *( source->data + i + source->channels * ( sx + (sy + yoff) * source->width) ) ) / 4;

      }
    }
  }
  return mip;
}
