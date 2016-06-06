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

// Audio
constexpr auto knockEffectFile = "audio/knock.mp3";
constexpr auto menuSceneMusicFile = "audio/maple_island.mp3";

// Sprite frame
constexpr auto ballFrameFile = "texture/ball.png";
constexpr auto computerTextureFile = "texture/computer.png";
constexpr auto aboutTextureFile = "texture/about.png";
constexpr auto onlineTextureFile = "texture/online.png";
constexpr auto exitTextureFile = "texture/exit.png";
constexpr auto battleSceneFrameFile = "texture/background.png";
constexpr auto menuSceneFrameFile = "texture/bg.png";
constexpr auto bombFrameFile = "texture/bomb.png";

// Position
#define SMALL_SPRITE Point(200, 200)
#define MEDIUM_SPRITE Point(400, 400)

#endif // CONFIG_H_
