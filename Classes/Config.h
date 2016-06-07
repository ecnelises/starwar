//
//  Config.h
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#ifndef CONFIG_H_
#define CONFIG_H_

#include "cocos2d.h"
USING_NS_CC;

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
constexpr auto battleMapFrameFile = "texture/battle_map.png";
constexpr auto menuSceneFrameFile = "texture/bg.png";
constexpr auto bombFrameFile = "texture/bomb.png";
constexpr auto earthFrameFile = "texture/earth.png";
constexpr auto moonFrameFile = "texture/moon.png";
constexpr auto sunFrameFile = "texture/sun.png";
// Game data
constexpr auto ballStatusInterval = 0.1f;
constexpr auto menuItemDistance = 55.0f;
constexpr auto menuFirstItemY = 150.0f;
constexpr auto menuItemScale = 0.33f;
// Position
#define moonPoint Point(200.0f, 200.0f)
#define earthPoint Point(400.0f, 400.0f)
#define sunPoint Point(400.0f, 400.0f)

constexpr auto moonDistance = 300.0f;

#endif // CONFIG_H_
