#include "base/nebu_math.h"
#include "video/nebu_mesh.h"
#include "Nebu_video.h"

#include <string.h>
#include "base/nebu_assert.h"

#include "base/nebu_debug_memory.h"

vec3* nebu_Mesh_ComputeFaceNormals(const nebu_Mesh_VB *pVB, const nebu_Mesh_IB *pIB)
{
	int i;

	vec3 *pNormals = (vec3*) malloc( pIB->nPrimitives * sizeof(vec3) );

	for(i = 0; i < pIB->nPrimitives; i++)
	{
		vec3_TriNormalDirection(pNormals + i,
			(vec3*) (pVB->pVertices + 3 * pIB->pIndices[ 3 * i + 0 ]),
			(vec3*) (pVB->pVertices + 3 * pIB->pIndices[ 3 * i + 1 ]),
			(vec3*) (pVB->pVertices + 3 * pIB->pIndices[ 3 * i + 2 ]) );
		vec3_Normalize(pNormals + i, pNormals + i);
	}
	return pNormals;
}

nebu_Mesh_Adjacency* nebu_Mesh_Adjacency_Create(const nebu_Mesh_VB *pVB, const nebu_Mesh_IB *pIB)
{
	int i, j, k, l, iCurrentTriangle;
	int **ppTriangles;
	int *pTriangles;
	int *pVertexOrder;
	int *pVertexTriCount;

	nebu_Mesh_Adjacency *pAdjacency = (nebu_Mesh_Adjacency*) malloc( sizeof(nebu_Mesh_Adjacency) );	

	nebu_assert(pIB->nPrimitivesPerIndex == 3);
	pAdjacency->nTriangles = pIB->nPrimitives;
	pAdjacency->pAdjacency = (int*) malloc( 3 * pAdjacency->nTriangles * sizeof(int) );
	memset(pAdjacency->pAdjacency, 0xff /* -1 */, 3 * pAdjacency->nTriangles * sizeof(int));
	
	//	for every vertex, create a list of triangles it's part of (euler says ???)
	//	so the number of total elements is < 6 nTris
	//	then walk the list for each vertex and test which triangles are adjacent
	//	(can be done in constant time per vertex, but finds each adjacency multiple
	//	times)

	pVertexOrder = (int*) malloc( pVB->nVertices * sizeof(int) );
	memset(pVertexOrder, 0, pVB->nVertices * sizeof(int) );
	for(i = 0; i < pIB->nPrimitives; i++)
	{
		for(j = 0; j < 3; j++)
		{
			pVertexOrder[ pIB->pIndices[ 3 * i + j ] ] += 1;
		}
	}
	ppTriangles = (int**) malloc( pVB->nVertices * sizeof(int*) );
	pTriangles = (int*) malloc( 3 * pIB->nPrimitives * sizeof(int) );
	iCurrentTriangle = 0;
	for(i = 0; i < pVB->nVertices; i++)
	{
		ppTriangles[i] = pTriangles + iCurrentTriangle;
		iCurrentTriangle += pVertexOrder[i];
	}

	pVertexTriCount = (int*) malloc( pVB->nVertices * sizeof(int) );
	memset(pVertexTriCount, 0, pVB->nVertices * sizeof(int) );
	for(i = 0; i < pIB->nPrimitives; i++)
	{
		for(j = 0; j < 3; j++)
		{
			int iVertex = pIB->pIndices[3 * i + j];
			ppTriangles[iVertex][pVertexTriCount[iVertex]] = i;
			pVertexTriCount[iVertex]++;
		}
	}
	nebu_assert( memcmp(pVertexOrder, pVertexTriCount, pVB->nVertices * sizeof(int)) == 0 );
	free(pVertexTriCount);

	// now, ppTriangles contains a list of pVertexOrder[vertex] triangles, which the vertex is part of
	for(i = 0; i < pAdjacency->nTriangles; i++)
	{
		for(j = 0; j < 3; j++)
		{
			int iCurVertex = pIB->pIndices[3 * i + j];
			int iNextVertex = pIB->pIndices[3 * i + (j + 1) % 3];
			for(k = 0; k < pVertexOrder[iCurVertex]; k++)
			{
				int iTriangle = ppTriangles[iCurVertex][k];
				for(l = 0; l < 3; l++)
				{
					if(pIB->pIndices[3 * iTriangle + (l + 1)%3] == iCurVertex &&
						pIB->pIndices[3 * iTriangle + l] == iNextVertex)
					{
						// we found an adjacency
						nebu_assert(pAdjacency->pAdjacency[3 * i + j] == -1 ||
							pAdjacency->pAdjacency[3 * i + j] == iTriangle);
						pAdjacency->pAdjacency[3 * i + j] = iTriangle;
						// break out of outer loop too, but then we won't detect inconsitencies
						// k = pVertexOrder[iCurVertex];
						// TODO (performance enhancement): Set Adjacency for the other triangle too
						break;
					}
				}
			}
		}
	}

	free(pTriangles);
	free(ppTriangles);
	free(pVertexOrder);

	return pAdjacency;
}

void nebu_Mesh_Adjacency_Free(nebu_Mesh_Adjacency *pAdjacency)
{
	free(pAdjacency->pAdjacency);
	free(pAdjacency);
}

nebu_Mesh_ShadowInfo* nebu_Mesh_Shadow_Create(nebu_Mesh_VB *pVB, nebu_Mesh_IB *pIB)
{
	nebu_Mesh_ShadowInfo *pSI = (nebu_Mesh_ShadowInfo*) malloc(sizeof(nebu_Mesh_ShadowInfo));
	pSI->pFrontfaces = NULL;
	pSI->pBackfaces = NULL;
	pSI->pEdges = NULL;
	pSI->pAdjacency = NULL;
	pSI->pFaceNormals = NULL;
	pSI->pDotsigns = NULL;
	vec3_Zero(&pSI->vLight);
	pSI->pVB = pVB;
	pSI->pVB_Extruded = NULL;
	pSI->pIB = pIB;
	
	return pSI;
}

void nebu_Mesh_Shadow_Free(nebu_Mesh_ShadowInfo* pSI)
{
	if(pSI->pAdjacency)
		nebu_Mesh_Adjacency_Free(pSI->pAdjacency);
	if(pSI->pFrontfaces)
		nebu_Mesh_IB_Free(pSI->pFrontfaces);
	if(pSI->pBackfaces)
		nebu_Mesh_IB_Free(pSI->pBackfaces);
	if(pSI->pEdges)
		nebu_Mesh_IB_Free(pSI->pEdges);
	if(pSI->pFaceNormals)
		free(pSI->pFaceNormals);
	if(pSI->pDotsigns)
		free(pSI->pDotsigns);
	nebu_Mesh_VB_Free(pSI->pVB);
	if(pSI->pVB_Extruded)
		nebu_Mesh_VB_Free(pSI->pVB_Extruded);
	nebu_Mesh_IB_Free(pSI->pIB);
	free(pSI);
}

void nebu_Mesh_Shadow_SetLight(nebu_Mesh_ShadowInfo* pSI, const vec3* pvLight)
{
	if(vec3_Cmp(&pSI->vLight, pvLight) == 0)
		return;

	pSI->vLight = *pvLight;

	// recompute dotsigns, front faces, back faces & edges

	if(!pSI->pAdjacency)
		pSI->pAdjacency = nebu_Mesh_Adjacency_Create(pSI->pVB, pSI->pIB);
	if(!pSI->pFaceNormals)
	{
		pSI->pFaceNormals = nebu_Mesh_ComputeFaceNormals(pSI->pVB, pSI->pIB);
	}
	if(!pSI->pDotsigns)
	{
		pSI->pDotsigns = (int*) malloc( pSI->pIB->nPrimitives * sizeof(int) );
	}

	// compute dotsigns && build front & back list
	{
		int i;
		int nFront = 0, nBack = 0;
		int iBackPos = 0, iFrontPos = 0;

		for(i = 0; i < pSI->pIB->nPrimitives; i++)
		{
			vec3 vNormal;
			vec3_TriNormalDirection(&vNormal,
				(vec3*) (pSI->pVB->pVertices + 3 * pSI->pIB->pIndices[ 3 * i + 0 ]),
				(vec3*) (pSI->pVB->pVertices + 3 * pSI->pIB->pIndices[ 3 * i + 1 ]),
				(vec3*) (pSI->pVB->pVertices + 3 * pSI->pIB->pIndices[ 3 * i + 2 ]) );
			if(vec3_Dot(&vNormal, &pSI->vLight) < 0)
			{
				pSI->pDotsigns[i] = -1;
				nBack++;
			}
			else
			{
				pSI->pDotsigns[i] = 1;
				nFront++;
			}
		}


		if(pSI->pFrontfaces && pSI->pFrontfaces->nPrimitives != nFront)
		{
			nebu_Mesh_IB_Free(pSI->pFrontfaces);
			pSI->pFrontfaces = NULL;
		}
		if(!pSI->pFrontfaces)
		{
			pSI->pFrontfaces = nebu_Mesh_IB_Create(nFront, 3);
		}
		if(pSI->pBackfaces && pSI->pBackfaces->nPrimitives != nBack)
		{
			nebu_Mesh_IB_Free(pSI->pBackfaces);
			pSI->pBackfaces = NULL;
		}
		if(!pSI->pBackfaces)
		{
			pSI->pBackfaces = nebu_Mesh_IB_Create(nBack, 3);
		}

		for(i = 0; i < pSI->pIB->nPrimitives; i++)
		{
			if(pSI->pDotsigns[i] == 1)
			{
				nebu_assert(iFrontPos < nFront);
				memcpy(pSI->pFrontfaces->pIndices + 3 * iFrontPos, pSI->pIB->pIndices + 3 * i, 3 * sizeof(int));
				iFrontPos++;
			}
			else
			{
				int j;
				nebu_assert(iBackPos < nBack);
				// memcpy(pSI->pBackfaces->pIndices[3 * iBackPos], pSI->pIB->pIndices[i], 3 * sizeof(int));
				for(j = 0; j < 3; j++)
				{
					// pSI->pBackfaces->pIndices[3 * iBackPos + j] = pSI->pIB->pIndices[3 * i + j] + pSI->pVB->nVertices;
					pSI->pBackfaces->pIndices[3 * iBackPos + j] = pSI->pIB->pIndices[3 * i + j];
				}
				iBackPos++;
			}
		}
		nebu_assert(iFrontPos == pSI->pFrontfaces->nPrimitives);
		nebu_assert(iBackPos == pSI->pBackfaces->nPrimitives);
	}
	// compute edge list
	{
		int i, j;
		int nEdge = 0;
		int iCurEdge = 0;

		for(i = 0; i < pSI->pIB->nPrimitives; i++)
		{
			for(j = 0; j < 3; j++)
			{
				int neighbor = pSI->pAdjacency->pAdjacency[3 * i + j];
				if(neighbor != -1 && pSI->pDotsigns[neighbor] != pSI->pDotsigns[i]
					&& i < neighbor)
				{
					nEdge++;
				}
			}
		}
		if(pSI->pEdges && pSI->pEdges->nPrimitives != nEdge)
		{
			nebu_Mesh_IB_Free(pSI->pEdges);
			pSI->pEdges = NULL;
		}
		if(!pSI->pEdges)
			pSI->pEdges = nebu_Mesh_IB_Create(nEdge, 2);

		for(i = 0; i < pSI->pIB->nPrimitives; i++)
		{
			for(j = 0; j < 3; j++)
			{
				int neighbor = pSI->pAdjacency->pAdjacency[3 * i + j];
				if(neighbor != -1 && i < neighbor &&
					pSI->pDotsigns[neighbor] != pSI->pDotsigns[i])
				{
					int v1, v2;
					v1 = pSI->pIB->pIndices[3 * i + j];
					v2 = pSI->pIB->pIndices[3 * i + (j + 1) % 3];
					nebu_assert(v1 < pSI->pVB->nVertices);
					nebu_assert(v2 < pSI->pVB->nVertices);
					nebu_assert(iCurEdge < nEdge);
					if(pSI->pDotsigns[i] == 1)
					{
						pSI->pEdges->pIndices[2 * iCurEdge + 0] = v1;
						pSI->pEdges->pIndices[2 * iCurEdge + 1] = v2;
					}
					else
					{
						pSI->pEdges->pIndices[2 * iCurEdge + 0] = v2;
						pSI->pEdges->pIndices[2 * iCurEdge + 1] = v1;
					}
					iCurEdge++;
				}
			}
		}
		nebu_assert(iCurEdge == nEdge);
	}
}
