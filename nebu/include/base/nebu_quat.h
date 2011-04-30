#ifndef NEBU_QUAT_H
#define NEBU_QUAT_H

#include "base/nebu_matrix.h"

typedef struct {
  float x,y,z,w;
} nebu_Quat;

nebu_Matrix4D quat_RotationMatrix(const nebu_Quat *quat);
nebu_Quat quat_FromLookAt(const vec3 *vLookAt, const vec3 *vUp);
nebu_Quat quat_FromRotationMatrix(const nebu_Matrix4D *mat);

#endif
