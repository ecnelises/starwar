#include "Item.h"
USING_NS_CC;

bool Item::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto item1 = Sprite::create("item.png");
    
    item1->setPosition(Vec2(item1->getContentSize().width / 2, visibleSize.height - item1->getContentSize().height / 2));
    
    this->addChild(item1);
    return true;
}