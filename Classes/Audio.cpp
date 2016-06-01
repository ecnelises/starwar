//
//  Sound.cpp
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#include "Audio.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

void Audio::playEffect(const char *fileName)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(fileName);
}

void Audio::playMusic(const char *fileName)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(fileName);
}