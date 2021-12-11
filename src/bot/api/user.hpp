//
// Created by X-ray on 10/17/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_USER_HPP
#define DISCORD_CPP_BOT_USER_HPP
#include <rapidjson/document.h>
#include "api.hpp"
#include "../datatypes.hpp"

namespace bot {
	namespace api {
		namespace user {
			datatypes::user::user_t CurrentUser() {
				auto r = ApiGet("users/@me");
				rapidjson::Document json;
				json.Parse(r.text.c_str());
				return datatypes::user::user_t(json.GetObject());
			}

			datatypes::user::user_t GetUser(const std::string& id) {
				auto r = ApiGet(fmt::format("users/{0}", id));
				rapidjson::Document json;
				json.Parse(r.text.c_str());
				return datatypes::user::user_t(json.GetObject());
			}
		} // user
	} // api
} // bot
#endif //DISCORD_CPP_BOT_USER_HPP
