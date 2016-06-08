//===--- GameController.cpp - GameController class implementation ---===//
/// \file GameController.cpp
/// This file implements interfaces of GameController.

#include "GameController.h"
#include "../Player.h"
#include "../Contact.h"

bool GameController::init(void)
{
    if (!Node::init()) {
        return false;
    }
    
    _status = READY;
    auto roundSwitchListener = cocos2d::EventListenerCustom::create("RoundSwitch", [=](cocos2d::EventCustom* event){
        // TODO
    });
    auto localPlayer = LocalPlayer::create();
    auto AIplayer = AIPlayer::create();
    auto contact = Contact::create();
    
    _AIplayer = AIplayer;
    _localPlayer = localPlayer;
    _eventDispatcher->addEventListenerWithSceneGraphPriority(roundSwitchListener, this);
    
    // schedule(schedule_selector(GameController::_handleBallStatus), ballStatusInterval, kRepeatForever, 0);
    this->setCurrentPlayer(LOCAL_PLAYER);
    this->addChild(localPlayer, 10);
    this->addChild(AIplayer, 10);
    this->addChild(contact, 0);
    
    return true;
}

void GameController::run(void)
{
    _status = PROCESSING;
}

void GameController::_handleBallStatus(float dt)
{
    
}

void GameController::setCurrentPlayer(currentPlayer player)
{
    if(player == LOCAL_PLAYER) {
        _localPlayer->setActive(true);
        _AIplayer->setActive(false);
    } else {
        _AIplayer->setActive(true);
        _localPlayer->setActive(false);
    }
    _currentPlayer = player;
}