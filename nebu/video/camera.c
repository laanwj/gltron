#include "video/nebu_camera.h"
#include "video/nebu_gl.h"
#include "base/nebu_math.h"
#include "base/nebu_matrix.h"

#include <stdio.h>
#include "base/nebu_assert.h"

#include "base/nebu_debug_memory.h"


static void doLookAt(const float *cam, const float *target, const float *up) {
  matrix m;
  vec3 x, y, z;

	vec3_Sub(&z, (vec3*) cam, (vec3*) target);
	vec3_Normalize(&z, &z);
	vec3_Cross(&x, (vec3*) up, &z);
	vec3_Normalize(&x, &x);
	vec3_Cross(&y, &z, &x);
	vec3_Normalize(&y, &y);

#define M(row,col)  m.m[col*4+row]
  M(0,0) = x.v[0];  M(0,1) = x.v[1];  M(0,2) = x.v[2];  M(0,3) = 0.0;
  M(1,0) = y.v[0];  M(1,1) = y.v[1];  M(1,2) = y.v[2];  M(1,3) = 0.0;
  M(2,0) = z.v[0];  M(2,1) = z.v[1];  M(2,2) = z.v[2];  M(2,3) = 0.0;
  M(3,0) = 0.0;   M(3,1) = 0.0;   M(3,2) = 0.0;   M(3,3) = 1.0;
#undef M
  glMultMatrixf( m.m );

  /* Translate Eye to Origin */
  glTranslatef( -cam[0], -cam[1], -cam[2]);
}

void nebu_Camera_GetRotationMatrix(nebu_Matrix4D *matrix, 
																	 const nebu_Camera *pCamera) {
  vec3 x, y, z;

	vec3_Sub(&z, &pCamera->vEye, &pCamera->vLookAt);
	vec3_Normalize(&z, &z);
	vec3_Cross(&x, &pCamera->vUp, &z);
	vec3_Normalize(&x, &x);
	vec3_Cross(&y, &z, &x);
	vec3_Normalize(&y, &y);

#define M(row,col)  matrix->m[col*4+row]
  M(0,0) = x.v[0];  M(0,1) = x.v[1];  M(0,2) = x.v[2];  M(0,3) = 0.0;
  M(1,0) = y.v[0];  M(1,1) = y.v[1];  M(1,2) = y.v[2];  M(1,3) = 0.0;
  M(2,0) = z.v[0];  M(2,1) = z.v[1];  M(2,2) = z.v[2];  M(2,3) = 0.0;
  M(3,0) = 0.0;   M(3,1) = 0.0;   M(3,2) = 0.0;   M(3,3) = 1.0;
#undef M
	return;
}

void nebu_Camera_LookAt(const nebu_Camera *pCamera) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	doLookAt(pCamera->vEye.v, pCamera->vLookAt.v , pCamera->vUp.v);
}

void nebu_Camera_Zoom(nebu_Camera *pCamera, float d) {
	vec3 v;
	vec3_Sub(&v, &pCamera->vLookAt, &pCamera->vEye);
	vec3_Normalize(&v, &v);
	vec3_Scale(&v, &v, d);
	vec3_Add(&pCamera->vEye, &pCamera->vEye, &v);
}

void nebu_Camera_Roll(nebu_Camera *pCamera, float d) {
	vec3 v;
	matrix m;
	vec3_Sub(&v, &pCamera->vLookAt, &pCamera->vEye);
	vec3_Normalize(&v, &v);
	matrixRotationAxis(&m, d * 2 * (float)M_PI / 360.0f, &v);
	vec3_Transform(&pCamera->vUp, &pCamera->vUp, &m);
}

/*!
	\param pCamera
	\param flags
	\param dx
	\param dy

	Rotates the camera around the target specified by flags
*/

void nebu_Camera_Rotate(nebu_Camera *pCamera, int flags,
	float dx, float dy)
{
	// adjust up vector, so that it is orthogonal to
	// view direction
	vec3 vDiff, vView, vRight, vUp;
	vec3 vdx, vdy;
	vec3 *pvCenter, *pvMoving;

	vec3 vBackup;

	switch(flags & NEBU_CAMERA_ROTATE_MASK)
	{
	case NEBU_CAMERA_ROTATE_AROUND_EYE:
		pvCenter = &pCamera->vEye;
		pvMoving = &pCamera->vLookAt;
		break;
	case NEBU_CAMERA_ROTATE_AROUND_LOOKAT:
		pvCenter = &pCamera->vLookAt;
		pvMoving = &pCamera->vEye;
		break;
	default:
		nebu_assert(0);
		return;
	}
	vec3_Copy(&vBackup, pvCenter);
	
	vec3_Sub(&vDiff, pvCenter, pvMoving);
	vec3_Normalize(&vView, &vDiff);
	vec3_Cross(&vRight, &pCamera->vUp, &vView);
	vec3_Normalize(&vRight, &vRight);
	vec3_Cross(&vUp, &vView, &vRight);
	vec3_Normalize(&vUp, &vUp);
	
	// horizontal movement (dx):
	if(dx == 0) {
		vec3_Zero(&vdx);
	} else {
		// rotate moving around up vector through center point
		vec3 v = vDiff;
		float fAngle = dx * 2 * (float)M_PI / 360.0f;
		matrix matRotation;
		matrixRotationAxis(&matRotation, fAngle, &vUp);
		vec3_Transform(&v, &vDiff, &matRotation);
		vec3_Sub(&vdx, &v, &vDiff);
	}

	// vertical movement (dy):
	if(dy == 0) {
		vec3_Zero(&vdy);
	} else {
		// rotate eye point around up vector through lookAt point
		vec3 v = vDiff;
		float fAngle = dy * 2 * (float)M_PI / 360.0f;
		matrix matRotation;
		matrixRotationAxis(&matRotation, fAngle, &vRight);
		vec3_Transform(&v, &vDiff, &matRotation);
		vec3_Sub(&vdy, &v, &vDiff);

		matrixTranspose(&matRotation, &matRotation);
		if(!(flags & NEBU_CAMERA_FIXED_UP))
			vec3_Transform(&pCamera->vUp, &pCamera->vUp, &matRotation);
	}

	{
		vec3 v;
		vec3_Add(&v, &vdx, &vdy);
		vec3_Add(&v, &v, pvMoving);
		vec3_Sub(&v, &v, pvCenter);
		vec3_Normalize(&v, &v);
		// printf("up dot view: %.4f\n", - vec3_Dot(&v, &pCamera->vUp));
		vec3_Scale(&v, &v, vec3_Length(&vDiff));
		vec3_Add(pvMoving, &v, pvCenter);

	}
	nebu_assert( !memcmp( &vBackup, pvCenter, sizeof(vec3) ) );
}

void nebu_Camera_Print(const nebu_Camera *pCamera) {
	printf("LookAt: ");
	vec3_Print(&pCamera->vLookAt);
	printf("\n");
	printf("Eye: ");
	vec3_Print(&pCamera->vEye);
	printf("\n");
	printf("Up: ");
	vec3_Print(&pCamera->vUp);
	printf("\n");
}

nebu_Camera* nebu_Camera_Create(void)
{
	nebu_Camera *pCamera = NULL;
	
	pCamera = (nebu_Camera*) malloc(sizeof(nebu_Camera));
	if(!pCamera) return NULL;

	{
		vec3 vEye = { { 0, 0, 0 } };
		vec3 vUp = { { 0, 1, 0 } };
		vec3 vLookAt = { { 0, 0, -1 } };
		pCamera->vEye = vEye;
		pCamera->vUp = vUp;
		pCamera->vLookAt = vLookAt;
	}
	return pCamera;
}

void nebu_Camera_SetupEyeUpLookAt(nebu_Camera *pCamera, vec3 *pEye, vec3 *pUp, vec3 *pLookAt)
{
	pCamera->vEye = *pEye;
	pCamera->vUp = *pUp;
	pCamera->vLookAt = *pLookAt;
}

void nebu_Camera_Slide(nebu_Camera *pCamera, float dx, float dy, float dz)
{
	vec3 vLook, vUp, vRight, vMovement;
	vec3_Sub(&vLook, &pCamera->vLookAt, &pCamera->vEye);
	vec3_Cross(&vRight, &vLook, &pCamera->vUp);
	vec3_Cross(&vUp, &vRight, &vLook);

	vec3_Normalize(&vUp, &vUp);
	vec3_Normalize(&vRight, &vRight);
	vec3_Normalize(&vLook, &vLook);
	vec3_Scale(&vUp, &vUp, dy);
	vec3_Scale(&vRight, &vRight, dx);
	vec3_Scale(&vLook, &vLook, dz);
	vec3_Add(&vMovement, &vUp, &vRight);
	vec3_Add(&vMovement, &vMovement, &vLook); 
	vec3_Add(&pCamera->vEye, &pCamera->vEye, &vMovement);
	vec3_Add(&pCamera->vLookAt, &pCamera->vLookAt, &vMovement);
}

float nebu_Camera_GetDistance(const nebu_Camera *pCamera)
{
	vec3 v;
	vec3_Sub(&v, &pCamera->vLookAt, &pCamera->vEye);
	return vec3_Length(&v);
}
