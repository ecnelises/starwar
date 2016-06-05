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
    
    return true;
}

void GameController::run(void)
{
    _status = PROCESSING;
}