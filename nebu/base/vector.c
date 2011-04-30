#include "base/nebu_vector.h"
#include "base/nebu_math.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "base/nebu_assert.h"

// review: 64bit ok

vec4* vec4_Add(vec4 *pOut, const vec4 *pV1, const vec4 *pV2) {
  pOut->v[0] = pV1->v[0] + pV2->v[0];
  pOut->v[1] = pV1->v[1] + pV2->v[1];
  pOut->v[2] = pV1->v[2] + pV2->v[2];
  pOut->v[3] = pV1->v[3] + pV2->v[3];
  return pOut;
}

vec4* vec4_Sub(vec4 *pOut, const vec4 *pV1, const vec4 *pV2) {
  pOut->v[0] = pV1->v[0] - pV2->v[0];
  pOut->v[1] = pV1->v[1] - pV2->v[1];
  pOut->v[2] = pV1->v[2] - pV2->v[2];
  pOut->v[3] = pV1->v[3] - pV2->v[3];
  return pOut;
}

vec3* vec3_Add(vec3 *pOut, const vec3 *pV1, const vec3 *pV2) {
  pOut->v[0] = pV1->v[0] + pV2->v[0];
  pOut->v[1] = pV1->v[1] + pV2->v[1];
  pOut->v[2] = pV1->v[2] + pV2->v[2];
  return pOut;
}

vec3* vec3_Sub(vec3 *pOut, const vec3 *pV1, const vec3 *pV2) {
  pOut->v[0] = pV1->v[0] - pV2->v[0];
  pOut->v[1] = pV1->v[1] - pV2->v[1];
  pOut->v[2] = pV1->v[2] - pV2->v[2];
  return pOut;
}

vec3* vec3_Cross(vec3 *pOut, const vec3 *pV1, const vec3 *pV2) {
  vec3 tmp;
  tmp.v[0] = pV1->v[1] * pV2->v[2] - pV1->v[2] * pV2->v[1];
  tmp.v[1] = pV1->v[2] * pV2->v[0] - pV1->v[0] * pV2->v[2];
  tmp.v[2] = pV1->v[0] * pV2->v[1] - pV1->v[1] * pV2->v[0];
  memcpy(pOut, &tmp, sizeof(vec3));
  return pOut;
}

float vec3_Dot(const vec3 *pV1, const vec3 *pV2) {
  return pV1->v[0] * pV2->v[0] + 
    pV1->v[1] * pV2->v[1] + pV1->v[2] * pV2->v[2];
}

float vec4_Dot(const vec4 *pV1, const vec4 *pV2) {
  return 
    pV1->v[0] * pV2->v[0] + pV1->v[1] * pV2->v[1] + 
    pV1->v[2] * pV2->v[2] + pV1->v[3] * pV2->v[3];
}

float vec3_Length(const vec3 *pV) {
  return (float) sqrt( pV->v[0] * pV->v[0] + 
	       pV->v[1] * pV->v[1] + pV->v[2] * pV->v[2] );
}

float vec3_LengthSqr(const vec3 *pV) {
  return pV->v[0] * pV->v[0] + 
    pV->v[1] * pV->v[1] + pV->v[2] * pV->v[2];
}
    
vec3* vec3_Normalize(vec3 *pOut, const vec3 *pV) {
  float fLength = vec3_Length(pV);
  if(fLength != 0) {
    pOut->v[0] = pV->v[0] / fLength;
    pOut->v[1] = pV->v[1] / fLength;
    pOut->v[2] = pV->v[2] / fLength;
  }
  return pOut;
}

void vec4_Print(const vec4 *v) {
  int i;
  printf("[ ");
  for(i = 0; i < 4; i++) {
    printf("%.3f ", v->v[i]);
  }
  printf(" ]\n");
}

void vec3_Print(const vec3 *v) {
  int i;
  printf("[ ");
  for(i = 0; i < 3; i++) {
    printf("%.3f ", v->v[i]);
  }
  printf(" ]\n");
}

vec4* vec4_fromVec3(vec4 *pOut, const vec3 *pV) {
  memcpy(pOut, pV, sizeof(vec3));
  pOut->v[3] = 1;
  return pOut;
}

vec3* vec3_fromVec4(vec3 *pOut, const vec4 *pV) {
  if(pV->v[3] != 0) {
    pOut->v[0] = pV->v[0] / pV->v[3];
    pOut->v[1] = pV->v[1] / pV->v[3];
    pOut->v[2] = pV->v[2] / pV->v[3];
    return pOut;
  } else {
    memset(pOut, 0, sizeof(vec3));
    return NULL;
  }
}

vec3* vec3_Copy(vec3 *pOut, const vec3 *pV) {
	memcpy(pOut, pV, sizeof(vec3));
	return pOut;
}

vec3* vec3_Scale(vec3 *pOut, const vec3 *pV, float f) {
	pOut->v[0] = f * pV->v[0];
	pOut->v[1] = f * pV->v[1];
	pOut->v[2] = f * pV->v[2];
	return pOut;
}

unsigned int uintFromVec3(vec3 *pV) {
	return
		( ( (unsigned int)(pV->v[0] * 127.0f + 128.0f) ) << 16 ) +
		( ( (unsigned int)(pV->v[1] * 127.0f + 128.0f) ) << 8 ) +
		( ( (unsigned int)(pV->v[2] * 127.0f + 128.0f) ) << 0 );
}
		
vec3* vec3_Zero(vec3 *pV) {
	memset(pV, 0, sizeof(vec3));
	return pV;
}

vec3* vec3_TriNormalDirection(vec3* pOut, 
	 const vec3* pV1, 
	 const vec3* pV2, 
	 const vec3 *pV3)
{
	vec3 v1, v2;
	vec3_Sub(&v1, pV2, pV1);
	vec3_Sub(&v2, pV3, pV1);
	vec3_Cross(pOut, &v1, &v2);
	return pOut;
}

vec2* vec2_Copy(vec2 *pOut, const vec2 *pV) {
	memcpy(pOut, pV, sizeof(vec2));
	return pOut;
}

vec2* vec2_Orthogonal(vec2 *pOut, const vec2 *pV) {
	pOut->v[0] = pV->v[1];
	pOut->v[1] = - pV->v[0];
	return pOut;
}

vec2* vec2_Add(vec2 *pOut, const vec2 *pV1, const vec2 *pV2) {
	pOut->v[0] = pV1->v[0] + pV2->v[0];
	pOut->v[1] = pV1->v[1] + pV2->v[1];
	return pOut; 
}

vec2* vec2_Sub(vec2 *pOut, const vec2 *pV1, const vec2 *pV2) { 
	pOut->v[0] = pV1->v[0] - pV2->v[0];
	pOut->v[1] = pV1->v[1] - pV2->v[1];
	return pOut; 
}

vec2* vec2_Scale(vec2 *pV, float fScale) { 
	pV->v[0] *= fScale;
	pV->v[1] *= fScale;
	return pV; 
}

vec2* vec2_Normalize(vec2 *pV) {
	return vec2_Scale(pV, 1 / vec2_Length(pV));
}

float vec2_Dot(const vec2 *pV1, const vec2 *pV2) {
	return pV1->v[0] * pV2->v[0] + pV1->v[1] * pV2->v[1];
}

float vec2_Length(const vec2 *pV) {
	float fLength2 = (pV->v[0] * pV->v[0] + pV->v[1] * pV->v[1]);
	float l = (float) sqrt(fLength2);
	return l;
}

int vec3_Cmp(const vec3 *pV1, const vec3 *pV2)
{
	int i;
	for(i = 0; i < 3; i++)
	{
		if(pV1->v[i] < pV2->v[i]) return -1;
		if(pV1->v[i] > pV2->v[i]) return 1;
	}
	return 0;
}

int segment2_findT(float *t, const segment2 *s, const vec2 *v) {
	float epsilon = 0.001f;
	if( fabs(s->vDirection.v[0]) > fabs(s->vDirection.v[1]) ) {
		*t = (v->v[0] - s->vStart.v[0]) / s->vDirection.v[0];
		if( fabs(v->v[1] - (s->vStart.v[1] + *t * s->vDirection.v[1])) >
				epsilon ) {
			return 1;
		}
	} else {
		*t = (v->v[1] - s->vStart.v[1]) / s->vDirection.v[1];
		if( fabs(v->v[0] - (s->vStart.v[0] + *t * s->vDirection.v[0])) >
				epsilon ) {
			return 1;
		}
	}
	return 0;
}

vec2* segment2_IntersectParallel(vec2 *pOut, float *t1, float *t2,
												 const segment2 *s1, const segment2 *s2) {
	// if the lines don't overlap, return NULL
	// else find t2 for t1 == 0
	// if t2 in [0,1] return t2, t1 = 0
	// else find t1 for t2 == 0 and t2 == 1
	// if t1 < 0 return NULL (no intersection)
	// else return the smaller t1 and the corresponding t2

	vec2 v; float t;

	// if the lines don't overlap, return NULL
	// else find t2 for t1 == 0
	vec2_Copy(&v, &s1->vStart);
	if(segment2_findT(t2, s2, &v)) {
		// printf("[vector] are not collinear\n");
		return NULL;
	}
	
	// if t2 in [0,1] return t2, t1 = 0
	if(*t2 >= 0 && *t2 <= 1) {
		vec2_Copy(pOut, &s1->vStart);
		*t1 = 0;
		return pOut;
	}
	// else find t1 for t2 == 0 and t2 == 1
	vec2_Copy(&v, &s2->vStart);
	if(segment2_findT(t1, s1, &v))
		return NULL;
	// if t1 < 0 return NULL (no intersection)
	if(*t1 < 0) return NULL;
	vec2_Add(&v, &s2->vStart, &s2->vDirection);
	if(segment2_findT(&t, s1, &v))
		return NULL;
	nebu_assert(t >= 0);
	
	if(*t1 > 1 && t > 1)
		return NULL;
	// else return the smaller t1 and the corresponding t2
	if(t < *t1) {
		*t1 = t;
		*t2 = 1;
		vec2_Copy(pOut, &v);
	} else {
		*t2 = 0;
		vec2_Copy(pOut, &s2->vStart);
	}
	return pOut;
}

vec2* segment2_IntersectNonParallel(vec2 *pOut, float *t1, float *t2,
												 const segment2 *s1, const segment2 *s2) {
	vec3 v1, v2;
	vec3 tmp1, tmp2;
	vec3 vIntersection;
	
	// compute the homogenous line coordinates
	tmp1.v[0] = s1->vStart.v[0];
	tmp1.v[1] = s1->vStart.v[1];
	tmp1.v[2] = 1;
	tmp2.v[0] = s1->vStart.v[0] + s1->vDirection.v[0];
	tmp2.v[1] = s1->vStart.v[1] + s1->vDirection.v[1];
	tmp2.v[2] = 1;
	vec3_Cross(&v1, &tmp1, &tmp2);

	tmp1.v[0] = s2->vStart.v[0];
	tmp1.v[1] = s2->vStart.v[1];
	tmp1.v[2] = 1;
	tmp2.v[0] = s2->vStart.v[0] + s2->vDirection.v[0];
	tmp2.v[1] = s2->vStart.v[1] + s2->vDirection.v[1];
	tmp2.v[2] = 1;
	vec3_Cross(&v2, &tmp1, &tmp2);

	// compute the intersection in homogenous coordinates and
	// project back to 2d
	vec3_Cross(&vIntersection, &v1, &v2);
	pOut->v[0] = vIntersection.v[0] / vIntersection.v[2];
	pOut->v[1] = vIntersection.v[1] / vIntersection.v[2];
	
	// compute t1, t2
	if(fabs(s1->vDirection.v[0]) > fabs(s1->vDirection.v[1]))
		*t1 = (pOut->v[0] - s1->vStart.v[0]) / s1->vDirection.v[0];
	else
		*t1 = (pOut->v[1] - s1->vStart.v[1]) / s1->vDirection.v[1];
	if(fabs(s2->vDirection.v[0]) > fabs(s2->vDirection.v[1]))
		*t2 = (pOut->v[0] - s2->vStart.v[0]) / s2->vDirection.v[0];
	else
		*t2 = (pOut->v[1] - s2->vStart.v[1]) / s2->vDirection.v[1];
	
	return pOut;
}

vec2* segment2_Intersect(vec2 *pOut, float *t1, float *t2,
	const segment2 *s1, const segment2 *s2)
{
	// check if s1, s2 are parallel
	vec2 tmp;

	if( fabs(vec2_Dot(&s1->vDirection, 
							vec2_Orthogonal(&tmp, & s2->vDirection) )) < 0.1) {
		// printf("[vector] lines are parallel\n");
		pOut = segment2_IntersectParallel(pOut, t1, t2, s1, s2);
		if(!pOut) {
			*t1 = 0;
			*t2 = 0;
		}
	}	else {
		pOut = segment2_IntersectNonParallel(pOut, t1, t2, s1, s2);
	}
	return pOut;
}

float segment2_Length(const segment2 *s) {
	return vec2_Length(&s->vDirection);
}

void segment2_Scale(segment2* pSegment, float fScale)
{
	vec2_Scale(&pSegment->vStart, fScale);
	vec2_Scale(&pSegment->vDirection, fScale);
}

float box2_Width(const box2 *pBox) {
	return pBox->vMax.v[0] - pBox->vMin.v[0];
}

float box2_Height(const box2 *pBox) {
	return pBox->vMax.v[1] - pBox->vMin.v[1];
}

float box2_Diameter(const box2 *pBox) {
	vec2 v;
	vec2_Sub(&v, &pBox->vMax, &pBox->vMin);
	return vec2_Length(&v);
}

void box2_Center(vec2 *pOut, const box2* pBox)
{
	vec2_Add(pOut, &pBox->vMin, &pBox->vMax);
	vec2_Scale(pOut, 0.5f);
}

void box2_Init(box2 *pBox)
{
	pBox->vMin.v[0] = FLT_MAX;
	pBox->vMin.v[1] = FLT_MAX;
	pBox->vMax.v[0] = FLT_MIN;
	pBox->vMax.v[1] = FLT_MIN;
}

void box2_Extend(box2 *pBox, const vec2 *pV)
{
	if(pBox->vMin.v[0] > pV->v[0]) pBox->vMin.v[0] = pV->v[0];
	if(pBox->vMin.v[1] > pV->v[1]) pBox->vMin.v[1] = pV->v[1];
	if(pBox->vMax.v[0] < pV->v[0]) pBox->vMax.v[0] = pV->v[0];
	if(pBox->vMax.v[1] < pV->v[1]) pBox->vMax.v[1] = pV->v[1];
}

void box2_Scale(box2 *pBox, float fScale)
{
	vec2_Scale(&pBox->vMin, fScale);
	vec2_Scale(&pBox->vMax, fScale);
}

void box3_Init(box3 *pBox)
{
	pBox->vMin.v[0] = FLT_MAX;
	pBox->vMin.v[1] = FLT_MAX;
	pBox->vMin.v[2] = FLT_MAX;
	pBox->vMax.v[0] = FLT_MIN;
	pBox->vMax.v[1] = FLT_MIN;
	pBox->vMax.v[2] = FLT_MIN;
}

void box3_Compute(box3 *pBox, const vec3 *pVertices, int nVertices)
{
	int i, j;

	box3_Init(pBox);

	for(i = 0; i < nVertices; i++)
	{
		for(j = 0; j < 3; j++)
		{
			float f = pVertices[i].v[j];
			if(f < pBox->vMin.v[j])
				pBox->vMin.v[j] = f;
			if(f > pBox->vMax.v[j])
				pBox->vMax.v[j] = f;
		}
	}
}
