#ifndef NEBU_SCENE_H
#define NEBU_SCENE_H

#include "video/nebu_mesh.h"
#include "video/nebu_camera.h"
#include "base/nebu_util.h"
#include "base/nebu_math.h"

typedef struct {
	nebu_List meshList; // list of meshes
	nebu_List objectList; // list of shader/mesh pairs
	nebu_Camera *pCamera;
} nebu_Scene;

typedef struct {
	void (*setup)(void);
	void (*cleanup)(void);
} nebu_Shader;

typedef struct {
	nebu_Shader *pShader;
	nebu_Mesh *pMesh;
	matrix mat;
} nebu_Scene_Object;

nebu_Scene* nebu_Scene_Create(void);
void nebu_Scene_AddMesh(nebu_Scene *pScene, nebu_Mesh* pMesh);
void nebu_Scene_AddObject(nebu_Scene *pScene, nebu_Shader *pShader, nebu_Mesh *pMesh, matrix mat);
void nebu_Scene_SetCamera(nebu_Scene *pScene, nebu_Camera *pCamera);
void nebu_Scene_Draw(nebu_Scene *pScene);

nebu_Shader* nebu_Shader_Create(void);
nebu_Scene_Object* nebu_Scene_Object_Create(void);

#endif