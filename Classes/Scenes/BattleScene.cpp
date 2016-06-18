#include "BattleScene.h"
#include "Config.h"
#include "MenuScene.h"
#include "../Controllers/GameController.h"

USING_NS_CC;

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
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    return scene;
}

bool BattleScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto bg = Sprite::create(battleSceneFrameFile);
    auto map = Sprite::create(battleMapFrameFile);
    auto backToMenuSceneEvent = cocos2d::EventListenerCustom::create("backToMenuScene", CC_CALLBACK_1(BattleScene::_backToMenuScene, this));
    
    map->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    
    this->addChild(bg, 0);
    this->addChild(map, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(backToMenuSceneEvent, 1);
    return true;
}

void BattleScene::_backToMenuScene(cocos2d::EventCustom *event)
{
    auto menuScene = MenuScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(menuScene);
}