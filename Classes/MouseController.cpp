//
//  MouseController.cpp
//  ball
//
//  Created by Dcalsky on 16/5/30.
//
//

#include "MouseController.h"
USING_NS_CC;

bool MouseController::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    // Keyboard Listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseUp = CC_CALLBACK_1(MouseController::_handleMouseUp, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(MouseController::_handleMouseDown, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(MouseController::_handleMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}

void MouseController::addBalls(std::vector<Ball*> balls) {
    _balls = balls;
}

void MouseController::_handleMouseUp(cocos2d::Event *event) {
    EventMouse *e = (EventMouse*) event;
    auto itBall = _balls.begin();
    while(itBall != _balls.end()) {
//        auto ballBox = itBall->getSprite()->getBoundingBox();
//        if(ballBox.containsPoint(Point(e->getCursorX(), e->getCursorY()))) {
//            printf("cc\n");
//        }
        ++itBall;
    }

}

void MouseController::_handleMouseMove(cocos2d::Event *event) {
    
}

void MouseController::_handleMouseDown(cocos2d::Event *event) {
    
}
