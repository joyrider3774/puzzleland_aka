// What the states have in common: the images, volume and settings
#ifndef GAMEFUNCS_H
#define GAMEFUNCS_H

#include <stdint.h>

// the images, from the firmware
void LoadImages();
void IncVolume();
void DecVolume();
// sound and music on or off and the volume, in DATA_DIR/settings.dat
void LoadSettings();
void SaveSettings();
// Mix_PlayMusic when the music is on
void PlayMusicIfOn(int Nr);
// the seconds since the AKA started, what time(NULL) was for the time a room takes
uint32_t Seconds();

#endif
