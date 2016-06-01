#include "HelloWorldScene.h"

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
        auto curling = new Curling(MEDIUM);
        curlings.push_back(curling);
        this->addBall(curling, 9);
    }
//    
//    const int MAXFORCE = 250;
//    const int FORCE = 1200;
    auto map = Sprite::create("background.png");
    auto mapFrame = PhysicsBody::createEdgeBox(map->getContentSize());
    auto mouseController = MouseController::create();
    auto contact = Contact::create();
    
    mouseController->addBalls(curlings);
    // 设置物理对象
    map->setPhysicsBody(mapFrame);
    
    // 大小与位置
    map->setScale(1.3f);
    map->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
#if 0
    ball1->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    ball2->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    mouseListener->onMouseDown = [=](Event *event) {
        EventMouse *e = (EventMouse*) event;
        auto ball1Box = ball1->getBoundingBox();
        if(ball1Box.containsPoint(Point(e->getCursorX(), e->getCursorY()))) {
            selected = make_pair(ball1, true);
        }
    };
    
    mouseListener->onMouseMove = [=](Event *event) {
        EventMouse *e = (EventMouse*) event;
        glLineWidth(5); // 规定线段的粗细
        float ballX = ball1->getPositionX(), ballY = ball1->getPositionY();
        float cursorX = e->getCursorX(), cursorY = e->getCursorY();
        if(selected.first == ball1 && selected.second == true) {
            draw->clear(); // 不清除的话，会出现重影现象
            float diff = sqrt(pow((ballX - cursorX), 2.0f) + pow((ballY - cursorY), 2.0f)); // 当前点到球心线段长度
            double ctan = atan(abs(ballY - cursorY) / abs(ballX - cursorX)); // 线段与水平x的夹角
            if(diff > MAXFORCE) { // 判断是不是超出最大长度
                float diffX = MAXFORCE * cos(ctan); // 超出就按照最大长度来重新计算 应当所在的点
                float diffY = MAXFORCE * sin(ctan);
                cursorX = ballX > cursorX ? ballX - diffX : ballX + diffX; // 应当点
                cursorY = ballY > cursorY ? ballY - diffY : ballY + diffY;
            }
            finalPoint = make_pair(cursorX, cursorY); // 记录最后点的位置
            draw->drawSegment(ball1->getPosition(), Vec2(cursorX, cursorY), 2, Color4F(0, 1, 0, 1)); // 画线段
        }
    };
    
    mouseListener->onMouseUp = [=](Event *event) {
        if(selected.first == ball1 && selected.second) {
            selected = make_pair(ball1, false); // 改变按下点的状态
            draw->clear();
            float x = ball1->getPositionX() - finalPoint.first;
            float y = ball1->getPositionY() - finalPoint.second;
            x = x < ball1->getPositionX() ? x : -x;
            y = y < ball1->getPositionY() ? y : -y;
            ball1Body->applyImpulse(Vec2(x, y) * FORCE);
            //ball1Body->setVelocity(Vec2(x, y));
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, ball1);
#endif

    this->addChild(mouseController, 10);
    this->addChild(map, 1);
    this->addChild(contact, 0);
//    this->scheduleUpdate();
    return true;
}


void HelloWorld::addBall(Ball *ball, int zIndex) {
    this->addChild(ball->getSprite(), zIndex);
}


void HelloWorld::update(float dt)
{
    
}
