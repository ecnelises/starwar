#include "MenuScene.h"
#include "HelloWorldScene.h"
#include "Config.h"
#include "Audio.h"


USING_NS_CC;

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
	auto computerItem = MenuItemImage::create(computerTextureFile, computerTextureFile, [=](Ref *sender) {
        if(!_waiting) {
            audio->playItemClickEffect();
        }
	});

    auto onlineItem = MenuItemImage::create(onlineTextureFile, onlineTextureFile, [=](Ref *sender) {
        if(!_waiting) {
            auto network = new NetworkController();
            _network = network;
            auto connectLayer = Layer::create();
            audio->playItemClickEffect();
            auto connectBg = Sprite::create(connectingTextureFile);
            connectBg->setPosition(Vec2(visibleSize.width/ 2, visibleSize.height / 2));
            connectLayer->addChild(connectBg, 2);
            _waiting = true;
            this->addChild(connectLayer, 3);
        }
    });
    auto aboutItem = MenuItemImage::create(aboutTextureFile, aboutTextureFile, [=](Ref *sender) {
        audio->playItemClickEffect();
    });
    auto exitItem = MenuItemImage::create(exitTextureFile, exitTextureFile, [](Ref *sender) {
		Director::getInstance()->end();
	});
    auto menu = Menu::create(computerItem, onlineItem, aboutItem, exitItem, nullptr);
    auto intoBattleScene = cocos2d::EventListenerCustom::create("intoBattleScene", CC_CALLBACK_1(MenuScene::_intoBattleScene, this));
    
    _bg = Sprite::create(menuSceneFrameFile);
    _bg->setScale(visibleSize.width / _bg->getContentSize().width, visibleSize.height / _bg->getContentSize().height);
    _bg->setPosition(Vec2(visibleSize.width/ 2, visibleSize.height / 2));
    for(const auto &child : menu->getChildren()) {
        float offset = menuItemDistance * i;
        child->setScale(menuItemScale);
        child->setPosition(Vec2(-visibleSize.width / 2 + child->getContentSize().width / 4, menuFirstItemY - offset));
        i++;
    }
    
    this->addChild(menu, 2);
    this->addChild(_bg, 1);
    this->schedule(schedule_selector(MenuScene::_inZoom), 1/25);
    _eventDispatcher->addEventListenerWithFixedPriority(intoBattleScene, 1);
    return true;
}

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

void MenuScene::_intoBattleScene(cocos2d::EventCustom* event)
{
    _eventDispatcher->removeAllEventListeners();
    auto battleScene = HelloWorld::createScene(_network);
    Director::getInstance()->replaceScene(battleScene);
}

