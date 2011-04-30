#ifndef NEBU_Sound_SourceCopy_H
#define NEBU_Sound_SourceCopy_H

#include "audio/nebu_Sound.h"
#include "audio/nebu_SoundSystem.h"
#include "audio/nebu_SourceSample.h"
#include "base/nebu_Vector3.h"

namespace Sound {
  class SourceCopy : public Source { 
  public:
    SourceCopy(SourceSample *source) { 
      _source = source;

      _position = 0;
    };
    SourceSample* _source;
    virtual int Mix(Uint8 *data, int len);
    // protected:
    int _position;
  };
}

#endif
