#include "gfx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gamebuino.h"
#include "esp_heap_caps.h"
#include "platform.h"

// assets.S: assets.bin, made by tools/convert_assets.py
extern const uint8_t dm_assets[];

static gb_graphics gfx;

Surface Screen = {SCREEN_WIDTH, SCREEN_HEIGHT, framebuffer, NULL};
Surface Buffer;
const AssetFont *font = &ASSET_FONT;

void GfxInit()
{
    gfx.set_backlight_percent(80);
    Buffer.w = SCREEN_WIDTH;
    Buffer.h = SCREEN_HEIGHT;
    Buffer.a = NULL;
    // drawn all over every frame: internal RAM when there is room for it, PSRAM otherwise
    Buffer.px = (uint16_t*)heap_caps_malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (!Buffer.px)
        Buffer.px = (uint16_t*)AllocLarge(SCREEN_WIDTH * SCREEN_HEIGHT * 2);
    memset(Buffer.px, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 2);
    memset(Screen.px, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 2);
    gfx.update();
}

uint16_t MapRGB(uint8_t r, uint8_t g, uint8_t b)
{
    return lcd_color_rgb(r, g, b);
}

Surface SurfaceFromAsset(const AssetSurface& Asset)
{
    Surface s;
    s.w = Asset.w;
    s.h = Asset.h;
    s.px = (uint16_t*)(dm_assets + Asset.pixels);
    s.a = (Asset.alpha < 0) ? NULL : (uint8_t*)(dm_assets + Asset.alpha);
    return s;
}

// s over d with alpha 0..255. The panel's order has green in the middle as RGB565 does, so the
// usual trick of spreading the three fields over 32 bits works for it as it stands
static inline uint16_t Blend(uint16_t s, uint16_t d, uint32_t alpha)
{
    uint32_t a = (alpha + 4) >> 3;
    uint32_t sx = (s | ((uint32_t)s << 16)) & 0x07E0F81F;
    uint32_t dx = (d | ((uint32_t)d << 16)) & 0x07E0F81F;
    uint32_t r = (dx + (((sx - dx) * a) >> 5)) & 0x07E0F81F;
    return (uint16_t)(r | (r >> 16));
}

static inline void PutPixel(uint16_t* dp, uint16_t s, uint32_t alpha)
{
    if (alpha >= 255)
        *dp = s;
    else if (alpha)
        *dp = Blend(s, *dp, alpha);
}

void PresentBuffer(uint8_t alpha)
{
    const int Count = SCREEN_WIDTH * SCREEN_HEIGHT;
    if (alpha == 255)
        memcpy(Screen.px, Buffer.px, Count * 2);
    else if (alpha)
        for (int i = 0; i < Count; i++)
            Screen.px[i] = Blend(Buffer.px[i], Screen.px[i], alpha);
    gfx.update();
}

void BlitSurface(const Surface& src, const Rect* srcRect, Surface& dst, int x, int y)
{
    if (!src.px)
        return;
    int sx = 0, sy = 0, w = src.w, h = src.h;
    if (srcRect)
    {
        sx = srcRect->x;
        sy = srcRect->y;
        w = srcRect->w;
        h = srcRect->h;
    }
    if (sx < 0) { w += sx; x -= sx; sx = 0; }
    if (sy < 0) { h += sy; y -= sy; sy = 0; }
    if (sx + w > src.w) w = src.w - sx;
    if (sy + h > src.h) h = src.h - sy;
    if (x < 0) { w += x; sx -= x; x = 0; }
    if (y < 0) { h += y; sy -= y; y = 0; }
    if (x + w > dst.w) w = dst.w - x;
    if (y + h > dst.h) h = dst.h - y;
    if ((w <= 0) || (h <= 0))
        return;
    for (int row = 0; row < h; row++)
    {
        const uint16_t* sp = src.px + (sy + row) * src.w + sx;
        uint16_t* dp = dst.px + (y + row) * dst.w + x;
        if (!src.a)
        {
            memcpy(dp, sp, w * 2);
            continue;
        }
        const uint8_t* ap = src.a + (sy + row) * src.w + sx;
        for (int col = 0; col < w; col++)
            PutPixel(dp + col, sp[col], ap[col]);
    }
}

void BoxRGBA(Surface& dst, int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if (x1 > x2) { int t = x1; x1 = x2; x2 = t; }
    if (y1 > y2) { int t = y1; y1 = y2; y2 = t; }
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= dst.w) x2 = dst.w - 1;
    if (y2 >= dst.h) y2 = dst.h - 1;
    const uint16_t c = MapRGB(r, g, b);
    for (int y = y1; y <= y2; y++)
        for (int x = x1; x <= x2; x++)
            PutPixel(dst.px + y * dst.w + x, c, a);
}

// ---------------------------------------------------------------------------- the fonts

static const AssetGlyph* Glyph(const AssetFont* Font, char c)
{
    if ((c < 32) || (c > 126))
        c = '?';
    return &Font->glyphs[c - 32];
}

static void DrawLine(Surface& dst, const AssetFont* Font, const char* Line, int Len, int x, int y, uint16_t c)
{
    for (int i = 0; i < Len; i++)
    {
        const AssetGlyph* g = Glyph(Font, Line[i]);
        if (g->alpha >= 0)
        {
            const uint8_t* ap = dm_assets + g->alpha;
            for (int row = 0; row < Font->height; row++)
            {
                const int dy = y + row;
                if ((dy < 0) || (dy >= dst.h))
                    continue;
                for (int col = 0; col < g->w; col++)
                {
                    const int dx = x + g->x + col;
                    if ((dx >= 0) && (dx < dst.w))
                        PutPixel(dst.px + dy * dst.w + dx, c, ap[row * g->w + col]);
                }
            }
        }
        x += g->advance;
    }
}

int TextWidth(const AssetFont* Font, const char* Tekst)
{
    int w = 0;
    for (const char* c = Tekst; *c && (*c != '\n'); c++)
        w += Glyph(Font, *c)->advance;
    return w;
}

void WriteText(Surface& dst, const AssetFont* Font, const char* Tekst, int NrOfChars, int X, int Y, int YSpacing,
               Color ColorIn, bool Underline)
{
    const uint16_t c = MapRGB(ColorIn.r, ColorIn.g, ColorIn.b);
    const char* const TextEnd = Tekst + NrOfChars;
    int Line = 0;
    const char* Start = Tekst;
    while (Start < TextEnd)
    {
        const char* End = Start;
        while ((End < TextEnd) && *End && (*End != '\n'))
            End++;
        const int Len = (int)(End - Start);
        if (Len > 0)
        {
            const int y = Y + Line * Font->lineSkip + Line * YSpacing;
            DrawLine(dst, Font, Start, Len, X, y, c);
            // the underline on the row SDL_ttf drew it on
            if (Underline)
            {
                int w = 0;
                for (int i = 0; i < Len; i++)
                    w += Glyph(Font, Start[i])->advance;
                BoxRGBA(dst, X, y + Font->underline, X + w - 1, y + Font->underline, ColorIn.r, ColorIn.g, ColorIn.b, 255);
            }
        }
        if ((End >= TextEnd) || !*End)
            break;
        Start = End + 1;
        Line++;
    }
}
