#include "base/nebu_quat.h"

#include <math.h>

typedef nebu_Quat quat;

quat quat_FromRotationMatrix(const nebu_Matrix4D *m) {
	quat q;
	float tr = m->m[0] + m->m[5] + m->m[10] + 1;
	float s;
	if(tr >= 1) {
		s = sqrt(tr);
		q.w = s / 2;
		q.x = (m->m[9] - m->m[6]) * 0.5 / s;
		q.y = (m->m[2] - m->m[8]) * 0.5 / s;
		q.z = (m->m[4] - m->m[1]) * 0.5 / s;
	} else {
		if((m->m[0] > m->m[5])&(m->m[0] > m->m[10])) { 
			s = sqrt( 1.0 + m->m[0] - m->m[5] - m->m[10] ) * 2; 
			q.x = 0.25 * s;
			q.y = (m->m[4] + m->m[1] ) / s; 
			q.z = (m->m[2] + m->m[8] ) / s; 
			q.w = (m->m[9] - m->m[6] ) / s;

		} else if ((m->m[5] > m->m[10])) { 
			s = sqrt( 1.0 + m->m[5] - m->m[0] - m->m[10] ) * 2; 
			q.x = (m->m[4] + m->m[1] ) / s; 
			q.y = 0.25 * s;
			q.z = (m->m[9] + m->m[6] ) / s; 
			q.w = (m->m[2] - m->m[8] ) / s;

		} else { 
			s = sqrt( 1.0 + m->m[10] - m->m[0] - m->m[5] ) * 2; 
			q.x = (m->m[2] + m->m[8] ) / s; 
			q.y = (m->m[6] + m->m[9] ) / s; 
			q.z = 0.25 * s;
			q.w = (m->m[4] - m->m[1] ) / s;
		} 
	}
	return q;
}

nebu_Matrix4D quat_RotationMatrix(const quat *q) {
	nebu_Matrix4D m;

	float x = q->x;
	float y = q->y;
	float z = q->z;
	float w = q->w;

	m.m[0] = 1 - 2 * (y * y + z * z);
	m.m[1] = 2 * (x * y - w * z);
	m.m[2] = 2 * (w  * y + x * z);
	m.m[3] = 0;

	m.m[4] = 2 * (x * y + w * z);
	m.m[5] = 1 - 2 *(x * x + z * z);
	m.m[6] = 2 * (y * z - w * x);
	m.m[7] = 0;

	m.m[8] = 2 * (x * z - w * y);
	m.m[9] = 2 * (y * z + w * x );
	m.m[10] = 1 - 2 * (x * x + y * y);
	m.m[11] = 0;

	m.m[12] = 0;
	m.m[13] = 0;
	m.m[14] = 0;
	m.m[15] = 1;
	
	return m;
}

quat quat_FromLookAt(const vec3 *vLookAt, const vec3 *vUp) {
	nebu_Matrix4D m;
	vec3 x, y, z;
	vec3_Cross(&x, vLookAt, vUp);
	vec3_Copy(&y, vUp);
	vec3_Copy(&z, vLookAt);

#define M(row,col)  m.m[col*4+row]
  M(0,0) = x.v[0];  M(0,1) = x.v[1];  M(0,2) = x.v[2];  M(0,3) = 0.0;
  M(1,0) = y.v[0];  M(1,1) = y.v[1];  M(1,2) = y.v[2];  M(1,3) = 0.0;
  M(2,0) = z.v[0];  M(2,1) = z.v[1];  M(2,2) = z.v[2];  M(2,3) = 0.0;
  M(3,0) = 0.0;   M(3,1) = 0.0;   M(3,2) = 0.0;   M(3,3) = 1.0;
#undef M
	
	return quat_FromRotationMatrix(&m);
}

