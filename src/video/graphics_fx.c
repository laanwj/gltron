#include "video/video.h"
#include "game/game.h"
#include "game/camera.h"
#include "video/explosion.h"

#include "base/nebu_math.h"
#include "video/nebu_renderer_gl.h"

void drawGlow(PlayerVisual *pV, Player *pTarget, float dim)
{
	float mat[4*4];
	float alpha, dist;
	float x, y;
	vec3 v;
	const float fMin = 30;
	const float fMax = 100;

	Camera *pCamera = &pV->camera;
	Visual *d = &pV->display;

	getPositionFromData(&x, &y, &pTarget->data);

	v.v[0] = x - pCamera->cam[0];
	v.v[1] = y - pCamera->cam[1];
	v.v[2] = 0 - pCamera->cam[2];
	dist = vec3_Length(&v);
	if(dist < fMin)
		return;
	if(dist > fMax) alpha = 1.0f;
	else alpha = (dist - fMin) / (fMax - fMin);
	// printf("dist: %.2f, alpha: %.2f\n", dist, alpha);

	glPushMatrix();
	
  glTranslatef(x, y, 0);

  glDepthMask(GL_FALSE);
  glEnable(GL_DEPTH_TEST);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glEnable(GL_BLEND);

  glGetFloatv(GL_MODELVIEW_MATRIX, mat);
  mat[0] = mat[5] = mat[10] = 1.0;
  mat[1] = mat[2] = 0.0;
  mat[4] = mat[6] = 0.0;
  mat[8] = mat[9] = 0.0;
  glLoadMatrixf(mat);


  glBegin(GL_TRIANGLE_FAN);
  glColor4f(pTarget->profile.pColorDiffuse[0], 
						pTarget->profile.pColorDiffuse[1], 
						pTarget->profile.pColorDiffuse[2],
						alpha);

  glVertex3f(0,TRAIL_HEIGHT/2, 0);
  glColor4f(0,0,0,0.0);
  glVertex3d(dim*cos(-0.2*3.1415/5.0),
	     TRAIL_HEIGHT/2+dim*sin(-0.2*3.1415/5.0), 0);
  glVertex3d(dim*cos(1.0*3.1415/5.0),
	     TRAIL_HEIGHT/2+dim*sin(1.0*3.1415/5.0), 0);
  glVertex3d(dim*cos(2.0*3.1415/5.0),
	     TRAIL_HEIGHT/2+dim*sin(2.0*3.1415/5.0), 0);
  glVertex3d(dim*cos(3.0*3.1415/5.0),
	     TRAIL_HEIGHT/2+dim*sin(3.0*3.1415/5.0), 0);
  glVertex3d(dim*cos(4.0*3.1415/5.0),
	     TRAIL_HEIGHT/2+dim*sin(4.0*3.1415/5.0), 0);
  glVertex3d(dim*cos(5.2*3.1415/5.0),
	     TRAIL_HEIGHT/2+dim*sin(5.2*3.1415/5.0), 0);
  glEnd();

  glBegin(GL_TRIANGLES);
  glColor4f(pTarget->profile.pColorDiffuse[0], 
						pTarget->profile.pColorDiffuse[1], 
						pTarget->profile.pColorDiffuse[2],
						alpha);
  glVertex3f(0,TRAIL_HEIGHT/2, 0);
  glColor4f(0,0,0,0.0);
  glVertex3f(0,-TRAIL_HEIGHT/4,0);
  glVertex3d(dim*cos(-0.2f*3.1415/5.0),
	     TRAIL_HEIGHT/2+dim*sin(-0.2*3.1415/5.0), 0);

  glColor4f(pTarget->profile.pColorDiffuse[0], 
						pTarget->profile.pColorDiffuse[1], 
						pTarget->profile.pColorDiffuse[2],
						alpha);
  glVertex3f(0,TRAIL_HEIGHT/2, 0);
  glColor4f(0,0,0,0.0);
  glVertex3d(dim*cos(5.2*3.1415/5.0),
	     TRAIL_HEIGHT/2+dim*sin(5.2*3.1415/5.0), 0);
  glVertex3f(0,-TRAIL_HEIGHT/4,0);
  glEnd();

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);

  glPopMatrix();  
}

void drawImpact(int player) {
	glPushMatrix();
	glRotatef(90, 90, 0, 1);
	/*
	* Drop impact slightly below the floor and move it a tiny bit
	* back from the impact point so we don't draw behind the
	* trail/wall that's hit.
	*/
	glTranslatef(0.0, -0.5, -0.5);
	glColor4f(0.68f, 0, 0, 1.0f);
	drawExplosion(game->player[player].data.exp_radius);
	glPopMatrix();
}

