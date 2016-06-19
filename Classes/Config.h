//===--- Config.h - Game data and configures ---===//
/// \file Config.h
/// This file declares Config and several sub-namespaces,
/// in which global data are stored in.

#ifndef CONFIG_H_
#define CONFIG_H_

template<typename T>
using observer_ptr = T*;

template<typename T>
using const_observer_ptr = const T*;

using Force = cocos2d::Vec2;

/// \namespace Config
/// \brief All data and configures are in this namespace.
namespace Config {

/// \namespace File
/// \brief File paths, 'Resources/res' as path root.
namespace File {
using str = const char *;

extern str knockAudio;
extern str menuSceneMusic;
extern str enterBattleAudio;
extern str itemClickAudio;
extern str defeatAudio;
extern str victoryAudio;

extern str beginSceneFrame;
extern str computerTexture;
extern str aboutTexture;
extern str onlineTexture;
extern str connectingTexture;
extern str exitTexture;
extern str battleSceneFrame;
extern str timerFrame;
extern str cursorFrame;
extern str timerContentFrame;
extern str battleMapFrame;
extern str menuSceneFrame;
extern str bombFrame;

extern str victoryLogo;
extern str drawLogo;
extern str defeatLogo;
} // namespace File

/// \namespace Map
/// \brief Define constants about map.
namespace Map {
extern float leftBorder;
extern float rightBorder;
extern float topBorder;
extern float bottomBorder;
extern int battleMapFrameSizeX;
extern int battleMapFrameSizeY;
extern int battleSceneFrameSizeX;
extern int battleSceneFrameSizeY;
extern float width;
extern float height;
extern float otherSide;
} // namespace Map

/// \namespace Data
/// \brief Data about menu, ball number and time interval.
namespace Data {
extern int timeEachRound;
extern float menuItemDistance;
extern float menuTopY;
extern float menuItemScale;
extern float ballStatusInterval;
extern float checkRestingInterval;
extern unsigned moonNumber;
extern unsigned earthNumber;
extern unsigned sunNumber;
extern float shootEfficiency;
} // namespace Data

/// \namespace Ball
/// \brief positions.
namespace Ball {
extern float moonPositionX;
extern float moonPositionY;

extern float earthPositionX;
extern float earthPositionY;

extern float sunPositionX;
extern float sunPositionY;

extern float moonDistance;
extern float earthDistance;
extern float sunDistance;
} // namespace Ball

/// \namespace Tag
/// \brief MouseController and ball cursor tags.
namespace Tag {
extern int mouseController;
extern int cursor;
extern int departed;
} // namespace Tag

} // namespace Config

#endif // CONFIG_H_
