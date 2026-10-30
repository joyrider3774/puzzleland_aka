#include <stdio.h>
#include <string.h>

#include "CBetweenStage.h"
#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

// the screen between two rooms: the player walks to the next room over a bridge, a fairy drops
// the block the next rooms add, and a space ship flies the password of the next room over
static void NextStageLevel1to35()
{
	char ChrLevel[5];
	char PrevLevel[5],NextLevel[5];
	CStageBlock StageBlock;
	CPlayer Player(30,167,30,268);
	CCloud Cloud1(259,15,-0.40,Big);
	CCloud Cloud2(185,45,-0.25,Small);
	CCloud Cloud3(134,7,-0.40,Big);
	CCloud Cloud4(50,50,-0.25,Small);
	CSpaceShip SpaceShip;
	Color Color1 = {25,50,50};
	Color Color2 = {0,0,0};
	CFairy Fairy(172,160,3);
	bool BridgeShown,BridgeDrawing;
	int BridgeDrawnWidth;
	const Surface &Bridge = IMG[IMGBridge];
	Rect BridgeSrcRect = {0,0,0,0};
	const int BridgeX = 111, BridgeY = 188;
	BridgeShown = false;
	BridgeDrawing = false;
	BridgeDrawnWidth = 0;
	PlayMusicIfOn(MUS_Stage);
	switch (Level)
	{
		case 1:
			StageBlock.Load(1);
			break;
  		case 2:
			StageBlock.Load(2);
  			break;
  		case 3:
			StageBlock.Load(3);
  			break;
  		case 4:
			StageBlock.Load(4);
  			break;
  		case 5:
			StageBlock.Load(5);
  			break;
  		case 6:
			StageBlock.Load(6);
  			break;
  		case 8:
  			StageBlock.Load(7);
  			break;
  		case 9:
  			StageBlock.Load(8);
  			break;
  		case 11:
  			StageBlock.Load(9);
  			break;
  		case 35:
  			StageBlock.Load(10);
  			break;
  		default: Fairy.Hidden = true;
	}
	const char *Password = LevelPasswords[Level];
	const int PasswordX = 160 - TextWidth(font,Password) / 2;
	snprintf(ChrLevel,sizeof(ChrLevel),"%02d",Level+1);
	strcpy(NextLevel,ChrLevel);
	snprintf(ChrLevel,sizeof(ChrLevel),"%02d",Level);
	strcpy(PrevLevel,ChrLevel);
	ReadButtons();
	while (GameState == GSNextStage)
	{
		ReadButtons();
		if (Pressed(BUT_SELECT))
			GameState = GSTitleScreen;
		// up walks in too (as in the Playdate version). The room changes once the player is inside: the GP2X game counted every press while he
		// walked in, two presses skipped a room (fixed as in the Playdate version)
		if ((Player.State != EnterBuilding) && (Pressed(BUT_A) || Pressed(BUT_START) || Pressed(BUT_X) || Pressed(BUT_UP)))
		{
			if((Player.GetX() > 265) || (Player.GetX() < 32))
				Player.State = EnterBuilding;
		}
		BlitSurface(IMG[IMGBetweenStage],NULL,Buffer,0,0);
		Cloud1.Move();
  		Cloud1.Draw();
  		Cloud2.Move();
  		Cloud2.Draw();
  		Cloud3.Move();
  		Cloud3.Draw();
  		Cloud4.Move();
  		Cloud4.Draw();
		// the bridge builds itself once the player walks up to it
		if (!BridgeShown)
		{
			if(!BridgeDrawing)
			{
				if(Player.GetX() > 92)
				{
					BridgeDrawing = true;
					if (SoundEnabled)
						PlaySound(SND_Bridge);
				}
			}
			else
			{
				BridgeSrcRect.w = BridgeDrawnWidth;
				BridgeSrcRect.h = Bridge.h;
				BridgeDrawnWidth = BridgeDrawnWidth + 5;
				if (BridgeDrawnWidth >= Bridge.w)
				{
					BridgeDrawing = false;
					BridgeShown = true;
					BridgeSrcRect.w = Bridge.w;
				}
				BlitSurface(Bridge,&BridgeSrcRect,Buffer,BridgeX,BridgeY);
			}
		}
		else
			BlitSurface(Bridge,NULL,Buffer,BridgeX,BridgeY);
		Player.Move();
		Player.Draw();
		switch(Level)
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 8:
			case 9:
			case 11:
			case 35:
				Fairy.Draw();
				if((Player.State != LookingUp) && (! Fairy.Hidden) && (Player.GetX() + Player.GetWidth() >= Fairy.GetX()))
				{
					if (SoundEnabled)
						PlaySound(SND_Elf);
					Fairy.Hidden = true;
					Player.State = LookingUp;
				}
				if (Player.State == LookingUp)
				{
					StageBlock.Draw();
					StageBlock.Move();
					if( StageBlock.GetY() + StageBlock.GetHeight() >= Player.GetY())
					{
						Player.State = Waiting;
						StageBlock.Hidden = true;
					}
				}
				break;
		}
		SpaceShip.Move();
		if (SpaceShip.GetX() > PasswordX)
			WriteText(Buffer,font,Password,strlen(Password),PasswordX,25,0,Color1);
		SpaceShip.Draw();
		WriteText(Buffer,font,PrevLevel,strlen(PrevLevel),31,102,0,Color2);
		WriteText(Buffer,font,NextLevel,strlen(NextLevel),270,102,0,Color2);
		PresentBuffer(255);
		FramerateDelay();
		if (Player.State == EnteredBuilding)
		{
			if (Player.GetX() > 265)
				Level++;
			GameState = GSGame;
		}
	}
	HaltMusic();
}

// before the first room: there is only the building of room 1 to walk into
static void NextStageLevel0()
{
	CPlayer Player(225,167,225,268);
	CCloud Cloud1(259,15,-0.40,Big);
	CCloud Cloud2(185,45,-0.25,Small);
	CCloud Cloud3(134,7,-0.40,Big);
	CCloud Cloud4(50,50,-0.25,Small);
	Color Color1 ={0,0,0};
	char ChrLevel[5];
	PlayMusicIfOn(MUS_Stage);
	snprintf(ChrLevel,sizeof(ChrLevel),"%02d",Level+1);
	ReadButtons();
	while (GameState == GSNextStage)
	{
		ReadButtons();
		if (Pressed(BUT_SELECT))
		{
			HaltMusic();
			GameState = GSTitleScreen;
		}
		if ((Player.State != EnterBuilding) && (Pressed(BUT_A) || Pressed(BUT_START) || Pressed(BUT_X) || Pressed(BUT_UP)))
		{
			if(Player.GetX() > 265)
				Player.State = EnterBuilding;
		}
		BlitSurface(IMG[IMGBetweenStageLevel1],NULL,Buffer,0,0);
		WriteText(Buffer,font,ChrLevel,strlen(ChrLevel),270,105,0,Color1);
		Cloud1.Move();
		Cloud1.Draw();
		Cloud2.Move();
		Cloud2.Draw();
		Cloud3.Move();
		Cloud3.Draw();
		Cloud4.Move();
		Cloud4.Draw();
		Player.Move();
		Player.Draw();
		if (Player.State == EnteredBuilding)
		{
			Level++;
			GameState = GSGame;
		}
		PresentBuffer(255);
		FramerateDelay();
	}
	HaltMusic();
}

void NextStage()
{
	if (Level ==0)
		NextStageLevel0();
	else
		NextStageLevel1to35();
}
