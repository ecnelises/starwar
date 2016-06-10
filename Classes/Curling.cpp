//===--- Curling.cpp  ---===//

#include "Ball.h"
USING_NS_CC;


Ball::Ball(ballType type, int id, Vec2 position) : _type(type), _id(id)
{
    switch(_type) {
        case MOON:
            _radius = moonRadius;
            _force = moonMaxForce;
            _mass = moonMass;
            _linearDamping = moonLinearDamping;
            //_position = moonPosition;
            _sprite = cocos2d::Sprite::create(moonFrameFile);
            _sprite->setTag(moonTag);
            break;
        case EARTH:
            _radius = earthRadius;
            _force = earthMaxForce;
            _mass = earthMass;
            _linearDamping = earthLinearDamping;
            //_position = earthPosition;
            _sprite = cocos2d::Sprite::create(earthFrameFile);
            _sprite->setTag(earthTag);
            break;
        case SUN:
            _radius = sunRadius;
            _force = sunMaxForce;
            _mass = sunMass;
            _linearDamping = sunLinearDamping;
            //_position = sunPosition;
            _sprite = cocos2d::Sprite::create(sunFrameFile);
            _sprite->setTag(sunTag);
            break;
        default:
            break;
    }
    // TODO
    // If we really want to set balls randomly?
    // If so, using C++11-style random generator is better.
    _sprite->setPosition(position);
    _missed = true;
    // Arguments of PhysicsMaterial: density, restitution, friction
    _ballBody = cocos2d::PhysicsBody::createCircle(_radius,
                                                   cocos2d::PhysicsMaterial(1.0, 1.1, 1.0));
    // We just set up a non-zero value here.
    // Curlings can make collision and contact with other balls.
    _ballBody->setCategoryBitmask(0x00000001);
    _ballBody->setCollisionBitmask(0x00000001);
    _ballBody->setContactTestBitmask(0x00000001);
    _ballBody->setGravityEnable(false);
    _ballBody->setMass(_mass);
    _ballBody->setLinearDamping(_linearDamping);

    _ballBody->setAngularDamping(0.2);
    _ballBody->setRotationEnable(true);

    _sprite->setPhysicsBody(_ballBody);
}

cocos2d::Sprite* Ball::getSprite()
{
    return _sprite;
}


cocos2d::PhysicsBody* Ball::getBallBody()
{
    return _ballBody;
}

void Ball::move(const Force& f)
{
    _ballBody->applyImpulse(f.direction * _force);
    _moved = true;
}

// todo 如何真正去掉ball
void Ball::depart()
{
    _sprite->setVisible(false);
    _ballBody->setEnabled(false);
    _ballBody->setDynamic(false);
}

int Ball::getId()
{
    return this->_id;
}