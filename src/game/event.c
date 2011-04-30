#include "base/nebu_math.h"

#include "video/video.h"
#include "video/recognizer.h"
#include "video/explosion.h"
#include "game/game.h"
#include "game/game_level.h"
#include "scripting/scripting.h"
#include "audio/audio.h"
#include "Nebu_scripting.h"
#include "Nebu_base.h"
#include "Nebu_input.h"
#include "video/nebu_console.h"
#include "configuration/settings.h"
#include "game/event.h"
#include "game/camera.h"
#include "game/engine.h"

#include "base/nebu_debug_memory.h"

/*! \fn int processEvent(GameEvent* e)
  handle events, e.g.
  left/right/crash/stop

  frees e
*/

int processEvent(GameEvent* e) {
	int value = 0;

	switch(e->type) {
	case EVENT_TURN_LEFT:
		doTurn(e, TURN_LEFT);
		break;
	case EVENT_TURN_RIGHT:
		doTurn(e, TURN_RIGHT);
		break;
	case EVENT_CRASH: 
		displayMessage(TO_CONSOLE, "player %d crashed", e->player + 1);
		doCrashPlayer(e);
		break;
	case EVENT_STOP:
		// displayMessage(TO_STDOUT, "game stopped");
		if(e->player < game->players && game->player[e->player].ai.active != AI_NONE) {
			game->winner = e->player;
			displayMessage(TO_CONSOLE, "winner: %d", game->winner + 1);
		} else {
			game->winner = -2;
			displayMessage(TO_CONSOLE, "everyone died! no one wins!");
		}
		nebu_System_ExitLoop(eSRC_Game_End);
		game->pauseflag = PAUSE_GAME_FINISHED;
		value = 1;
		break;
	}
	free(e);
	return value;
}

int crashTestPlayers(int i, const segment2 *movement) {
	int j, k;
	int crash = 0;
	Data *data = &game->player[i].data;

	for(j = 0; j < game->players; j++) {
		int crash = 0;

		if(game->player[j].data.trail_height < TRAIL_HEIGHT)
			continue;

		for(k = 0; k < game->player[j].data.nTrails; k++) {
			segment2 *wall;
			vec2 v;
			float t1, t2;
				
			// don't test against your own current segment, or your last segment before that
			if(j == i && k >= game->player[j].data.nTrails - 2)
				break;

			// segment to test against
			wall = game->player[j].data.trails + k;
						
			if(segment2_Intersect(&v, &t1, &t2, movement, wall)) {
#if 0
				printf("(%.2f, %.2f), (%.2f, %.2f), %.2f, %.2f\n",
							 data->posx, data->posy,
							 v.v[0], v.v[1],
							 t1, t2); 
#endif
				if(t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
					// set current trail end to crash location
					data->trails[data->nTrails - 1].vDirection.v[0] = v.v[0] - data->trails[data->nTrails - 1].vStart.v[0];
					data->trails[data->nTrails - 1].vDirection.v[1] = v.v[1] - data->trails[data->nTrails - 1].vStart.v[1];
					createEvent(i, EVENT_CRASH);
					crash = 1;
					break;
				}
			}
		}
		if(crash)
			break;
	}
	return crash;
}

int crashTestWalls(int i, const segment2 *movement) {
	int j;
	vec2 v;
	float t1, t2;
	int crash = 0;

	Data *data = &game->player[i].data;
	segment2 *current = data->trails + data->nTrails ;
	
	for(j = 0; j < game2->level->nBoundaries; j++) {
		if(segment2_Intersect(&v, &t1, &t2, movement, game2->level->boundaries + j)) {
		// if(segment2_Intersect(&v, &t1, &t2, current, game2->level->boundaries + j)) {
			if(t1 >= 0 && t1 < 1 && t2 >= 0 && t2 < 1) {
				current->vDirection.v[0] = v.v[0] - current->vStart.v[0];
				current->vDirection.v[1] = v.v[1] - current->vStart.v[1];
				createEvent(i, EVENT_CRASH);
				crash = 1;
				break;
			}
		}
	}
	return crash;
}

int regenerate(int player, int dt)
{
	float curEnergy = game->player[player].data.energy + getSettingf("energy_increase") * dt / 1000.0f;
	float maxEnergy = getSettingf("energy");
	if(curEnergy > maxEnergy) curEnergy = maxEnergy;
	game->player[player].data.energy = curEnergy;
	return 0;
}

int applyBooster(int player, int dt) {
	float boost;
	
	Data *data = &game->player[player].data;
	if(!data->boost_enabled)
		return 0;

	boost = getSettingf("booster_use") * dt / 1000.0f;
	if(boost > data->energy) {
		boost = data->energy;
		data->boost_enabled = 0;
	}
	data->speed += boost * getSettingf("booster_speed_increase");
	// TODO: this is somewhat framerate dependent:
	// + data->speed * booster_speed_factor * dt
	data->energy -= boost;
	return 1;
}

int applyWallBuster(int player, int dt) {
	float consumption;
	
	Data *data = &game->player[player].data;
	if(!data->wall_buster_enabled) {
		return 0;
	}

	consumption = getSettingf("wall_buster_use") * dt / 1000.0f;
	if(consumption > data->energy)
	{
		consumption = 0;
		data->wall_buster_enabled = 0;
		return 0;
	}
	data->energy -= consumption;
	return 1;
}

void applyDecceleration(int player, int dt, float factor) {
	Data *data = &game->player[player].data;
	if(data->speed > game2->rules.speed) {
		float decrease = factor * dt / 1000.0f;
		data->speed -= decrease;
		if(data->speed < game2->rules.speed)
			data->speed = game2->rules.speed;
	}
}

int applyWallAcceleration(int player, int dt) {
	// find distance to enemy walls left & right
	enum { eLeft, eRight, eMax };
	segment2 segments[eMax];

	Data *pData = &game->player[player].data;
	int dirLeft = (pData->dir - 1 + game2->level->nAxis) % game2->level->nAxis;
	int dirRight = (pData->dir + 1) % game2->level->nAxis;

	float left, right;

	float x, y;
	vec2 vPos;

	int i, j;

	getPositionFromIndex(&x, &y, player);
	vPos.v[0] = x;
	vPos.v[1] = y;

	for(i = 0; i < eMax; i++) {
		vec2_Copy(&segments[i].vStart, &vPos);
	}

	// TODO: this might not work corretly for wall acceleration
	segments[eLeft].vDirection.v[0] = (float) game2->level->pAxis[dirLeft].v[0];
	segments[eLeft].vDirection.v[1] = (float) game2->level->pAxis[dirLeft].v[1];
	segments[eRight].vDirection.v[0] = (float) game2->level->pAxis[dirRight].v[0];
	segments[eRight].vDirection.v[1] = (float) game2->level->pAxis[dirRight].v[1];

	left = FLT_MAX;
	right = FLT_MAX;

	for(i = 0; i < game->players; i++) {
		segment2 *wall = game->player[i].data.trails;

		// can't ride your own walls
		if(i == player)
			continue;
		// can't ride vanishing walls
		if(game->player[i].data.trail_height < TRAIL_HEIGHT)
			continue;
		
		// find the closest wall left or right of the current player location
		for(j = 0; j < game->player[i].data.nTrails; j++) {
			float t1, t2;
			vec2 v;
			if(segment2_Intersect(&v, &t1, &t2, segments + eLeft, wall) &&
				 t1 > 0 && t1 < left && t2 >= 0 && t2 <= 1)
				left = t1;
			if(segment2_Intersect(&v, &t1, &t2, segments + eRight, wall) &&
				 t1 > 0 && t1 < right && t2 >= 0 && t2 <= 1)
				right = t1;
			wall++;
		}
	}

	{
		float accell_max = getSettingf("wall_accel_max");
		if(left < accell_max || right < accell_max) {
			float boost = getSettingf("wall_accel_use") * dt / 1000.0f;
			pData->speed += boost;
			return 1;
		} else {
			return 0;
		}
	}
}

/*! \fn void doMovement(int dt)
	\param dt	time for this tick

	do physics, create CRASH and STOP events
*/

void doMovement(int dt)
{
	int i;

	for(i = 0; i < game->players; i++)
	{ // foreach player
		Data *pData = &game->player[i].data;
		if(pData->speed > 0)
		{ // still alive
			float fs;
			float t;

			float deccel = 0;
			if(getSettingf("wall_accel_on") == 1)
			{ // wall acceleration
				if(!applyWallAcceleration(i, dt))
				{
					deccel = getSettingf("wall_accel_decrease");
				}
				else
				{
					deccel = -1; // forbid deacceleration for booster
				}
			} // wall acceleration

			// recharge energy
			if(getSettingi("wall_buster_on") || getSettingi("booster_on"))
				regenerate(i, dt);

			if(getSettingf("wall_buster_on") == 1)
			{ // wall buster
				// printf("applying wallbuster for player %i\n");
				applyWallBuster(i, dt);
			} // wall buster

			if(getSettingf("booster_on") == 1)
			{ // booster
				if(!applyBooster(i, dt) && deccel != -1)
				{
					float d = getSettingf("booster_speed_decrease");
					deccel = d > deccel ? d : deccel;
				}
				else
				{
					deccel = -1;
				}
			} // booster

			if(deccel > 0)
				applyDecceleration(i, dt, deccel);

			// if(i == 0)
			// printf("speed: %.2f, boost: %.2f\n", pData->speed, pData->energy);

			fs = 1.0f - SPEED_OZ_FACTOR + SPEED_OZ_FACTOR * 
				cosf(i * PI / 4.0f + 
				(game2->time.current % SPEED_OZ_FREQ) * 
				2.0f * PI / SPEED_OZ_FREQ);

			t = dt / 100.0f * pData->speed * fs;

			{	// movement
				segment2 *current = pData->trails + pData->nTrails - 1;
				segment2 movement;
				int crash = 0;
				float x, y;

				getPositionFromData(&x, &y, pData);
				movement.vStart.v[0] = x;
				movement.vStart.v[1] = y;
				movement.vDirection.v[0] = t * game2->level->pAxis[pData->dir].v[0];
				movement.vDirection.v[1] = t * game2->level->pAxis[pData->dir].v[1];

				current->vDirection.v[0] += t * game2->level->pAxis[pData->dir].v[0];
				current->vDirection.v[1] += t * game2->level->pAxis[pData->dir].v[1];

				if(!pData->wall_buster_enabled)
				{ // collision detection against players
					crash = crashTestPlayers(i, &movement);
					if(crash)
					{
						printf("player %d crashed into other players \n", i);
						printf("%f %f %f %f\n",
							movement.vStart.v[0],
							movement.vStart.v[1],
							movement.vDirection.v[0],
							movement.vDirection.v[1]);
					} // crash debug output
				} // collision detection against players
				if(!crash)
				{ // collision detection against walls
					crash = crashTestWalls(i, &movement);
					if(crash)
					{
						printf("player %d crashed into the walls\n", i);
						printf("%f %f %f %f\n",
							movement.vStart.v[0],
							movement.vStart.v[1],
							movement.vDirection.v[0],
							movement.vDirection.v[1]);
					} // crash debut output
				} // collision detection against walls
			} // movement
		} // still alive
		else
		{ // already crashed
			if(pData->impact_radius < IMPACT_MAX_RADIUS)
				pData->impact_radius += game2->time.dt * IMPACT_RADIUS_DELTA;

			if(game2->rules.eraseCrashed == 1 && pData->trail_height > 0)
				pData->trail_height -= (dt * TRAIL_HEIGHT) / 1000.0f;
			if(pData->exp_radius < EXP_RADIUS_MAX)
				pData->exp_radius += dt * EXP_RADIUS_DELTA;
			else if (pData->speed == SPEED_CRASHED)
			{ // done exploding
				int winner = -1;

				pData->speed = SPEED_GONE;
				game->running--;
				if(game->running <= 1)
				{ // all dead
					/* find survivor, create winner event */
					int i;
					float maxSpeed = SPEED_GONE;
					for(i = 0; i < game->players; i++)
					{
						if(game->player[i].data.speed >= maxSpeed)
						{
							winner = i;
							maxSpeed = game->player[i].data.speed;
						}
					}
					createEvent(winner, EVENT_STOP);
					/* a stop event is the last event that happens */
					return;
				} // all dead
			} // done exploding
		} // already crashed
	} // foreach player
}

int game_ComputeTimeDelta(void)
{
	/* check for fast finish */
	if (gSettingsCache.fast_finish != 1)
	{
		// time since last idle processing
		return game2->time.dt;
	}
	else
	{
		// compute an acceleration factor (speedup time by factor 4/6/12/25)
		// depending on the size of the arena
		int factors[4] = { 4, 6, 12, 25 };
		int threshold[4] = { 0, 300, 600, 800 };
		int factor = 1;
		int i;

		for(i = 0; i < 4; i++)
		{
			if(box2_Diameter(&game2->level->boundingBox) > threshold[i])
				factor = factors[i];
		}
		for (i = 0; i < game->players; i++)
		{
			if (game->player[i].ai.active != AI_COMPUTER &&
				game->player[i].data.exp_radius < EXP_RADIUS_MAX)
			{
				factor = 1;
			}
		}
		return game2->time.dt * factor;
	} 
}
void game_RunAI(int dt)
{
	int i;

	/* run AI */
	for(i = 0; i < game->players; i++)
	{
		if(game->player[i].ai.active == AI_COMPUTER &&
			PLAYER_IS_ACTIVE(&game->player[i]))
		{
			doComputer(i, 0);
			// schedules events (e.g. turns)
		}
	}
}

/*! \fn void Game_Idle( void )
  game loop:
  run ai, process events, do physics, process events again,
  do camera movement
*/

void Game_Idle(void) {
	// time since last frame
	int dt = game_ComputeTimeDelta();
	
	while(dt > 0) {
		nebu_List *p;
		int t; // time for a single tick to be processed

		// chop time since last frame into ticks of maximally PHYSICS_RATE milliseconds
		if(dt > PHYSICS_RATE) t = PHYSICS_RATE;
		else t = dt;

		/*
		game_ComputeMovement(dt); // compute new speed, energy levels, movement vectors
		game_HandleMovement(dt); // handle movement based events
		game_AdjustMovement(dt); // adjust movement vectors due to collision response
		game_AdjustWorld(dt); // write back changes to world
		*/
		game_RunAI(dt);
		/*
		game_ProcesssInput(dt);
		*/

		/* process any outstanding events (turns, etc) */
		for(p = &(game2->events); p->next != NULL; p = p->next) {
			if(processEvent((GameEvent*) p->data))
				// a STOP event was encountered, exiting
				// TODO: fix, because this leaks the list element, and also all unprocessed turn events!
				return;
		}

		/* free list items (processed events are already freed in processEvents */
		p = game2->events.next;
		while(p != NULL) {
			nebu_List *l = p;
			p = p->next;
			free(l);
		}
		game2->events.next = NULL;

		doMovement(t); /* this can generate new events */

		dt -= t;
	}

	doCameraMovement();
	{
		// why I can't just warp the mouse to the orig is still not 100% clear to me
		// oh well, it's not a big loss.
		int mouse_dx, mouse_dy;
		nebu_Input_Mouse_GetDelta(&mouse_dx, &mouse_dy);
		if(mouse_dx != 0 || mouse_dy != 0)
			nebu_Input_Mouse_WarpToOrigin();
	}
	doRecognizerMovement();
}

/*! \fn void createEvent(int player, event_type_e eventType)
  helper function to create an event struct and insert it into the
  global event queue
*/

void createEvent(int player, event_type_e eventType) {
	GameEvent *e = (GameEvent*) malloc(sizeof(GameEvent));
	// store event information
	e->type = eventType;
	getPositionFromIndex(&e->x, &e->y, player);
	e->player = player;
	e->timestamp = game2->time.current;
	// TODO: This sometimes creates a (trivial) memory leak
	nebu_List_AddTail2(&(game2->events), e);
}
