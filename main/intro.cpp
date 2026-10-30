#include "commonvars.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "states.h"

// The SDL powered and Willems Soft pictures and the title: each next picture closes in from the
// edges over the one before, as a rectangle in the middle (x by y) shrinks. The GP2X version did
// this with a copy of the next picture with a magenta (colour keyed) hole in it
void Intro()
{
    int Pictnr,x,y;
	x = 320;
	y = 240;
	Pictnr = 1;
	// picture 1 is a black screen
	const Surface *Picts[4] = {NULL, &IMG[IMGSdlPowered], &IMG[IMGWillemsSoft], &IMG[IMGTitle]};
	ReadButtons();
    while (GameState == GSIntro)
    {
		// any button goes to the title screen (the GP2X's SELECT quit, only the title screen's
		// Quit entry and RUN + MENU leave the game on the AKA)
		if (ReadButtons())
			GameState = GSTitleScreen;

		// the picture there is, and the next one around the rectangle in the middle
		const Surface *Current = Picts[Pictnr - 1], *Next = Picts[Pictnr];
		BlitSurface(*Next,NULL,Buffer,0,0);
		Rect Hole;
		Hole.x = 160 - x/2;
		Hole.y = 120 - y/2;
		Hole.w = x;
		Hole.h = y;
		if ((Hole.w > 0) && (Hole.h > 0))
		{
			if (Current)
				BlitSurface(*Current,&Hole,Buffer,Hole.x,Hole.y);
			else
				BoxRGBA(Buffer,Hole.x,Hole.y,Hole.x + Hole.w - 1,Hole.y + Hole.h - 1,0,0,0,255);
		}
		PresentBuffer(255);
		FramerateDelay();
        x -= 20;
        if (y >0)
        {
        	y -= 15;
        }
        else
        {
        	Pictnr++;
        	if (Pictnr == 4)
        	{
				GameState = GSTitleScreen;
        	}
        	else
        	Delay(2000);
        	x = 320;
        	y = 120;
        }
    }
}
