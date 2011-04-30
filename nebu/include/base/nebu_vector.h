#ifndef NEBU_VECTOR_H
#define NEBU_VECTOR_H

typedef struct { float v[2]; } vec2;
typedef struct { float v[3]; } vec3;
typedef struct { float v[4]; } vec4;

typedef struct { vec2 vStart, vDirection; } segment2;

typedef struct { vec2 vMin, vMax; } box2;
typedef struct { vec3 vMin, vMax; } box3;

vec4* vec4_Add(vec4 *pOut, const vec4 *pV1, const vec4 *pV2);
vec4* vec4_Sub(vec4 *pOut, const vec4 *pV1, const vec4 *pV2);

vec3* vec3_Add(vec3 *pOut, const vec3 *pV1, const vec3 *pV2);
vec3* vec3_Sub(vec3 *pOut, const vec3 *pV1, const vec3 *pV2);
vec3* vec3_Cross(vec3 *pOut, const vec3 *pV1, const vec3 *pV2);

float vec4_Dot(const vec4 *pV1, const vec4 *pV2);

float vec3_Dot(const vec3 *pV1, const vec3 *pV2);

float vec3_Length(const vec3 *pV);
float vec3_LengthSqr(const vec3 *pV);
vec3* vec3_Normalize(vec3 *pOut, const vec3 *pV);

vec3* vec3_FromVec4(vec3 *pOut, const vec4 *pV);
vec4* vec4_FromVec3(vec4 *pOut, const vec3 *pV);

vec3* vec3_Copy(vec3 *pOut, const vec3 *pV);
vec3* vec3_Scale(vec3 *pOut, const vec3 *pV, float f);
int vec3_Cmp(const vec3 *pV1, const vec3 *pV2);

void vec4_Print(const vec4 *pV);
void vec3_Print(const vec3 *pV);
vec3* vec3_Zero(vec3 *pV);
vec3* vec3_TriNormalDirection(vec3* pOut, const vec3* pV1, const vec3* pV2, const vec3 *pV3);

unsigned int uintFromVec3(vec3 *pV);
// vec4* vec4Transform(vec4* pOut, const vec4* pV, const matrix16 *pM);

vec2* segment2_Intersect(vec2 *pOut, float *t1, float *t2,
												 const segment2 *s1, const segment2 *s2);
float segment2_Length(const segment2 *s);
void segment2_Scale(segment2* pSegment, float fScale);

vec2* vec2_Orthogonal(vec2 *pOut, const vec2 *pV);
vec2* vec2_Copy(vec2 *pOut, const vec2 *pV);
vec2* vec2_Add(vec2 *pOut, const vec2 *pV1, const vec2 *pV2);
vec2* vec2_Sub(vec2 *pOut, const vec2 *pV1, const vec2 *pV2);
vec2* vec2_Scale(vec2 *pV, float fScale);
vec2* vec2_Normalize(vec2 *pV);
float vec2_Dot(const vec2 *pV1, const vec2 *pV2);
float vec2_Length(const vec2 *pV);

float box2_Width(const box2* pBox);
float box2_Height(const box2* pBox);
float box2_Diameter(const box2* pBox);
void box2_Center(vec2 *pOut, const box2* pBox);
void box2_Init(box2 *pBox);
void box2_Extend(box2 *pBox, const vec2 *pV);
void box2_Scale(box2 *pBox, float fScale);
void box3_Compute(box3 *pBox, const vec3 *pVertices, int nVertices);
#endif
