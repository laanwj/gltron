#include "video/video.h"
#include "game/game.h"
#include "game/resource.h"
#include "base/nebu_resource.h"
#include "filesystem/path.h"
#include "configuration/settings.h"

#include "base/nebu_math.h"
#include "scripting/nebu_scripting.h"
#include "video/nebu_renderer_gl.h"

#include <string.h>

#include "base/nebu_debug_memory.h"

void getPauseString(char *buf, float* color);

void hud_MaskSetup(int maskId, int maskIndex) {
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);

	glEnable(GL_STENCIL_TEST);
	glStencilMask(~0);
	glStencilFunc(GL_ALWAYS, maskIndex, ~0);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	nebu_2d_Draw((nebu_2d*)resource_Get(gpTokenHUD[maskId], eRT_2d));
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

	// draw gauge where stencil is set
	glStencilFunc(GL_EQUAL, maskIndex, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void hud_MaskFinish(void)
{
	glDisable(GL_STENCIL_TEST);
}

#define FPS_HSIZE 20


int getFPS(void)
{
	/* draws FPS in upper left corner of Display d */
	static int fps_h[FPS_HSIZE];
	static int pos = -FPS_HSIZE;
	static int fps_min = 0;
	static int fps_avg = 0;

	int dt = nebu_Time_GetTimeForLastFrame();
	if(dt <= 0)
		dt = 1;

	if(pos < 0)
	{
		fps_avg = 1000 / dt;
		fps_min = 1000 / dt;
		fps_h[pos + FPS_HSIZE] = 1000 / dt;
		pos++;
	}
	else
	{
		fps_h[pos] = 1000 / dt;
		pos = (pos + 1) % FPS_HSIZE;
		if(pos % 10 == 0)
		{
			int i;
			int sum = 0;
			int min = 1000;
			for(i = 0; i < FPS_HSIZE; i++)
			{
				sum += fps_h[i];
				if(fps_h[i] < min)
					min = fps_h[i];
			}
			fps_min = min;
			fps_avg = sum / FPS_HSIZE;
			// printf("FPS: min = %d, avg = %d\n", fps_min, fps_avg);
		}
	}
	return fps_avg;
}

void drawHUD(Player *p, PlayerVisual *pV)
{
	char temp[1024];
	char pause_message[128];
	float pause_color[3];

	getPauseString(pause_message, pause_color);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	rasonly(&pV->display);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*
		drawHud: parameters
		- Viewport Width
		- Viewport Height
		- Score (or -1 if disabled)
		- AI status ("computer player" or "")
		- Speed digital (absolute value)
		- Speed analog (1 for default speed, > 1 during acceleration)
		- Booster value (between 0 and 1)
		- fps
		- pause message
		- pause message r,g,b colors
	*/

	sprintf(temp, "drawHUD(%d, %d, %d, \"%s\", %f, %f, %f, %f, %d, \"%s\", %f, %f, %f)",
		pV->display.vp_w, pV->display.vp_h,
		gSettingsCache.show_scores ? p->data.score : -1,
		gSettingsCache.show_ai_status ?
		(p->ai.active ? "AI_COMPUTER" : "") : "",
		p->data.speed,
		p->data.speed / (2 * game2->rules.speed),
		p->data.energy / getSettingf("energy"),
		0.0f, // formerly wallbuster
		getFPS(),
		pause_message,
		pause_color[0],
		pause_color[1],
		pause_color[2]
		);

	glScalef(pV->display.vp_w / 1024.0f, pV->display.vp_w / 1024.0f, 1.0f);
	// fprintf(stderr, "%s\n", temp);
	scripting_Run(temp);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void getPauseString(char *buf, float* color) {
  char pause[] = "Game is paused";
  char winner[] = "Player %d wins!";
  char nowinner[] = "No one wins!";
  char starting[] = "Press a key to start";
  static float d = 0;
  static int lt = 0;
  int now;

  now = nebu_Time_GetElapsed();
  d += (now - lt) / 500.0f;
  lt = now;
  /* printf("%.5f\n", delta); */
  
  if (d > 2 * PI) { 
    d -= 2 * PI;
  }

  if ((game->pauseflag & PAUSE_GAME_FINISHED) && game->winner != -1) {
    if (game->winner >= -1) {

      float* player_color = game->player[game->winner].profile.pColorAlpha;

      /* 
         make the 'Player wins' message oscillate between 
         white and the winning bike's color 
       */
      color[0] = ((player_color[0] + ((sinf(d) + 1) / 2) * (1 - player_color[0])));
	  color[1] = ((player_color[1] + ((sinf(d) + 1) / 2) * (1 - player_color[1])));
	  color[2] = ((player_color[2] + ((sinf(d) + 1) / 2) * (1 - player_color[2])));
   
      sprintf(buf, winner, game->winner + 1);
    } else {
		color[0] = 1.0f;
		color[1] = (sinf(d) + 1) / 2;
		color[2] = (sinf(d) + 1) / 2;
	  sprintf(buf, "%s", nowinner);
    }
  } else if(game->pauseflag & PAUSE_GAME_SUSPENDED) {
		color[0] = 1.0f;
		color[1] = (sinf(d) + 1) / 2;
		color[2] = (sinf(d) + 1) / 2;
	  sprintf(buf, "%s", pause);
  } else if(game->pauseflag & PAUSE_GAME_STARTING) {
		color[0] = 1.0f;
		color[1] = (sinf(d) + 1) / 2;
		color[2] = (sinf(d) + 1) / 2;
	  sprintf(buf, "%s", starting);
  } else {
	  // game is running
	  color[0] = 0;
	  color[1] = 0;
	  color[2] = 0;
	  buf[0] = 0;
  }
}

/* new hud stuff starts here */

void drawCircle(float phiStart, float phiEnd, 
								int nSegments, 
								float r1, float r2,
								float *c1, float *c2, float *c3, float *c4);
void drawRect(float width, float height, float *colors);

static void getColor(lua_State *l, float *out) {
	char *names[] = { "r", "g", "b" };
	int i;
	for(i = 0; i < 3; i++) {
		lua_pushstring(l, names[i]);
		lua_gettable(l, -2);
		out[i] = (float) lua_tonumber(l, -1);
		lua_pop(l, 1); // result
	}
	lua_pop(l, 1); // table
}

int c_drawRectangle(lua_State *l) {
	float width = 0, height = 0;
	float colors[12];

	getColor(l, colors + 9);
	getColor(l, colors + 6);
	getColor(l, colors + 3);
	getColor(l, colors + 0);
	
	height = (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	width =  (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	
	drawRect(width, height, colors);

	return 0;
}
	
int c_drawCircle(lua_State *l) {
	float phiStart = 0, phiEnd = 0;
	int nSegments = 0;
	float r1 = 0, r2 = 0;

	float c1[3] = { 0, 0, 0 };
	float c2[3] = { 0, 0, 0 };
	float c3[3] = { 0, 0, 0 };
	float	c4[3] = { 0, 0, 0 };
	
	getColor(l, c4);
	getColor(l, c3);
	getColor(l, c2);
	getColor(l, c1);

	r2 = (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	r1 = (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	nSegments = (int) lua_tonumber(l, -1);		lua_pop(l, 1);
	phiEnd = (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	phiStart = (float) lua_tonumber(l, -1);		lua_pop(l, 1);

	drawCircle(phiStart, phiEnd, nSegments, r1, r2, c1, c2, c3, c4);

	return 0;
}

int c_translate(lua_State *l) {
	float x = 0, y = 0, z = 0;
	z = (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	y = (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	x = (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	glTranslatef(x,y,z);

	return 0;
}

int c_scale(lua_State *l) {
	float x = 0, y = 0, z = 0;
	z = (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	y = (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	x = (float) lua_tonumber(l, -1);		lua_pop(l, 1);
	glScalef(x,y,z);

	return 0;
}

int c_pushMatrix(lua_State *l) {
	glPushMatrix();
	return 0;
}

int c_popMatrix(lua_State *l) {
	glPopMatrix();
	return 0;
}

int c_color(lua_State *l)
{
	float r, g, b, a;
	a = (float) lua_tonumber(l, -1);	lua_pop(l, 1);
	b = (float) lua_tonumber(l, -1);	lua_pop(l, 1);
	g = (float) lua_tonumber(l, -1);	lua_pop(l, 1);
	r = (float) lua_tonumber(l, -1);	lua_pop(l, 1);
	glColor4f(r, g, b, a);
	return 0;
}

int c_drawTextFitIntoRect(lua_State *l) {
	// text, width, height, flags
	char *text;
	float width;
	float height;
	int flags;
	box2 box;

	scripting_GetIntegerResult(&flags);
	scripting_GetFloatResult(&height);
	scripting_GetFloatResult(&width);
	scripting_GetStringResult(&text);
	// these are ignored!
	box.vMin.v[0] = 0;
	box.vMin.v[1] = 0;
	box.vMax.v[0] = width;
	box.vMax.v[1] = height;
	nebu_Font_RenderToBox((nebu_Font*)resource_Get(gTokenGameFont, eRT_Font), text, strlen(text), &box, flags);
	scripting_StringResult_Free(text);

	return 0;
}

int c_draw2D(lua_State* l)
{
	nebu_Rect rect = { 0, 0, 0, 0 };
	scripting_GetIntegerResult(&rect.height);
	scripting_GetIntegerResult(&rect.width);
	draw2D(&rect);
	return 0;
}

int c_drawHUDSurface(lua_State* l)
{
	int surface;
	scripting_GetIntegerResult(&surface);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	nebu_2d_Draw((nebu_2d*)resource_Get(gpTokenHUD[surface], eRT_2d));
	glDisable(GL_BLEND);
	return 0;
}

int c_drawHUDMask(lua_State* l)
{
	int maskId, maskIndex;
	scripting_GetIntegerResult(&maskIndex);
	scripting_GetIntegerResult(&maskId);
	if(maskId < 0)
	{
		hud_MaskFinish();
	}
	else
	{
		hud_MaskSetup(maskId, maskIndex);
	}
	return 0;
}
		
void rgb_interpolate(float *color, float t, float *c1, float *c2) {
	int i;
	// printf("%.2f\n", t);
	for(i = 0; i < 3; i++) {
		color[i] = c1[i] * (1 - t) + c2[i] * t;
	}
}

void drawCircle(float phiStart, float phiEnd, 
								int nSegments, 
								float r1, float r2,
								float *c1, float *c2, float *c3, float *c4) {
	float *pVertices;
	float *pColors;
	unsigned short* pIndices;

	int i;

	if(nSegments < 1)
		return;

	pVertices = (float*) malloc( (nSegments + 1) * 2 * 3 * sizeof(float) );
	pColors = (float*) malloc( (nSegments + 1) * 2 * 3 * sizeof(float) );

	for(i = 0; i < nSegments + 1; i++) {
		float t = i / (float)nSegments;
		float rad = (1 - t) * phiStart + t * phiEnd;
		pVertices[3 * (2 * i + 0) + 0] = cosf(rad) * r1;
		pVertices[3 * (2 * i + 0) + 1] = sinf(rad) * r1;
		pVertices[3 * (2 * i + 0) + 2] = 0;

		pVertices[3 * (2 * i + 1) + 0] = cosf(rad) * r2;
		pVertices[3 * (2 * i + 1) + 1] = sinf(rad) * r2;
		pVertices[3 * (2 * i + 1) + 2] = 0;
		
		rgb_interpolate(pColors + 3 * (2 * i + 0), t, c1, c2);
		rgb_interpolate(pColors + 3 * (2 * i + 1), t, c3, c4);
	}
	
	pIndices = (unsigned short*) 
		malloc( (nSegments) * 2 * 3 * sizeof(unsigned short) );

	for(i = 0; i < nSegments; i++) {
		pIndices[3 * (2 * i + 0) + 0] = (i + 0) * 2 + 0;
		pIndices[3 * (2 * i + 0) + 1] = (i + 0) * 2 + 1;
		pIndices[3 * (2 * i + 0) + 2] = (i + 1) * 2 + 0;

		pIndices[3 * (2 * i + 1) + 0] = (i + 1) * 2 + 0;
		pIndices[3 * (2 * i + 1) + 1] = (i + 0) * 2 + 1;
		pIndices[3 * (2 * i + 1) + 2] = (i + 1) * 2 + 1;
	}
	
	glVertexPointer(3, GL_FLOAT, 0, pVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, pColors);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glDrawElements(GL_TRIANGLES, 3 * 2 * nSegments, GL_UNSIGNED_SHORT, pIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	free(pVertices);
	free(pColors);
	free(pIndices);
	
}

void drawRect(float width, float height, 
							float *colors) {
	float vertices[12] = { 
		0, 0, 0, 
		0, height, 0,
		width, height, 0,
		width, 0, 0
	};
		
	unsigned short indices[] = { 
		0, 1, 2, 0, 2, 3
	};

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

