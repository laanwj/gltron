#include "filesystem/path.h"
#include "filesystem/dirsetup.h"

#include "Nebu_filesystem.h"

#include "base/nebu_assert.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "base/nebu_debug_memory.h"

#ifndef PATH_MAX
// #warning PATH_MAX "is not defined in limits.h!"
#define PATH_MAX 255
#endif

static char preferences_dir[PATH_MAX];
static char snapshots_dir[PATH_MAX];
static char data_dir[PATH_MAX];
static char art_dir[PATH_MAX];
static char music_dir[PATH_MAX];
static char scripts_dir[PATH_MAX];
static char level_dir[PATH_MAX];

void initDirectories(void) {
  if(PREF_DIR[0] != '~')
    sprintf(preferences_dir, PREF_DIR);
  else
    sprintf(preferences_dir, "%s%s", getHome(), PREF_DIR + 1);

  if(SNAP_DIR[0] != '~')
    sprintf(snapshots_dir, SNAP_DIR);
  else
    sprintf(snapshots_dir, "%s%s", getHome(), SNAP_DIR + 1);

#ifdef LOCAL_DATA
  #ifdef macintosh
  sprintf(data_dir, ":data");
  sprintf(art_dir, ":art");
  sprintf(scripts_dir, ":scripts");
  sprintf(music_dir, ":music");
  sprintf(level_dir, ":levels");
  #else
  sprintf(data_dir, "data");
  sprintf(art_dir, "art");
  sprintf(scripts_dir, "scripts");
  sprintf(music_dir, "music");
  sprintf(level_dir, "levels");
  #endif

#else
  sprintf(data_dir, "%s%c%s", DATA_DIR, SEPARATOR, "data");
  sprintf(art_dir, "%s%c%s", DATA_DIR, SEPARATOR, "art");
  sprintf(scripts_dir, "%s%c%s", DATA_DIR, SEPARATOR, "scripts");
  sprintf(music_dir, "%s%c%s", DATA_DIR, SEPARATOR, "music");
  sprintf(level_dir, "%s%c%s", DATA_DIR, SEPARATOR, "levels");
#endif

	/*
  printf("directories:\n"
	 "\tprefs: %s\n"
	 "\tsnaps: %s\n"
	 "\tdata:  %s\n"
	 "\tart:   %s\n"
	 "\tscripts:   %s\n"
	 "\tmusic: %s\n",
	 preferences_dir, snapshots_dir, 
	 data_dir, art_dir, scripts_dir, 
	 music_dir);
	*/

  makeDirectory(preferences_dir);
  makeDirectory(snapshots_dir);
}

char* getPath( ePathLocation eLocation, const char *filename) {
  char *path = getPossiblePath( eLocation, filename );
  if( nebu_FS_Test(path) )
    return path;


  fprintf(stderr, "*** failed to locate file '%s' at '%s' (type %d)\n",
	  filename, path, eLocation);
  nebu_assert(0);

  free(path);
  return NULL;
}

char* getPossiblePath( ePathLocation eLocation, const char *filename ) {
  char *path = malloc( PATH_MAX );
  sprintf(path, "%s%c%s", getDirectory( eLocation ), SEPARATOR, filename);
  return path;
}

const char* getDirectory( ePathLocation eLocation ) {
  switch( eLocation ) {
  case PATH_PREFERENCES: return preferences_dir; break;
  case PATH_SNAPSHOTS: return snapshots_dir; break;
  case PATH_DATA: return data_dir; break;
  case PATH_SCRIPTS: return scripts_dir; break;
  case PATH_MUSIC: return music_dir; break;
  case PATH_ART: return art_dir; break;
	case PATH_LEVEL: return level_dir; break;
  default:
    fprintf(stderr, "invalid path type\n");
    nebu_assert(0);
  }
  return NULL;
}
