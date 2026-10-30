#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"
#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "platform.h"
#include "sound.h"

// the images, in the order of the Images enum
static const AssetSurface *const Images[NrOfImages] = {
    &ASSET_ROOMBACKGROUND, &ASSET_STAGECLEARKADER, &ASSET_BRIDGE, &ASSET_HAND, &ASSET_PLAYER,
    &ASSET_SHADOW, &ASSET_SPACESHIP, &ASSET_CLOUD, &ASSET_SMALLCLOUD, &ASSET_FAIRY,
    &ASSET_BETWEENSTAGE, &ASSET_BETWEENSTAGELEVEL1, &ASSET_OPTIONSSELECT, &ASSET_PAPER,
    &ASSET_SELECT, &ASSET_OLDMAN, &ASSET_TITLE, &ASSET_TITLESELECTOR, &ASSET_SDLPOWERED,
    &ASSET_WILLEMSSOFT,
    &ASSET_BLOCK1, &ASSET_BLOCK2, &ASSET_BLOCK3, &ASSET_BLOCK4, &ASSET_BLOCK5, &ASSET_BLOCK6,
    &ASSET_BLOCK7, &ASSET_BLOCK8, &ASSET_BLOCK9, &ASSET_BLOCK10, &ASSET_BLOCK11, &ASSET_BLOCK12,
    &ASSET_BLOCK13,
    &ASSET_BORDER1, &ASSET_BORDER2, &ASSET_BORDER3, &ASSET_BORDER4, &ASSET_BORDER5,
    &ASSET_BORDER6, &ASSET_BORDER7,
    &ASSET_STAGEBLOCK1, &ASSET_STAGEBLOCK2, &ASSET_STAGEBLOCK3, &ASSET_STAGEBLOCK4,
    &ASSET_STAGEBLOCK5, &ASSET_STAGEBLOCK6, &ASSET_STAGEBLOCK7, &ASSET_STAGEBLOCK8,
    &ASSET_STAGEBLOCK9, &ASSET_STAGEBLOCK10};
static_assert(sizeof(ASSET_IMAGE_NAMES) / sizeof(ASSET_IMAGE_NAMES[0]) == NrOfImages,
              "the Images enum does not match tools/convert_assets.py");

void LoadImages()
{
	for (int teller = 0; teller < NrOfImages; teller++)
		IMG[teller] = SurfaceFromAsset(*Images[teller]);
}

void IncVolume()
{
	if (Volume < 128)
	{
		Volume += 8;
		SetVolume(Volume);
	}
}

void DecVolume()
{
	if (Volume > 0)
	{
		Volume-= 8;
		SetVolume(Volume);
	}
}

void LoadSettings()
{
 	FILE *SettingsFile;
 	char Line[64];
	SoundEnabled = true;
	MusicEnabled = true;
	Volume = 128;
 	SettingsFile = fopen(DATA_DIR "/settings.dat","r");
 	if(SettingsFile)
 	{
		// the GP2X's file, read a line at a time so its StartScreenX/Y (the TV out position) do not matter
		while (fgets(Line, sizeof(Line), SettingsFile))
		{
			int Value;
			if (sscanf(Line, "SoundOn=%d", &Value) == 1)
				SoundEnabled = Value > 0;
			else if (sscanf(Line, "MusicOn=%d", &Value) == 1)
				MusicEnabled = Value > 0;
			else if (sscanf(Line, "Volume=%d", &Value) == 1)
				Volume = Value;
		}
		fclose(SettingsFile);
		if ((Volume < 0) || (Volume > 128))
			Volume = 128;
 	}
	SetVolume(Volume);
}

void SaveSettings()
{
 	FILE *SettingsFile;
 	SettingsFile = fopen(DATA_DIR "/settings.dat","w");
 	if(SettingsFile)
 	{
		fprintf(SettingsFile,"SoundOn=%d\n",SoundEnabled ? 1 : 0);
		fprintf(SettingsFile,"MusicOn=%d\n",MusicEnabled ? 1 : 0);
		fprintf(SettingsFile,"Volume=%d\n",Volume);
		fclose(SettingsFile);
 	}
}

void PlayMusicIfOn(int Nr)
{
	if (MusicEnabled)
		PlayMusic(Nr);
}

uint32_t Seconds()
{
	return GetTicks() / 1000;
}
