//===--- AIPlayer.cpp  ---===//

#include "Player.h"
#include "Controllers/NetworkController.h"
#include "Config.h"
#include <memory>
#include <utility>
#include <algorithm>

AIPlayer::AIPlayer(bool isStarter)
{
    this->createBalls(isStarter ? 0 : Config::Map::otherSide);
    _balls.addBallsToNode(this);
    this->setActive(isStarter);
}

void AIPlayer::applyShoot(BallsCollection::BallId id, const Force& force)
{
    if (!_active) {
        return;
    }
    _balls.shootBall(id, force * Config::Data::shootEfficiency);
    emitMsg("aiShoot");
    this->schedule(CC_CALLBACK_1(AIPlayer::_isResting, this), Config::Data::checkRestingInterval,
                   kRepeatForever, 0, "isResting");
}

namespace {
    
float shortestDistanceToBorder(cocos2d::Vec2 pos)
{
    return std::min({
        fabsf(pos.x - Config::Map::leftBorder),
        fabsf(pos.x - Config::Map::rightBorder),
        fabsf(pos.y - Config::Map::topBorder),
        fabsf(pos.y - Config::Map::bottomBorder)
    });
}
    
} // anonymous namespace

void AIPlayer::findAndShoot(const_observer_ptr<BallsCollection> enemyBalls)
{
    // Find the ball of the enemy nearest to border
    // Find a ball of us nearest to that ball
    // Shoot our ball to the direction of that ball
    BallsCollection::BallId targetEnemyBall = BallsCollection::BallId::nil();
    if (!enemyBalls->empty()) {
        targetEnemyBall = *std::min_element(enemyBalls->begin(), enemyBalls->end(),
                                                 [=](const BallsCollection::BallId& b1,
                                                     const BallsCollection::BallId& b2){
            return shortestDistanceToBorder(enemyBalls->getPosition(b1)) < shortestDistanceToBorder(enemyBalls->getPosition(b2));
        });
    }
    auto targetAIBall = *std::min_element(_balls.begin(), _balls.end(),
                                          [=](const BallsCollection::BallId& b1,
                                              const BallsCollection::BallId& b2){
        return shortestDistanceToBorder(_balls.getPosition(b1)) < shortestDistanceToBorder(_balls.getPosition(b2));
    });
    if (!targetEnemyBall.isNil()) {
        this->applyShoot(targetAIBall, enemyBalls->getPosition(targetEnemyBall) - _balls.getPosition(targetAIBall));
    }
}

void AIPlayer::setActive(bool activity)
{
    _active = activity;
}

void AIPlayer::_isResting(float dt)
{
    if (!_balls.rest()) {
        return;
    }
    emitMsg("aiOverRound");
    this->unschedule("isResting");
}

void AIPlayer::listenDepart()
{
    this->schedule(CC_CALLBACK_1(AIPlayer::_isDeparted, this), Config::Data::checkRestingInterval,
                   kRepeatForever, 0, "isDeparted");
}

void AIPlayer::unlistenDepart()
{
    this->unschedule("isDeparted");
}
