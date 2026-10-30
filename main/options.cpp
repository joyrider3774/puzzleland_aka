#include <stdio.h>
#include <string.h>

#include "CSelectors.h"
#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

void Options()
{
	Color Color1 = {0,0,0};
	char Msg[20];
	COptionsSelector Selector;
	ReadButtons();
	while (GameState == GSOptions)
	{
		ReadButtons();
		if (Pressed(BUT_SELECT) || Pressed(BUT_B) || Pressed(BUT_Y))
			GameState = GSTitleScreen;
		if ((Pressed(BUT_A) || Pressed(BUT_X) || Pressed(BUT_START)) && (Selector.GetSelection() < 3))
		{
			if(Selector.GetSelection() == 1)
				SoundEnabled = ! SoundEnabled;
			if(Selector.GetSelection() == 2)
			{
				MusicEnabled = ! MusicEnabled;
				if (!MusicEnabled)
					HaltMusic();
				else
					PlayMusic(MUS_Title);
			}
			if(SoundEnabled)
				PlaySound(SND_Select);
		}
		// the volume, which the GP2X had buttons for
		if (Selector.GetSelection() == 3)
		{
			if (Pressed(BUT_LEFT))
				DecVolume();
			if (Pressed(BUT_RIGHT))
				IncVolume();
		}
		if (Pressed(BUT_UP))
			Selector.MoveUp();
		if (Pressed(BUT_DOWN))
			Selector.MoveDown();

		BlitSurface(IMG[IMGPaper],NULL,Buffer,0,0);
		WriteText(Buffer,font,"OPTIONS",7,Buffer.w / 2 - 25,20,0,Color1,true);
		if(SoundEnabled)
			strcpy(Msg,"SOUND: ON");
		else
			strcpy(Msg,"SOUND: OFF");
		WriteText(Buffer,font,Msg,strlen(Msg),Buffer.w / 2 -35,40,0,Color1);
		if (MusicEnabled)
			strcpy(Msg,"MUSIC: ON");
		else
			strcpy(Msg,"MUSIC: OFF");
		WriteText(Buffer,font,Msg,strlen(Msg),Buffer.w / 2 -35,60,0,Color1);
		snprintf(Msg,sizeof(Msg),"VOLUME: %d%%",Volume * 100/ 128);
		WriteText(Buffer,font,Msg,strlen(Msg),Buffer.w / 2 - 35,80,0,Color1);
		Selector.Draw();
		PresentBuffer(255);
		FramerateDelay();
	}
	SaveSettings();
}
