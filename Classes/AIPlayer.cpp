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
    
    auto centerX = (mapLeftBorder + mapRightBorder) / 2.0f;
    
    auto moonYPos = fabsf(diff - moonPositionY);
    _balls += BallInitializer(MoonBall())
        .atCenter(cocos2d::Point(centerX, moonYPos))
        .withDistance(moonDistance)
        .byLine() * 4;
    
    auto earthYPos = fabsf(diff - earthPositionY);
    _balls += BallInitializer(EarthBall())
        .atCenter(cocos2d::Point(centerX, earthYPos))
        .withDistance(earthDistance)
        .byLine() * 2;
    
    auto sunYPos = fabsf(diff - sunPositionY);
    _balls += BallInitializer(SunBall())
        .atCenter(cocos2d::Point(centerX, sunYPos))
        .withDistance(sunDistance)
        .byLine();
    
    
    
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
    _balls.addBallsToNode(this);
    
    this->setActive(isStarter);
}

void AIPlayer::applyShoot(BallsCollection::BallId id, const Force& force)
{
    if (!_active) {
        return;
    }
    EventCustom aiShootEvent("aiShoot");
    _balls.shootBall(id, force * 100);
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
    auto targetEnemyBall = *std::min_element(enemyBalls->begin(), enemyBalls->end(),
                                             [=](const BallsCollection::BallId& b1,
                                                 const BallsCollection::BallId& b2){
        return shortestDistanceToBorder(enemyBalls->getPosition(b1)) < shortestDistanceToBorder(enemyBalls->getPosition(b2));
    });
    auto targetAIBall = *std::min_element(_balls.begin(), _balls.end(),
                                          [=](const BallsCollection::BallId& b1,
                                              const BallsCollection::BallId& b2){
        return shortestDistanceToBorder(_balls.getPosition(b1)) < shortestDistanceToBorder(_balls.getPosition(b2));
    });
    this->applyShoot(targetAIBall, enemyBalls->getPosition(targetEnemyBall) - _balls.getPosition(targetAIBall));
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
