#include "Player.h"
#include "Config.h"
#include <memory>
#include <utility>
#include <algorithm>

USING_NS_CC;

AIPlayer::AIPlayer(bool isStarter)
{
    float diff = isStarter ? 0 : 768.0f;
    int initNumber = isStarter ? 0 : 7;
//    for (int i = 0; i < moonNumber; ++i) {
//        auto ball = new Ball(MOON, initNumber + i + 1, Vec2(moonPositionX + moonDistance * i, fabsf(diff - moonPositionY)));
//        _balls.push_back(ball);
//        this->addChild(ball->getSprite(), 4); // Why 4 ? todo
//    }
    
    // earth 2
//    for (int i = 0; i < earthNumber; ++i) {
//        auto ball = new Ball(EARTH, initNumber + i + 5, Vec2(earthPositionX + earthDistance * i, fabsf(diff - earthPositionY)));
//        _balls.push_back(ball);
//        this->addChild(ball->getSprite(), 4);
//    }
    
    // sun 1
//    for (int i = 0; i < sunNumber; ++i) {
//        auto ball = new Ball(SUN, initNumber + i + 7, Vec2(sunPositionX + sunDistance * i, fabsf(diff - sunPositionY)));
//        _balls.push_back(ball);
//        this->addChild(ball->getSprite(), 4);
//    }
    
    this->setActive(isStarter);
}

void AIPlayer::applyShoot(Ball* ball, const Force& force)
{
    if (!_active) {
        return;
    }
    EventCustom aiShootEvent("aiShoot");
    ball->move(force * ball->getMaxForce());
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
    
    bool compareBallWithDistanceToBorder(Ball* b1, Ball* b2)
    {
        return shortestDistanceToBorder(b1->_sprite->getPosition()) < shortestDistanceToBorder(b2->_sprite->getPosition());
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

void AIPlayer::setActive(bool activity)
{
    _active = activity;
}

void AIPlayer::_isResting(float dt)
{
    if (_balls.rest()) {
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
