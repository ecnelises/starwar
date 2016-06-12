//
//  Player.cpp
//  ball
//
//  Created by Dcalsky on 16/5/31.
//
//

#include "Player.h"

USING_NS_CC;


RemotePlayer::RemotePlayer(bool isStarter)
{
    float diff = isStarter ? 0 : 768.0f;
    int initNumber = isStarter ? 0 : 7;
    for (int i = 0; i < moonNumber; ++i) {
        auto ball = new Ball(MOON, initNumber + i + 1, Vec2(moonPositionX + moonDistance * i, fabsf(diff - moonPositionY)));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4); // Why 4 ? todo
    }
    
    // earth 2
    for (int i = 0; i < earthNumber; ++i) {
        auto ball = new Ball(EARTH, initNumber + i + 5, Vec2(earthPositionX + earthDistance * i, fabsf(diff - earthPositionY)));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4);
    }
    
    // sun 1
    for (int i = 0; i < sunNumber; ++i) {
        auto ball = new Ball(SUN, initNumber + i + 7, Vec2(sunPositionX + sunDistance * i, fabsf(diff - sunPositionY)));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4);
    }
    this->setActive(isStarter);
    
}

void RemotePlayer::applyShoot(int ballId, const Vec2 &position)
{
    if(!_active) {
        return;
    }
    for(const auto &ball : _balls) {
        if(ball->getId() == ballId) {
            //            auto moveTo =  cocos2d::MoveTo::create(isRestingInterval * 3, position);
            //            ball->getSprite()->runAction(moveTo);
            printf("position: %.4f  %.4f\n", position.x, position.y);
            ball->move(position);
            break;
        }
    }
    this->schedule(CC_CALLBACK_1(RemotePlayer::_isDeparted, this), isRestingInterval, kRepeatForever, 0, "isDeparted"); // 发射完小球后立即检测
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