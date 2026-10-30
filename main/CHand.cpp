#include "CHand.h"
#include "Common.h"
#include "input.h"

CHand::CHand()
{
	Hidden = false;
	X = 0;
	Y = 0;
}

void CHand::Draw()
{
	if (!Hidden)
		BlitSurface(IMG[IMGHand],NULL,Buffer,MinPlayAreaX + X * BlockWidth,MinPlayAreaY + Y * BlockHeight);
}

void CHand::Move()
{
	if (!Hidden && DirectionReady())
	{
		if (Held(BUT_UP))
		{
			if (Y - 1 >= 0)
				Y = Y - 1;
		}
		else if (Held(BUT_DOWN))
		{
			if (Y + 1 < Rows)
				Y = Y + 1;
		}
		else if (Held(BUT_LEFT))
		{
			if (X - 1 >= 0)
				X = X - 1;
		}
		else if (Held(BUT_RIGHT))
		{
			if (X + 1 < Cols)
				X = X + 1;
		}
		else if (Held(BUT_UPRIGHT))
		{
			if (Y - 1 >= 0)
				Y = Y - 1;
			if (X + 1 < Cols)
				X = X + 1;
		}
		else if (Held(BUT_UPLEFT))
		{
			if (Y - 1 >= 0)
				Y = Y - 1;
			if (X - 1 >= 0)
				X = X - 1;
		}
		else if (Held(BUT_DOWNRIGHT))
		{
			if (Y + 1 < Rows)
				Y = Y + 1;
			// the GP2X version had X + 1 <= Cols here, which let the hand go a column past the room
			if (X + 1 < Cols)
				X = X + 1;
		}
		else if (Held(BUT_DOWNLEFT))
		{
			if (Y + 1 < Rows)
				Y = Y + 1;
			if (X - 1 >= 0)
				X = X - 1;
		}
	}
}

void CHand::SetPosition(const int XIn,const int YIn)
{
	X = XIn;
	Y = YIn;
}

int CHand::GetPlayFieldX()
{
	return X;
}

int CHand::GetPlayFieldY()
{
	return Y;
}

void CHand::Hide()
{
	Hidden = true;
}

void CHand::Show()
{
	Hidden = false;
}
