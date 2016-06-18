#ifndef CONFIG_H_
#define CONFIG_H_

//#include "cocos2d.h"

template<typename T>
using observer_ptr = T*;

template<typename T>
using const_observer_ptr = const T*;

typedef enum {LOCAL_PLAYER, AI_PLAYER, REMOTE_PLAYER} currentPlayer;

namespace Config {

namespace File {
using str = decltype("");

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
}

namespace Map {
extern float leftBorder;
extern float rightBorder;
extern float topBorder;
extern float bottomBorder;
extern int battleMapFrameSizeX;
extern int battleMapFrameSizeY;
extern int battleSceneFrameSizeX;
extern int battleSceneFrameSizeY;
}

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
}

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
}
}

// Audio
constexpr auto knockEffectFile = "audio/knock.mp3";
constexpr auto menuSceneMusicFile = "audio/bg1.mp3";
constexpr auto enterBattleEffectFile = "audio/enter_battle.mp3";
constexpr auto itemClickEffectFile = "audio/item_click.mp3";
constexpr auto defeatEffectFile = "audio/defeat.mp3";
constexpr auto victoryEffectFile = "audio/victory.mp3";

// Sprite frame
constexpr auto beginSceneFrameFile = "texture/begin_bg.png";
constexpr auto computerTextureFile = "texture/computer.png";
constexpr auto aboutTextureFile = "texture/about.png";
constexpr auto onlineTextureFile = "texture/online.png";
constexpr auto connectingTextureFile = "texture/connecting.png";
constexpr auto exitTextureFile = "texture/exit.png";
constexpr auto battleSceneFrameFile = "texture/battle_bg.png";
constexpr auto timerFrameFile = "texture/timer.png";
constexpr auto cursorFrameFile = "texture/cursor.png";
constexpr auto timerContentFrameFile = "texture/timer_content.png";
constexpr auto battleSceneFrameSizeX = 1024;
constexpr auto battleSceneFrameSizeY = 678;
constexpr auto battleMapFrameFile = "texture/map.png";
constexpr auto battleMapFrameSizeX = 641;
constexpr auto battleMapFrameSizeY = 636;
constexpr auto menuSceneFrameFile = "texture/menu_bg.jpg";
constexpr auto bombFrameFile = "texture/bomb.png";
//constexpr auto earthFrameFile = "texture/earth.png";
//constexpr auto moonFrameFile = "texture/moon.png";
//constexpr auto sunFrameFile = "texture/sun.png";
constexpr auto victoryFrameFile = "texture/victory.png";
constexpr auto drawFrameFile = "texture/draw.png";
constexpr auto defeatFrameFile = "texture/defeat.png";

// left is larger?
constexpr auto mapLeftBorder = (battleSceneFrameSizeX - battleMapFrameSizeX) / 2.0f;
constexpr auto mapRightBorder = battleMapFrameSizeX + mapLeftBorder;
constexpr auto mapTopBorder = (battleSceneFrameSizeY - battleMapFrameSizeY) / 2.0f;
constexpr auto mapBottomBorder = battleMapFrameSizeY + mapTopBorder;

// Game data
constexpr auto timeLeftDefault = 12;
constexpr auto ballStatusInterval = 1.0f;
constexpr auto menuItemDistance = 80.0f;
constexpr auto menuFirstItemY = 150.0f;
constexpr auto menuItemScale = 0.33f;
constexpr auto ballsNumber = 7;
constexpr auto isRestingInterval = 0.1;
constexpr auto mapWidth = 556.0f;
constexpr auto mapHeight = 547.0f;

// Tags
/* constexpr int moonTag = 30; */
/* constexpr int earthTag = 31; */
/* constexpr int sunTag = 32; */
/* constexpr int AIPlayerTag = 0; */
/* constexpr int localPlayerTag = 1; */
/* constexpr int remotePlayer = 2; */
constexpr int mouseControllerTag = 20;
constexpr int cursorTag = 60;

// ball number
constexpr auto moonNumber = 4;
constexpr auto earthNumber = 2;
constexpr auto sunNumber = 1;

// ball radius
// These should be defined as constant values instead of enums.
//constexpr float moonRadius = 16.0f;
//constexpr float earthRadius = 25.0f;
//constexpr float sunRadius = 35.0f;
// ball force
constexpr float moonMaxForce = 50.0f;
constexpr float earthMaxForce = 160.0f;
constexpr float sunMaxForce = 350.0f;
// ball mass
//constexpr float moonMass = 20.0f;
//constexpr float earthMass = 40.0f;
//constexpr float sunMass = 80.0f;

// ball linear damping
//constexpr float moonLinearDamping = 4.8f;
//constexpr float earthLinearDamping = 5.3f;
//constexpr float sunLinearDamping = 6.0f;

// Position
constexpr float moonPositionX = 260.0f;
constexpr float moonPositionY = 240.0f;

constexpr float earthPositionX = 345.0f;
constexpr float earthPositionY = 200.0f;

constexpr float sunPositionX = 520.0f;
constexpr float sunPositionY = 150.0f;

// ball distance
constexpr auto moonDistance = 170.0f;
constexpr auto earthDistance = 360.0f;
constexpr auto sunDistance = 20.0f;

// player data

// mouse controller data
constexpr float drawWidth = 5.0f;

constexpr float shootEfficiency = 128.0f;


#endif // CONFIG_H_
