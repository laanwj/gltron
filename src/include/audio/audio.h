#ifndef AUDIO_H
#define AUDIO_H

#define SONG_PREFIX "song_"

void Sound_loadFX(void);
void Sound_init(void);
void Sound_shutdown(void);
void Sound_load(char *name);
void Sound_play(void);
void Sound_stop(void);
void Sound_idle(void);
void Sound_setMusicVolume(float volume);
void Sound_setFxVolume(float volume);
void Sound_reloadTrack(void);
void Sound_initTracks(void);
void Sound_setup(void);

#endif
