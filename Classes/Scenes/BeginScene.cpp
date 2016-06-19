//===--- BeginScene.cpp - BeginScene class implementation ---===//
/// \file BeginScene.cpp
/// This file implement interface of BeginScene.

#include "BeginScene.h"
#include "MenuScene.h"
#include "Config.h"
#include "Audio.h"

cocos2d::Scene* BeginScene::createScene()
{
    auto scene = cocos2d::Scene::create();
    auto audio = new Audio();
    audio->playMenuSceneMusic();
    auto layer = BeginScene::create();
    scene->addChild(layer);
    return scene;
}

bool BeginScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    _time = 0.0f;
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto bg = cocos2d::Sprite::create(Config::File::beginSceneFrame);
    auto windowSize = cocos2d::Director::getInstance()->getWinSize();
    
    // Make background fix game screen.
    bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    bg->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bg);
    this->schedule(schedule_selector(BeginScene::_update), 0.8f);
    return true;
}

void BeginScene::_update(float dt)
{
    _time += 1.0f;

    // After 2.4 seconds, give controll to MenuScene.
    if (_time >= 2.4f) {
        this->unschedule(schedule_selector(BeginScene::_update));
        auto menuScene = MenuScene::createScene();
        auto scene = cocos2d::TransitionFade::create(0.8f, menuScene);
        cocos2d::Director::getInstance()->replaceScene(scene);
    }
}