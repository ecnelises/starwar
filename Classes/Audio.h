//===--- Audio.h - Audio class ---===//
/// \file Audio.h
/// Audio class is delegated to play audio during game processing.

#ifndef AUDIO_H_
#define AUDIO_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

/// \class Audio
/// \brief Audio uses simple audio engine to play audio during different events.
class Audio {
public:
    Audio();
    virtual ~Audio() = default;
    void preloadyMenuSceneMusic();
    void preloadKnockEffect();
    void playKnockEffect();
    void playMenuSceneMusic();
    void playEnterBattleEffect();
    void playItemClickEffect();
    void stopAllMusic();
    void playDefeatEffect();
    void playVictoryEffect();
private:
    CocosDenshion::SimpleAudioEngine* _player;
};

#endif // AUDIO_H_
