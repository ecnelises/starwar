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
    _selectedBall = nullptr;
    
    auto mouseListener = cocos2d::EventListenerMouse::create();
    
    _drawer = cocos2d::DrawNode::create();
    glLineWidth(drawWidth);
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
    this->setTag(mouseControllerTag);
    
    return true;
}

namespace {

    cocos2d::Point getCurrentCursor(cocos2d::Event* event)
    {
        return cocos2d::Point(static_cast<cocos2d::EventMouse*>(event)->getCursorX(), static_cast<cocos2d::EventMouse*>(event)->getCursorY());
    }
    
} // anonymous namespace

void MouseController::handleMouseUp(cocos2d::Event* event)
{
    if (!_active || _selectedBall == nullptr) {
        return;
    }
    const float maxDistance = 250;
    cocos2d::EventCustom applyShoot("applyShoot");
    _drawer->clear();
    
    auto destPoint = getCurrentCursor(event);
    auto pointDiff = destPoint - _selectedBall->getPosition();
    auto force = Force(-pointDiff);
    if (pointDiff.length() >= maxDistance) {
        pointDiff.normalize();
        pointDiff.scale(maxDistance);
        force = Force(-pointDiff);
    }
    
    auto data = std::make_tuple(_selectedBall, force);
    applyShoot.setUserData(&data);
    _eventDispatcher->dispatchEvent(&applyShoot);
    _selectedBall = nullptr;
}

void MouseController::handleMouseMove(cocos2d::Event* event)
{
    const float maxDistance = 250;
    if (!_active || _selectedBall == nullptr) {
        return;
    }
    // TODO: really need this?
    
    _drawer->clear();
    auto destPoint = getCurrentCursor(event);
    auto pointDiff = destPoint - _selectedBall->getPosition();
    
    // Distance has a limit.
    if (pointDiff.length() >= maxDistance) {
        pointDiff.normalize();
        pointDiff.scale(maxDistance);
        destPoint = _selectedBall->getPosition() + pointDiff;
    }
    auto ballCenter = _selectedBall->getSprite()->getPosition();
    auto bottom = (destPoint - ballCenter).getNormalized().getRPerp();
    bottom.scale(10);
    //_drawer->drawSegment(_selectedBall->getSprite()->getPosition(), destPoint,
    //                     2, cocos2d::Color4F(1, 0, 0, 0.7)); // todo config
    _drawer->drawTriangle(destPoint, ballCenter + bottom, ballCenter - bottom, cocos2d::Color4F(0.3, 0.3, 0.3, 0.4));
    // Semi-transparent gray.
}

void MouseController::handleMouseDown(cocos2d::Event* event)
{
    if (!_active) {
        return;
    }
    // Find if there is any ball the cursor in.
    auto currentCursor = getCurrentCursor(event);
    for (const auto f : _balls) {
        if ((f->getBoundingBox()).containsPoint(currentCursor)) {
            _selectedBall = f;
            return;
        }
    }
    _selectedBall = nullptr;
}

void MouseController::setActive(bool state)
{
    _active = state;
    if(!state) {
        _drawer->clear();
    }
}

void MouseController::addBalls(BallsCollection balls)
{
    _balls = balls;
}