//===--- MouseController.h - MouseController class definition ---===//
/// \file MouseController.h
/// This file declares MouseControlller class which is responsible
/// for handling game events related to mouse.

#ifndef MOUSE_CONTROLLER_H_
#define MOUSE_CONTROLLER_H_

#include "cocos2d.h"
#include <vector>

class Ball;

class MouseController : public cocos2d::Node {
public:
    virtual bool init() override;
    virtual ~MouseController() {}
    void addBalls(std::vector<Ball*>);
    CREATE_FUNC(MouseController)
private:
    void _handleMouseUp(cocos2d::Event*);
    void _handleMouseMove(cocos2d::Event*);
    void _handleMouseDown(cocos2d::Event*);
    // mousecontroller should not "own" balls, it can just "observe".
    // TODO: move balls to Player and set MouseController friend.
    std::vector<Ball*> _balls;
    std::pair<Ball*, bool> selected;
    std::pair<float, float> finalPoint;
    cocos2d::DrawNode *_draw;
};

#endif // MOUSE_CONTROLLER_H_
