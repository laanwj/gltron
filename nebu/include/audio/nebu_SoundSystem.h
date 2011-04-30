#ifndef NEBU_Sound_System_H
#define NEBU_Sound_System_H

extern "C" {
	#include "base/nebu_util.h"
}

#include "audio/nebu_Source.h"
#include "base/nebu_Vector3.h"

#include "SDL_sound.h"

namespace Sound {
  extern "C" {
    void c_callback(void *userdata, Uint8 *stream, int len);
  }

  class Listener {
  public:
    Listener() { _isValid = 0; };
    Vector3 _location;
    Vector3 _velocity;
    Vector3 _direction;
    Vector3 _up;
	int _isValid; 
  };

  enum { eUninitialized, eInitialized };

  class System {
  public:

    System(SDL_AudioSpec *spec); 
	~System();
    typedef void(*Audio_Callback)(void *userdata, Uint8* data, int len);
    Audio_Callback GetCallback() { return c_callback; };
    void Callback(Uint8* data, int len);
    void Idle(); /* remove dead sound sources */
    void AddSource(Source* source);
	void RemoveSource(Source* source);
    Sound_AudioInfo* GetAudioInfo() { return &_info; };
    Listener& GetListener() { return _listener; };
    void SetMixMusic(int value) { _mix_music = value; };
    void SetMixFX(int value) { _mix_fx = value; };
    void SetStatus(int eStatus) { _status = eStatus; };

  private:
    SDL_AudioSpec *_spec;
    Sound_AudioInfo _info;
    Listener _listener;
    nebu_List *_sources;
    int _mix_music;
    int _mix_fx;
    int _status;
  };

}

#endif
