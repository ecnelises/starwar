//===--- BeginScene.h - BeginScene class definition ---===//
/// \file BeginScene.h
/// This file declares BeginScene class, which handles logo showing when entering game.

#ifndef PLAYING_SCENE_H_
#define PLAYING_SCENE_H_

#include "cocos2d.h"

/// \class BeginScene
/// \brief Scene used in game starting.
class BeginScene : public cocos2d::Layer {
public:
    /// Create real scene.
    /// \return The scene with BeginScene layer as we want.
    static cocos2d::Scene* createScene();
    
    /// Real initializing function in two-phase construction in cocos2d-x programs.
    /// \return Whether the initialization succeeds.
    virtual bool init() override;
    
    /// Factory method for creating scene.
    /// \return A new instance of BeginScene.
    CREATE_FUNC(BeginScene)
private:
    /// Handler for time passing.
    /// \param dt Time, just as the scheduler requires.
    void _update(float dt);
    
    /// Time passed since game start.
    float _time;
};

#endif // PLAYING_SCENE_H_