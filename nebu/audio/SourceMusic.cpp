#include "audio/nebu_SourceMusic.h"

#include <string.h>

#include "base/nebu_debug_memory.h"

namespace Sound {

SourceMusic::SourceMusic(System *system) { 
	_system = system;

	_sample = NULL;

	_sample_buffersize = 8192;
	_buffersize = 20 * _sample_buffersize;
	_buffer = new Uint8[_buffersize];
	memset(_buffer, 0, _buffersize);

	_decoded = 0;
	_read = 0;

	_filename = NULL;
	_rwops = NULL;
}

SourceMusic::~SourceMusic()
{
	// fprintf(stderr, "nebu_SourceMusic destructor called\n");
#ifndef macintosh
	SDL_SemWait(_sem);
#else
    SDL_LockAudio();
#endif
	if(_buffer)
		delete _buffer;
	
	if(_sample) {
		Sound_FreeSample( _sample );
		_sample = NULL;
	}

	if(_filename)
		delete _filename;

#ifndef macintosh		
	SDL_SemPost(_sem);
#else
    SDL_UnlockAudio();
#endif
}

/*! 
	\fn void SourceMusic::CreateSample(void)
	
	call this function only between semaphores
*/

void SourceMusic::CreateSample(void) {
	_rwops = SDL_RWFromFile(_filename, "rb");
	char *ext = _filename;
	for(int i = 0; *(_filename + i); i++)
	{
		if(*(_filename + i) == '.')
			ext = _filename + i + 1;
	}
	_sample = Sound_NewSample(_rwops, ext,
		_system->GetAudioInfo(),
		_sample_buffersize );

    if(_sample == NULL) {
		fprintf(stderr, "[error] failed loading sample type %s, from %s: %s\n", ext,
			_filename, Sound_GetError());
		return;
	}

    _read = 0;
    _decoded = 0;
    // fprintf(stderr, "created sample\n");
}

void SourceMusic::Load(char *filename) {
	int n = strlen(filename);
	_filename = new char[n+1];
	memcpy(_filename, filename, n + 1);
	CreateSample();
}

  void SourceMusic::CleanUp(void) {
		_read = 0;
    _decoded = 0;

    if(_sample != NULL) {
      Sound_FreeSample(_sample);
			_sample = NULL;
		}
  }

  int SourceMusic::Mix(Uint8 *data, int len) {
    if(_sample == NULL) return 0;
#ifndef macintosh
		if( SDL_SemTryWait(_sem) ) {
			fprintf(stderr, "semaphore locked, skipping mix\n");
			return 0;
		}
#endif
		// printf("mixing %d bytes\n", len);

    int volume = (int)(_volume * SDL_MIX_MAXVOLUME);
    // fprintf(stderr, "setting volume to %.3f -> %d\n", _volume, volume);
    // fprintf(stderr, "entering mixer\n");
		
    if(len < (_decoded - _read + _buffersize) % _buffersize) {
			// enough data to mix
			if(_read + len <= _buffersize) {
				SDL_MixAudio(data, _buffer + _read, len, volume);
				_read = (_read + len) % _buffersize;
			} else {
				// wrap around in buffer
				fprintf(stderr, "wrap around in buffer (%d, %d, %d)\n", 
								len, _read, _buffersize);
				
				SDL_MixAudio(data, _buffer + _read, _buffersize - _read, volume);
				len -= _buffersize - _read;
				SDL_MixAudio(data + _buffersize - _read, _buffer, len, volume);
				_read = len;
			}
		} else {
			// buffer under-run
			fprintf(stderr, "buffer underrun!\n");
			// don't do anything
		}

#ifndef macintosh
		SDL_SemPost(_sem);
#endif
    return 1;
  }

	void SourceMusic::Idle(void) {
		if(_sample == NULL)
			return;
		
		// printf("idling\n");
		while( _isPlaying && 
					 (_read == _decoded || 
					 (_read - _decoded + _buffersize) % _buffersize >
					 _sample_buffersize ) )	{
			// if(_read == _decoded)	printf("_read == _decoded == %d\n", _read);
			// fill the buffer
			int count = Sound_Decode(_sample);
			// printf("adding %d bytes to buffer\n", count);
			if(count <= _buffersize - _decoded) {
				memcpy(_buffer + _decoded, _sample->buffer, count);
			} else {
				// wrapping around end of buffer (usually doesn't happen when 
				// _buffersize is a multiple of _sample_buffersize)
				// printf("wrapping around end of buffer\n");
				memcpy(_buffer + _decoded, _sample->buffer, _buffersize - _decoded);
				memcpy(_buffer, (Uint8*) _sample->buffer + _buffersize - _decoded,
							 count - (_buffersize - _decoded));
			}
			_decoded = (_decoded + count) % _buffersize;

			// check for end of sample, loop
			if((_sample->flags & SOUND_SAMPLEFLAG_ERROR) || 
			   (_sample->flags & SOUND_SAMPLEFLAG_EOF)) {
				// some error has occured, maybe end of sample reached
#ifndef macintosh
				SDL_SemWait(_sem);
#else
                SDL_LockAudio();
#endif
				// todo: let playback finish, because there's still data
				// in the buffer that has to be mixed
				CleanUp();
				// fprintf(stderr, "end of sample reached!\n");
				if(_loop) {
					// fprintf(stderr, "looping music\n");
					if(_loop != 255) 
						_loop--;
					CreateSample();
				} else {
					_isPlaying = 0;
					// todo: notify sound system (maybe load another song?)
				}
#ifndef macintosh
				SDL_SemPost(_sem);
#else
                SDL_UnlockAudio();
#endif
			}
		} // buffer has been filled
	}
}
