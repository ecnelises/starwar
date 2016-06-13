#ifndef PLAYING_SCENE_H_
#define PLAYING_SCENE_H_

#include "cocos2d.h"

class BeginScene : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(BeginScene)
private:
    void _update(float);
    float _time;
};
#endif // PLAYING_SCENE_H_