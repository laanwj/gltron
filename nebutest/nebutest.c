#include "video/nebu_mesh_3ds.h"
#include "video/nebu_scene.h"
#include "video/nebu_camera.h"
#include "video/nebu_renderer_gl.h"
#include "base/nebu_math.h"
#include "base/nebu_system.h"
#include "base/nebu_vector.h"
#include "base/nebu_math.h"
#include "base/nebu_argv.h"
#include "input/nebu_input_system.h"
#include "video/nebu_geometry.h"
#include "video/nebu_video_system.h"

extern void display(void);
extern void idle(void);

nebu_Mesh *pMesh = NULL;
nebu_Scene *pScene = NULL;
nebu_Shader *pShader = NULL;

float fLeafAngle = 65;
float fLeafRotation = 200;
float fLeafTranslation = 0.3f;

int nLeafSubcount = 6;

float fTrunkRotation = 207;
float fTrunkAngle = 75;
int nLeafCount = 25;
float fTrunkTranslation = 0.05f;
float fTrunkScale = 0.95f;
float fLeafScale = 0.85f;

void testMesh(void)
{
	nebu_Mesh_3ds_File *pFile;
	
	pFile = nebu_Mesh_3ds_LoadFile("models/teapot.3ds");
	if(!pFile)
		return;
	pMesh = nebu_Mesh_3ds_GetFromFile(pFile);
	nebu_Mesh_3ds_FreeFile(pFile);

	nebu_Scene_AddMesh(pScene, pMesh);
}

void leaf(matrix mat, float fLength)
{
	int i;

	vec3 vs = { { fLeafScale, fLeafScale, fLeafScale,  } };
	vec3 vt = { { 0, 0, fLeafTranslation * fLength } };
	vec3 vr = { { 0, 1, 0 } };
	vec3 vr2 = { { 0, 0, 1 } };
	matrix ts, tv, tr, tr2;
	matrix matLocal, matObj;
	matrixRotationAxis(&tr2, (float) (fLeafRotation * M_PI / 180), &vr2);
	matrixRotationAxis(&tr, (float) (fLeafAngle * M_PI / 180), &vr);
	matrixScale(&ts, &vs);
	matrixTranslation(&tv, &vt);

	matrixIdentity(&matLocal);

	for(i = 0; i < nLeafSubcount; i++)
	{

		matrixMultiply(&matLocal, &ts, &matLocal);
		matrixMultiply(&matLocal, &tr, &matLocal);
		matrixMultiply(&matLocal, &tr2, &matLocal);
		// if(j == 3)
		matrixMultiply(&matObj, &mat, &matLocal);

		nebu_Scene_AddObject(pScene, pShader, pMesh, matObj);
		matrixMultiply(&matLocal, &tv, &matLocal);
	}
}

void testCorals(void)
{
	int j;
	matrix mat;

	float fRadius = 2.5;
	float fLength = 50;
	pMesh = nebu_geom_CreateCylinder(fRadius, fRadius * 0.8f, fLength, 10, 1);

	nebu_Scene_AddMesh(pScene, pMesh);

	matrixIdentity(&mat);
	nebu_Scene_AddObject(pScene, pShader, pMesh, mat);

	{
		matrix tr1, tr2, tv, ts;
		vec3 vr1 = { { 0, 0, 1 } };
		vec3 vr2 = { { 0, 1, 0 } };
		vec3 vs = { { fTrunkScale, fTrunkScale, fTrunkScale } };
		vec3 vt = { { 0, 0, fTrunkTranslation * fLength } };

		matrixRotationAxis(&tr1, (float) ( fTrunkRotation * M_PI / 180), &vr1);
		matrixRotationAxis(&tr2, (float) ( fTrunkAngle * M_PI / 180), &vr2);
		matrixScale(&ts, &vs);
		matrixTranslation(&tv, &vt);

		for(j = 0; j < nLeafCount; j++)
		{	
			matrix matLeaf;
			matrixIdentity(&matLeaf);

			matrixMultiply(&mat, &tr1, &mat);
			// matrixMultiply(&mat, &tr2, &mat);
			matrixMultiply(&mat, &ts, &mat);
			matrixMultiply(&mat, &tv, &mat);
			matrixMultiply(&matLeaf, &mat, &tr2);
			leaf(matLeaf, fLength);
		}
	}
}

void keyboard(int state, int key, int x, int y)
{
	if(key == 27)
	{
		nebu_System_ExitLoop(0);
	}
	if(state == NEBU_INPUT_KEYSTATE_DOWN)
	{
		switch(key)
		{
		case 'p':
			nebu_Camera_Print(pScene->pCamera);
			break;
		case '=':
			nebu_Camera_Zoom(pScene->pCamera, 0.1f * nebu_Camera_GetDistance(pScene->pCamera));
			nebu_System_PostRedisplay();
			break;
		case '-':
			nebu_Camera_Zoom(pScene->pCamera, -0.1f * nebu_Camera_GetDistance(pScene->pCamera));
			nebu_System_PostRedisplay();
			break;
		}
	}
}

void mouseMotion(int x, int y)
{
	int dx, dy;
	nebu_Input_Mouse_GetDelta(&dx, &dy);
	if(dx || dy)
	{
		nebu_Camera_Rotate(pScene->pCamera,
			// NEBU_CAMERA_ROTATE_AROUND_EYE |
			NEBU_CAMERA_ROTATE_AROUND_LOOKAT |
			NEBU_CAMERA_FIXED_UP,
			(float)dx, (float)dy);
		nebu_Input_Mouse_WarpToOrigin();
		nebu_System_PostRedisplay();
	}
}

void lightShaderStart(void) {
	glEnable(GL_LIGHTING);
}

void lightShaderEnd(void)
{
	glDisable(GL_LIGHTING);
}

void solidShaderStart(void) {
	glColor4f(1,1,1, 1.0f);
}

void wireFrameSetup(void)
{
#ifndef OPENGL_ES
	glColor4f(1,0,0, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
}
void wireFrameCleaup(void)
{
#ifndef OPENGL_ES
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
}

void testSolidShader(void)
{
	pShader = nebu_Shader_Create();
	pShader->setup = solidShaderStart;
}

void testLightShader(void)
{
	pShader = nebu_Shader_Create();
	pShader->setup = lightShaderStart;
	pShader->cleanup = lightShaderEnd;
}

void testWireFrameShader(void)
{
	pShader = nebu_Shader_Create();
	pShader->setup = wireFrameSetup;
	pShader->cleanup = wireFrameCleaup;
}

void setupScene(void)
{
	{
		pScene = nebu_Scene_Create();
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	{
		float light[] = { 0, 0, 1, 0 };
		float yellow[] = { .8f, .7f, .2f, 1 };
		glLightfv(GL_LIGHT0, GL_POSITION, light);
		glEnable(GL_LIGHT0);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		// glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		// glEnable(GL_COLOR_MATERIAL);
	}

	{
		nebu_Camera *pCamera;
		vec3 eye;
		vec3 up;
		vec3 lookat;

		pCamera = nebu_Camera_Create();
		eye.v[0] = 100; eye.v[1] = 100; eye.v[2] = 25;
		up.v[0] = 0; up.v[1] = 0; up.v[2] = 1;
		lookat.v[0] = 0; lookat.v[1] = 0; lookat.v[2] = 25;
		nebu_Camera_SetupEyeUpLookAt(pCamera, &eye, &up, &lookat);
		nebu_Scene_SetCamera(pScene, pCamera);		
	}
}

void doPerspective(float fov, float ratio, float znear, float zfar) {
  float top;
  float left;

  top = tanf( fov * PI / 360.0 ) * znear;
  left = - top * ratio;
  glFrustum(left, -left, -top, top, znear, zfar);
}

int main(int argc, char *argv[])
{
	int window_id = 0;
	nebu_argv* pArguments = NULL;

	nebu_Init();
	pArguments = nebu_argv_Parse(argc, argv);
	nebu_argv_Debug_Print(pArguments, stdout);
	nebu_Video_Init();
	nebu_Video_SetWindowMode(0, 0, 800, 600);
	nebu_Video_SetDisplayMode(SYSTEM_32_BIT | SYSTEM_RGBA | SYSTEM_DOUBLE | SYSTEM_STENCIL | SYSTEM_DEPTH);
	window_id = nebu_Video_Create("");

	nebu_Input_Mouse_WarpToOrigin();
	nebu_Input_Grab();

	glViewport(0,0,800,600);
	glMatrixMode(GL_PROJECTION);
	doPerspective(35, 4.0f/3.0f, 0.1f, 1000);
	// setup emtpy scene
	setupScene();

	// test: mesh handling
	// testMesh();
	// testWireFrameShader();
	testLightShader();
	testCorals();
	// test: wire frame shading
	// testSolidShader();

	nebu_System_PostRedisplay();

	nebu_System_SetCallback_Display(display);
	nebu_System_SetCallback_Key(keyboard);
	nebu_System_SetCallback_MouseMotion(mouseMotion);
	nebu_System_SetCallback_Idle(idle);
	
	nebu_System_MainLoop();
	nebu_Video_Destroy(window_id);

	return 0;
}
