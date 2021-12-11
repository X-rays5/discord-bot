//
// Created by X-ray on 10/17/2021.
//

#include "message_create.hpp"

namespace bot {
	namespace events {
		message_create::message_create(events::handler& h, const std::string& prefix, const std::string& user_id) : commandhandler_(prefix, user_id) {
			h.AddEventHandler("MESSAGE_CREATE", [this](const std::string& payload){
				rapidjson::Document json;
				json.Parse(payload.c_str());
				this->commandhandler_.HandleCommand(datatypes::message::message_t(json["d"].GetObject()));
			});
		}

		void message_create::AddCommand(std::string name, std::string description, events::commandhandler::command_t handler) {
			commandhandler_.AddCommand(std::move(name), std::move(description), std::move(handler));
		}
	}
}