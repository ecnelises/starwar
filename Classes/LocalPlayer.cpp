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

std::unique_ptr<BallsCollection> Player::getBalls()
{
    return std::move(_balls);
}

LocalPlayer::LocalPlayer()
{
    std::unique_ptr<BallsCollection> curlings;
    for( int i = 0; i < 9; ++i ) {
        auto curling = new Curling(24.0f);
        curlings.push_back(curling);
    }
    _mouse = MouseController::create();
    _balls = curlings;
}




