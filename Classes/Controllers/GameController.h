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
    bool receiveData();
    bool sendData();
    CREATE_FUNC(GameController)
private:
    // READY: 游戏初始化成功, LOADING: 正在加载或等待数据到达, WAITING: 等待player出招
    enum { READY, LOADING, WAITING } _status;
    currentPlayer _currentPlayer;
    //AIPlayer* _AIplayer;
    RemotePlayer* _remotePlayer;

    LocalPlayer* _localPlayer;
    std::unique_ptr<NetworkController> _net;
    bool _waitingDone;
    int _timeLeft;
    void _handleBallStatus(float);
    // For local game, id is set to 0.
    // Otherwise, it's distributed from the server.
    unsigned _currentGameId;
};

#endif // GAME_CONTROLLER_H_
