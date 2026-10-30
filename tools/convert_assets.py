#!/usr/bin/env python3
"""Converts the assets of the GP2X version of Puzzle Land into what the Gamebuino AKA build uses.

    python tools/convert_assets.py <ffmpeg.exe>

Reads the original files from assets/ (graphics/, sounds/, music/, levels/ and font.ttf). Writes:
  main/assets/assets.bin   the images, the font and the sound effects, in the LCD's own pixel
                           format, built into the firmware
  main/assets/assets.h     where each of them sits in assets.bin
  SD_files/PUZZLELAND/     the folder that goes on the SD card: the levels, the music and
                           screen.bmp

Images become 16 bit pixels in the order the AKA's panel takes them (lcd_color_rgb in the
library: red in the low bits). The images the game gave a colour key get an alpha plane, 0
where they are magenta (255,0,255); the ones it also gave a surface alpha (SDL_SetAlpha) get
that alpha for the rest of their pixels.

The TrueType font is rendered once here at the size the game opened it at, anti-aliased, each
glyph's coverage becoming its alpha, as in the other ports (the GP2X version itself drew its
text black and white with TTF_RenderText_Solid). The glyphs are placed the way the SDL_ttf of the GP2X (2.0.8) placed them: the base line at the
top of the font's bounding box, not at its ascender, which for this font is 9 pixels higher.
The game's selectors were lined up with the text that way.

Sounds are resampled to 44100 Hz mono 16 bit, the only format the library's WAV player takes;
the music is decoded from the .ogg files by ffmpeg and written with the plain 44 byte WAV
header the library's player checks for.
"""
import math
import os
import shutil
import struct
import subprocess
import sys

from PIL import Image, ImageDraw, ImageFont

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_DIR = os.path.join(ROOT, "assets")
OUT_DIR = os.path.join(ROOT, "main", "assets")
SD_DIR = os.path.join(ROOT, "SD_files", "PUZZLELAND")

# name in the code, file, and the alpha of its other pixels when the game gave it a magenta
# colour key (255, or the surface alpha it set as well), None when it has no colour key
IMAGES = [
    ("ROOMBACKGROUND", "roombackground", None),
    ("STAGECLEARKADER", "stageclearkader", 225),
    ("BRIDGE", "bridge", 255),
    ("HAND", "hand", 255),
    ("PLAYER", "ryf-player", 255),
    ("SHADOW", "ryf-shadow", 128),
    ("SPACESHIP", "spaceship", 255),
    ("CLOUD", "ryf-cloud", 255),
    ("SMALLCLOUD", "ryf-smallcloud", 255),
    ("FAIRY", "ryf-fairy", 255),
    ("BETWEENSTAGE", "betweenstage", None),
    ("BETWEENSTAGELEVEL1", "betweenstagelevel1", None),
    ("OPTIONSSELECT", "optionsselect", 255),
    ("PAPER", "paper", None),
    ("SELECT", "select", 255),
    ("OLDMAN", "oldman", None),
    ("TITLE", "title", None),
    ("TITLESELECTOR", "titleselector", 255),
    ("SDLPOWERED", "sdlpowered", None),
    ("WILLEMSSOFT", "willemssoft", None),
    ("BLOCK1", "block1", None),
    ("BLOCK2", "block2", None),
    ("BLOCK3", "block3", None),
    ("BLOCK4", "block4", None),
    ("BLOCK5", "block5", None),
    ("BLOCK6", "block6", None),
    ("BLOCK7", "block7", None),
    ("BLOCK8", "block8", None),
    ("BLOCK9", "block9", None),
    ("BLOCK10", "block10", None),
    ("BLOCK11", "block11", None),
    ("BLOCK12", "block12", None),
    ("BLOCK13", "block13", None),
    ("BORDER1", "border1", 255),
    ("BORDER2", "border2", 255),
    ("BORDER3", "border3", 255),
    ("BORDER4", "border4", 255),
    ("BORDER5", "border5", 255),
    ("BORDER6", "border6", 255),
    ("BORDER7", "border7", 255),
    ("STAGEBLOCK1", "stageblock1", 255),
    ("STAGEBLOCK2", "stageblock2", 255),
    ("STAGEBLOCK3", "stageblock3", 255),
    ("STAGEBLOCK4", "stageblock4", 255),
    ("STAGEBLOCK5", "stageblock5", 255),
    ("STAGEBLOCK6", "stageblock6", 255),
    ("STAGEBLOCK7", "stageblock7", 255),
    ("STAGEBLOCK8", "stageblock8", 255),
    ("STAGEBLOCK9", "stageblock9", 255),
    ("STAGEBLOCK10", "stageblock10", 255),
]

# name in the code, file, size the game opened it at (TTF_OpenFont)
FONTS = [
    ("FONT", "font.ttf", 25),
]

# same order as the SND_ constants of the game
SOUNDS = ["rotate", "drop", "rotate", "pickup", "error", "menu", "select", "bridge", "stageend", "elf"]

# the music, in the order of the MUS_ constants of the game
MUSIC = ["title", "game1", "game2", "oldman", "stage"]


def lcd_color(r, g, b):
    return (r >> 3) | ((g >> 2) << 5) | ((b >> 3) << 11)


class Blob:
    def __init__(self):
        self.data = bytearray()

    def align(self):
        while len(self.data) % 4:
            self.data.append(0)

    def add(self, raw):
        self.align()
        offset = len(self.data)
        self.data += raw
        return offset


def surface_planes(path, keyed):
    """pixels and alpha plane (None when opaque) of an image"""
    rgba = Image.open(path).convert("RGBA")
    w, h = rgba.size
    pixels = bytearray()
    alpha = bytearray()
    opaque = True
    for r, g, b, a in rgba.getdata():
        if keyed is not None:
            a = 0 if (r, g, b) == (255, 0, 255) else keyed
        if a != 255:
            opaque = False
        pixels += struct.pack("<H", lcd_color(r, g, b) if a else 0)
        alpha.append(a)
    return w, h, bytes(pixels), (None if opaque else bytes(alpha))


def sdl_ttf_metrics(path, size):
    """The ascent, height, line skip and underline row SDL_ttf 2.0.8 gave the font at size."""
    with open(path, "rb") as f:
        d = f.read()
    tables = {}
    for i in range(struct.unpack(">H", d[4:6])[0]):
        e = 12 + i * 16
        tables[d[e:e + 4].decode("latin1")] = struct.unpack(">I", d[e + 8:e + 12])[0]
    head, hhea, post = tables["head"], tables["hhea"], tables["post"]
    units = struct.unpack(">H", d[head + 18:head + 20])[0]
    y_min, y_max = struct.unpack(">hh", d[head + 38:head + 40] + d[head + 42:head + 44])
    asc, desc, gap = struct.unpack(">hhh", d[hhea + 4:hhea + 10])
    ul_pos, ul_thick = struct.unpack(">hh", d[post + 8:post + 12])
    scale = size / units
    ascent = math.ceil(y_max * scale)
    descent = math.ceil(y_min * scale)
    height = ascent - descent + 1
    line_skip = math.ceil((asc - desc + gap) * scale)
    underline = min(ascent - math.floor(ul_pos * scale) - 1, height - 1)
    return ascent, height, line_skip, underline


def font_glyphs(blob, path, size):
    font = ImageFont.truetype(path, size)
    ascent, height, line_skip, underline = sdl_ttf_metrics(path, size)
    entries = []
    for code in range(32, 127):
        ch = chr(code)
        advance = int(round(font.getlength(ch)))
        left, _, right, _ = font.getbbox(ch, anchor="ls")
        x0 = min(0, left)
        gw = max(advance, right) - x0
        if gw <= 0 or ch == " ":
            entries.append("{%d, 0, 0, -1}" % advance)
            continue
        img = Image.new("L", (gw, height), 0)
        ImageDraw.Draw(img).text((-x0, ascent), ch, font=font, fill=255, anchor="ls")
        off = blob.add(img.tobytes())
        entries.append("{%d, %d, %d, %d}" % (advance, gw, x0, off))
    return height, line_skip, underline, entries


def ffmpeg_pcm(ffmpeg, path):
    """the file as 44100 Hz mono signed 16 bit samples"""
    return subprocess.run([ffmpeg, "-hide_banner", "-loglevel", "error", "-i", path,
                           "-ac", "1", "-ar", "44100", "-f", "s16le", "-acodec", "pcm_s16le", "-"],
                          check=True, stdout=subprocess.PIPE).stdout


def write_wav(path, pcm):
    with open(path, "wb") as f:
        f.write(struct.pack("<4sI4s4sIHHIIHH4sI", b"RIFF", 36 + len(pcm), b"WAVE", b"fmt ", 16, 1, 1,
                            44100, 88200, 2, 16, b"data", len(pcm)))
        f.write(pcm)


def write_bmp(path, im):
    """24 bit BMP, what the launcher shows next to the game's name"""
    im = im.convert("RGB")
    w, h = im.size
    row = (w * 3 + 3) & ~3
    with open(path, "wb") as f:
        f.write(struct.pack("<2sIHHI", b"BM", 54 + row * h, 0, 0, 54))
        f.write(struct.pack("<IiiHHIIiiII", 40, w, h, 1, 24, 0, row * h, 2835, 2835, 0, 0))
        for y in range(h - 1, -1, -1):
            line = bytearray()
            for x in range(w):
                r, g, b = im.getpixel((x, y))
                line += bytes((b, g, r))
            line += b"\0" * (row - w * 3)
            f.write(line)


def main():
    if len(sys.argv) != 2:
        print(__doc__)
        sys.exit(1)
    ffmpeg = sys.argv[1]
    gfx = os.path.join(SRC_DIR, "graphics")
    os.makedirs(OUT_DIR, exist_ok=True)
    blob = Blob()
    h = ["// Generated by tools/convert_assets.py from the GP2X version's graphics, font and sounds.",
         "// Offsets are into assets.bin (dm_assets), -1 = not there.", "#pragma once", "#include <stdint.h>", "",
         "struct AssetSurface { int16_t w, h; int32_t pixels, alpha; };",
         "struct AssetGlyph { int8_t advance; int8_t w; int8_t x; int32_t alpha; };",
         "struct AssetFont { int16_t height, lineSkip, underline; const AssetGlyph* glyphs; };",
         "struct AssetSound { int32_t samples; uint32_t count; };", ""]

    # the images, in the firmware
    names = []
    for name, file, keyed in IMAGES:
        w, hh, px, a = surface_planes(os.path.join(gfx, file + ".png"), keyed)
        pxo = blob.add(px)
        ao = blob.add(a) if a else -1
        h.append("static const AssetSurface ASSET_%s = {%d, %d, %d, %d};" % (name, w, hh, pxo, ao))
        names.append('"%s"' % file)
    h.append("")
    h.append("// the file names of the images")
    h.append("static const char* const ASSET_IMAGE_NAMES[] = {%s};" % ", ".join(names))
    h.append("")

    for name, file, size in FONTS:
        height, line_skip, underline, glyphs = font_glyphs(blob, os.path.join(SRC_DIR, file), size)
        h.append("// %s at %d pixels, characters 32 to 126" % (file, size))
        h.append("static const AssetGlyph %s_GLYPHS[] = {\n    %s};" % (name, ",\n    ".join(glyphs)))
        h.append("static const AssetFont ASSET_%s = {%d, %d, %d, %s_GLYPHS};" % (name, height, line_skip, underline, name))
        h.append("")

    sounds = []
    for s in SOUNDS:
        pcm = ffmpeg_pcm(ffmpeg, os.path.join(SRC_DIR, "sounds", s + ".wav"))
        sounds.append("{%d, %d}" % (blob.add(pcm), len(pcm) // 2))
    h.append("static const AssetSound ASSET_SOUNDS[] = {%s};" % ", ".join(sounds))
    h.append("")

    with open(os.path.join(OUT_DIR, "assets.bin"), "wb") as f:
        f.write(blob.data)
    with open(os.path.join(OUT_DIR, "assets.h"), "w", newline="\n") as f:
        f.write("\n".join(h))
    print("assets.bin: %d bytes" % len(blob.data))

    # the levels are the game's own text files, they go on the card as they are
    levels_dst = os.path.join(SD_DIR, "levels")
    if os.path.exists(levels_dst):
        shutil.rmtree(levels_dst)
    shutil.copytree(os.path.join(SRC_DIR, "levels"), levels_dst)

    os.makedirs(os.path.join(SD_DIR, "music"), exist_ok=True)
    for music in MUSIC:
        write_wav(os.path.join(SD_DIR, "music", music + ".wav"),
                  ffmpeg_pcm(ffmpeg, os.path.join(SRC_DIR, "music", music + ".ogg")))
    write_bmp(os.path.join(SD_DIR, "screen.bmp"), Image.open(os.path.join(gfx, "title.png")).resize((160, 120), Image.LANCZOS))
    print("SD folder written to %s" % SD_DIR)


if __name__ == "__main__":
    main()
