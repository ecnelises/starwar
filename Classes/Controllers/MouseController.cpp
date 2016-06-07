//===--- MouseController.cpp - MouseController class implementation ---===//
/// \file MouseController.cpp
/// This file implements interfaces of MouseController.

// TODO: 还需要实现Click的击打方式

#include "MouseController.h"
#include "Ball.h"
#include "Player.h"
#include <functional>

bool MouseController::init()
{
    if (!Node::init()) {
        return false;
    }
    
    _active = false;
    shootMethod = Drag;
    _localBalls = nullptr;
    _selectedBall = nullptr;
    
    auto mouseListener = cocos2d::EventListenerMouse::create();
    
    _drawer = cocos2d::DrawNode::create();
    
    // Bind member functions to mouse listener.
    mouseListener->onMouseUp =
        std::bind(&MouseController::handleMouseUp, this, std::placeholders::_1);
    mouseListener->onMouseDown =
        std::bind(&MouseController::handleMouseDown, this, std::placeholders::_1);
    mouseListener->onMouseMove =
        std::bind(&MouseController::handleMouseMove, this, std::placeholders::_1);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    
    // XXX: Why 10 here?
    this->addChild(_drawer, 10);
    
    // TODO: always true?
    return true;
}

//void MouseController::setPlayer(LocalPlayer* player)
//{
//    _localBalls = (player->_balls).get();
//    _localPlayer = player;
//}

namespace {

cocos2d::Point getCurrentCursor(cocos2d::Event* event)
{
    // TODO: static is safe?
    return cocos2d::Point(static_cast<cocos2d::EventMouse*>(event)->getCursorX(), static_cast<cocos2d::EventMouse*>(event)->getCursorY());
}

} // anonymous namespace

// FIXME: 如果鼠标位置超出了游戏窗口会出现反方向击打的情况
void MouseController::handleMouseUp(cocos2d::Event* event)
{
    if (!_active || _selectedBall == nullptr) {
        return;
    }
    const float maxDistance = 250;
    const float shootForceEfficiency = 1300;
    _drawer->clear();
    // TODO: static_cast is safe?
    auto destPoint = getCurrentCursor(event);
    auto pointDiff = destPoint - _selectedBall->getSprite()->getPosition();
    if (pointDiff.length() > maxDistance) {
        _selectedBall->move(Force(pointDiff, maxDistance * shootForceEfficiency));
    } else {
        _selectedBall->move(Force(pointDiff, pointDiff.length() * shootForceEfficiency));
    }
    _selectedBall = nullptr;
    _active = false;
}

void MouseController::handleMouseMove(cocos2d::Event* event)
{
    const float maxDistance = 250;
    if (!_active || _selectedBall == nullptr) {
        return;
    }
    // TODO: really need this?
    glLineWidth(5.0);
    _drawer->clear();
    auto destPoint = getCurrentCursor(event);
    auto pointDiff = destPoint - _selectedBall->getSprite()->getPosition();
    
    // Distance has a limit.
    if (pointDiff.length() > maxDistance) {
        pointDiff.normalize();
        pointDiff.scale(sqrtf(maxDistance));
        destPoint = _selectedBall->getSprite()->getPosition() + pointDiff;
    }
    _drawer->drawSegment(_selectedBall->getSprite()->getPosition(), destPoint,
                         2, cocos2d::Color4F(1, 0, 0, 0.7));
}

void MouseController::handleMouseDown(cocos2d::Event* event)
{
//    if (!_active) {
//        return;
//    }
//    
//    // Find if there is any ball the cursor in.
//    auto currentCursor = getCurrentCursor(event);
//    for (const auto& f : *_localBalls) {
//        if (!f.moved && ((f.ball)->getSprite()->getBoundingBox()).containsPoint(currentCursor)) {
//            _selectedBall = f.ball.get();
//            return;
//        }
//    }
//    _selectedBall = nullptr;
}
