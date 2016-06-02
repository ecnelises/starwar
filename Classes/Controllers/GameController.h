//===--- GameController.h - GameController class definition ---===//
/// \file GameController.h
/// This file declares GameController class. GameController includes
/// main facilities when game running, controlling how balls are operated
/// and handling various input from user.

#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

#include "cocos2d.h"
#include "MouseController.h"
#include "NetworkController.h"
#include <memory>

class GameController : public cocos2d::Node {
public:
    GameController();
    ~GameController();
private:
    std::unique_ptr<MouseController> _mouser;
    std::unique_ptr<NetworkController> _networker;
};

#endif // GAME_CONTROLLER_H_
