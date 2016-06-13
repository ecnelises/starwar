//===--- GameController.cpp - GameController class implementation ---===//
/// \file GameController.cpp
/// This file implements interfaces of GameController.

#include <chrono>
#include <random>
#include "GameController.h"
#include "NetworkController.h"
#include "../Player.h"
#include "../Contact.h"
#include "../Timer.h"
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
    auto timer = Timer::create();
    auto localOverRound = cocos2d::EventListenerCustom::create("localOverRound", CC_CALLBACK_1(GameController::_localOverRoundEvent, this));
    auto remoteOverRound = cocos2d::EventListenerCustom::create("remoteOverRound", CC_CALLBACK_1(GameController::_remoteOverRoundEvent, this));
    
    auto localShoot = cocos2d::EventListenerCustom::create("localShoot", CC_CALLBACK_1(GameController::_localShootEvent, this));
    auto remoteShoot = cocos2d::EventListenerCustom::create("remoteShoot", CC_CALLBACK_1(GameController::_remoteShootEvent, this));
    
    auto gameOverEvent = cocos2d::EventListenerCustom::create("gameOver", CC_CALLBACK_1(GameController::_gameOverEvent, this));
    
    auto fixEvent = cocos2d::EventListenerCustom::create("fix", CC_CALLBACK_1(GameController::_fixEvent, this));
    auto endFixEvent = cocos2d::EventListenerCustom::create("endFix", CC_CALLBACK_1(GameController::_endFixEvent, this));
    
    this->schedule(schedule_selector(GameController::_handleBallStatus), ballStatusInterval);
    this->addChild(contact, 0);
    this->addChild(timer, 9);
    
    _eventDispatcher->addEventListenerWithFixedPriority(fixEvent, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(endFixEvent, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(localOverRound, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteOverRound, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(localShoot, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteShoot, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(gameOverEvent, 1);
    
    return true;
}


void GameController::_localShootEvent(cocos2d::EventCustom* event)
{
    auto data = *static_cast<std::tuple<int, cocos2d::Vec2>*>(event->getUserData());
    auto ballId = std::get<0>(data);
    auto force = std::get<1>(data);
    printf("local shoot\n");
    _status = BLOCKING;
    // 开始监听消失事件
    _localPlayer->listenDepart();
    _remotePlayer->listenDepart();
    _localPlayer->setActive(false);
    _network->sendShoot(ballId, force);
}

void GameController::_remoteShootEvent(cocos2d::EventCustom* event)
{
    auto message = static_cast<std::string*>(event->getUserData());
    rapidjson::Document d;
    d.Parse(message->c_str());
    auto ballId = d["ballId"].GetInt();
    auto forceX = d["force"][0].GetDouble();
    auto forceY = d["force"][1].GetDouble();
    _remotePlayer->applyShoot(ballId, cocos2d::Vec2(forceX, forceY));
    // 开始监听消失事件
    _localPlayer->listenDepart();
    _remotePlayer->listenDepart();
    printf("remote shoot");
    _status = BLOCKING;
}

void GameController::_fixEvent(cocos2d::EventCustom* event)
{
    auto message = static_cast<std::string*>(event->getUserData());
    rapidjson::Document d;
    d.Parse(message->c_str());
    auto ballId = d["ballId"].GetInt();
    auto ballX = d["position"][0].GetDouble();
    auto ballY = d["position"][1].GetDouble();
    _localPlayer->fixBall(ballId, cocos2d::Vec2(ballX, ballY));
    _remotePlayer->fixBall(ballId, cocos2d::Vec2(ballX, ballY));
    printf("fixed\n");
    _fixTimes += 1;
    printf("times :: %d\n", _fixTimes);
    _status = FIXED;
}

void GameController::_endFixEvent(cocos2d::EventCustom *event)
{
    if(_status == FIXED) {
        _network->sendOverRound();
        this->_overRound();
    }
}

void GameController::_localOverRoundEvent(cocos2d::EventCustom* event)
{
    // 在这里判断球数
    int localPlayerBalls = _localPlayer->getBallsNumber();
    int remotePlayerBalls = _remotePlayer->getBallsNumber();
    if(localPlayerBalls == 0 && remotePlayerBalls == 0) {
        _network->sendGameOver(DRAW);
    } else if(localPlayerBalls == 0) {
        _network->sendGameOver(LOSE);
    } else if(remotePlayerBalls == 0) {
        _network->sendGameOver(WIN);
    } else {
        // 未分胜负，下一回合
        auto localBalls = _localPlayer->getBalls();
        auto remoteBalls = _remotePlayer->getBalls();
        for(const auto &ball : localBalls) {
            _network->sendFixed(ball->getId(), ball->getSprite()->getPosition());
        }
        for(const auto &ball : remoteBalls) {
            _network->sendFixed(ball->getId(), ball->getSprite()->getPosition());
        }
        _network->sendEndFixed();
    }
    
}

void GameController::_remoteOverRoundEvent(cocos2d::EventCustom* event)
{
    
    _fixTimes = 0;
    this->_overRound();
}


void GameController::_gameOverEvent(cocos2d::EventCustom* event)
{
    // 决定谁先
    auto status = *static_cast<int*>(event->getUserData());
    switch (status) {
        case WIN: // win is lose
            // LOSE
            break;
        case LOSE: // lose is win
            // WIN
            break;
        case DRAW: // draw is draw
            // DRAW
            break;
        default:
            break;
    }
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
    // 时钟重置
    _timeLeft = timeLeftDefault;
    // 停止监听消失事件
    _localPlayer->unlistenDepart();
    _remotePlayer->unlistenDepart();
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