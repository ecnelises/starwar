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
            _sprite = cocos2d::Sprite::create(moonFrameFile);
            _sprite->setTag(moonTag);
            break;
        case EARTH:
            _radius = earthRadius;
            _force = earthMaxForce;
            _mass = earthMass;
            _linearDamping = earthLinearDamping;
            _sprite = cocos2d::Sprite::create(earthFrameFile);
            _sprite->setTag(earthTag);
            break;
        case SUN:
            _radius = sunRadius;
            _force = sunMaxForce;
            _mass = sunMass;
            _linearDamping = sunLinearDamping;
            _sprite = cocos2d::Sprite::create(sunFrameFile);
            _sprite->setTag(sunTag);
            break;
        default:
            break;
    }
    _sprite->setPosition(position);
    _missed = true;
    // Arguments of PhysicsMaterial: density, restitution, friction
    _ballBody = cocos2d::PhysicsBody::createCircle(_radius,
                                                   cocos2d::PhysicsMaterial(1.0, 1.2, 1.0));
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

//class ABall {
//    //friend Player::fixBall(int ballId, cocos2d::Vec2 pos);
//public:
//    ABall(ABallInitializer bi, cocos2d::Vec2 position);
//    ~ABall();
//    void move(const Force& force);
//    void depart(void);
//    cocos2d::Vec2 position(void) const;
//    bool is(GameTag tag) const;
//    float currentVelocity(void) const;
//protected:
//    cocos2d::Sprite* _sprite;
//    cocos2d::PhysicsBody* _ballBody;
//    float _linearDamping;
//    float _mass;
//};

ABall::ABall(ABallInitializer bi, cocos2d::Vec2 position) : _mass(bi.mass), _linearDamping(bi.linearDamping)
{
    _sprite = cocos2d::Sprite::create(bi.file);
    _sprite->setTag(bi.tag);
    cocos2d::PhysicsMaterial ballMaterial(bi.density,
                                          bi.restitution, bi.friction);
    _ballBody = cocos2d::PhysicsBody::createCircle(bi.radius, ballMaterial);
    _ballBody->setTag(bi.tag);
    
    _ballBody->setCollisionBitmask(1);
    _ballBody->setCategoryBitmask(1);
    _ballBody->setContactTestBitmask(1);
    _ballBody->setDynamic(true);
    _ballBody->setMass(bi.mass);
    _ballBody->setLinearDamping(bi.linearDamping);
    //_ballBody->setAngularDamping(0.2);
    _ballBody->setRotationEnable(false);
    _sprite->setPhysicsBody(_ballBody);
    _sprite->setPosition(position);
}

ABall::~ABall()
{
    _sprite->release();
    _ballBody->removeFromWorld();
    _ballBody->release();
}

void ABall::move(const Force& force)
{
    _ballBody->applyImpulse(force);
}

void ABall::depart(void)
{
    _sprite->setVisible(false);
    _ballBody->setDynamic(false);
    _ballBody->setEnabled(false);
}

cocos2d::Point ABall::position(void) const
{
    return _sprite->getPosition();
}

bool ABall::is(int tag) const
{
    return (_sprite->getTag() == tag) && (_ballBody->getTag() == tag);
}

float ABall::currentVelocity(void) const
{
    return (_ballBody->getVelocity()).length();
}

//struct ABallInitializer {
//    template <class ABallType>
//    ABallInitializer(ABallType ap);
//    
//    ABallInitializerAggregate atCenter(const cocos2d::Point& center);
//    ABallInitializerAggregate withDistance(float distance);
//    ABallInitializerAggregate byLine(void);
//    ABallInitializerAggregate byColumn(void);
//    
//    float density;
//    float restitution;
//    float friction;
//    
//    float radius;
//    float linearDamping;
//    float mass;
//    int tag;
//    std::string file;
//};

ABallInitializerAggregate ABallInitializer::atCenter(const cocos2d::Point &center)
{
    ABallInitializerAggregate aba(*this);
    aba.atCenter(center);
    return aba;
}

ABallInitializerAggregate ABallInitializer::withDistance(float distance)
{
    ABallInitializerAggregate aba(*this);
    aba.withDistance(distance);
    return aba;
}

ABallInitializerAggregate ABallInitializer::byLine(void)
{
    ABallInitializerAggregate aba(*this);
    aba.byLine();
    return aba;
}

ABallInitializerAggregate ABallInitializer::byColumn(void)
{
    ABallInitializerAggregate aba(*this);
    aba.byColumn();
    return aba;
}
