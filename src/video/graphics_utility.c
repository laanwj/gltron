#include "video/video.h"
#include "video/nebu_font.h"
#include "game/game.h"

#include "video/nebu_video_utility.h"
#include "video/nebu_renderer_gl.h"
#include "base/nebu_math.h"

#include <string.h>

void rasonly(Visual *d) {
	nebu_Video_rasonly(d->vp_x, d->vp_y, d->vp_w, d->vp_h);
}

void doPerspective(float fov, float ratio, float znear, float zfar) {
	float top;
	float left;

	top = tanf( fov * PI / 360.0 ) * znear;
	left = - top * ratio;
	glFrustum(left, -left, -top, top, znear, zfar);
}

void doLookAt(float *cam, float *target, float *up) {
	float m[16];
	vec3 x, y, z;

	vec3_Sub(&z, (vec3*) cam, (vec3*) target);
	vec3_Normalize(&z, &z);
	vec3_Cross(&x, (vec3*) up, &z);
	vec3_Normalize(&x, &x);
	vec3_Cross(&y, &z, &x);
	vec3_Normalize(&y, &y);

#define M(row,col)  m[col*4+row]
  M(0,0) = x.v[0];  M(0,1) = x.v[1];  M(0,2) = x.v[2];  M(0,3) = 0.0;
  M(1,0) = y.v[0];  M(1,1) = y.v[1];  M(1,2) = y.v[2];  M(1,3) = 0.0;
  M(2,0) = z.v[0];  M(2,1) = z.v[1];  M(2,2) = z.v[2];  M(2,3) = 0.0;
  M(3,0) = 0.0;   M(3,1) = 0.0;   M(3,2) = 0.0;   M(3,3) = 1.0;
#undef M
	glMultMatrixf( m );

	/* Translate Eye to Origin */
	glTranslatef( -cam[0], -cam[1], -cam[2]);
}

void drawText(nebu_Font* ftx, float x, float y, float size, const char *text) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	glTranslatef(x, y, 0);
	glScalef(size, size, size);
	nebu_Font_Render(ftx, text, strlen(text));

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

