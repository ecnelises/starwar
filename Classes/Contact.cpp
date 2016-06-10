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
    physicsListener->onContactBegin = CC_CALLBACK_1(Contact::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(physicsListener, this);
    
    return true;
}

bool Contact::onContactBegin(PhysicsContact &contact)
{
    // TODO: Bombs should explode.
    // How to explode?
    // Define a explosion distance. When touching,
    // find everyball in the distance and apply forces to them.
    auto bitA = contact.getShapeA()->getCollisionBitmask();
    auto bitB = contact.getShapeB()->getCollisionBitmask();
    if ((bitA & bitB) == 0x01) {
        _audio->playKnockEffect();
    } else if (bitA == 0x10) {
        // maybe having a explosion audio is better?
        // assuming that here's explosion
        // raising event of explosion
        contact.getShapeA()->getCenter();
    } else if (bitB == 0x10) {
        contact.getShapeB()->getCenter();
    }
    return true;
}