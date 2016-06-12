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
    virtual bool init() override;
    void initNetwork(NetworkController*);
    CREATE_FUNC(GameController);
private:
    // READY: 游戏初始化成功, LOADING: 正在加载或等待数据到达, WAITING: 等待player出招, BLOCKING: 等待小球停止滚动, END: 游戏结束
    enum { READY, LOADING, WAITING, BLOCKING, END } _status;
    currentPlayer _currentPlayer;
    RemotePlayer* _remotePlayer;
    LocalPlayer* _localPlayer;
    
    std::unique_ptr<NetworkController> _net;
    int _timeLeft;
    std::string _token;
    void _handleBallStatus(float);
    void _overRound();
    void _localShootEvent(cocos2d::EventCustom*);
    void _remoteShootEvent(cocos2d::EventCustom*);
    void _localOverRoundEvent(cocos2d::EventCustom*);
    void _remoteOverRoundEvent(cocos2d::EventCustom*);
    void _remoteRegisterEvent(cocos2d::EventCustom*);
    void _remoteResultEvent(cocos2d::EventCustom*);
    void _connect(cocos2d::EventCustom*);
    NetworkController* _network;
};

#endif // GAME_CONTROLLER_H_
