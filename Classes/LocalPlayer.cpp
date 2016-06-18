#include "Player.h"
#include "Controllers/NetworkController.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <memory>

USING_NS_CC;

LocalPlayer::LocalPlayer(bool isStarter)
{
    auto applyShoot = cocos2d::EventListenerCustom::create("applyShoot", CC_CALLBACK_1(LocalPlayer::_applyShoot, this));
    auto mouseController = MouseController::create();
    //int initNumber = isStarter ? 0 : 7;
    
    this->createBalls(isStarter ? 0 : 800.0f);
    _balls.addBallsToNode(this);
    mouseController->addBalls(&_balls);
    _mouse = mouseController;
    _eventDispatcher->addEventListenerWithFixedPriority(applyShoot, 1);
    
    this->setActive(isStarter);
    this->addChild(mouseController, 10); // Why 10 ? todo
}

void LocalPlayer::setActive(bool state)
{
    // Set cursors on each ball.
    if (!state) {
        auto children = this->getChildren();
        for (const auto& child : children) {
            if (child->getTag() == cursorTag) {
                child->removeFromParentAndCleanup(true);
            }
        }
    } else {
        _mouse->addBalls(&_balls); // Really necessary?
        for (const auto& ball : _balls) {
            // Using textures is better here?
            auto cursor = Sprite::create(cursorFrameFile);
            auto ballPos = _balls.getPosition(ball);
            cursor->setPosition(cocos2d::Vec2(ballPos.x, ballPos.y + 20));
            cursor->setTag(cursorTag);
            this->addChild(cursor, 5);
        }
    }
    _active = state;
    _mouse->setActive(state);
}

void LocalPlayer::_isResting(float dt)
{
    if (!_balls.rest()) {
        return;
    }
    EventCustom overRoundEvent("localOverRound");
    _eventDispatcher->dispatchEvent(&overRoundEvent);
    this->unschedule("isResting"); // 取消监听事件减少消耗
}

void LocalPlayer::applyShoot(BallsCollection::BallId ball, const Force& force)
{
    _balls.shootBall(ball, force * shootEfficiency);
}

void LocalPlayer::_applyShoot(cocos2d::EventCustom *event)
{
    if(!_active) {
        return;
    }
    auto message = static_cast<UnifiedMessageBody*>(event->getUserData());
    EventCustom shootEvent("localShoot");
    applyShoot(message->targetId, message->vec);
    shootEvent.setUserData(message);
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

void Player::createBalls(float diff)
{
    auto centerX = (mapLeftBorder + mapRightBorder) / 2.0f;
    
    auto moonYPos = fabsf(diff - moonPositionY);
    _balls += BallInitializer(MoonBall())
                .atCenter(cocos2d::Point(centerX, moonYPos))
                .withDistance(moonDistance)
                .byLine() * 4;
    
    auto earthYPos = fabsf(diff - earthPositionY);
    _balls += BallInitializer(EarthBall())
                .atCenter(cocos2d::Point(centerX, earthYPos))
                .withDistance(earthDistance)
                .byLine() * 2;
    
    auto sunYPos = fabsf(diff - sunPositionY);
    _balls += BallInitializer(SunBall())
                .atCenter(cocos2d::Point(centerX, sunYPos))
                .withDistance(sunDistance)
                .byLine();
}
