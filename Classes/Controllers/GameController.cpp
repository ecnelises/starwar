//===--- GameController.cpp - GameController class implementation ---===//
/// \file GameController.cpp
/// This file implements interfaces of GameController.

#include <chrono>
#include <random>
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

    _timeLeft = timeLeftDefault;
    _status = LOADING;
    
    auto contact = Contact::create();
    auto localOverRound = cocos2d::EventListenerCustom::create("localOverRound", CC_CALLBACK_1(GameController::_localOverRoundEvent, this));
    auto remoteOverRound = cocos2d::EventListenerCustom::create("remoteOverRound", CC_CALLBACK_1(GameController::_remoteOverRoundEvent, this));
    
    auto localShoot = cocos2d::EventListenerCustom::create("localShoot", CC_CALLBACK_1(GameController::_localShootEvent, this));
    auto remoteShoot = cocos2d::EventListenerCustom::create("remoteShoot", CC_CALLBACK_1(GameController::_remoteShootEvent, this));
    
    auto remoteResult = cocos2d::EventListenerCustom::create("remoteResult", CC_CALLBACK_1(GameController::_remoteResultEvent, this));
    
    
    this->schedule(schedule_selector(GameController::_handleBallStatus), ballStatusInterval);
    this->addChild(contact, 0);
    
    _eventDispatcher->addEventListenerWithFixedPriority(localOverRound, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteOverRound, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(localShoot, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteShoot, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteResult, 1);
    
    return true;
}


void GameController::_localShootEvent(cocos2d::EventCustom* event)
{
    auto data = *static_cast<std::tuple<int, cocos2d::Vec2>*>(event->getUserData());
    auto ballId = std::get<0>(data);
    auto force = std::get<1>(data);
    printf("local shoot\n");
    _status = BLOCKING;
    _localPlayer->setActive(false);
    _network->sendShoot(ballId, force);
}

void::GameController::_remoteShootEvent(cocos2d::EventCustom* event)
{
    auto message = static_cast<std::string*>(event->getUserData());
    rapidjson::Document d;
    d.Parse(message->c_str());
    auto ballId = d["ballId"].GetInt();
    auto forceX = d["force"][0].GetDouble();
    auto forceY = d["force"][1].GetDouble();
    _remotePlayer->applyShoot(ballId, cocos2d::Vec2(forceX, forceY));
    printf("remote shoot");
    _status = BLOCKING;
}

void GameController::_localOverRoundEvent(cocos2d::EventCustom* event)
{
    // 在这里判断球数
    int localPlayerBalls = _localPlayer->getBallsNumber();
    int remotePlayerBalls = _remotePlayer->getBallsNumber();
    if(localPlayerBalls == 0 && remotePlayerBalls == 0) {
        // 平局
    } else if(localPlayerBalls == 0) {
        // 我方赢
    } else if(remotePlayerBalls == 0) {
        // 对方赢
    } else {
        // 未分胜负，下一回合
        printf("overRound\n");
        _network->sendOverRound();
        this->_overRound();
    }
    
}

void GameController::_remoteOverRoundEvent(cocos2d::EventCustom* event)
{
    this->_overRound();
}


void GameController::_remoteResultEvent(cocos2d::EventCustom* event)
{
    // 决定谁先
    printf("end\n");
    _status = END;
}

// 当时钟到0，就跳入下一回合
void GameController::_handleBallStatus(float dt)
{
    if(_status == WAITING) {
        printf("%d\n", _timeLeft);
        _timeLeft -= 1;
    }
    
    // remote player 时钟到0的时候不算回合结束，可能是网络延迟的问题，只有等到接受到overRound的数据才能算回合结束
    if(_timeLeft == 0 && _currentPlayer == LOCAL_PLAYER) {
        cocos2d::EventCustom *event;
        this->_localOverRoundEvent(event);
    }
}


// 进入下一回合
void GameController::_overRound()
{
    _timeLeft = timeLeftDefault; // 时钟重置
    if(_currentPlayer == LOCAL_PLAYER) {
        _localPlayer->setActive(false);
        _remotePlayer->setActive(true);
        _currentPlayer = REMOTE_PLAYER;
        _status = LOADING; // 状态: 等待数据
        //this->_sendData(buf); // todo 发送回合结束的信息
    } else {
        _remotePlayer->setActive(false);
        _localPlayer->setActive(true);
        _currentPlayer = LOCAL_PLAYER;
        _status = WAITING; // 状态: 等待出招
    }
}

void GameController::initNetwork(NetworkController *network)
{
    std::string starter = network->getStarter();
    std::string token = network->getToken();
    bool first = token == starter;
    auto localPlayer = new LocalPlayer(first);
    auto remotePlayer = new RemotePlayer(!first);
    
    _network = network;
    _localPlayer = localPlayer;
    _remotePlayer = remotePlayer;
    _status =  first ? WAITING : LOADING;
    
    _currentPlayer = first ? LOCAL_PLAYER : REMOTE_PLAYER;
    
    this->addChild(localPlayer, 10);
    this->addChild(remotePlayer, 10);
    
    printf("ready\n");
}

//void GameController::connected()
//{
//    _network->sendRegisteration(_token);
//}