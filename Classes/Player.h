//
//  Player.h
//  ball
//
//  Created by Dcalsky on 16/5/31.
//
//
// Player 功能: 1、向GameController发送请求；2、接受MouseController或者NetworkController发送的请求；3、管理所有属于自己的ball状态;
#ifndef PLAYER_H_
#define PLAYER_H_

#include "Ball.h"
#include "Controllers/MouseController.h"
#include "cocos2d.h"
#include <vector>
#include <memory>

class MouseController;
class NetworkController;

class Player {
public:
    Player() : _balls(std::make_unique<BallsCollection>()) {}
    virtual ~Player() = default;
    //virtual std::unique_ptr<BallsCollection> getBalls();
protected:
    std::unique_ptr<BallsCollection> _balls;
};

class LocalPlayer : public cocos2d::Node, Player {
public:
    virtual bool init() override;
    virtual ~LocalPlayer() {}
    CREATE_FUNC(LocalPlayer);
private:
    MouseController* _mouse;
};

class RemotePlayer : public cocos2d::Node, Player {
public:
    virtual bool init() override;
    virtual ~RemotePlayer() {}
    CREATE_FUNC(RemotePlayer);
private:
    NetworkController* _net;
};

class AIPlayer : public cocos2d::Node, Player {
public:
    virtual bool init() override;
    virtual ~AIPlayer() {}
    CREATE_FUNC(AIPlayer);
};


#endif // PLAYER_H_
