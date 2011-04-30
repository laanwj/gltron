#include "video/nebu_geometry.h"
#include "base/nebu_math.h"
#include "base/nebu_assert.h"

#include <string.h>

#include "base/nebu_debug_memory.h"

nebu_Mesh* nebu_geom_CreateCylinder(float radius, float radius2, float height, int slices, int stacks)
{
	int i, j;
	// vertices:
	// - 2: top & bottom center
	// - 2 * slices: top & bottom edge (normals pointing along the axis)
	// - (stacks + 1) * slices: mantle (normalis pointing away from the axis)
	// triangles:
	// - 2 * slices: top & bottom cap
	// - 2 * slices * stacks: mantle
	// - 2 * slices: degenerate triangles between caps & mantle
	nebu_Mesh *pMesh =
		nebu_Mesh_Create(
			NEBU_MESH_POSITION | NEBU_MESH_NORMAL,
			// NEBU_MESH_POSITION | NEBU_MESH_NORMAL | NEBU_MESH_COLOR0,
			2 + slices * (stacks + 3), // vertexcount
			2 * slices + 2 * slices * stacks + 2 * slices); // triangles

	nebu_assert(stacks >= 1 && slices >= 3);

	/*
	// debug colors
	memset(pMesh->pVB->pColor0, 0xff, 4 * pMesh->pVB->nVertices);
	pMesh->pVB->pColor0[1] = 0xff0000ff;
	pMesh->pVB->pColor0[2] = 0xff00ff00;
	pMesh->pVB->pColor0[slices] = 0xffff0000;
	*/

	pMesh->pVB->pVertices[0] = 0;
	pMesh->pVB->pVertices[1] = 0;
	pMesh->pVB->pVertices[2] = 0;

	pMesh->pVB->pNormals[0] = 0;
	pMesh->pVB->pNormals[1] = 0;
	pMesh->pVB->pNormals[2] = -1;

	// top & bottom center
	pMesh->pVB->pVertices[3 * ((stacks + 3) * slices + 1) + 0] = 0;
	pMesh->pVB->pVertices[3 * ((stacks + 3) * slices + 1) + 1] = 0;
	pMesh->pVB->pVertices[3 * ((stacks + 3) * slices + 1) + 2] = height;

	pMesh->pVB->pNormals[3 * ((stacks + 3) * slices + 1) + 0] = 0;
	pMesh->pVB->pNormals[3 * ((stacks + 3) * slices + 1) + 1] = 0;
	pMesh->pVB->pNormals[3 * ((stacks + 3) * slices + 1) + 2] = 1;
	// top & bottom edge
	for(i = 0; i < slices; i++)
	{
		float angle = (float) i / slices * 2 * (float)M_PI;
		// bottom
		pMesh->pVB->pVertices[3 * (i + 1) + 0] = cosf(angle) * radius;
		pMesh->pVB->pVertices[3 * (i + 1) + 1] = sinf(angle) * radius;
		pMesh->pVB->pVertices[3 * (i + 1) + 2] = 0;

		pMesh->pVB->pNormals[3 * (i + 1) + 0] = 0;
		pMesh->pVB->pNormals[3 * (i + 1) + 1] = 0;
		pMesh->pVB->pNormals[3 * (i + 1) + 2] = -1;

		// top
		pMesh->pVB->pVertices[3 * ((stacks + 2) * slices + i + 1) + 0] = cosf(angle) * radius2;
		pMesh->pVB->pVertices[3 * ((stacks + 2) * slices + i + 1) + 1] = sinf(angle) * radius2;
		pMesh->pVB->pVertices[3 * ((stacks + 2) * slices + i + 1) + 2] = height;

		pMesh->pVB->pNormals[3 * ((stacks + 2) * slices + i + 1) + 0] = 0;
		pMesh->pVB->pNormals[3 * ((stacks + 2) * slices + i + 1) + 1] = 0;
		pMesh->pVB->pNormals[3 * ((stacks + 2) * slices + i + 1) + 2] = 1;
	}
	// mantle
	for(j = 0; j < stacks + 1; j++)
	{
		for(i = 0; i < slices; i++)
		{
			float t = (float) j / (float) stacks;
			float r = radius + t * (radius2 - radius);
			float angle = (float) i / slices * 2 * (float)M_PI;
			pMesh->pVB->pVertices[3 * ((j + 1) * slices + i + 1) + 0] = cosf(angle) * r;
			pMesh->pVB->pVertices[3 * ((j + 1) * slices + i + 1) + 1] = sinf(angle) * r;
			pMesh->pVB->pVertices[3 * ((j + 1) * slices + i + 1) + 2] = j * (height / stacks);

			pMesh->pVB->pNormals[3 * ((j + 1) * slices + i + 1) + 0] = cosf(angle);
			pMesh->pVB->pNormals[3 * ((j + 1) * slices + i + 1) + 1] = sinf(angle);
			pMesh->pVB->pNormals[3 * ((j + 1) * slices + i + 1) + 2] = 0;
		}
	}

	nebu_assert(pMesh->pIB->nPrimitivesPerIndex == 3);

	memset(pMesh->pIB->pIndices, 0, pMesh->pIB->nPrimitives * pMesh->pIB->nPrimitivesPerIndex * sizeof(int));

	for(i = 0; i < slices; i++)
	{
		// bottom
		pMesh->pIB->pIndices[3 * i + 0] = 0;
		pMesh->pIB->pIndices[3 * i + 1] = 1 + i;
		pMesh->pIB->pIndices[3 * i + 2] = 1 + ((i + 1) % slices);

		// top
		pMesh->pIB->pIndices[3 * slices + 6 * slices * (stacks + 1) + 3 * i + 0] = slices * (stacks + 3) + 1;
		pMesh->pIB->pIndices[3 * slices + 6 * slices * (stacks + 1) + 3 * i + 1] = 1 + slices * (stacks + 2) + i;
		pMesh->pIB->pIndices[3 * slices + 6 * slices * (stacks + 1) + 3 * i + 2] = 1 + slices * (stacks + 2) + ((i + 1) % slices);
	}

	for(j = 0; j < stacks; j++)
	{
		for(i = 0; i < slices; i++)
		{
			// mantle			
			pMesh->pIB->pIndices[3 * (slices + (j + 1) * 2 * slices + 2 * i) + 0] = 1 + ((j + 1) * slices) + i;
			pMesh->pIB->pIndices[3 * (slices + (j + 1) * 2 * slices + 2 * i) + 1] = 1 + ((j + 1) * slices) + ((i + 1) % slices);
			pMesh->pIB->pIndices[3 * (slices + (j + 1) * 2 * slices + 2 * i) + 2] = 1 + ((j + 2) * slices) + i;

			pMesh->pIB->pIndices[3 * (slices + (j + 1) * 2 * slices + 2 * i) + 3] = 1 + ((j + 1)  * slices) + ((i + 1) % slices);
			pMesh->pIB->pIndices[3 * (slices + (j + 1) * 2 * slices + 2 * i) + 4] = 1 + ((j + 2) * slices) + ((i + 1) % slices);
			pMesh->pIB->pIndices[3 * (slices + (j + 1) * 2 * slices + 2 * i) + 5] = 1 + ((j + 2) * slices) + i;
		}
	}

	for(i = 0; i < slices; i++)
	{
		pMesh->pIB->pIndices[3 * (2 + 2 * stacks) * slices + i + 0] = 1 + i;
		pMesh->pIB->pIndices[3 * (2 + 2 * stacks) * slices + i + 1] = 1 + ((i + 1) % slices);
		pMesh->pIB->pIndices[3 * (2 + 2 * stacks) * slices + i + 2] = 1 + i + slices;

		pMesh->pIB->pIndices[3 * (2 + 2 * stacks + 1) * slices + i + 0] = 2 + (stacks + 1) * slices + ((i + 1) % slices);
		pMesh->pIB->pIndices[3 * (2 + 2 * stacks + 1) * slices + i + 1] = 2 + (stacks + 2) * slices + i + 1;
		pMesh->pIB->pIndices[3 * (2 + 2 * stacks + 1) * slices + i + 2] = 2 + (stacks + 2) * slices + ((i + 1) % slices);
	}
	return pMesh;
}

