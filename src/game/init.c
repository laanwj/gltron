#include "filesystem/path.h"
#include "filesystem/dirsetup.h"
#include "game/engine.h"
#include "game/init.h"
#include "game/gltron.h"
#include "game/game.h"
#include "game/resource.h"
#include "base/nebu_resource.h"
#include "input/input.h"
#include "base/util.h"
#include "scripting/scripting.h"
#include "base/nebu_system.h"
#include "audio/audio.h"
#include "audio/nebu_audio_system.h"
#include "configuration/settings.h"
#include "configuration/configuration.h"
#include "video/nebu_video_system.h"
#include "filesystem/nebu_filesystem.h"
#include "scripting/nebu_scripting.h"
#include "input/nebu_input_system.h"
#include "video/video.h"

#include <stdlib.h>

#include "base/nebu_assert.h"

void initFilesystem(int argc, const char *argv[]);
void initGUIs(void);

void exitSubsystems(void)
{
	Sound_shutdown();

	freeVideoData();

	if(gWorld)
		video_FreeLevel(gWorld);
	gWorld = NULL;

	if(game)
		game_FreeGame(game);
	game = NULL;

	if(game2)
		game_FreeGame2(game2);
	game2 = NULL;

	scripting_Quit();
	nebu_FS_ClearAllPaths();
	resource_FreeAll();
	resource_Shutdown();
}

void initSubsystems(int argc, const char *argv[]) {
	nebu_Init();

	resource_Init();

	initFilesystem(argc, argv);
	initScripting();

	initConfiguration(argc, argv);
	initArtpacks(); // stores the artpack directory names in a lua table, so we can display it in the menu later on

	initGUIs();
	initVideo();
	initAudio();
	initInput();

	fprintf(stderr, "[status] done loading level...\n");
}

void initFilesystem(int argc, const char *argv[]) {
	dirSetup(argv[0]);
	// FIXME: why should argc be 1
	nebu_assert(argc == 1);
}

void initScripting(void) {
	scripting_Init(NEBU_SCRIPTING_DEBUG);
	init_c_interface();

  /* load basic scripting services */
	runScript(PATH_SCRIPTS, "basics.lua");
	runScript(PATH_SCRIPTS, "joystick.lua");
	runScript(PATH_SCRIPTS, "path.lua");

	runScript(PATH_SCRIPTS, "video.lua");

	runScript(PATH_SCRIPTS, "console.lua");
}

void initConfiguration(int argc, const char *argv[])
{
	/* load some more defaults from config file */
	runScript(PATH_SCRIPTS, "config.lua");
	runScript(PATH_SCRIPTS, "artpack.lua");
	
	/* go for .gltronrc (or whatever is defined in RC_NAME) */
	{
		char *path;
		path = getPossiblePath(PATH_PREFERENCES, RC_NAME);
		if (path != NULL) {
		if (nebu_FS_Test(path)) {
			printf("[status] loading settings from %s\n", path);
			scripting_RunFile(path);
		} else {
			printf("[error] cannot load %s from %s\n", RC_NAME, path);
		}
			free(path);
		}
		else {
			printf("[fatal] can't get valid pref path for %s\n", RC_NAME);
			nebu_assert(0); exit(1); // something is seriously wrong
		}
	}
	
	// check if the config file is from the same version
	// if not, override using defaults
	{
		float ini_version = 0, app_version;
		if(isSetting("version"))
			ini_version = getSettingf("version");
		scripting_GetGlobal("app_version", NULL);
		scripting_GetFloatResult(&app_version);
		if(ini_version < app_version)
		{
			/* load some more defaults from config file */
			runScript(PATH_SCRIPTS, "config.lua");
			runScript(PATH_SCRIPTS, "artpack.lua");
			printf("[warning] old config file version %f found, app version is %f, overriding using defaults\n",
				ini_version, app_version);
			setSettingf("version", app_version);
		}
	}
	// check if config is valid
	{
		int isValid = 1;
		scripting_GetGlobal("save_completed", NULL);
		if(scripting_IsNil()) {
			isValid = 0;
		}
		scripting_Pop();
		scripting_GetGlobal("settings", "keys", NULL);
		if(!scripting_IsTable())
		{
			isValid = 0;
		}
		scripting_Pop();
		if(!isValid)
		{
			printf("[warning] defunct config file found, overriding using defaults\n");
			runScript(PATH_SCRIPTS, "config.lua");
			runScript(PATH_SCRIPTS, "artpack.lua");

		}
	}

	/* parse any comandline switches overrinding the loaded settings */
	parse_args(argc, argv);

	/* sanity check some settings */
	checkSettings();

	scripting_Run("setupArtpackPaths()");
	scripting_Run("setupLevels()");
		
	/* intialize the settings cache, remember to do that everytime you
	   change something */
	updateSettingsCache();
}

void initVideo(void) {
	nebu_Video_Init();
	// this requuires the player data
	initVideoData();
	setupDisplay();

	loadArt();
	loadModels();
}

void initAudio(void) {
	nebu_Audio_Init();
	runScript(PATH_SCRIPTS, "audio.lua");
	/* probe for artpacks & songs */
	Sound_initTracks();
	Sound_setup();
}
	
void initGUIs(void)
{
	// menu
	runScript(PATH_SCRIPTS, "menu_functions.lua");
	runScript(PATH_SCRIPTS, "menu.lua");

	// hud stuff
	runScript(PATH_SCRIPTS, "hud-config.lua");
	runScript(PATH_SCRIPTS, "hud.lua");
	runScript(PATH_SCRIPTS, "gauge.lua");
}

void initInput(void) {
	nebu_Input_Init();

	gInput.mouse1 = 0;
	gInput.mouse2 = 0;
}
