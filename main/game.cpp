#include <stdlib.h>

#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "playfield.h"
#include "sound.h"
#include "states.h"

void Game()
{
	int PlayFieldX,PlayFieldY,BlockNr;
	LoadLevel();
	Hand.SetPosition(10, 8);
	Hand.Show();
	BlockActive = false;
	StartTime = Seconds();
	switch(rand()%2)
	{
		case 0 : PlayMusicIfOn(MUS_Game1);
				 break;
		case 1 : PlayMusicIfOn(MUS_Game2);
				 break;
	}
	ReadButtons();
	while (GameState == GSGame)
	{
		ReadButtons();
		if (Pressed(BUT_SELECT))
		{
			GameState = GSTitleScreen;
			HaltMusic();
		}
		if (Pressed(BUT_A))
		{
			if(!BlockActive)
			{
				PlayFieldX = Hand.GetPlayFieldX();
				PlayFieldY = Hand.GetPlayFieldY();
				BlockNr = PlayField[0][PlayFieldX][PlayFieldY];
				if (BlockNr > 0)
				{
					if(SoundEnabled)
						PlaySound(SND_PickupBlock);
					BlockActive = true;
					Hand.Hide();
					MakeBlockActive(PlayFieldX,PlayFieldY,BlockNr);
				}
			}
			else
			{
				if(MakeBlockUnActive())
				{
					if(SoundEnabled)
						PlaySound(SND_DropBlock);
					BlockActive = false;
					if(IsStageClear())
					{
						GameState = GSStageClear;
						if(SoundEnabled)
							PlaySound(SND_StageEnd);
					}
					else
						Hand.Show();
				}
				else
					if(SoundEnabled)
						PlaySound(SND_Error);
			}
		}
		if (Pressed(BUT_Y))
			if(BlockActive)
				FlipBlock(true);
		if (Pressed(BUT_X))
			if(BlockActive)
				FlipBlock(false);
		if (Pressed(BUT_B))
			if(BlockActive)
				RotateBlock();
		if (Pressed(BUT_START))
		{
			LoadLevel();
			BlockActive = false;
			Hand.SetPosition(10,8);
			Hand.Show();
		}
		// the block that is picked up moves as long as a direction is held
		if (BlockActive && DirectionReady())
		{
			if (Held(BUT_RIGHT))
				MoveBlockRight();
			if (Held(BUT_LEFT))
				MoveBlockLeft();
			if (Held(BUT_UP))
				MoveBlockUp();
			if (Held(BUT_DOWN))
				MoveBlockDown();
			if (Held(BUT_UPRIGHT))
			{
				MoveBlockRight();
				MoveBlockUp();
			}
			if (Held(BUT_UPLEFT))
			{
				MoveBlockLeft();
				MoveBlockUp();
			}
			if (Held(BUT_DOWNRIGHT))
			{
				MoveBlockRight();
				MoveBlockDown();
			}
			if (Held(BUT_DOWNLEFT))
			{
				MoveBlockLeft();
				MoveBlockDown();
			}
		}
		EndTime = Seconds();
		BlitSurface(IMG[IMGRoomBackground],NULL,Buffer,0,0);
		DrawPanel();
		DrawPlayField();
		Hand.Move();
		Hand.Draw();
		PresentBuffer(255);
		FramerateDelay();
	}
}
