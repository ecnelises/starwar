//===--- NetworkController.h - NetworkController class definition ---===//
/// \file NetworkController.h
/// This file declares both the delegate class and controller
/// class used in network communication of this game.

#ifndef NETWORK_CONTROLLER_H_
#define NETWORK_CONTROLLER_H_

#include "cocos2d.h"
#include "SocketIO.h"
#include "Ball.h"
#include <string>

/// \struct UnifiedMessageBody
/// \brief Class for transmitting information in network.
struct UnifiedMessageBody {
    unsigned targetId;
    cocos2d::Vec2 vec;
};

class GameController;
using Socket = cocos2d::network::SIOClient;

/// \class GameSocketDelegate
/// \brief A helper class mainly used to handle error and close event.
class GameSocketDelegate : public cocos2d::network::SocketIO::SIODelegate {
public:
    GameSocketDelegate() = default;
    virtual ~GameSocketDelegate() = default;

    /// Callback when the client disconnect.
    /// \param socket Current client.
    virtual void onClose(Socket* socket) override;
    /// Callback when the client receive error signal or fail to connect.
    /// \param socket Current client.
    /// \param msg Error message.
    virtual void onError(Socket* socket, const std::string& msg) override;
};

/// \class NetworkController
/// \brief Part of gamecontroller, just receiving and sending messages to remote server.
class NetworkController : public cocos2d::Node {
public:
    /// Default constructor, generate token and set callbacks.
    NetworkController();
    
    // We can just overload constructors, cannot rename it.
    // So we have to add a parameter to it, just a sign.
    /// Constructor when NOT USING NETWORK.
    /// \param hasNetwork Using network or not, must be false.
    NetworkController(bool hasNetwork) : _enabled(false)
    {
        assert(!hasNetwork);
    }
    ~NetworkController();
    
    /// Get starter id of game.
    /// \return Game starter id.
    const std::string& getStarter() const
    {
        return _starter;
    }
    
    /// Get id of current player.
    /// \return Id of current player.
    const std::string& getToken() const
    {
        return _token;
    }
    
    /// Send shooting message to server.
    /// \param ballid Which ball is shooted.
    /// \param force Force of shoot.
    void sendShoot(int ballid, const Force& force);
    
    /// Send turning round message to server.
    void sendOverRound(void);
    
    /// Send message that ball-adjusting is end to server.
    void sendEndFixed(void);
    
    /// Disconnect to server.
    void sendDisconnect(void);
    
    /// Send gameover message to server.
    /// \param Current game status, includeing Win, Lose and Draw.
    void sendGameOver(int winner);
    
    /// Send adjusting-ball message to server.
    /// \param ballId Id of ball to adjust, added a mask to show owner of ball.
    /// \param vec Destination position of that ball.
    void sendFixed(int ballId, cocos2d::Vec2 vec);
private:
    /// Server IP address and port.
    static constexpr auto _destUri = "115.159.189.232:6619";
    
    /// Whether network controller is enabled.
    bool _enabled;
    
    /// Game controller associated with this network controller.
    observer_ptr<GameController> _game;
    
    /// Socket connection client.
    Socket* _client;
    
    /// Delegate object for tackling with error and close events.
    GameSocketDelegate _delegate;
    
    /// Function to tackle with remote messages encoded in JSON.
    void dispatchShoot(Socket*, const std::string&);
    void dispatchFixed(Socket*, const std::string&);
    void dispatchReady(Socket*, const std::string&);
    void dispatchGameOver(Socket*, const std::string&);
    void dispatchRound(Socket*);
    void dispatchEndFixed(Socket*);
    void dispatchWait(Socket*);
    void dispatchConnect(Socket*);
    void dispatchDisconnect(Socket*);
    
    /// Parse remote message to UnifiedMessageBody.
    UnifiedMessageBody parseRemoteMessage(const std::string& msg);
    
    /// Whether network is enabled.
    /// \return Network availability.
    bool hasInstance(void) const;
    
    /// Room id of current network game.
    std::string _room;
    
    /// Game starter id.
    std::string _starter;
    
    /// token, authenticate player identity
    std::string _token;
};

#endif // NETWORK_CONTROLLER_H_
