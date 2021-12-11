//
// Created by X-ray on 10/17/2021.
//

#include "event_handler.hpp"

namespace bot {
	namespace events {
		void handler::HandleEvent(const std::string& event, std::string payload) {
			auto result = events_.equal_range(event);

			unsigned int handlerC = 0;
			for (auto it = result.first; it != result.second; it++) {
				handlerC++;

        std::invoke(it->second, payload);
#ifndef NDEBUG
				std::cout << "Handled event: " << event << "\n";

			}

			if (handlerC < 1) {
				std::cerr << "Unkown event occured: " << event << "\n";
			}
#endif
    }

		void handler::AddEventHandler(std::string eventname, handler::event_handler_t handler) {
			events_.insert(std::pair<std::string, event_handler_t>(std::move(eventname), std::move(handler)));
		}
	}
}