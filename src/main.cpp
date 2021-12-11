#include "main.hpp"

#include <utility>

int main() {
    std::ifstream reader("token");
    std::string token;
    std::getline(reader, token);
    bot::client c(token, INTENT_GUILD_MESSAGES | INTENT_DIRECT_MESSAGES);
    while (c.GetLocalUser().id.empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    bot::events::message_create msg(c.GetEventHandler(), ".", c.GetLocalUser().id);
		msg.AddCommand("user", "Get a user by id", [](std::vector<std::string> args, bot::datatypes::message::message_t msg){
			bot::commands::GetUser(std::move(args), std::move(msg));
		});
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}
