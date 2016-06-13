// Player 功能: 1、向GameController发送请求；2、接受MouseController发送的请求；3、管理所有属于自己的ball状态;
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
    virtual void listenDepart() = 0;
    virtual void unlistenDepart() = 0;
    virtual BallsCollection getBalls()
    {
        return _balls;
    }
    virtual int getBallsNumber()
    {
        return _balls.size();
    }
    virtual void fixBall(int ballId, cocos2d::Vec2 pos)
    {
        for(const auto &ball : _balls) {
            if(ball->getId() == ballId) {
                ball->getSprite()->setPosition(pos);
            }
        }
    }
protected:
    BallsCollection _balls;
    bool _active;
    std::string _playerId;
    std::string _nickname;
    void _isDeparted(float dt);
};

class LocalPlayer : public cocos2d::Node, public Player {
public:
    LocalPlayer(bool isStarter);
    virtual ~LocalPlayer() = default;
    virtual void setActive(bool) override;
    void applyShoot(Ball*, const Force&);
    virtual void listenDepart() override;
    virtual void unlistenDepart() override;
private:
    void _isResting(float);
    void _applyShoot(cocos2d::EventCustom*);
    MouseController* _mouse;
};


class RemotePlayer : public cocos2d::Node, public Player {
public:
    RemotePlayer(bool isStarter);
    virtual ~RemotePlayer() = default;
    void applyShoot(int, const cocos2d::Vec2&);
    virtual void setActive(bool) override;
    virtual void listenDepart() override;
    virtual void unlistenDepart() override;
};

class AIPlayer : public cocos2d::Node, public Player {
public:
    virtual ~AIPlayer() = default;
    void applyShoot(Ball*, const Force&);
    virtual void setActive(bool) override {}
    virtual void listenDepart() override {}
    virtual void unlistenDepart() override {}
    void findAndShoot(observer_ptr<BallsCollection> aiBalls, observer_ptr<BallsCollection> enemyBalls);
};

#endif // PLAYER_H_
