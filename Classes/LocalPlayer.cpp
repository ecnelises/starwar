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
using namespace rapidjson;

bool LocalPlayer::init() //: _balls(std::make_unique<BallsCollection>)
{
    if (!Node::init()) {
        return false;
    }
    auto mouseController = MouseController::create();
    unsigned count = 1;
    // moon 4
    /*
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
    */
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
//    rapidjson::Document document;
//    rapidjson::Value detailObject(rapidjson::kObjectType);
//    rapidjson::Value forceObject(rapidjson::kObjectType);
//    document.SetObject();
//    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
//    
//    forceObject.AddMember("x", force.direction.x, allocator);
//    forceObject.AddMember("y", force.direction.y, allocator);
//    detailObject.AddMember("ballId", ball->getId(), allocator);
//    detailObject.AddMember("force", detailObject, allocator);
//    
//    document.AddMember("type", "shoot", allocator);
//    document.AddMember("detail", detailObject, allocator);
//
    ball->move(force);
    EventCustom shootEvent("shoot");
//    shootEvent.setUserData(&document);
    _eventDispatcher->dispatchEvent(&shootEvent);
    this->schedule(CC_CALLBACK_1(LocalPlayer::_isResting, this), isRestingInterval, kRepeatForever, 0, "isResting"); // 发射完小球后立即检测
}

void LocalPlayer::_isResting(float dt)
{
    auto child = this->getChildren();
    auto lterator = child.begin();
    while(lterator != child.end()) {
        if((*lterator)->getTag() != mouseControllerTag && ((*lterator)->getPhysicsBody()->getVelocity().length() > 1e-4)) {
            return;
        }
        ++lterator;
    }
    // 表示全部小球都不动了
    // 发送回合结束命令 todo: 封装
    EventCustom overRoundEvent("overRound");
    _eventDispatcher->dispatchEvent(&overRoundEvent);
    this->unschedule("isResting"); // 取消监听事件减少消耗
}



