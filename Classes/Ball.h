//===--- Ball.h - Ball class definition ---===//
/// \file Ball.h
/// This file declares the Ball class.

#ifndef BALL_H_
#define BALL_H_

#include "Config.h"
#include "cocos2d.h"
#include <vector>
#include <string>
#include <memory>

using Force = cocos2d::Vec2;

/// \class Ball
/// \brief Ball is the main object on the game arena.
class Ball {
public:
    // What's the parameters?
    Ball(ballType type, int id, cocos2d::Vec2 position);
    virtual ~Ball() {}
    void move(const Force& force) {
        _ballBody->applyImpulse(force);
        _moved = true;
    }
    void depart()
    {
        _sprite->setVisible(false);
        _ballBody->setDynamic(false);
        _ballBody->setEnabled(false);
    }
    int getId()
    {
        return this->_id;
    }
    float getMaxForce()
    {
        return _force;
    }
    cocos2d::Vec2 getPosition() {
        return _sprite->getPosition();
    }
    cocos2d::Sprite *getSprite() {
        return _sprite;
    }
    cocos2d::PhysicsBody *getBallBody() {
        return _ballBody;
    }
    cocos2d::Rect getBoundingBox() {
        return _sprite->getBoundingBox();
    }
protected:
    cocos2d::Sprite *_sprite;
    cocos2d::PhysicsBody *_ballBody;
    bool _moved;
    bool _missed;
    ballType _type;
    float _radius;
    float _force;
    float _mass;
    float _linearDamping;
    int _id;
    //float _speed;
};

using BallsCollection = std::vector<Ball*>;

class ABallInitializerAggregate;

struct ABallInitializer {
    template <class ABallType>
    ABallInitializer(ABallType ap) : density(ap.density),
        restitution(ap.restitution), friction(ap.friction), radius(ap.radius),
        linearDamping(ap.linearDamping), mass(ap.mass), tag(ap.tag),
        file(ap.file)
    {
    }
    
    ABallInitializerAggregate atCenter(const cocos2d::Point& center);
    ABallInitializerAggregate withDistance(float distance);
    ABallInitializerAggregate byLine(void);
    ABallInitializerAggregate byColumn(void);
    
    float density;
    float restitution;
    float friction;

    float radius;
    float linearDamping;
    float mass;
    int tag;
    std::string file;
};

class ABall {
    //friend Player::fixBall(int ballId, cocos2d::Vec2 pos);
public:
    ABall(ABallInitializer bi, cocos2d::Vec2 position);
    ABall(const ABall& b) = delete;
    ~ABall();
    void move(const Force& force);
    void depart(void);
    cocos2d::Point position(void) const;
    bool is(int tag) const;
    float currentVelocity(void) const;
protected:
    cocos2d::Sprite* _sprite;
    cocos2d::PhysicsBody* _ballBody;
    float _linearDamping;
    float _mass;
};

class ABallInitializerAggregate {
public:
    ABallInitializerAggregate(const ABallInitializer& bi);
    ABallInitializerAggregate& atCenter(const cocos2d::Point& center);
    ABallInitializerAggregate& withDistance(float distance);
    ABallInitializerAggregate& byLine(void);
    ABallInitializerAggregate& byColumn(void);
    ABallInitializerAggregate& operator * (unsigned number);
    std::vector<cocos2d::Point> getPositions(void);
private:
    ABallInitializer _unitedInitializer;
    unsigned _number;
    float _spacing;
    cocos2d::Point _center;
    enum { ByLine, ByColumn } _layout;
};

class ABallsCollection {
public:
    ABallsCollection() = default;
    ~ABallsCollection() = default;
    ABallsCollection& operator += (const ABallInitializerAggregate& ballInit);
private:
    std::vector<std::unique_ptr<ABall>> _balls;
};

#endif // BALL_H_