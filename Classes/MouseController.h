//
//  MouseController.hpp
//  ball
//
//  Created by Dcalsky on 16/5/30.
//
//

#ifndef MOUSE_CONTROLLER_H_
#define MOUSE_CONTROLLER_H_

#include "Ball.h"
#include "cocos2d.h"

class MouseController : public cocos2d::Node {
public:
    virtual bool init() override;
    virtual ~MouseController() {};
    void addBalls(std::vector<Ball*>);
    CREATE_FUNC(MouseController);
private:
    void _handleMouseUp(cocos2d::Event*);
    void _handleMouseMove(cocos2d::Event*);
    void _handleMouseDown(cocos2d::Event*);
    std::vector<Ball*> _balls;
    std::pair<Ball*, bool> selected;
    std::pair<float, float> finalPoint;
    cocos2d::DrawNode *_draw;
};

#endif // MOUSE_CONTROLLER_H_
