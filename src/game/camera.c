#include "video/recognizer.h"
#include "video/video.h"
#include "game/camera.h"
#include "game/game.h"
#include "input/input.h"
#include "configuration/settings.h"

#include "base/nebu_math.h"
#include "input/nebu_input_system.h"
#include "video/nebu_renderer_gl.h"
#include "video/nebu_console.h"
#include "base/nebu_assert.h"

#include <string.h>

typedef enum eCamFreedom { 
	CAM_FREE_R = 0,
	CAM_FREE_PHI,
	CAM_FREE_CHI 
} eCamFreedom;

float cam_defaults[][3] =  { 
  { CAM_CIRCLE_DIST, PI / 3, 0 }, /* circle */
  { CAM_FOLLOW_DIST, PI / 4, PI / 72 }, /* follow */
  { CAM_COCKPIT_Z, PI / 8, 0 }, /* cockpit */
  { CAM_CIRCLE_DIST, PI / 3, 0 } /* free */
};


static void writeCamDefaults(Camera *cam, int type) {
	cam_defaults[cam->type.type][type] = cam->movement[type];
}

static void clampCam(Camera *cam) {
	if(cam->type.freedom[CAM_FREE_R]) {
		float clamp_cam_r_min = getSettingf("clamp_cam_r_min");
		float clamp_cam_r_max = getSettingf("clamp_cam_r_max");
		if(cam->movement[CAM_R] < clamp_cam_r_min)
			cam->movement[CAM_R] = clamp_cam_r_min;
		if(cam->movement[CAM_R] > clamp_cam_r_max)
			cam->movement[CAM_R] = clamp_cam_r_max;
	}

	if(cam->type.freedom[CAM_FREE_CHI]) {
		float clamp_cam_chi_min = getSettingf("clamp_cam_r_min");
		float clamp_cam_chi_max = getSettingf("clamp_cam_r_max");
		if(cam->movement[CAM_CHI] < clamp_cam_chi_min)
			cam->movement[CAM_CHI] = clamp_cam_chi_min;
		if(cam->movement[CAM_CHI] > clamp_cam_chi_max)
			cam->movement[CAM_CHI] = clamp_cam_chi_max;
	}
}

static void initCircleCamera(Camera *cam) {
	cam->movement[CAM_R] = cam_defaults[CAM_CIRCLE][CAM_R];
	cam->movement[CAM_CHI] = cam_defaults[CAM_CIRCLE][CAM_CHI];
	cam->movement[CAM_PHI] = cam_defaults[CAM_CIRCLE][CAM_PHI];
	cam->movement[CAM_PHI_OFFSET] = 0;

	cam->type.interpolated_cam = 0;
	cam->type.interpolated_target = 0;
	cam->type.coupled = 0;
	cam->type.freedom[CAM_FREE_R] = 1;
	cam->type.freedom[CAM_FREE_PHI] = 0;
	cam->type.freedom[CAM_FREE_CHI] = 1;
}


static void initFollowCamera(Camera *cam) {
	cam->movement[CAM_R] = cam_defaults[CAM_FOLLOW][CAM_R];
	cam->movement[CAM_CHI] = cam_defaults[CAM_FOLLOW][CAM_CHI];
	cam->movement[CAM_PHI] = cam_defaults[CAM_FOLLOW][CAM_PHI];
	cam->movement[CAM_PHI_OFFSET] = 0;

	cam->type.interpolated_cam = 1;
	cam->type.interpolated_target = 0;
	cam->type.coupled = 1;
	cam->type.freedom[CAM_FREE_R] = 1;
	cam->type.freedom[CAM_FREE_PHI] = 1;
	cam->type.freedom[CAM_FREE_CHI] = 1;
}

static void initCockpitCamera(Camera *cam) {
	cam->movement[CAM_R] = cam_defaults[CAM_COCKPIT][CAM_R];
	cam->movement[CAM_CHI] = cam_defaults[CAM_COCKPIT][CAM_CHI];
	cam->movement[CAM_PHI] = PI; // cam_defaults ignored
	cam->movement[CAM_PHI_OFFSET] = 0;

	cam->type.interpolated_cam = 0;
	cam->type.interpolated_target = 1;
	cam->type.coupled = 1;
	cam->type.freedom[CAM_FREE_R] = 0;
	cam->type.freedom[CAM_FREE_PHI] = 1;
	cam->type.freedom[CAM_FREE_CHI] = 0;
}


static void initFreeCamera(Camera *cam) {
	cam->movement[CAM_R] = cam_defaults[CAM_FREE][CAM_R];
	cam->movement[CAM_CHI] = cam_defaults[CAM_FREE][CAM_CHI];
	cam->movement[CAM_PHI] = cam_defaults[CAM_FREE][CAM_PHI];
	cam->movement[CAM_PHI_OFFSET] = 0;

	cam->type.interpolated_cam = 0;
	cam->type.interpolated_target = 0;
	cam->type.coupled = 0;
	cam->type.freedom[CAM_FREE_R] = 1;
	cam->type.freedom[CAM_FREE_PHI] = 1;
	cam->type.freedom[CAM_FREE_CHI] = 1;
}

static void initOffsetCamera(Camera *cam) {
	cam->movement[CAM_R] = cam_defaults[CAM_FOLLOW][CAM_R];
	cam->movement[CAM_CHI] = cam_defaults[CAM_FOLLOW][CAM_CHI];
	cam->movement[CAM_PHI] = cam_defaults[CAM_FOLLOW][CAM_PHI];

	cam->type.interpolated_cam = 0;
	cam->type.interpolated_target = 0;
	cam->type.coupled = 0;
	cam->type.freedom[CAM_FREE_R] = 1;
	cam->type.freedom[CAM_FREE_PHI] = 1;
	cam->type.freedom[CAM_FREE_CHI] = 1;
}

void camera_ResetAll(void)
{
	int i;
	int camType;

	if(!game)
		return;

	for(i = 0; i < gnPlayerVisuals; i++) {
		Player *pPlayer = gppPlayerVisuals[i]->pPlayer;
		camType = (pPlayer->ai.active == AI_COMPUTER) ? 
			0 : getSettingi("camType");
		initCamera(gppPlayerVisuals[i], camType);
	}
}

void initCamera(PlayerVisual *pV, int type) {
	float x,y;
	Camera *cam = &pV->camera;

	getPositionFromData(&x, &y, &pV->pPlayer->data);

	cam->type.type = type;

	switch(cam->type.type) {
	case CAM_TYPE_CIRCLING: initCircleCamera(cam); break;
	case CAM_TYPE_FOLLOW: initFollowCamera(cam); break;
	case CAM_TYPE_COCKPIT: initCockpitCamera(cam); break;
	case CAM_TYPE_MOUSE: initFreeCamera(cam); break;
	case CAM_TYPE_OFFSET: initOffsetCamera(cam); break;
	}
	cam->target[0] = x;
	cam->target[1] = y;
	cam->target[2] = 0;

	cam->cam[0] = x + CAM_CIRCLE_DIST;
	cam->cam[1] = y;
	cam->cam[2] = CAM_CIRCLE_Z;

	cam->bIsGlancing = 0;

	memset(cam->pUser, 0, sizeof(cam->pUser));
	cam->pUserData = NULL;
}

/* place user into recognizer */
void observerCamera(PlayerVisual *pV) {
	vec2 p, v;
	Camera *cam = &pV->camera;
	getRecognizerPositionVelocity(&p, &v);

	cam->cam[0] = p.v[0];
	cam->cam[1] = p.v[1];
	cam->cam[2] = RECOGNIZER_HEIGHT;
	cam->target[0] = p.v[0] + v.v[0];
	cam->target[1] = p.v[1] + v.v[1];
	cam->target[2] = RECOGNIZER_HEIGHT - 2;
}  

float getAngle(int dir)
{
	float fAngle;
	vec2 *v = &game2->level->pAxis[dir];

	fAngle = (float) atan2(- v->v[1], - v->v[0]);
	if(fAngle < 0) fAngle += (float) (2 * M_PI);
	// if(v->v[1] < 0) fAngle += (float) M_PI; // atan2 makes this unnecessary

	return fAngle;
}

float getInterpolatedAngle(float t, float phiLast, float phiNow)
{
	// e.g. phiLast == 3/2 PI and phiNow = 0, interpolate from 3/2 PI to 2 PI
	if( phiLast > phiNow && phiNow + 2 * M_PI - phiLast < phiLast - phiNow)
		phiNow += (float)(2 * M_PI);

	// e.g. phiLast == 0 and phiNow == 3/2 PI, interpolate from 2PI to 3/2 PI
	if( phiNow > phiLast && phiLast + 2 * M_PI - phiNow < phiNow - phiLast)
		phiLast += (float) (2 * M_PI);
	return (1 - t) * phiLast + t * phiNow;
}

void playerCamera(PlayerVisual *pV) {
	float dest[3];
	float tdest[3];
	float phi, chi, r;
	float x, y;
	int mouse_dx, mouse_dy;
	Camera *cam;
	Data *data;
	/* first, process all movement commands */
	/* that means, check for mouse input mainly */

	/* dt hack: the time since the last frame is not necessarily the game
		time, since the game maybe hasn't started yet, or was paused */
	static int last=0;
	int dt;

	if(game2->time.dt == 0) {
		dt = nebu_Time_GetElapsed() - last;
		last = nebu_Time_GetElapsed();
	} else {
		dt = game2->time.dt;
	}

	cam = &pV->camera;
	data = &pV->pPlayer->data;
	getPositionFromData(&x, &y, data);

	if(game->pauseflag != PAUSE_GAME_RUNNING || !getSettingi("mouse_lock_ingame"))
	{
		if(cam->type.freedom[CAM_FREE_R]) {
			// mouse buttons let you zoom in/out
			if(gInput.mouse1 == 1)
				cam->movement[CAM_R] += (cam->movement[CAM_R]-getSettingf("clamp_cam_r_min")+1) * dt / 300.0f;
			if(gInput.mouse2 == 1)
				cam->movement[CAM_R] -= (cam->movement[CAM_R]-getSettingf("clamp_cam_r_min")+1) * dt / 300.0f;
			writeCamDefaults(cam, CAM_R);
		}
		nebu_Input_Mouse_GetDelta(&mouse_dx, &mouse_dy);
		if(cam->type.freedom[CAM_FREE_PHI] && mouse_dx != 0) {
			// mouse x axis lets you rotate horizontally
			int sign = getSettingi("mouse_invert_x") ? -1 : 1;
			cam->movement[CAM_PHI] += sign * (- mouse_dx) * MOUSE_CX;
			writeCamDefaults(cam, CAM_PHI);
		}
		if(cam->type.freedom[CAM_FREE_CHI] && mouse_dy != 0) {
			// mouse y axis lets you rotate vertically
			int sign = getSettingi("mouse_invert_y") ? -1 : 1;
			cam->movement[CAM_CHI] += sign * mouse_dy * MOUSE_CY;
			writeCamDefaults(cam, CAM_CHI);
		}
		/* done with mouse movement, now clamp the camera to legal values */
		clampCam(cam);
	}

	phi = cam->movement[CAM_PHI] + cam->movement[CAM_PHI_OFFSET];
	chi = cam->movement[CAM_CHI];
	r = cam->movement[CAM_R];

	/* if the cam is coupled to player movement, change the phi accordingly */
	if(cam->type.coupled) {
		int time = game2->time.current - data->turn_time;
		if(time < TURN_LENGTH)
		{
			phi += 
				getInterpolatedAngle(
					1 - (float)(TURN_LENGTH - time) / TURN_LENGTH,
					getAngle(data->last_dir),
					getAngle(data->dir));
		}
		else
			phi += getAngle(data->dir);
	}

	/* position the camera */
	dest[0] = x + r * cosf(phi) * sinf(chi);
	dest[1] = y + r * sinf(phi) * sinf(chi);
	dest[2] = r * cosf(chi);

	/* ok, now let's calculate the new camera destination coordinates */
	/* also, perform some camera dependent movement */
	switch(cam->type.type) {
	case CAM_TYPE_CIRCLING: /* Andi-cam */
		cam->movement[CAM_PHI] += CAM_SPEED * game2->time.dt;
		tdest[0] = x;
		tdest[1] = y;
		tdest[2] = B_HEIGHT;
		break;
	case CAM_TYPE_FOLLOW: /* Mike-cam */
	case CAM_TYPE_MOUSE: /* mouse camera */
		tdest[0] = x;
		tdest[1] = y;
		tdest[2] = B_HEIGHT;
		break;
	case CAM_TYPE_COCKPIT: /* 1st person */
		tdest[0] = x + 4.0f * game2->level->pAxis[data->dir].v[0] + 2.0f * cosf(phi);
		tdest[1] = y + 4.0f * game2->level->pAxis[data->dir].v[1] + 2.0f * sinf(phi);
		tdest[2] = CAM_COCKPIT_Z;
		dest[0] = x + 4.0f * game2->level->pAxis[data->dir].v[0] + 0.1f * cosf(phi);
		dest[1] = y + 4.0f * game2->level->pAxis[data->dir].v[1] + 0.1f * sinf(phi);
		dest[2] = CAM_COCKPIT_Z + 0.1f;
		break;
	case CAM_TYPE_OFFSET: /* AMR-cam */
		{
			float dx,dy,tx,ty,gs,d;
	
			gs= box2_Width(&game2->level->boundingBox);
			tx=(x-gs/2)*(1+15/gs)+gs/2; /* Scale position of cycle */
			ty=(y-gs/2)*(1+15/gs)+gs/2;

			dx=cam->pUser[0]-tx;
			dy=cam->pUser[1]-ty;
			d=1.3f * sqrtf(dx*dx+dy*dy); /* Find distance between old viewpoint and scaled cycle pos */
			cam->pUser[0] = dest[0] = tx + CAM_CIRCLE_DIST * dx/d; /* Set viewpoint a fixed distance from */
			cam->pUser[1] = dest[1] = ty + CAM_CIRCLE_DIST * dy/d; /* scaled pos, preserving angle */

			dx=(tx-x)*(tx-x); /* find distance between scaled and actual */
			dy=(ty-y)*(ty-y); /* cycle positions... */
			d=sqrtf(dx*dx+dy*dy)/8.0f;
			dest[2] = CAM_CIRCLE_Z-(d*CAM_CIRCLE_Z)/24; /* ...and lift camera when close to cycle */

			tdest[0] = x;
			tdest[1] = y;
			tdest[2] = B_HEIGHT;
		}
		break;
	}
	memcpy(cam->cam, dest, sizeof(cam->cam));
	memcpy(cam->target, tdest, sizeof(cam->target));
}

void doCameraMovement(void) {
	int i;

	for(i = 0; i < gnPlayerVisuals; i++) {
		if(gppPlayerVisuals[i]->pPlayer->data.speed == SPEED_GONE)
			observerCamera(gppPlayerVisuals[i]);
		else
			playerCamera(gppPlayerVisuals[i]);
	}
}

void nextCameraType(void) {
	int i;
	int current_cam_type = getSettingi("camType");
	int new_cam_type = (current_cam_type + 1) % CAM_COUNT;
	  
	setSettingi("camType", new_cam_type);

	/* update the cached setting */
	gSettingsCache.camType = new_cam_type;
	  
	for(i = 0; i  < gnPlayerVisuals; i++)
	{
		initCamera(gppPlayerVisuals[i], new_cam_type);
	}
	if (getSettingi("debug_output")) {
		switch (new_cam_type) {
		case 0 :
			displayMessage(TO_CONSOLE, "[camera] Circling Camera");
			break;
		case 1 :
			displayMessage(TO_CONSOLE, "[camera] Behind Camera");
			break;
		case 2 :
			displayMessage(TO_CONSOLE, "[camera] Cockpit Camera");
			break;
		case 3 :
			displayMessage(TO_CONSOLE, "[camera] Mouse Camera");
			break;
		case 4 :
			displayMessage(TO_CONSOLE, "[camera] Offset Camera");
			break;
		}
	}
}

