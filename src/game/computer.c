#include "game/game.h"
#include "game/ai.h"
#include "video/video.h"
#include "configuration/settings.h"
#include "game/engine.h"
#include "game/event.h"

#include "base/nebu_math.h"

#define OPP_MAX_DIST (0.2f * criticalDistance())
#define SAVE_T_DIFF 0.500f
#define SAVE_SPEED_DIFF 1.0f
#define HOPELESS_T 0.80f

AI_Parameters ai_params = {
	// minTurnTime, time to pass between turns, in milliseconds
	{ 600, 400, 200, 100 },
	// maxSegLength 
	{ 0.6f, 0.3f, 0.3f, 0.3f },
	// critical
	{ 0.2f, 0.08037f, 0.08037f, 0.08037f },
	// spiral, turns until a spiral is detected
	{ 10, 10, 10, 10 },
	// rlDelta
	{	0, 10, 20, 30 }
};

float criticalDistance(void) {
	return 84.0f * game2->rules.speed;
}

// play as if nothing concerned us
void doComputerSimple(int player, int target, AI_Distances *distances) {
  AI *ai;
  Data *data;
  Player *me, *him;
  int level;

  me = &(game->player[ player ]);
  him = &(game->player[ target ]);
  data = &me->data;
  ai = &me->ai;
  
  level = gSettingsCache.ai_level;
  if(level > MAX_AI_LEVEL)
	level = MAX_AI_LEVEL;

	// turn off booster
	data->boost_enabled = 0;

  /* avoid too short turns */
  if(game2->time.current - ai->lasttime < ai_params.minTurnTime[level])
    return;

	// ai_getDistances(player, &front, &left, &right, &backleft);

	/* first, check if we are in danger, or if we should consider
		 turning once in a while
	 */

  if(distances->front > ai_params.critical[level] * criticalDistance() &&
		 segment2_Length(data->trails + data->nTrails - 1) <
		 ai_params.maxSegLength[level] * criticalDistance())
		return;
		
	// printf("%.2f, %.2f, %.2f\n", distances->front, 
	// distances->left, distances->right);
	/* decide where to turn */
	if(distances->front > distances->right && 
		 distances->front > distances->left) { 
		return; /* no way out */
	} else if(distances->left > ai_params.rlDelta[level] &&
						fabs(distances->right - distances->left) < 
						ai_params.rlDelta[level] &&
						distances->backleft > distances->left &&
						ai->tdiff < ai_params.spiral[level]) {
		createEvent(player, EVENT_TURN_LEFT);
		ai->tdiff++;
	} else if(distances->right > distances->left && 
						ai->tdiff > - ai_params.spiral[level] ) {
		createEvent(player, EVENT_TURN_RIGHT);
		ai->tdiff--;
		// printf("turn right\n");
	} else if(distances->right < distances->left && 
						ai->tdiff < ai_params.spiral[level] ) {
		createEvent(player, EVENT_TURN_LEFT);
		ai->tdiff++;
		// printf("turn left\n");
	} else {
		printf("tdiff decision\n");
		if(ai->tdiff > 0) { 
			createEvent(player, EVENT_TURN_RIGHT);
			ai->tdiff--; 
			// printf("turn right\n");
		} else { 
			createEvent(player, EVENT_TURN_LEFT);
			ai->tdiff++; 
			// printf("turn left\n");
		}
	}
	ai->lasttime = game2->time.current;
}

// play against the other player, either cut him off, or take evasive action
void doComputerActive(int player, int target, AI_Distances *distances) {
	Data *data = &game->player[player].data;
	AI *ai = &game->player[player].ai;

	AI_Configuration config;

  if(game2->time.current - ai->lasttime < 200)
    return;

	ai_getConfig(player, target, &config);
	
	// who's going to reach the intersection point first?
	// printf("active, location %d (direction: %d)\n", 
	// config.location, data->dir);
	
	switch(config.location) {
	case 0: case 7: 
	case 1: case 6:
		if(config.t_player < config.t_opponent) {
			// printf("I'm ahead: %.2f, %.2f\n", config.t_player, config.t_opponent);
			if(config.t_player - config.t_opponent < SAVE_T_DIFF)
				data->boost_enabled = 1;
			if(game->player[player].data.speed -
				 game->player[target].data.speed < SAVE_SPEED_DIFF)
				data->boost_enabled = 1;
			// aggressive
			ai_aggressive(player, target, config.location, distances, &ai_params);
		} else {
			// printf("I'm behind: %.2f, %.2f\n", 
			// config.t_player, config.t_opponent);
			// check speed, adjust or evasive
			if(config.t_opponent < HOPELESS_T) {
				// evasive
				ai_evasive(player, target, config.location, distances, &ai_params);
			}	else if(config.t_opponent - config.t_player < SAVE_T_DIFF)	{
				data->boost_enabled = 1;
				// aggressive
				ai_aggressive(player, target, config.location, distances, &ai_params);
			}	else {
				// evasive
				ai_evasive(player, target, config.location, distances, &ai_params);
			}
		}
		break;
	case 2:	case 4:
	case 3:	case 5:
		doComputerSimple(player, target, distances);
		break;
	}
}

void doComputer(int player, int target) {
	float d;
	int opponent;
	AI_Distances distances;

	ai_getDistances(player, &distances);
	ai_getClosestOpponent(player, &opponent, &d);
	if(opponent == -1 || d > OPP_MAX_DIST || distances.front < d ||
		 game->player[opponent].ai.active == 1 ||
		 gSettingsCache.ai_level == 0 /* dumb setting */) { 
		// only fight humans
		// printf("inactive, closest opponent: %d, distance %.2f, front %.2f\n", 
		//	 opponent, d, distances.front);
		doComputerSimple(player, target, &distances);
	} else {
		doComputerActive(player, opponent, &distances);
	}
}
