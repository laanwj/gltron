#ifndef EVENT_H
#define EVENT_H

typedef enum {
  EVENT_TURN_LEFT = 1,
  EVENT_TURN_RIGHT = 2,
  EVENT_CRASH = 4,
  EVENT_STOP = 8
} event_type_e;

typedef struct GameEvent {
  int type; /* what */
  int player; /* who */
  float x; /* where */
  float y;
  unsigned int timestamp;
} GameEvent;

void createEvent(int player, event_type_e eventType);
int processEvent(GameEvent *e);

#endif /* EVENT_H */
