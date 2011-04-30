#ifndef NEBU_Sound_SourceMusic_H
#define NEBU_Sound_SourceMusic_H

#include "nebu_Sound.h"

#include "nebu_Source.h"
#include "nebu_SoundSystem.h"

namespace Sound {
  class SourceMusic : public Source {
  public:
    SourceMusic(System *system);
    virtual ~SourceMusic();
    void Load(char *filename);
    virtual int Mix(Uint8 *data, int len);
		virtual void Idle(void);

  protected:
    virtual void Reset(void) { 
      if(_sample != NULL) {
				CleanUp();
				CreateSample();
				// fprintf(stderr, "sample resetted\n");
      };
    };
    void CleanUp(void);
    void CreateSample(void);

  private:
    Sound_Sample* _sample;
		int _sample_buffersize;
		
		Uint8* _buffer;
    int _buffersize;
    int _read;
    int _decoded;

		char *_filename;
    SDL_RWops *_rwops;
  };
}
#endif
