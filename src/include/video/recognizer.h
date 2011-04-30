#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include "base/nebu_vector.h"

extern void resetRecognizer(void);
extern void doRecognizerMovement(void);
extern void drawRecognizer(void);
extern void drawRecognizerShadow(void);
extern void getRecognizerPositionVelocity(vec2 *p, vec2 *v);

#endif
