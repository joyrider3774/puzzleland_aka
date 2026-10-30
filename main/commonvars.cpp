#include "commonvars.h"

const char* const LevelPasswords[36] = {"DAVY","DISK","TREE","SEAL",
									 "OPEN","LION","WINE","GROW",
									 "FOOD","DUEL","MEAT","FLEE",
									 "DUST","LOST","HELP","MILK",
									 "SAFE","DULL","SAVE","BEER",
									 "BILL","BLOW","BIKE","VENT",
									 "ROOM","HOOD","MOOD","FEAR",
									 "REEF","POLE","BEEF","SONG",
									 "HOPE","BLUE","MAIL","MONK"};
const char* const  Letters[4][7] = {{"A","B","C","D","E","F","G"},
											  {"H","I","J","K","L","M","N"},
											  {"O","P","Q","R","S","T","U"},
											  {"V","W","X","Y","Z","0","0"}};

Surface IMG[NrOfImages];

    //Game Stuff
GameStates GameState = GSIntro;
int Level;
CHand Hand;
uint32_t StartTime,EndTime;

    //Audio
bool SoundEnabled=true,MusicEnabled=true;
int Volume=128;

    //Puzzle Game Stuff
int PlayField[2][Cols][Rows];
bool BlockActive;
