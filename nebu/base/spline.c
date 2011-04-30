#include "base/nebu_spline.h"

#include "base/nebu_assert.h"

#include "base/nebu_debug_memory.h"

// review: 64bit ok

nebu_Spline* nebu_Spline_Create(nebu_Spline_Type type, int dimension, int length)
{
	nebu_Spline *pSpline = (nebu_Spline*) malloc(sizeof(nebu_Spline));
	pSpline->type = type;
	pSpline->dimension = dimension;
	pSpline->length = length;
	pSpline->pVertices = (float*) malloc(dimension * length * sizeof(float));
	pSpline->pTangents = (float*) malloc(dimension * length * sizeof(float));
	return pSpline;
}
void nebu_Spline_Free(nebu_Spline* pSpline)
{
	free(pSpline->pTangents);
	free(pSpline->pVertices);
	free(pSpline);
}
void nebu_Spline_Eval(nebu_Spline* pSpline, int segment, float t, float **ppVertex, float **ppTangent)
{
	switch(pSpline->type)
	{
	// case nebu_Spline_Type::eCatmull3:
	case eCatmull3:
		nebu_Spline_Eval_Catmull3(pSpline, segment, t, ppVertex, ppTangent);
		break;
	default:
		nebu_assert(0);
	}
}

void nebu_Spline_Eval_Catmull3(nebu_Spline* pSpline, int segment, float t, float **ppVertex, float **ppTangent)
{
}
