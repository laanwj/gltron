#include "video/video.h"
#include "game/game.h"
#include "video/explosion.h" 

#include "base/nebu_math.h"
#include "video/nebu_renderer_gl.h"

/* shockwave behavior constants */
#define SHOCKWAVE_MIN_RADIUS 0.0f
#define SHOCKWAVE_MAX_RADIUS 45.0f
#define SHOCKWAVE_WIDTH 0.2f
#define SHOCKWAVE_SPACING 6.0f
#define SHOCKWAVE_SPEED 1.2f /* speed relative to IMPACT_RADIUS_DELTA */
#define SHOCKWAVE_SEGMENTS 25
#define NUM_SHOCKWAVES 3 

/* draw a single semi-circular shockwave of a given radius */
static void drawWave(double radius) {
  int i, j;
  double angle;
  double delta_radius = SHOCKWAVE_WIDTH / SHOCKWAVE_SEGMENTS;
  double delta_angle = (180.0 / SHOCKWAVE_SEGMENTS) * (PI/180);
  double start_angle = 270.0 * (PI/180);
  
  for (i = 0; i < SHOCKWAVE_SEGMENTS; i++) {
    glBegin(GL_QUAD_STRIP);
    angle = start_angle;
    for (j = 0; j <= SHOCKWAVE_SEGMENTS; j++) {
      glVertex2d((radius + delta_radius) * sin(angle),
          (radius + delta_radius) * cos(angle));
      glVertex2d(radius * sin(angle), radius * cos(angle));
      angle += delta_angle;
    }
    glEnd();
    radius += delta_radius;
  }
}

static void drawShockwaves(float radius) {
  int waves;
  
  glColor4f(1,0,0, 1.0f);

  for(waves = 0; waves < NUM_SHOCKWAVES; waves++) {
    if (radius > SHOCKWAVE_MIN_RADIUS && radius < SHOCKWAVE_MAX_RADIUS) {
      drawWave(radius);
    }
    radius -= SHOCKWAVE_SPACING;
  }
}

/* impact spire behavior constants */
#define SPIRE_MAX_RADIUS 25
#define SPIRE_WIDTH 0.40f
#define NUM_SPIRES 21 

static void drawSpires(float radius) {
  int i;
  vec3 right, left;
  vec3 zUnit = { { 0, 0, 1} };
  
  vec3 vectors[NUM_SPIRES] = {
    { {  1.00f,  0.20f,  0.00f  } },
    { {  0.80f,  0.25f,  0.00f  } },
    { {  0.90f,  0.50f,  0.00f  } },
    { {  0.70f,  0.50f,  0.00f  } },
    { {  0.52f,  0.45f,  0.00f  } },
    { {  0.65f,  0.75f,  0.00f  } },
    { {  0.42f,  0.68f,  0.00f  } },
    { {  0.40f,  1.02f,  0.00f  } },
    { {  0.20f,  0.90f,  0.00f  } },
    { {  0.08f,  0.65f,  0.00f  } },
    { {  0.00f,  1.00f,  0.00f  } }, /* vertical spire */
    { { -0.08f,  0.65f,  0.00f  } },
    { { -0.20f,  0.90f,  0.00f  } },
    { { -0.40f,  1.02f,  0.00f  } },
    { { -0.42f,  0.68f,  0.00f  } },
    { { -0.65f,  0.75f,  0.00f  } },
    { { -0.52f,  0.45f,  0.00f  } },
    { { -0.70f,  0.50f,  0.00f  } },
    { { -0.90f,  0.50f,  0.00f  } },
    { { -0.80f,  0.30f,  0.00f  } },
    { { -1.00f,  0.20f,  0.00f  } }
  };

  glColor4f(1, 1, 1, 1.0f);
  glVertex3f(0, 0, 0);
 
  glBlendFunc(GL_ONE, GL_ONE);

  glBegin(GL_TRIANGLES);
  
  for (i=0; i < NUM_SPIRES; i++) {
		vec3_Cross(&right, vectors + i, &zUnit);
		vec3_Normalize(&right, &right);
		vec3_Scale(&right, &right, SPIRE_WIDTH);

		vec3_Cross(&left, &zUnit, vectors + i);
		vec3_Normalize(&left, &left);
		vec3_Scale(&left, &left, SPIRE_WIDTH);

    glColor4f(1,1,1,0.0);
    glVertex3fv(right.v);
    glVertex3f(radius * vectors[i].v[0], radius * vectors[i].v[1], 0.0);
    glVertex3fv(left.v);
  } 
  
  glEnd();
}

#define GLOW_START_OPACITY 1.2f
#define GLOW_INTENSITY 1.0f

static void drawImpactGlow(float glow_radius) {

  float opacity;
  
  opacity = GLOW_START_OPACITY - (glow_radius / IMPACT_MAX_RADIUS);
  
  glPushMatrix();
  glScalef(glow_radius, glow_radius, 1.0f);
  
  glBindTexture(GL_TEXTURE_2D, gScreen->textures[TEX_IMPACT]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_TEXTURE_2D);

  glColor4f(GLOW_INTENSITY, GLOW_INTENSITY, GLOW_INTENSITY, opacity);
  glDepthMask(0);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, -1.0);
  glTexCoord2f(1.0, 0.0); glVertex2f(1.0, -1.0);
  glTexCoord2f(1.0, 1.0); glVertex2f(1.0, 1.0);
  glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, 1.0);
  glEnd();
  glDepthMask(1);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void drawExplosion(float radius) {

  float shockwave_radius = (radius * SHOCKWAVE_SPEED);

  drawShockwaves(shockwave_radius);

  if (radius < IMPACT_MAX_RADIUS) {
    drawImpactGlow(radius);
    drawSpires(radius);
  }
}

