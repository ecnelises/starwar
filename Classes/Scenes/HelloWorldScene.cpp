#include "HelloWorldScene.h"
#include "Config.h"
#include "../Controllers/GameController.h"
//#include "Item.h"
USING_NS_CC;
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);  // Debug
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    //auto itemLayer = Item::create();
    // add layer as a child to scene
    scene->addChild(layer);
    //scene->addChild(itemLayer);
    // return the scene
    return scene;
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    std::vector<Ball*> curlings;
    
    for( int i = 0; i < 9; ++i ) {
        auto curling = new Curling(Curling::Medium);
        curlings.push_back(curling);
        this->addBall(curling, 9);
    }
//    
//    const int MAXFORCE = 250;
//    const int FORCE = 1200;
    auto map = Sprite::create(battleSceneFrameFile);
    auto mapFrame = PhysicsBody::createEdgeBox(map->getContentSize());
    auto gameController = GameController::create();
    auto contact = Contact::create();
    
    //mouseController->addBalls(curlings);
    // 设置物理对象
    map->setPhysicsBody(mapFrame);
    // 大小与位置
    map->setScale(1.3f);
    map->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    this->addChild(gameController, 3);
    this->addChild(map, 1);

    return true;
}

void HelloWorld::addBall(Ball *ball, int zIndex) {
    this->addChild(ball->getSprite(), zIndex);
}
void HelloWorld::update(float dt)
{
    
}

