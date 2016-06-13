//
//  Timer.cpp
//  ball
//
//  Created by Dcalsky on 16/5/31.
//
//

#include "Timer.h"
#include "Config.h"


bool Timer::init()
{
    if(!Node::init()) {
        return false;
    }
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    _timer = cocos2d::Sprite::create(timerFrameFile);
    _content = cocos2d::Sprite::create(timerContentFrameFile);
    _timer->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height - _timer->getContentSize().height / 2));
    _content->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height - _timer->getContentSize().height / 2));
    
    this->addChild(_timer, 10);
    this->addChild(_content, 9);
    return true;
}

int Timer::timeGo()
{
    _timeLeft -= 1;
    return _timeLeft;
}

bool Timer::isTimeOut()
{
    return _timeLeft == 0;
}