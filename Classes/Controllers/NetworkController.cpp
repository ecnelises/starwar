//
//  NetworkController.cpp
//  bumpflow
//
//  Created by 邱超凡 on 16/6/2.
//
//

#include "GameController.h"
#include "NetworkController.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <sstream>
#include <functional>

NetworkController::NetworkController(GameController* game) : _game(game), _delegate()
{
    _client = cocos2d::network::SocketIO::connect(_destUri, _delegate);
    _client->on("shoot", CC_CALLBACK_2(NetworkController::dispatchShoot, this));
    _client->on("wait", CC_CALLBACK_1(NetworkController::dispatchWait, this));
    _client->on("ready", CC_CALLBACK_2(NetworkController::dispatchReady, this));
    _client->on("round", CC_CALLBACK_2(NetworkController::dispatchRound, this));
    _client->on("result", CC_CALLBACK_2(NetworkController::dispatchResult, this));
    _client->on("connect", CC_CALLBACK_1(NetworkController::dispatchConnect, this));
}

NetworkController::~NetworkController()
{
    _client->disconnect();
    delete _client;
}

void GameSocketDelegate::onClose(cocos2d::network::SIOClient* client)
{
    printf("network close \n");
}

void GameSocketDelegate::onError(cocos2d::network::SIOClient* client,
                                 const std::string& data)
{
    printf("network error \n");
}

void NetworkController::sendShoot(int gameid, const std::string& player,
                                  int ballid, const Force& force)
{
    std::ostringstream stream;
    stream << R"({"type":"shoot","detail":{)";
    stream << R"("gameid":)" << gameid << ","
    << R"("player":)" << "\"" << player << "\","
    << R"("ball":)"   << ballid << ","
    << R"("force":[)" << force.x << "," << force.y
    << "]}}";
    _client->send(stream.str());
}

void NetworkController::sendSkip(int gameId, const std::string& player)
{
    std::ostringstream stream;
    stream << R"({"gameId":)" << gameId << ","
    << R"("player":)" << "\"" << player << "\""
    << "}}";
    _client->emit("overRound", stream.str());
}

void NetworkController::sendRegisteration(const std::string& playerToken)
{
    std::ostringstream stream;
    stream << R"({"player":)" << R"(")" << playerToken << R"("})";
    _client->emit("register", stream.str());
}

void NetworkController::sendStop(int gameid, const std::string& player)
{
    std::ostringstream stream;
    stream << R"({"type":"stop","detail":{)";
    stream << R"("player":)" <<  "\"" << player << "\","
    << R"("gameid":)" << gameid << "}}";
    _client->send(stream.str());
}

void NetworkController::sendFinish(int gameid, const std::string& winner)
{
    std::ostringstream stream;
    stream << R"({"type":"finish","detail":{)";
    stream << R"("player":)" << "\"" << winner << "\","
    << R"("gameid":)" << gameid << "}}";
    _client->send(stream.str());
}

// The fucking cocos2d-x document has no words on arguments of this
// callback function. Here I just assume the second string argument
// means the message sent from remote server.

void NetworkController::dispatchShoot(cocos2d::network::SIOClient* client,
                                      const std::string& message)
{
    rapidjson::Document d;
    d.Parse(message.c_str());
    //_game->enemyShoot(d["player"].GetString(), d["ball"].GetUint(), cocos2d::Vec2(d["force"][0].GetDouble(), d["force"][1].GetDouble()));
}

void NetworkController::dispatchReady(cocos2d::network::SIOClient* client,
                                               const std::string& message)
{
    rapidjson::Document d;
    //d.Parse(message.c_str());
    printf("%s\n", message.c_str());
    //_game->initializeGame(d["gameid"].GetUint(), d["starter"].GetString());
}

void NetworkController::dispatchRound(cocos2d::network::SIOClient* client,
                                      const std::string& message)
{
    //_game->roundChange();
}
void NetworkController::dispatchConnect(cocos2d::network::SIOClient* client)
{
    cocos2d::EventCustom connectEvent("connect");
    _eventDispatcher->dispatchEvent(&connectEvent);
}
                                                             
                                                             
void NetworkController::dispatchWait(cocos2d::network::SIOClient* client)
{
    printf("waiting...\n");
    cocos2d::EventCustom waitEvent("wait");
    _eventDispatcher->dispatchEvent(&waitEvent);
}

void NetworkController::dispatchResult(cocos2d::network::SIOClient* client,
                                       const std::string& message)
{
    rapidjson::Document d;
    d.Parse(message.c_str());
    //_game->endGame(d["winner"].GetString());
}