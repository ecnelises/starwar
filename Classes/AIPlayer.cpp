#include "Player.h"
#include "Config.h"
#include <memory>
#include <utility>
#include <algorithm>

USING_NS_CC;


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

