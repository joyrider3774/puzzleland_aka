# Puzzle Land for the Gamebuino AKA

A port of the GP2X version of Puzzle Land to the Gamebuino AKA. Puzzle Land is a puzzle game, a
remake of Daedalian Opus for the Game Boy: fill each room's shape with the blocks you have,
turning and flipping them until they fit. There are 36 rooms. Between two rooms you walk over to
the next one, a fairy brings a new block every few rooms, and a space ship shows the next room's
password. The graphics, sounds and music are the GP2X version's, at the same 320x240 the AKA's
screen has.

## Controls

| AKA | Action |
|---|---|
| D-pad / joystick | move in the menus, move the hand or the block you hold (two directions held move diagonally); up walks into a building |
| A | select; pick up a block, put it down; walk into a building |
| B | back; rotate the block you hold |
| C / D | flip the block you hold |
| R1 | start the room over; select in the menus |
| MENU | back to the title screen (from the room select: back to the password entry) |

The volume is in **Options**, with music and sound on or off.

**RUN + MENU** held for half a second anywhere in the game goes back to the launcher (the title
screen has no Quit entry); RUN on its own switches the AKA off as in every game.

## Installing

Copy the `SD_files/PUZZLELAND` folder to the root of the SD card and start Puzzle Land from the
launcher. The folder holds `firmware.bin`, `meta.json`, `screen.bmp`, the music, the rooms in
`levels/` and the `LICENSE`. The game writes its options to `settings.dat` there.

## Building

ESP-IDF 5.5 and the official [Gamebuino AKA library](https://github.com/jmp42/Gamebuino_AKA_lib),
which is used where it is checked out (`C:/github/Gamebuino_AKA_lib` by default, or set
`AKA_LIB_DIR`):

```
idf.py set-target esp32s3
idf.py build
```

`build/puzzleland.bin` is copied to `SD_files/PUZZLELAND/firmware.bin` by the build.

The graphics, font, sounds, music and rooms are the GP2X version's original files in `assets/`,
converted by `tools/convert_assets.py` (Pillow, and ffmpeg for the music):

```
python tools/convert_assets.py <ffmpeg>
```

It writes `main/assets/assets.bin` (the images, the font and the sounds, built into the firmware)
and the SD card folder's music, rooms and `screen.bmp`.

## Source layout

The GP2X version was a single `puzzleland.cpp`, split up here:

| File | What it is |
|---|---|
| `main/main.cpp` | start up and the state loop |
| `main/intro.cpp`, `titlescreen.cpp`, `oldmanspeaking.cpp`, `passwordentry.cpp`, `options.cpp`, `credits.cpp`, `game.cpp`, `stageclear.cpp`, `nextstage.cpp`, `stageselect.cpp` | one file per game state |
| `main/playfield.cpp` | the room, the blocks, moving, turning and flipping them |
| `main/CHand.cpp`, `CSelectors.cpp`, `CBetweenStage.cpp` | the hand, the menu selectors, and the player, clouds, space ship, fairy and block between rooms |
| `main/gamefuncs.cpp`, `commonvars.cpp` | what the states share: images, volume, settings, music, the globals |
| `main/gfx.cpp`, `sound.cpp`, `input.cpp`, `platform.cpp` | what SDL did on the GP2X, on the AKA library |

## Credits

Puzzle Land by Willems Davy (Willems Soft). A remake of Daedalian Opus by Vic Tokai Inc. (1990).

Yann R. Fernandez aka Ryf made the main character, fairy, shadow and the clouds graphics.
Thanks for that!

The original music comes from [vgmusic.com](https://www.vgmusic.com):

| Game | Song | By |
|---|---|---|
| Ragnarok Online | Streamside (2) | Chris Ng |
| Ragnarok Online | Ancient Groover | octahedron80 |
| Ragnarok Online | Theme of Prontera | octahedron80 |
| The Legend of Kyrandia, Book 3 | Main Menu | unknown |
| Dark Lantern | Dance of Sorrow | Aaron Roney - David DeTomaso |

## License

MIT, see [LICENSE](LICENSE).
