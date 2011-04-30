#ifndef MODEL_H
#define MODEL_H

#include "base/nebu_vector.h"
#include "video/nebu_mesh.h"

typedef enum eMaterialType
{
	eMT_Ambient		= 0x01,
	eMT_Diffuse		= 0x02,
	eMT_Specular	= 0x04,
	eMT_Shininess	= 0x08
} eMaterialType;

typedef struct gltron_Mesh_Material {
  float ambient[4];
  float diffuse[4];
  float specular[4];
  float shininess;
  char *name;
  int flags;
} gltron_Mesh_Material;

typedef struct gltron_Mesh {
	/*
	float *pVertices;
	float *pNormals;
	int nVertices, nNormals;
	*/
	nebu_Mesh_VB *pVB;
	nebu_Mesh_ShadowInfo *pSI;
	// unsigned short **ppIndices;
	// int *pnFaces;
	nebu_Mesh_IB **ppIB;
	int nMaterials;
	gltron_Mesh_Material** ppMaterials;
	box3 BBox;
	int bIsBBoxValid;
} gltron_Mesh;

typedef enum gltron_MeshType {
  TRI_MESH,
  QUAD_MESH
} gltron_MeshType;
    
gltron_Mesh* gltron_Mesh_Create(int vertexformat, int nVertices, int *pnPrimitives, int nMaterials);
gltron_Mesh* gltron_Mesh_LoadFromFile(const char* pFilename, gltron_MeshType type);
gltron_Mesh* gltron_Mesh_LoadFromMesh(nebu_Mesh *pMesh, gltron_Mesh_Material *pMaterial);
gltron_Mesh_Material* gltron_Mesh_Material_Create(const char *pName, int flags);
void gltron_Mesh_Material_Free(gltron_Mesh_Material *pMaterial);
void gltron_Mesh_Material_Copy(gltron_Mesh_Material *pDst, const gltron_Mesh_Material *pSrc);
void gltron_Mesh_Material_Set(gltron_Mesh_Material *pMat);

void gltron_Mesh_Draw(gltron_Mesh* pMesh, gltron_MeshType type);
void gltron_Mesh_DrawExplosion(gltron_Mesh *pMesh, float fRadius);
void gltron_Mesh_Free(gltron_Mesh* pMesh);
void gltron_Mesh_Scale(gltron_Mesh* pMesh, float fScale);

typedef enum ColorType {
  eAmbient = 0,
  eDiffuse,
  eSpecular
} ColorType;

void gltron_Mesh_Material_SetAlpha(gltron_Mesh *pMesh, float fAlpha);
void gltron_Mesh_Material_SetColor(gltron_Mesh *pMesh, char *name, 
	 ColorType eType, float pColor[4]);

void gltron_Mesh_ComputeBBox(gltron_Mesh* pMesh);

#endif /* MODEL_H */
