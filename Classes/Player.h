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
#include "Skill.h"
#include "cocos2d.h"

class Player {
public:
    Player() {}
    virtual ~Player() {}
    //virtual bool applySkill(const Skill&) {}
    virtual std::vector<Ball*> getBalls();
protected:
    std::vector<Ball*> _balls;
};


class RemotePlayer : public Player {
public:
    RemotePlayer();
    virtual ~RemotePlayer() {}
};

class LocalPlayer : public Player {
public:
    LocalPlayer();
    virtual ~LocalPlayer() {}
};

#endif // PLAYER_H_
