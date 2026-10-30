#include <string.h>

#include "CBetweenStage.h"
#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

void OldManSpeaking()
{
	const char* const Tekst[5] = {"Welcome to the world of\npuzzle land!\nIn level one we'll start with\nsimple three-block\ncombinations ...$When you've mastered level\none, we'll add another block.\nHang in there!",
								  "Congratulations you made it!\nNow we'll boggle your mind,\nthere are 2339 diffrent\nconfigurations in this stage!$Don't give up!\nRelax!",
								  "Hah! This next stage will\nsurprise you!\nYou now have 12 diffrent\nshapes to work with ...$Show me how far you can\ngo dude!",
								  "Well, well, well.\nYou seem to have made it all\nthis way! As a token of our\nappreciation for your skill,...$Here's another square block!\nTake the square block, and try\nit for the last level!$It looks simple, but there are\n16146 configurations!\n\nHey dude, you're on it again.",
          						  "Congratulations you made it!\nYou've earned the password\nfor the level select!\nthe password is 'davy'"};
    Color Color1 = {0,0,0};
    int Lines,Chars,TextDelay,NrOfChars,PageNr,TekstNr;
    char List[10][255];
	CFairy Fairy(240,175,6);
	ReadButtons();
	switch (Level)
	{
		case 0:
		case 11:
		case 12:
		case 35:
		case 36:
			PlayMusicIfOn(MUS_Oldman);
			TextDelay = 0;
			NrOfChars = 0;
			Lines = 0;
			Chars =0;
			PageNr = 0;
			switch (Level)
			{
				case 0:
					TekstNr = 0;
					break;
				case 11:
					TekstNr = 1;
					break;
				case 12:
					TekstNr = 2;
					break;
				case 35:
					TekstNr = 3;
					break;
				case 36:
					TekstNr = 4;
					break;
				default: TekstNr = 0;
			}
			// the pages of the text are split on '$'
			for(size_t Teller=0;Teller<strlen(Tekst[TekstNr]);Teller++)
			{
				if (Tekst[TekstNr][Teller] == '$')
				{
					List[Lines][Chars] = '\0';
					Lines++;
					Chars=0;
				}
				else
				{
					List[Lines][Chars] = Tekst[TekstNr][Teller];
					Chars++;
				}
			}
			List[Lines][Chars] = '\0';
			while (GameState == GSOldManSpeaking)
			{
				ReadButtons();
				if (Pressed(BUT_A) || Pressed(BUT_X))
				{
					if (PageNr < Lines)
					{
						PageNr++;
						NrOfChars = 0;
					}
					else
						if (Level < 36)
							GameState = GSNextStage;
						else
							GameState = GSCredits;
				}
				if (Pressed(BUT_SELECT))
				{
					HaltMusic();
					GameState = GSTitleScreen;
				}
				if (Pressed(BUT_Y) || Pressed(BUT_B))
				{
					if (Level < 36)
						GameState = GSNextStage;
					else
						GameState = GSCredits;
				}
				BlitSurface(IMG[IMGOldMan],NULL,Buffer,0,0);
				Fairy.Draw();
				TextDelay++;
				if (TextDelay == 2)
				{
					if (NrOfChars < (int)strlen(List[PageNr]))
						NrOfChars++;
					TextDelay = 0;
				}
				WriteText(Buffer,font,List[PageNr],NrOfChars,50,50,5,Color1);
				PresentBuffer(255);
				FramerateDelay();
			}
			break;
		default:
			GameState = GSNextStage;
			break;
	}
}
