//===--- Ball.cpp  ---===//

#include "Ball.h"

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
    _ballBody->setRotationEnable(false);
    _sprite->setPhysicsBody(_ballBody);
    _sprite->setPosition(position);
}

Ball::~Ball()
{
    this->_sprite->setTag(Config::Tag::departed);
    this->_ballBody->setTag(Config::Tag::departed);
    this->depart();
    //_sprite->release();
    //_ballBody->removeFromWorld();
    //_ballBody->release();
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
    auto num = _number;
    if (num % 2 == 0) {
        offset = _spacing / 2.0f;
    } else {
        positions.push_back(_center);
        offset = _spacing;
        --num;
    }
    if (_layout == ByLine) {
        for (int i = 0; i < num / 2; ++i) {
            positions.push_back(cocos2d::Point(_center.x - offset, _center.y));
            positions.push_back(cocos2d::Point(_center.x + offset, _center.y));
            offset += _spacing;
        }
    } else if (_layout == ByColumn) {
        for (int i = 0; i < num / 2; ++i) {
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

void BallsCollection::shootBall(BallId id, const Force &force)
{
    if (_balls.find(id.id) == _balls.end()) {
        return;
    }
    _balls.at(id.id)->move(force);
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

BallsCollection::BallId BallsCollection::inWhichBall(const cocos2d::Point& cursor) const
{
    for (const auto& ball : _balls) {
        if ((ball.second)->_sprite->getBoundingBox().containsPoint(cursor)) {
            return BallId(ball.first);
        }
    }
    return BallId::nil();
}

void BallsCollection::adjustBallPosition(BallsCollection::BallId id, cocos2d::Vec2 dest)
{
    for (const auto& ball : _balls) {
        if (ball.first == id.id) {
            (ball.second)->_sprite->setPosition(dest);
            return;
        }
    }
}

cocos2d::Point BallsCollection::getPosition(BallsCollection::BallId id) const
{
    for (const auto& ball : _balls) {
        if (ball.first == id.id) {
            return (ball.second)->_sprite->getPosition();
        }
    }
    return cocos2d::Point(0, 0);
}

void BallsCollection::addBallsToNode(cocos2d::Node* parent)
{
    for (auto& ball : _balls) {
        parent->addChild(ball.second->_sprite);
    }
}

bool BallsCollection::ballWithTag(BallsCollection::BallId id, int tag) const
{
    for (const auto& i : _balls) {
        if (i.first == id.id) {
            return i.second->is(tag);
        }
    }
    return false;
}
