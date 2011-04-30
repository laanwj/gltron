#ifndef NEBU_Sound_SourceEngine_H
#define NEBU_Sound_SourceEngine_H

#include "nebu_Source3D.h"

namespace Sound {
  class SourceEngine : public Source3D {
  public:
	  SourceEngine(System *system, SourceSample *source) : Source3D(system, source) {
      _speedShift = 1.0f;
      _pitchShift = 1.0f;
    };

    virtual void GetModifiers(float &fPan, float &fVolume, float &fShift);

    float _speedShift;
    float _pitchShift;
  };
}

#endif
