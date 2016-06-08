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

class NetworkController {
public:
    NetworkController() = default;
    virtual ~NetworkController() = default;
    bool sendData();
    bool receiveData();
};

#endif // NETWORK_CONTROLLER_H_
