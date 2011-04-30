#include "video/nebu_renderer_gl.h"
#include "video/nebu_video_system.h"
#include "video/nebu_texture2d.h"
#include "video/nebu_png_texture.h"
#include "filesystem/nebu_filesystem.h"

#include <string.h>

#include "base/nebu_debug_memory.h"
#include "base/nebu_assert.h"

static void loadTexture(const char *path, int format);
static png_texture* loadTextureData(const char *path);
static void freeTextureData(png_texture *tex);

void nebu_Texture2D_Free(nebu_Texture2D* pTexture)
{
	glDeleteTextures(1, &pTexture->id);
	free(pTexture);
}

nebu_Texture2D* nebu_Texture2D_Load(const char *path, const nebu_Texture2D_meta* meta)
{
	nebu_Texture2D *pTexture;
	pTexture = (nebu_Texture2D*)malloc(sizeof(nebu_Texture2D));

	glGenTextures(1, & pTexture->id);
	glBindTexture(GL_TEXTURE_2D, pTexture->id);

	loadTexture(path, meta->format);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, meta->min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, meta->mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, meta->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, meta->wrap_t);	
	if(GLEW_EXT_texture_filter_anisotropic)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
			meta->anisotropy);
	}
	return pTexture;
}


void freeTextureData(png_texture *tex) {
  free(tex->data);
  free(tex);
}

png_texture* loadTextureData(const char *path) {
	png_texture *tex = load_png_texture(path);

	if(tex == NULL) {    
		fprintf(stderr, "fatal: failed loading %s, exiting...\n", path);
		nebu_assert(0); exit(1); // OK: critical, installation corrupt
	}
	return tex;
}

void loadTexture(const char *path, int format) {
	png_texture *tex;
	GLint internal;
	int maxSize;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);

	tex = loadTextureData(path);
	if(tex->channels == 3)
		internal = GL_RGB;
	else
		internal = GL_RGBA;
	if(format == GL_DONT_CARE)
	{
		if(tex->channels == 3) format = GL_RGB;
		if(tex->channels == 4) format = GL_RGBA;
	}
	/* TODO: build mipmaps the proper way, box filters suck */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	{
		png_texture *newtex;
		int level = 0;
		while (tex->width > 1 || tex->height > 1)
		{
			if(tex->width <= maxSize && tex->height <= maxSize)
			{
				glTexImage2D(GL_TEXTURE_2D, level, format, 
					 tex->width, tex->height,
					 0, internal, GL_UNSIGNED_BYTE, tex->data);
#ifdef PRINTF_VERBOSE
				printf("uploading level %d, %dx%d texture\n", 
					 level, tex->width, tex->height);
#endif
				level++;
			}
			newtex = mipmap_png_texture(tex, 1, 0, 0);
			freeTextureData(tex);
			tex = newtex;
		}
		/* upload 1x1 mip level */
		glTexImage2D(GL_TEXTURE_2D, level, format, 
			 tex->width, tex->height,
			 0, internal, GL_UNSIGNED_BYTE, tex->data);
#ifdef PRINTF_VERBOSE
		printf("uploading level %d, %dx%d texture\n", 
			level, tex->width, tex->height);
#endif
		freeTextureData(tex);
	}
}
