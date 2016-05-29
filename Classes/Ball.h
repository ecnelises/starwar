//
//  Ball.hpp
//  ball
//
//  Created by Dcalsky on 16/5/19.
//
//

#ifndef Ball_hpp
#define Ball_hpp
/*
#include "cocos2d.h"
using namespace cocos2d;

/*
class Ball: public Sprite
{
public:
    static Ball* createBall(const int type);
    Ball();
    virtual ~Ball();
private:
    Sprite *sprite;
    PhysicsBody *ballBody;
    void applyForce(const Vec2 force);
    virtual Vec2& getPosition() const override;
    virtual void setPosition(const Vec2&) override;
};
*/

struct Force {
    Force() = delete;
    Force(const Vec2 d, float f = 1000.0f) : force(f), direction(d) {}
    Vec2 direction;
    float force;
};

class Ball {
public:
    Ball();
    virtual ~Ball();
    virtual void move(const Force& direction);
private:
    Sprite *_sprite;
    PhysicsBody *_ballBody;
    enum {
        small = 24,
        medium = 32,
        big = 48
    } _size;
};
*/

#endif /* Ball_hpp */
