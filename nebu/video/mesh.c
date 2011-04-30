#include "base/nebu_math.h"
#include "video/nebu_mesh.h"
#include "Nebu_video.h"
#include "base/nebu_assert.h"
#include "video/nebu_renderer_gl.h"

#include <string.h>

#include "base/nebu_debug_memory.h"

static void normalize(float *v)
{
	float fSqrLength = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	if(fSqrLength != 0)
	{
		float fLength = (float) sqrt(fSqrLength); 
		v[0] /= fLength;
		v[1] /= fLength;
		v[2] /= fLength;
	}
}

void nebu_Mesh_VB_Enable(nebu_Mesh_VB *pVB)
{
	int i;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pVB->pVertices);

	for(i = 0; i < NEBU_MESH_TEXCOORD_MAXCOUNT; i++)
	{
		if(pVB->vertexformat & (NEBU_MESH_TEXCOORD0 << i)) {
			glClientActiveTexture(GL_TEXTURE0_ARB + i);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			if(pVB->pTexCoords[i])
				glTexCoordPointer(2, GL_FLOAT, 0, pVB->pTexCoords[i]);
			else
				glTexCoordPointer(2, GL_FLOAT, 0, pVB->pTexCoords[0]);
		}
	}

	if(pVB->vertexformat & NEBU_MESH_NORMAL) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, pVB->pNormals);
	}
	if(pVB->vertexformat & NEBU_MESH_COLOR0)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, pVB->pColor0);
	}
	if(pVB->vertexformat & NEBU_MESH_COLOR1)
	{
		glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
		glSecondaryColorPointer(4, GL_UNSIGNED_BYTE, 0, pVB->pColor1);
	}
}

void nebu_Mesh_VB_Disable(nebu_Mesh_VB *pVB)
{
	int i;

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
	for(i = 0; i < NEBU_MESH_TEXCOORD_MAXCOUNT; i++)
	{
		if(pVB->vertexformat & (NEBU_MESH_TEXCOORD0 << i)) {
			glClientActiveTexture(GL_TEXTURE0_ARB + i);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
	glClientActiveTexture(GL_TEXTURE0_ARB);
}

int nebu_Mesh_Validate(nebu_Mesh *pMesh)
{
	int i;
	for(i = 0; i < pMesh->pIB->nPrimitives * pMesh->pIB->nPrimitivesPerIndex; i++)
	{
		if(pMesh->pIB->pIndices[i] > pMesh->pVB->nVertices - 1)
			return 1;
	}
	return 0;
}

void nebu_Mesh_DrawGeometry(nebu_Mesh *pMesh)
{
	nebu_assert(!nebu_Mesh_Validate(pMesh));

	nebu_Mesh_VB_Enable(pMesh->pVB);
	glDrawElements(GL_TRIANGLES, 3 * pMesh->pIB->nPrimitives, GL_UNSIGNED_INT, pMesh->pIB->pIndices);
	nebu_Mesh_VB_Disable(pMesh->pVB);
}

void nebu_Mesh_ComputeTriangleNormal(nebu_Mesh *pMesh, int triangle, float* normal)
{
	float v1[3], v2[3];
	int a, b, c, i;

	a = pMesh->pIB->pIndices[3 * triangle + 0];
	b = pMesh->pIB->pIndices[3 * triangle + 1];
	c = pMesh->pIB->pIndices[3 * triangle + 2];
	for(i = 0; i < 3; i++)
	{
		v1[i] = pMesh->pVB->pVertices[3 * b + i] - pMesh->pVB->pVertices[3 * a + i];
		v2[i] = pMesh->pVB->pVertices[3 * c + i] - pMesh->pVB->pVertices[3 * a + i];
	}
	normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
	normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
	normal[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void nebu_Mesh_ComputeNormals(nebu_Mesh *pMesh)
{
	int i, j;

	if(!pMesh->pVB->pNormals)
		pMesh->pVB->pNormals = (float*) malloc( 3 * sizeof(float) * pMesh->pVB->nVertices );
	pMesh->pVB->vertexformat |= NEBU_MESH_NORMAL;

	memset(pMesh->pVB->pNormals, 0, 3 * sizeof(float) * pMesh->pVB->nVertices);
	for(i = 0; i < pMesh->pIB->nPrimitives; i++)
	{
		float normal[3];
		nebu_Mesh_ComputeTriangleNormal(pMesh, i, normal);

		for(j = 0; j < 3; j++)
		{
			int vertex = pMesh->pIB->pIndices[3 * i + j];
			pMesh->pVB->pNormals[3 * vertex + 0] += normal[0];
			pMesh->pVB->pNormals[3 * vertex + 1] += normal[1];
			pMesh->pVB->pNormals[3 * vertex + 2] += normal[2];
		}
	}
	for(i = 0; i < pMesh->pVB->nVertices; i++)
	{
		normalize(pMesh->pVB->pNormals + 3 * i);
	}
}

void nebu_Mesh_VB_Scale(nebu_Mesh_VB *pVB, float fScale)
{
	int i, j;
	for(i = 0; i < pVB->nVertices; i++)
	{
		for(j = 0; j < 3; j++)
		{
			pVB->pVertices[3 * i + j] *= fScale;
		}
	}
}

nebu_Mesh_IB* nebu_Mesh_IB_Create(int nPrimitives, int nPrimitivesPerIndex)
{
	nebu_Mesh_IB *pIB = (nebu_Mesh_IB*) malloc(sizeof(nebu_Mesh_IB));
	pIB->pIndices = (int*) malloc(nPrimitives * nPrimitivesPerIndex * sizeof(int));
	pIB->nPrimitives = nPrimitives;
	pIB->nPrimitivesPerIndex = nPrimitivesPerIndex;
	return pIB;
}

void nebu_Mesh_IB_Free(nebu_Mesh_IB *pIB)
{
	free(pIB->pIndices);
	free(pIB);
}

void nebu_Mesh_VB_Free(nebu_Mesh_VB *pVB)
{
	int i;

	if(pVB->vertexformat & NEBU_MESH_POSITION)
		free(pVB->pVertices);
	if(pVB->vertexformat & NEBU_MESH_NORMAL)
		free(pVB->pNormals);
	if(pVB->vertexformat & NEBU_MESH_COLOR0)
		free(pVB->pColor0);
	if(pVB->vertexformat & NEBU_MESH_COLOR1)
		free(pVB->pColor1);

	for(i = 0; i < NEBU_MESH_TEXCOORD_MAXCOUNT; i++)
	{
		if(pVB->vertexformat & (NEBU_MESH_TEXCOORD0 << i) &&
			pVB->pTexCoords[i])
			free(pVB->pTexCoords[i]);
	}
	free(pVB);
}

void nebu_Mesh_Free(nebu_Mesh *pMesh)
{
	nebu_Mesh_IB_Free(pMesh->pIB);
	nebu_Mesh_VB_Free(pMesh->pVB);
	free(pMesh);
}

nebu_Mesh_VB* nebu_Mesh_VB_Create(int flags, int nVertices)
{
	// TODO: add error checking
	int i;

	nebu_Mesh_VB* pVB = (nebu_Mesh_VB*) malloc(sizeof(nebu_Mesh_VB));
	pVB->nVertices = nVertices;
	pVB->vertexformat = flags;

	if(flags & NEBU_MESH_POSITION)
		pVB->pVertices = (float*) malloc(3 * sizeof(float) * nVertices);
	else
		pVB->pVertices = NULL;

	if(flags & NEBU_MESH_NORMAL)
		pVB->pNormals = (float*) malloc(3 * sizeof(float) * nVertices);
	else
		pVB->pNormals = NULL;

	if(flags & NEBU_MESH_COLOR0)
	{
		nebu_assert(sizeof(int) == 4);
		pVB->pColor0 = (int*) malloc( sizeof(int) * nVertices);
	}
	else
		pVB->pColor0 = NULL;

	if(flags & NEBU_MESH_COLOR1)
	{
		nebu_assert(sizeof(int) == 4);
		pVB->pColor1 = (int*) malloc( sizeof(int) * nVertices);
	}
	else
		pVB->pColor1 = NULL;

	for(i = 0; i < NEBU_MESH_TEXCOORD_MAXCOUNT; i++)
	{
		if(flags & (NEBU_MESH_TEXCOORD0 << i))
			pVB->pTexCoords[i] = (float*) malloc(2 * sizeof(float) * nVertices);
		else
			pVB->pTexCoords[i] = NULL;
	}
	return pVB;
}

nebu_Mesh* nebu_Mesh_Create(int flags, int nVertices, int nTriangles)
{
	nebu_Mesh *pMesh = (nebu_Mesh*) malloc(sizeof(nebu_Mesh));
	pMesh->pVB = nebu_Mesh_VB_Create(flags, nVertices);
	pMesh->pIB = nebu_Mesh_IB_Create(nTriangles, 3);
	return pMesh;
}

void nebu_Mesh_VB_ComputeBBox(nebu_Mesh_VB *pVB, box3* box)
{
	box3_Compute(box, (vec3*)pVB->pVertices, pVB->nVertices);
}
