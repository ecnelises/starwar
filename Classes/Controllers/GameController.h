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
#include "Timer.h"
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

    /// Uniformly register all callback functions used in game controller.
    /// \param table A list containing the mapping relationship between event name and handlers.
    void registerCallbacks(std::initializer_list<std::pair<std::string, void (GameController::*)(cocos2d::EventCustom*)>> table);

    /// Initalize the AI player;
    void initAIGame(void);
    
    /// Check time passing, when time is over, automatically overround.
    /// \param dt Time, just as the schduler requires.
    void handleTime(float dt);
    
    /// Turn the round over and give shoot chance to the other.
    void overRound(void);
    
    /// Handling events during battle.
    void localShootEvent(cocos2d::EventCustom*);
    void remoteShootEvent(cocos2d::EventCustom*);
    void localOverRoundEvent(cocos2d::EventCustom*);
    void fixEvent(cocos2d::EventCustom*);
    void endFixEvent(cocos2d::EventCustom*);
    void remoteOverRoundEvent(cocos2d::EventCustom*);
    void remoteResultEvent(cocos2d::EventCustom*);
    void gameOverEvent(cocos2d::EventCustom*);
    void disconnectEvent(cocos2d::EventCustom*);
    void aiShootEvent(cocos2d::EventCustom* event);
    void aiOverRoundEvent(cocos2d::EventCustom*);
    
    /// Current type of player: AI, remote or local.
    PlayerType _currentPlayer;
    
    /// The other player, can be either remote or AI.
    Player* _enemy;
    
    /// Player under our control.
    LocalPlayer* _localPlayer;
    
    /// Whether current game need network or not.
    bool _isNetworkGame;
    
    /// Seconds left in current round, if counted down to zero, overround.
    int _timeLeft;
    
    /// Round time counter.
    Timer* _timer;
    
    /// Times balls are fixed. (for network games only)
    int _fixTimes;
    
    /// NetworkController used in game, wrapped in smart pointer.
    NetworkController* _network;
};

#endif // GAME_CONTROLLER_H_
