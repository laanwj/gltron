#include "video/video.h"
#include "video/trail_geometry.h"
#include "game/game.h"
#include "game/camera.h"
#include "configuration/settings.h"

#include "base/nebu_math.h"
#include "video/nebu_video_system.h"
#include "video/nebu_renderer_gl.h"

#include "base/nebu_debug_memory.h"

#define TEX_SPLIT (1.0 - BOW_DIST2) / (1 - BOW_DIST1)
#undef TEX_SPLIT

static float normal1[] = { 1.0, 0.0, 0.0 };
static float normal2[] = { 0.0, 1.0, 0.0 };

/* 
   getDists returns the minimum distance from (the wall) *line to the
   specified (eye) point
   the z component is ignored
 */
float getDist(segment2 *s, float* eye) {
	vec2 n, tmp;

	n.v[0] = s->vStart.v[0] + s->vDirection.v[1];
	n.v[1] = s->vStart.v[1] - s->vDirection.v[0];
	tmp.v[0] = eye[0] - s->vStart.v[0];
	tmp.v[1] = eye[1] - s->vStart.v[1];
	if(n.v[0] == n.v[1] == 0) return vec2_Length(&tmp);
	return (float) fabs(vec2_Dot(&n, &tmp) / vec2_Length(&n));
}

/*
  getSegmentEnd[XY]() returns the end point of the
  last trail segment line (before the lightcycles bow starts)
*/

float dists[] = { BOW_DIST2, BOW_DIST3, BOW_DIST1, 0 };

float getSegmentEndX(Data *data, int dist) {
	float tlength, blength;
	segment2 *s = data->trails + data->nTrails - 1;

	if(game2->level->pAxis[data->dir].v[0] == 0) 
		return s->vStart.v[0] + s->vDirection.v[0];

	tlength = segment2_Length(s);
	blength = (tlength < 2 * BOW_LENGTH) ? tlength / 2 : BOW_LENGTH;
	return 
		s->vStart.v[0] + s->vDirection.v[0] -
		dists[dist] * blength * game2->level->pAxis[data->dir].v[0];
}

float getSegmentEndY(Data *data, int dist) {
	float tlength, blength;
	segment2 *s = data->trails + data->nTrails - 1;

	if(game2->level->pAxis[data->dir].v[1] == 0)
		return s->vStart.v[1] + s->vDirection.v[1];

	tlength = segment2_Length(s);
	blength = (tlength < 2 * BOW_LENGTH) ? tlength / 2 : BOW_LENGTH;
	return 
		s->vStart.v[1] + s->vDirection.v[1] -
		dists[dist] * blength * game2->level->pAxis[data->dir].v[1];
}

/* getSegmentEndUV() calculates the texture coordinates for the last segment */
float getSegmentEndUV(segment2 *s, Data *data) {
	float tlength, blength;
	tlength = segment2_Length(s);
	blength = (tlength < 2 * BOW_LENGTH) ? tlength / 2 : BOW_LENGTH;
	return (tlength - 2 * blength) / DECAL_WIDTH;
}

/* getSegmentUV gets UV coordinates for an ordinary segment */
float getSegmentUV(segment2 *s) {
	return segment2_Length(s) / DECAL_WIDTH;
}

/* 
   drawTrailLines() draws a white line on top of each trail segment
   the alpha value is reduced with increasing distance to the player
*/

void drawTrailLines(Camera *pCamera, Player *p) {
	segment2 *s;
	int i;
	float height;

	float *normal;
	float dist;
	float alpha;
	Data *data;

	float trail_top[] = { 1.0, 1.0, 1.0, 1.0 };

	data = & p->data;

	height = data->trail_height;
	if(height <= 0)
		return;

	/*
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	*/

	if (gSettingsCache.antialias_lines) {
		glEnable(GL_LINE_SMOOTH); /* enable line antialiasing */
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_LINES);

	/* the current line is not drawn */
	for(i = 0; i < data->nTrails - 1; i++)
	{
		s = data->trails + i;
		/* compute distance from line to eye point */
		dist = getDist(s, pCamera->cam);
		alpha = (400 - dist / 2) / 400;
		// TODO: compute the 'magic' 400 somehow
		if(alpha < 0)
			alpha = 0;
		// trail_top[3] = alpha;
		glColor4f(trail_top[0],
			trail_top[1],
			trail_top[2],
			trail_top[3]);

		if(s->vDirection.v[1] == 0)
			normal = normal1;
		else
			normal = normal2;
		glNormal3fv(normal);
		glVertex3f(s->vStart.v[0],
			s->vStart.v[1],
			height);
		glVertex3f(s->vStart.v[0] + s->vDirection.v[0],
			s->vStart.v[1] + s->vDirection.v[1],
			height);
	}
	glEnd();

	// current line now
	s = data->trails + data->nTrails - 1;
	/* compute distance from line to eye point */
	dist = getDist(s, pCamera->cam);
	// TODO: compute the 'magic' 400 somehow
	alpha = (400 - dist / 2) / 400;
	if(alpha < 0)
		alpha = 0;
	// trail_top[3] = alpha;
	glColor4f(
		trail_top[0],
		trail_top[1],
		trail_top[2],
		1);

	glBegin(GL_LINES);

	glVertex3f(s->vStart.v[0],
		s->vStart.v[1],
		height);
	glVertex3f( getSegmentEndX(data, 0),
		getSegmentEndY(data, 0),
		height );

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH); /* disable line antialiasing */

	/*
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	*/
}

/* 
	drawTrailShadow() draws a alpha-blended shadow on the floor for each
	trail segment.
	The light source source is (in homogenous coordinates)
	at (-1,-1,1,0) (I hope that's correct)
*/

void drawTrailShadow(Player* p) {
	int vOffset = 0;
	int iOffset = 0;
	TrailMesh mesh;

	nebu_Video_CheckErrors("before trail shadows\n");

	/* transformation */
	glPushMatrix();
	glMultMatrixf(shadow_matrix);

	/* geometry */
	mesh.pVertices = (vec3*) malloc(1000 * sizeof(vec3));
	mesh.pNormals = (vec3*) malloc(1000 * sizeof(vec3));
	mesh.pColors = (unsigned char*) malloc(1000 * 4 * sizeof(float));
	mesh.pTexCoords = (vec2*) malloc(1000 * sizeof(vec2));
	mesh.pIndices = (unsigned short*) malloc(1000 * 2);
	mesh.iUsed = 0;
	
	trailGeometry(p, &p->profile, &mesh, &vOffset, &iOffset);
	bowGeometry(p, &p->profile, &mesh, &vOffset, &iOffset);
	trailStatesShadowed();
	trailRender(&mesh);
	// no states restore, because we're drawing shadowed geometry

	free(mesh.pVertices);
	free(mesh.pNormals);
	free(mesh.pColors);
	free(mesh.pTexCoords);
	free(mesh.pIndices);

	/* restore */
	glPopMatrix();

	nebu_Video_CheckErrors("after trail shadows");
}
