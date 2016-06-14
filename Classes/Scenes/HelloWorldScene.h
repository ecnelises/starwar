#ifndef HELLOWORLD_SCENE_H_
#define HELLOWORLD_SCENE_H_

#include "Ball.h"
#include "cocos2d.h"
#include "Contact.h"
#include "../Controllers/MouseController.h"
#include "../Controllers/NetworkController.h"

class HelloWorld : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene(NetworkController*);
    virtual bool init() override;
    CREATE_FUNC(HelloWorld)
private:
    void _backToMenuScene(cocos2d::EventCustom*);
};

#endif // HELLOWORLD_SCENE_H_