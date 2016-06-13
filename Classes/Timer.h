#ifndef TIMER_H_
#define TIMER_H_

#include "cocos2d.h"

class Timer : public cocos2d::Node {
public:
    virtual ~Timer() {};
    virtual bool init() override;
    int timeGo();
    bool isTimeOut();
    CREATE_FUNC(Timer)
private:
    cocos2d::Sprite *_timer;
    cocos2d::Sprite *_content;
    int _timeLeft;
};

#endif // TIMER_H_
