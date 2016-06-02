//
//  Sound.h
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#ifndef AUDIO_H_
#define AUDIO_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class Audio {
public:
    Audio();
    virtual ~Audio() {}
    void playKnockEffect();
    void playMenuSceneMusic();
    void preloadyMenuSceneMusic();
    void preloadKnockEffect();
private:
    CocosDenshion::SimpleAudioEngine *_player;
};


#endif // AUDIO_H_
