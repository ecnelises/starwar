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
    NetworkController();
    ~NetworkController();
    
    std::string getStarter() {
        return _starter;
    }
    std::string getToken() {
        return _token;
    }
    void sendShoot(int ballid, const Force& force);
    void sendOverRound();
    void sendRegisteration(const std::string& playerToken);
    void sendStop(const std::string& player);
    void sendFinish(const std::string& winner);
private:
    static constexpr auto _destUri = "115.159.189.232:6619";
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
    std::string _room;
    std::string _starter;
    std::string _token;
};

#endif // NETWORK_CONTROLLER_H_
