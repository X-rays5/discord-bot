//
// Created by X-ray on 8/18/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_CREATE_MESSAGE_HPP
#define DISCORD_CPP_BOT_CREATE_MESSAGE_HPP
#include "../datatypes.hpp"
#include "command_handler.hpp"
#include "event_handler.hpp"

namespace bot {
    namespace events {
        class message_create {
        public:
            explicit message_create(events::handler& h, const std::string& prefix, const std::string& user_id);

						void AddCommand(std::string name, std::string description, commandhandler::command_t handler);
        private:
            commandhandler commandhandler_;
        };
    }
}
#endif //DISCORD_CPP_BOT_CREATE_MESSAGE_HPP
