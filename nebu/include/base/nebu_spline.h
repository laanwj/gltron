#ifndef NEBU_SPLINE_H
#define NEBU_SPLINE_H

typedef enum nebu_Spline_Type {
	eCatmull3 = 0 // interpolating hermite spline, order of 3
} nebu_Spline_Type;

typedef struct nebu_Spline {
	nebu_Spline_Type type;
	int dimension;
	int length;
	float *pVertices;
	float *pTangents;
} nebu_Spline;

nebu_Spline* nebu_Spline_Create(nebu_Spline_Type type, int dimension, int length);
void nebu_Spline_Free(nebu_Spline* pSpline);
void nebu_Spline_Eval(nebu_Spline* pSpline, int segment, float t, float **ppVertex, float **ppTangent);
void nebu_Spline_Eval_Catmull3(nebu_Spline* pSpline, int segment, float t, float **ppVertex, float **ppTangent);

#endif