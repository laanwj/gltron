/*
  gltron
  Copyright (C) 1999 by Andreas Umbach <marvin@dataway.ch>
*/
#include "base/nebu_system.h" // includes main
#include "game/init.h"
#include "filesystem/path.h"
#include "base/util.h"
#include "base/nebu_debug_memory.h"

#include "base/nebu_assert.h"

int main(int argc, char *argv[] ) {
	nebu_debug_memory_CheckLeaksOnExit();
	// nebu_assert_config(NEBU_ASSERT_PRINT_STDERR);
	initSubsystems(argc, (const char**) argv);
	runScript(PATH_SCRIPTS, "main.lua");
	exitSubsystems();
	return 0;
}







