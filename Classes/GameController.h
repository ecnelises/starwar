//
//  GameController.hpp
//  ball
//
//  Created by Dcalsky on 16/6/2.
//
//
// GameController 功能：1、创建并初始化地图等非玩家控制的元素；2、拥有两个Player并通过拥有的球数判断输赢；

#ifndef GameController_H_
#define GameController_H_

#include "cocos2d.h"
#include "Player.h"
#include "MouseController.h"

class GameController : public cocos2d::Node {
public:
    virtual ~GameController() {}
    virtual bool init() override;
    CREATE_FUNC(GameController);
private:
    RemotePlayer *_remotePlayer;
    LocalPlayer *_localPlayer;
};

#endif /* GameController_h */
