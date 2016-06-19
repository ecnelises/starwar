//===--- BattleScene.cpp - BattleScene class implementation ---===//
/// \file BattleScene.cpp
/// This file implements interfaces of BattleScene class.

#include "BattleScene.h"
#include "Config.h"
#include "MenuScene.h"
#include "Controllers/GameController.h"

using cocos2d::Vec2;
using cocos2d::Director;
using cocos2d::Sprite;
using cocos2d::Scene;

Scene* BattleScene::createScene(std::tuple<bool, NetworkController*> arg)
{
    auto scene = Scene::createWithPhysics();
    
    bool& isNetworkGame = std::get<0>(arg);
    NetworkController* network = std::get<1>(arg);
    
    auto layer = BattleScene::create();
    auto gameController = GameController::create(isNetworkGame);
    
    gameController->initNetwork(network);
    scene->addChild(gameController, 3);
    scene->addChild(layer);
    // Because of the god view, gravity is zero.
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    return scene;
}

bool BattleScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto bg = Sprite::create(Config::File::battleSceneFrame);
    auto map = Sprite::create(Config::File::battleMapFrame);
    auto backToMenuSceneEvent = cocos2d::EventListenerCustom::create("backToMenuScene", CC_CALLBACK_1(BattleScene::_backToMenuScene, this));
    
    map->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    // Make background fix game screen.
    bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    
    this->addChild(bg, 0);
    this->addChild(map, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(backToMenuSceneEvent, 1);
    return true;
}

// Listen the back to menu scene event from game controller.
void BattleScene::_backToMenuScene(cocos2d::EventCustom *event)
{
    auto menuScene = MenuScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(menuScene);
}