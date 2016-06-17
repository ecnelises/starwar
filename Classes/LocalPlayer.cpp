#include <memory>
#include "json/rapidjson.h"
#include "json/document.h"
#include "Player.h"

USING_NS_CC;

LocalPlayer::LocalPlayer(bool isStarter)
{
    auto applyShoot = cocos2d::EventListenerCustom::create("applyShoot", CC_CALLBACK_1(LocalPlayer::_applyShoot, this));
    auto mouseController = MouseController::create();
    float diff = isStarter ? 0 : 800.0f;
    int initNumber = isStarter ? 0 : 7;
//    for (int i = 0; i < moonNumber; ++i) {
//        auto ball = new Ball(MOON, initNumber + i + 1, Vec2(moonPositionX + moonDistance * i, fabsf(diff - moonPositionY)));
//        _balls.push_back(ball);
//        this->addChild(ball->getSprite(), 4); // Why 4 ? todo
//    }
//    
//    // earth 2
//    for (int i = 0; i < earthNumber; ++i) {
//        auto ball = new Ball(EARTH, initNumber + i + 5, Vec2(earthPositionX + earthDistance * i, fabsf(diff - earthPositionY)));
//        _balls.push_back(ball);
//        this->addChild(ball->getSprite(), 4);
//    }
//    
//    // sun 1
//    for (int i = 0; i < sunNumber; ++i) {
//        auto ball = new Ball(SUN, initNumber + i + 7, Vec2(sunPositionX + sunDistance * i, fabsf(diff - sunPositionY)));
//        _balls.push_back(ball);
//        this->addChild(ball->getSprite(), 4);
//    }
    
    mouseController->addBalls(&_balls);
    _mouse = mouseController;
    _eventDispatcher->addEventListenerWithFixedPriority(applyShoot, 1);
    
    this->setActive(isStarter);
    this->addChild(mouseController, 10); // Why 10 ? todo
}

void LocalPlayer::setActive(bool state)
{
    // 如果轮到我操作，就显示光标
//    if(!state) {
//        auto childs = this->getChildren();
//        for(const auto &child : childs) {
//            if(child->getTag() == cursorTag) {
//                child->removeFromParentAndCleanup(true);
//            }
//        }
//    } else {
//        _mouse->addBalls(_balls); // 同步mouseController能控制的球
//        for(const auto &ball : _balls) {
//            auto cursor = Sprite::create(cursorFrameFile);
//            cursor->setPosition(Vec2(ball->getSprite()->getPositionX(), ball->getSprite()->getPositionY() + 20));
//            cursor->setTag(cursorTag);
//            this->addChild(cursor, 5);
//        }
//    }
    _active = state;
    _mouse->setActive(state);
}

void LocalPlayer::_isResting(float dt)
{
    if (_balls.rest()) {
        return;
    }
    EventCustom overRoundEvent("localOverRound");
    _eventDispatcher->dispatchEvent(&overRoundEvent);
    this->unschedule("isResting"); // 取消监听事件减少消耗
}

void LocalPlayer::applyShoot(Ball* ball, const Force& force)
{
    return;
}

void LocalPlayer::_applyShoot(cocos2d::EventCustom *event)
{
    if(!_active) {
        return;
    }
    auto message = *static_cast<std::tuple<Ball*, Force>*>(event->getUserData());
    auto ball = std::get<0>(message);
    auto force = std::get<1>(message);
    auto data = std::make_tuple(ball->getId(), force * ball->getMaxForce());
    EventCustom shootEvent("localShoot");
    
    ball->move(force * ball->getMaxForce());
    shootEvent.setUserData(&data);
    _eventDispatcher->dispatchEvent(&shootEvent);
    
    // Check whether all balls are at rest.
    this->schedule(CC_CALLBACK_1(LocalPlayer::_isResting, this),
                   isRestingInterval, kRepeatForever, 0, "isResting");
}

// Check whether any ball is departed.
void LocalPlayer::listenDepart()
{
    this->schedule(CC_CALLBACK_1(LocalPlayer::_isDeparted, this),
                   isRestingInterval, kRepeatForever, 0, "isDeparted");
}

void LocalPlayer::unlistenDepart()
{
    this->unschedule("isDeparted");
}

void Player::_isDeparted(float dt)
{
    // There's an offset. Only when the ball center has 2.5 away from the border,
    // we call the ball departed.
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    _balls.filter([=](const std::unique_ptr<Ball>& b){
        auto b1 = b->position().x - 2.5f >= visibleSize.width / 2.0f + mapWidth / 2.0f;
        auto b2 = b->position().y - 2.5f >= visibleSize.height / 2.0f + mapHeight / 2.0f;
        auto b3 = b->position().x + 2.5f <= visibleSize.width / 2.0f - mapWidth / 2.0f;
        auto b4 = b->position().y + 2.5f <= visibleSize.height / 2.0f - mapHeight / 2.0f;
        return b1 || b2 || b3 || b4;
    });
}
