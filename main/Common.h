#ifndef COMMON_H
#define COMMON_H

#include "gfx.h"

enum GameStates {GSTitleScreen,GSOldManSpeaking,GSPasswordEntry,GSOptions,GSCredits,GSIntro,GSQuit,GSGame,GSStageClear,GSNextStage,GSStageSelect};
enum PlayerStates {Walking,Waiting,LookingUp,EnterBuilding,EnteredBuilding};
enum CloudStyles {Big,Small};

const Color TextColor = {255,255,255};
//Player anims
const int AnimLeft[6]= {12,11,10,9,8,7};
const int AnimRight[6]  = {1,2,3,4,5,6};
const int AnimEnterBuilding[4]  = {15,16,17,18};

//audio
const int NROFSOUNDS= 10,
          SND_RotateBlock = 0,
          SND_DropBlock = 1,
          SND_FlipBlock = 2,
          SND_PickupBlock = 3,
          SND_Error = 4,
          SND_Menu = 5,
          SND_Select = 6,
          SND_Bridge = 7,
          SND_StageEnd = 8,
          SND_Elf = 9,

  		  NROFMUSIC = 5,
  		  MUS_Title = 0,
  		  MUS_Game1 = 1,
  		  MUS_Game2 = 2,
  		  MUS_Oldman = 3,
  		  MUS_Stage = 4,

  		  BlockCount = 13,
  		  BorderCount = 7,
  		  BlockWidth = 12,
  		  BlockHeight = 12,
  		  XOffsetGame = 40,
  		  YOffsetGame = 34,
  		  XOffsetPassword = 50,
  		  YOffsetPassword = 75,
  		  XOffsetStageSelect = 50,
  		  YOffsetStageSelect = 40,
  		  Cols = 20,
  		  Rows = 16,
  		  MinPlayAreaX = XOffsetGame,
  		  MinPlayAreaY = YOffsetGame,
  		  MaxPlayAreaX = XOffsetGame + BlockWidth * Cols,
  		  MaxPlayAreaY = YOffsetGame + BlockHeight * Rows,
  		  LetterRows = 4,
  		  LetterCols = 7;

extern const char* const LevelPasswords[36];
extern const char* const Letters[4][7];

// the images, in the order of ASSET_IMAGE_NAMES (tools/convert_assets.py)
enum Images {
    IMGRoomBackground, IMGStageClearKader, IMGBridge, IMGHand, IMGPlayer, IMGShadow, IMGSpaceShip,
    IMGCloud, IMGSmallCloud, IMGFairy, IMGBetweenStage, IMGBetweenStageLevel1, IMGOptionsSelect,
    IMGPaper, IMGSelect, IMGOldMan, IMGTitle, IMGTitleSelector, IMGSdlPowered, IMGWillemsSoft,
    IMGBlock1, IMGBorder1 = IMGBlock1 + BlockCount, IMGStageBlock1 = IMGBorder1 + BorderCount,
    NrOfImages = IMGStageBlock1 + 10
};

// the images, built into the firmware
extern Surface IMG[NrOfImages];

#endif
