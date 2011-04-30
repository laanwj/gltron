#include "game/game_level.h"
#include "filesystem/path.h"
#include "Nebu_scripting.h"
#include <stdlib.h>

void game_FreeLevel(game_level *l) {
	free(l->boundaries);
	free(l->spawnPoints);
	free(l);
}

void game_ScaleLevel(game_level *l, float fSize)
{
	int i;
	for(i = 0; i < l->nBoundaries; i++)
	{
		vec2_Scale(& l->boundaries[i].vStart, &l->boundaries[i].vStart, fSize);
		vec2_Scale(& l->boundaries[i].vDirection, &l->boundaries[i].vDirection, fSize);
	}
	for(i = 0; i < l->nSpawnPoints; i++)
	{
		vec2_Scale(& l->spawnPoints[i].v, & l->spawnPoints[i].v, fSize);
	}

	vec2_Scale(& l->boundingBox.vMin, & l->boundingBox.vMin, fSize);
	vec2_Scale(& l->boundingBox.vMax, & l->boundingBox.vMax, fSize);
}

void computeBoundingBox(game_level *l)
{
	int i;

	box2_Init(& l->boundingBox);
	for(i = 0; i < l->nBoundaries; i++)
	{
		vec2 vEnd;
		vec2_Add(&vEnd, & l->boundaries[i].vStart, & l->boundaries[i].vDirection);
		box2_Extend(& l->boundingBox, & l->boundaries[i].vStart);
		box2_Extend(& l->boundingBox, & vEnd);
	}
}

game_level* game_CreateLevel(void) {
	int i;
	game_level* l;

	l = malloc( sizeof(game_level) );
	scripting_GetGlobal("level", NULL);
	// get scalability flag
	scripting_GetValue("scalable");
	scripting_GetIntegerResult(& l->scalable);
	// get number of spawnpoints
	scripting_GetValue("spawn");
	scripting_GetArraySize(& l->nSpawnPoints);
	// copy spawnpoints into vec2's
	l->spawnPoints = malloc(l->nSpawnPoints * sizeof(game_spawnpoint));

	// fixme, use scalability
	for(i = 0; i < l->nSpawnPoints; i++) {
		scripting_GetArrayIndex(i + 1);

		scripting_GetValue("x");
		scripting_GetFloatResult(& l->spawnPoints[i].v.v[0]);
		scripting_GetValue("y");
		scripting_GetFloatResult(& l->spawnPoints[i].v.v[1]);
		scripting_GetValue("dir");
		scripting_GetIntegerResult(& l->spawnPoints[i].dir);

		scripting_PopTable(); // index i
	}
	scripting_PopTable(); // spawn
	
	// get number of boundary segments
	scripting_GetValue("boundary");
	scripting_GetArraySize(& l->nBoundaries);
	// copy boundaries into segments
	l->boundaries = malloc(l->nBoundaries * sizeof(segment2));
	for(i = 0; i < l->nBoundaries; i++) {
		scripting_GetArrayIndex(i + 1);
		
		scripting_GetArrayIndex(1);
		scripting_GetValue("x");
		scripting_GetFloatResult(& l->boundaries[i].vStart.v[0]);
		scripting_GetValue("y");
		scripting_GetFloatResult(& l->boundaries[i].vStart.v[1]);
		scripting_PopTable(); // index 0
		
		scripting_GetArrayIndex(2);
		{
			vec2 v;
			scripting_GetValue("x");
			scripting_GetFloatResult(& v.v[0]);
			scripting_GetValue("y");
			scripting_GetFloatResult(& v.v[1]);
			vec2_Sub(& l->boundaries[i].vDirection, &v, &l->boundaries[i].vStart);
		}
		scripting_PopTable(); // index 1
	
		scripting_PopTable(); // index i
	}
	scripting_PopTable(); // boundary

	scripting_PopTable(); // level

	computeBoundingBox(l);

	return l;
}
