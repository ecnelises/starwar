#ifndef MENU_SCENE_H_
#define MENU_SCENE_H_


#include "cocos2d.h"
#include "../Controllers/NetworkController.h"

class MenuScene : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual ~MenuScene() {};
    virtual bool init() override;
    CREATE_FUNC(MenuScene)
private:
    void _intoBattleScene(cocos2d::EventCustom*);
    void _inZoom(float);
    void _outZoom(float);
    float _scale;
    bool _waiting;
    NetworkController *_network;
    cocos2d::Sprite *_bg;
};

#endif // MENU_SCENE_H_
