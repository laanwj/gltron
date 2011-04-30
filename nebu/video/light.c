#include "geom.h"

static float normal[4];
static float eye[4];
static float vertex[4];
static float color[4];
static float factor[4];
static float light[4];

void setNormal3fv(float *f) {
  normal[0] = f[0];
  normal[1] = f[1];
  normal[2] = f[2];
  normal[3] = 1.0;
}

void setEye3fv(float *f) {
  eye[0] = f[0];
  eye[1] = f[1];
  eye[2] = f[2];
  eye[3] = 1.0;
}

void setVertex3f(float a, float b, float c) {
  vertex[0] = a;
  vertex[1] = b;
  vertex[2] = c;
  vertex[3] = 1.0;
}

void setVertex3fv(float *f) {
  vertex[0] = f[0];
  vertex[1] = f[1];
  vertex[2] = f[2];
  vertex[3] = 1.0;
}

void setLight3fv(float *f) {
  light[0] = f[0];
  light[1] = f[1];
  light[2] = f[2];
  light[3] = 1.0;
}

void setLight4fv(float *f) {
  light[0] = f[0];
  light[1] = f[1];
  light[2] = f[2];
  light[3] = f[3];
}

void setColor3fv(float *f) {
  color[0] = f[0];
  color[1] = f[1];
  color[2] = f[2];
  color[3] = 1.0;
}

void setColor4fv(float *f) {
  color[0] = f[0];
  color[1] = f[1];
  color[2] = f[2];
  color[3] = f[3];
}

void setFactor3fv(float *f) {
  factor[0] = f[0];
  factor[1] = f[1];
  factor[2] = f[2];
  factor[3] = 1.0;
}

void light4fv(float *f) {
  float cphi;
  float tmp[4];

  vsub4(light, vertex, tmp);
  cphi = scalarprod4(tmp, normal) / length4(tmp);
  f[0] = color[0] + cphi * factor[0];
  f[1] = color[1] + cphi * factor[1];
  f[2] = color[2] + cphi * factor[2];
  f[3] = color[3];
  if(f[0] < 0) f[0] = 0;
  else if(f[0] > 1) f[0] = 1;
  if(f[1] < 0) f[1] = 0;
  else if(f[1] > 1) f[1] = 1;
  if(f[2] < 0) f[2] = 0;
  else if(f[2] > 1) f[2] = 1;
  /* printf("returning color %.2f %.2f %.2f\n", f[0], f[1], f[2]); */
}





