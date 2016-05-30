#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace std;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float dt) override;
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    pair<cocos2d::Sprite*, bool> selected;
    pair<float, float> finalPoint;
    cocos2d::Sprite* hero;
};

#endif // __HELLOWORLD_SCENE_H__
