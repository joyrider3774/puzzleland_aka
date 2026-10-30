// One function per game state, each runs while GameState is its own
#ifndef STATES_H
#define STATES_H

void Intro();
void TitleScreen();
void OldManSpeaking();
void PasswordEntry();
void Options();
void Credits();
void Game();
void StageClear();
// the screen between two rooms (NextStageLevel0 before the first room, NextStageLevel1to35 after the others)
void NextStage();
void StageSelect();

#endif
