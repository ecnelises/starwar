//===--- Curling.cpp  ---===//

#include "Ball.h"

Curling::Curling(float radius) : _radius(radius)
{
    const float fileBallSize = 24.0;
    _sprite = cocos2d::Sprite::create("ball.png");
    _sprite->setScale(radius / fileBallSize);
    // TODO
    // If we really want to set balls randomly?
    // If so, using C++11-style random generator is better.
    _sprite->setPosition(cocos2d::Vec2(rand() % 901, rand() % 901));
    
    // Arguments of PhysicsMaterial: density, restitution, friction
    _ballBody = cocos2d::PhysicsBody::createCircle(radius,
                                                   cocos2d::PhysicsMaterial(1.0, 1.25, 1.0));
    // We just set up a non-zero value here.
    // Curlings can make collision and contact with other balls.
    _ballBody->setCategoryBitmask(0x00000001);
    _ballBody->setCollisionBitmask(0x00000001);
    _ballBody->setContactTestBitmask(0x00000001);
    
    _ballBody->setGravityEnable(false);
    _ballBody->setLinearDamping(5.0f);
    _ballBody->setRotationEnable(false);
    
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