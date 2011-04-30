
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* this structure is defined in nebu/base/nebu_types.h */
/* we declare it here because GLTron's headers conflict with */
/* the macos headers used below (Mac OS headers define "Point") */
typedef struct nebu_List List;
struct nebu_List {
  void *data;
  nebu_List* next;
};

/* macintosh code by Darrell Walisser */

#include <IterateDirectory.h>
#include <FullPath.h>
#include <Types.h>
#include <Files.h>
#include <Processes.h>

static char *filter_prefix = NULL;

/* -dw- From Technical Q&A FL14 */
/* http://developer.apple.com/qa/fl/fl14.html */

 /* GetApplicationDirectory returns the volume reference number
  and directory ID for the current application's directory. */
     
 OSStatus GetApplicationDirectory(short *vRefNum, long *dirID) {
     ProcessSerialNumber PSN;
     ProcessInfoRec pinfo;
     FSSpec pspec;
     OSStatus err;
         /* valid parameters */
     if (vRefNum == NULL || dirID == NULL) return paramErr;
         /* set up process serial number */
     PSN.highLongOfPSN = 0;
     PSN.lowLongOfPSN = kCurrentProcess;
         /* set up info block */
     pinfo.processInfoLength = sizeof(pinfo);
     pinfo.processName = NULL;
     pinfo.processAppSpec = &pspec;
         /* grab the vrefnum and directory */
     err = GetProcessInformation(&PSN, &pinfo);
     if (err == noErr) {
         *vRefNum = pspec.vRefNum;
         *dirID = pspec.parID;
     }
     return err;
 }

pascal void iterateProc (const CInfoPBRec * const cpb_ptr,
								  Boolean *quit_flag,
								  void *user_data) {
   StringPtr filename;							  
   int   len;
   
   nebu_List *l = (nebu_List*) user_data;
	/* filter invisible files & folders */
   if ( (cpb_ptr->hFileInfo.ioFlFndrInfo.fdFlags & kIsInvisible) == 0) {	
   	
   	len = cpb_ptr->hFileInfo.ioNamePtr[0];
   	cpb_ptr->hFileInfo.ioNamePtr[len+1] = '\0';
   	filename = cpb_ptr->hFileInfo.ioNamePtr + 1;
   	
   	/* filter file names if necessary */
   	if ( filter_prefix && (strstr ((const char*)filename, filter_prefix) != (const char*)filename) )
   		return;
   	
   	while (l->next != NULL)
   	   l=l->next;
   	
   	l->data = (char*) malloc (sizeof(char) * len + 1);  							  
      strcpy ((char*)l->data, (const char*)filename);
      
      l->next = (nebu_List*) malloc (sizeof (nebu_List));
      if (l == NULL) {
  	    fprintf (stderr, "iterateProc: out of memory\n");
  		exit (-1);
      }
      l->next->next = NULL;
      l->next->data = NULL;
   }
}

List* readDirectoryContents(const char *dirname, const char *prefix) {

  nebu_List *l;
  OSErr err;
  char *path;
  
  Str255 relPath;  
  short vRefNum;
  long  dirID;
  FSSpec spec;

  if (*dirname != ':')
    sprintf ((char*)relPath, "%c:%s", strlen(dirname) + 1, dirname);
  else
    sprintf ((char*)relPath, "%c%s", strlen(dirname) , dirname);

  err = GetApplicationDirectory (&vRefNum, &dirID);
  if (err != noErr) {
    fprintf (stderr, "GetApplicationDirectory failed\n");
    exit (-1);
  }
  
  err = FSMakeFSSpec  (vRefNum, dirID, relPath, &spec);
     
  if (err != noErr) {
    fprintf (stderr, "FSMakeFSSpec failed\n");
    exit (-1);
  }
  
  l = NULL;
  l = (nebu_List*) malloc(sizeof(nebu_List));
  if (l == NULL) {
  	fprintf (stderr, "readDirectoryContents: out of memory\n");
  	exit (-1);
  }
  l->data = NULL;
  l->next = NULL;
 
  filter_prefix = (char*)prefix;
  
  err = FSpIterateDirectory (&spec, 1, NewIterateFilterUPP(iterateProc), l);
    
  if (err != noErr)  {
    fprintf (stderr, "FSpIterateDirectory failed\n");
    exit (-1);
  }
 
  return l;
}

void makeDirectory(const char *name) {
}
