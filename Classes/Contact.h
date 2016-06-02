//
//  Contact.hpp
//  ball
//
//  Created by Dcalsky on 16/6/1.
//
//

#ifndef CONTACT_H_
#define CONTACT_H_

#include "cocos2d.h"
#include "Audio.h"

class Contact : public cocos2d::Node {
public:
    virtual ~Contact() {};
    virtual bool init() override;
    CREATE_FUNC(Contact);
private:
    bool _onContactBegin(cocos2d::PhysicsContact& contact);
    Audio *_audio;
};


#endif // CONTACT_H_
