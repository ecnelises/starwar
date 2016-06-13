//
//  Config.h
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#ifndef CONFIG_H_
#define CONFIG_H_

//#include "cocos2d.h"

template<typename T>
using observer_ptr = T*;

typedef enum {MOON, EARTH, SUN} ballType;
typedef enum {LOCAL_PLAYER, AI_PLAYER, REMOTE_PLAYER} currentPlayer;

// Audio
constexpr auto knockEffectFile = "audio/knock.mp3";
constexpr auto menuSceneMusicFile = "audio/maple_island.mp3";

// Sprite frame
constexpr auto ballFrameFile = "texture/ball.png";
constexpr auto computerTextureFile = "texture/computer.png";
constexpr auto aboutTextureFile = "texture/about.png";
constexpr auto onlineTextureFile = "texture/online.png";
constexpr auto exitTextureFile = "texture/exit.png";
constexpr auto battleSceneFrameFile = "texture/battle_bg.jpg";
constexpr auto timerFrameFile = "texture/timer.png";
constexpr auto cursorFrameFile = "texture/cursor.png";
constexpr auto timerContentFrameFile = "texture/timer.png";
constexpr auto battleSceneFrameSizeX = 1024;
constexpr auto battleSceneFrameSizeY = 678;
constexpr auto battleMapFrameFile = "texture/battle_map.png";
constexpr auto battleMapFrameSizeX = 641;
constexpr auto battleMapFrameSizeY = 636;
constexpr auto menuSceneFrameFile = "texture/bg.png";
constexpr auto bombFrameFile = "texture/bomb.png";
constexpr auto earthFrameFile = "texture/earth.png";
constexpr auto moonFrameFile = "texture/moon.png";
constexpr auto sunFrameFile = "texture/sun.png";
// Game data
constexpr auto timeLeftDefault = 12;
constexpr auto ballStatusInterval = 1.0f;
constexpr auto menuItemDistance = 55.0f;
constexpr auto menuFirstItemY = 150.0f;
constexpr auto menuItemScale = 0.33f;
constexpr auto ballsNumber = 7;
constexpr auto isRestingInterval = 0.1;
constexpr auto mapWidth = 641.0f;
constexpr auto mapHeight = 636.0f;
constexpr auto WIN = 1;
constexpr auto LOSE = 0;
constexpr auto DRAW = 2;

// Tags
constexpr int moonTag = 30;
constexpr int earthTag = 31;
constexpr int sunTag = 32;
constexpr int AIPlayerTag = 0;
constexpr int localPlayerTag = 1;
constexpr int remotePlayer = 2;
constexpr int mouseControllerTag = 20;
constexpr int cursorTag = 60;

// ball number
constexpr auto moonNumber = 4;
constexpr auto earthNumber = 2;
constexpr auto sunNumber = 1;

// ball radius
// These should be defined as constant values instead of enums.
constexpr float moonRadius = 16.0f;
constexpr float earthRadius = 25.0f;
constexpr float sunRadius = 35.0f;
// ball force
constexpr float moonMaxForce = 80.0f;
constexpr float earthMaxForce = 150.0f;
constexpr float sunMaxForce = 250.0f;
// ball mass
constexpr float moonMass = 20.0f;
constexpr float earthMass = 40.0f;
constexpr float sunMass = 80.0f;

// ball linear damping
constexpr float moonLinearDamping = 4.2f;
constexpr float earthLinearDamping = 5.3f;
constexpr float sunLinearDamping = 7.0f;

// Position
constexpr float moonPositionX = 240.0f;
constexpr float moonPositionY = 240.0f;

constexpr float earthPositionX = 330.0f;
constexpr float earthPositionY = 180.0f;

constexpr float sunPositionX = 520.0f;
constexpr float sunPositionY = 120.0f;

// ball distance
constexpr auto moonDistance = 180.0f;
constexpr auto earthDistance = 360.0f;
constexpr auto sunDistance = 20.0f;

// player data

// mouse controller data
constexpr float drawWidth = 5.0f;


#endif // CONFIG_H_
