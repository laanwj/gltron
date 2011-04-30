#ifndef _NEBU_GEOMETRY_H
#define _NEBU_GEOMETRY_H

#include "video/nebu_Mesh.h"

nebu_Mesh* nebu_geom_CreateCylinder(float radius, float radius2, float height, int slices, int stacks);

#endif