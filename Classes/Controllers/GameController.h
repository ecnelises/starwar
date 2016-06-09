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
#include "../Player.h"
#include <vector>
#include <memory>

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
    NetworkController* _net;
    int _timeLeft;
    void _handleBallStatus(float);
    void _overRound();
};

#endif // GAME_CONTROLLER_H_
