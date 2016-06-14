#include "BeginScene.h"
#include "MenuScene.h"
#include "Config.h"
#include "Audio.h"


cocos2d::Scene* BeginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();
    
    // 'layer' is an autorelease object
    auto audio = new Audio();
    audio->playMenuSceneMusic();
    auto layer = BeginScene::create();
    // 获取netWork，初始化给gameController
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool BeginScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto bg = cocos2d::Sprite::create(beginSceneFrameFile);
    _time = 0.0f;
    auto windowSize = cocos2d::Director::getInstance()->getWinSize(); // background image for full screen
    
    bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    bg->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bg);
    this->schedule(schedule_selector(BeginScene::_update), 0.8f);
    return true;
}

void BeginScene::_update(float dt)
{
    _time += 1.0f;
    if(_time >= 2.4f ) {
        this->unschedule(schedule_selector(BeginScene::_update));
        auto menuScene = MenuScene::createScene();
        auto scene =  cocos2d::TransitionFade::create(0.8f, menuScene);
        cocos2d::Director::getInstance()->replaceScene(scene);
    }
}