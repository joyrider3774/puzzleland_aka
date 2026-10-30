#include <string.h>

#include "commonvars.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "states.h"

void Credits()
{
	const char *Tekst = "Puzzle Land is programmed by\nWillems Davy. Graphics are made\nusing Paint Shop Pro 7. A Big thanks\ngoes out to Yann R. Fernandez for\ncreating some of the graphics.\nPuzzle Land is a remake of\nDaedalian opus for gameboy by\nVic Tokai Inc. 1990\n\nWillems Soft 2006";
	Color Color1 = {0,0,0};
	int NrOfChars = 0;
	ReadButtons();
	while (GameState == GSCredits)
	{
		// any button goes back to the title screen
		if (ReadButtons())
			GameState = GSTitleScreen;
		if (NrOfChars < (int)strlen(Tekst))
			NrOfChars++;
		BlitSurface(IMG[IMGPaper],NULL,Buffer,0,0);
		WriteText(Buffer,font,"CREDITS",7,Buffer.w / 2 - 32,20,0,Color1,true);
		WriteText(Buffer,font,Tekst,NrOfChars,30,40,0,Color1);
		PresentBuffer(255);
		FramerateDelay();
	}
}
