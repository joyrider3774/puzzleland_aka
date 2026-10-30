// The GP2X buttons of the game on the AKA's keys.
//
// The game read the GP2X's buttons two ways: held (SDL_JoystickGetButton, to move the hand, a
// block or the player) and pressed (SDL_JOYBUTTONDOWN, everything else). Both are here, for the
// buttons as the GP2X had them:
//   d-pad / joystick   BUT_UP, BUT_DOWN, BUT_LEFT, BUT_RIGHT; two of them together are one of the
//                      GP2X's diagonal buttons (BUT_UPLEFT ...) and not the two directions
//   A                  BUT_A        select, pick up and drop a block
//   B                  BUT_B        back; in a room: rotate the block
//   C                  BUT_X        in a room: flip the block left to right
//   D                  BUT_Y        back; in a room: flip the block upside down
//   R1                 BUT_START    in a room: start the room over
//   MENU               BUT_SELECT   back to the title screen
#ifndef INPUT_H
#define INPUT_H

enum Buttons {
    BUT_UP, BUT_DOWN, BUT_LEFT, BUT_RIGHT, BUT_UPLEFT, BUT_UPRIGHT, BUT_DOWNLEFT, BUT_DOWNRIGHT,
    BUT_A, BUT_B, BUT_X, BUT_Y, BUT_START, BUT_SELECT, NROFBUTTONS
};

// reads the keys once a frame, true when any button went down
bool ReadButtons();
// the button is held (SDL_JoystickGetButton)
bool Held(int Button);
// the button went down at the last ReadButtons (SDL_JOYBUTTONDOWN)
bool Pressed(int Button);
// the hand or the block in a room may move a cell this frame: straight away when a direction goes
// down, then every other frame while it is held, 10 cells a second (the GP2X moved one every
// frame; slowed down as in the Playdate version). Called once a frame by what moves.
bool DirectionReady();

#endif
