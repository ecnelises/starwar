//===--- Config.cpp - Game data and configures ---===//
/// \file Config.cpp
/// This file defines those data.

#include "Config.h"

using namespace Config;

using str = const char *;

str File::knockAudio = "audio/knock.mp3";
str File::menuSceneMusic = "audio/bg1.mp3";
str File::enterBattleAudio = "audio/enter_battle.mp3";
str File::itemClickAudio = "audio/item_click.mp3";
str File::defeatAudio = "audio/defeat.mp3";
str File::victoryAudio = "audio/victory.mp3";
 
str File::beginSceneFrame = "texture/begin_bg.png";
str File::computerTexture = "texture/computer.png";
str File::aboutTexture = "texture/about.png";
str File::onlineTexture = "texture/online.png";
str File::connectingTexture = "texture/connecting.png";
str File::exitTexture = "texture/exit.png";
str File::battleSceneFrame = "texture/battle_bg.png";
str File::timerFrame = "texture/timer.png";
str File::cursorFrame = "texture/cursor.png";
str File::timerContentFrame = "texture/timer_content.png";
str File::battleMapFrame = "texture/map.png";
str File::menuSceneFrame = "texture/menu_bg.jpg";
 
str File::victoryLogo = "texture/victory.png";
str File::drawLogo = "texture/draw.png";
str File::defeatLogo = "texture/defeat.png";

float Map::leftBorder = (battleSceneFrameSizeX - battleMapFrameSizeX) / 2.0f;
float Map::rightBorder = battleMapFrameSizeX + leftBorder;
float Map::topBorder = (battleSceneFrameSizeY - battleMapFrameSizeY) / 2.0f;
float Map::bottomBorder = battleMapFrameSizeY + topBorder;
int Map::battleMapFrameSizeX = 641;
int Map::battleMapFrameSizeY = 636;
int Map::battleSceneFrameSizeX = 1024;
int Map::battleSceneFrameSizeY = 678;
float Map::width = 556.0f;
float Map::height = 547.0f;
float Map::otherSide = 800.0f;

int Data::timeEachRound = 12;
float Data::menuItemDistance = 80.0f;
float Data::menuTopY = 150.0f;
float Data::menuItemScale = 0.33f;
float Data::ballStatusInterval = 1.0f;
float Data::checkRestingInterval = 0.1f;
unsigned Data::moonNumber = 4;
unsigned Data::earthNumber = 2;
unsigned Data::sunNumber = 1;
float Data::shootEfficiency = 128.0f;

float Ball::moonPositionX = 260.0f;;
float Ball::moonPositionY = 240.0f;
float Ball::earthPositionX = 345.0f;;
float Ball::earthPositionY = 200.0f;;
float Ball::sunPositionX = 520.0f;
float Ball::sunPositionY = 150.0f;
float Ball::moonDistance = 170.0f;
float Ball::earthDistance = 360.0f;
float Ball::sunDistance = 20.0f;

int Tag::mouseController = 20;
int Tag::cursor = 60;
int Tag::departed = 619;
