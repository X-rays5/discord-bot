//
// Created by X-ray on 8/17/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_WEBSOCKET_HPP
#define DISCORD_CPP_BOT_WEBSOCKET_HPP
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <fmt/format.h>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef std::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

namespace bot {
    namespace connection {
        class websocket {
        public:
            explicit websocket() : c_(client())
            {
            }

            ~websocket() {
                websocketpp::lib::error_code ec;
                c_.pause_reading(hdl_);
                c_.close(hdl_, websocketpp::close::status::going_away, "");
            }

            void run(std::function<void(client *c, websocketpp::connection_hdl hdl, const message_ptr& msg)> msg_handler);
        private:
            client c_;
            websocketpp::connection_hdl hdl_;
        private:
            static context_ptr on_tls_init();
        };
    }
}
#endif //DISCORD_CPP_BOT_WEBSOCKET_HPP
