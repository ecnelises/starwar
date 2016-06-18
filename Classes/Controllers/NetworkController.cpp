#include "GameController.h"
#include "NetworkController.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <sstream>
#include <functional>

NetworkController::NetworkController() : _enabled(true)
{
    auto currentTime = std::chrono::system_clock::now();
    std::default_random_engine re;
    std::uniform_int_distribution<int> rdis(619, 414124121);
    _token = std::to_string(std::chrono::system_clock::to_time_t(currentTime)) + std::to_string(rdis(re)); // 时间戳+随机 = token
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
    printf("network close \n");
}

void GameSocketDelegate::onError(cocos2d::network::SIOClient* client,
                                 const std::string& data)
{
    printf("error: %s\n", data.c_str());
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

void NetworkController::sendFixed(int ballId, cocos2d::Vec2 pos)
{
    // 回合结束的时候传出所有球的位置来修正
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
    printf(message.c_str());
}

void NetworkController::dispatchReady(cocos2d::network::SIOClient *client, const std::string &message)
{
    // 收到服务器的ready后，将房间号和首发者保存在network里
    // 首发者用getStarter来获取
    rapidjson::Document d;
    d.Parse(message.c_str());
    cocos2d::EventCustom intoBattleScene("intoBattleScene");
    _room = d["room"].GetString();
    _starter = d["starter"].GetString();
    _eventDispatcher->dispatchEvent(&intoBattleScene);
    printf(message.c_str());
}

void NetworkController::dispatchGameOver(cocos2d::network::SIOClient *client, const std::string &message)
{
    rapidjson::Document d;
    d.Parse(message.c_str());
    cocos2d::EventCustom gameOverEvent("gameOver");
    auto winner = d["winner"].GetInt();
    gameOverEvent.setUserData(&winner);
    _eventDispatcher->dispatchEvent(&gameOverEvent);
    printf(message.c_str());
}

void NetworkController::dispatchRound(cocos2d::network::SIOClient *client)
{
    emitMsg("remoteOverRound");
}

void NetworkController::dispatchEndFixed(cocos2d::network::SIOClient *client)
{
    emitMsg("endFix");
}

void NetworkController::dispatchFixed(cocos2d::network::SIOClient *client, const std::string &message)
{
    auto data = parseRemoteMessage(message);
    cocos2d::EventCustom fixEvent("fix");
    fixEvent.setUserData(&data);
    _eventDispatcher->dispatchEvent(&fixEvent);
    printf(message.c_str());
}

void NetworkController::dispatchConnect(cocos2d::network::SIOClient *client)
{
    // 连接成功就加入房间
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

void NetworkController::dispatchDisconnect(cocos2d::network::SIOClient *client)
{
    // 断开网络
    emitMsg("disconnect");
}

void NetworkController::dispatchWait(cocos2d::network::SIOClient *client)
{
    // 等待其他人进入房间
    printf("waiting...\n");
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

bool NetworkController::hasInstance(void) const
{
    return _enabled;
}
