//
// Created by X-ray on 8/18/2021.
//
#include "heartbeat.hpp"

namespace bot {
    namespace connection {
        void heartbeat::run(int interval, websocketpp::client<websocketpp::config::asio_tls_client> *c, websocketpp::connection_hdl hdl) {
            while (running) {
                try {
                    SendHeartBeat(c, hdl);
                    receivedlock_.lock();
                    lastreceived_ = false;
                    receivedlock_.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(interval - 250));
                } catch(websocketpp::exception &e) {
                    std::cerr << "Websocket exception while sending heartbeat: " << e.what() << "\n";
                }
            }
        }

        void heartbeat::SendHeartBeat(websocketpp::client<websocketpp::config::asio_tls_client> *c, websocketpp::connection_hdl hdl) {
            rapidjson::Document json;
            json.SetObject();
            json.AddMember("op", 1, json.GetAllocator());
            sequencelock_.lock();
            json.AddMember("d", lastsequence_, json.GetAllocator());
            sequencelock_.unlock();

            c->send(hdl, JsonStringify(json), websocketpp::frame::opcode::text);
            bool received = false;
            int ticks = 0;
            while (!received) {
                receivedlock_.lock();
                received = lastreceived_;
                receivedlock_.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                ticks++;
                if (ticks > 50) {
                    SendHeartBeat(c, hdl);
                    break;
                }
            }
        }
    }
}