//
//  Player.cpp
//  ball
//
//  Created by Dcalsky on 16/5/31.
//
//



#include "Player.h"

USING_NS_CC;



bool RemotePlayer::init()
{
    if (!Node::init()) {
        return false;
    }
    // moon 4
    for (int i = 0; i < moonNumber; ++i) {
        auto ball = new Ball(MOON, Vec2(moonPositionX + moonDistance * i, 750.0f - moonPositionY));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4); // Why 4 ? todo
    }
    
    // earth 2
    for (int i = 0; i < earthNumber; ++i) {
        auto ball = new Ball(EARTH, Vec2(earthPositionX + earthDistance * i, 750.0f - earthPositionY));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4);
    }
    
    // sun 1
    for (int i = 0; i < sunNumber; ++i) {
        auto ball = new Ball(SUN, Vec2(sunPositionX + sunDistance * i, 750.0f - sunPositionY));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4);
    }
    
    return true;
}

void RemotePlayer::setActive(bool state)
{
    _active = state;
}