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
    Player() = default;
    virtual ~Player() = default;
    virtual void attack() = 0;
protected:
    std::unique_ptr<BallsCollection> _balls;
};

class LocalPlayer : public Player {
    friend class MouseController;
public:
    LocalPlayer()
    {
        _mouse = MouseController::create();
    }
    
    virtual ~LocalPlayer()
    {
        _mouse->release();
    }
    
    virtual void attack() override;
private:
    MouseController* _mouse;
};

class RemotePlayer : public Player {
public:
    RemotePlayer();
    virtual ~RemotePlayer();
    virtual void attack() override;
private:
    NetworkController* _net;
};

#endif // PLAYER_H_
