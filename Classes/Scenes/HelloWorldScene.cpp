#include "HelloWorldScene.h"
#include "Config.h"
#include "../Controllers/GameController.h"
#include "../Controllers/MouseController.h"

//#include "Item.h"
USING_NS_CC;
Scene* HelloWorld::createScene(NetworkController* network)
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    // scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);  // Debug
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    // 获取netWork，初始化给gameController
    auto gameController = GameController::create();
    gameController->initNetwork(network);
    scene->addChild(gameController, 3);
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    std::vector<Ball*> curlings;
    

//    const int MAXFORCE = 250;
//    const int FORCE = 1200;
    Size windowSize = Director::getInstance()->getWinSize(); // background image for full screen
    auto bg = Sprite::create(battleSceneFrameFile);
    auto map = Sprite::create(battleMapFrameFile);
    auto mapFrame = PhysicsBody::createEdgeBox(map->getContentSize());
    
    
    //mouseController->addBalls(curlings);
    // 设置物理对象
    map->setPhysicsBody(mapFrame);
    // 大小与位置
    //map->setScale(0.65f);
    
    bg->setScale(1.2f);
    
    map->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    bg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    this->addChild(bg, 0);
    this->addChild(map, 1);

    return true;
}

void HelloWorld::addBall(Ball *ball, int zIndex) {
    this->addChild(ball->getSprite(), zIndex);
}
void HelloWorld::update(float dt)
{
    
}

