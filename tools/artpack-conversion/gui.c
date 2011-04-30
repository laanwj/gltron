// load gltron.png & gltron_logo.png
// find out which one has the higher resolution
// create render surface for high-res image
// blend images
// capture screenshot
// save to file

// optional: conversion to jpg, since the final image won't be used for
// compositing

#include "video/nebu_video_system.h"
#include "video/nebu_video_utility.h"
#include "video/nebu_renderer_gl.h"
#include "video/nebu_2d.h"
#include "base/nebu_surface.h"
#include "base/nebu_system.h"
#include "base/nebu_types.h"
#include "filesystem/nebu_filesystem.h"

#include <assert.h>
#include <stdio.h>
#include <direct.h> // win32 directory access
#include <windows.h>

int width, height;
float bgScale, logoScale;
nebu_Surface *psfBackground, *psfLogo;
nebu_2d *pBackground, *pLogo;
nebu_Surface *psfGui;

void render() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	nebu_Video_rasonly(0, 0, width, height);

	glPushMatrix();
	glScalef( bgScale, bgScale, 1 );
	nebu_2d_Draw(pBackground);
	glPopMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslatef(width / 512.0f * (512 - 320 - 10), height / 384.0f * (384 - 80), 0);
	glScalef( logoScale, logoScale, 1 );
	nebu_2d_Draw(pLogo);
	glPopMatrix();

	nebu_System_SwapBuffers();
}

void convertGUI(void)
{
	int id;

	if(!nebu_FS_Test("gltron.png") || !nebu_FS_Test("gltron_logo.png"))
		return;

	psfBackground = nebu_Surface_LoadPNG("gltron.png");
	psfLogo = nebu_Surface_LoadPNG("gltron_logo.png");

	width = max(psfBackground->w, psfLogo->w * 512 / 320);
	while(width % 4)
		width++;
	height = width * 3/4;
	if(width > 1024) width = 1024; // max. limit of texture size

	bgScale = (float) width / psfBackground->w;
	logoScale = (float) width * 320 / 512 / psfLogo->w;
	printf("scaling bg by %f, logo by %f\n", bgScale, logoScale);

	nebu_Video_SetWindowMode(0, 0, width, height);
	id = nebu_Video_Create("conversion");
	pBackground = nebu_2d_Create(psfBackground, 0);
	pLogo = nebu_2d_Create(psfLogo, 0);
	render();
	psfGui = nebu_Video_GrabScreen(width, height);
	nebu_Surface_SavePNG(psfGui, "gui.png");
	nebu_Video_Destroy(id);
}

void convertWalls(void)
{
	nebu_Surface* pWalls[4];
	nebu_Surface* pResult;
	int i;

	for(i = 0; i < 4; i++)
	{
		char buf[64];
		sprintf(buf, "gltron_wall_%d.png", i + 1);
		if(!nebu_FS_Test(buf))
			return;

		pWalls[i] = nebu_Surface_LoadPNG(buf);
	}
	pResult = nebu_Surface_Create(4 * pWalls[0]->w, pWalls[0]->h, pWalls[0]->format);
	for(i = 0; i < 4; i++)
	{
		nebu_Rect r;
		r.width = pWalls[0]->w;
		r.height = pWalls[1]->h;
		r.x = i * r.width;
		r.y = 0;
		nebu_Surface_Blit(pWalls[i], NULL, pResult, &r);
	}
	nebu_Surface_SavePNG(pResult, "walls.png");
}

int main(int argc, char *argv[]) {
	nebu_Init();
	nebu_Video_Init();

	if(argc == 1)
	{
		convertGUI();
		convertWalls(); // creates OGL window
	}
	else
	{
		int i;
		for(i = 0; i < argc - 1; i++)
		{
			_chdir(argv[i + 1]);
			MessageBox(NULL, argv[i + 1], "Processing...", 0);
			convertGUI();
			convertWalls();
		}
	}

	nebu_System_Exit();

	return 0;
}
