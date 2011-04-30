#ifndef PATH_H
#define PATH_H

/* if you update this listing, also make sure you update scripts/path.lua */

typedef enum ePathLocation { 
	PATH_PREFERENCES = 0,
	PATH_SNAPSHOTS, /* demos, screenshots */
	PATH_DATA,
	PATH_SCRIPTS,
	PATH_MUSIC,
	PATH_ART,
	PATH_LEVEL
} ePathLocation;

const char* getDirectory(ePathLocation eLocation);
char* getPath(ePathLocation eLocation, const char *filename);
char* getPossiblePath(ePathLocation eLocation, const char *filename);
void initDirectories(void);

#endif
