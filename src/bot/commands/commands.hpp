//
// Created by X-ray on 10/17/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_COMMANDS_HPP
#define DISCORD_CPP_BOT_COMMANDS_HPP
#include <string>
#include <vector>
#include "../datatypes.hpp"

namespace bot {
	namespace commands {
		void GetUser(std::vector<std::string> args, bot::datatypes::message::message_t msg);
	}
}
#endif //DISCORD_CPP_BOT_COMMANDS_HPP
