//===--- Balls.h - Ball class definition ---===//
/// \file Balls.h
/// This file declares several classes related to Ball,
/// including base Ball, Bomb, Curling, etc.

#ifndef BALLS_H_
#define BALLS_H_

#include "cocos2d.h"

// TODO: Such simple classes should be moved to a single header.
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
    Ball();
    virtual ~Ball() = 0;
    virtual void move(const Force& force) = 0;
protected:
    cocos2d::Sprite *_sprite;
    cocos2d::PhysicsBody *_ballBody;
};

/// \class Bomb
/// \brief Any bomb will explode immediately when touching another ball.
class Bomb : public Ball {
public:
    Bomb();
    virtual ~Bomb();
    virtual void move(const Force& force);
};

/// \class Curling
/// \brief A curling is a ordinary ball can collide with other balls.
class Curling : public Ball {
public:
    Curling();
    virtual ~Curling();
    virtual void move(const Force& force);
};

#endif // BALLS_H_
