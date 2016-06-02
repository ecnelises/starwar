//===--- Ball.h - Ball class definition ---===//
/// \file Ball.h
/// This file declares several classes related to Ball,
/// including base Ball, Bomb, Curling, etc.

#ifndef BALL_H_
#define BALL_H_

#include "cocos2d.h"

struct Force {
    Force() = delete;
    Force(const cocos2d::Vec2 d, float f = 1000.0f) : direction(d), force(f) {}
    cocos2d::Vec2 direction;
    float force;
};

/// \class Ball
/// \brief Ball is the base class of all other controllable objects on arena.
class Ball {
public:
    // What's the parameters?
    Ball() {}
    virtual ~Ball() {}
    virtual void move(const Force& force) = 0;
    cocos2d::Sprite* getSprite();
    cocos2d::PhysicsBody* getBallBody();
    bool stationary(void) const
    {
        return _ballBody->isResting();
    }
protected:
    cocos2d::Sprite *_sprite;
    cocos2d::PhysicsBody *_ballBody;
    //float _speed;
};

/// \class Bomb
/// \brief Any bomb will explode immediately when touching another ball.
class Bomb : public Ball {
public:
    Bomb()
    {
        _sprite = cocos2d::Sprite::create("bomb.png");
        _ballBody = cocos2d::PhysicsBody::createCircle(radius);
        // TODO: Where is physics material?
        _sprite->setPhysicsBody(_ballBody);
    }
    
    virtual ~Bomb()
    {
        _sprite->release();
        _ballBody->release();
    }
    
    virtual void move(const Force& force);
private:
    const float radius = 24;
};

/// \class Curling
/// \brief A curling is a ordinary ball can collide with other balls.
class Curling : public Ball {
public:
    Curling() = delete;
    // TODO: color of Curling should in constructor
    Curling(float radius);
    virtual ~Curling() {}
    virtual void move(const Force&) override {}
    
    // These should be defined as constant values instead of enums.
    static constexpr float Small = 24.0;
    static constexpr float Medium = 32.0;
    static constexpr float Large = 48.0;
private:
    float _radius;
};

#endif // BALL_H_
