#include "filesystem/path.h"

char home = 0;
const char* getHome(void) {
	return &home;
}

void dirSetup(const char *executable[]) {
  initDirectories();
}
