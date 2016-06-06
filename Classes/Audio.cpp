//
//  Sound.cpp
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#include "Audio.h"
#include "Config.h"

USING_NS_CC;


Audio::Audio()
{
    _player = CocosDenshion::SimpleAudioEngine::getInstance();
}

void Audio::playMenuSceneMusic()
{
    _player->playBackgroundMusic(menuSceneMusicFile);
}

void Audio::playKnockEffect()
{
    _player->playEffect(knockEffectFile);
}

void Audio::preloadyMenuSceneMusic()
{
    _player->preloadBackgroundMusic(menuSceneMusicFile);
}

void Audio::preloadKnockEffect()
{
    _player->preloadEffect(knockEffectFile);
}
