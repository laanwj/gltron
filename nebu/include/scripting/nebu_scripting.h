#ifndef NEBU_SCRIPTING_H
#define NEBU_SCRIPTING_H

#include "lua.h"

enum {
	NEBU_SCRIPTING_DEBUG = 1,
	NEBU_SCRIPTING_NOCRLF = 0,
	NEBU_SCRIPTING_CRLF = 1
};

typedef char *scripting_StringResult;

extern void scripting_Init(int flags);
extern void scripting_Quit();
extern void Scripting_Idle();

extern int scripting_GetGlobal(const char *global, const char *s, ...);
extern int scripting_GetValue(const char *name);

extern int scripting_SetFloat(float f, const char *name, const char *global, const char *s, ...);

extern int scripting_GetOptional_Float(const char *name, float *f, float fValue);
extern int scripting_GetOptional_Int(const char *name, int *i, int iValue);

extern int scripting_IsNil();
extern int scripting_IsTable();
extern int scripting_GetIntegerResult(int *i);
extern int scripting_GetFloatResult(float *f);
extern void scripting_GetFloatArrayResult(float *f, int n);
extern int scripting_GetStringResult(scripting_StringResult *s);
extern void scripting_StringResult_Free(scripting_StringResult);
extern int scripting_CopyStringResult(char *s, int len);
extern int scripting_GetArraySize(int *i);
extern int scripting_GetArrayIndex(int i);
extern int scripting_Pop(void);

extern int scripting_StackGuardStart(void);
extern void scripting_StackGuardEnd(int iPos);

extern void scripting_PushInteger(int iValue);

extern int scripting_RunFile(const char *name);
extern int scripting_Run(const char *command);
extern int scripting_RunFormat(const char *format, ...);
extern void scripting_RunGC();
extern void scripting_Register(const char *name, int(*func) (lua_State *L));

#endif
