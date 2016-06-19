//===--- Audio.cpp - Audio class ---===//
/// \file Audio.cpp
/// Audio class is delegated to play audio during game processing.

#include "Audio.h"
#include "Config.h"

Audio::Audio()
{
    _player = CocosDenshion::SimpleAudioEngine::getInstance();
}

void Audio::playMenuSceneMusic()
{
    _player->playBackgroundMusic(Config::File::menuSceneMusic);
}

void Audio::playKnockEffect()
{
    _player->playEffect(Config::File::knockAudio);
}

void Audio::playEnterBattleEffect()
{
    _player->playEffect(Config::File::enterBattleAudio);
}

void Audio::playItemClickEffect()
{
    _player->playEffect(Config::File::itemClickAudio);
}

void Audio::playDefeatEffect()
{
    _player->playEffect(Config::File::defeatAudio);
}

void Audio::playVictoryEffect()
{
    _player->playEffect(Config::File::victoryAudio);
}

void Audio::preloadyMenuSceneMusic()
{
    _player->preloadBackgroundMusic(Config::File::menuSceneMusic);
}

void Audio::preloadKnockEffect()
{
    _player->preloadEffect(Config::File::knockAudio);
}

void Audio::stopAllMusic()
{
    _player->stopBackgroundMusic();
}
