//
//  MenuScene.cpp
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#include "MenuScene.h"
#include "HelloWorldScene.h"
#include "Config.h"
#include "Audio.h"


USING_NS_CC;

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();
    auto audio = new Audio();
    audio->playMenuSceneMusic();
    // add layer as a child to scene
    scene->addChild(layer);
    //scene->addChild(itemLayer);
    // return the scene
    return scene;
}

bool MenuScene::init()
{
    if (!Node::init()) {
        return false;
    }
    
    int i = 0;
    auto bg = Sprite::create(menuSceneFrameFile);
	auto computerItem = MenuItemImage::create(computerTextureFile, computerTextureFile, [=](Ref *sender) {
		
	});
    auto onlineItem = MenuItemImage::create(onlineTextureFile, onlineTextureFile, [=](Ref *sender) {
        auto network = new NetworkController();
        _network = network;
    });
    auto aboutItem = MenuItemImage::create(aboutTextureFile, aboutTextureFile);
    auto exitItem = MenuItemImage::create(exitTextureFile, exitTextureFile, [](Ref *sender) {
		Director::getInstance()->end();
	});
    
    auto menu = Menu::create(computerItem, onlineItem, aboutItem, exitItem, nullptr);
    auto intoBattleScene = cocos2d::EventListenerCustom::create("intoBattleScene", CC_CALLBACK_1(MenuScene::_intoBattleScene, this));
    
    Size windowSize = Director::getInstance()->getWinSize(); // background image for full screen
    
    bg->setScale(windowSize.width / bg->getContentSize().width, windowSize.height / bg->getContentSize().height);
    bg->setPosition(Vec2(windowSize.width/ 2, windowSize.height / 2));
    
    for(const auto &child : menu->getChildren()) {
        float offset = menuItemDistance * i;
        child->setScale(menuItemScale);
        child->setPosition(Vec2(-windowSize.width / 2 + child->getContentSize().width / 4, menuFirstItemY - offset));
        i++;
    }
    
    _eventDispatcher->addEventListenerWithFixedPriority(intoBattleScene, 1);
    
    this->addChild(menu, 2);
    this->addChild(bg, 1);
    return true;
}

void MenuScene::_intoBattleScene(cocos2d::EventCustom* event)
{
    auto battleScene = HelloWorld::createScene(_network);
    Director::getInstance()->pushScene(battleScene);
}

