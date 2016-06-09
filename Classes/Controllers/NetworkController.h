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

class GameSocketDelegate : public cocos2d::network::SocketIO::SIODelegate {
public:
    GameSocketDelegate() = default;
    virtual ~GameSocketDelegate() = default;
    virtual void onClose(cocos2d::network::SIOClient* client) override;
    virtual void onError(cocos2d::network::SIOClient* client,
                         const std::string& data) override;
};

class NetworkController {
    friend class GameController;
public:
    NetworkController()
    {
        _client = cocos2d::network::SocketIO::connect(_destUri, _delegate);
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
    // TODO: how to tackle with the callback?
private:
    static constexpr auto _destUri = "139.129.12.132:6619";
    cocos2d::network::SIOClient* _client;
    GameSocketDelegate _delegate;
    cocos2d::EventListenerCustom* _networkListener;
};

#endif // NETWORK_CONTROLLER_H_
