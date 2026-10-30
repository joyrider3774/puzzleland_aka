#include "sound.h"

#include <stdio.h>

#include "gamebuino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "assets.h"
#include "platform.h"

extern const uint8_t dm_assets[];

// the player mixes 4 tracks: the music and 3 for sound effects, so effects can overlap
#define SFXTRACKS 3

static gb_audio_player Player;
static gb_audio_track_wav MusicTrack;
static gb_audio_track_wav SfxTracks[SFXTRACKS];
static int NextSfxTrack = 0;
static SemaphoreHandle_t Lock = NULL;

// the music, in the order of the MUS_ constants of the game
static const char* const MusicFiles[] = {
    DATA_DIR "/music/title.wav", DATA_DIR "/music/game1.wav", DATA_DIR "/music/game2.wav",
    DATA_DIR "/music/oldman.wav", DATA_DIR "/music/stage.wav"};
#define MUSICCOUNT ((int)(sizeof(MusicFiles) / sizeof(MusicFiles[0])))

// the music that is playing, which starts over when it ends; -1 while there is none
static int CurrentMusic = -1;

// the player has to be fed all the time, also while the game waits or loads
static void AudioTask(void*)
{
    for (;;)
    {
        xSemaphoreTake(Lock, portMAX_DELAY);
        if ((CurrentMusic >= 0) && !MusicTrack.is_playing())
        {
            // the music loops; a file that cannot be played is not tried over and over
            if (MusicTrack.play_wav(MusicFiles[CurrentMusic]) != 0)
                CurrentMusic = -1;
        }
        Player.pool();
        xSemaphoreGive(Lock);
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

void SoundInit()
{
    Lock = xSemaphoreCreateMutex();
    Player.add_track(&MusicTrack);
    for (int Teller = 0; Teller < SFXTRACKS; Teller++)
        Player.add_track(&SfxTracks[Teller]);
    // the game runs on core 0, the music keeps streaming on the other one
    xTaskCreatePinnedToCore(AudioTask, "audio", 4096, NULL, 5, NULL, 1);
}

void SetVolume(int VolumeIn)
{
    int Master = VolumeIn * 2;
    if (Master > 255)
        Master = 255;
    if (Master < 0)
        Master = 0;
    xSemaphoreTake(Lock, portMAX_DELAY);
    Player.set_master_volume((uint8_t)Master);
    xSemaphoreGive(Lock);
}

void PlaySound(int Snd)
{
    if ((Snd < 0) || (Snd >= (int)(sizeof(ASSET_SOUNDS) / sizeof(ASSET_SOUNDS[0]))))
        return;
    const AssetSound& Sound = ASSET_SOUNDS[Snd];
    xSemaphoreTake(Lock, portMAX_DELAY);
    // a free track if there is one, the one that started longest ago otherwise
    int Track = -1;
    for (int Teller = 0; Teller < SFXTRACKS; Teller++)
        if (!SfxTracks[(NextSfxTrack + Teller) % SFXTRACKS].is_playing())
        {
            Track = (NextSfxTrack + Teller) % SFXTRACKS;
            break;
        }
    if (Track < 0)
        Track = NextSfxTrack;
    NextSfxTrack = (Track + 1) % SFXTRACKS;
    SfxTracks[Track].play_raw((const int16_t*)(dm_assets + Sound.samples), Sound.count);
    xSemaphoreGive(Lock);
}

void HaltSounds()
{
    xSemaphoreTake(Lock, portMAX_DELAY);
    for (int Teller = 0; Teller < SFXTRACKS; Teller++)
        SfxTracks[Teller].stop_playing();
    xSemaphoreGive(Lock);
}

void PlayMusic(int Nr)
{
    if ((Nr < 0) || (Nr >= MUSICCOUNT))
        return;
    xSemaphoreTake(Lock, portMAX_DELAY);
    MusicTrack.stop_playing();
    CurrentMusic = Nr;
    if (MusicTrack.play_wav(MusicFiles[Nr]) != 0)
        CurrentMusic = -1;
    xSemaphoreGive(Lock);
}

bool MusicPlaying()
{
    xSemaphoreTake(Lock, portMAX_DELAY);
    bool Playing = CurrentMusic >= 0;
    xSemaphoreGive(Lock);
    return Playing;
}

void HaltMusic()
{
    xSemaphoreTake(Lock, portMAX_DELAY);
    CurrentMusic = -1;
    MusicTrack.stop_playing();
    xSemaphoreGive(Lock);
}
