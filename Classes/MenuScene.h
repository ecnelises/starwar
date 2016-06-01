//
//  MenuScene.h
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#ifndef MenuScene_h
#define MenuScene_h

#include "cocos2d.h"

class MenuScene : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual ~MenuScene() {};
    virtual bool init() override;
    CREATE_FUNC(MenuScene);
};

#endif /* MenuScene_h */
