//
// Created by X-ray on 8/18/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_COMMAND_HANDLER_HPP
#define DISCORD_CPP_BOT_COMMAND_HANDLER_HPP
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <regex>
#include <boost/algorithm/string.hpp>
#include "../datatypes.hpp"

namespace bot {
    namespace events {
        class commandhandler {
        public:
            explicit commandhandler(std::string prefix, std::string user_id);

            using command_t = std::function<void(std::vector<std::string>, datatypes::message::message_t)>;
            void AddCommand(std::string name, std::string description, command_t handler);

            void HandleCommand(datatypes::message::message_t msg);
        private:
            struct command {
                std::string name;
                std::string desc;
                command_t func;
            };

            std::string prefix_;
            std::string user_id_;
            std::unordered_map<std::string, command> commands_;
        };
    }
}
#endif //DISCORD_CPP_BOT_COMMAND_HANDLER_HPP
