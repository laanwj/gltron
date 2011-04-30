#include "audio/nebu_SoundSystem.h"

#include <string.h>

#include "base/nebu_debug_memory.h"

namespace Sound {
  System::System(SDL_AudioSpec *spec) { 
    _spec = spec;

    _info.format = _spec->format;
    _info.rate = spec->freq;
    _info.channels = spec->channels;
    
    _mix_music = 1; // TODO: add 'master' volume for music and fx
    _mix_fx = 1;

	_sources = nebu_List_Create();

    _status = 0; // sound system is not initialized
  }
  System::~System()
  {
	  for(nebu_List *p = _sources; p->next; p = p->next)
	  {
		  delete (Source*) p->data;
	  }
	  nebu_List_Free(_sources);

	  delete _spec;
  }

  void System::Callback(Uint8* data, int len) {
    // printf("callback got called for %d bytes of data\n", len);

    // ensure silence
    memset(data, 0, len);

    if(_status == eUninitialized) 
      return;

    nebu_List* p;
    int sources_mixed = 0;
    for(p = _sources; p->next != NULL; p = p->next) {
      Source* s = (Source*) p->data;
      if(s->IsPlaying()) {
				// fprintf(stderr, "mixing source\n");
				if(!(
						 (s->GetType() & eSoundFX && ! _mix_fx ) ||
						 (s->GetType() & eSoundMusic && ! _mix_music) )
					 )
					{
						if( s->Mix(data, len) )
							sources_mixed++;
					}
				// fprintf(stderr, "done mixing %d sources\n", sources_mixed);
      }
    }
  }

  void System::AddSource(Source* source) { 
	  nebu_List_AddTail(_sources, source);
  }

  void System::RemoveSource(Source* source)
  {
	  nebu_List *p, *pPrev = NULL;
	  for(p = _sources; p->next != NULL; p = p->next)
	  {
		  if((Source*)p->data == source)
		  {
			  nebu_List_RemoveAt(p, pPrev);
			  break;
		  }
	  }
  }

  void System::Idle(void) {
		/* idle processing */
		nebu_List *p, *pPrev;
		int bCheckRemove = 1;

		for(p = _sources; p->next != NULL; p = p->next)
		{
			Source *source = (Source*) p->data;
			source->Idle();
		}

		while(bCheckRemove)
		{
			bCheckRemove = 0;
			pPrev = NULL;
			for(p = _sources; p->next != NULL; p = p->next)
			{
				// check if source is removable & has stopped playing
				Source *source = (Source*) p->data;
				if(source->IsRemovable() && !source->IsPlaying())
				{
					delete p->data;
					nebu_List_RemoveAt(p, pPrev);
					bCheckRemove = 1;
					break;
				}
				pPrev = p;
			}
		}
	}

	extern "C" {
    void c_callback(void *userdata, Uint8 *stream, int len) { 
      // printf("c_callback got called for %d bytes of data\n", len);
      ((System*)userdata)->Callback(stream, len);
    }
  }
}

