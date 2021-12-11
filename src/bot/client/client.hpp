//
// Created by X-ray on 8/17/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_CLIENT_HPP
#define DISCORD_CPP_BOT_CLIENT_HPP
#include <string>
#include <memory>
#include <thread>
#include <rapidjson/document.h>
#include "../connection/websocket.hpp"
#include "../connection/heartbeat.hpp"
#include "../threadpool/threadpool.hpp"
#include "../datatypes.hpp"
#include "../events/event_handler.hpp"
#include "../api/api.hpp"

// https://discord.com/developers/docs/topics/gateway#list-of-intents
#define INTENT_GUILDS 1 << 0
#define INTENT_GUILD_MEMBERS 1 << 1
#define INTENT_GUILD_BANS 1 << 2
#define INTENT_GUILD_EMOJIS_AND_STICKERS 1 << 3
#define INTENT_GUILD_INTEGRATIONS 1 << 4
#define INTENT_GUILD_WEBHOOKS 1 << 5
#define INTENT_GUILD_INVITES  1 << 6
#define INTENT_GUILD_VOICE_STATES 1 << 7
#define INTENT_GUILD_PRESENCES 1 << 8
#define INTENT_GUILD_MESSAGES 1 << 9
#define INTENT_GUILD_MESSAGE_REACTIONS 1 << 10
#define INTENT_GUILD_MESSAGE_TYPING 1 << 11
#define INTENT_DIRECT_MESSAGES 1 << 12
#define INTENT_DIRECT_MESSAGE_REACTIONS 1 << 13
#define INTENT_DIRECT_MESSAGE_TYPING 1 << 14

namespace bot {
    class client {
    public:
        enum opcodes : int {
            kDispatch = 0,
            kHeartbeat = 1,
            kIdentify = 2,
            kPresenceUpdate = 3,
            kVoiceStateUpdate = 4,
            kResume = 6,
            kReconnect = 7,
            kRequestGuildMembers = 8,
            kInvalidSession = 9,
            kHello = 10,
            kHeartbeatACK = 11,
            };

        using message_handler_t = std::function<void(websocketpp::client<websocketpp::config::asio_tls_client> *c, websocketpp::connection_hdl hdl, const message_ptr& msg)>;

        explicit client(std::string token, std::uint32_t intent);

        ~client();

        inline bot::events::handler& GetEventHandler() noexcept {
            return eventhandler_;
        }

        inline bot::datatypes::user::user_t GetLocalUser() noexcept {
            return localuser_;
        }
    private:
        std::string token_;
        std::uint32_t intent_;
        std::string sessiondid_;
        bot::datatypes::user::user_t localuser_;

        std::shared_ptr<connection::websocket> sock_;
        std::thread sockthread_;

        std::shared_ptr<connection::heartbeat> heartbeat_;
        std::thread heartbeatthread_;

        bot::thread_pool threadpool_;

        bot::events::handler eventhandler_;
    private:
        void Identify(websocketpp::client<websocketpp::config::asio_tls_client> *c, websocketpp::connection_hdl hdl);
        void Resume(websocketpp::client<websocketpp::config::asio_tls_client> *c, websocketpp::connection_hdl hdl);
        inline static std::string JsonStringify(rapidjson::Document& json);
        void message_handler(websocketpp::client<websocketpp::config::asio_tls_client> *c, websocketpp::connection_hdl hdl, const message_ptr& msg);
    };
}
#endif //DISCORD_CPP_BOT_CLIENT_HPP
