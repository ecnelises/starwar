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

class Audio {
public:
    Audio() {}
    virtual ~Audio() {}
    void playEffect(const char *fileName);
    void playMusic(const char *fileName);
};


#endif // AUDIO_H_
