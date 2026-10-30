// The selectors of the menus: the title screen, the password, the room select and the options
#ifndef CSELECTORS_H
#define CSELECTORS_H

class CTitleScreenSelector
{
 public:
 	CTitleScreenSelector();
 	void MoveUp();
 	void MoveDown();
 	void Draw();
 	int GetSelection();
 private:
	int Selection;
};

class CPasswordSelector
{
 public:
 	CPasswordSelector();
 	void MoveUp();
 	void MoveDown();
 	void MoveLeft();
 	void MoveRight();
 	void Draw();
 	int GetX();
 	int GetY();
 private:
 	int X,Y;
};

class CStageSelectSelector
{
 public:
 	CStageSelectSelector();
 	void MoveLeft();
 	void MoveRight();
 	void MoveUp();
 	void MoveDown();
 	void Draw();
 	int GetSelection();
 private:
 	int X,Y;
};

class COptionsSelector
{
 public:
 	COptionsSelector();
 	void MoveUp();
 	void MoveDown();
 	void Draw();
 	int GetSelection();
 private:
 	int Selection;
};

#endif
