#ifndef NEBU_Sound_Source3D_H
#define NEBU_Sound_Source3D_H

#include "nebu_Sound.h"
#include "nebu_SoundSystem.h"
#include "nebu_SourceSample.h"
#include "base/nebu_Vector3.h"

#define USOUND 50
#define EPSILON 0.1f	 
#define SOUND_VOL_THRESHOLD 0.1
#define VOLSCALE_BASE 1000

namespace Sound {
  class Source3D : public Source { 
  public:
    Source3D(System *system, SourceSample *source) { 
      _system = system; 
      _source = source;

      _location = Vector3(0,0,0);
      _velocity = Vector3(0,0,0);

      _position = 0;
    };
    Vector3 _location;
    Vector3 _velocity;
    SourceSample* _source;

    virtual int Mix(Uint8 *data, int len);
    virtual void GetModifiers(float &fPan, float &fVolume, float &fShift);
    //  protected:
    int _position;

  protected:
    Source3D() {
      _location = Vector3(0,0,0);
      _velocity = Vector3(0,0,0);

      _position = 0;
    };
  };
}

#endif
