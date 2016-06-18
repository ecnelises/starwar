//===--- MouseController.h - MouseController class definition ---===//
/// \file MouseController.h
/// This file declares MouseControlller class which is responsible
/// for handling game events related to mouse.

#ifndef MOUSE_CONTROLLER_H_
#define MOUSE_CONTROLLER_H_

#include "cocos2d.h"
#include "Ball.h"
#include <vector>
#include <memory>

class LocalPlayer;

/// \class MouseController
/// \brief Receiving user operations from mouse and call game controller to tackle.
class MouseController : public cocos2d::Node {
public:
    virtual bool init() override;
    void setActive(bool);
    void addBalls(BallsCollection* balls);
    CREATE_FUNC(MouseController)
private:
    // Users can have two ways to shoot.
    void handleMouseUp(cocos2d::Event*);
    void handleMouseMove(cocos2d::Event*);
    void handleMouseDown(cocos2d::Event*);
    bool _active;
    cocos2d::DrawNode* _drawer;
    // Here the mouse controller just "observe" the balls, which are
    // owned by the player. So we using raw pointer here is good.
    BallsCollection* _balls;
    BallsCollection::BallId _selectedBall;
};

#endif // MOUSE_CONTROLLER_H_
