#ifndef SOUND_GLUE_H
#define SOUND_GLUE_H

#ifdef __cplusplus
extern "C" {
#endif

void Audio_EnableEngine(void);
void Audio_DisableEngine(void);
void Audio_Idle(void);
void Audio_CrashPlayer(int player);
void Audio_LoadPlayers(void);
void Audio_Init(void);
void Audio_Start(void);
void Audio_Quit(void);
void Audio_ResetData(void);
void Audio_LoadPlayers(void);
void Audio_UnloadPlayers(void);
void Audio_LoadSample(char *name, int number);
void Audio_LoadMusic(char *name);
void Audio_PlayMusic(void);
void Audio_StopMusic(void);
void Audio_SetMusicVolume(float volume);
void Audio_SetFxVolume(float volume);

void Audio_StartEngine(int player);
void Audio_StopEngine(int player);
#ifdef __cplusplus
}
#endif

#endif // defined SOUND_GLUE_H
