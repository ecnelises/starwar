//
//  Player.h
//  ball
//
//  Created by Dcalsky on 16/5/31.
//
//

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Balls.h"
#include "cocos2d.h"

class Player : cocos2d::Node {
public:
    Player();
    virtual ~Player();
    virtual update(float dt) override;
};


#endif // PLAYER_H_
