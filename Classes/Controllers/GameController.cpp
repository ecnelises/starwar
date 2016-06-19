//===--- GameController.cpp - GameController class implementation ---===//
/// \file GameController.cpp
/// This file implements interfaces of GameController.

#include "GameController.h"
#include "NetworkController.h"
#include "Scenes/MenuScene.h"
#include "Player.h"
#include "Contact.h"
#include "Timer.h"
#include "Config.h"
#include "json/rapidjson.h"
#include "json/document.h"

bool GameController::init(void)
{
    if (!Node::init()) {
        return false;
    }
    _status = Loading;
 
    auto contact = Contact::create();
    auto timer = Timer::create();
    registerCallbacks({
        {"localOverRound",  &GameController::localOverRoundEvent},
        {"remoteOverRound", &GameController::remoteOverRoundEvent},
        {"aiOverRound",     &GameController::aiOverRoundEvent},
        {"localShoot",      &GameController::localShootEvent},
        {"remoteShoot",     &GameController::remoteShootEvent},
        {"aiShoot",         &GameController::aiShootEvent},
        {"gameOver",        &GameController::gameOverEvent},
        {"disconnect",      &GameController::disconnectEvent},
        {"fix",             &GameController::fixEvent},
        {"endFix",          &GameController::endFixEvent}
    });
    
    _timer = timer;
 
    this->schedule(schedule_selector(GameController::handleTime), Config::Data::ballStatusInterval);
    this->addChild(contact, 0);
    this->addChild(timer, 9);
 
    if (!_isNetworkGame) {
        this->initAIGame();
    }
 
    return true;
}

void GameController::initAIGame(void)
{
    // Local is always first, because AI shoot so quickly that
    // depart check cannot be finished before round over.
    bool localFirst = true;

    auto localPlayer = new LocalPlayer(localFirst);
    auto aiPlayer = new AIPlayer(!localFirst);
    _localPlayer = localPlayer;
    _enemy = aiPlayer;
    _status = localFirst ? Waiting : Loading;
    _currentPlayer = localFirst ? PlayerType::LocalPlayer : PlayerType::AIPlayer;

    this->addChild(localPlayer, 10);
    this->addChild(aiPlayer, 10);

    // AI cannot actively play first, so if AI is first, we have to call it manually.
    if (!localFirst) {
        ((AIPlayer*)_enemy)->findAndShoot(_localPlayer->getBalls());
    }
}

void GameController::localShootEvent(cocos2d::EventCustom* event)
{
    auto data = static_cast<UnifiedMessageBody*>(event->getUserData());
    _status = Blocking;

    // Start listening whether any ball goes out of border.
    // Every player just check balls of himself.
    _localPlayer->listenDepart();
    _enemy->listenDepart();
    _localPlayer->setActive(false);
    if (_isNetworkGame) {
        _network->sendShoot(data->targetId, data->vec);
    }
}

void GameController::aiShootEvent(cocos2d::EventCustom* event)
{
    _status = Blocking;
    _localPlayer->listenDepart();
    _enemy->listenDepart();
    _enemy->setActive(false);
}

void GameController::remoteShootEvent(cocos2d::EventCustom* event)
{
    auto message = static_cast<UnifiedMessageBody*>(event->getUserData());
    _enemy->applyShoot(BallsCollection::BallId(message->targetId), message->vec);
    _localPlayer->listenDepart();
    _enemy->listenDepart();
    _status = Blocking;
}

void GameController::fixEvent(cocos2d::EventCustom* event)
{
    auto message = static_cast<UnifiedMessageBody*>(event->getUserData());
    if (message->targetId & 0x8000) {
        _enemy->fixBall(message->targetId & 0x0FFF, message->vec);
    } else {
        _localPlayer->fixBall(message->targetId, message->vec);
    }
    ++_fixTimes;
    _status = Fixed;
}

void GameController::endFixEvent(cocos2d::EventCustom *event)
{
    if(_status == Fixed) {
        _network->sendOverRound();
        this->overRound();
    }
}

namespace {

GameController::GameStatus statusFromBallNumber(bool local, bool enemy)
{
    if (local && enemy) {
        return GameController::Draw;
    } else if (local) {
        return GameController::Win;
    } else if (enemy) {
        return GameController::Lose;
    } else {
        return GameController::Processing;
    }
}

} // anonymous namespace

void GameController::localOverRoundEvent(cocos2d::EventCustom* event)
{
    // The longest distance in the world
    // is when the game-over function is just below
    // but I have to invoke it by event dispatcher.
    cocos2d::EventCustom gameOverEvent("gameOver");
    GameStatus status = statusFromBallNumber(_localPlayer->noBall(), _enemy->noBall());
    if (status != Processing) {
        _network->sendGameOver(status);
        gameOverEvent.setUserData(&status);
        _eventDispatcher->dispatchEvent(&gameOverEvent);
    } else {
        if (_isNetworkGame) {
            auto localBalls = _localPlayer->getBalls();
            auto remoteBalls = _enemy->getBalls();
            // Player A and Player B may have balls with the same id,
            // so when transmitting information, we should add bit mask to it.
            for(const auto& ball : *localBalls) {
                if (!localBalls->ballWithTag(ball, Config::Tag::departed)) {
                    _network->sendFixed(0x8000 | ball.id, localBalls->getPosition(ball));
                }
            }
            for(const auto& ball : *remoteBalls) {
                if (!localBalls->ballWithTag(ball, Config::Tag::departed)) {
                    _network->sendFixed(0x0000 | ball.id, remoteBalls->getPosition(ball));
                }
            }
            _network->sendEndFixed();
        } else {
            this->overRound();
            ((AIPlayer*)_enemy)->findAndShoot(_localPlayer->getBalls());
        }
    }
}

void GameController::aiOverRoundEvent(cocos2d::EventCustom* event)
{
    this->overRound();
}

void GameController::remoteOverRoundEvent(cocos2d::EventCustom* event)
{
    
    _fixTimes = 0;
    this->overRound();
}

void GameController::disconnectEvent(cocos2d::EventCustom *event)
{
    emitMsg("backToMenuScene");
}

void GameController::gameOverEvent(cocos2d::EventCustom* event)
{
    _status = End;
    auto audio = new Audio;
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto status = *static_cast<int*>(event->getUserData());
    auto clickListenter = cocos2d::EventListenerMouse::create();
    cocos2d::Sprite *resultImg;

    // When game is over, click to return to menu scene.
    clickListenter->onMouseDown = [=](cocos2d::Event* event) {
        emitMsg("backToMenuScene");
        _network->sendDisconnect();
    };

    // Status comes from remote player.
    // So if he wins, that means I lose.
    switch (status) {
    case Win:
        audio->playDefeatEffect();
        resultImg = cocos2d::Sprite::create(Config::File::defeatLogo);
        break;
    case Lose:
        audio->playVictoryEffect();
        resultImg = cocos2d::Sprite::create(Config::File::victoryLogo);
        break;
    case Draw:
        audio->playEnterBattleEffect();
        resultImg = cocos2d::Sprite::create(Config::File::drawLogo);
        break;
    default:
        break;
    }

    resultImg->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(clickListenter, resultImg);
    _enemy->setActive(false);
    _localPlayer->setActive(false);
    this->addChild(resultImg, 10);
}

void GameController::handleTime(float dt)
{
    if(_status == End) {
        return;
    } else if(_status == Waiting) {
        _timer->timeGo();
    }

    // Until we get the overRound message rather than counting down finished, the round is over.
    if(_timer->isTimeOut() && _currentPlayer.is(PlayerType::LocalPlayer)) {
        cocos2d::EventCustom *event;
        this->localOverRoundEvent(event);
    }
}

// Move to next round.
void GameController::overRound()
{
    _timer->revertTime();
    _localPlayer->unlistenDepart();
    _enemy->unlistenDepart();

    if (_currentPlayer.is(PlayerType::LocalPlayer)) {
        _localPlayer->setActive(false);
        _enemy->setActive(true);
        if (_isNetworkGame) {
            _currentPlayer = PlayerType::RemotePlayer;
        } else {
            _currentPlayer = PlayerType::AIPlayer;
        }
        _status = Loading;
    } else {
        _enemy->setActive(false);
        _localPlayer->setActive(true);
        _currentPlayer = PlayerType::LocalPlayer;
        _status = Waiting;
    }
}

void GameController::initNetwork(NetworkController* network)
{
    if (network == nullptr) {
        _network = new NetworkController(false);
        return;
    }
    
    // starter token will be returned from network controller.
    std::string starter = network->getStarter();
    std::string token = network->getToken();
    bool first = token == starter;
    auto localPlayer = new LocalPlayer(first);
    auto remotePlayer = new RemotePlayer(!first);

    _network = network;
    _localPlayer = localPlayer;
    _enemy = remotePlayer;
    _status =  first ? Waiting : Loading;
    _currentPlayer = first ? PlayerType::LocalPlayer : PlayerType::RemotePlayer;
    
    // Players zoom is top of game controller layer to ensure their childs showed.
    this->addChild(localPlayer, 9);
    this->addChild(remotePlayer, 9);
}

// Here we use an intilizer_list to store any number of callbacks.
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
