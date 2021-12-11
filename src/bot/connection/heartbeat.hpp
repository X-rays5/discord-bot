//
// Created by X-ray on 8/17/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_HEARTBEAT_HPP
#define DISCORD_CPP_BOT_HEARTBEAT_HPP
#include <thread>
#include <mutex>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

namespace bot {
    namespace connection {
        class heartbeat {
        public:
            ~heartbeat() {
                running = false;
            }

            void run(int interval, websocketpp::client<websocketpp::config::asio_tls_client> *c, websocketpp::connection_hdl hdl);

            void SetLastLastsequenceId(int lastsequence) {
                std::lock_guard lock(sequencelock_);
                lastsequence_ = lastsequence;
            }

            int GetLastLastsequenceId() {
                std::lock_guard lock(sequencelock_);
                return lastsequence_;
            }

            void SetLastReceived(bool received) {
                std::lock_guard lock(receivedlock_);
                lastreceived_ = received;
            }
        private:
            int lastsequence_;
            bool running = true;
            bool lastreceived_;
            std::mutex sequencelock_;
            std::mutex receivedlock_;
        private:
            void SendHeartBeat(websocketpp::client<websocketpp::config::asio_tls_client> *c, websocketpp::connection_hdl hdl);

            inline static std::string JsonStringify(rapidjson::Document& json) {
                rapidjson::StringBuffer strbuf;
                strbuf.Clear();

                rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
                json.Accept(writer);

                return strbuf.GetString();
            }
        };
    }
}
#endif //DISCORD_CPP_BOT_HEARTBEAT_HPP
