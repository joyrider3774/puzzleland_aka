#include <stdio.h>

#include "CBetweenStage.h"
#include "input.h"

// ---------------------------------------------------------------------------- CPlayer

CPlayer::CPlayer(const int Xin,const int Yin,const int MinXin, const int MaxXin)
{
 	Y = Yin;
	X = Xin;
 	Width = 23;
 	Height = 28;
 	AnimCounter = 0;
 	Delay = 0;
 	AnimPhase = 0;
 	MaxX = MaxXin;
 	MinX = MinXin;
 	State = Waiting;
}

void CPlayer::Draw()
{
	Rect SrcRect;
	// the shadow at half alpha, see tools/convert_assets.py
 	BlitSurface(IMG[IMGShadow],NULL,Buffer,X + 1,Y + 26);
 	SrcRect.x = AnimPhase * Width;
 	SrcRect.y = 0;
 	SrcRect.w = Width;
 	SrcRect.h = Height;
	BlitSurface(IMG[IMGPlayer],&SrcRect,Buffer,X,Y);
}

int CPlayer::GetX()
{
	return X;
}

int CPlayer::GetY()
{
	return Y;
}

int CPlayer::GetWidth()
{
	return Width;
}

void CPlayer::Move()
{

	if ((State == Walking) || (State==Waiting))
	{
   		if(Held(BUT_LEFT))
		{
			if(X - 3 >= MinX)
			{
				X = X -3;
				AnimPhase = AnimLeft[AnimCounter];
				State = Walking;
			}
			else
				State = Waiting;
		}
 		else
			if(Held(BUT_RIGHT))
			{
				if(X + 3 <= MaxX)
				{
					X = X +3;
					AnimPhase = AnimRight[AnimCounter];
					State = Walking;
				}
				else
					State = Waiting;
			}
			else
				State = Waiting;


	}
	if (State == Walking)
	{
     	AnimCounter++;
     	if (AnimCounter > 5)
     		AnimCounter = 0;
    }
  	if (State == EnterBuilding)
  	{
  		Delay++;
  		if(Delay==3)
  		{
  			AnimCounter++;
  			Y = Y - 1;
  			Delay = 0;
  		}
  		if (AnimCounter > 3)
  		{
  			AnimCounter = 0;
			if(Y <=160)
			  State = EnteredBuilding;
		}
 		AnimPhase = AnimEnterBuilding[AnimCounter];
	}
	if (State == Waiting)
	{
		if ((AnimPhase != 0) && (AnimPhase !=13))
		{
			if (AnimPhase==14)
				AnimPhase = 0;
			else
				if (AnimPhase < 7)
					AnimPhase = 0;
				else
					AnimPhase=13;
		}
	}
 	if (State ==LookingUp)
 		AnimPhase = 14;

}

// ---------------------------------------------------------------------------- CSpaceShip

CSpaceShip::CSpaceShip()
{
	X=-32;
	Y= 28;
}

void CSpaceShip::Draw()
{
	BlitSurface(IMG[IMGSpaceShip],NULL,Buffer,X,Y);
}

void CSpaceShip::Move()
{
	if (X < 352)
		X = X + 3;
}

int CSpaceShip::GetX()
{
	return X;
}

// ---------------------------------------------------------------------------- CCloud

CCloud::CCloud(const int XIn,const int YIn,float XiIn,CloudStyles Style)
{
	if (Style == Big)
	{
		Image = &IMG[IMGCloud];
		Width = 60;
		Height = 28;
	}
	else
	{
		Image = &IMG[IMGSmallCloud];
		Width=30;
		Height=14;
	 }
	X = XIn;
	Y = YIn;
	Xi = XiIn;
}

void CCloud::Draw()
{
	// the GP2X version drew the part of the cloud that is on the screen in three cases; the blit
	// clips to the screen itself
	if ((X >= -Width) && (X < 320))
		BlitSurface(*Image,NULL,Buffer,(int)X,Y);
}

void CCloud::Move()
{
	if ((X > -Width) && (X < 320))
		X = X + Xi;
	else
		if (X <= -Width)
			X = 319;
		else
			X = -Width + 1;
}

// ---------------------------------------------------------------------------- CFairy

CFairy::CFairy(const int XIn, const int YIn,const int AnimDelayIn)
{
	X = XIn;
	Y = YIn;
	Width = 18;
	Height = 22;
	AnimPhase = 0;
	Delay = 0;
	Hidden = false;
	AnimDelay = AnimDelayIn;
}

void CFairy::Draw()
{
	Rect SrcRect;
	if (!Hidden)
	{
		SrcRect.x = AnimPhase * Width;
		SrcRect.y = 0;
		SrcRect.w = Width;
		SrcRect.h = Height;
		BlitSurface(IMG[IMGFairy],&SrcRect,Buffer,X,Y);
		Delay++;
		if (Delay >= AnimDelay)
		{
			AnimPhase++;
			Delay = 0;
			if (AnimPhase == 2)
				AnimPhase = 0;
		}
	}
}

int CFairy::GetX()
{
	return X;
}

// ---------------------------------------------------------------------------- CStageBlock

CStageBlock::CStageBlock()
{
	Image = NULL;
	Hidden = false;
	X = Y = Yi = Width = Height = 0;
}

void CStageBlock::Load(const int BlockNr)
{
	Image = &IMG[IMGStageBlock1 + BlockNr - 1];
	Hidden = false;
	Yi = 3;
	X = 160 - Image->w / 2;
	Y = -Image->h;
	Width = Image->w;
	Height = Image->h;
}

void CStageBlock::Draw()
{
	if (!Hidden && Image)
		BlitSurface(*Image,NULL,Buffer,X,Y);
}

void CStageBlock::Move()
{
	if (!Hidden)
		Y = Y + Yi;
}

int CStageBlock::GetY()
{
	return Y;
}

int CStageBlock::GetHeight()
{
	return Height;
}
