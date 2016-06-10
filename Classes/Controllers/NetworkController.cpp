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
    stream << R"({"type":"shoot","detail":{)";
    stream << R"("gameid":)" << gameid << ","
           << R"("player":)" << "\"" << player << "\","
           << R"("ball":)"   << ballid << ","
           << R"("force":[)" << force.direction.x << "," << force.direction.y
           << "]}}";
    _client->send(stream.str());
}

void NetworkController::sendSkip(int gameid, const std::string& player)
{
    std::ostringstream stream;
    stream << R"({"type":"skip","detail":{)";
    stream << R"("gameid":)" << gameid << ","
           << R"("player":)" << "\"" << player << "\""
           << "}}";
    _client->send(stream.str());
}

void NetworkController::sendRegisteration(const std::string& player,
                                          unsigned ballNum,
                                          const std::string& nickname)
{
    std::ostringstream stream;
    stream << R"({"type":"registration","detail":{)";
    stream << R"("player":)" << "\"" << player << "\","
           << R"("ballnum":)" << ballNum << ","
           << R"("nickname":)" << "\"" << nickname << "\""
           << "}}";
    _client->send(stream.str());
}

bool NetworkController::init()
{
    return true;
}

// The fucking cocos2d-x document has no words on arguments of this
// callback function. Here I just assume the second string argument
// means the message sent from remote server.
void NetworkController::dispatchRemoteMessage(cocos2d::network::SIOClient *client,
                                              const std::string &message)
{
    rapidjson::Document data;
    data.Parse<0>(message.c_str());
    
//    auto detail = data["detail"];
//    if (data["type"] == "initialization") {
//        _game->initializeGame(detail["gameid"], detail["starter"]);
//    } else if (data["type"] == "result") {
//        _game->endGame(detail["winner"]);
//    } else if (data["type"] == "round") {
//        // to next
//    }
}