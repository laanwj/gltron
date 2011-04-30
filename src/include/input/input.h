#ifndef INPUT_H
#define INPUT_H

typedef struct Input {
  int mouse1;
  int mouse2;
} Input;


#define MOUSE_ORIG_X 100
#define MOUSE_ORIG_Y 100
#define MOUSE_CX 0.003f
#define MOUSE_CY 0.003f

typedef enum ReservedKeys {
  eEscape = 0,
  eSpace,
  eF1,
  eF2,
  eF3,
  eF4,
  eF5,
  eF6,
  eF7,
  eF8,
  eF9,
  eF10,
  eF11,
  eF12,
  eAltLeft,
  eReservedKeys,
} ReservedKeys;

extern int ReservedKeyCodes[eReservedKeys];

void keyGame(int state, int key, int x, int y);
void Input_Idle();

extern int joy_threshold;
extern Input gInput;
#endif
