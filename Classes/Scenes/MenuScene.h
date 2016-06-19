//===--- MenuScene.h - BeginScene class definition ---===//
/// \file MenuScene.h
/// This file declares MenuScene class, in which Player can choose to play or exit.

#ifndef MENU_SCENE_H_
#define MENU_SCENE_H_

#include "cocos2d.h"

class NetworkController;

/// \class MenuScene
/// \brief MenuScene shows menu items for player to click into battle scene.
class MenuScene : public cocos2d::Layer {
public:
    /// Create real scene.
    /// \return The scene with MenuScene layer as we want.
    static cocos2d::Scene* createScene();
    
    virtual ~MenuScene() = default;
    
    /// Real initializing function in two-phase construction in cocos2d-x programs.
    /// \return Whether the initialization succeeds.
    virtual bool init() override;
    
    /// Factory method for creating scene.
    /// \return A new instance of MenuScene.
    CREATE_FUNC(MenuScene)
private:
    /// Replace into battle scene, when player clicks ONLINE.
    /// \param event Just as the callback setter requires.
    void _intoBattleScene(cocos2d::EventCustom* event);
    
    /// Replace into battle scene, when player clicks COMPUTER.
    void _intoAIScene(void);
    
    /// Zoom in the background image.
    void _inZoom(float);
    
    /// Zoom out the background image.
    void _outZoom(float);
    
    /// Current background image scale.
    float _scale;
    
    /// Whether player is waiting for network now.
    bool _waiting;
    
    /// NetworkController used in waiting, will be given to battlescene.
    NetworkController* _network;
    
    /// Background image sprite.
    cocos2d::Sprite* _bg;
};

#endif // MENU_SCENE_H_
