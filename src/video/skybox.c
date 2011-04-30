#include "video/video.h"
#include "game/game.h"
#include "configuration/settings.h"
#include "video/nebu_renderer_gl.h"

static void bindSkyboxTexture(int index) {
	glBindTexture(GL_TEXTURE_2D, gScreen->textures[ TEX_SKYBOX0 + index ]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void drawSkybox(float d) {  
	/* these are the values for y == up, x == front */
	/* 
	float sides[6][4][3] = {
		{ { 1, -1, -1 }, { 1, -1, 1 }, { 1, 1, 1 },  { 1, 1, -1 } }, // front
		{ { 1, 1, -1 }, { 1, 1, 1 }, { -1, 1, 1 }, { -1, 1, -1 } }, // top
		{ { -1, -1, -1 }, { 1, -1, -1 },  { 1, 1, -1 }, { -1, 1, -1 } }, // left
		{ { 1, -1, 1 }, { -1, -1, 1 }, { -1, 1, 1 }, { 1, 1, 1 } }, // right
		{ { -1, -1, -1 }, { 1, -1, -1 }, { 1, -1, 1 }, { -1, -1, 1 } }, // bottom
		{ { -1, -1, 1 }, { -1, -1, -1 }, { -1, 1, -1 }, { -1, 1, a1 } } // back
		};
	*/

	/* these values are for z == up, x == front */
	float sides[6][12] = {
		{ 1, 1, -1,
		  1, -1, -1,
		  1, 1, 1,
		  1, -1, 1 }, /* front */
		{ 1, 1, 1,
		 -1, 1, 1,
		 1, -1, 1,
		 -1, -1, 1 }, /* top */
		{ -1, 1, -1,
		  1, 1, -1,
		  -1, 1, 1,
		  1, 1, 1 }, /* left */
		{ 1, -1, -1,
		-1, -1, -1,
		1, -1, 1,
		-1, -1, 1 }, /* right */
		{ -1, 1, -1,
		  -1, -1, -1,
		  1, 1, -1,
		  1, -1, -1 }, /* bottom */
		{ -1, -1, -1,
		  -1, 1, -1,
		  -1, -1, 1,
		  -1, 1, 1 } /* back */
	};

	float uv[4][2] = { 0, 0, 1, 0, 0, 1, 1, 1 };
	int i;

	if (!gSettingsCache.show_skybox)
		return;

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, uv);
	glPushMatrix();
	glScalef(d, d, d);

	for(i = 0; i < 6; i++) {
		bindSkyboxTexture(i);
		glVertexPointer(3, GL_FLOAT, 0, sides[i]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
