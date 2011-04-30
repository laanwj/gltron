#ifndef TRAIL_GEOMETRY_H
#define TRAIL_GEOMETRY_H

#include "base/nebu_vector.h"
#include "game/game_data.h" // Player
#include "video/video.h" // PlayerVisual

typedef struct TrailMesh {
	vec3* pVertices; // each vertex consists of three floats
	vec3* pNormals; // each normal consists of three float
	vec2* pTexCoords; // each texcoord consists of three float
	unsigned short *pIndices; // each triangle consists of three 16bit indices
	unsigned char *pColors; // each vertex color consists of 4 bytes
	unsigned int iSize;
	unsigned int iUsed;
} TrailMesh;

void storeVertex(TrailMesh *pMesh, int offset, 
								 segment2 *s, float t, float fFloor, float fTop,
								 float fSegLength, float fTotalLength);
void storeIndices(TrailMesh *pMesh, int indexOffset, int vertexOffset);
void trailGeometry(Player *pPlayer, Player_Profile *pProfile,
									 TrailMesh *pMesh,
								 int *pVertexOffset, int *pIndexOffset);
void bowGeometry(Player *pPlayer, Player_Profile *pProfile,
								 TrailMesh *pMesh,
								 int *pVertexOffset, int *pIndexOffset);
void trailRender(TrailMesh *pMesh);
void trailStatesNormal(Player *pPlayer, int texture);
void trailStatesShadowed(void);
void trailStatesRestore(void);

#endif
