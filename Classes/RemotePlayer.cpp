#include "Player.h"
#include "Config.h"

USING_NS_CC;


RemotePlayer::RemotePlayer(bool isStarter)
{
    //int initNumber = isStarter ? 0 : 7;

    this->createBalls(isStarter ? 0 : 800.0f);
    _balls.addBallsToNode(this);
    this->setActive(isStarter);
}

void RemotePlayer::applyShoot(BallsCollection::BallId id, const Force& force)
{
    if(!_active) {
        return;
    }
    _balls.shootBall(id, force * shootEfficiency);
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