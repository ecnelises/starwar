//===--- BattleScene.h - BattleScene class definition ---===//
/// \file BattleScene.h
/// This file declares BattleScene class, containing main game scene.

#ifndef BATTLE_SCENE_H_
#define BATTLE_SCENE_H_

#include "Ball.h"
#include "cocos2d.h"
#include "Contact.h"
#include "Controllers/MouseController.h"
#include "Controllers/NetworkController.h"
#include <tuple>

/// \class BattleScene
/// \brief Scene used in game playing, either network game or local AI game.
class BattleScene : public cocos2d::Layer {
public:
    /// Create real scene.
    /// \param net Whether it's network game, if so, with instance of network controller.
    /// \return The scene with MenuScene layer as we want.
    static cocos2d::Scene* createScene(std::tuple<bool, NetworkController*> net);
    
    /// Real initializing function in two-phase construction in cocos2d-x programs.
    /// \return Whether the initialization succeeds.
    virtual bool init() override;
    
    /// Factory method for creating scene.
    /// \return A new instance of BattleScene.
    CREATE_FUNC(BattleScene)
private:
    /// Goes back to menu scene.
    /// \param event Just as callback setter requires, not used.
    void _backToMenuScene(cocos2d::EventCustom* event);
};

#endif // BATTLE_SCENE_H_