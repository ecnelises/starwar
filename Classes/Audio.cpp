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

void Audio::playEnterBattleEffect()
{
    _player->playEffect(enterBattleEffectFile);
}

void Audio::playItemClickEffect()
{
    _player->playEffect(itemClickEffectFile);
}

void Audio::playDefeatEffect()
{
    _player->playEffect(defeatEffectFile);
}

void Audio::playVictoryEffect()
{
    _player->playEffect(victoryEffectFile);
}

void Audio::preloadyMenuSceneMusic()
{
    _player->preloadBackgroundMusic(menuSceneMusicFile);
}

void Audio::preloadKnockEffect()
{
    _player->preloadEffect(knockEffectFile);
}

void Audio::stopAllMusic()
{
    _player->stopBackgroundMusic();
}
