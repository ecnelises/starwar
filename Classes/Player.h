//
//  Player.h
//  ball
//
//  Created by Dcalsky on 16/5/31.
//
//

#ifndef Player_h
#define Player_h

#include "Ball.h"
#include "cocos2d"

class Player : cocos2d::Node {
public:
    Player();
    virtual ~Player();
    virtual update(float dt) override;
};


#endif /* Player_h */
