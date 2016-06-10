//
//  Contact.cpp
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#include "Contact.h"
#include "Audio.h"

USING_NS_CC;


bool Contact::init()
{
    if (!Node::init()) {
        return false;
    }
    
    auto physicsListener = EventListenerPhysicsContact::create();
    
    _audio = new Audio();
    physicsListener->onContactBegin = CC_CALLBACK_1(Contact::_onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(physicsListener, this);
    
    return true;
}

bool Contact::_onContactBegin(PhysicsContact &contact)
{
    // TODO: Bombs should explode.
    // How to explode?
    // Define a explosion distance. When touching,
    // find everyball in the distance and apply forces to them.
    _audio->playKnockEffect();
    return true;
}