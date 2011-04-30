#ifndef DATA_H
#define DATA_H

/* general data structures */
  
typedef enum EGameType {
  GAME_SINGLE = 1,
#ifdef RECORD
  GAME_SINGLE_RECORD = 2,
  GAME_PLAY = 4,
  GAME_PLAY_NETWORK = 8,
  GAME_NETWORK_RECORD
#endif
} EGameType;

#endif
