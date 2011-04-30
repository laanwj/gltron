#include "video/video.h"
#include "game/game.h"
#include "game/resource.h"
#include "base/nebu_resource.h"
#include "filesystem/path.h"
#include "base/util.h"
#include "video/video_level.h"

#include "Nebu_scripting.h"
#include "Nebu_filesystem.h"

#include <string.h>
#include "base/nebu_assert.h"

#include <base/nebu_debug_memory.h>

/*!
	scan the artpack's directory's ( art/ ) contents and store the
	file/directory names in a lua list
*/

void initArtpacks(void)
{
	const char *art_path;
	nebu_List *artList;
	nebu_List *p;
	int i;

	art_path = getDirectory( PATH_ART );
	artList = readDirectoryContents(art_path, NULL);
	if(artList->next == NULL) {
	fprintf(stderr, "[fatal] no art files found...exiting\n");
	nebu_assert(0); exit(1); // OK: critical, installation corrupt
	}

	scripting_Run("artpacks = {}");
	i = 1;
	for(p = artList; p->next != NULL; p = p->next) {
	  scripting_RunFormat("artpacks[%d] = \"%s\"", i, (char*) p->data);
	  free(p->data);
	  i++;
	}
	nebu_List_Free(artList);
	scripting_Run("setupArtpackPaths()");
}

/*! load the HUD surfaces */
void artpack_LoadSurfaces(void)
{
	// char *pHUDNames[eHUDElementCount] = {
	char *pHUDNames[] = {
		"hud-speed.png",
		"hud-mask-speed.png",
		"hud-mask-turbo.png",
		"hud-ai.png",
		"hud-map.png",
		"hud-scores.png",
		"hud-fps.png",
		"hud-buster.png",
		"hud-mask-buster.png"
	};
	int i;
	for(i = 0; i < eHUDElementCount; i++)
	{
		nebu_assert(!gpTokenHUD[i]);

		gpTokenHUD[i] = resource_GetToken(pHUDNames[i], eRT_2d);
		if(!gpTokenHUD[i])
		{
			fprintf(stderr, "fatal: failed loading %s, exiting...\n", pHUDNames[i]);
			nebu_assert(0); exit(1); // OK: critical, installation corrupt
		}
	}
}

/*!
	parse global & customized artpack.lua files
	
*/
void loadArt(void) {
	char *path;

	// load default art settings
	runScript(PATH_SCRIPTS, "artpack.lua");

	// load custom artpack settings
	path = nebu_FS_GetPath(PATH_ART, "artpack.lua");
	if(path != NULL) {
		scripting_RunFile(path);
		free(path);
	}

	initTexture(gScreen); // load skybox, trail & crash texture
	fprintf(stderr, "[status] done loading textures...\n");
	initFonts();
	fprintf(stderr, "[status] done loading fonts...\n");
	
	artpack_LoadSurfaces();
}

    

