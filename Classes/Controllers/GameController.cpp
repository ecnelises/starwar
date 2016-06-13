//===--- GameController.cpp - GameController class implementation ---===//
/// \file GameController.cpp
/// This file implements interfaces of GameController.

#include <chrono>
#include <random>
#include "GameController.h"
#include "NetworkController.h"
#include "../Scenes/MenuScene.h"
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
    _status = LOADING;
    
    // --- ai start
    // TODO: move to other place
    if (!_isNetworkGame) {
        std::default_random_engine dre;
        std::uniform_int_distribution<> uid(0, 1);
        bool first = uid(dre);
        auto localPlayer = new LocalPlayer(first);
        auto aiPlayer = new AIPlayer(!first);
        
        _localPlayer = localPlayer;
        _enemy = aiPlayer;
        _status =  first ? WAITING : LOADING;
        
        _currentPlayer = first ? LOCAL_PLAYER : AI_PLAYER;
        
        this->addChild(localPlayer, 10);
        this->addChild(aiPlayer, 10);
        
        printf("ready\n");
    }
    
    // --- ai end
    
    auto contact = Contact::create();
    auto timer = Timer::create();
    auto localOverRound = cocos2d::EventListenerCustom::create("localOverRound", CC_CALLBACK_1(GameController::_localOverRoundEvent, this));
    auto remoteOverRound = cocos2d::EventListenerCustom::create("remoteOverRound", CC_CALLBACK_1(GameController::_remoteOverRoundEvent, this));
    auto aiOverRound = cocos2d::EventListenerCustom::create("aiOverRound", CC_CALLBACK_1(GameController::_aiOverRoundEvent, this));
    
    auto localShoot = cocos2d::EventListenerCustom::create("localShoot", CC_CALLBACK_1(GameController::_localShootEvent, this));
    auto remoteShoot = cocos2d::EventListenerCustom::create("remoteShoot", CC_CALLBACK_1(GameController::_remoteShootEvent, this));
    auto aiShoot = cocos2d::EventListenerCustom::create("aiShoot", CC_CALLBACK_1(GameController::_aiShootEvent, this));
    
    auto gameOverEvent = cocos2d::EventListenerCustom::create("gameOver", CC_CALLBACK_1(GameController::_gameOverEvent, this));
    
    auto fixEvent = cocos2d::EventListenerCustom::create("fix", CC_CALLBACK_1(GameController::_fixEvent, this));
    auto endFixEvent = cocos2d::EventListenerCustom::create("endFix", CC_CALLBACK_1(GameController::_endFixEvent, this));
    
    _timer = timer;
    
    this->schedule(schedule_selector(GameController::_handleTime), ballStatusInterval);
    this->addChild(contact, 0);
    this->addChild(timer, 9);
    
    _eventDispatcher->addEventListenerWithFixedPriority(fixEvent, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(endFixEvent, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(localOverRound, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteOverRound, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(aiOverRound, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(localShoot, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(remoteShoot, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(aiShoot, 1);
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
    _enemy->listenDepart();
    _localPlayer->setActive(false);
    if (_isNetworkGame) {
        _network->sendShoot(ballId, force);
    }
}

void GameController::_aiShootEvent(cocos2d::EventCustom* event)
{
    _status = BLOCKING;
    _localPlayer->listenDepart();
    printf("AI shoot!\n");
    _enemy->listenDepart();
    _enemy->setActive(false);
}

void GameController::_remoteShootEvent(cocos2d::EventCustom* event)
{
    auto message = static_cast<std::string*>(event->getUserData());
    rapidjson::Document d;
    d.Parse(message->c_str());
    auto ballId = d["ballId"].GetInt();
    auto forceX = d["force"][0].GetDouble();
    auto forceY = d["force"][1].GetDouble();
    
    Ball* target = nullptr;
    for (auto i : _enemy->getBalls()) {
        if (i->getId() == ballId) {
            target = i;
            break;
        }
    }
    _enemy->applyShoot(target, cocos2d::Vec2(forceX, forceY));
    // 开始监听消失事件
    _localPlayer->listenDepart();
    _enemy->listenDepart();
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
    _enemy->fixBall(ballId, cocos2d::Vec2(ballX, ballY));
    printf("fixed\n");
    _fixTimes += 1;
    printf("fixed times :: %d\n", _fixTimes);
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
    int remotePlayerBalls = _enemy->getBallsNumber();
    if(localPlayerBalls == 0 && remotePlayerBalls == 0) {
        _network->sendGameOver(DRAW);
    } else if(localPlayerBalls == 0) {
        _network->sendGameOver(LOSE);
    } else if(remotePlayerBalls == 0) {
        _network->sendGameOver(WIN);
    } else {
        // 未分胜负，下一回合
        auto localBalls = _localPlayer->getBalls();
        auto remoteBalls = _enemy->getBalls();
        for(const auto &ball : localBalls) {
            _network->sendFixed(ball->getId(), ball->getSprite()->getPosition());
        }
        for(const auto &ball : remoteBalls) {
            _network->sendFixed(ball->getId(), ball->getSprite()->getPosition());
        }
        _network->sendEndFixed();
    }
}

void GameController::_aiOverRoundEvent(cocos2d::EventCustom* event)
{
    this->_overRound();
}

void GameController::_remoteOverRoundEvent(cocos2d::EventCustom* event)
{
    
    _fixTimes = 0;
    this->_overRound();
}

void GameController::_gameOverEvent(cocos2d::EventCustom* event)
{
    
    
    auto status = *static_cast<int*>(event->getUserData());
    auto clickListenter = cocos2d::EventListenerMouse::create();
    printf("stats:: %d\n", status);
    clickListenter->onMouseDown = [=](cocos2d::Event* event) {
        auto menuScene = MenuScene::createScene();
        cocos2d::Director::getInstance()->pushScene(menuScene);
    };

    
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(clickListenter, _resultBoard);
//    this->_openResultBoard(status);
    this->unschedule(schedule_selector(GameController::_handleTime)); // 取消timer调度器
    printf("end\n");
    _status = END;
}

// 当时钟到0，就跳入下一回合
void GameController::_handleTime(float dt)
{
    if(_status == WAITING) {
        _timer->timeGo();
    }
    
    // remote player 时钟到0的时候不算回合结束，可能是网络延迟的问题，只有等到接受到overRound的数据才能算回合结束
    if(_timer->isTimeOut() && _currentPlayer == LOCAL_PLAYER) {
        cocos2d::EventCustom *event;
        this->_localOverRoundEvent(event);
    }
}

// 进入下一回合
void GameController::_overRound()
{
    // 时钟重置
    _timer->revertTime();
    // 停止监听消失事件
    _localPlayer->unlistenDepart();
    _enemy->unlistenDepart();
    if (_currentPlayer == LOCAL_PLAYER) {
        _localPlayer->setActive(false);
        _enemy->setActive(true);
        if (_isNetworkGame) {
            _currentPlayer = REMOTE_PLAYER;
        } else {
            _currentPlayer = AI_PLAYER;
        }
        _status = LOADING;
    } else {
        _enemy->setActive(false);
        _localPlayer->setActive(true);
        _currentPlayer = LOCAL_PLAYER;
        _status = WAITING;
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
    _enemy = remotePlayer;
    _status =  first ? WAITING : LOADING;
    
    _currentPlayer = first ? LOCAL_PLAYER : REMOTE_PLAYER;
    
    this->addChild(localPlayer, 8);
    this->addChild(remotePlayer, 8);
//    cocos2d::EventCustom gameOverEvent("gameOver");
//    auto winner = 1;
//    gameOverEvent.setUserData(&winner);
//    _eventDispatcher->dispatchEvent(&gameOverEvent);
    printf("ready\n");
}