#include "game/game_level.h"
#include "game/resource.h"
#include "base/nebu_resource.h"
#include "video/model.h"
#include "video/video.h"
#include "filesystem/path.h"
#include "Nebu_scripting.h"

#include "base/nebu_debug_memory.h"

#include "base/nebu_assert.h"
#include <math.h>
#include <string.h>

/*! Scales the level by the factor fScale

	- The boundary segments are scaled
	- The boundingbox is scaled
	- If the spawn coordinates are not relative to the bounding box, they
	  are scaled along accordingly
*/
void game_ScaleLevel(game_level *l, float fScale)
{
	int i, j;
	for(i = 0; i < l->nBoundaries; i++)
	{
		segment2_Scale(&l->boundaries[i], fScale);
	}
	if(!l->spawnIsRelative)
	{
		for(j = 0; j < l->nSpawnSets; j++)
		{
			for(i = 0; i < l->ppSpawnSets[j]->nPoints; i++)
			{
				vec2_Scale(& l->ppSpawnSets[j]->pSpawnPoints[i].vStart, fScale);
				vec2_Scale(& l->ppSpawnSets[j]->pSpawnPoints[i].vEnd, fScale);
			}
		}
	}

	box2_Scale(&l->boundingBox, fScale);
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

void computeBoundaries(game_level *l)
{
	int i, j;
	gltron_Mesh *pFloor = resource_Get(gpTokenCurrentFloor, eRT_GLtronTriMesh);
	int iCurIndex;

	nebu_assert(pFloor);
	nebu_assert(pFloor->pSI);

	if(!pFloor->pSI->pAdjacency)
	{
		pFloor->pSI->pAdjacency = nebu_Mesh_Adjacency_Create(pFloor->pSI->pVB, pFloor->pSI->pIB);
	}

	l->nBoundaries = 0;
	for(i = 0; i < pFloor->pSI->pAdjacency->nTriangles; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(pFloor->pSI->pAdjacency->pAdjacency[3 * i + j] == -1)
			{
				l->nBoundaries += 1;
			}
		}
	}
	l->boundaries = malloc(l->nBoundaries * sizeof(segment2));

	iCurIndex = 0;
	for(i = 0; i < pFloor->pSI->pAdjacency->nTriangles; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(pFloor->pSI->pAdjacency->pAdjacency[3 * i + j] == -1)
			{
				vec3 *pV1, *pV2;
				pV1 = (vec3*)(pFloor->pSI->pVB->pVertices +
					3 * pFloor->pSI->pIB->pIndices[3 * i + j]);
				pV2 = (vec3*)(pFloor->pSI->pVB->pVertices +
					3 * pFloor->pSI->pIB->pIndices[3 * i + (j + 1) % 3]);
					
				nebu_assert(iCurIndex < l->nBoundaries);
				{
					float fEpsilon = 0.001f;
					nebu_assert(fabs(pV1->v[2]) < fEpsilon);
					nebu_assert(fabs(pV2->v[2]) < fEpsilon);
				}
				l->boundaries[iCurIndex].vStart = *(vec2*)pV1;
				vec2_Sub(&l->boundaries[iCurIndex].vDirection, (vec2*)pV2, (vec2*)pV1);

				iCurIndex++;
			}
		}
	}
	nebu_assert(iCurIndex == l->nBoundaries);
}	

void game_UnloadLevel(void)
{
	// delete the current (global) lua table
	// delete global 'level' table (garbage collected)
	scripting_Run("level = nil");
}

int game_LoadLevel(void)
{
	// Load the selected level into the (global) lua table
	char *path; 
	char *pFilename;

	int iPos = scripting_StackGuardStart();

	// make sure there's no level already loaded
	scripting_GetGlobal("level", NULL);
	nebu_assert(scripting_IsNil());
	if(!scripting_IsNil())
	{
		scripting_Pop();
		scripting_StackGuardEnd(iPos);
		return GAME_ERROR_LEVEL_ALREADYLOADED;
	}
	scripting_Pop();

	scripting_GetGlobal("settings", "current_level", NULL);
	scripting_GetStringResult(&pFilename);
	fprintf(stderr, "[status] loading level '%s'\n", pFilename);
	path = getPath(PATH_LEVEL, pFilename);
	scripting_StringResult_Free(pFilename);

	if(path) {
		scripting_RunFile(path);
		free(path);
	}
	else
	{
		// fail silently, unloaded lvl will be caught later
	}

	scripting_GetGlobal("level", NULL);
	nebu_assert(!scripting_IsNil());
	if(scripting_IsNil())
	{
		scripting_Pop();
		scripting_StackGuardEnd(iPos);
		return GAME_ERROR_LEVEL_NOTLOADED;
	}
	scripting_Pop(); // level

	scripting_StackGuardEnd(iPos);

	return GAME_SUCCESS;
}

void game_spawnset_Free(game_spawnset* pSpawnSet)
{
	if(pSpawnSet->nPoints)
	{
		nebu_assert(pSpawnSet->pSpawnPoints);
		free(pSpawnSet->pSpawnPoints);
	}
	free(pSpawnSet);
}

game_spawnset* game_spawnset_Create(void)
{
	scripting_StringResult s;
	int i;

	game_spawnset* pSpawnSet = (game_spawnset*) malloc(sizeof(game_spawnset));

	int iPos = scripting_StackGuardStart();

	// get spawnset type
	scripting_GetValue("type");
	nebu_assert(!scripting_IsNil());
	scripting_GetStringResult(&s);
	if(strcmp(s, "list") == 0) { pSpawnSet->type = eGameSpawnPoint; }
	else if(strcmp(s, "lines") == 0) { pSpawnSet->type = eGameSpawnLine; }
	else { pSpawnSet->type = eGameSpawnUndef; }
	scripting_StringResult_Free(s);

	scripting_GetValue("set");
	nebu_assert(!scripting_IsNil());

	scripting_GetArraySize(& pSpawnSet->nPoints);
	nebu_assert(pSpawnSet->nPoints);

	pSpawnSet->pSpawnPoints = malloc(pSpawnSet->nPoints * sizeof(game_spawnpoint));

	// Spawn points are relative to the bounding box of the floor
	for(i = 0; i < pSpawnSet->nPoints; i++)
	{
		game_spawnpoint *pSpawnPoint = pSpawnSet->pSpawnPoints + i;

		scripting_GetArrayIndex(i + 1);

		switch(pSpawnSet->type)
		{
		case eGameSpawnPoint:
			scripting_GetValue("x");
			scripting_GetFloatResult(& pSpawnPoint->vStart.v[0]);
			scripting_GetValue("y");
			scripting_GetFloatResult(& pSpawnPoint->vStart.v[1]);
			scripting_GetValue("dir");
			scripting_GetIntegerResult(& pSpawnPoint->dir);
			break;
		case eGameSpawnLine:
			scripting_GetValue("vStart");
			scripting_GetValue("x");
			scripting_GetFloatResult(& pSpawnPoint->vStart.v[0]);
			scripting_GetValue("y");
			scripting_GetFloatResult(& pSpawnPoint->vStart.v[1]);
			scripting_Pop(); // vStart

			scripting_GetValue("vEnd");
			scripting_GetValue("x");
			scripting_GetFloatResult(& pSpawnPoint->vEnd.v[0]);
			scripting_GetValue("y");
			scripting_GetFloatResult(& pSpawnPoint->vEnd.v[1]);
			scripting_Pop(); // vEnd

			scripting_GetValue("n");
			scripting_GetIntegerResult(& pSpawnPoint->n);
			if(pSpawnPoint->n == 0)
			{
				// TODO: find a different limit somehow?
				pSpawnPoint->n = 999;
			}

			scripting_GetValue("dir");
			scripting_GetIntegerResult(& pSpawnPoint->dir);
			break;
		default:
			nebu_assert(0);
			break;
		}

		scripting_Pop(); // index i
	}

	scripting_Pop(); // set

	scripting_StackGuardEnd(iPos);

	return pSpawnSet;
}

void game_FreeLevel(game_level *l) {
	int i;

	if(l->nAxis)
	{
		nebu_assert(l->pAxis);
		free(l->pAxis);
	}

	if(l->nBoundaries)
	{
		nebu_assert(l->boundaries);
		free(l->boundaries);
	}
	nebu_assert(l->nSpawnSets);
	for(i = 0; i < l->nSpawnSets; i++)
	{
		game_spawnset_Free(l->ppSpawnSets[i]);
	}
	free(l->ppSpawnSets);
	free(l);
}

/*!
	parses the lua description of the current level, computes the level's
	boundingbox and, if necessary, computers a boundary using the level's
	floor's adjacency data
*/

game_level* game_CreateLevel(void)
{
	int i;
	game_level* l;
	int iHasBoundary;

	l = malloc( sizeof(game_level) );
	scripting_GetGlobal("level", NULL);
	nebu_assert(!scripting_IsNil());
	// get scale factor, if present
	scripting_GetOptional_Float("scale_factor", &l->scale_factor, 1);
	// are spawn points relative?
	scripting_GetOptional_Int("spawn_is_relative", &l->spawnIsRelative, 1);

	// get number of spawnpoints
	scripting_GetValue("spawn");
	nebu_assert(!scripting_IsNil());
	scripting_GetArraySize(& l->nSpawnSets);

	l->ppSpawnSets = malloc(l->nSpawnSets * sizeof(game_spawnset*));

	for(i = 0; i < l->nSpawnSets; i++)
	{
		scripting_GetArrayIndex(i + 1);
		l->ppSpawnSets[i] = game_spawnset_Create();
		scripting_Pop(); // index
	}
	scripting_Pop(); // spawn
	
	// TODO: in testing

	// two possibilities:
	// 1) The level contains boundary, vertices & indices fields,
	//    so we can load everything from there
	// 2) There's a 'model' field, and the boundaries	are the triangle edges
	//    without an adjacency

	scripting_GetValue("boundary");
	iHasBoundary = scripting_IsNil() ? 0 : 1;
	scripting_Pop();

	if(iHasBoundary)
	{
		// store boundary from lua
		scripting_GetValue("boundary");

		// get number of boundary segments
		scripting_GetArraySize(& l->nBoundaries);
		// copy boundaries into segments
		l->boundaries = malloc(l->nBoundaries * sizeof(segment2));
		for(i = 0; i < l->nBoundaries; i++)
		{
			scripting_GetArrayIndex(i + 1);
			
			scripting_GetArrayIndex(1);
			scripting_GetValue("x");
			scripting_GetFloatResult(& l->boundaries[i].vStart.v[0]);
			scripting_GetValue("y");
			scripting_GetFloatResult(& l->boundaries[i].vStart.v[1]);
			scripting_Pop(); // index 0
			
			scripting_GetArrayIndex(2);
			{
				vec2 v;
				scripting_GetValue("x");
				scripting_GetFloatResult(& v.v[0]);
				scripting_GetValue("y");
				scripting_GetFloatResult(& v.v[1]);
				vec2_Sub(& l->boundaries[i].vDirection, &v, &l->boundaries[i].vStart);
			}
			scripting_Pop(); // index 1
		
			scripting_Pop(); // index i
		}
		scripting_Pop(); // boundary
	}		
	else
	{
		if(!gpTokenCurrentFloor)
		{
			// needs either floor mesh or boundary description
			fprintf(stderr, "fatal: arena incomplete - exiting...\n");
			nebu_assert(0); exit(1); // OK (somewhat): the level is unplayable. could be handled better
		}
		// compute boundaries from level model
		computeBoundaries(l);
	}

	// load movement directions
	// store boundary from lua
	scripting_GetValue("axis");

	// get number of movement directions
	scripting_GetArraySize(& l->nAxis);
	// copy movement directions
	l->pAxis = malloc(l->nAxis * sizeof(vec2));
	for(i = 0; i < l->nAxis; i++)
	{
		scripting_GetArrayIndex(i + 1);
		
		scripting_GetValue("x");
		scripting_GetFloatResult(& l->pAxis[i].v[0]);
		scripting_GetValue("y");
		scripting_GetFloatResult(& l->pAxis[i].v[1]);
			
		scripting_Pop(); // index i
	}
	scripting_Pop(); // axis

	scripting_Pop(); // level

	computeBoundingBox(l);

	return l;
}
