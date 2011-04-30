#include "video/nebu_video_utility.h"
#include "video/video.h"
#include "filesystem/path.h"
#include "Nebu_filesystem.h"

#include <png.h>

#include <stdio.h>

#include <base/nebu_debug_memory.h>

#ifndef PATH_MAX
#define PATH_MAX 255
#endif

#define SCREENSHOT_PREFIX "gltron"


/*
    getNextFilename - find the next free filename in series.
 */
static char* getNextFilename(const char *suffix, int *start_at) {
  char *path = NULL;
  char fname[PATH_MAX];
  do {
    if(path != NULL)
      free(path);

    (*start_at)++;
    sprintf(fname, "%s-%s-%d%s", SCREENSHOT_PREFIX, VERSION, *start_at, 
            suffix);
    path = getPossiblePath( PATH_SNAPSHOTS, fname );
  } while ( nebu_FS_Test(path));

  return path;
}

void doPngScreenShot(Visual *display) {
	static int last_png_num; /* store last free file index. */
	nebu_Surface *pScreen;

	char *path = getNextFilename(".png", &last_png_num);
	if(!path)
		return;

	pScreen = nebu_Video_GrabScreen(display->w, display->h);
	nebu_Surface_SavePNG(pScreen, path);
	nebu_Surface_Free(pScreen);
	free(path);
}

void doBmpScreenShot(Visual *display) {
	static int last_bmp_num; /* store last free file index. */
	nebu_Surface *pScreen;

	char *path = getNextFilename(".bmp", &last_bmp_num);
	if(!path)
		return;

	pScreen = nebu_Video_GrabScreen(display->w, display->h);
	nebu_Surface_SaveBMP(pScreen, path);
	nebu_Surface_Free(pScreen);
	free(path);
}

