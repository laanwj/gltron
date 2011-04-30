#include "video/nebu_mesh_3ds.h"

#include <lib3ds/file.h>                        
#include <lib3ds/camera.h>
#include <lib3ds/mesh.h>
#include <lib3ds/node.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <lib3ds/light.h>

#include <stdlib.h>

static void countVerticesAndTriangles(nebu_Mesh_3DSFile *pFile, 
															 int *pnVertices, int *pnTriangles)
{
	Lib3dsMesh *p;
	for(p = pFile->meshes; p != NULL; p = p->next)
	{
		*pnVertices += p->points;
		*pnTriangles += p->faces;
	}
}

static void addToMesh(nebu_Mesh *pMesh, int *pCurVertex, int *pCurTri, Lib3dsMesh *pLib3dsMesh)
{
	// TODO: add vertices & triangles of this node to mesh
	memcpy(pMesh->pVertices + 3 * *pCurVertex, pLib3dsMesh->pointL, pLib3dsMesh->points * 3 * sizeof(float));

	for(unsigned int i = 0; i < pLib3dsMesh->faces; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			pMesh->pTriangles[3 * (i + *pCurTri) + j] = pLib3dsMesh->faceL[i].points[j] + *pCurVertex;
		}
	}
	*pCurVertex += pLib3dsMesh->points;
	*pCurTri += pLib3dsMesh->faces;
}


nebu_Mesh* nebu_Mesh_GetFrom3DSFile(nebu_Mesh_3DSFile* pFile)
{
	int nTriangles = 0;
	int nVertices = 0;

	countVerticesAndTriangles(pFile, &nVertices, &nTriangles);
	nebu_Mesh* pMesh = nebu_Mesh_Create(NEBU_MESH_VERTICES, nVertices, nTriangles);

	Lib3dsMesh *p;

	int curVertex = 0;
	int curTri = 0;
	for(p = pFile->meshes; p != NULL; p = p->next)
	{
		addToMesh(pMesh, &curVertex, &curTri, p);
	}

	return pMesh;
}

nebu_Mesh_3DSFile* nebu_Mesh_Load3DSFile(const char *filename)
{
	return lib3ds_file_load(filename);
}

nebu_Mesh* nebu_Mesh_Load3DS(const char *filename) {
	nebu_Mesh* pMesh = NULL;
	nebu_Mesh_3DSFile *p3DSFile = nebu_Mesh_Load3DSFile(filename);
	if(!p3DSFile)
		return NULL;
	
	pMesh = nebu_Mesh_GetFrom3DSFile(p3DSFile);
	nebu_Mesh_Free3DSFile(p3DSFile);
	return pMesh;
}
