#include "Contact.h"
#include "Audio.h"

bool Contact::init()
{
    if (!Node::init()) {
        return false;
    }

    auto physicsListener = cocos2d::EventListenerPhysicsContact::create();
    
    _audio = new Audio();
    physicsListener->onContactBegin = CC_CALLBACK_1(Contact::_onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(physicsListener, this);
    
    return true;
}

// Check every contact events to play the knock effect.
bool Contact::_onContactBegin(cocos2d::PhysicsContact &contact)
{
    _audio->playKnockEffect();
    return true;
}