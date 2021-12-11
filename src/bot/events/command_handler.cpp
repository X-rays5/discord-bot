//
// Created by X-ray on 10/17/2021.
//

#include "command_handler.hpp"

namespace bot {
	namespace events {
		commandhandler::commandhandler(std::string prefix, std::string user_id) :
			prefix_(std::move(prefix)),
			user_id_(std::move(user_id))
		{
		}

		void commandhandler::AddCommand(std::string name, std::string description, commandhandler::command_t handler) {
			command c {
				std::move(name),
				std::move(description),
				std::move(handler),
			};
			commands_[c.name] = c;
		}

		void commandhandler::HandleCommand(datatypes::message::message_t msg) {
			if (msg.author.id != user_id_)
				return;
			std::cout << "handling command\n";
			if (msg.content.find(prefix_) == 0) {
				std::regex commandargsregex(R"((\w+)\s(.+))");
				std::regex commandregex(R"((\w+))");
				std::smatch commandmatch;

				if (std::regex_search(msg.content, commandmatch, commandargsregex) || std::regex_search(msg.content, commandmatch, commandregex)) {
					auto command = commands_.find(commandmatch[1].str());
					if (command != commands_.end()) {
						std::cout << "executing command " << commandmatch[1].str() << "\n";
						std::vector<std::string> args;
						if (commandmatch.size() == 3)
							boost::split(args, commandmatch[2].str(), boost::is_any_of(" "));
						std::invoke(command->second.func, args, msg);
					} else {
						std::cerr << "unkown command\n";
					}
				}
			}
		}
	}
}