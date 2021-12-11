//
// Created by X-ray on 10/17/2021.
//

#include "commands.hpp"
#include "../api/user.hpp"
#include "../api/channel.hpp"

namespace bot {
	namespace commands {
		void GetUser(std::vector<std::string> args, bot::datatypes::message::message_t msg) {
			if (args.size() == 1) {
				auto user = bot::api::user::GetUser(args[0]);
				bot::api::channel::EditMessage(msg.channel_id, msg.id, user.username);
			} else if (args.empty()) {
				auto user = bot::api::user::CurrentUser();
				bot::api::channel::EditMessage(msg.channel_id, msg.id, user.username);
			} else {
				bot::api::channel::EditMessage(msg.channel_id, msg.id, "**Wrong usage:** ```user ?<id>```");
			}
		}
	}
}
