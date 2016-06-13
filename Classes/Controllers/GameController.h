//===--- GameController.h - GameController class definition ---===//
/// \file GameController.h
/// This file declares GameController class. GameController is the main
/// dispatching component in game, as a bridge between showing scene
/// and game inner logic.

#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

#include "cocos2d.h"
#include "MouseController.h"
#include "NetworkController.h"
#include "Player.h"
#include <vector>
#include <memory>

template<typename T>
using observer_ptr = T*;

/// \class GameController
/// \brief Main dispatcher in game processing.
class GameController : public cocos2d::Node {
public:
    GameController(bool hasNetwork) : _isNetworkGame(hasNetwork) {}
    virtual bool init() override;
    void initNetwork(NetworkController*);
    static GameController* create(bool hasNetwork)
    {
        GameController *res = new(std::nothrow) GameController(hasNetwork);
        if (res && res->init()) {
            res->autorelease();
            return res;
        } else {
            delete res;
            res = nullptr;
            return nullptr;
        }
    }
private:
    // READY: 游戏初始化成功, LOADING: 正在加载或等待数据到达, WAITING: 等待player出招, BLOCKING: 等待小球停止滚动, END: 游戏结束
    enum { READY, LOADING, WAITING, BLOCKING, END, FIXED} _status;
    currentPlayer _currentPlayer;
    // TODO
    Player* _enemy;
    LocalPlayer* _localPlayer;
    
    std::unique_ptr<NetworkController> _net;
    bool _isNetworkGame;
    int _timeLeft;
    int _fixTimes;
    void _handleBallStatus(float);
    void _overRound();
    void _localShootEvent(cocos2d::EventCustom*);
    void _remoteShootEvent(cocos2d::EventCustom*);
    void _localOverRoundEvent(cocos2d::EventCustom*);
    void _fixEvent(cocos2d::EventCustom*);
    void _endFixEvent(cocos2d::EventCustom*);
    void _remoteOverRoundEvent(cocos2d::EventCustom*);
    void _remoteResultEvent(cocos2d::EventCustom*);
    void _gameOverEvent(cocos2d::EventCustom*);
    void _aiShootEvent(cocos2d::EventCustom* event);
    void _aiOverRoundEvent(cocos2d::EventCustom*);
    NetworkController* _network;
};

#endif // GAME_CONTROLLER_H_
