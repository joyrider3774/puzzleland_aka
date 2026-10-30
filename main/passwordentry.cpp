#include <string.h>

#include "CSelectors.h"
#include "commonvars.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

// the room a password is for, -1 for none
static int GetLevel(const char *Password)
{
	int Result,Teller;
	Result = -1;
	for (Teller = 0;Teller < 36;Teller++)
	{
		if (strcmp(Password,LevelPasswords[Teller]) ==0)
		{
			Result = Teller;
			break;
		}
	}
	return Result;
}

void PasswordEntry()
{
	Color Color1 = {0,0,0};
	int CharNr = 0;
	int LetterX,LetterY;
	char Password[5] = "    ";
	CPasswordSelector Selector;
	ReadButtons();
	while (GameState == GSPasswordEntry)
	{
		ReadButtons();
		if (Pressed(BUT_SELECT) || Pressed(BUT_Y) || Pressed(BUT_B))
			GameState = GSTitleScreen;
		if (Pressed(BUT_X) || Pressed(BUT_A))
		{
			Password[CharNr] = Letters[Selector.GetY()][Selector.GetX()][0];
			if (SoundEnabled)
				PlaySound(SND_Select);
			CharNr++;
			if (CharNr > 3)
				CharNr = 0;
		}
		if (Pressed(BUT_UP))
			Selector.MoveUp();
		if (Pressed(BUT_DOWN))
			Selector.MoveDown();
		if (Pressed(BUT_LEFT))
			Selector.MoveLeft();
		if (Pressed(BUT_RIGHT))
			Selector.MoveRight();

		BlitSurface(IMG[IMGPaper],NULL,Buffer,0,0);
		WriteText(Buffer,font,Password,4,Buffer.w / 2 - TextWidth(font,Password) / 2,37,0,Color1);
		for (LetterY = 0 ;LetterY < LetterRows;LetterY++)
			for (LetterX = 0;LetterX < LetterCols;LetterX++)
			{
				if(strcmp(Letters[LetterY][LetterX], "0") != 0)
					WriteText(Buffer,font,Letters[LetterY][LetterX],1,XOffsetPassword + LetterX * 35,YOffsetPassword + LetterY * 35,0,Color1);
			}
		Selector.Draw();
		PresentBuffer(255);
		FramerateDelay();
		if (Password[3] != ' ')
		{
			Delay(250);
			if (GetLevel(Password) == -1)
			{
				Password[0] = ' ';
				Password[1] = ' ';
				Password[2] = ' ';
				Password[3] = ' ';
				if (SoundEnabled)
					PlaySound(SND_Error);
			}
			else
				if(GetLevel(Password) == 0)
				{
					GameState = GSStageSelect;
				}
				else
				{
					Level = GetLevel(Password);
					GameState = GSOldManSpeaking;
					HaltMusic();
				}
		}
	}
}
