#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "base/nebu_vector.h"

enum {
	eGameSpawnUndef,
	eGameSpawnPoint,
	eGameSpawnLine
};

typedef struct game_spawnpoint {
	int type;
	vec2 vStart;
	vec2 vEnd; // unused for eGameSpawnLine
	int n;
	int dir;
} game_spawnpoint;

typedef struct game_spawnset {
	int type;
	int nPoints;
	game_spawnpoint *pSpawnPoints;
} game_spawnset;

typedef struct game_level {
	int nBoundaries;
	segment2 *boundaries;
	int nAxis;
	vec2 *pAxis;
	int nSpawnSets;
	int spawnIsRelative;
	game_spawnset **ppSpawnSets;
	box2 boundingBox;
	float scale_factor;
} game_level;

void game_FreeLevel(game_level *l);
game_level* game_CreateLevel();
void game_ScaleLevel(game_level *l, float fSize);
void game_UnloadLevel(void);
int game_LoadLevel(void);

enum {
	GAME_SUCCESS = 0,
	GAME_ERROR_LEVEL_ALREADYLOADED = 1,
	GAME_ERROR_LEVEL_NOTLOADED,
};

#endif
