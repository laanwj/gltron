#include "filesystem/path.h"
#include "filesystem/dirsetup.h"
#include "Nebu_filesystem.h"

#include "stdlib.h"

const char* getHome() {
  return getenv("HOME");
}

void dirSetup(const char *executable) {
#ifdef LOCAL_DATA
	goto_installpath(executable);
#endif
  initDirectories();
}

