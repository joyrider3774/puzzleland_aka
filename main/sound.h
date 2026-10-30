// What the game used SDL_mixer for: the sound effects (built into the firmware) on their own
// tracks, and the music, WAV files streamed from the music folder on the SD card.
#ifndef SOUND_H
#define SOUND_H

void SoundInit();
// 0 to 128, the range Mix_Volume had
void SetVolume(int VolumeIn);
// Mix_PlayChannel(-1, Sounds[Snd], 0)
void PlaySound(int Snd);
// Mix_HaltChannel: stops every sound effect, not the music
void HaltSounds();
// Mix_PlayMusic(Music[Nr], -1): music Nr (the MUS_ constants) over and over until it is halted
void PlayMusic(int Nr);
// Mix_PlayingMusic
bool MusicPlaying();
// Mix_HaltMusic
void HaltMusic();

#endif
