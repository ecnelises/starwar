//===--- NetworkController.h - Ball class definition ---===//
/// \file NetworkController.h
/// This file declares both the delegate class and controller
/// class used in network communication of this game.

#ifndef NETWORK_CONTROLLER_H_
#define NETWORK_CONTROLLER_H_

#include "cocos2d.h"
#include "SocketIO.h"
#include "Ball.h"
#include <string>

class GameController;

/// \class GameSocketDelegate
/// \brief A helper class mainly used to handle error and close event.
class GameSocketDelegate : public cocos2d::network::SocketIO::SIODelegate {
public:
    GameSocketDelegate() = delete;
    GameSocketDelegate(cocos2d::EventDispatcher* dispatcher)
    {
        _gameDispatcher = dispatcher;
    }
    
    virtual ~GameSocketDelegate() = default;
    virtual void onClose(cocos2d::network::SIOClient* client) override;
    virtual void onError(cocos2d::network::SIOClient* client,
                         const std::string& data) override;
private:
    cocos2d::EventDispatcher* _gameDispatcher;
};

/// \class NetworkController
/// \brief Part of gamecontroller, just receiving and sending messages to remote server.
class NetworkController {
    friend class GameController;
public:
    NetworkController(GameController* game);
    NetworkController(cocos2d::EventDispatcher* dispatcher);
    ~NetworkController();
    
    void sendShoot(int gameid, const std::string& player,
                   int ballid, const Force& force);
    void sendSkip(int gameid, const std::string& player);
    void sendRegisteration(const std::string& player, unsigned ballNum,
                           const std::string& nickname);
    void sendStop(int gameid, const std::string& player);
    void sendFinish(int gameid, const std::string& winner);
private:
    static constexpr auto _destUri = "139.129.12.132:6619";
    observer_ptr<GameController> _game;
    cocos2d::network::SIOClient* _client;
    GameSocketDelegate _delegate;
    cocos2d::EventDispatcher* _gameDispatcher;
    //cocos2d::EventListenerCustom* _networkListener;
    void dispatchShoot(cocos2d::network::SIOClient* client,
                               const std::string& message);
    void dispatchInitialization(cocos2d::network::SIOClient* client,
                                const std::string& message);
    void dispatchRound(cocos2d::network::SIOClient* client,
                                const std::string& message);
    void dispatchResult(cocos2d::network::SIOClient* client,
                                const std::string& message);
};

#endif // NETWORK_CONTROLLER_H_
