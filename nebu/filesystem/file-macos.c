
#include <stdio.h>


/* fopen() in MacOS on a directory returns NULL, here is a better solution */
/* that tells us if a file OR directory exists */

#include <Files.h>

/* in directory.c */
extern OSStatus GetApplicationDirectory(short *vRefNum, long *dirID);

int itemExists (const char* path) {
    
  OSStatus  err;
  Str255    relPath; 
  short     vRefNum;
  long      dirID;
  FSSpec    spec;

  if (*path != ':')
   sprintf (relPath, "%c:%s", (strlen(path) + 1), path);
  else 
   sprintf (relPath, "%c%s", (strlen(path)) , path);

  err = GetApplicationDirectory (&vRefNum, &dirID);
  if (err != noErr) {
    fprintf (stderr, "GetApplicationDirectory failed\n");
    exit (-1);
  }
  
  err = FSMakeFSSpec  (vRefNum, dirID, relPath, &spec);
  
  return (err == noErr);
}
