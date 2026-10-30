#include "commonvars.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "playfield.h"
#include "sound.h"
#include "states.h"

void StageClear()
{
 	bool KaderVisible;
 	KaderVisible = true;
	ReadButtons();
 	while (GameState == GSStageClear)
 	{
		ReadButtons();
		if (Pressed(BUT_SELECT))
		{
			HaltSounds();
			HaltMusic();
			GameState = GSTitleScreen;
		}
		if (Pressed(BUT_A) || Pressed(BUT_START) || Pressed(BUT_X))
		{
			HaltSounds();
			HaltMusic();
			GameState = GSOldManSpeaking;
		}
		// the frame with the message can be hidden to see the room
		if (Pressed(BUT_Y) || Pressed(BUT_B))
			KaderVisible = !KaderVisible;
  		BlitSurface(IMG[IMGRoomBackground],NULL,Buffer,0,0);
  		DrawPanel();
  		DrawPlayField();
  		if (KaderVisible)
  		  BlitSurface(IMG[IMGStageClearKader],NULL,Buffer,XOffsetGame-4,YOffsetGame + 75);
		PresentBuffer(255);
		FramerateDelay();
	}
}
