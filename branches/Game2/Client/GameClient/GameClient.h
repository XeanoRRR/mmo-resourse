#pragma once

#include "resource.h"

// -------------------------------------------------------
// Name: GameLoop()
// Describe: ѭ�h
// -------------------------------------------------------
long GameLoop(bool bControl=true);
long GameDisplay(bool bLimitFPS=true);
void GameUpdateScreen();
void GameLimitFps();