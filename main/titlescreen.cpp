#include "CSelectors.h"
#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

void TitleScreen()
{
	CTitleScreenSelector Selector;
	ReadButtons();
	if (MusicEnabled && !MusicPlaying())
		PlayMusic(MUS_Title);
	while (GameState == GSTitleScreen)
	{
		ReadButtons();
		// the GP2X's SELECT quit the game here; on the AKA RUN + MENU goes back to the launcher
		if (Pressed(BUT_UP))
			Selector.MoveUp();
		if (Pressed(BUT_DOWN))
			Selector.MoveDown();
		if (Pressed(BUT_A) || Pressed(BUT_X) || Pressed(BUT_START))
		{
			if (SoundEnabled)
				PlaySound(SND_Select);
			switch (Selector.GetSelection())
			{
				case 1:
					Level = 0;
					GameState = GSOldManSpeaking;
					break;
				case 2:
					GameState = GSPasswordEntry;
					break;
				case 3:
					GameState = GSOptions;
					break;
				case 4:
					GameState = GSCredits;
					break;
			}
		}
		BlitSurface(IMG[IMGTitle],NULL,Buffer,0,0);
		Selector.Draw();
		PresentBuffer(255);
		FramerateDelay();
	}
}
