//
//  Player.cpp
//  ball
//
//  Created by Dcalsky on 16/5/31.
//
//




#include <memory>
#include "json/rapidjson.h"
#include "json/document.h"
#include "Player.h"

USING_NS_CC;

bool LocalPlayer::init() //: _balls(std::make_unique<BallsCollection>)
{
    if (!Node::init()) {
        return false;
    }
    auto mouseController = MouseController::create();
    for (int i = 0; i < moonNumber; ++i) {
        auto ball = new Ball(MOON, i + 1, Vec2(moonPositionX + moonDistance * i, moonPositionY));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4);
    }
    
    // earth 2
    for (int i = 0; i < earthNumber; ++i) {
        auto ball = new Ball(EARTH, i + 5, Vec2(earthPositionX + earthDistance * i, earthPositionY));
        _balls.push_back(ball);
        this->addChild(ball->getSprite(), 4);
    }
    
    // sun 1
    for (int i = 0; i < sunNumber; ++i) {
        auto ball = new Ball(SUN, i + 7, Vec2(sunPositionX + sunDistance * i, sunPositionY));
        _balls.push_back(ball);
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

void LocalPlayer::applyShoot(Ball *ball, const Force &force)
{
    if(!_active) {
        return;
    }
    
    ball->move(force * ball->getMaxForce());
    EventCustom shootEvent("localShoot");
    auto data = std::make_tuple(ball, force * ball->getMaxForce());
    auto dataPoint = &data;
    shootEvent.setUserData(dataPoint);
    _eventDispatcher->dispatchEvent(&shootEvent);
    this->schedule(CC_CALLBACK_1(LocalPlayer::_isResting, this), isRestingInterval, kRepeatForever, 0, "isResting"); // 发射完小球后立即检测
    this->schedule(CC_CALLBACK_1(LocalPlayer::_isDeparted, this), isRestingInterval, kRepeatForever, 0, "isDeparted"); // 发射完小球后立即检测
}

void LocalPlayer::_isResting(float dt)
{
    for (const auto& l : _balls) {
        if (l->getSprite()->getTag() != mouseControllerTag && l->getBallBody()->getVelocity().length() > 1e-1) {
            return;
        }
    }
    // 表示全部小球都不动了
    // 发送回合结束命令 todo: 封装
    EventCustom overRoundEvent("overRound");
    _eventDispatcher->dispatchEvent(&overRoundEvent);
    this->unschedule("isResting"); // 取消监听事件减少消耗
    this->unschedule("isDeparted"); // 取消监听事件减少消耗
}

void LocalPlayer::_isDeparted(float dt)
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
            }
        ++lterator;
    }
}


// offset: 2.5f 偏移量球中心出地图才算出界




