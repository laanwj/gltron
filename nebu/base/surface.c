#include "base/nebu_surface.h"
#include "base/nebu_png_texture.h"

#include <png.h>
#include "base/nebu_system.h"

#include "base/nebu_debug_memory.h"

// review: 64bit ok

int nebu_Surface_FormatSize[] = { 0, 3, 4, 1 };

nebu_Surface* nebu_Surface_Create(int width, int height, int format)
{
	nebu_Surface *pSurface = (nebu_Surface*) malloc( sizeof(nebu_Surface) );
	pSurface->w = width;
	pSurface->h = height;
	pSurface->format = format;
	pSurface->data = malloc(width * height * nebu_Surface_FormatSize[format]);
	return pSurface;
}

void nebu_Surface_Free(nebu_Surface* pSurface)
{
	free(pSurface->data);
	free(pSurface);
}

nebu_Surface* nebu_Surface_LoadPNG(const char *filename)
{
	nebu_Surface *pSurface;

	/* use old code to load texture, do format conversion */
	/* TODO (would be nice): clean this up, throw away old format */
	
	png_texture *pTex;
	pTex = load_png_texture(filename);
	if(!pTex)
		return NULL;

	pSurface = (nebu_Surface*) malloc( sizeof(nebu_Surface) );
	pSurface->w = pTex->width;
	pSurface->h = pTex->height;
	switch(pTex->channels) {
	case 1: pSurface->format = NEBU_SURFACE_FMT_ALPHA; break;
	case 3: pSurface->format = NEBU_SURFACE_FMT_RGB; break;
	case 4: pSurface->format = NEBU_SURFACE_FMT_RGBA; break;
	default: pSurface->format = NEBU_SURFACE_FMT_UNKNOWN; break;
	}
	pSurface->data = pTex->data;
	free(pTex);
	return pSurface;
}

static FILE *fp;


static void user_write_data(png_structp png_ptr,
		     png_bytep data, png_size_t length) {
  fwrite(data, length, 1, fp);
}

static void user_flush_data(png_structp png_ptr) {
  fflush(fp);
}

int nebu_Surface_SaveBMP(nebu_Surface *pSurface, const char *filename)
{
	int x, y;
	unsigned char *pixels;

	x = pSurface->w;
	y = pSurface->h;
	pixels = (unsigned char*) pSurface->data;

	{
		/* this code is shamelessly stolen from Ray Kelm, but I believe he
			put it in the public domain */
		SDL_Surface *temp;
		int i;

		temp = SDL_CreateRGBSurface(SDL_SWSURFACE, x, y, 24,
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
						0x000000FF, 0x0000FF00, 0x00FF0000, 0
		#else
						0x00FF0000, 0x0000FF00, 0x000000FF, 0
		#endif
						);

		if (temp == NULL)
			return -1;

		for (i = 0; i < y; i++)
			memcpy(((char *) temp->pixels) + temp->pitch * i, 
			pixels + 3 * x * (y - i - 1), x * 3);

		SDL_SaveBMP(temp, filename);
		SDL_FreeSurface(temp);
	}
	return 0;
}

int nebu_Surface_SavePNG(nebu_Surface *pSurface, const char *filename)
{
	png_structp png_ptr;
	png_infop info_ptr;
	png_byte **row_ptrs;
	int i;
	int png_format, bitdepth, bytesperpixel;

	switch(pSurface->format) {
		case NEBU_SURFACE_FMT_RGB: 
			png_format = PNG_COLOR_TYPE_RGB;
			bitdepth = 8;
			bytesperpixel = 3;
			break;
		case NEBU_SURFACE_FMT_RGBA:
			png_format = PNG_COLOR_TYPE_RGBA;
			bitdepth = 8;
			bytesperpixel = 4;
			break;
		case NEBU_SURFACE_FMT_ALPHA:
			png_format = PNG_COLOR_TYPE_GRAY;
			bitdepth = 8;
			bytesperpixel = 1;
			break;
		default:
			fprintf(stderr, "[savepng] unsupported format\n");
			return -1;
	}


	if (!(fp = fopen(filename, "wb"))) {
		fprintf(stderr, "[savepng] can't open %s for writing\n", filename);
		return -1;
	}

	if (!(png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
											NULL, NULL, NULL))) {
		return -1;
	}

	if (!(info_ptr = png_create_info_struct(png_ptr))) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return -1;
	}

	png_set_write_fn(png_ptr, 0, user_write_data, user_flush_data);
	/* png_init_io(png_ptr, fp); */

	png_set_IHDR(png_ptr, info_ptr, pSurface->w, pSurface->h,
					bitdepth, png_format, 
					PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
					PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);

	/* get pointers */
	if(!(row_ptrs = (png_byte**) malloc(pSurface->h * sizeof(png_byte*)))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return -1;
	}
	  
	for(i = 0; i < pSurface->h; i++) {
		row_ptrs[i] = pSurface->data + (pSurface->h - i - 1) 
		* nebu_Surface_FormatSize[pSurface->format] * pSurface->w;
	}

	png_write_image(png_ptr, row_ptrs);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr); 

	free(row_ptrs);
	fclose(fp);
	return 0;	
}

void nebu_Surface_Blit(nebu_Surface *pSrcSurface, nebu_Rect *pSrcRect, nebu_Surface* pDstSurface, nebu_Rect *pDstRect)
{
	nebu_Rect src, dst;
	int v, srcBpp, srcPitch, dstBpp, dstPitch;

	if(pSrcRect != NULL)
	{
		src = *pSrcRect;
	}
	else
	{
		src.x = src.y = 0;
		src.width = pSrcSurface->w;
		src.height = pSrcSurface->h;
	}
	if(pDstRect != NULL)
		dst = *pDstRect;
	else
	{
		dst.x = dst.y = 0;
	}
	// TODO: clip rects to surfaces

	srcBpp = nebu_Surface_FormatSize[pSrcSurface->format];
	srcPitch = pSrcSurface->w * srcBpp;
	dstBpp = nebu_Surface_FormatSize[pDstSurface->format];
	dstPitch = pDstSurface->w * dstBpp;

	// TODO: check if formats match, and do conversion if necessary
	for(v = 0; v < src.height; v++)
	{
		memcpy(pDstSurface->data + (v + dst.y) * dstPitch + dst.x * dstBpp,
			pSrcSurface->data + (v + src.y) * srcPitch + src.x * srcBpp,
			src.width * srcBpp);
	}
}	

