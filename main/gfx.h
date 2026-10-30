// The part of SDL, SDL_image and SDL_ttf the game used, on the AKA library's framebuffer.
//
// A surface is 16 bit pixels in the panel's own order (see lcd_color_rgb) and, when it is not
// opaque, an alpha plane next to them.
//
// The game drew every frame into Buffer and blitted that onto the screen. Here Buffer is the same
// and Screen is the framebuffer.
#ifndef GFX_H
#define GFX_H

#include <stddef.h>
#include <stdint.h>
#include "assets.h"

struct Surface {
    int16_t w, h;
    uint16_t* px;
    uint8_t* a;         // NULL: opaque
};

struct Rect {
    int16_t x, y, w, h;
};

struct Color {
    uint8_t r, g, b;
};

// the screen, the library's framebuffer, and the buffer a frame is drawn in
extern Surface Screen, Buffer;
extern const AssetFont *font;

void GfxInit();
// Buffer onto the screen with alpha (255: as it is) and the screen to the panel
void PresentBuffer(uint8_t alpha);

uint16_t MapRGB(uint8_t r, uint8_t g, uint8_t b);
// an image built into the firmware
Surface SurfaceFromAsset(const AssetSurface& Asset);
// SDL_BlitSurface of src (or the part srcRect of it) with its top left corner at x, y
void BlitSurface(const Surface& src, const Rect* srcRect, Surface& dst, int x, int y);
// boxRGBA: x1,y1 to x2,y2 inclusive, blended with alpha a
void BoxRGBA(Surface& dst, int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
// WriteText of the game: the first NrOfChars characters of Tekst, lines split on newlines, each
// TTF_FontLineSkip + YSpacing below the one before; Underline is TTF_STYLE_UNDERLINE
void WriteText(Surface& dst, const AssetFont* Font, const char* Tekst, int NrOfChars, int X, int Y, int YSpacing,
               Color ColorIn, bool Underline = false);
// the width of a line of text, what TTF_RenderText_Solid's surface would be wide
int TextWidth(const AssetFont* Font, const char* Tekst);

#endif
