//===--- Timer.h - Timer class definition ---===//
/// \file Timer.h
/// Timer class handles time counting down.

#ifndef TIMER_H_
#define TIMER_H_

#include "cocos2d.h"

/// \class Timer
/// \brief Timer is delegated for time counting and set callbacks for time out.
class Timer : public cocos2d::Node {
public:
    virtual ~Timer() {};
    virtual bool init() override;
    
    /// Decrease time counter.
    /// \return Seconds left.
    int timeGo(void);
    
    /// Check if time is out.
    /// \return If time's out, return true.
    bool isTimeOut(void);
    
    /// Set time left to 12 and restart counting.
    void revertTime(void);
    CREATE_FUNC(Timer)
private:
    /// Time counter as a sprite.
    cocos2d::Sprite* _timer;
    
    /// Widget for showing times left.
    cocos2d::Sprite* _content;
    
    /// Original position of the widget.
    cocos2d::Size _originPosition;
    
    /// Seconds left for the player. If time's out, control will be
    /// automatically given to enemy.
    int _timeLeft;
};

#endif // TIMER_H_
