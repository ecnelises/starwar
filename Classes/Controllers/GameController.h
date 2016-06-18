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
#include "../Timer.h"
#include <initializer_list>
#include <vector>
#include <memory>

/// \class GameController
/// \brief Main dispatcher in game processing.
class GameController : public cocos2d::Node {
public:
    /// Constructor that just set the network enabled or not.
    /// \param hasNetwork Whether the round of game is online-game.
    GameController(bool hasNetwork) : _isNetworkGame(hasNetwork) {}
    
    /// Real initializing function in two-phase construction in cocos2d-x programs.
    /// \return Whether the initialization succeeds.
    virtual bool init() override;
    
    /// Initalize the network of current game.
    /// \param network The network controller provided by previous scene, null means local game.
    void initNetwork(NetworkController* network);
    
    /// Factory method of GameController, rewrite to acquire one more parameter.
    /// \param hasNetwork Whether the game is local game.
    /// \return An instance of GameController.
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
    using GameStatus = int;
    
    // Here are several constants describing game result.
    enum { Win = 1, Lose = 2, Draw = 3, Processing = 0 };
private:
    enum {
        Ready,    /// \enum Ready Game initalizing succeeded.
        Loading,  /// \enum Loading Loading game or waiting for remote data.
        Waiting,  /// \enum Waiting Waiting for current player to attack.
        Blocking, /// \enum Blocking Waiting until all balls stop.
        End,      /// \enum End Game end.
        Fixed     /// \enum Fixed Ball positions have been fixed this round.
    } _status;
    currentPlayer _currentPlayer;
    Player* _enemy;
    LocalPlayer* _localPlayer;
    
    std::unique_ptr<NetworkController> _net;

    /// Uniformly register all callback functions used in game controller.
    /// \param table A list containing the mapping relationship between event name and handlers.
    void registerCallbacks(std::initializer_list<std::pair<std::string, void (GameController::*)(cocos2d::EventCustom*)>> table);
    bool _isNetworkGame;
    int _timeLeft;
    Timer* _timer;
    int _fixTimes;
    void _handleTime(float);
    void _overRound();
    void _localShootEvent(cocos2d::EventCustom*);
    void _remoteShootEvent(cocos2d::EventCustom*);
    void _localOverRoundEvent(cocos2d::EventCustom*);
    void _fixEvent(cocos2d::EventCustom*);
    void _endFixEvent(cocos2d::EventCustom*);
    void _remoteOverRoundEvent(cocos2d::EventCustom*);
    void _remoteResultEvent(cocos2d::EventCustom*);
    void _gameOverEvent(cocos2d::EventCustom*);
    void _disconnectEvent(cocos2d::EventCustom*);
    void _aiShootEvent(cocos2d::EventCustom* event);
    void _aiOverRoundEvent(cocos2d::EventCustom*);
    NetworkController* _network;
};

#endif // GAME_CONTROLLER_H_
