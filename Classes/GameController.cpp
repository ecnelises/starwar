//
//  GameController.cpp
//  ball
//
//  Created by Dcalsky on 16/6/2.
//
//

#include "GameController.h"
#include "Contact.h"

bool GameController::init()
{
    if (!Node::init()) {
        return false;
    }
    
    _remotePlayer = new RemotePlayer();
    _localPlayer = new LocalPlayer();
    
    auto mouseController = MouseController::create();
    auto contact = Contact::create();
    auto localBalls = _localPlayer->getBalls();
    
    this->addChild(contact);
    this->addChild(mouseController, 10);
    for (int i = 0; i < localBalls.size(); ++i) {
        this->addChild(localBalls[i]->getSprite(), 3);
    }
    
    return true;
}


