#include "game/resource.h"
#include "base/nebu_util.h"
#include "base/nebu_resource.h"
#include "video/nebu_font.h"
#include "video/nebu_texture2d.h"
#include "filesystem/nebu_filesystem.h"
#include "base/nebu_assert.h"
#include <string.h>

// TODO: move user type load functions somewhere else
#include "filesystem/path.h"
#include "video/model.h"

#include "base/nebu_debug_memory.h"

void* getTriMesh(char *filename, void *dummy)
{
	return gltron_Mesh_LoadFromFile(filename, TRI_MESH);
}

void* getQuadMesh(char *filename, void *dummy)
{
	return gltron_Mesh_LoadFromFile(filename, QUAD_MESH);
}

static void releaseMesh(void *pData)
{
	gltron_Mesh_Free((gltron_Mesh*)pData);
}

void* get2d(char *filename, void *dummy)
{
	char *path;
	path = nebu_FS_GetPath(PATH_ART, filename);
	if(path)
	{
		void *pData = nebu_2d_LoadPNG(path, 0);
		free(path);
		return pData;
	}
	else
	{
		fprintf(stderr, "failed to locate %s", filename);
		nebu_assert(0); exit(1); // installation corrupt
	}
}

static void release2d(void *pData)
{
	nebu_2d_Free((nebu_2d*)pData);
}

void* getFont(char *filename, void *dummy)
{
	return nebu_Font_Load(filename, PATH_ART);
}

static void releaseFont(void *pData)
{
	nebu_Font_Free(pData);
}

void* getTexture(char *filename, void *meta)
{
	char *path;
	path = nebu_FS_GetPath(PATH_ART, filename);
	if(path)
	{
		void *pData = nebu_Texture2D_Load(path, (nebu_Texture2D_meta*) meta);
		free(path);
		return pData;
	}
	else
	{
		fprintf(stderr, "failed to locate %s", filename);
		nebu_assert(0); exit(1); // installation corrupt
	}
}

static void releaseTexture(void *pData)
{
	nebu_Texture2D_Free((nebu_Texture2D*)pData);
}

void resource_Init()
{
	resource_RegisterHandler(eRT_GLtronTriMesh, getTriMesh, releaseMesh);
	resource_RegisterHandler(eRT_GLtronQuadMesh, getQuadMesh, releaseMesh);
	resource_RegisterHandler(eRT_2d, get2d, release2d);
	resource_RegisterHandler(eRT_Font, getFont, releaseFont);
	resource_RegisterHandler(eRT_Texture, getTexture, releaseTexture);
}

void resource_Shutdown(void)
{
	resource_UnregisterHandler(eRT_GLtronTriMesh);
	resource_UnregisterHandler(eRT_GLtronQuadMesh);
	resource_UnregisterHandler(eRT_2d);
	resource_UnregisterHandler(eRT_Font);
	resource_UnregisterHandler(eRT_Texture);
}
