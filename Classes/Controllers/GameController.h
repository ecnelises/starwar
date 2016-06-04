//===--- GameController.h - GameController class definition ---===//
/// \file GameController.h
/// This file declares GameController class. GameController is the main
/// dispatching component in game, as a bridge between showing scene
/// and game inner logic.

#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

#include "cocos2d.h"
#include "MouseController.h"
#include "NetworkController.h"
#include <vector>
#include <memory>

class Player;

class GameController : public cocos2d::Node {
public:
    bool run();
    virtual bool init() override;
    CREATE_FUNC(GameController)
private:
    // TODO: why shared_ptr?
    enum { OVER, PROCESSING } _status;
    std::vector<std::shared_ptr<Player>> _players;
};

#endif // GAME_CONTROLLER_H_
