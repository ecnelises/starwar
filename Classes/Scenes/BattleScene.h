#ifndef BATTLE_SCENE_H_
#define BATTLE_SCENE_H_

#include "Ball.h"
#include "cocos2d.h"
#include "Contact.h"
#include "../Controllers/MouseController.h"
#include "../Controllers/NetworkController.h"
#include <tuple>

class BattleScene : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene(std::tuple<bool, NetworkController*>);
    virtual bool init() override;
    CREATE_FUNC(BattleScene)
private:
    void _backToMenuScene(cocos2d::EventCustom*);
};

#endif // BATTLE_SCENE_H_