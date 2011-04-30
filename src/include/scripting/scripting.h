#ifndef SCRIPTING_H
#define SCRIPTING_H

// make sure this list matches the one in main.lua
typedef enum EScriptingReturnCode {
	eSRC_Quit = 0,
	eSRC_Game_End = 1,
	eSRC_Game_Pause = 2,
	eSRC_Game_Unpause = 3,
	eSRC_Game_Credits = 4,
	eSRC_Game_Escape = 5,
	eSRC_Timedemo = 7,
	eSRC_Timedemo_Abort = 8,
	eSRC_Credits = 9,
	eSRC_Game_Launch = 10,
	eSRC_GUI_Escape = 11,
	eSRC_GUI_Prompt = 12,
	eSRC_GUI_Prompt_Escape = 13,
	eSRC_Pause_Escape = 14
} eScriptingReturnCodes;
	
void init_c_interface(void);

#endif
