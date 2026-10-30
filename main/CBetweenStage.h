// The sprites of the screens between the rooms: the player, the space ship, the clouds, the
// fairy and the block the fairy drops
#ifndef CBETWEENSTAGE_H
#define CBETWEENSTAGE_H

#include "Common.h"
#include "gfx.h"

class CPlayer
{
 public:
 	CPlayer(const int Xin, const int Yin,const int MinXin,const int MaxXin);
 	void Move();
 	void Draw();
 	int GetX();
 	int GetY();
 	int GetWidth();
 	PlayerStates State;
 private:
 	int X,Y,AnimPhase,AnimCounter,Delay,Width,Height,MinX,MaxX;
};

class CSpaceShip
{
 public:
 	CSpaceShip();
 	void Move();
 	void Draw();
 	int GetX();
 private:
 	int X,Y;
};

class CCloud
{
 public:
 	CCloud(const int XIn,const int YIn,const float XiIn,CloudStyles Style);
	void Move();
	void Draw();
 private:
 	float X,Xi;
 	int Y,Width,Height;
 	const Surface* Image;
};

class CFairy
{
 public:
 	CFairy(const int XIn,const int YIn,const int AnimDelay);
 	void Draw();
 	int GetX();
 	bool Hidden;
 private:
 	int X,Y,Width,Height,Delay,AnimPhase,AnimDelay;
};

class CStageBlock
{
 public:
 	CStageBlock();
 	void Move();
 	void Draw();
 	int GetHeight();
 	int GetY();
 	void Load(const int BlockNr);
 	bool Hidden;
 private:
 	int X,Y,Yi,Width,Height;
 	const Surface* Image;
};

#endif
