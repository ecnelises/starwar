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
    /// Real initializing function in two-phase construction in cocos2d-x programs.
    /// \return Whether the initialization succeeds.
    virtual bool init() override;
    
    /// Set whether current mouse player is enabled.
    /// \param activity If the other is controlling, it's unabled.
    void setActive(bool activity);
    
    /// Set balls of current local player to mouse player.
    /// \param balls Pointer to balls of local player.
    void addBalls(BallsCollection* balls);
    
    /// Factory method of MouseController.
    /// \return An instance of GameController.
    CREATE_FUNC(MouseController)
private:
    /// When mouse is up, player will shoot the ball.
    /// \param event Mouse event containing cursor position.
    void handleMouseUp(cocos2d::Event* event);
    
    /// Redraw triangle.
    /// \param event Mouse event containing cursor position.
    void handleMouseMove(cocos2d::Event* event);
    
    /// Judging if any ball is selected and start drawing.
    /// \param event Mouse event containing cursor position
    void handleMouseDown(cocos2d::Event* event);
    
    /// Whether mouse controller is enabled now.
    bool _active;
    
    /// Drawer used to draw triangles when mouse moving.
    cocos2d::DrawNode* _drawer;
    
    // Here the mouse controller just "observe" the balls, which are
    // owned by the player. So we using raw pointer here is good.
    /// Pointer to local player's balls.
    BallsCollection* _balls;
    
    /// Id of selected ball, if no ball selected, it's nil.
    BallsCollection::BallId _selectedBall;
};

#endif // MOUSE_CONTROLLER_H_
