//===--- MenuScene.cpp - MenuScene class implementation ---===//
/// \file MenuScene.h
/// This file implements interfaces provided by MenuScene class.

#include "MenuScene.h"
#include "BattleScene.h"
#include "Config.h"
#include "Audio.h"
#include <tuple>

using cocos2d::Sprite;
using cocos2d::Director;
using cocos2d::Vec2;
using cocos2d::Scene;
using cocos2d::MenuItemImage;
using cocos2d::Menu;

Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init()
{
    if (!Node::init()) {
        return false;
    }
    
    int i = 0;
    auto audio = new Audio();
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    // *Create menu begin*
	auto computerItem = MenuItemImage::create(Config::File::computerTexture, Config::File::computerTexture, [=](Ref *sender) {
        this->_intoAIScene();
	});

    auto onlineItem = MenuItemImage::create(Config::File::onlineTexture, Config::File::onlineTexture, [=](Ref *sender) {
        if(!_waiting) {
            auto network = new NetworkController();
            _network = network;
            auto connectLayer = Layer::create();
            audio->playItemClickEffect();
            auto connectBg = Sprite::create(Config::File::connectingTexture);
            connectBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            connectLayer->addChild(connectBg, 2);
            _waiting = true;
            this->addChild(connectLayer, 3);
        }
    });
    auto aboutItem = MenuItemImage::create(Config::File::aboutTexture, Config::File::aboutTexture, [=](Ref *sender) {
        audio->playItemClickEffect();
    });
    auto exitItem = MenuItemImage::create(Config::File::exitTexture, Config::File::exitTexture, [](Ref *sender) {
		Director::getInstance()->end();
	});
    auto menu = Menu::create(computerItem, onlineItem, aboutItem, exitItem, nullptr);
    // *Create menu end*
    
    auto intoBattleScene = cocos2d::EventListenerCustom::create("intoBattleScene", CC_CALLBACK_1(MenuScene::_intoBattleScene, this));
    
    _bg = Sprite::create(Config::File::menuSceneFrame);
    _bg->setScale(visibleSize.width / _bg->getContentSize().width, visibleSize.height / _bg->getContentSize().height);
    _bg->setPosition(Vec2(visibleSize.width/ 2, visibleSize.height / 2));
    for(const auto &child : menu->getChildren()) {
        float offset = Config::Data::menuItemDistance * i;
        child->setScale(Config::Data::menuItemScale);
        child->setPosition(Vec2(-visibleSize.width / 2 + child->getContentSize().width / 4,
                                Config::Data::menuTopY - offset));
        i++;
    }
    
    this->addChild(menu, 2);
    this->addChild(_bg, 1);
    // add a scheduler to change background fade and zoom. :60fps
    this->schedule(schedule_selector(MenuScene::_inZoom), 1 / 25);
    _eventDispatcher->addEventListenerWithFixedPriority(intoBattleScene, 1);
    return true;
}

// background image fade in and zoom in
void MenuScene::_inZoom(float dt)
{
    _scale += 0.0005;
    if(_scale >= 0.51) {
        this->unschedule(schedule_selector(MenuScene::_inZoom));
        this->schedule(schedule_selector(MenuScene::_outZoom));
    }
    _bg->setOpacity(_scale * 500);
    _bg->setScale(_bg->getScaleX() + 0.0005, _bg->getScaleY() + 0.0005);
}

// background image fade out and zoom out
void MenuScene::_outZoom(float dt)
{
    _scale -= 0.0005;
    if(_scale <= 0.22) {
        this->unschedule(schedule_selector(MenuScene::_outZoom));
        this->schedule(schedule_selector(MenuScene::_inZoom));
    }
    _bg->setOpacity(_scale * 500);
    _bg->setScale(_bg->getScaleX() - 0.0005, _bg->getScaleY() - 0.0005);
}

// Receive network's network, enter the battle scene
void MenuScene::_intoBattleScene(cocos2d::EventCustom* event)
{
    _eventDispatcher->removeAllEventListeners();
    auto battleScene = BattleScene::createScene(std::make_tuple(true, _network));
    Director::getInstance()->replaceScene(battleScene);
}

// Click computer button, enter the AI scene
void MenuScene::_intoAIScene(void)
{
    _eventDispatcher->removeAllEventListeners();
    auto aiScene = BattleScene::createScene(std::make_tuple(false, nullptr));
    Director::getInstance()->replaceScene(aiScene);
}
