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
    GameSocketDelegate() = default;
    
    virtual ~GameSocketDelegate() = default;
    virtual void onClose(cocos2d::network::SIOClient* client) override;
    virtual void onError(cocos2d::network::SIOClient* client,
                         const std::string& data) override;
};

/// \class NetworkController
/// \brief Part of gamecontroller, just receiving and sending messages to remote server.
class NetworkController : public cocos2d::Node {
public:
    NetworkController(GameController* game);
    ~NetworkController();
    
    void sendShoot(int gameid, const std::string& player,
                   int ballid, const Force& force);
    void sendSkip(int gameid, const std::string& player);
    void sendRegisteration(const std::string& playerToken);
    void sendStop(int gameid, const std::string& player);
    void sendFinish(int gameid, const std::string& winner);
private:
    static constexpr auto _destUri = "127.0.0.1:6619";
    observer_ptr<GameController> _game;
    cocos2d::network::SIOClient* _client;
    GameSocketDelegate _delegate;
    void dispatchShoot(cocos2d::network::SIOClient* client,
                       const std::string& message);
    void dispatchReady(cocos2d::network::SIOClient* client,
                                const std::string& message);
    void dispatchRound(cocos2d::network::SIOClient* client,
                       const std::string& message);
    void dispatchResult(cocos2d::network::SIOClient* client,
                        const std::string& message);
    void dispatchWait(cocos2d::network::SIOClient* client);
    void dispatchConnect(cocos2d::network::SIOClient* client);
};

#endif // NETWORK_CONTROLLER_H_
