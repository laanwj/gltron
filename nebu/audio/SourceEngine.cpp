#include "audio/nebu_SourceEngine.h"

namespace Sound {
  void SourceEngine::GetModifiers(float& fPan, float& fVolume, float& fShift) {
    Source3D::GetModifiers(fPan, fVolume, fShift);
    fVolume *= _speedShift;
    fShift *= _pitchShift;
  }
}





