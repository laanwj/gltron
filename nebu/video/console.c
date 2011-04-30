#include "video/nebu_console.h"
#include "scripting/nebu_scripting.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define CONSOLE_WIDTH 60
#define BUFSIZE 100
char buf[BUFSIZE];

void consoleAddLine(const char *text) {
#define BUFSIZE 100
	char buf[BUFSIZE];
	if(strlen(text) > BUFSIZE - 20)
		return;

	// FIXME: get rid of hard limit
	// FIXME: escape "" properly
	// TODO: push string onto stack, and call lua function
	//       directly
	sprintf(buf, "console_AddLine(\"%s\")", text);
	scripting_Run(buf);
}
  
void console_Seek(int range) {
	sprintf(buf, "console_SeekRelative(%d)", range);
	scripting_Run(buf);
}

/*
  displayMessage

  post a message to the console and/or stdout/stderr.

  NOTE: Don't put newlines at the end of the format string,
        the function will handle adding them when appropriate.
 */
void displayMessage(outloc_e where, const char *fmt_str, ...) {

  char message[CONSOLE_WIDTH];
  va_list ap;
  
  va_start(ap, fmt_str);
  
  if (where & TO_CONSOLE) {
    vsprintf(message, fmt_str, ap);
    consoleAddLine(message); 
  }

  if (where & TO_STDOUT) {
    vfprintf(stdout, fmt_str, ap);
    fputc('\n', stdout);
  }

  if (where & TO_STDERR) {
    vfprintf(stderr, fmt_str, ap);
    fputc('\n', stderr);
  }
}

