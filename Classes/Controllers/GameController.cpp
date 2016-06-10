//===--- GameController.cpp - GameController class implementation ---===//
/// \file GameController.cpp
/// This file implements interfaces of GameController.

#include "GameController.h"
#include "NetworkController.h"
#include "../Player.h"
#include "../Contact.h"
#include "json/rapidjson.h"
#include "json/document.h"

bool GameController::init(void)
{
    if (!Node::init()) {
        return false;
    }
    
    
//    auto roundSwitchListener = cocos2d::EventListenerCustom::create("RoundSwitch", [=](cocos2d::EventCustom* event){
//        // TODO
//    });
#if 0
    /*
    auto localPlayer = LocalPlayer::create();
    auto remotePlayer = RemotePlayer::create();
    auto contact = Contact::create();
    auto overRoundListener = cocos2d::EventListenerCustom::create("overRound", CC_CALLBACK_1(GameController::_handleOverRoundEvent, this));
    auto shootListener = cocos2d::EventListenerCustom::create("shoot", CC_CALLBACK_1(GameController::_handleShootEvent, this));
    auto networkListener = cocos2d::EventListenerCustom::create("init", CC_CALLBACK_1(GameController::_handleGameInitEvent, this));
    auto resultListener = cocos2d::EventListenerCustom::create("result", CC_CALLBACK_1(GameController::_handleResultEvent, this));
    
    _localPlayer = localPlayer;
    _remotePlayer = remotePlayer;
    _timeLeft = timeLeftDefault;
    _status = LOADING;
    
    this->schedule(schedule_selector(GameController::_handleBallStatus), ballStatusInterval);
    this->addChild(localPlayer, 10);
    this->addChild(remotePlayer, 10);
    this->addChild(contact, 0);
    _status = LOADING;
    // _eventDispatcher->addEventListenerWithSceneGraphPriority(roundSwitchListener, this);
     */
#endif
    
    //_eventDispatcher->addEventListenerWithFixedPriority(overRoundListener, 1);
    //_eventDispatcher->addEventListenerWithFixedPriority(shootListener, 1);
    //_eventDispatcher->addEventListenerWithFixedPriority(networkListener, 1);
    //_eventDispatcher->addEventListenerWithFixedPriority(resultListener, 1);
    return true;
}

void GameController::_handleShootEvent(cocos2d::EventCustom* event)
{
    char *buf = static_cast<char*>(event->getUserData());
    printf("shoot\n");
    _status = BLOCKING;
    _localPlayer->setActive(false);
}

void GameController::_handleOverRoundEvent(cocos2d::EventCustom* event)
{
    printf("overRound\n");
    this->_overRound();
}

void GameController::_handleGameInitEvent(cocos2d::EventCustom* event)
{
    // 决定谁先
    printf("ready\n");
    _status = READY;
}

void GameController::_handleResultEvent(cocos2d::EventCustom* event)
{
    // 决定谁先
    printf("end\n");
    _status = END;
}

// 当时钟到0，就跳入下一回合
void GameController::_handleBallStatus(float dt)
{
#if 0
    /*
    if(_currentPlayer == LOCAL_PLAYER) {
    // this->addChild(netWorkController, 0);
//    _eventDispatcher->addEventListenerWithFixedPriority(overRoundListener, 1);
//    _eventDispatcher->addEventListenerWithFixedPriority(moveBallListener, 1);
        //auto player = _AIplayer;
    }
    auto childs = _localPlayer->getChildren();
    auto l = childs.begin();
    while(l != childs.end()) {
        auto node = *l;
        int tag = node->getTag();
    if(_status == WAITING) {
        printf("%d\n", _timeLeft);
        _timeLeft -= 1;
    }
    
    // remote player 时钟到0的时候不算回合结束，可能是网络延迟的问题，只有等到接受到overRound的数据才能算回合结束
    if(_timeLeft == 0 && _currentPlayer == LOCAL_PLAYER) {
        this->_overRound();
    }
}


// 进入下一回合
void GameController::_overRound()
{
    char *buf = "overRound";
    _timeLeft = timeLeftDefault; // 时钟重置
    if(_currentPlayer == LOCAL_PLAYER) {
        _localPlayer->setActive(false);
        _remotePlayer->setActive(true);
        _currentPlayer = REMOTE_PLAYER;
        _status = LOADING; // 状态: 等待数据
        this->_sendData(buf); // todo 发送回合结束的信息
    } else {
        _remotePlayer->setActive(false);
        _localPlayer->setActive(true);
        _currentPlayer = LOCAL_PLAYER;
        _status = WAITING; // 状态: 等待出招
    }
         */
#endif
}

// todo game controller给network发送数据
/*
bool GameController::_sendData(char *buf)
{
    return true; // 成功: true
}
 */

// todo game controller接受network的数据
/*
bool GameController::_receiveData()
{
    return true; // 成功: true
}
 */

void GameController::initNetwork(NetworkController*)
{
    printf("network init!");
    
    // 选定谁先
    _status = WAITING;
    _localPlayer->setActive(true);
    _remotePlayer->setActive(false);
    _currentPlayer = LOCAL_PLAYER;
}

cocos2d::EventDispatcher* GameController::getEventDispatcher(void)
{
    return this->_eventDispatcher;
}