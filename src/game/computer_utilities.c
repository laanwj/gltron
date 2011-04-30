#include "base/nebu_math.h"
#include "game/game.h"
#include "game/game_data.h"
#include "game/ai.h"
#include "configuration/settings.h"
#include "game/game_level.h"
#include "game/event.h"

void ai_getClosestOpponent(int player, int* opponent, float *distance) {
	int i;
	vec2 v_player;
	vec2 v_opponent;

	*opponent = -1;
	*distance = FLT_MAX;

	getPositionFromIndex(v_player.v + 0, v_player.v + 1, player);
	
	for(i = 0; i < game->players; i++) {
		if(i == player)
			continue;
		if(game->player[i].data.speed > 0) {
			vec2 diff;
			float d;

			getPositionFromIndex(v_opponent.v + 0, v_opponent.v + 1, i);
			vec2_Sub(&diff, &v_player, &v_opponent);
			// use manhattan distance instead of euclidean distance
			d = (float) ( fabs(diff.v[0]) + fabs(diff.v[1]) );
			// d = vec2Length(&diff);
			if(d < *distance) {
				*opponent = i;
				*distance = d;
			}
		}
	}
} 

void ai_getDistances(int player, AI_Distances *distances) {
	enum { eFront = 0, eLeft, eRight, eBackleft, eMax };
	segment2 segments[eMax];
	vec2 v, vPos;
	Data *pData = &game->player[player].data;
	int dirLeft = (pData->dir - 1 + game2->level->nAxis) % game2->level->nAxis;
	int dirRight = (pData->dir + 1) % game2->level->nAxis;
	int i, j;
	float *front = &distances->front;
	float *right = &distances->right;
	float *left = &distances->left;
	float *backleft = &distances->backleft;

	getPositionFromIndex(vPos.v + 0, vPos.v + 1, player);

	for(i = 0; i < eMax; i++) {
		vec2_Copy(&segments[i].vStart, &vPos);
	}

	segments[eFront].vDirection.v[0] = (float) game2->level->pAxis[pData->dir].v[0];
	segments[eFront].vDirection.v[1] = (float) game2->level->pAxis[pData->dir].v[1];
	segments[eLeft].vDirection.v[0] = (float) game2->level->pAxis[dirLeft].v[0];
	segments[eLeft].vDirection.v[1] = (float) game2->level->pAxis[dirLeft].v[1];
	segments[eRight].vDirection.v[0] = (float) game2->level->pAxis[dirRight].v[0];
	segments[eRight].vDirection.v[1] = (float) game2->level->pAxis[dirRight].v[1];
	segments[eBackleft].vDirection.v[0] = (float) game2->level->pAxis[dirLeft].v[0] - game2->level->pAxis[pData->dir].v[0];
	segments[eBackleft].vDirection.v[1] = (float) game2->level->pAxis[dirLeft].v[1] - game2->level->pAxis[pData->dir].v[1];
	vec2_Normalize(&segments[eBackleft].vDirection);
	*front = FLT_MAX;
	*left = FLT_MAX;
	*right = FLT_MAX;
	*backleft = FLT_MAX;

	// loop over all segment
	for(i = 0; i < game->players; i++) {
		segment2 *wall = game->player[i].data.trails;
		if(game->player[i].data.trail_height < TRAIL_HEIGHT)
			continue;

		for(j = 0; j < game->player[i].data.nTrails; j++) {
			float t1, t2;
			if(i == player && j == game->player[i].data.nTrails - 1)
				break;
			if(segment2_Intersect(&v, &t1, &t2, segments + eFront, wall) &&
				 t1 > 0 && t1 < *front && t2 >= 0 && t2 <= 1)
				*front = t1;
			if(segment2_Intersect(&v, &t1, &t2, segments + eLeft, wall) &&
				 t1 > 0 && t1 < *left && t2 >= 0 && t2 <= 1)
				*left = t1;
			if(segment2_Intersect(&v, &t1, &t2, segments + eRight, wall) &&
				 t1 > 0 && t1 < *right && t2 >= 0 && t2 <= 1)
				*right = t1;
			if(segment2_Intersect(&v, &t1, &t2, segments + eBackleft, wall) &&
				 t1 > 0 && t1 < *backleft && t2 >= 0 && t2 <= 1)
				*backleft = t1;
			wall++;
		}
	}
	for(i = 0; i < game2->level->nBoundaries; i++) {
		float t1, t2;
		segment2* wall = game2->level->boundaries + i;
		if(segment2_Intersect(&v, &t1, &t2, segments + eFront, wall) &&
			 t1 > 0 && t1 < *front && t2 >= 0 && t2 <= 1)
			*front = t1;
		if(segment2_Intersect(&v, &t1, &t2, segments + eLeft, wall) &&
			 t1 > 0 && t1 < *left && t2 >= 0 && t2 <= 1)
			*left = t1;
		if(segment2_Intersect(&v, &t1, &t2, segments + eRight, wall) &&
			 t1 > 0 && t1 < *right && t2 >= 0 && t2 <= 1)
			*right = t1;
		if(segment2_Intersect(&v, &t1, &t2, segments + eBackleft, wall) &&
			 t1 > 0 && t1 < *backleft && t2 >= 0 && t2 <= 1)
			*backleft = t1;
	}
	
	// update debug render segments
	{
		AI *ai = &game->player[player].ai;
		vec2_Copy(&ai->front.vStart, &vPos);
		vec2_Copy(&ai->left.vStart, &vPos);
		vec2_Copy(&ai->right.vStart, &vPos);
		vec2_Copy(&ai->backleft.vStart, &vPos);
		
		ai->front.vDirection.v[0] = *front * game2->level->pAxis[pData->dir].v[0];
		ai->front.vDirection.v[1] = *front * game2->level->pAxis[pData->dir].v[1];
		ai->left.vDirection.v[0] = *left * game2->level->pAxis[dirLeft].v[0];
		ai->left.vDirection.v[1] = *left * game2->level->pAxis[dirLeft].v[1];
		ai->right.vDirection.v[0] = *right * game2->level->pAxis[dirRight].v[0];
		ai->right.vDirection.v[1] = *right * game2->level->pAxis[dirRight].v[1];
		ai->backleft.vDirection.v[0] = (float) (game2->level->pAxis[dirLeft].v[0] - game2->level->pAxis[pData->dir].v[0]);
		ai->backleft.vDirection.v[1] = (float) (game2->level->pAxis[dirLeft].v[1] - game2->level->pAxis[pData->dir].v[1]);
		vec2_Normalize(&ai->backleft.vDirection);
		vec2_Scale(&ai->backleft.vDirection,*backleft);
	}
		
	// printf("%.2f, %.2f, %.2f\n", *front, *right, *left);
	return;
}

void ai_getConfig(int player, int target,
										AI_Configuration *config) {

	Data *pData;
	
	getPositionFromIndex(config->player.vStart.v + 0,
											 config->player.vStart.v + 1,
											 player);
	getPositionFromIndex(config->opponent.vStart.v + 0,
											 config->opponent.vStart.v + 1,
											 target);
	
	pData = &game->player[player].data;
	config->player.vDirection.v[0] = game2->level->pAxis[pData->dir].v[0] * pData->speed;
	config->player.vDirection.v[1] = game2->level->pAxis[pData->dir].v[1] * pData->speed;

	pData = &game->player[target].data;
	config->opponent.vDirection.v[0] = game2->level->pAxis[pData->dir].v[0] * pData->speed;
	config->opponent.vDirection.v[1] = game2->level->pAxis[pData->dir].v[1] * pData->speed;
	
	// compute sector
	{
		vec2 diff;
		vec3 v1, v2, v3;
		vec3 up = { { 0, 0, 1 } };
		float cosphi;
		float phi;
		int i;

		vec2_Sub(&diff, &config->player.vStart, &config->opponent.vStart);
		v1.v[0] = diff.v[0];
		v1.v[1] = diff.v[1];
		v1.v[2] = 0;

		v2.v[0] = config->opponent.vDirection.v[0];
		v2.v[1] = config->opponent.vDirection.v[1];
		v2.v[2] = 0;

		vec3_Normalize(&v1, &v1);
		vec3_Normalize(&v2, &v2);

		vec3_Cross(&v3, &v1, &v2);
		vec3_Normalize(&v3, &v3);
	
		cosphi = vec3_Dot(&v1, &v2);
		nebu_Clamp(&cosphi, -1, 1);
		phi = (float) acos(cosphi);
		if(vec3_Dot(&v3, &up) > 0)
			phi = 2 * (float) M_PI - phi;
	
		for(i = 0; i < 8; i++) {
			phi -= (float) M_PI / 4;
			if(phi < 0) {
				config->location = i;
				break;
			}
		}
	}
	// compute intersection
	{
		segment2 seg1;
		segment2 seg2;
		seg1.vStart = config->opponent.vStart;
		seg1.vDirection = config->opponent.vDirection;
		seg2.vStart = config->player.vStart;
		vec2_Orthogonal( &seg2.vDirection, &config->opponent.vDirection );
		vec2_Normalize( &seg2.vDirection );
		vec2_Scale( &seg2.vDirection, vec2_Length( &config->player.vDirection ) );
							 
		segment2_Intersect( &config->intersection, 
												&config->t_opponent, &config->t_player,
												&seg1, &seg2 );
		if(config->t_player < 0)
			config->t_player *= -1;
	}
	
}

void ai_left(int player, AI_Distances *distances, AI_Parameters *pAIParameters) {
	// printf("trying left turn...");
	AI *ai = &game->player[player].ai;
	Data *pData = &game->player[player].data;
	float save_distance;

	int level = gSettingsCache.ai_level;
	if(level > MAX_AI_LEVEL)
		level = MAX_AI_LEVEL;
	
	save_distance = (pAIParameters->minTurnTime[level] * pData->speed / 1000.0f) + 20;
	
	if(distances->left > save_distance) {
		createEvent(player, EVENT_TURN_LEFT);
		ai->tdiff++;
		ai->lasttime = game2->time.current;
		// printf("succeeded\n");
	} else {
		// printf("failed\n");
	}
}

void ai_right(int player, AI_Distances *distances, AI_Parameters *pAIParameters) {
	// printf("trying right turn...");
	AI *ai = &game->player[player].ai;
	Data *pData = &game->player[player].data;
	float save_distance;

	int level = gSettingsCache.ai_level;
	if(level > MAX_AI_LEVEL)
		level = MAX_AI_LEVEL;

	save_distance = (pAIParameters->minTurnTime[level] * pData->speed / 1000.0f) + 20;

	if(distances->right > save_distance) {
		createEvent(player, EVENT_TURN_RIGHT);
		ai->tdiff--;
		ai->lasttime = game2->time.current;
		// printf("succeeded\n");
	} else {
		// printf("failed\n");
	}
}

static int agressive_action[8][4] = {
	{ 2, 0, 2, 2 },
	{ 0, 1, 1, 2 },
	{ 0, 1, 1, 2 },
	{ 0, 1, 1, 2 },
	{ 0, 2, 2, 1 },
	{ 0, 2, 2, 1 },
	{ 0, 2, 2, 1 },
	{ 1, 1, 1, 0 }
};

int evasive_action[8][4] = {
	{ 1, 1, 2, 2 },
	{ 1, 1, 2, 0 },
	{ 1, 1, 2, 0 },
	{ 1, 1, 2, 0 },
	{ 2, 0, 1, 1 },
	{ 2, 0, 1, 1 },
	{ 2, 0, 1, 1 },
	{ 2, 2, 1, 1 }
};


void ai_action(int action, int player, AI_Distances *distances, AI_Parameters *pAIParameters) {
	switch(action) {
	case 0: break;
	case 1: ai_left(player, distances, pAIParameters); break;
	case 2: ai_right(player, distances, pAIParameters); break;
	}
}
		
void ai_aggressive(int player, int target, int location,
									 AI_Distances *distances, AI_Parameters *pAIParameters) {
	int dirdiff = (game2->level->nAxis + 
		game->player[player].data.dir -
		game->player[target].data.dir) % game2->level->nAxis;
	
	// printf("aggressive mode (%d, %d)\n", player, target, location, dirdiff);
	if(dirdiff < 4)
		ai_action(agressive_action[location][dirdiff], player, distances, pAIParameters);
}

void ai_evasive(int player, int target, int location, 
								AI_Distances *distances, AI_Parameters *pAIParameters) {
	int dirdiff = (game2->level->nAxis + 
		game->player[player].data.dir -
		game->player[target].data.dir) % game2->level->nAxis;
	// printf("evasive mode (%d,%d,%d)\n", player, target, location);
	if(dirdiff < 4)
		ai_action(evasive_action[location][dirdiff], player, distances, pAIParameters);
}

