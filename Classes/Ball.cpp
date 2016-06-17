//===--- Curling.cpp  ---===//

#include "Ball.h"

//USING_NS_CC;

//Ball::Ball(ballType type, int id, Vec2 position) : _type(type), _id(id)
//{
//    switch(_type) {
//        case MOON:
//            _radius = moonRadius;
//            _force = moonMaxForce;
//            _mass = moonMass;
//            _linearDamping = moonLinearDamping;
//            _sprite = cocos2d::Sprite::create(moonFrameFile);
//            _sprite->setTag(moonTag);
//            break;
//        case EARTH:
//            _radius = earthRadius;
//            _force = earthMaxForce;
//            _mass = earthMass;
//            _linearDamping = earthLinearDamping;
//            _sprite = cocos2d::Sprite::create(earthFrameFile);
//            _sprite->setTag(earthTag);
//            break;
//        case SUN:
//            _radius = sunRadius;
//            _force = sunMaxForce;
//            _mass = sunMass;
//            _linearDamping = sunLinearDamping;
//            _sprite = cocos2d::Sprite::create(sunFrameFile);
//            _sprite->setTag(sunTag);
//            break;
//        default:
//            break;
//    }
//    _sprite->setPosition(position);
//    _missed = true;
//    // Arguments of PhysicsMaterial: density, restitution, friction
//    _ballBody = cocos2d::PhysicsBody::createCircle(_radius,
//                                                   cocos2d::PhysicsMaterial(1.0, 1.2, 1.0));
//    // We just set up a non-zero value here.
//    // Curlings can make collision and contact with other balls.
//    _ballBody->setCategoryBitmask(0x00000001);
//    _ballBody->setCollisionBitmask(0x00000001);
//    _ballBody->setContactTestBitmask(0x00000001);
//    _ballBody->setGravityEnable(false);
//    _ballBody->setMass(_mass);
//    _ballBody->setLinearDamping(_linearDamping);
//
//    _ballBody->setAngularDamping(0.2);
//    _ballBody->setRotationEnable(true);
//
//    _sprite->setPhysicsBody(_ballBody);
//}

Ball::Ball(const BallInitializer& bi, cocos2d::Vec2 position) : _mass(bi.mass), _linearDamping(bi.linearDamping)
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

Ball::~Ball()
{
    _sprite->release();
    _ballBody->removeFromWorld();
    _ballBody->release();
}

void Ball::move(const Force& force)
{
    _ballBody->applyImpulse(force);
}

void Ball::depart(void)
{
    _sprite->setVisible(false);
    _ballBody->setDynamic(false);
    _ballBody->setEnabled(false);
}

cocos2d::Point Ball::position(void) const
{
    return _sprite->getPosition();
}

bool Ball::is(int tag) const
{
    return (_sprite->getTag() == tag) && (_ballBody->getTag() == tag);
}

float Ball::currentVelocity(void) const
{
    return (_ballBody->getVelocity()).length();
}

BallInitializerAggregate BallInitializer::atCenter(const cocos2d::Point &center)
{
    BallInitializerAggregate aba(*this);
    aba.atCenter(center);
    return aba;
}

BallInitializerAggregate BallInitializer::withDistance(float distance)
{
    BallInitializerAggregate aba(*this);
    aba.withDistance(distance);
    return aba;
}

BallInitializerAggregate BallInitializer::byLine(void)
{
    BallInitializerAggregate aba(*this);
    aba.byLine();
    return aba;
}

BallInitializerAggregate BallInitializer::byColumn(void)
{
    BallInitializerAggregate aba(*this);
    aba.byColumn();
    return aba;
}

BallInitializerAggregate::BallInitializerAggregate(const BallInitializer& bi) : _unitedInitializer(bi), _number(1)
{
}

BallInitializerAggregate& BallInitializerAggregate::atCenter(const cocos2d::Point &center)
{
    _center = center;
    return *this;
}

BallInitializerAggregate& BallInitializerAggregate::withDistance(float distance)
{
    _spacing = distance;
    return *this;
}

BallInitializerAggregate& BallInitializerAggregate::byLine(void)
{
    _layout = ByLine;
    return *this;
}

BallInitializerAggregate& BallInitializerAggregate::byColumn(void)
{
    _layout = ByColumn;
    return *this;
}

BallInitializerAggregate& BallInitializerAggregate::operator * (unsigned number)
{
    if (_number == 0) {
        _number = number;
    } else {
        _number *= number;
    }
    return *this;
}

std::vector<cocos2d::Point> BallInitializerAggregate::getPositions(void) const
{
    std::vector<cocos2d::Point> positions;
    positions.reserve(_number);
    auto offset = 0.0f;
    if (_number % 2 == 0) {
        offset = _spacing / 2.0f;
    } else {
        positions.push_back(_center);
        offset = _spacing;
    }
    if (_layout == ByLine) {
        for (int i = 0; i < (_number - 1) / 2; ++i) {
            positions.push_back(cocos2d::Point(_center.x - offset, _center.y));
            positions.push_back(cocos2d::Point(_center.x + offset, _center.y));
            offset += _spacing;
        }
    } else if (_layout == ByColumn) {
        for (int i = 0; i < (_number - 1) / 2; ++i) {
            positions.push_back(cocos2d::Point(_center.x, _center.y - offset));
            positions.push_back(cocos2d::Point(_center.x, _center.y + offset));
            offset += _spacing;
        }
    }
    return positions;
}

const BallInitializer& BallInitializerAggregate::getInitializer() const
{
    return _unitedInitializer;
}

BallsCollection& BallsCollection::operator += (const BallInitializerAggregate& ballInit)
{
    auto&& positions = ballInit.getPositions();
    for (const auto& p : positions) {
        _balls.insert(std::make_pair(++_vernier, std::make_unique<Ball>(ballInit.getInitializer(),p)));
    }
    return *this;
}

bool BallsCollection::empty() const
{
    return _balls.empty();
}

void BallsCollection::shootBall(int id, const Force &force)
{
    if (_balls.find(id) == _balls.end()) {
        return;
    }
    _balls.at(id)->move(force);
}

bool BallsCollection::rest(void) const
{
    for (const auto& ball : _balls) {
        if ((ball.second)->currentVelocity() > 0.01f) {
            return false;
        }
    }
    return true;
}