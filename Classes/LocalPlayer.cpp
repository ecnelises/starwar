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

LocalPlayer::LocalPlayer(bool isStarter) //: _balls(std::make_unique<BallsCollection>)
{
    auto mouseController = MouseController::create();
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
    
    mouseController->addBalls(_balls);
    _mouse = mouseController;
    _mouse->setPlayer(this);
    this->setActive(isStarter);
    this->addChild(mouseController, 10); // Why 10 ? todo
}

void LocalPlayer::setActive(bool state)
{
    // 如果轮到我操作，就显示光标
    if(!state) {
        auto childs = this->getChildren();
        for(const auto &child : childs) {
            if(child->getTag() == cursorTag) {
                child->removeFromParentAndCleanup(true);
            }
        }
    } else {
        for(const auto &ball : _balls) {
            auto cursor = Sprite::create(cursorFrameFile);
            cursor->setPosition(Vec2(ball->getSprite()->getPositionX(), ball->getSprite()->getPositionY() + 20));
            cursor->setTag(cursorTag);
            this->addChild(cursor, 5);
        }
    }
    _active = state;
    _mouse->setActive(state);
}

void LocalPlayer::applyShoot(Ball *ball, const Force &force)
{
    if(!_active) {
        return;
    }
    
    ball->move(force * ball->getMaxForce());
    auto data = std::make_tuple(ball->getId(), force * ball->getMaxForce());
    auto dataPoint = &data;
    EventCustom shootEvent("localShoot");
    shootEvent.setUserData(dataPoint);
    _eventDispatcher->dispatchEvent(&shootEvent);
    
    this->schedule(CC_CALLBACK_1(LocalPlayer::_isResting, this), isRestingInterval, kRepeatForever, 0, "isResting"); // 发射完小球后立即检测
    
}

void LocalPlayer::_isResting(float dt)
{
    for (const auto& l : _balls) {
        if (l->getSprite()->getTag() != mouseControllerTag && l->getBallBody()->getVelocity().length() > 1e-1) {
            //            auto data = std::make_tuple(l->getId(), l->getSprite()->getPosition());
            //            auto dataPoint = &data;
            //            EventCustom shootEvent("localShoot");
            //            shootEvent.setUserData(dataPoint);
            //            _eventDispatcher->dispatchEvent(&shootEvent);
            return;
        }
    }
    // 表示全部小球都不动了
    // 发送回合结束命令 todo: 封装
    EventCustom overRoundEvent("localOverRound");
    _eventDispatcher->dispatchEvent(&overRoundEvent);
    this->unschedule("isResting"); // 取消监听事件减少消耗
    
}

void LocalPlayer::listenDepart()
{
    this->schedule(CC_CALLBACK_1(LocalPlayer::_isDeparted, this), isRestingInterval, kRepeatForever, 0, "isDeparted"); // 发射完小球后立即检测
}

void LocalPlayer::unlistenDepart()
{
    this->unschedule("isDeparted"); // 取消监听事件减少消耗
}

void Player::_isDeparted(float dt)
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto lterator = _balls.begin();
    while(lterator != _balls.end()) {
        auto l = *lterator;
        if (l->getSprite()->getTag() != mouseControllerTag &&
            (l->getSprite()->getPosition().x - 2.5f >= visibleSize.width / 2 + mapWidth / 2 || l->getSprite()->getPosition().y - 2.5f >= visibleSize.height / 2 + mapHeight / 2 ||
             l->getSprite()->getPosition().x + 2.5f <= visibleSize.width / 2 - mapWidth / 2 || l->getSprite()->getPosition().y + 2.5f <= visibleSize.height / 2 - mapHeight / 2 )) {
                printf("remote depart");
                l->depart();
                _balls.erase(lterator);
            } else {
                ++lterator;
            }
    }
}

// offset: 2.5f 偏移量球中心出地图才算出界




