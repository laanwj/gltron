#include "filesystem/path.h"
#include "game/resource.h"
#include "base/nebu_resource.h"
#include "video/video.h"
#include "Nebu_filesystem.h"

#include "base/nebu_debug_memory.h"

#include "base/nebu_assert.h"

void initFonts(void) {
	char *path;

	path = getPath(PATH_DATA, "xenotron.ftx");
	if(!gTokenGameFont)
		gTokenGameFont = resource_GetToken(path, eRT_Font);
	if(!gTokenGameFont)
	{
		fprintf(stderr, "can't load font %s\n", path);
		nebu_assert(0); exit(1); // OK: critical, installation corrupt
	}
	free(path);
}