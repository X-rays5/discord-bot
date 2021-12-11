//
// Created by X-ray on 8/18/2021.
//

#include "websocket.hpp"

namespace bot {
    namespace connection {
        void websocket::run(std::function<void(client *c, websocketpp::connection_hdl hdl, const message_ptr& msg)> msg_handler) {
            try {
                //c_.set_access_channels(websocketpp::log::alevel::all);
                c_.clear_access_channels(websocketpp::log::alevel::all);

                c_.init_asio();
                c_.set_tls_init_handler(bind(&on_tls_init));

                c_.set_message_handler(bind(msg_handler,&c_,::_1,::_2));

                websocketpp::lib::error_code ec;
                client::connection_ptr con = c_.get_connection("wss://gateway.discord.gg/?v=9&encoding=json", ec);
                if (ec) {
                    std::cerr << fmt::format("could not create connection because: {0}\n", ec.message());
                }

                hdl_ = con->get_handle();
                c_.connect(con);

                c_.run();
            } catch (websocketpp::exception const & e) {
                std::cerr << e.what() << "\n";
            }
        }

        context_ptr websocket::on_tls_init() {
            context_ptr ctx = std::make_shared<websocketpp::lib::asio::ssl::context>(websocketpp::lib::asio::ssl::context::sslv23);

            try {
                ctx->set_options(websocketpp::lib::asio::ssl::context::default_workarounds |
                websocketpp::lib::asio::ssl::context::no_sslv2 |
                websocketpp::lib::asio::ssl::context::no_sslv3 |
                websocketpp::lib::asio::ssl::context::single_dh_use);
            } catch (std::exception &e) {
                std::cerr << fmt::format("Error in context pointer: {0}\n", e.what());
            }
            return ctx;
        }
    }
}