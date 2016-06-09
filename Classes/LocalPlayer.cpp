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

bool LocalPlayer::init() //: _balls(std::make_unique<BallsCollection>)
{
    if (!Node::init()) {
        return false;
    }
    auto mouseController = MouseController::create();
    unsigned count = 1;
    // moon 4
    for (int i = 0; i < moonNumber; ++i) {
        auto ball = new Ball(MOON, Vec2(moonPositionX + moonDistance * i, moonPositionY));
        _balls.push_back({ball, count++});
        this->addChild(ball->getSprite(), 4);
    }
    
    // earth 2
    for (int i = 0; i < earthNumber; ++i) {
        auto ball = new Ball(EARTH, Vec2(earthPositionX + earthDistance * i, earthPositionY));
        _balls.push_back({ball, count++});
        this->addChild(ball->getSprite(), 4);
    }
    
    // sun 1
    for (int i = 0; i < sunNumber; ++i) {
        auto ball = new Ball(SUN, Vec2(sunPositionX + sunDistance * i, sunPositionY));
        _balls.push_back({ball, count++});
        this->addChild(ball->getSprite(), 4);
    }
    
    mouseController->addBalls(_balls);
    _mouse = mouseController;
    _mouse->setPlayer(this);
    this->addChild(mouseController, 10); // Why 10 ? todo
    return true;
}

void LocalPlayer::setActive(bool state)
{
    _active = state;
    _mouse->setActive(state);
}

void LocalPlayer::applyMove(Ball *ball, const Force &force)
{
    if(!_active) {
        return;
    }
    ball->move(force);
}



