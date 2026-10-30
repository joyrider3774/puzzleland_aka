// The game's globals, the ones every state of puzzleland.cpp shared
#ifndef COMMONVARS_H
#define COMMONVARS_H

#include <stdint.h>

#include "CHand.h"
#include "Common.h"

    //Game Stuff
extern GameStates GameState;
extern int Level;
extern CHand Hand;
// when the room started and the time now, in seconds
extern uint32_t StartTime,EndTime;

    //Audio
extern bool SoundEnabled,MusicEnabled;
extern int Volume;

    //Puzzle Game Stuff
extern int PlayField[2][Cols][Rows];
extern bool BlockActive;

#endif
