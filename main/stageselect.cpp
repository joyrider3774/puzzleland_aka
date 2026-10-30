#include <stdio.h>

#include "CSelectors.h"
#include "commonvars.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

void StageSelect()
{
	int Teller;
	Color Color1 = {0,0,0};
	char ChrRoom[10];
	CStageSelectSelector Selector;
	ReadButtons();
	while (GameState == GSStageSelect)
	{
		ReadButtons();
		// back to the password entry the room select is reached from (the GP2X went to the title
		// screen, the Playdate version goes back here)
		if (Pressed(BUT_SELECT) || Pressed(BUT_Y) || Pressed(BUT_B))
			GameState = GSPasswordEntry;
		if (Pressed(BUT_A) || Pressed(BUT_X) || Pressed(BUT_START))
		{
			if (SoundEnabled)
				PlaySound(SND_Select);
			Level = Selector.GetSelection();
			GameState = GSOldManSpeaking;
			HaltMusic();
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
		WriteText(Buffer,font,"ROOM SELECT",11,Buffer.w / 2 - TextWidth(font,"ROOM SELECT") / 2,20,0,Color1);
		for (Teller=0;Teller< 36;Teller++)
		{
			snprintf(ChrRoom,sizeof(ChrRoom),"Room %d",Teller+1);
			WriteText(Buffer,font,ChrRoom,10,XOffsetStageSelect + (Teller % 3) * 82,YOffsetStageSelect + (Teller / 3) * 14,0,Color1);
		}
		Selector.Draw();
		PresentBuffer(255);
		FramerateDelay();
	}
}
