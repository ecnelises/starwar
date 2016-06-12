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
    for (int i = moonNumber - 1; i >= 0; --i) {
        auto ball = new Ball(MOON, moonNumber - i, Vec2(moonPositionX + moonDistance * i, 750.0f - moonPositionY));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4); // Why 4 ? todo
    }
    
    // earth 2
    for (int i = earthNumber - 1; i >= 0; --i) {
        auto ball = new Ball(EARTH, earthNumber - i + 4, Vec2(earthPositionX + earthDistance * i, 750.0f - earthPositionY));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4);
    }
    
    // sun 1
    for (int i = sunNumber - 1; i >= 0; --i) {
        auto ball = new Ball(SUN, sunNumber - i + 6, Vec2(sunPositionX + sunDistance * i, 750.0f - sunPositionY));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4);
    }
    
    return true;
}

void RemotePlayer::applyShoot(int ballId, const Vec2 &position)
{
    if(!_active) {
        return;
    }
    for(const auto &ball : _balls) {
        if(ball->getId() == ballId) {
            ball->getSprite()->setPosition(position);
            break;
        }
    }
    // this->schedule(CC_CALLBACK_1(RemotePlayer::_isDeparted, this), isRestingInterval, kRepeatForever, 0, "isDeparted"); // 发射完小球后立即检测
}

void RemotePlayer::_isDeparted(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto lterator = _balls.begin();
    while(lterator != _balls.end()) {
        auto l = *lterator;
        if (l->getSprite()->getTag() != mouseControllerTag &&
            (l->getSprite()->getPosition().x - 2.5f >= visibleSize.width / 2 + mapWidth / 2 || l->getSprite()->getPosition().y - 2.5f >= visibleSize.height / 2 + mapHeight / 2 ||
             l->getSprite()->getPosition().x + 2.5f <= visibleSize.width / 2 - mapWidth / 2 || l->getSprite()->getPosition().y + 2.5f <= visibleSize.height / 2 - mapHeight / 2 )) {
                printf("depart");
                l->depart();
                _balls.erase(lterator);
            } else {
        ++lterator;
            }
    }
}


void RemotePlayer::setActive(bool state)
{
    _active = state;
}