// Puzzle Land for the Gamebuino AKA, ported from Willems Davy's GP2X version (puzzleland.cpp there).
//
// The game is the GP2X one at its own 320x240, the size of the AKA's screen: the same intro,
// title screen, old man, rooms, screens between them, passwords, room select and options. What
// sat under it changed: SDL is gfx.*, SDL_mixer is sound.*, the GP2X buttons are input.*, and
// the files live in the PUZZLELAND folder on the SD card. The TV out screen position of the GP2X
// is gone. puzzleland.cpp is split up: its classes are in CHand.*, CBetweenStage.* and
// CSelectors.*, the rooms in playfield.*, its states are a file each and what they share is in
// gamefuncs.*.
//
// Keys: d-pad or joystick, A select / pick up and drop a block, B back / rotate the block, C and D
// flip the block, R1 start the room over, MENU back to the title screen. RUN + MENU held goes
// back to the launcher.

#include <stdlib.h>

#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

static void Run()
{
	PlatformInit();
	GfxInit();
	MakeDataDirs();
	SoundInit();
	srand(GetTicks());
	LoadSettings();
	LoadImages();
	while (GameState != GSQuit)
	{
		switch(GameState)
		{
			case GSIntro :
				Intro();
				break;
			case GSTitleScreen :
				TitleScreen();
				break;
			case GSOldManSpeaking :
				OldManSpeaking();
				break;
			case GSPasswordEntry :
				PasswordEntry();
				break;
			case GSCredits :
				Credits();
				break;
			case GSOptions :
				Options();
				break;
			case GSGame :
				Game();
				break;
			case GSStageClear:
				StageClear();
				break;
			case GSNextStage:
				NextStage();
				break;
			case GSStageSelect:
				StageSelect();
				break;
			default:
				break;
		}
	}
	// nothing sets GSQuit: the game is left with RUN + MENU (see PollKeys) or switched off
	ReturnToLoader();
}

extern "C" void app_main(void)
{
	Run();
}
