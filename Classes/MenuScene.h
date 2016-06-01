//
//  MenuScene.h
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#ifndef MENU_SCENE_H_
#define MENU_SCENE_H_

#include "cocos2d.h"

class MenuScene : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual ~MenuScene() {};
    virtual bool init() override;
    CREATE_FUNC(MenuScene);
};

#endif // MENU_SCENE_H_
