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

NetworkController::NetworkController(cocos2d::EventDispatcher* dispatcher) : _delegate(dispatcher), _gameDispatcher(dispatcher)
{
    _client = cocos2d::network::SocketIO::connect(_destUri, _delegate);
    _client->on("shoot", [=](auto client, const std::string& message){
        cocos2d::EventCustom shootEvent("EnemyShoot");
        //shootEvent.setUserData(message.c_str());
        dispatcher->dispatchEvent(&shootEvent);
    });
    _client->on("initialization", [=](auto client, const std::string& message){
        cocos2d::EventCustom initEvent("Initialization");
        //
        dispatcher->dispatchEvent(&initEvent);
    });
    _client->on("round", [=](auto client, const std::string& message){
        cocos2d::EventCustom roundEvent("RoundChange");
        //
        dispatcher->dispatchEvent(&roundEvent);
    });
}

NetworkController::~NetworkController()
{
    _client->disconnect();
    delete _client;
}

void GameSocketDelegate::onClose(cocos2d::network::SIOClient* client)
{
    cocos2d::EventCustom closeEvent("NetworkClose");
    closeEvent.setUserData(nullptr); // nothing to send
    _game->getEventDispatcher()->dispatchEvent(&closeEvent);
}

void GameSocketDelegate::onError(cocos2d::network::SIOClient* client,
                                 const std::string& data)
{
    cocos2d::EventCustom errorEvent("NetworkError");
    errorEvent.setUserData(nullptr); // also, nothing to send
    _game->getEventDispatcher()->dispatchEvent(&errorEvent);
}

void NetworkController::sendShoot(int gameid, const std::string& player,
                                  int ballid, const Force& force)
{
    std::ostringstream stream;
    stream << R"({)" << R"("gameid":)" << gameid << ","
           << R"("player":)" << "\"" << player << "\","
           << R"("ball":)"   << ballid << ","
           << R"("force":[)" << force.direction.x << "," << force.direction.y
           << "]}";
    _client->emit("shoot", stream.str());
}

void NetworkController::sendSkip(int gameid, const std::string& player)
{
    std::ostringstream stream;
    stream << R"({)" << R"("gameid":)" << gameid << ","
           << R"("player":)" << "\"" << player << "\"" << "}";
    _client->emit("skip", stream.str());
}

void NetworkController::sendRegisteration(const std::string& player,
                                          unsigned ballNum,
                                          const std::string& nickname)
{
    std::ostringstream stream;
    stream << R"({)" << R"("player":)" << "\"" << player << "\","
           << R"("ballnum":)" << ballNum << ","
           << R"("nickname":)" << "\"" << nickname << "\"" << "}";
    _client->emit("registration", stream.str());
}

void NetworkController::sendStop(int gameid, const std::string& player)
{
    std::ostringstream stream;
    stream << R"({)" << R"("player":)" <<  "\"" << player << "\","
           << R"("gameid":)" << gameid << "}";
    _client->emit("stop", stream.str());
}

void NetworkController::sendFinish(int gameid, const std::string& winner)
{
    std::ostringstream stream;
    stream << R"({)" << R"("player":)" << "\"" << winner << "\","
           << R"("gameid":)" << gameid << "}";
    _client->emit("finish", stream.str());
}

// The fucking cocos2d-x document has no words on arguments of this
// callback function. Here I just assume the second string argument
// means the message sent from remote server.

void NetworkController::dispatchShoot(cocos2d::network::SIOClient* client,
                   const std::string& message)
{
    rapidjson::Document d;
    d.Parse(message.c_str());
    _game->enemyShoot(d["player"].GetString(), d["ball"].GetUint(), cocos2d::Vec2(d["force"][0].GetDouble(), d["force"][1].GetDouble()));
}

void NetworkController::dispatchInitialization(cocos2d::network::SIOClient* client,
                            const std::string& message)
{
    rapidjson::Document d;
    d.Parse(message.c_str());
    _game->initializeGame(d["gameid"].GetUint(), d["starter"].GetString());
}

void NetworkController::dispatchRound(cocos2d::network::SIOClient* client,
                   const std::string& message)
{
    _game->roundChange();
}

void NetworkController::dispatchResult(cocos2d::network::SIOClient* client,
                    const std::string& message)
{
    rapidjson::Document d;
    d.Parse(message.c_str());
    _game->endGame(d["winner"].GetString());
}