// The rooms: the blocks and borders on the playfield, moving, flipping and rotating the block the
// hand picked up, loading a room and seeing if it is cleared
#ifndef PLAYFIELD_H
#define PLAYFIELD_H

void MoveBlockRight();
void MoveBlockLeft();
void MoveBlockUp();
void MoveBlockDown();
void MakeBlockActive(const int X,const int Y,const int BlockNr);
bool MakeBlockUnActive();
void FlipBlock(const bool Horizontal);
void RotateBlock();
void DrawPlayField();
// room Level from DATA_DIR/levels
void LoadLevel();
bool IsStageClear();
// the room number and the time at the top of a room
void DrawPanel();

#endif
