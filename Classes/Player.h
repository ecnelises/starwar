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
#include <string>

class MouseController;
class NetworkController;

class Player {
public:
    Player() : _active(false) {}
    virtual ~Player() = default;
    virtual void setActive(bool) = 0;
    virtual int getBallsNumber()
    {
        return _balls.size();
    }
protected:
    BallsCollection _balls;
    bool _active;
    std::string _playerId;
    std::string _nickname;
};

class LocalPlayer : public cocos2d::Node, public Player {
public:
    virtual bool init() override;
    virtual ~LocalPlayer() = default;
    virtual void setActive(bool) override;
    void applyShoot(Ball*, const Force&);
    CREATE_FUNC(LocalPlayer);
private:
    void _isResting(float);
    void _isDeparted(float);
    MouseController* _mouse;
};


class RemotePlayer : public cocos2d::Node, public Player {
public:
    virtual bool init() override;
    virtual ~RemotePlayer() = default;
    void applyShoot(int, const Force&);
    virtual void setActive(bool) override;
    CREATE_FUNC(RemotePlayer);
private:
    void _isDeparted(float);
};

//class AIPlayer : public cocos2d::Node, Player {
//public:
//    virtual bool init() override;
//    void applyShoot(Ball*, const Force&) {}
//    virtual void setActive(bool) override;
//    virtual ~AIPlayer() {}
//    CREATE_FUNC(AIPlayer);
//};


#endif // PLAYER_H_
