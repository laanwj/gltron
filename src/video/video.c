#include "video/video.h"
#include "game/game.h"
#include "game/resource.h"
#include "base/nebu_resource.h"
#include "filesystem/path.h"
#include "configuration/settings.h"

#include "Nebu_video.h"
#include "Nebu_input.h"
#include "Nebu_scripting.h"

#include <string.h>

#include "base/nebu_debug_memory.h"

#include "base/nebu_assert.h"

void displayGame(void) {
  drawGame();
  nebu_System_SwapBuffers();
}

void video_LoadResources(void)
{
}

void video_ReleaseResources(void)
{
}

int initWindow(void) {
	int win_id;
	int flags;
	/* char buf[20]; */

	nebu_Video_SetWindowMode(0, 0, getSettingi("width"), getSettingi("height"));

	flags = SYSTEM_RGBA | SYSTEM_DOUBLE | SYSTEM_DEPTH | SYSTEM_STENCIL; 
	if(getSettingi("bitdepth_32"))
		flags |= SYSTEM_32_BIT;
	if(getSettingi("windowMode") == 0)
		flags |= SYSTEM_FULLSCREEN;

	nebu_Video_SetDisplayMode(flags);

	win_id = nebu_Video_Create("gltron");      
	// check if we have destination alpha,
	// if not, display warning
	{
		int r, g, b, a;
		nebu_Video_GetDisplayDepth(&r, &g, &b, &a);
		if(a == 0)
		{
			scripting_Run("gui_hide_background = 1");
			scripting_Run("Menu = WarningMenu");
		}
	}

	if (win_id < 0) { 
		if( getSettingi("use_stencil") ) {
			flags &= ~SYSTEM_STENCIL;
			nebu_Video_SetDisplayMode(flags);
			win_id = nebu_Video_Create("gltron");      
			if(win_id >= 0) {
				setSettingi("use_stencil", 0);
				goto SKIP;
			}
		}
		printf("[fatal] could not create window...exiting\n");
		nebu_assert(0); exit(1); /* OK: critical, no visual */
	}

	SKIP:

	if(getSettingi("windowMode") == 0 || getSettingi("mouse_warp") == 1) {
		nebu_Input_Grab();
	} else {
		nebu_Input_Ungrab();
	}
	return win_id;
}

void reshape(int x, int y) {
	if(x < getSettingi("height") || x < getSettingi("width"))
		initGameScreen();
	if(x > getSettingi("width") )
		gScreen->vp_x = (x - getSettingi("width")) / 2;
	if(y > getSettingi("height") )
		gScreen->vp_y = (y - getSettingi("height")) / 2;
	if(game)
		changeDisplay(-1);
}

void shutdownDisplay() {
	resource_ReleaseType(eRT_Font);
	resource_ReleaseType(eRT_Texture);
	resource_ReleaseType(eRT_2d);
	gui_ReleaseResources();
	nebu_Video_Destroy(gScreen->win_id);
	// printf("[video] window destroyed\n");
}

void setupDisplay() {
	// fprintf(stderr, "[video] trying to create window\n");
	gScreen->win_id = initWindow();
	// fprintf(stderr, "[video] window created\n");
	// initRenderer();
	// printRendererInfo();
	// printf("win_id is %d\n", d->win_id);
	// fprintf(stderr, "[status] loading art\n");

	SystemReshapeFunc(reshape);
}

void loadModels(void)
{
	char *path;
	int i;

	nebu_assert(!gTokenRecognizer);
	nebu_assert(!gTokenRecognizerQuad);

	for(i = 0; i < LC_LOD; i++)
	{
		nebu_assert(!gpTokenLightcycles[i]);
	}

	/* load recognizer model */
	path = getPath(PATH_DATA, "recognizer.obj");
	gTokenRecognizer = resource_GetToken(path, eRT_GLtronTriMesh);
	if(!gTokenRecognizer)
	{
		fprintf(stderr, "fatal: could not load %s - exiting...\n", path);
		nebu_assert(0); exit(1); // OK: critical, installation corrupt
	}
	free(path);

	/* load recognizer quad model (for recognizer outlines) */
	path = getPath(PATH_DATA, "recognizer_quad.obj");
	gTokenRecognizerQuad = resource_GetToken(path, eRT_GLtronQuadMesh);
	if(!gTokenRecognizerQuad)
	{
		fprintf(stderr, "fatal: could not load %s - exiting...\n", path);
		nebu_assert(0); exit(1); // OK: critical, installation corrupt
	}
	free(path);

	/* load lightcycle models */
	for(i = 0; i < LC_LOD; i++) {
		path = getPath(PATH_DATA, lc_lod_names[i]);
		gpTokenLightcycles[i] = resource_GetToken(path, eRT_GLtronTriMesh);
		if(!gpTokenLightcycles[i])
		{
			fprintf(stderr, "fatal: could not load model %s - exiting...\n", lc_lod_names[i]);
			nebu_assert(0); exit(1); // OK: critical, installation corrupt
		}
		free(path);
	}
}

void freeVideoData(void)
{
	int i;
	for(i = 0; i < gnPlayerVisuals; i++)
	{
		free(gppPlayerVisuals[i]);
	}
	free(gppPlayerVisuals);
	free(gScreen->textures);
	free(gScreen->ridTextures);
	free(gScreen);
}

void initVideoData(void) {
	int i;

	gScreen = (Visual*) malloc(sizeof(Visual));
	gViewportType = getSettingi("display_type"); 

	{
		Visual *d = gScreen;
		d->w = getSettingi("width"); 
		d->h = getSettingi("height"); 
		d->vp_x = 0; d->vp_y = 0;
		d->vp_w = d->w; d->vp_h = d->h;
		d->onScreen = -1;
		d->ridTextures = (int*) malloc(TEX_COUNT * sizeof(int));
		memset(d->ridTextures, 0, TEX_COUNT * sizeof(int));
		d->textures = (unsigned int*) malloc(TEX_COUNT * sizeof(unsigned int));
		memset(d->textures, 0, TEX_COUNT * sizeof(unsigned int));
	}

	gnPlayerVisuals = MAX_PLAYER_VISUALS;
	gppPlayerVisuals = (PlayerVisual**) malloc(gnPlayerVisuals * sizeof(PlayerVisual*));

	for(i = 0; i < gnPlayerVisuals; i++)
	{
		gppPlayerVisuals[i] = (PlayerVisual*) malloc(sizeof(PlayerVisual));
		gppPlayerVisuals[i]->pPlayer = NULL;
	}

	for(i = 0; i < eHUDElementCount; i++)
	{
		gpTokenHUD[i] = 0;
	}
}

void initGameScreen(void)
{
	Visual *d;
	d = gScreen;
	d->w = getSettingi("width");
	d->h = getSettingi("height"); 
	d->vp_x = 0; d->vp_y = 0;
	d->vp_w = d->w; d->vp_h = d->h;
}

void video_UnloadLevel(void)
{
	if(gWorld)
		video_FreeLevel(gWorld);
	gWorld = NULL;
}

void video_LoadLevel(void) {
	printf("[status] load/reload video data\n");

	nebu_assert(!gWorld);
	gWorld = video_CreateLevel();
}
	
void video_ResetData(void) {
	int i;

	printf("[status] reset video data\n");

	for(i = 0; i < game->players; i++) {
		Player *p = game->player + i;
		{
			char name[32];
			sprintf(name, "model_diffuse_%d", i % MAX_PLAYER_COLORS);
					scripting_GetGlobal(name, NULL);
			scripting_GetFloatArrayResult(p->profile.pColorDiffuse, 4);
			sprintf(name, "model_specular_%d", i % MAX_PLAYER_COLORS);
					scripting_GetGlobal(name, NULL);
			scripting_GetFloatArrayResult(p->profile.pColorSpecular, 4);
			sprintf(name, "trail_diffuse_%d", i % MAX_PLAYER_COLORS);
					scripting_GetGlobal(name, NULL);
			scripting_GetFloatArrayResult(p->profile.pColorAlpha, 4);
		}
	}
}

void initDisplay(Visual *d, int type, int p, int onScreen) {
	float field_x = gScreen->vp_w / 32.0f;
	float field_y = gScreen->vp_h / 24.0f;
	d->h = gScreen->h;
	d->w = gScreen->w;
	d->vp_x = gScreen->vp_x + (int) ( vp_x[type][p] * field_x );
	d->vp_y = gScreen->vp_y + (int) ( vp_y[type][p] * field_y );
	d->vp_w = (int) ( vp_w[type][p] * field_x );
	d->vp_h = (int) ( vp_h[type][p] * field_y );
	d->onScreen = onScreen;
}  

static void defaultViewportPositions(void) {
	int i;

	for(i = 0; i < gnPlayerVisuals; i++)
	{
		gppPlayerVisuals[i]->pPlayer = &game->player[i % game->players];
	}
}

/*
  autoConfigureDisplay - configure viewports so every human player has one
 */
static void autoConfigureDisplay(void) {
	int n_humans = 0;
	int i;
	int vp;

	// nebu_assert(gnPlayerVisuals <= game->players);
	// the other way round makes more sense
	nebu_assert(getSettingi("players") <= gnPlayerVisuals);

	defaultViewportPositions();

	/* loop thru players and find the humans */
	for (i=0; i < game->players; i++) {
		if (game->player[i].ai.active == AI_HUMAN)
		{
			gppPlayerVisuals[n_humans]->pPlayer = &game->player[i];
			n_humans++;
		}    
	}

	switch(n_humans) {
	case 0 :
		/*
			Not sure what the default should be for
			a game without human players. For now 
			just show a single viewport.
		*/
		/* fall thru */
	case 1 :
		vp = VP_SINGLE;
		break;
	case 2 :
		vp = VP_SPLIT;
		break;
	default :
		defaultViewportPositions();
		vp = VP_FOURWAY;
	}  
	updateDisplay(vp);
}

void changeDisplay(int view) {
	nebu_assert(game);

	/* passing -1 to changeDisplay tells it to use the view from settings */
	if (view == -1) {
		view = getSettingi("display_type");
	}
	  
	if (view == 3) {
		autoConfigureDisplay(); 
	} else {
		defaultViewportPositions(); 
		updateDisplay(view);
	}

	// displayMessage(TO_STDOUT, "set display to %d", view);
	setSettingi("display_type", view);
}

void updateDisplay(int vpType) {
	int i;

	gViewportType = vpType;

	for (i = 0; i < gnPlayerVisuals; i++) {
		gppPlayerVisuals[i]->display.onScreen = 0;
	}
	for (i = 0; i < vp_max[vpType]; i++) {
		initDisplay(& gppPlayerVisuals[i]->display, vpType, i, 1);
	}

}

void Video_Idle(void) { }
