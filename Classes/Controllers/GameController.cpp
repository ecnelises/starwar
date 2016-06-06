//===--- GameController.cpp - GameController class implementation ---===//
/// \file GameController.cpp
/// This file implements interfaces of GameController.

#include "GameController.h"
#include "Player.h"

bool GameController::init(void)
{
    if (!Node::init()) {
        return false;
    }
    
    _status = READY;
    
    auto roundSwitchListener = cocos2d::EventListenerCustom::create("RoundSwitch", [=](cocos2d::EventCustom* event){
        // TODO
    });
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(roundSwitchListener, this);
    
    schedule(schedule_selector(GameController::_handleBallStatus), 1.0f, kRepeatForever, 0);
    return true;
}

void GameController::run(void)
{
    _status = PROCESSING;
}

void GameController::_handleBallStatus(float dt)
{
    
}