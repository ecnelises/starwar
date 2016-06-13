#include "Timer.h"
#include "Config.h"

bool Timer::init()
{
    if(!Node::init()) {
        return false;
    }
    _originPosition = cocos2d::Director::getInstance()->getVisibleSize();
    _timer = cocos2d::Sprite::create(timerFrameFile);
    _content = cocos2d::Sprite::create(timerContentFrameFile);
    _timer->setPosition(cocos2d::Vec2(_originPosition.width / 2, _originPosition.height - _timer->getContentSize().height / 2));
    _content->setPosition(cocos2d::Vec2(_originPosition.width / 2 + 2, _originPosition.height - _timer->getContentSize().height / 2 - 2));
    _timeLeft = 12;
    
    this->addChild(_timer, 10);
    this->addChild(_content, 9);
    return true;
}

int Timer::timeGo()
{
    _timeLeft -= 1;
    _content->setScaleX(((float)_timeLeft / 12));
    return _timeLeft;
}

bool Timer::isTimeOut()
{
    return _timeLeft == 0;
}

void Timer::revertTime()
{
    _timeLeft = 12;
    _content->removeFromParent();
    _content = cocos2d::Sprite::create(timerContentFrameFile);
    _content->setPosition(cocos2d::Vec2(_originPosition.width / 2 + 2, _originPosition.height - _timer->getContentSize().height / 2 - 2));
    this->addChild(_content, 9);
}