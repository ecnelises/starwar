//===--- Contact.h - Contact class definition ---===//
/// \file Contact.h
/// Contact is responsible for detecting collisions and play audio.

#ifndef CONTACT_H_
#define CONTACT_H_

#include "cocos2d.h"
#include "Audio.h"

/// \class Contact
/// \brief Contact is delegate class for collisions in.
class Contact : public cocos2d::Node {
public:
    virtual ~Contact() {};
    virtual bool init() override;
    CREATE_FUNC(Contact);
private:
    /// Callback function when contact begins.
    /// \param The contact object.
    /// \return Whether the collision succeed.
    bool _onContactBegin(cocos2d::PhysicsContact& contact);
    Audio* _audio;
};

#endif // CONTACT_H_
