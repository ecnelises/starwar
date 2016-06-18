#include "GameController.h"
#include "NetworkController.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <sstream>
#include <functional>

NetworkController::NetworkController()
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

void GameSocketDelegate::onClose(cocos2d::network::SIOClient *client)
{
    printf("network close \n");
}

void GameSocketDelegate::onError(cocos2d::network::SIOClient *client,
                                 const std::string& data)
{
    printf("error: %s\n", data.c_str());
}

void NetworkController::sendShoot(int ballId, const Force &force)
{
    std::ostringstream stream;
    stream << R"({"ballId":)" << ballId << ","
    << R"("force":[)" << force.x << "," << force.y << "],"
    << R"("room":)" << "\"" <<  _room << "\"}";
    _client->emit("shoot", stream.str());
}

void NetworkController::sendOverRound()
{
    std::ostringstream stream;
    stream << R"({"room":)" << "\"" << _room << "\"}";
    _client->emit("overRound", stream.str());
}

void NetworkController::sendEndFixed()
{
    std::ostringstream stream;
    stream << R"({"room":)" << "\"" << _room << "\"}";
    _client->emit("endFix", stream.str());
}

void NetworkController::sendFixed(int ballId, cocos2d::Vec2 pos)
{
    // 回合结束的时候传出所有球的位置来修正
    std::ostringstream stream;
    stream << R"({"ballId":)" << ballId << ","
    << R"("position":[)" << pos.x << "," << pos.y << "],"
    << R"("room":)" << "\"" <<  _room << "\"}";
    _client->emit("fix", stream.str());
}

void NetworkController::sendDisconnect()
{
    _client->disconnect();
    this->unscheduleAllCallbacks();
}

void NetworkController::sendGameOver(int status)
{
    std::ostringstream stream;
    stream << R"({"winner":)" << status << ","
    << R"("room":)" << "\"" <<  _room << "\"}";
    _client->emit("gameOver", stream.str());
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
}

void NetworkController::dispatchGameOver(cocos2d::network::SIOClient *client, const std::string &message)
{
    rapidjson::Document d;
    d.Parse(message.c_str());
    cocos2d::EventCustom gameOverEvent("gameOver");
    auto winner = d["winner"].GetInt();
    gameOverEvent.setUserData(&winner);
    _eventDispatcher->dispatchEvent(&gameOverEvent);
}

void NetworkController::dispatchRound(cocos2d::network::SIOClient *client)
{
    cocos2d::EventCustom overRoundEvent("remoteOverRound");
    _eventDispatcher->dispatchEvent(&overRoundEvent);
}

void NetworkController::dispatchEndFixed(cocos2d::network::SIOClient *client)
{
    cocos2d::EventCustom endFixEvent("endFix");
    _eventDispatcher->dispatchEvent(&endFixEvent);
}

void NetworkController::dispatchFixed(cocos2d::network::SIOClient *client, const std::string &message)
{
    auto data = parseRemoteMessage(message);
    cocos2d::EventCustom fixEvent("fix");
    fixEvent.setUserData(&data);
    _eventDispatcher->dispatchEvent(&fixEvent);
}

void NetworkController::dispatchConnect(cocos2d::network::SIOClient *client)
{
    // 连接成功就加入房间
    std::ostringstream stream;
    stream << R"({"player":)" << R"(")" << _token << R"("})";
    _client->emit("register", stream.str());
}

void NetworkController::dispatchDisconnect(cocos2d::network::SIOClient *client)
{
    // 断开网络
    cocos2d::EventCustom disconnectEvent("disconnect");
    _eventDispatcher->dispatchEvent(&disconnectEvent);
}

void NetworkController::dispatchWait(cocos2d::network::SIOClient *client)
{
    // 等待其他人进入房间
    printf("waiting...\n");
    cocos2d::EventCustom waitEvent("wait");
    _eventDispatcher->dispatchEvent(&waitEvent);
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
