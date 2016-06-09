#ifndef HELLOWORLD_SCENE_H_
#define HELLOWORLD_SCENE_H_

#include "Ball.h"
#include "cocos2d.h"
#include "Contact.h"
#include "../Controllers/MouseController.h"


class HelloWorld : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene(int);
    virtual bool init() override;
    void update(float dt) override;
    CREATE_FUNC(HelloWorld)
private:
    std::pair<cocos2d::Sprite*, bool> selected;
    std::pair<float, float> finalPoint;
    void addBall(Ball *ball, int zIndex);
};

#endif // HELLOWORLD_SCENE_H_
