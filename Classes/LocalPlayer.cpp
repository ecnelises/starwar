//
//  Player.cpp
//  ball
//
//  Created by Dcalsky on 16/5/31.
//
//



#include "Player.h"

USING_NS_CC;

//bool Player::applyImpulse(const Ball* ball, const Force& force)
//{
//
//}

std::vector<Ball*> Player::getBalls()
{
    return _balls;
}

LocalPlayer::LocalPlayer()
{
    std::vector<Ball*> curlings;
    for( int i = 0; i < 9; ++i ) {
        auto curling = new Curling(24.0f);
        curlings.push_back(curling);
    }
    _balls = curlings;
}




