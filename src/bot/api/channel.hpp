//
// Created by X-ray on 8/20/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_CHANNEL_HPP
#define DISCORD_CPP_BOT_CHANNEL_HPP
#include <rapidjson/document.h>
#include "../datatypes.hpp"
#include "api.hpp"

namespace bot {
    namespace api {
        namespace channel {
            datatypes::message::message_t SendMessage(std::string channel_id, std::string content, bool tts = false) {
                rapidjson::Document json;
                json.SetObject();
                rapidjson::Value content_(content.c_str(), content.size(), json.GetAllocator());
                json.AddMember("content", content_, json.GetAllocator());
                json.AddMember("tts", tts, json.GetAllocator());
                auto response = api::ApiPost(fmt::format("channels/{0}/messages", channel_id), api::JsonStringify(json));

                json.RemoveAllMembers();
                json.Parse(response.text.c_str());
                return datatypes::message::message_t(json.GetObject());
            }

            datatypes::message::message_t EditMessage(std::string channel_id, std::string message_id, std::string content, bool tts = false) {
                rapidjson::Document json;
                json.SetObject();
                rapidjson::Value content_(content.c_str(), content.size(), json.GetAllocator());
                json.AddMember("content", content_, json.GetAllocator());
                json.AddMember("tts", tts, json.GetAllocator());
                auto response = api::ApiPatch(fmt::format("channels/{0}/messages/{1}", channel_id, message_id), api::JsonStringify(json));

                json.RemoveAllMembers();
                json.Parse(response.text.c_str());
                return datatypes::message::message_t(json.GetObject());
            }
        }
    }
}
#endif //DISCORD_CPP_BOT_CHANNEL_HPP
