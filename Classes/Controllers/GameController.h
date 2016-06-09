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
    void networkError();
    void networkClose();
    cocos2d::EventDispatcher* getEventDispatcher();
    bool _receiveData();
    bool _sendData(char*);
    CREATE_FUNC(GameController)
    void initializeGame(unsigned gameid, const std::string& starter);
    void endGame(const std::string& winner);
private:
    // READY: 游戏初始化成功, LOADING: 正在加载或等待数据到达, WAITING: 等待player出招
    enum { READY, LOADING, WAITING, BLOCKING, END } _status;
    currentPlayer _currentPlayer;
    //AIPlayer* _AIplayer;
    RemotePlayer* _remotePlayer;

    LocalPlayer* _localPlayer;
    std::unique_ptr<NetworkController> _net;
    bool _waitingDone;
    int _timeLeft;
    void _handleBallStatus(float);
    void _overRound();
    void _handleShootEvent(cocos2d::EventCustom*);
    void _handleOverRoundEvent(cocos2d::EventCustom*);
    void _handleGameInitEvent(cocos2d::EventCustom*);
    void _handleResultEvent(cocos2d::EventCustom*);
};

#endif // GAME_CONTROLLER_H_
