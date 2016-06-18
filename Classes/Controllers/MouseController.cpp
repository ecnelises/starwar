//===--- MouseController.cpp - MouseController class implementation ---===//
/// \file MouseController.cpp
/// This file implements interfaces of MouseController.

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
    
    auto mouseListener = cocos2d::EventListenerMouse::create();
    
    _drawer = cocos2d::DrawNode::create();
    glLineWidth(drawWidth);
    
    // Bind member functions to mouse listener.
    mouseListener->onMouseUp = CC_CALLBACK_1(MouseController::handleMouseUp, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(MouseController::handleMouseDown, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(MouseController::handleMouseMove, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    
    // XXX: Why 10 here?
    this->addChild(_drawer, 10);
    this->setTag(mouseControllerTag);
    
    return true;
}

namespace {

    cocos2d::Point getCurrentCursor(cocos2d::Event* event)
    {
        return cocos2d::Point(static_cast<cocos2d::EventMouse*>(event)->getCursorX(),
                              static_cast<cocos2d::EventMouse*>(event)->getCursorY());
    }
    
} // anonymous namespace

void MouseController::handleMouseUp(cocos2d::Event* event)
{
    if (!_active || _selectedBall.isNil()) {
        return;
    }
    const float maxDistance = 250;
    cocos2d::EventCustom applyShoot("applyShoot");
    _drawer->clear();
    
    auto destPoint = getCurrentCursor(event);
    auto pointDiff = destPoint - _balls->getPosition(_selectedBall);
    auto force = Force(-pointDiff);
    if (pointDiff.length() >= maxDistance) {
        pointDiff.normalize();
        pointDiff.scale(maxDistance);
        force = Force(-pointDiff);
    }
    
    auto data = std::make_tuple(_selectedBall, force);
    applyShoot.setUserData(&data);
    _eventDispatcher->dispatchEvent(&applyShoot);
    _selectedBall = BallsCollection::BallId::nil();
}

void MouseController::handleMouseMove(cocos2d::Event* event)
{
    constexpr float maxDistance = 250;
    if (!_active || _selectedBall.isNil()) {
        return;
    }
    // TODO: really need this?
    
    _drawer->clear();
    auto destPoint = getCurrentCursor(event);
    auto pointDiff = destPoint - _balls->getPosition(_selectedBall);
    
    // Distance has a limit.
    if (pointDiff.length() >= maxDistance) {
        pointDiff.normalize();
        pointDiff.scale(maxDistance);
        destPoint = _balls->getPosition(_selectedBall) + pointDiff;
    }
    auto ballCenter = _balls->getPosition(_selectedBall);
    auto bottom = (destPoint - ballCenter).getNormalized().getRPerp();
    bottom.scale(10);
    _drawer->drawTriangle(destPoint, ballCenter + bottom, ballCenter - bottom, cocos2d::Color4F(0.3, 0.3, 0.3, 0.4));
}

void MouseController::handleMouseDown(cocos2d::Event* event)
{
    if (!_active) {
        return;
    }
    // Find if there is any ball the cursor in.
    _selectedBall = _balls->inWhichBall(getCurrentCursor(event));
}

void MouseController::setActive(bool state)
{
    _active = state;
    if(!state) {
        _drawer->clear();
    }
}

void MouseController::addBalls(BallsCollection* balls)
{
    _balls = balls;
}
