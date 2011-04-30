#include "video/video.h"
#include "game/game.h"
#include "game/game_level.h"
#include "game/resource.h"
#include "base/nebu_resource.h"
#include "configuration/settings.h"

#include "base/nebu_math.h"
#include "video/nebu_renderer_gl.h"

static float alpha = 0;

const static float rec_scale_factor = 0.25f;

static float xv[] = { 0.5f, 0.3245f, 0.6f, 0.5f, 0.68f, -0.3f };
static float yv[] = { 0.8f, 1.0f, 0.0f, 0.2f, 0.2f, 0.0f };

static float x(void) { return xv[0] * sinf(xv[1] * alpha + xv[2]) - xv[3] * sinf(xv[4] * alpha + xv[5]); }
static float y(void) { return yv[0] * cosf(yv[1] * alpha + yv[2]) - yv[3] * sinf(yv[4] * alpha + yv[5]); }

static float dx(void) { return xv[1] * xv[0] * cosf(xv[1] * alpha + xv[2]) - xv[4] * xv[3] * cosf(xv[4] * alpha + xv[5]); }
static float dy(void) { return - yv[1] * yv[0] * sinf(yv[1] * alpha + yv[2]) - yv[4] * yv[3] * sinf(yv[4] * alpha + yv[5]); }

float getRecognizerAngle(vec2 *velocity)
{
  float dxval = velocity->v[0];
  float dyval = velocity->v[0];
  
  float phi = acosf ( dxval / sqrtf( dxval * dxval + dyval * dyval ) );
  if (dyval < 0) {
    phi = 2 * PI - phi;
  }
  return (phi + PI / 2) * 180 / PI;
}

void getRecognizerPositionVelocity(vec2 *p, vec2 *v)
{
	float rec_boundry = box2_Diameter(& game2->level->boundingBox) * (1-rec_scale_factor);
	box2_Center(p, & game2->level->boundingBox);
	p->v[0] += x() * rec_boundry / 2.0f;
	p->v[1] += y() * rec_boundry / 2.0f;
	v->v[0] = dx() * rec_boundry / 100.f;
	v->v[1] = dy() * rec_boundry / 100.f;
}

void drawRecognizerShadow(void) {
	float dirx;
	vec2 p, v;
	/* states */

	glEnable(GL_CULL_FACE);

	/* transformations */
	getRecognizerPositionVelocity(&p, &v);
	dirx = getRecognizerAngle(&v);

	glPushMatrix();
	glMultMatrixf(shadow_matrix);
	glTranslatef( p.v[0], p.v[1], RECOGNIZER_HEIGHT);
	glRotatef(dirx, 0, 0, 1); /* up */
	glScalef(rec_scale_factor, rec_scale_factor, rec_scale_factor);
	glEnable(GL_NORMALIZE);

	/* render */

	gltron_Mesh_Draw(((gltron_Mesh*)resource_Get(gTokenRecognizer, eRT_GLtronTriMesh)), TRI_MESH);

	/* restore */

	glDisable(GL_CULL_FACE);
	glPopMatrix();
}

void drawRecognizer(void) {
  float dirx;
  vec2 p, v;

  glPushMatrix();

  /* transformations */
  getRecognizerPositionVelocity(&p, &v);
  dirx = getRecognizerAngle(&v);

  glTranslatef( p.v[0], p.v[1], RECOGNIZER_HEIGHT);
  glRotatef(dirx, 0, 0, 1); /* up */
  
  glScalef(rec_scale_factor, rec_scale_factor, rec_scale_factor); 
  
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
  glLightfv(GL_LIGHT2, GL_SPECULAR, rec_spec_color); 
  glEnable(GL_LIGHT2);

  glDisable(GL_BLEND);

  glEnable(GL_CULL_FACE);
  
  if (gSettingsCache.light_cycles) {
	glEnable(GL_LIGHTING);
  }
  
  glEnable(GL_NORMALIZE);
  glColor4f(0.0, 0.0, 0.0, 1.0f);
  gltron_Mesh_Draw((gltron_Mesh*)resource_Get(gTokenRecognizer, eRT_GLtronTriMesh), TRI_MESH);

  glDisable(GL_LIGHT2);
  glEnable(GL_LIGHT1);
  glDisable(GL_LIGHTING);

  glColor4f(
	  rec_outline_color[0],
	  rec_outline_color[1],
	  rec_outline_color[2],
	  1);

  glPolygonOffset(1,8);
  glEnable(GL_POLYGON_OFFSET_LINE);
  drawSharpEdges((gltron_Mesh*)resource_Get(gTokenRecognizer, eRT_GLtronTriMesh));
  glDisable(GL_POLYGON_OFFSET_LINE);

  glDisable(GL_CULL_FACE);
  
  glPopMatrix();
}  

void doRecognizerMovement(void) {
  alpha += game2->time.dt / 2000.0f;
}

void resetRecognizer(void) {
	alpha = 0;
}




