//===--- NetworkController.cpp - NetworkController class implementation ---===//
/// \file NetworkController.h
/// This file declares both the delegate class and controller
/// class used in network communication of this game.

#include "GameController.h"
#include "NetworkController.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <sstream>
#include <functional>

NetworkController::NetworkController() : _enabled(true)
{
    auto currentTime = std::chrono::system_clock::now();
    // Generate random token to authenticate player identity
    std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> rdis(19260817, 20220817);
    _token = std::to_string(std::chrono::system_clock::to_time_t(currentTime)) + std::to_string(rdis(re));
    _client = cocos2d::network::SocketIO::connect(_destUri, _delegate);
    _client->on("shoot", CC_CALLBACK_2(NetworkController::dispatchShoot, this));
    _client->on("fix", CC_CALLBACK_2(NetworkController::dispatchFixed, this));
    _client->on("endFix", CC_CALLBACK_1(NetworkController::dispatchEndFixed, this));
    _client->on("wait", CC_CALLBACK_1(NetworkController::dispatchWait, this));
    _client->on("gameOver", CC_CALLBACK_2(NetworkController::dispatchGameOver, this));
    _client->on("ready", CC_CALLBACK_2(NetworkController::dispatchReady, this));
    _client->on("overRound", CC_CALLBACK_1(NetworkController::dispatchRound, this));
    _client->on("connect", CC_CALLBACK_1(NetworkController::dispatchConnect, this));
    _client->on("disconnect", CC_CALLBACK_1(NetworkController::dispatchDisconnect, this));
}

NetworkController::~NetworkController()
{
    _client->disconnect();
}

void GameSocketDelegate::onClose(cocos2d::network::SIOClient* client)
{
#ifndef NDEBUG
    printf("network close \n");
#endif
}

void GameSocketDelegate::onError(cocos2d::network::SIOClient* client,
                                 const std::string& data)
{
#ifndef NDEBUG
    printf("error: %s\n", data.c_str());
#endif
}

namespace {
    bool stringReplace(std::string& target, const std::string& from,
                       const std::string& to)
    {
        auto start_pos = target.find(from);
        if(start_pos == std::string::npos) {
            return false;
        }
        target.replace(start_pos, from.length(), to);
        return true;
    }
    
    bool stringReplace(std::string& target, const std::string& from,
                       double to)
    {
        return stringReplace(target, from, std::to_string(to));
    }
    
    bool stringReplace(std::string& target, const std::string& from,
                       int to)
    {
        return stringReplace(target, from, std::to_string(to));
    }
} // anonymous namespace

void NetworkController::sendShoot(int ballId, const Force &force)
{
    if (!this->hasInstance()) {
        return;
    }
    std::ostringstream stream;
    stream << R"(
    {
        "ballId": @b,
        "force": [
            @forcex,
            @forcey
        ],
        "room": "@room"
    }
    )";
    auto dest = stream.str();
    stringReplace(dest, "@b", ballId);
    stringReplace(dest, "@forcex", force.x);
    stringReplace(dest, "@forcey", force.y);
    stringReplace(dest, "@room", _room);
    _client->emit("shoot", dest);
}

void NetworkController::sendOverRound()
{
    if (!this->hasInstance()) {
        return;
    }
    std::ostringstream stream;
    stream << R"(
    {
        "room": "@room"
    }
    )";
    auto dest = stream.str();
    stringReplace(dest, "@room", _room);
    _client->emit("overRound", dest);
}

void NetworkController::sendEndFixed()
{
    if (!this->hasInstance()) {
        return;
    }
    std::ostringstream stream;
    stream << R"(
    {
        "room": "@room"
    }
    )";
    auto dest = stream.str();
    stringReplace(dest, "@room", _room);
    _client->emit("endFix", dest);
}

// In the beginning, "As long as we send the force information correctly every time,
// no deviation will present", we thought. But cocos2d-x broke our expectation. We
// have to send positions of every ball before overround to maintain consistency.
void NetworkController::sendFixed(int ballId, cocos2d::Vec2 pos)
{
    if (!this->hasInstance()) {
        return;
    }
    std::ostringstream stream;
    stream << R"(
    {
        "ballId": @b,
        "position": [
            @posx,
            @posy
        ],
        "room": "@room"
    }
    )";
    auto dest = stream.str();
    stringReplace(dest, "@b", ballId);
    stringReplace(dest, "@posx", pos.x);
    stringReplace(dest, "@posy", pos.y);
    stringReplace(dest, "@room", _room);
    _client->emit("fix", dest);
}

void NetworkController::sendDisconnect()
{
    if (!this->hasInstance()) {
        return;
    }
    _client->disconnect();
    this->unscheduleAllCallbacks();
}

void NetworkController::sendGameOver(int status)
{
    if (!this->hasInstance()) {
        return;
    }
    std::ostringstream stream;
    stream << R"(
    {
        "winner": @status,
        "room": "@room"
    }
    )";
    auto dest = stream.str();
    stringReplace(dest, "@status", status);
    stringReplace(dest, "@room", _room);
    _client->emit("gameOver", dest);
}

// The f**king cocos2d-x document has no words on arguments of this
// callback function. Here I just assume the second string argument
// means the message sent from remote server.
void NetworkController::dispatchShoot(cocos2d::network::SIOClient *client, const std::string &message)
{
    auto data = parseRemoteMessage(message);
    cocos2d::EventCustom remoteShootEvent("remoteShoot");
    remoteShootEvent.setUserData(&data);
    _eventDispatcher->dispatchEvent(&remoteShootEvent);
}

// When receiving 'ready' message from server, save room id and start in current object.
void NetworkController::dispatchReady(cocos2d::network::SIOClient *client, const std::string &message)
{
    rapidjson::Document d;
    d.Parse(message.c_str());
    cocos2d::EventCustom intoBattleScene("intoBattleScene");
    _room = d["room"].GetString();
    _starter = d["starter"].GetString();
    _eventDispatcher->dispatchEvent(&intoBattleScene);
}

void NetworkController::dispatchGameOver(cocos2d::network::SIOClient *client, const std::string &message)
{
    rapidjson::Document d;
    d.Parse(message.c_str());
    cocos2d::EventCustom gameOverEvent("gameOver");
    int winner;
    switch (d["winner"].GetInt()) {
    case GameController::Win:
        winner = GameController::Lose;
        break;
    case GameController::Lose:
        winner = GameController::Win;
    case GameController::Draw:
        winner = GameController::Draw;
    default:
        break;
    }
    gameOverEvent.setUserData(&winner);
    _eventDispatcher->dispatchEvent(&gameOverEvent);
}


void NetworkController::dispatchRound(cocos2d::network::SIOClient *client)
{
    emitMsg("remoteOverRound");
}

void NetworkController::dispatchEndFixed(cocos2d::network::SIOClient *client)
{
    emitMsg("endFix");
}

// Receive adjusting ball message from server.
void NetworkController::dispatchFixed(cocos2d::network::SIOClient *client, const std::string &message)
{
    auto data = parseRemoteMessage(message);
    cocos2d::EventCustom fixEvent("fix");
    fixEvent.setUserData(&data);
    _eventDispatcher->dispatchEvent(&fixEvent);
}

// When connect succeeds, send player id to server.
void NetworkController::dispatchConnect(cocos2d::network::SIOClient *client)
{
    std::ostringstream stream;
    stream << R"(
    {
        "player": "@token"
    }
    )";
    auto dest = stream.str();
    stringReplace(dest, "@token", _token);
    _client->emit("register", dest);
}

// Tell opponent you are disconnect.
void NetworkController::dispatchDisconnect(cocos2d::network::SIOClient *client)
{
    emitMsg("disconnect");
}

// Waiting for another player come in.
void NetworkController::dispatchWait(cocos2d::network::SIOClient *client)
{
    emitMsg("wait");
}

UnifiedMessageBody NetworkController::parseRemoteMessage(const std::string &msg)
{
    rapidjson::Document d;
    d.Parse(msg.c_str());
    UnifiedMessageBody res;
    res.targetId = d["ballId"].GetInt();
    if (d.HasMember("force")) {
        res.vec = cocos2d::Vec2(d["force"][0].GetDouble(), d["force"][1].GetDouble());
    } else if (d.HasMember("position")) {
        res.vec = cocos2d::Vec2(d["position"][0].GetDouble(), d["position"][1].GetDouble());
    }
    return res;
}

// At first, the way I do in this function is `if (this == nullptr)`
// This really works, but later I know it's undefined behavior, sigh...
bool NetworkController::hasInstance(void) const
{
    return _enabled;
}
