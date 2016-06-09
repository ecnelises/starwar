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

class NetworkController : public cocos2d::Node {
public:
    virtual bool init() override;
    CREATE_FUNC(NetworkController)
private:
    cocos2d::network::SIOClient* _client;
};

#endif // NETWORK_CONTROLLER_H_
