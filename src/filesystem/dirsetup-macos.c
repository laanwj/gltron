
void setupMacDirectories();

void dirSetup(const char *executable[]) {
  setupMacDirectories();
  initDirectories();
}

char* getHome() {
	
	return "<<fake-home-dir>>";
}

/* we want GLTron to run on locked volumes (CD-ROMs etc),
   so we must save settings to the prefs and screenshots
   to the desktop folder */

#include <Folders.h>
#include "FullPath.h" /* MoreFiles Library */
#include <assert.h>

char *_MacOS_Prefs_Directory;
char *_MacOS_Desktop_Directory;

/* Get path to a standard Mac OS directory */
static char* getStandardDirectory (UInt32 directoryType) {

    short  volume_ref_number;
    long   directory_id;
    short  len;
    Handle path;

    if ( noErr != FindFolder ( kOnSystemDisk, directoryType, kDontCreateFolder,
                               &volume_ref_number, &directory_id) ) {
        return NULL;
    }

    if ( noErr == GetFullPath (volume_ref_number, directory_id, "\p", &len, &path) ) {

        char *findDir = (char*) malloc ( sizeof(*findDir) * len + 1);
        assert (findDir != NULL);
        memcpy (findDir, *path, len-1); /* we want to remove the terminating ':' (hence len-1)*/
        findDir[len-1] = '\0';              
    	return findDir;
    }
    
    return NULL;
}

void setupMacDirectories() {

	_MacOS_Prefs_Directory = getStandardDirectory (kPreferencesFolderType);
	assert (_MacOS_Prefs_Directory != NULL);
	
	_MacOS_Desktop_Directory = getStandardDirectory (kDesktopFolderType);
	assert (_MacOS_Desktop_Directory != NULL);
}
