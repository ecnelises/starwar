#include "Player.h"
#include "Config.h"
#include <memory>
#include <utility>
#include <algorithm>

USING_NS_CC;

AIPlayer::AIPlayer(bool isStarter)
{
    //int initNumber = isStarter ? 0 : 7;
    
    this->createBalls(isStarter ? 0 : 800.0f);
    _balls.addBallsToNode(this);
    this->setActive(isStarter);
}

void AIPlayer::applyShoot(BallsCollection::BallId id, const Force& force)
{
    if (!_active) {
        return;
    }
    EventCustom aiShootEvent("aiShoot");
    _balls.shootBall(id, force * shootEfficiency);
    aiShootEvent.setUserData(nullptr);
    _eventDispatcher->dispatchEvent(&aiShootEvent);
    this->schedule(CC_CALLBACK_1(AIPlayer::_isResting, this), isRestingInterval, kRepeatForever, 0, "isResting");
}

namespace {
    float shortestDistanceToBorder(Vec2 pos)
    {
        return std::min({
            fabsf(pos.x - mapLeftBorder),
            fabsf(pos.x - mapRightBorder),
            fabsf(pos.y - mapTopBorder),
            fabsf(pos.y - mapBottomBorder)
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
    EventCustom overRoundEvent("aiOverRound");
    _eventDispatcher->dispatchEvent(&overRoundEvent);
    this->unschedule("isResting");
}

void AIPlayer::listenDepart()
{
    this->schedule(CC_CALLBACK_1(AIPlayer::_isDeparted, this), isRestingInterval, kRepeatForever, 0, "isDeparted");
}

void AIPlayer::unlistenDepart()
{
    this->unschedule("isDeparted"); // 取消监听事件减少消耗
}
