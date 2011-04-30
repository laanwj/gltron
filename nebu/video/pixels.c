#include "base/nebu_debug_memory.h"

unsigned char *scalePixelBitmap(unsigned char *source, int sw, int sh, 
			   int posx, int posy, int width, int height,
			   int dw, int dh, int bytes) {
  int x, y;
  unsigned char *data;
  unsigned char byte;
  int i;

  int row;

  row = dw / 8;
  if(dw % 8) row++;
  
  data = (unsigned char*) malloc( row * dh );
  for(y = 0; y < dh; y++) {
    byte = 0;
    for(x = 0; x < dw; x++) {
      int sx, sy;
      sx = posx + ((x * width) / dw);
      sy = posy + ((y * height) / dh);
      for(i = 0; (i < bytes) && (i < 3); i++) {
	if(source[bytes * (sx + sy * sw) + i] != 0) {
	  /* printf("source value: (%d %d, %d): %d\n", 
	     sx, sy, i, source[bytes * (sx + sy * sw) + i]); */
	  byte |= (1 << (x % 8) );
	  break;
	} else {
	  /* printf("source value: (%d %d, %d): %d\n", 
	     sx, sy, i, source[bytes * (sx + sy * sw) + i]); */
	}
      }
      if((x % 8) == 7) {
	data[x / 8 + y * row] = byte;
      }
    }
    if(x % 8)
      data[x / 8 + y * row] = byte;
    byte = 0;
  }
  return data;
}

unsigned char *scalePixels(const unsigned char *source, int sw, int sh, 
			   int posx, int posy, int width, int height,
			   int dw, int dh, int bytes) {
  int x, y;
  unsigned char *data;
  /*
  fprintf(stderr, "scaling from (%d,%d-%d,%d) in a (%d, %d)"
	  "to (%d,%d), bytes: %d\n", posx, posy, posx + width, posy + width,
	  sw, sh, dw, dh, bytes);
  */
  data = (unsigned char*) malloc( dw * dh * bytes );
  for(y = 0; y < dh; y++) {
    for(x = 0; x < dw; x++) {
      int sx, sy, j;
      int sx2, sy2;
      
      sx = posx + ((x * width) / dw);
      sy = posy + ((y * height) / dh);
      (sx == sw - 1) ? (sx2 = sx - 1) : (sx2 = sx + 1);
      (sy == sh - 1) ? (sy2 = sy - 1) : (sy2 = sy + 1);
      /* printf("%d, %d to %d, %d\n", x, y, sx, sy); */
      for(j = 0; j < bytes; j++) {
	data[bytes * (x + y * dw) + j] = 
	  (source[bytes * (sx + sy * sw) + j] +
	   source[bytes * (sx2 + sy * sw) + j] +
	   source[bytes * (sx + sy2 * sw) + j] +
	   source[bytes * (sx2 + sy2 * sw) + j]) / 4;
      }
    }
  }
  return data;
}
  







