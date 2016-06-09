//
//  NetworkController.cpp
//  bumpflow
//
//  Created by 邱超凡 on 16/6/2.
//
//

#include "NetworkController.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <sstream>

void GameSocketDelegate::onClose(cocos2d::network::SIOClient* client)
{
    ;
}

void GameSocketDelegate::onError(cocos2d::network::SIOClient* client,
                                 const std::string& data)
{
    ;
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
    if ( !Node::init() )
    {
        return false;
    }
    
    _client = cocos2d::network::SocketIO::connect(_destUri, _delegate);
    return true;
}