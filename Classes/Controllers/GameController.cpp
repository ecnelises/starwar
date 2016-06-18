//===--- GameController.cpp - GameController class implementation ---===//
/// \file GameController.cpp
/// This file implements interfaces of GameController.

#include <chrono>
#include <random>
#include <ctime>
#include "GameController.h"
#include "NetworkController.h"
#include "../Scenes/MenuScene.h"
#include "../Player.h"
#include "../Contact.h"
#include "../Timer.h"
#include "Config.h"
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
    bool first;
    if (!_isNetworkGame) {
        //std::default_random_engine dre;
        //dre.seed(std::time(nullptr));
        //std::uniform_int_distribution<> uid(0, 10);
        //first = !(uid(dre) % 3);
        //bool first = true;
        first = true;
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
    registerCallbacks({
        {"localOverRound",  &GameController::_localOverRoundEvent},
        {"remoteOverRound", &GameController::_remoteOverRoundEvent},
        {"aiOverRound",     &GameController::_aiOverRoundEvent},
        {"localShoot",      &GameController::_localShootEvent},
        {"remoteShoot",     &GameController::_remoteShootEvent},
        {"aiShoot",         &GameController::_aiShootEvent},
        {"gameOver",        &GameController::_gameOverEvent},
        {"disconnect",      &GameController::_disconnectEvent},
        {"fix",             &GameController::_fixEvent},
        {"endFix",          &GameController::_endFixEvent}
    });
    
    _timer = timer;
    
    this->schedule(schedule_selector(GameController::_handleTime), ballStatusInterval);
    this->addChild(contact, 0);
    this->addChild(timer, 9);
    
    if (!_isNetworkGame && !first) {
        ((AIPlayer*)_enemy)->findAndShoot(_localPlayer->getBalls());
    }
    
    return true;
}

void GameController::_localShootEvent(cocos2d::EventCustom* event)
{
    auto data = static_cast<UnifiedMessageBody*>(event->getUserData());
    //printf("local shoot\n");
    _status = BLOCKING;
    // 开始监听消失事件
    _localPlayer->listenDepart();
    _enemy->listenDepart();
    _localPlayer->setActive(false);
    if (_isNetworkGame) {
        _network->sendShoot(data->targetId, data->vec);
    }
}

void GameController::_aiShootEvent(cocos2d::EventCustom* event)
{
    _status = BLOCKING;
    _localPlayer->listenDepart();
    //printf("AI shoot!\n");
    _enemy->listenDepart();
    _enemy->setActive(false);
}

void GameController::_remoteShootEvent(cocos2d::EventCustom* event)
{
    auto message = static_cast<UnifiedMessageBody*>(event->getUserData());
    _enemy->applyShoot(BallsCollection::BallId(message->targetId), message->vec);
    // 开始监听消失事件
    _localPlayer->listenDepart();
    _enemy->listenDepart();
    //printf("remote shoot");
    _status = BLOCKING;
}

void GameController::_fixEvent(cocos2d::EventCustom* event)
{
    auto message = static_cast<UnifiedMessageBody*>(event->getUserData());
    // TODO 暂且如此
    _localPlayer->fixBall(message->targetId, message->vec);
    _enemy->fixBall(message->targetId, message->vec);
    //printf("fixed\n");
    ++_fixTimes;
    //printf("fixed times :: %d\n", _fixTimes);
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
    if (!_isNetworkGame) {
        this->_overRound();
        ((AIPlayer*)_enemy)->findAndShoot(_localPlayer->getBalls());
        return;
    } // TODO
    // 在这里判断球数
    if(_status == END) {
        return;
    }
    // 本地给本地发, 世界上最遥远的距离莫过于游戏结束函数就在下面，而我却要通过事件分发器调用它
    cocos2d::EventCustom gameOverEvent("gameOver");
    int winner;
    if(_localPlayer->noBall() && _enemy->noBall()) {
        winner = DRAW;
        _network->sendGameOver(DRAW);
        gameOverEvent.setUserData(&winner);
        _eventDispatcher->dispatchEvent(&gameOverEvent);
    } else if(_localPlayer->noBall()) {
        winner = WIN;
        _network->sendGameOver(LOSE);
        gameOverEvent.setUserData(&winner);
        _eventDispatcher->dispatchEvent(&gameOverEvent);
    } else if(_enemy->noBall()) {
        winner = LOSE;
        _network->sendGameOver(WIN);
        gameOverEvent.setUserData(&winner);
        _eventDispatcher->dispatchEvent(&gameOverEvent);
    } else {
        // 未分胜负，下一回合
        auto localBalls = _localPlayer->getBalls();
        auto remoteBalls = _enemy->getBalls();
        for(const auto& ball : *localBalls) {
            _network->sendFixed(ball.id, localBalls->getPosition(ball));
        }
        for(const auto& ball : *remoteBalls) {
            _network->sendFixed(ball.id, localBalls->getPosition(ball));
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

void GameController::_disconnectEvent(cocos2d::EventCustom *event)
{
    cocos2d::EventCustom backToMenuSceneEvent("backToMenuScene");
    _eventDispatcher->dispatchEvent(&backToMenuSceneEvent);
}

void GameController::_gameOverEvent(cocos2d::EventCustom* event)
{
    _status = END;
    auto audio = new Audio;
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto status = *static_cast<int*>(event->getUserData());
    auto clickListenter = cocos2d::EventListenerMouse::create();
    cocos2d::Sprite *resultImg;
    
    clickListenter->onMouseDown = [=](cocos2d::Event* event) {
        cocos2d::EventCustom backToMenuSceneEvent("backToMenuScene");
        _eventDispatcher->dispatchEvent(&backToMenuSceneEvent);
    };
    
    switch (status) {
    case WIN: // win is lose
        // LOSE
        audio->playDefeatEffect();
        resultImg = cocos2d::Sprite::create(defeatFrameFile);
        break;
    case LOSE: // lose is win
        // WIN
        audio->playVictoryEffect();
        resultImg = cocos2d::Sprite::create(victoryFrameFile);
        break;
    case DRAW: // draw is draw
        // DRAW
        audio->playEnterBattleEffect();
        resultImg = cocos2d::Sprite::create(drawFrameFile);
        break;
    default:
        break;
    }
    resultImg->setPosition(cocos2d::Vec2(visibleSize.width/2, visibleSize.height/2));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(clickListenter, resultImg);
    _enemy->setActive(false);
    _localPlayer->setActive(false);
    _network->sendDisconnect();
    
    this->addChild(resultImg, 10);
    printf("end\n");
    
}

// 当时钟到0，就跳入下一回合
void GameController::_handleTime(float dt)
{
    if(_status == END) {
        return;
    }
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
    
    this->addChild(localPlayer, 9);
    this->addChild(remotePlayer, 9);
    printf("ready\n");
}

void GameController::registerCallbacks(std::initializer_list<std::pair<std::string,
                                       void (GameController::*)(cocos2d::EventCustom *)>> table)
{
    for (const auto& binder : table) {
        auto currentListener = cocos2d::EventListenerCustom::create(binder.first,
                                                                    std::bind(binder.second, this,
                                                                              std::placeholders::_1));
        _eventDispatcher->addEventListenerWithFixedPriority(currentListener, 1);
    }
}
