#ifndef GAME_AI_H
#define GAME_AI_H

#include "game/game.h"
#include "base/nebu_vector.h"

typedef struct AI_Configuration {
	int location;
	segment2 player;
	segment2 opponent;
	vec2 intersection;
	float t_player, t_opponent;
} AI_Configuration;
	
typedef struct AI_Distances {
	float front, left, right, backleft;
} AI_Distances;

#define MAX_AI_LEVEL 3

typedef struct AI_Parameters {
	unsigned int minTurnTime[MAX_AI_LEVEL + 1];
	float maxSegLength[MAX_AI_LEVEL + 1];
	float critical[MAX_AI_LEVEL + 1];
	int  spiral[MAX_AI_LEVEL + 1];
	int rlDelta[MAX_AI_LEVEL + 1];
} AI_Parameters;

extern AI_Parameters ai_params;

void ai_getDistances(int player, AI_Distances *distances);
void ai_getClosestOpponent(int player, int* opponent, float *distance);
void ai_getConfig(int player, int opponent, AI_Configuration *config);
void ai_aggressive(int player, int opponent, int location, 
									 AI_Distances *distances, AI_Parameters *pAIParameters);
void ai_evasive(int player, int opponent, int location,
								AI_Distances *distances, AI_Parameters *pAIParameters);

#endif
