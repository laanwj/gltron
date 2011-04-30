#include "video/nebu_scene.h"
#include "video/nebu_video_system.h"
#include "video/nebu_renderer_gl.h"

#include "base/nebu_debug_memory.h"

void nebu_Scene_AddMesh(nebu_Scene *pScene, nebu_Mesh* pMesh)
{
	nebu_List_AddTail(&pScene->meshList, pMesh);
}

void nebu_Scene_AddObject(nebu_Scene *pScene, nebu_Shader *pShader, nebu_Mesh *pMesh, matrix mat)
{
	nebu_Scene_Object* pObject = nebu_Scene_Object_Create();
	pObject->pMesh = pMesh;
	pObject->pShader = pShader;
	pObject->mat = mat;

	nebu_List_AddTail(&pScene->objectList, pObject);
}

nebu_Scene* nebu_Scene_Create(void)
{
	nebu_Scene *pScene = NULL;

	pScene = (nebu_Scene*) malloc(sizeof(nebu_Scene));
	if(!pScene) return NULL;

	pScene->meshList.next = NULL;
	pScene->objectList.next = NULL;
	pScene->pCamera = NULL;
	return pScene;
}

nebu_Scene_Object* nebu_Scene_Object_Create(void)
{
	nebu_Scene_Object *pObject = NULL;

	pObject = (nebu_Scene_Object*) malloc(sizeof(nebu_Scene_Object));
	if(!pObject) return NULL;

	pObject->pMesh = NULL;
	pObject->pShader = NULL;

	return pObject;
}

void nebu_Scene_SetCamera(nebu_Scene *pScene, nebu_Camera *pCamera)
{
	pScene->pCamera = pCamera;
}

void nebu_Scene_Draw(nebu_Scene *pScene)
{
	nebu_List *l;

	nebu_Video_CheckErrors("[video errors] before scene");

	if(!pScene->pCamera)
		return;

	nebu_Camera_LookAt(pScene->pCamera);
	for(l = &pScene->objectList; l->next; l = l->next)
	{
		nebu_Scene_Object *pObject = (nebu_Scene_Object*) l->data;
		if(pObject->pShader->setup)
			(pObject->pShader->setup)();
		glPushMatrix();
		glMultMatrixf(pObject->mat.m);
		if(pObject->pMesh)
			nebu_Mesh_DrawGeometry(pObject->pMesh);
		glPopMatrix();
		if(pObject->pShader->cleanup)
			(pObject->pShader->cleanup)();
	}

	nebu_Video_CheckErrors("[video errors] after scene");
}

nebu_Shader* nebu_Shader_Create(void)
{
	nebu_Shader *pShader = NULL;

	pShader = (nebu_Shader*) malloc(sizeof(nebu_Shader));
	if(!pShader) return NULL;

	pShader->setup = NULL;
	pShader->cleanup = NULL;

	return pShader;
}
