//===--- GameController.cpp - GameController class implementation ---===//
/// \file GameController.cpp
/// This file implements interfaces of GameController.

#include "GameController.h"
#include "NetworkController.h"
#include "../Player.h"
#include "../Contact.h"

bool GameController::init(void)
{
    if (!Node::init()) {
        return false;
    }
    
    _status = LOADING;
    auto roundSwitchListener = cocos2d::EventListenerCustom::create("RoundSwitch", [=](cocos2d::EventCustom* event){
        // TODO
    });
    auto localPlayer = LocalPlayer::create();
    auto AIplayer = AIPlayer::create();
    auto contact = Contact::create();
    
    
    _AIplayer = AIplayer;
    _localPlayer = localPlayer;
    //_net = netWorkController;
    _waitingDone = false;
    _timeLeft = timeLeftDefault;
    _eventDispatcher->addEventListenerWithSceneGraphPriority(roundSwitchListener, this);
    
    this->schedule(schedule_selector(GameController::_handleBallStatus), ballStatusInterval);
    this->setCurrentPlayer(LOCAL_PLAYER); // todo 听netWork安排
    this->addChild(localPlayer, 10);
    this->addChild(AIplayer, 10);
    this->addChild(contact, 0);
    //this->addChild(netWorkController, 0);
    
    return true;
}

void GameController::run(void)
{
    _status = PROCESSING;
}

void GameController::_handleBallStatus(float dt)
{
    if(_currentPlayer == LOCAL_PLAYER) {
        auto player = _localPlayer;
    } else {
        auto player = _AIplayer;
    }
    auto childs = _localPlayer->getChildren();
    auto l = childs.begin();
    while(l != childs.end()) {
        auto node = *l;
        int tag = node->getTag();
        
        if(tag == moonTag || tag == earthTag || tag == sunTag) {
            if((node->getPhysicsBody()->getVelocity()).length() > 1e-4) { // isResting
                _waitingDone = true;
                _timeLeft = timeLeftDefault;
                return;
            }
        }
        ++l;
    }
    if(_waitingDone) {
        _waitingDone = false;
        _timeLeft = timeLeftDefault;
        this->setCurrentPlayer(_currentPlayer == LOCAL_PLAYER ? AI_PLAYER : LOCAL_PLAYER);
    } else {
        _timeLeft -= 1;
        printf("%d\n", _timeLeft);
        if(_timeLeft == 0) {
            _waitingDone = false;
            _timeLeft = timeLeftDefault;
            this->setCurrentPlayer(_currentPlayer == LOCAL_PLAYER ? AI_PLAYER : LOCAL_PLAYER);
        }
    }
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