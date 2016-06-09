//
//  NetworkController.hpp
//  bumpflow
//
//  Created by 邱超凡 on 16/6/2.
//
//

#ifndef NETWORK_CONTROLLER_H_
#define NETWORK_CONTROLLER_H_

#include "cocos2d.h"
#include "SocketIO.h"
#include "Ball.h"
#include <string>
#include <functional>

class GameController;

class GameSocketDelegate : public cocos2d::network::SocketIO::SIODelegate {
public:
    GameSocketDelegate() = delete;
    GameSocketDelegate(GameController* game)
    {
        _game = game;
    }
    
    virtual ~GameSocketDelegate() = default;
    virtual void onClose(cocos2d::network::SIOClient* client) override;
    virtual void onError(cocos2d::network::SIOClient* client,
                         const std::string& data) override;
private:
    observer_ptr<GameController> _game;
};

class NetworkController : public cocos2d::Node {
    friend class GameController;
public:
    NetworkController(GameController* game) : _game(game), _delegate(game)
    {
        _client = cocos2d::network::SocketIO::connect(_destUri, _delegate);
        _client->on("message", CC_CALLBACK_2(NetworkController::dispatchRemoteMessage, this));
    }
    
    ~NetworkController()
    {
        _client->disconnect();
        delete _client;
    }
    
    void sendShoot(int gameid, const std::string& player,
                   int ballid, const Force& force);
    void sendSkip(int gameid, const std::string& player);
    void sendRegisteration(const std::string& player, unsigned ballNum,
                           const std::string& nickname);
private:
    static constexpr auto _destUri = "139.129.12.132:6619";
    GameController* _game;
    cocos2d::network::SIOClient* _client;
    GameSocketDelegate _delegate;
    cocos2d::EventListenerCustom* _networkListener;
    void dispatchRemoteMessage(cocos2d::network::SIOClient* client,
                               const std::string& message);
};

#endif // NETWORK_CONTROLLER_H_
