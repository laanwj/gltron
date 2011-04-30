#include "video/nebu_mesh_3ds.h"
#include "video/nebu_mesh.h"

#include <lib3ds/file.h>                        
#include <lib3ds/camera.h>
#include <lib3ds/mesh.h>
#include <lib3ds/node.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <lib3ds/light.h>

#include <string.h>

#include "base/nebu_debug_memory.h"

static void countVerticesAndTriangles(nebu_Mesh_3ds_File *pFile, 
	int *pnVertices, int *pnTriangles, int *pnTexCoords)
{
	Lib3dsMesh *p;

	*pnVertices = *pnTriangles = *pnTexCoords = 0;

	for(p = pFile->meshes; p != NULL; p = p->next)
	{
		lib3ds_mesh_dump(p);
		printf("----------------------\n");
		*pnVertices += p->points;
		*pnTriangles += p->faces;
		*pnTexCoords += p->texels;
	}
}

static void addToMesh(nebu_Mesh *pMesh, int *pCurVertex, int *pCurTri, Lib3dsMesh *pLib3dsMesh)
{
	unsigned int i, j;

	// add vertices & triangles of this node to mesh
	memcpy(pMesh->pVB->pVertices + 3 * *pCurVertex, pLib3dsMesh->pointL, pLib3dsMesh->points * 3 * sizeof(float));

	if(pMesh->pVB->vertexformat & NEBU_MESH_TEXCOORD0)
		memcpy(pMesh->pVB->pTexCoords[0] + 2 * *pCurVertex, pLib3dsMesh->texelL, pLib3dsMesh->texels * 2 * sizeof(float));

	for(i = 0; i < pLib3dsMesh->faces; i++)
	{
		for(j = 0; j < 3; j++)
		{
			pMesh->pIB->pIndices[3 * (i + *pCurTri) + j] = pLib3dsMesh->faceL[i].points[j] + *pCurVertex;
		}
	}
	*pCurVertex += pLib3dsMesh->points;
	*pCurTri += pLib3dsMesh->faces;
}


nebu_Mesh* nebu_Mesh_3ds_GetFromFile(nebu_Mesh_3ds_File* pFile)
{
	nebu_Mesh* pMesh = NULL;

	int nTriangles = 0;
	int nVertices = 0;
	int nTexCoords = 0;

	int vertexFlags = NEBU_MESH_POSITION;
	countVerticesAndTriangles(pFile, &nVertices, &nTriangles, &nTexCoords);
	if(nVertices == 0 || nTriangles == 0) {
		return NULL;
	}

	// Check if we actually have texture coordinates in the file.
	// FIXME: Actually, we could have multpliple meshes that have either texcoords or not.
	// Oh god, this is messy

	// TODO
	// if(???) // has texcoords?
		vertexFlags |= NEBU_MESH_TEXCOORD0;

	pMesh = nebu_Mesh_Create(vertexFlags, nVertices, nTriangles);
	{
		int curVertex = 0;
		int curTri = 0;
		Lib3dsMesh *p = NULL;

		for(p = pFile->meshes; p != NULL; p = p->next) {
			addToMesh(pMesh, &curVertex, &curTri, p);
		}
	}
	return pMesh;
}

nebu_Mesh_3ds_File* nebu_Mesh_3ds_LoadFile(const char *filename)
{
	return lib3ds_file_load(filename);
}

nebu_Mesh* nebu_Mesh_3ds_Load(const char *filename) {
	nebu_Mesh* pMesh = NULL;
	nebu_Mesh_3ds_File *p3DSFile = nebu_Mesh_3ds_LoadFile(filename);
	if(!p3DSFile)
		return NULL;
	
	pMesh = nebu_Mesh_3ds_GetFromFile(p3DSFile);
	nebu_Mesh_3ds_FreeFile(p3DSFile);
	return pMesh;
}

void nebu_Mesh_3ds_FreeFile(nebu_Mesh_3ds_File* pFile) {
	lib3ds_file_free(pFile);
}
