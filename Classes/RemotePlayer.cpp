//===--- RemotePlayer.cpp  ---===//

#include "Player.h"
#include "Config.h"

RemotePlayer::RemotePlayer(bool isStarter)
{
    this->createBalls(isStarter ? 0 : Config::Map::otherSide);
    _balls.addBallsToNode(this);
    this->setActive(isStarter);
}

void RemotePlayer::applyShoot(BallsCollection::BallId id, const Force& force)
{
    if(!_active) {
        return;
    }
    _balls.shootBall(id, force * Config::Data::shootEfficiency);
}

void RemotePlayer::listenDepart()
{
    this->schedule(CC_CALLBACK_1(RemotePlayer::_isDeparted, this), Config::Data::checkRestingInterval,
                   kRepeatForever, 0, "isDeparted");
}

void RemotePlayer::unlistenDepart()
{
    this->unschedule("isDeparted");
}

void RemotePlayer::setActive(bool state)
{
    _active = state;
}
