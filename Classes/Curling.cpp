//===--- Curling.cpp  ---===//

#include "Ball.h"
using namespace cocos2d;


Curling::Curling(ballSize ballSize) : Ball() {
    float scale = 1.0f;
    _ballSize = ballSize;
    switch (_ballSize) {
        case SMALL:
            scale = 1.0f;
            break;
        case MEDIUM:
            scale = 1.2f;
            break;
        default:
            scale = 1.5f;
            break;
    }
    _sprite = Sprite::create("ball.png");
    _sprite->setScale(scale);
    _sprite->setPosition(Vec2(rand() % 901, rand() % 901));
    _ballBody = PhysicsBody::createCircle(_sprite->getContentSize().width / 2);
    
    _ballBody->setCategoryBitmask(0x0000000F);
    _ballBody->setCollisionBitmask(0x0000000F);
    _ballBody->setContactTestBitmask(0x0000000F);
    _ballBody->setGravityEnable(false); // 不受重力影响
    _ballBody->setLinearDamping(5.0f); // 线性阻尼
    _ballBody->setRotationEnable(false); // 不转动
    _ballBody->getShape(0)->setRestitution(1.25f); // 反弹
    
    _sprite->setPhysicsBody(_ballBody);
}

Sprite* Ball::getSprite() {
    return _sprite;
}


PhysicsBody* Ball::getBallBody() {
    return _ballBody;
}