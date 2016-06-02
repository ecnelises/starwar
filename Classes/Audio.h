//
//  Sound.h
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#ifndef Sound_h
#define Sound_h

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class Audio {
public:
    Audio();
    virtual ~Audio() {}
    void playKnockEffect();
    void playMenuSceneMusic();
private:
    CocosDenshion::SimpleAudioEngine *_player;
};


#endif /* Sound_h */
