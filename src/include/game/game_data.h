#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <stdio.h>

#include "base/nebu_vector.h"
#include "base/nebu_util.h"
#include "base/nebu_system.h"
#include "base/nebu_types.h"

// #include "game/camera.h"
#include "game/game_level.h"

#define PLAYER_IS_ACTIVE(x) ((x)->data.speed > 0)

typedef enum eGameType {
	GAME_SINGLE = 1
#ifdef RECORD
	,
	GAME_SINGLE_RECORD = 2,
	GAME_PLAY = 4,
	GAME_PLAY_NETWORK = 8,
	GAME_NETWORK_RECORD
#endif
} eGameType;

/* 
   this struct contains all the necessary parameters to define a game round
   (except number of players)
   any change forces a restart of the round 
*/

typedef struct RuleSet {
	int eraseCrashed;
	float speed;
} RuleSet;

typedef struct Game2 {
	game_level *level;
	RuleSet rules;
	SystemTime time;
	nebu_List events;
	FILE *record;
	FILE *play;
	// Input input;
} Game2;

typedef struct Data {
	int dir;

	int score;
	float speed; /* set to -1 when dead */
	int boost_enabled;
	// float booster;
	// float wall_buster;
	float energy;
	int wall_buster_enabled;
	float trail_height;

	int last_dir;
	/* for cycle animation */
	unsigned int turn_time; 
	/* explosion  & impact stuff */
	float impact_radius;
	float exp_radius;

	segment2 *trails; // light cycle trail segments
	int nTrails; // segments currently in use
	int maxTrails; // segments where memory is allocated for
	// maxTrails is set to INITIAL_TRAIL_COUNT on creation
} Data;

typedef struct AI {
	int active;
	int tdiff;
	long lasttime;
	segment2 left, right, front, backleft;
} AI;

typedef struct Player_Profile {
	float pColorDiffuse[4];
	float pColorSpecular[4];
	float pColorAlpha[4];
} Player_Profile;

typedef struct Player {
	Data data;
	AI ai;
	Player_Profile profile;
} Player;

typedef struct Game {
	Player *player;
	int players; //!< number of players - currently limited to 4 somewhere
	int winner; //!< who won this round
	int pauseflag; 
	//!< if the game is finished: the PAUSE_GAME_FINISHED flag is set
	int running; //!< the number of players that are still alive
} Game;

#endif
