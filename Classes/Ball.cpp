//
//  Ball.cpp
//  ball
//
//  Created by Dcalsky on 16/5/19.
//
//

/*
#include "Ball.h"
USING_NS_CC;

Ball::Ball(void) { }
Ball::~Ball(void) { }

// 创建小球的简单实现
Ball* Ball::createBall(const int type) {
    auto ball = new Ball();
    if (!ball || !ball->initWithFile("ball.png")) { // 不成功就返回nullptr
        CC_SAFE_DELETE(ball);
        return ball = nullptr;
    }
    
    ball->sprite = Sprite::create("ball.png");
    //ball->sprite->get
    ball->ballBody = PhysicsBody::createBox(ball->getContentSize());
    auto ballBody = ball->ballBody;
    ballBody->setLinearDamping(5.0f); // 线性阻尼
    ballBody->setRotationEnable(false); // 不可滚动
    ballBody->setGravityEnable(false); // 不受重力影响
    ballBody->getShape(0)->setRestitution(1.0f); // 完全反弹
    
    ball->sprite->setPhysicsBody(ballBody);
    
    ball->autorelease();
    return ball;
};

void Ball::setPosition(const Vec2& pos)
{
    sprite->setPosition(pos);
};

Vec2& Ball::getPosition() const
{
    return sprite->getPosition();
};

void Ball::applyForce(const Vec2 force)
{
    ballBody->applyImpulse(force);
};
*/
