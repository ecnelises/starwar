// Player 功能: 1、向GameController发送请求；2、接受MouseController发送的请求；3、管理所有属于自己的ball状态;
#ifndef PLAYER_H_
#define PLAYER_H_

#include "Ball.h"
#include "BallType.h"
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
    
    bool noBall(void) const
    {
        return _balls.empty();
    }
    
    const BallsCollection* getBalls() const
    {
        return &_balls;
    }
    
    void fixBall(int id, cocos2d::Vec2 pos)
    {
        _balls.adjustBallPosition(id, pos);
    }
    virtual void applyShoot(BallsCollection::BallId id, const Force& force) = 0;
protected:
    BallsCollection _balls;
    bool _active;
    std::string _playerId;
    //std::string _nickname;
    void _isDeparted(float dt);
};

class LocalPlayer : public cocos2d::Node, public Player {
public:
    LocalPlayer(bool isStarter);
    virtual ~LocalPlayer()
    {
        _mouse->release();
    }
    
    virtual void setActive(bool) override;
    virtual void applyShoot(BallsCollection::BallId id, const Force& force) override;
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
    virtual void applyShoot(BallsCollection::BallId id, const cocos2d::Vec2& force) override;
    // TODO: parameter
    virtual void setActive(bool) override;
    virtual void listenDepart() override;
    virtual void unlistenDepart() override;
};

class AIPlayer : public cocos2d::Node, public Player {
public:
    AIPlayer(bool isStarter);
    virtual ~AIPlayer() = default;
    virtual void applyShoot(BallsCollection::BallId id, const Force& force) override;
    virtual void setActive(bool) override;
    virtual void listenDepart() override;
    virtual void unlistenDepart() override;
    void findAndShoot(const_observer_ptr<BallsCollection> enemyBalls);
private:
    void _isResting(float);
};

#endif // PLAYER_H_
