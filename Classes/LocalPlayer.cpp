//
//  Player.cpp
//  ball
//
//  Created by Dcalsky on 16/5/31.
//
//



#include "Player.h"
#include <memory>

USING_NS_CC;

//bool Player::applyImpulse(const Ball* ball, const Force& force)
//{
//
//}

LocalPlayer::LocalPlayer() //: _balls(std::make_unique<BallsCollection>)
{
    constexpr unsigned ballsNum = 9;
    
    for (int i = 0; i < ballsNum; ++i) {
		_balls->push_back(BallWithStatus());
    }
}




