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
    void run();
    virtual bool init() override;
    void setCurrentPlayer(currentPlayer);
    CREATE_FUNC(GameController)
private:
    // TODO: why shared_ptr?
    enum { OVER, PROCESSING, READY, LOADING } _status;
    std::vector<std::shared_ptr<Player>> _players;
    currentPlayer _currentPlayer;
    observer_ptr<AIPlayer> _AIplayer;
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
