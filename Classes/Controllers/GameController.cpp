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

    auto localPlayer = LocalPlayer::create();
    auto remotePlayer = RemotePlayer::create();
    
    _localPlayer = localPlayer;
    _remotePlayer = remotePlayer;
    _timeLeft = timeLeftDefault;
    _status = LOADING;
    
    auto contact = Contact::create();
    
    auto connect = cocos2d::EventListenerCustom::create("connect", CC_CALLBACK_1(GameController::_connect, this));
    auto waitEvent = cocos2d::EventListenerCustom::create("wait", [=](cocos2d::EventCustom* event) {
        printf("wait event ...\n");
    });
    auto readyEvent = cocos2d::EventListenerCustom::create("ready", [=](cocos2d::EventCustom* event) {
        auto data = static_cast<std::tuple<char*>*>(event->getUserData());
        auto starter = std::get<0>(*data);
        //auto room = std::get<1>(*data);
        //_room = room;

    });
    
    auto localOverRound = cocos2d::EventListenerCustom::create("localOverRound", CC_CALLBACK_1(GameController::_localOverRoundEvent, this));
    auto remoteOverRound = cocos2d::EventListenerCustom::create("remoteOverRound", CC_CALLBACK_1(GameController::_remoteOverRoundEvent, this));
    
    auto localShoot = cocos2d::EventListenerCustom::create("localShoot", CC_CALLBACK_1(GameController::_localShootEvent, this));
    auto remoteShoot = cocos2d::EventListenerCustom::create("remoteShoot", CC_CALLBACK_1(GameController::_remoteShootEvent, this));
    
    auto remoteRegister = cocos2d::EventListenerCustom::create("remoteRegister", CC_CALLBACK_1(GameController::_remoteRegisterEvent, this));
    auto remoteResult = cocos2d::EventListenerCustom::create("remoteResult", CC_CALLBACK_1(GameController::_remoteResultEvent, this));
    
    
    this->schedule(schedule_selector(GameController::_handleBallStatus), ballStatusInterval);
    this->addChild(localPlayer, 10);
    this->addChild(remotePlayer, 10);
    this->addChild(contact, 0);
    
    _eventDispatcher->addEventListenerWithFixedPriority(connect, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(waitEvent, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(readyEvent, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(localOverRound, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteOverRound, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(localShoot, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteShoot, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteRegister, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteResult, 1);
    return true;
}

void GameController::_connect(cocos2d::EventCustom* event)
{
    printf("connect!\n");
    _network->sendRegisteration(_token); // connect --> get token --> server
}

void GameController::_localShootEvent(cocos2d::EventCustom* event)
{
    auto data = *static_cast<std::tuple<Ball*, cocos2d::Vec2>*>(event->getUserData());
    auto force = std::get<1>(data);
    auto ballId = std::get<0>(data)->getId();
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
    _remotePlayer->setActive(false);
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

void GameController::_remoteRegisterEvent(cocos2d::EventCustom* event)
{
    // 决定谁先
    printf("ready\n");
    _status = READY;
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
    _network = network;
    std::string starter = _network->getStarter();
    if(_token == starter) {
        _status = WAITING;
        _localPlayer->setActive(true);
        _remotePlayer->setActive(false);
        _currentPlayer = LOCAL_PLAYER;
    } else {
        _localPlayer->setActive(false);
        _remotePlayer->setActive(true);
        _currentPlayer = REMOTE_PLAYER;
    }
    printf("ready\n");
}

//void GameController::connected()
//{
//    _network->sendRegisteration(_token);
//}