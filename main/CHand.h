#ifndef CHAND_H
#define CHAND_H

// the hand that picks up the blocks in a room
class CHand
{
 public:
 	CHand();
 	void Move();
 	void Draw();
 	void SetPosition(const int XIn, const int YIn);
 	void Hide();
 	void Show();
 	int GetPlayFieldX();
 	int GetPlayFieldY();
 private:
 	int X,Y;
 	bool Hidden;
};

#endif
