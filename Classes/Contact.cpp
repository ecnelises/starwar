//
//  Contact.cpp
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#include "Contact.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;


bool Contact::init() {
    if (!Node::init())
    {
        return false;
    }
    
    auto physicsListener = EventListenerPhysicsContact::create();
    physicsListener->onContactBegin = CC_CALLBACK_1(Contact::_onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(physicsListener, this);
    
    return true;
}

bool Contact::_onContactBegin(PhysicsContact &contact) {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("knock.mp3");
    return true;
}