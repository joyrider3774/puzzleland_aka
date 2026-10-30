#include "CSelectors.h"
#include "Common.h"
#include "commonvars.h"
#include "sound.h"

static void MenuSound()
{
	if (SoundEnabled)
		PlaySound(SND_Menu);
}

// ---------------------------------------------------------------------------- CTitleScreenSelector

CTitleScreenSelector::CTitleScreenSelector()
{
    Selection = 1;
}

void CTitleScreenSelector::MoveUp()
{
	if (Selection > 1)
	{
		Selection--;
		MenuSound();
	}
}

void CTitleScreenSelector::MoveDown()
{
	if (Selection < 4)
	{
		Selection++;
		MenuSound();
	}
}

void CTitleScreenSelector::Draw()
{
	static const int Ys[4] = {66, 100, 136, 175};
	BlitSurface(IMG[IMGTitleSelector],NULL,Buffer,71,Ys[Selection - 1]);
}

int CTitleScreenSelector::GetSelection()
{
	return Selection;
}

// ---------------------------------------------------------------------------- CPasswordSelector

CPasswordSelector::CPasswordSelector()
{
	X = 0;
	Y = 0;
}

void CPasswordSelector::Draw()
{
	BlitSurface(IMG[IMGSelect],NULL,Buffer,XOffsetPassword + X * 36,YOffsetPassword + Y * 36);
}

void CPasswordSelector::MoveDown()
{
	if (Y < LetterRows - 2)
	{
		Y = Y + 1;
		MenuSound();
	}
	else
		if ((Y == LetterRows -2) && (X < LetterCols -2))
		{
			Y = Y +1;
			MenuSound();
		}
}

void CPasswordSelector::MoveLeft()
{
	if (X > 0)
	{
		X = X -1;
		MenuSound();
	}
}

void CPasswordSelector::MoveRight()
{
	if (Y < LetterRows -1)
		if (X < LetterCols -1)
		{
			X = X +1;
			MenuSound();
		}
	if (Y == LetterRows - 1)
		if (X < LetterCols -3)
		{
			X = X +1;
			MenuSound();
		}
}

void CPasswordSelector::MoveUp()
{
	if (Y > 0)
	{
		Y = Y - 1;
		MenuSound();
	}
}

int CPasswordSelector::GetX()
{
	return X;
}

int CPasswordSelector::GetY()
{
	return Y;
}

// ---------------------------------------------------------------------------- CStageSelectSelector

CStageSelectSelector::CStageSelectSelector()
{
	X = 0;
	Y = 0;
}

void CStageSelectSelector::Draw()
{
	BlitSurface(IMG[IMGSelect],NULL,Buffer,XOffsetStageSelect + X * 82 + 52,YOffsetStageSelect + Y * 14+2);
}

void CStageSelectSelector::MoveDown()
{
	if (Y < 11)
	{
		Y++;
		MenuSound();
	}
}

void CStageSelectSelector::MoveLeft()
{
	if (X > 0)
	{
		X--;
		MenuSound();
	}
}

void CStageSelectSelector::MoveRight()
{
	if (X < 2)
	{
		X++;
		MenuSound();
	}
}

void CStageSelectSelector::MoveUp()
{
	if ( Y > 0)
	{
		Y--;
		MenuSound();
	}
}

int CStageSelectSelector::GetSelection()
{
	return  X + Y * 3;
}

// ---------------------------------------------------------------------------- COptionsSelector

COptionsSelector::COptionsSelector()
{
	Selection = 1;
}

void COptionsSelector::Draw()
{
	static const int Ys[3] = {42, 62, 82};
	BlitSurface(IMG[IMGOptionsSelect],NULL,Buffer,100,Ys[Selection - 1]);
}

int COptionsSelector::GetSelection()
{
	return Selection;
}

void COptionsSelector::MoveDown()
{
	// the GP2X version stopped at the music line, it had volume buttons; here the volume line
	// can be selected too and left and right change it
	if (Selection < 3)
	{
		Selection++;
		MenuSound();
	}
}

void COptionsSelector::MoveUp()
{
	if (Selection > 1)
	{
		Selection--;
		MenuSound();
	}
}
