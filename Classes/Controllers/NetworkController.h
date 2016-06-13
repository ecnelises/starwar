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
using Socket = cocos2d::network::SIOClient;
/// \class GameSocketDelegate
/// \brief A helper class mainly used to handle error and close event.
class GameSocketDelegate : public cocos2d::network::SocketIO::SIODelegate {
public:
    GameSocketDelegate() = default;
    
    virtual ~GameSocketDelegate() = default;
    virtual void onClose(Socket*) override;
    virtual void onError(Socket*, const std::string&) override;
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
    void sendShoot(int ballid, const Force&);
    void sendOverRound();
    void sendEndFixed();
    void sendGameOver(int);
    void sendFixed(int ballId, cocos2d::Vec2);
private:
    static constexpr auto _destUri = "115.159.189.232:6619";
    observer_ptr<GameController> _game;
    Socket* _client;
    GameSocketDelegate _delegate;
    void dispatchShoot(Socket*, const std::string&);
    void dispatchFixed(Socket*, const std::string&);
    void dispatchReady(Socket*, const std::string&);
    void dispatchGameOver(Socket*, const std::string&);
    void dispatchRound(Socket*);
    void dispatchEndFixed(Socket*);
    void dispatchWait(Socket*);
    void dispatchConnect(Socket*);
    std::string _room;
    std::string _starter;
    std::string _token;
};

#endif // NETWORK_CONTROLLER_H_
