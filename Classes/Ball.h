//===--- Ball.h - Ball class definition ---===//
/// \file Ball.h
/// This file declares several classes related to Ball,
/// including base Ball, Bomb, Curling, etc.

#ifndef BALL_H_
#define BALL_H_

#include "cocos2d.h"
#include "Config.h"
#include <vector>

struct Force {
    Force() = delete;
    Force(const cocos2d::Vec2 d) : direction(d) {}
    cocos2d::Vec2 direction;
};

/// \class Ball
/// \brief Ball is the base class of all other controllable objects on arena.
class Ball : public cocos2d::Ref {
public:
    // What's the parameters?
    Ball(ballType type, cocos2d::Vec2 position);
    virtual ~Ball() {}
    virtual void move(const Force& force);
    virtual void initSchedule(float);
    cocos2d::Sprite* getSprite();
    cocos2d::PhysicsBody* getBallBody();
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
    cocos2d::Vec2 _position;
    //float _speed;
};

/// \class Bomb
/// \brief Any bomb will explode immediately when touching another ball.
//class Bomb : public Ball {
//public:
//    Bomb()
//    {
//        _sprite = cocos2d::Sprite::create(bombFrameFile);
//        _ballBody = cocos2d::PhysicsBody::createCircle(radius);
//        // TODO: Where is physics material?
//        _sprite->setPhysicsBody(_ballBody);
//    }
//    
//    virtual ~Bomb()
//    {
//        _sprite->release();
//        _ballBody->release();
//    }
//    
//    virtual void move(const Force& force) {}
//private:
//    const float radius = 24;
//};

/// \class Curling
/// \brief A curling is a ordinary ball can collide with other balls.
//class Curling : public Ball {
//public:
//    Curling() = default;
//    // TODO: color of Curling should in constructor
//    Curling(ballType, Vec2);
//    virtual ~Curling() {}
//    virtual void move(const Force&) override {}
//};
//

struct NumberedBall {
    Ball* ball;
    unsigned id;
};

using BallsCollection = std::vector<NumberedBall>;

#endif // BALL_H_
