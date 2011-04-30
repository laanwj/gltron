#include "video/video.h"
#include "configuration/settings.h"
#include "game/resource.h"
#include "base/nebu_resource.h"
#include "base/nebu_surface.h"
#include "video/nebu_2d.h"
#include "video/nebu_texture2d.h"
#include "video/nebu_video_system.h"
#include "video/nebu_renderer_gl.h"

// TODO (resource): move this stuff into level & gui loading code
/*!
	load skybox, trail & explosion texture
*/
void initTexture(Visual *d) {
	GLint min_filter, mag_filter;

	int i, j;

	switch(getSettingi("mipmap_filter"))
	{
	case NEAREST:
		min_filter = GL_NEAREST;
		break;
	case LINEAR:
		min_filter = GL_LINEAR;
		break;
	case MIPMAP:
		min_filter = GL_LINEAR_MIPMAP_NEAREST;
		break;
	case TRILINEAR:
		min_filter = GL_LINEAR_MIPMAP_LINEAR;
		break;
	default:
		min_filter = GL_LINEAR_MIPMAP_LINEAR;
	}

	mag_filter = GL_LINEAR;

	if(getSettingi("softwareRendering"))
	{
		min_filter = GL_NEAREST;
		mag_filter = GL_NEAREST;
	}

	printf("using min_filter: %d (setting: %d)\n", min_filter,
		getSettingi("mipmap_filter"));

	for(i = 0; i < TEX_COUNT; i++)
	{
		for( j = 0; j < textures[i].count; j++)
		{
			nebu_Texture2D_meta meta;
			meta.format = textures[i].type;
			meta.mag_filter = mag_filter;
			meta.min_filter = min_filter;
			meta.wrap_s = textures[i].wrap_s;
			meta.wrap_t = textures[i].wrap_t;
			meta.anisotropy = textures[i].anisotropy;

			d->ridTextures[ textures[i].id + j ] =
				resource_GetTokenMeta(textures[i].name, eRT_Texture, &meta, sizeof(nebu_Texture2D_meta));
		}
	}
}

