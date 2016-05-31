#ifndef HELLOWORLD_SCENE_H__
#define HELLOWORLD_SCENE_H__

#include "Ball.h"
#include "cocos2d.h"
#include "MouseController.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    void update(float dt) override;
    CREATE_FUNC(HelloWorld);
private:
    std::pair<cocos2d::Sprite*, bool> selected;
    std::pair<float, float> finalPoint;
    void addBall(Ball *ball, int zIndex);
    void createMouseListener(MouseController*);
};

#endif // HELLOWORLD_SCENE_H__
