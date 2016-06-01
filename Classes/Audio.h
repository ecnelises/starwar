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

class Audio {
public:
    Audio() {}
    virtual ~Audio() {}
    void playEffect(const char *fileName);
    void playMusic(const char *fileName);
};


#endif /* Sound_h */
