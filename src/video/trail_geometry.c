#include "game/game_data.h"
#include "game/game.h"
#include "video/video.h"
#include "video/trail_geometry.h"
#include "configuration/settings.h"

#include "base/nebu_vector.h"
#include "base/nebu_math.h"
#include "video/nebu_renderer_gl.h"

#include <string.h>

enum {
	COLOR_TRAIL, COLOR_BRIGHT, COLOR_CYCLE
};

int cmpdir(segment2 *line1, segment2 *line2);

void float2ubyte(unsigned char* pubColor, float *pfColor) {
	pubColor[0] = (unsigned char)(pfColor[0] * 255.0f);
	pubColor[1] = (unsigned char)(pfColor[1] * 255.0f);
	pubColor[2] = (unsigned char)(pfColor[2] * 255.0f);
	pubColor[3] = (unsigned char)(pfColor[3] * 255.0f);
}
		 
void storeColor(TrailMesh *pMesh, int offset, Player_Profile* pProfile, int type) {
  float color[] = { 0, 0, 0, 1 };
	float white[] = { 1, 1, 1, 1 };

	switch(type) {
	case COLOR_TRAIL:
		if(gSettingsCache.alpha_trails)
			memcpy(color, pProfile->pColorAlpha, 4 * sizeof(float));
		else
			memcpy(color, pProfile->pColorAlpha, 3 * sizeof(float));
		break;
	case COLOR_BRIGHT:
		memcpy(color, white, 3 * sizeof(float));
		break;
	case COLOR_CYCLE:
		memcpy(color, pProfile->pColorDiffuse, 3 * sizeof(float));
		break;
	}
	float2ubyte(pMesh->pColors + 4 * offset, color);
	float2ubyte(pMesh->pColors + 4 * (offset + 1), color);
}
		
void storeVertex(TrailMesh *pMesh, int offset, 
								 segment2 *s, float t, /* 0: start, 1: end */
								 float fFloor, float fTop, 
								 float fSegLength, float fTotalLength) {
	vec3 *pVertices = pMesh->pVertices + offset;
	vec3 *pNormals = pMesh->pNormals + offset;
	vec2 *pTexCoords = pMesh->pTexCoords + offset;
	vec3 v;
	vec2 uv;
	float fUStart;
	vec3 pvNormals[] = {
		{ { 1, 0, 0 } },
		{ { -1, 0, 0 } },
		{ { 0, 1, 0 } },
		{ { 0, -1, 0 } }
	};
	
	
	int iNormal;
	if(s->vDirection.v[0] == 0)
		// iNormal = (line->sx <= line->ex) ? 0 : 1;
		iNormal = 0;
	else
		// iNormal = (line->sy <= line->sy) ? 2 : 3;
		iNormal = 2;

	fUStart = (fTotalLength / DECAL_WIDTH) - floorf(fTotalLength / DECAL_WIDTH);
	
	v.v[0] = s->vStart.v[0] + t * s->vDirection.v[0];
	v.v[1] = s->vStart.v[1] + t * s->vDirection.v[1];
	v.v[2] = fFloor;
	uv.v[0] = fUStart + t * fSegLength / DECAL_WIDTH;

	uv.v[1] = 0;
	vec3_Copy(pVertices, &v);
	vec3_Copy(pNormals, pvNormals + iNormal);
	vec2_Copy(pTexCoords, &uv);
			
	v.v[2] = fTop;
	uv.v[1] = 1;
	vec3_Copy(pVertices + 1, &v);
	vec3_Copy(pNormals + 1, pvNormals + iNormal);
	vec2_Copy(pTexCoords + 1, &uv);
}

void storeIndices(TrailMesh *pMesh, int indexOffset, int vertexOffset) {
	unsigned short ppBase[2][6] = {
		{ 0, 2, 1, 2, 3, 1 },
		{ 0, 1, 2, 1, 3, 2 }
	};
	int i;
	int winding;

	if(pMesh->pVertices[vertexOffset].v[0] ==
		 pMesh->pVertices[vertexOffset + 2].v[0])
		winding = (pMesh->pVertices[vertexOffset].v[1] <= 
							 pMesh->pVertices[vertexOffset + 2].v[1]) ? 0 : 1;
	else
		winding = (pMesh->pVertices[vertexOffset].v[0] < 
							 pMesh->pVertices[vertexOffset + 2].v[0]) ? 1 : 0;
	
	for(i = 0; i < 6; i++) {
		pMesh->pIndices[i + indexOffset] = ppBase[winding][i] + vertexOffset;
	}
}

int cmpdir(segment2 *s1, segment2 *s2) {
	if((s1->vDirection.v[0] == 0 && s2->vDirection.v[0] == 0) ||
		 (s1->vDirection.v[1] == 0 && s2->vDirection.v[1] == 0))
		return 0;
	return 1;
}

void trailGeometry(Player *pPlayer, Player_Profile *pProfile,
									 TrailMesh *pMesh,
									 int *pvOffset, int *piOffset) {
	Data *pData = &pPlayer->data;
	int curVertex = *pvOffset, curIndex = *piOffset;
	int i;
	float fTotalLength = 0;
	float fSegLength;
	// draw all trails except for the last one
	for(i = 0; i < pData->nTrails - 1; i++) {
		fSegLength = segment2_Length(pData->trails + i);
		if(i == 0 || cmpdir(pData->trails + i - 1, pData->trails + i)) {
			storeVertex(pMesh, curVertex, pData->trails + i, 0, 
									0, pData->trail_height, 
									fSegLength, fTotalLength);
			storeColor(pMesh, curVertex, pProfile, COLOR_TRAIL);
			curVertex += 2;
		}
			
		storeVertex(pMesh, curVertex, pData->trails + i, 1,
								0, pData->trail_height,
								fSegLength, fTotalLength);
		storeColor(pMesh, curVertex, pProfile, COLOR_TRAIL);
		curVertex += 2;

		storeIndices(pMesh, curIndex, curVertex - 4);
		curIndex += 6;

		fTotalLength += fSegLength;

	}
	{
		// the last trail
		segment2 s;
		vec2_Copy(&s.vStart, & pData->trails[pData->nTrails - 1].vStart);
		s.vDirection.v[0] = getSegmentEndX( pData, 1 ) - s.vStart.v[0];
		s.vDirection.v[1] = getSegmentEndY( pData, 1 ) - s.vStart.v[1];

		fSegLength = segment2_Length(&s);
		
		storeVertex(pMesh, curVertex, &s, 0,
								0, pData->trail_height, 
								fSegLength, fTotalLength);
		storeColor(pMesh, curVertex, pProfile, COLOR_TRAIL);
		curVertex += 2;
		
		storeVertex(pMesh, curVertex, &s, 1,
								0, pData->trail_height, 
								fSegLength, fTotalLength);
		storeColor(pMesh, curVertex, pProfile, COLOR_TRAIL);
		curVertex += 2;

		storeIndices(pMesh, curIndex, curVertex - 4);
		curIndex += 6;

		fTotalLength += fSegLength;

		vec2_Add(&s.vStart, &s.vStart, &s.vDirection);
		s.vDirection.v[0] = getSegmentEndX( pData, 0 ) - s.vStart.v[0];
		s.vDirection.v[1] = getSegmentEndY( pData, 0 ) - s.vStart.v[1];
		fSegLength = segment2_Length(&s);

		storeVertex(pMesh, curVertex, &s, 0,
								0, pData->trail_height, 
								fSegLength, fTotalLength);
		storeColor(pMesh, curVertex, pProfile, COLOR_TRAIL);
		curVertex += 2;

		storeVertex(pMesh, curVertex, &s, 1,
								0, pData->trail_height, 
								fSegLength, fTotalLength);
		storeColor(pMesh, curVertex, pProfile, COLOR_BRIGHT);
		curVertex += 2;

		storeIndices(pMesh, curIndex, curVertex - 4);
		curIndex += 6;
	}

	pMesh->iUsed += curIndex - *piOffset;
	*piOffset = curIndex;
	*pvOffset = curVertex;
}

void bowGeometry(Player *pPlayer, Player_Profile *pProfile,
								 TrailMesh *pMesh, int *pvOffset, int *piOffset) {
	Data *pData = &pPlayer->data;
	segment2 s;
	int bdist = PLAYER_IS_ACTIVE(pPlayer) ? 2 : 3;
	int i;
	int vOffset = *pvOffset; int iOffset = *piOffset;

	s.vStart.v[0] = getSegmentEndX( pData, 0 );
	s.vStart.v[1] = getSegmentEndY( pData, 0 );
	s.vDirection.v[0] = getSegmentEndX( pData, bdist ) - s.vStart.v[0];
	s.vDirection.v[1] = getSegmentEndY( pData, bdist ) - s.vStart.v[1];

	for(i = 0; i < 10; i++) {
		float t = i * 1.0f / 10;
		float fTop = sqrtf(1 - t*t);
		float fFloor = (t < 0.6f) ? 0 : 0.5f * (t - 0.6f);
		if(fTop < 0.3f) fTop = 0.3f;
		
		storeVertex(pMesh, vOffset, &s, t, 
								fFloor * pPlayer->data.trail_height, 
								fTop * pPlayer->data.trail_height, 
								DECAL_WIDTH, 0);
		storeColor(pMesh, vOffset, pProfile, COLOR_BRIGHT);
		vOffset += 2;
		if(i) {
			storeIndices(pMesh, iOffset, vOffset - 4);
			iOffset += 6;
		}
	}
	storeVertex(pMesh, vOffset, &s, 1, 
							0.2f * pData->trail_height, 0.3f * pData->trail_height, 
							DECAL_WIDTH, 0);
	storeColor(pMesh, vOffset, pProfile, COLOR_CYCLE);
	vOffset += 2;
	storeIndices(pMesh, iOffset, vOffset - 4);

	pMesh->iUsed += iOffset - *piOffset;
	*piOffset = iOffset;
	*pvOffset = vOffset;
}

