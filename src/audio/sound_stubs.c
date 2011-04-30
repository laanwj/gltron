#include "Sound.h"
#include "gltron.h"

void Sound_loadFX() {}
void Sound_reloadTrack() {}
void Sound_shutdown() {}
void Sound_load(char *name) {}
void Sound_play() {}
void Sound_stop() {}
void Sound_idle() {}
void Sound_setMusicVolume(float volume) {}
void Sound_setFxVolume(float volume) {}
void Sound_initTracks() {
  scripting_Run("setupSoundTrack()");
}
void Sound_setup() {}
