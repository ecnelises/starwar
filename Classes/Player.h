//===--- Player.h - Player and its derived classes definition ---===//
/// \file Player.h
/// This file declares Player class and its derived classes, including
/// AIPlayer, RemotePlayer and LocalPlayer. They are used in different
/// kinds of games.
///
/// Main function of Player is sending requests to GameController,
/// handling events from MouseController and manage Balls of the player.

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Ball.h"
#include "BallType.h"
#include "Controllers/MouseController.h"
#include "cocos2d.h"
#include <vector>
#include <memory>
#include <string>

#define emitMsg(msg) { \
    cocos2d::EventCustom eve(msg); \
    eve.setUserData(nullptr); \
    _eventDispatcher->dispatchEvent(&eve); \
}

/// \class PlayerType
/// \brief Type used to distinguish different players.
class PlayerType {
public:
    enum _innertype {
        AIPlayer,
        LocalPlayer,
        RemotePlayer
    };
    
    /// Get if current player is specific kind of player.
    /// \param type Player Type to compare with.
    /// \return If current player is in that kind, it's true.
    bool is(_innertype type) const
    {
        return _type == type;
    }
    
    /// Assign type to current object.
    PlayerType& operator = (_innertype type)
    {
        _type = type;
        return *this;
    }
private:
    /// Inner representation of player type.
    _innertype _type;
};

class MouseController;
class NetworkController;

/// \class Player
/// \brief Base class of all kinds of players.
class Player {
public:
    /// Default set activity to false.
    Player() : _active(false) {}
    virtual ~Player() = default;
    
    /// Create balls.
    /// \param diff Starting coordinate.
    void createBalls(float diff);
    
    /// Set activity of current player.
    virtual void setActive(bool activity) = 0;
    
    /// Start to listen if any ball goes out of bound, if so, erase it.
    /// \note Set this function virtual because it uses schedule, which is part of Node.
    virtual void listenDepart(void) = 0;
    
    /// Stop to listen depart.
    /// \note Set this function virtual because it uses schedule, which is part of Node.
    virtual void unlistenDepart(void) = 0;
    
    /// Apply shoot to a ball.
    /// \param id Id of the ball to shoot.
    /// \param force Force that will be applied to the ball.
    virtual void applyShoot(BallsCollection::BallId id, const Force& force) = 0;
    
    /// Get whether this player doesn't have any ball.
    /// \return If no ball of the player still exist, it's true.
    bool noBall(void) const
    {
        return _balls.empty();
    }
    
    /// Get collection of balls.
    /// \return Const pointer to player's collections of balls.
    const BallsCollection* getBalls() const
    {
        return &_balls;
    }
    
    /// Adjust ball to a position.
    /// \param id Id of ball to adjust.
    /// \param pos Destination position the ball will be adjusted to.
    void fixBall(unsigned id, cocos2d::Vec2 pos)
    {
        _balls.adjustBallPosition(id, pos);
    }
protected:
    /// Balls of the player.
    BallsCollection _balls;
    
    /// Activity of this player.
    bool _active;
    std::string _playerId;
    
    /// Private function to detect if there's any ball goes out of border.
    /// \param dt Just as the scheduler requires, not used.
    void _isDeparted(float dt);
};

/// \class LocalPlayer
/// \brief Player that can be controlled by mouse.
class LocalPlayer : public cocos2d::Node, public Player {
public:
    LocalPlayer(bool isStarter);
    virtual ~LocalPlayer()
    {
        _mouse->release();
    }
    
    /// Add or remove cursors on balls, and set availability of mouse.
    /// \param activity If true, set those enabled; vice versa.
    virtual void setActive(bool activity) override;
    
    /// Just shoot the ball.
    virtual void applyShoot(BallsCollection::BallId id, const Force& force) override;
    virtual void listenDepart(void) override;
    virtual void unlistenDepart(void) override;
private:
    void _isResting(float dt);
    void _applyShoot(cocos2d::EventCustom*);
    MouseController* _mouse;
};

/// \class RemotePlayer
/// \brief Object that simulates actions of enemy at the other side of network.
class RemotePlayer : public cocos2d::Node, public Player {
public:
    RemotePlayer(bool isStarter);
    virtual ~RemotePlayer() = default;
    
    /// Just shoot the ball.
    virtual void applyShoot(BallsCollection::BallId id, const cocos2d::Vec2& force) override;
    virtual void setActive(bool activity) override;
    virtual void listenDepart(void) override;
    virtual void unlistenDepart(void) override;
};

/// \class AIPlayer
/// \brief Player that can automatically do actions and play with localplayer.
class AIPlayer : public cocos2d::Node, public Player {
public:
    AIPlayer(bool isStarter);
    virtual ~AIPlayer() = default;
    
    /// Shoot, send "aiShoot" message and start detecting isRest.
    virtual void applyShoot(BallsCollection::BallId id, const Force& force) override;
    virtual void setActive(bool) override;
    virtual void listenDepart(void) override;
    virtual void unlistenDepart(void) override;
    
    /// Find the target and shoot it with AI algorithm.
    void findAndShoot(const_observer_ptr<BallsCollection> enemyBalls);
private:
    void _isResting(float dt);
};

#endif // PLAYER_H_
