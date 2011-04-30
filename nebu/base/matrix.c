#include "base/nebu_math.h"
#include "base/nebu_matrix.h"

#include "base/nebu_assert.h"
#include <stdio.h>
#include <string.h>

#include "base/nebu_debug_memory.h"

// review: 64bit ok

vec4* vec4_Transform(vec4* pOut, const vec4* pV, const matrix *pM) {
  vec4 tmp;
  tmp.v[0] = pM->m[0] * pV->v[0] + pM->m[4] * pV->v[1] + 
    pM->m[8] * pV->v[2] + pM->m[12] * pV->v[3];
  tmp.v[1] = pM->m[1] * pV->v[0] + pM->m[5] * pV->v[1] + 
    pM->m[9] * pV->v[2] + pM->m[13] * pV->v[3];
  tmp.v[2] = pM->m[2] * pV->v[0] + pM->m[6] * pV->v[1] + 
    pM->m[10] * pV->v[2] + pM->m[14] * pV->v[3];
  tmp.v[3] = pM->m[3] * pV->v[0] + pM->m[7] * pV->v[1] + 
    pM->m[11] * pV->v[2] + pM->m[15] * pV->v[3];
  memcpy(pOut, &tmp, sizeof(vec4));
  return pOut;
}

vec3* vec3_Transform(vec3* pOut, const vec3* pV, const matrix *pM) {
  vec3 tmp;
  tmp.v[0] = pM->m[0] * pV->v[0] + pM->m[4] * pV->v[1] + pM->m[8] * pV->v[2];
  tmp.v[1] = pM->m[1] * pV->v[0] + pM->m[5] * pV->v[1] + pM->m[9] * pV->v[2];
  tmp.v[2] = pM->m[2] * pV->v[0] + pM->m[6] * pV->v[1] + pM->m[10] * pV->v[2];
  memcpy(pOut, &tmp, sizeof(vec3));
  return pOut;
}

matrix* matrixIdentity(matrix *pOut) {
  memset(pOut, 0, sizeof(matrix));
  pOut->m[0] = pOut->m[5] = pOut->m[10] = pOut->m[15] = 1;
  return pOut;
}

float matrixCofactor(const matrix *pM, int cf_row, int cf_col) {
  int row, col;
  float prod[6] = { 1, 1, 1, -1, -1, -1 };
  int r = 0;
  float cf;
  for(row = 0; row < 4; row++) {
    if(row != cf_row) {
      int c = 0;
      for(col = 0; col < 4; col++) {
	if(col != cf_col) {
	  prod[ ((c - r + 3) % 3) + 0 ] *= pM->m[ row + 4 * col ];
	  prod[ ((c + r + 0) % 3) + 3 ] *= pM->m[ row + 4 * col ];
	  c++;
	}
      }
      r++;
    }
  }
  cf = prod[0] + prod[1] + prod[2] + prod[3] + prod[4] + prod[5];
  // printf("cf(%d,%d): %.3f\n", cf_row, cf_col, cf);
  return cf;
}

float matrixDeterminant(const matrix *pM) {
  float det = 
    + pM->m[0] * matrixCofactor(pM, 0, 0)
    - pM->m[4] * matrixCofactor(pM, 0, 1)
    + pM->m[8] * matrixCofactor(pM, 0, 2)
    - pM->m[12] * matrixCofactor(pM, 0, 3);
  return det;
}
 
matrix* matrixAdjoint(matrix *pOut, float* pDet, const matrix *pM) {
  matrix tmp;
  int col, row;
  int sign = 1;
  float det = 0;
  for(row = 0; row < 4; row++) {
    for(col = 0; col < 4; col++) {
      float cf = matrixCofactor(pM, row, col);
      // tmp.m[row + 4 * col] = sign * cf;
      // warum hier zeile und spalte tauschen?
      tmp.m[col + 4 * row] = sign * cf;
      if(row == 0)
	det += sign * cf * pM->m[col * 4];
      sign *= -1;
    }
    sign *= -1; // warum hier nochmal wechseln?
  }
  if(pDet)
    *pDet = det;

  memcpy(pOut, &tmp, sizeof(matrix));
  return pOut;
}

matrix* matrixInverse(matrix *pOut, float* pDet, const matrix *pM) {
  matrix adjoint;
  float det;
  int i;
  matrixAdjoint(&adjoint, &det, pM);
  if(pDet)
    *pDet = det;
  if(det == 0)
    return NULL;

  for(i = 0; i < 16; i++) {
    pOut->m[i] = adjoint.m[i] / det;
  }

  return pOut;
}

matrix* matrixMultiply(matrix *pOut, const matrix *pM1, const matrix *pM2) {
  int row, col;
  matrix tmp;
  for(row = 0; row < 4; row++) {
    for(col = 0; col < 4; col++) {
      tmp.m[row + col * 4] = 
	pM1->m[row + 0] * pM2->m[ 0 + col * 4] +
	pM1->m[row + 4] * pM2->m[ 1 + col * 4] +
	pM1->m[row + 8] * pM2->m[ 2 + col * 4] +
	pM1->m[row + 12] * pM2->m[ 3 + col * 4];
    }
  }
	
  memcpy(pOut, &tmp, sizeof(matrix));
  return pOut;
}

matrix* matrixTranspose(matrix *pOut, const matrix *pM) {
  matrix tmp;
  int i;
  for(i = 0; i < 16; i++) {
    tmp.m[i] = pM->m[ i / 4 + (i % 4) * 4 ];
  }
  memcpy(pOut, &tmp, sizeof(matrix));
  return pOut;
}

void matrixPrint(matrix *m) {
  int row, col;
  printf("[ ");
  for(row = 0; row < 4; row++) {
    for(col = 0; col < 4; col++) {
      printf("%.3f ", m->m[row + col * 4]);
    }
    printf("; \n");
  }
  printf(" ]\n");
}

matrix* matrixRotationAxis(matrix *pOut, float fAngle, const vec3 *vAxis)
{
	float cosa = (float)cos(fAngle);
	float sina = (float)sin(fAngle);
	float x = vAxis->v[0], y = vAxis->v[1], z = vAxis->v[2];
	float *m = pOut->m;
	
	m[0] = cosa + (1 - cosa) * x * x;
	m[1] = (1 - cosa) * x * y + sina * z;
	m[2] = (1 - cosa) * x * z - sina * y;
	m[3] = 0;
	m[4] = (1 - cosa) * x * y - z * sina;
	m[5] = cosa + (1 - cosa) * y * y;
	m[6] = (1 - cosa) * y * z + x * sina;
	m[7] = 0;
	m[8] = (1 - cosa) * x * z + sina * y;
	m[9] = (1 - cosa) * y * z - sina * x;
	m[10] = cosa + (1 - cosa) * z * z;
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;

	return pOut;
}

matrix* matrixTranslation(matrix *pOut, const vec3 *vTranslation)
{
	float *m = pOut->m;
	matrixIdentity(pOut);
	m[12] = vTranslation->v[0];
	m[13] = vTranslation->v[1];
	m[14] = vTranslation->v[2];

	return pOut;
}
	
matrix* matrixScale(matrix *pOut, const vec3 *vScale)
{
	float *m = pOut->m;
	memset(pOut, 0, sizeof(matrix));
	m[0] = vScale->v[0];
	m[5] = vScale->v[1];
	m[10] = vScale->v[2];
	m[15] = 1;
	
	return pOut;
}

matrix* matrixCopy(matrix *pOut, const matrix *pM)
{
	memcpy(pOut, pM, sizeof(matrix));
	return pOut;
}