#include "Player.h"
#include "Config.h"

USING_NS_CC;


RemotePlayer::RemotePlayer(bool isStarter)
{
    float diff = isStarter ? 0 : 800.0f;
    int initNumber = isStarter ? 0 : 7;
//    for (int i = 0; i < moonNumber; ++i) {
//        auto ball = new Ball(MOON, initNumber + i + 1, Vec2(moonPositionX + moonDistance * i, fabsf(diff - moonPositionY)));
//        _balls.push_back(ball);
//        this->addChild(ball->getSprite(), 4); // Why 4 ? todo
//    }
    auto centerX = (mapLeftBorder + mapRightBorder) / 2.0f;
    
    auto moonYPos = fabsf(diff - moonPositionY);
    _balls += BallInitializer(MoonBall())
                .atCenter(cocos2d::Point(centerX, moonYPos))
                .withDistance(moonDistance)
                .byLine() * 4;
    
    // earth 2
//    for (int i = 0; i < earthNumber; ++i) {
//        auto ball = new Ball(EARTH, initNumber + i + 5, Vec2(earthPositionX + earthDistance * i, fabsf(diff - earthPositionY)));
//        _balls.push_back(ball);
//        this->addChild(ball->getSprite(), 4);
//    }
    auto earthYPos = fabsf(diff - earthPositionY);
    _balls += BallInitializer(EarthBall())
                .atCenter(cocos2d::Point(centerX, moonYPos))
                .withDistance(earthDistance)
                .byLine() * 2;
    
    auto sunYPos = fabsf(diff - sunPositionY);
    _balls += BallInitializer(SunBall())
                .atCenter(cocos2d::Point(centerX, sunYPos))
                .withDistance(sunDistance)
                .byLine();
    
    // sun 1
//    for (int i = 0; i < sunNumber; ++i) {
//        auto ball = new Ball(SUN, initNumber + i + 7, Vec2(sunPositionX + sunDistance * i, fabsf(diff - sunPositionY)));
//        _balls.push_back(ball);
//        this->addChild(ball->getSprite(), 4);
//    }
    this->setActive(isStarter);
}

void RemotePlayer::applyShoot(Ball* ball, const Vec2 &position)
{
    if(!_active) {
        return;
    }
    ball->move(position);
}

void RemotePlayer::listenDepart()
{
    this->schedule(CC_CALLBACK_1(RemotePlayer::_isDeparted, this), isRestingInterval, kRepeatForever, 0, "isDeparted"); // 发射完小球后立即检测
}

void RemotePlayer::unlistenDepart()
{
    this->unschedule("isDeparted");
}

void RemotePlayer::setActive(bool state)
{
    _active = state;
}