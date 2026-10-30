#include "input.h"

#include <stdint.h>

#include "gb_common.h"
#include "platform.h"

static uint32_t HeldButtons = 0, PressedButtons = 0;

bool ReadButtons()
{
    const uint16_t Keys = PollKeys();
    const bool Up = Keys & GB_KEY_UP, Down = Keys & GB_KEY_DOWN, Left = Keys & GB_KEY_LEFT, Right = Keys & GB_KEY_RIGHT;
    uint32_t Now = 0;
    // the GP2X's stick gave a diagonal as a button of its own, not as two directions
    if (Up && Left)
        Now |= 1 << BUT_UPLEFT;
    else if (Up && Right)
        Now |= 1 << BUT_UPRIGHT;
    else if (Down && Left)
        Now |= 1 << BUT_DOWNLEFT;
    else if (Down && Right)
        Now |= 1 << BUT_DOWNRIGHT;
    else if (Up)
        Now |= 1 << BUT_UP;
    else if (Down)
        Now |= 1 << BUT_DOWN;
    else if (Left)
        Now |= 1 << BUT_LEFT;
    else if (Right)
        Now |= 1 << BUT_RIGHT;
    if (Keys & GB_KEY_A) Now |= 1 << BUT_A;
    if (Keys & GB_KEY_B) Now |= 1 << BUT_B;
    if (Keys & GB_KEY_C) Now |= 1 << BUT_X;
    if (Keys & GB_KEY_D) Now |= 1 << BUT_Y;
    if (Keys & GB_KEY_R1) Now |= 1 << BUT_START;
    if (Keys & GB_KEY_MENU) Now |= 1 << BUT_SELECT;
    PressedButtons = Now & ~HeldButtons;
    HeldButtons = Now;
    return PressedButtons != 0;
}

bool Held(int Button)
{
    return (HeldButtons >> Button) & 1;
}

bool Pressed(int Button)
{
    return (PressedButtons >> Button) & 1;
}

// the frames to wait after a move while a direction stays held
#define MOVE_REPEAT_FRAMES 1

bool DirectionReady()
{
    static int Wait = 0;
    const uint32_t Directions = (1 << BUT_UP) | (1 << BUT_DOWN) | (1 << BUT_LEFT) | (1 << BUT_RIGHT) |
                                (1 << BUT_UPLEFT) | (1 << BUT_UPRIGHT) | (1 << BUT_DOWNLEFT) | (1 << BUT_DOWNRIGHT);
    if (!(HeldButtons & Directions))
    {
        Wait = 0;
        return false;
    }
    if (Wait > 0)
    {
        Wait--;
        return false;
    }
    Wait = MOVE_REPEAT_FRAMES;
    return true;
}
