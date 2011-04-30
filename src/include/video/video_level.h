#ifndef VIDEO_LEVEL_H
#define VIDEO_LEVEL_H

#include "video/model.h"

typedef struct video_level_shader {
	int lit;
	float fDiffuseTextureScale;
	int passes; // number of passes necessary
	int ridTexture;	// resourceId for the texture
	int idTexture; // GL id for the texture, cached for each frame
} video_level_shader;

typedef struct video_level {
	gltron_Mesh *floor;
	gltron_Mesh *arena;

	video_level_shader floor_shader;
	video_level_shader arena_shader;
} video_level;

void video_LoadLevel(void);
void video_UnloadLevel(void);
void video_FreeLevel(video_level *l);
video_level* video_CreateLevel(void);
void video_ScaleLevel(video_level *l, float fSize);
void video_Shader_Setup(video_level_shader *shader, int pass);
void video_Shader_Cleanup(video_level_shader *shader, int pass);
void video_Shader_Geometry(gltron_Mesh *pMesh, gltron_MeshType eType, int pass);
void video_shader_InitResources(video_level_shader *shader);
#endif
