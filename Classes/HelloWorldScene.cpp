#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    // add layer as a child to scene
    scene->addChild(layer);
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
    maxDistance = make_pair(0.0f, 0.0f);

    auto map = Sprite::create("background.png");
    auto ball1 = Sprite::create("ball.png");
    auto ball2 = Sprite::create("ball.png");
    auto ball1Body = PhysicsBody::createCircle(ball1->getContentSize().width / 2);
    auto ball2Body = PhysicsBody::createCircle(ball1->getContentSize().width / 2);
    auto mapFrame = PhysicsBody::createEdgeBox(map->getContentSize());
    auto mouseListener = EventListenerMouse::create();
    auto draw = DrawNode::create();
    
    ball1Body->setGravityEnable(false); // 不受重力影响
    ball2Body->setGravityEnable(false);
    ball1Body->setLinearDamping(5.0f); // 线性阻尼
    ball2Body->setLinearDamping(5.0f);
    ball1Body->setRotationEnable(false); // 不转动
    ball2Body->setRotationEnable(false);
    
    map->setPhysicsBody(mapFrame);
    ball1->setPhysicsBody(ball1Body);
    ball2->setPhysicsBody(ball2Body);
    
    map->setScale(1.0f);
    map->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
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
        glLineWidth(5);
        if(selected.first == ball1 && selected.second == true) {
            draw->clear();
            float diff = sqrt(pow((ball1->getPositionX() - e->getCursorX()), 2.0f) + pow((ball1->getPositionY() - e->getCursorY()), 2.0f));
            float x = e->getCursorX();
            float y = e->getCursorY();
            double ctan = atan(abs(ball1->getPositionY() - e->getCursorY()) / abs(ball1->getPositionX() - e->getCursorX()));
            if(diff > 180) {
                float diffX = 180 * cos(ctan);
                float diffY = 180 * sin(ctan);
                if(ball1->getPositionX() > e->getCursorX()) {
                    x = ball1->getPositionX() - diffX;
                } else {
                    x = ball1->getPositionX() + diffX;
                }
                if(ball1->getPositionY() > e->getCursorY()) {
                    y = ball1->getPositionY() - diffY;
                } else {
                    y = ball1->getPositionY() + diffY;
                }
            }
            maxDistance = make_pair(x, y);
            draw->drawSegment(ball1->getPosition(), Vec2(x, y), 2, Color4F(0, 1, 0, 1));
        }
    };
    
    mouseListener->onMouseUp = [=](Event *event) {
        if(selected.first == ball1 && selected.second) {
            selected = make_pair(ball1, false);
            draw->clear();
            float x = ball1->getPositionX() - maxDistance.first;
            float y = ball1->getPositionY() - maxDistance.second;
            x = x < ball1->getPositionX() ? x : -x;
            y = y < ball1->getPositionY() ? y : -y;
            ball1Body->applyImpulse(Vec2(x*1000, y*1000));
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, ball1);

    this->addChild(map, 0);
    this->addChild(ball1, 1);
    this->addChild(ball2, 2);
    this->addChild(draw, 10);
    
    return true;
}
