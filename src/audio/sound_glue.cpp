#include "audio/sound_glue.h"
extern "C" {
#include "game/game.h"
#include "video/recognizer.h"
#include "scripting/nebu_scripting.h"
#include "video/video.h"
#include "game/camera.h"
#include "game/game_data.h"
#include "configuration/settings.h"
#include "base/nebu_assert.h"
}

#include "Nebu_audio.h"

#include "SDL.h"
#include "SDL_sound.h"

#include "base/nebu_debug_memory.h"

static Sound::System *sound = NULL;
static Sound::SourceMusic *music = NULL;
static Sound::SourceSample *sample_crash = NULL;
static Sound::SourceSample *sample_engine = NULL;
static Sound::SourceSample *sample_recognizer = NULL;

static int nPlayerSources;
static Sound::SourceEngine **ppPlayerSources;
static Sound::Source3D *recognizerEngine;

#define TURNLENGTH 250.0f

static void output_decoders(void)
{
    const Sound_DecoderInfo **rc = Sound_AvailableDecoders();
    const Sound_DecoderInfo **i;
    const char **ext;

    printf("Supported sound formats:\n");
    if (rc == NULL)
        printf(" * Apparently, NONE!\n");
    else
    {
        for (i = rc; *i != NULL; i++)
        {
            printf(" * %s\n", (*i)->description);
            for (ext = (*i)->extensions; *ext != NULL; ext++)
                printf("   File extension \"%s\"\n", *ext);
            printf("   Written by %s.\n   %s\n\n", (*i)->author, (*i)->url);
        } /* for */
    } /* else */

    printf("\n");
} /* output_decoders */

extern "C" {


  void Audio_EnableEngine(void) {
		int i;
		for(i = 0; i < game->players; i++)
			if( game->player[i].data.speed > 0)
				Audio_StartEngine(i);
    sample_engine->Start();
    if (gSettingsCache.show_recognizer)
      sample_recognizer->Start();
    // printf("[audio] turning on engine sound\n");
  }

  void Audio_DisableEngine(void) {
    sample_engine->Stop();
    sample_recognizer->Stop();
    // printf("[audio] turning off engine sound\n");
  }

  void Audio_Idle(void) { 
    // iterate over all the players and update the engines
    if(sample_engine->IsPlaying()) {
      for(int i = 0; i < game->players; i++) {
				if(!ppPlayerSources || !ppPlayerSources[i])
					continue;

				Player *p = game->player + i;
				Sound::SourceEngine *p3d = ppPlayerSources[i];

				float x, y;
				getPositionFromIndex(&x, &y, i);
				p3d->_location = Vector3(x, y, 0);
				float V = p->data.speed;

				int dt = game2->time.current - p->data.turn_time;
				if(dt < TURN_LENGTH) {
					float t = (float)dt / TURNLENGTH;

					float vx = (1 - t) * game2->level->pAxis[p->data.last_dir].v[0] +
						t * game2->level->pAxis[p->data.dir].v[0];
					float vy = (1 - t) * game2->level->pAxis[p->data.last_dir].v[1] +
						t * game2->level->pAxis[p->data.dir].v[1];
					p3d->_velocity = Vector3(V * vx, V * vy, 0);
				} else {
					p3d->_velocity = Vector3(V * game2->level->pAxis[p->data.dir].v[0], 
					 V * game2->level->pAxis[p->data.dir].v[1], 
					 0);
				}
				if(i == 0) {
					if(p->data.boost_enabled) {
						p3d->_speedShift = 1.2f;
					} else {
						p3d->_speedShift = 1.0f;
					}
					p3d->_pitchShift = p->data.speed / getSettingf("speed");
				}
						
#if 0
				if(i == 0) {
					if( dt < TURNLENGTH ) {
						float t = (float)dt / TURNLENGTH;
						float speedShift = ( 1 - t ) * 0.4 + t * 0.3;
						float pitchShift = ( 1 - t ) * 0.9 + t * 1.0;
						( (Sound::SourceEngine*) p3d )->_speedShift = speedShift;
						( (Sound::SourceEngine*) p3d )->_pitchShift = pitchShift;
					} else {
						( (Sound::SourceEngine*) p3d )->_speedShift = 0.3;
						( (Sound::SourceEngine*) p3d )->_pitchShift = 1.0;
					}
				}
#endif
      }
    }

    if(sample_recognizer->IsPlaying()) {
      if (gSettingsCache.show_recognizer) {
				vec2 p, v;
				getRecognizerPositionVelocity(&p, &v);
				// recognizerEngine->_location = Vector3(p.x, p.y, RECOGNIZER_HEIGHT);
				recognizerEngine->_location = Vector3(p.v[0], p.v[1], 10.0f);
				recognizerEngine->_velocity = Vector3(v.v[0], v.v[1], 0);
      }
    }

	if(music && !music->IsPlaying()) {
		// check if music is enabled. if it is, advance to
		// next song
		if(gSettingsCache.playMusic) {
			scripting_Run("nextTrack()");
		}
	}

	// TODO: add support for multiple listeners here
	// Problem/Constraint:
	// Each listener MUST consume exactly the same amount of samples
	// from each source (that means the pitch shift can't just be
	// done by speeding up playback)
    Sound::Listener& listener = sound->GetListener();
	if(gnPlayerVisuals == 0 || gppPlayerVisuals[0]->pPlayer == NULL)
	{
		listener._isValid = 0;
	}
	else
	{
		nebu_assert(gppPlayerVisuals);
		nebu_assert(gppPlayerVisuals[0]);
		listener._isValid = 1;
		listener._location = Vector3(gppPlayerVisuals[0]->camera.cam);
			Vector3 v1 = Vector3(gppPlayerVisuals[0]->camera.target);
			Vector3 v2 = Vector3(gppPlayerVisuals[0]->camera.cam);
		listener._direction = v1 - v2;
	      
		// listener._location = players[0]->_location;
		// listener._direction = players[0]->_velocity;
		if(ppPlayerSources)
			listener._velocity = ppPlayerSources[0]->_velocity;
		else
		listener._velocity = Vector3(0, 0, 0);
	}
    listener._up = Vector3(0, 0, 1);

    sound->SetMixMusic(gSettingsCache.playMusic);
    sound->SetMixFX(gSettingsCache.playEffects);
    sound->Idle();
  }

  void Audio_CrashPlayer(int player) {
    Sound::SourceCopy *copy = new Sound::SourceCopy(sample_crash);
    copy->Start();
    copy->SetRemovable();
    copy->SetType(Sound::eSoundFX);
	copy->SetName("crash (copy)");
    sound->AddSource(copy);
  }

  void Audio_Init(void) {
    Sound_Init(); // Init SDL_Sound
    // output_decoders();

    SDL_AudioSpec* spec = new SDL_AudioSpec;
    spec->freq = 22050;
    spec->format = AUDIO_S16SYS;
    spec->channels = 2;
    spec->samples = 1024;

    sound = new Sound::System(spec);

    spec->userdata = sound;
    spec->callback = sound->GetCallback();

	SDL_AudioSpec obtained;

    if(SDL_OpenAudio( spec, &obtained ) != 0) {
      fprintf(stderr, "[error] %s\n", SDL_GetError());
      sound->SetStatus(Sound::eUninitialized);
    } else {
      sound->SetStatus(Sound::eInitialized);
			/*
			fprintf(stderr, "[sound] frequency: %d\n", obtained.freq);
			fprintf(stderr, "[sound] format: %d\n", obtained.format);
			fprintf(stderr, "[sound] channels: %d\n", obtained.channels);
			fprintf(stderr, "[sound] silence: %d\n", obtained.silence);
			fprintf(stderr, "[sound] buffer in samples: %d\n", obtained.samples);
			fprintf(stderr, "[sound] buffer in bytes: %d\n", obtained.size);
			*/
    }
    sound->SetMixMusic(gSettingsCache.playMusic);
    sound->SetMixFX(gSettingsCache.playEffects);
  }

  void Audio_Start(void) {
    SDL_PauseAudio(0);
  }

  void Audio_Quit(void) {
    SDL_PauseAudio(1);
    Sound_Quit();
    SDL_CloseAudio();
	if(sound)
	{
		delete sound;
		sound = NULL;
	}
	if(sample_crash)
	{
		delete sample_crash;
		sample_crash = NULL;
	}
	if(sample_engine)
	{
		delete sample_engine;
		sample_engine = NULL;
	}
	if(sample_recognizer)
	{
		delete sample_recognizer;
		sample_recognizer = NULL;
	}
  }

  void Audio_LoadMusic(char *name) {
    if(music != NULL) {
      music->Stop();
      music->SetRemovable();
    }
    music = new Sound::SourceMusic(sound);
    music->Load(name);
		if(getSettingi("loopMusic"))
			music->SetLoop(255);
    music->SetType(Sound::eSoundMusic);

    music->SetName("music");
    sound->AddSource(music);
  }

  void Audio_PlayMusic(void) {
    music->Start();
  }

  void Audio_StopMusic(void) {
    music->Stop();
  }

  void Audio_SetMusicVolume(float volume) {
    music->SetVolume(volume);
  }
  
  void Audio_SetFxVolume(float volume) {
    sample_engine->SetVolume(volume);
    sample_crash->SetVolume(volume);
    if(volume > 0.8f)
      sample_recognizer->SetVolume(volume);
    else 
      sample_recognizer->SetVolume(volume * 1.25f);
  }

  void Audio_StartEngine(int iPlayer) {
	  if(ppPlayerSources && ppPlayerSources[iPlayer])
		ppPlayerSources[iPlayer]->Start();
  }

  void Audio_StopEngine(int iPlayer) {
	  if(ppPlayerSources && ppPlayerSources[iPlayer])
	ppPlayerSources[iPlayer]->Stop();
  }
 
  void Audio_ResetData(void)
  {
	  if(ppPlayerSources)
	  {
		  Audio_UnloadPlayers();
	  }
	  Audio_LoadPlayers();
  }

  void Audio_UnloadPlayers(void)
  {
	  if(ppPlayerSources)
	  {
		  for(int i = 0; i < nPlayerSources; i++)
		  {
			  sound->RemoveSource(ppPlayerSources[i]);
			  nebu_assert(ppPlayerSources[i]);
			  delete ppPlayerSources[i];
		  }
		  delete[] ppPlayerSources;
		  ppPlayerSources = NULL;
		  nPlayerSources = 0;
	  }
	  if(recognizerEngine)
	  {
		  sound->RemoveSource(recognizerEngine);
		  delete recognizerEngine;
		  recognizerEngine = NULL;
	  }
  }

  void Audio_LoadPlayers(void) {
	  nebu_assert(!ppPlayerSources);
	  nPlayerSources = game->players;
	ppPlayerSources = new Sound::SourceEngine*[nPlayerSources];
    for(int i = 0; i < nPlayerSources; i++)
	{
		ppPlayerSources[i] = new Sound::SourceEngine(sound, sample_engine);
		ppPlayerSources[i]->SetType(Sound::eSoundFX);
		sound->AddSource(ppPlayerSources[i]);

		char *name = new char[32];
		sprintf(name, "player %d", i);
		ppPlayerSources[i]->SetName(name);
		delete[] name;
	}

	nebu_assert(!recognizerEngine);
    recognizerEngine = new Sound::Source3D(sound, sample_recognizer);
    recognizerEngine->SetType(Sound::eSoundFX);
    recognizerEngine->Start();
    sound->AddSource(recognizerEngine);

    recognizerEngine->SetName("recognizer");
  }

  void Audio_LoadSample(char *name, int number) {
    switch(number) {
    case 0:
      sample_engine = new Sound::SourceSample(sound);
      sample_engine->Load(name);
	  sample_engine->SetName("sample: engine");
      break;
    case 1:
      sample_crash = new Sound::SourceSample(sound);
      sample_crash->Load(name);
	  sample_crash->SetName("sample: crash");
      break;
    case 2:
      sample_recognizer = new Sound::SourceSample(sound);
      sample_recognizer->Load(name);
	  sample_recognizer->SetName("sample: recognizer");
      break;
    default:
      /* programmer error, but non-critical */
      fprintf(stderr, "[error] unkown sample %d: '%s'\n", number, name);
    }
  }
}
