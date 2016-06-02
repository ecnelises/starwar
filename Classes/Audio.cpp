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
    _player->playBackgroundMusic(MENUSCENE_MUSIC);
}

void Audio::playKnockEffect()
{
    _player->playEffect(KNOCK_EFFECT);
}