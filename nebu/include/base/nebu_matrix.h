#ifndef NEBU_MATRIX_H
#define NEBU_MATRIX_H

#include "nebu_vector.h"

typedef struct {
  float m[16]; // column order
} matrix;
typedef matrix nebu_Matrix4D;

vec4* vec4_Transform(vec4* pOut, const vec4* pV, const matrix *pM);
vec3* vec3_Transform(vec3* pOut, const vec3* pV, const matrix *pM);

float matrixCofactor(const matrix *pM, int cf_row, int cf_col);
float matrixDeterminant(const matrix *pM);
matrix* matrixIdentity(matrix *pOut);
matrix* matrixInverse(matrix *pOut, float* pDet, const matrix *pM);
matrix* matrixMultiply(matrix *pOut, const matrix *pM1, const matrix *pM2);
matrix* matrixTranspose(matrix *pOut, const matrix *pM);
matrix* matrixAdjoint(matrix *pOut, float* pDet, const matrix *pM);

matrix* matrixRotationAxis(matrix *pOut, float fAngle, const vec3 *vAxis);
matrix* matrixTranslation(matrix *pOut, const vec3 *vTranslation);
matrix* matrixScale(matrix *pOut, const vec3 *vScale);
matrix* matrixCopy(matrix *pOut, const matrix *pM);

void matrixPrint(matrix *m);

#endif
