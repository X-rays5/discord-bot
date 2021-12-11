//
// Created by X-ray on 8/18/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_EVENT_HANDLER_HPP
#define DISCORD_CPP_BOT_EVENT_HANDLER_HPP
#include <functional>
#include <map>
#include <iostream>
#include <rapidjson/document.h>

namespace bot {
    namespace events {
        class handler {
        public:
            void HandleEvent(const std::string& event, std::string payload);

            // https://discord.com/developers/docs/topics/gateway#commands-and-events-gateway-events
            using event_handler_t = std::function<void(std::string)>;
            void AddEventHandler(std::string eventname, event_handler_t handler);
        private:
            std::unordered_multimap<std::string, event_handler_t> events_;
        };
    }
}
#endif //DISCORD_CPP_BOT_EVENT_HANDLER_HPP
