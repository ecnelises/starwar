//
//  AIPlayer.cpp
//  ball
//
//  Created by Dcalsky on 16/6/7.
//
//

#include "Player.h"
#include "Config.h"
#include <memory>
#include <utility>
#include <algorithm>

USING_NS_CC;
#if 0

class Player {
public:
    Player() : _active(false) {}
    virtual ~Player() = default;
    virtual void setActive(bool) = 0;
    int getBallsNumber() const
    {
        return _balls.size();
    }
protected:
    BallsCollection _balls;
    bool _active;
    std::string _playerId;
    std::string _nickname;
};

class AIPlayer : public cocos2d::Node, Player {
public:
    virtual ~AIPlayer() = default;
    virtual bool init() override;
    void applyShoot(Ball*, const Force&) {}
    virtual void setActive(bool) override;
    CREATE_FUNC(AIPlayer);
private:
    void _isDeparted(float);
};

#endif

bool AIPlayer::init(void)
{
    if (!Node::init()) {
        return false;
    }
    
    // place balls.
    
    return true;
}

void AIPlayer::applyShoot(Ball* ball, const Force& force)
{
    if (!_active) {
        return;
    }
    // TODO: Need shoot event
}

namespace {
    float shortestDistanceToBorder(Vec2 pos)
    {
        // TODO
        return 0.0;
    }
    
    bool compareBallWithDistanceToBorder(Ball* b1, Ball* b2)
    {
        return shortestDistanceToBorder(b1->getSprite()->getPosition()) < shortestDistanceToBorder(b2->getSprite()->getPosition());
    }
} // anonymous namespace

void AIPlayer::findAndShoot(observer_ptr<BallsCollection> aiBalls,
                            observer_ptr<BallsCollection> enemyBalls)
{
    // Find the ball of the enemy nearest to border
    // Find a ball of us nearest to that ball
    // Shoot our ball to the direction of that ball
    auto targetEnemyBall = *std::min_element(enemyBalls->begin(), enemyBalls->end(),
                                             compareBallWithDistanceToBorder);
    auto targetAIBall = *std::min_element(aiBalls->begin(), aiBalls->end(),
                                          compareBallWithDistanceToBorder);
    this->applyShoot(targetAIBall, targetEnemyBall->getSprite()->getPosition() - targetAIBall->getSprite()->getPosition());
}
