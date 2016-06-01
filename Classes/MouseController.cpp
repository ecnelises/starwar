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
    
    auto mouseListener = EventListenerMouse::create();
    
    finalPoint = std::make_pair(0.0f, 0.0f);
    _draw = DrawNode::create();;
    
    mouseListener->onMouseUp = CC_CALLBACK_1(MouseController::_handleMouseUp, this);
    //mouseListener->onMouseUp = MouseController::_handleMouseUp;
    mouseListener->onMouseDown = CC_CALLBACK_1(MouseController::_handleMouseDown, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(MouseController::_handleMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    this->addChild(_draw, 10);
    return true;
}

void MouseController::addBalls(std::vector<Ball*> balls)
{
    _balls = balls;
}

void MouseController::_handleMouseUp(cocos2d::Event *event)
{
    Ball* ball;
    if(selected.second) {
        ball = selected.first;
        selected = std::make_pair(ball, false); // 改变按下点的状态
        _draw->clear();
        float x = ball->getSprite()->getPositionX() - finalPoint.first;
        float y = ball->getSprite()->getPositionY() - finalPoint.second;
        x = x < ball->getSprite()->getPositionX() ? x : -x;
        y = y < ball->getSprite()->getPositionY() ? y : -y;
        ball->getBallBody()->applyImpulse(Vec2(x, y) * 1300);
    }
}

void MouseController::_handleMouseMove(cocos2d::Event *event)
{
    EventMouse *e = (EventMouse*) event;
    Ball* ball = nullptr;
    if(selected.first && selected.second) {
        ball = selected.first;
        if(ball != nullptr) {
            glLineWidth(5); // 规定线段的粗细
            float ballX = ball->getSprite()->getPositionX();
            float ballY = ball->getSprite()->getPositionY();
            float cursorX = e->getCursorX(), cursorY = e->getCursorY();
            if(selected.first == ball && selected.second == true) {
                _draw->clear(); // 不清除的话，会出现重影现象
                float diff = sqrt(pow((ballX - cursorX), 2.0f) + pow((ballY - cursorY), 2.0f)); // 当前点到球心线段长度
                float ctan = atan(fabs(ballY - cursorY) / fabs(ballX - cursorX)); // 线段与水平x的夹角
                if(diff > 250) { // 判断是不是超出最大长度
                    float diffX = 250 * cos(ctan); // 超出就按照最大长度来重新计算 应当所在的点
                    float diffY = 250 * sin(ctan);
                    cursorX = ballX > cursorX ? ballX - diffX : ballX + diffX; // 应当点
                    cursorY = ballY > cursorY ? ballY - diffY : ballY + diffY;
                }
                finalPoint = std::make_pair(cursorX, cursorY); // 记录最后点的位置
                _draw->drawSegment(ball->getSprite()->getPosition(), Vec2(cursorX, cursorY), 2, Color4F(1, 0, 0, 1));
            }
        }

    }
}

void MouseController::_handleMouseDown(cocos2d::Event *event) {
    EventMouse *e = (EventMouse*) event;
    for (int i = 0; i < _balls.size(); ++i) {
        auto ballBox = _balls[i]->getSprite()->getBoundingBox();
        if(ballBox.containsPoint(Point(e->getCursorX(), e->getCursorY()))) {
            selected = std::make_pair(_balls[i], true);
        }
    }
}
