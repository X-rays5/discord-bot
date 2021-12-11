//
// Created by X-ray on 10/17/2021.
//

#include "client.hpp"

namespace bot {
	client::client(std::string token, std::uint32_t intent) : token_(std::move(token)), intent_(intent), threadpool_(1) {
		heartbeat_ = std::make_shared<connection::heartbeat>();

		message_handler_t msg_handler = [this](websocketpp::client<websocketpp::config::asio_tls_client> *c, websocketpp::connection_hdl hdl, const message_ptr& msg){
			threadpool_.add_job([=]{
				this->message_handler(c, hdl, msg);
			});
		};

		sock_ = std::make_shared<connection::websocket>();
		sockthread_ = std::thread([this, msg_handler]{
			this->sock_->run(msg_handler);
		});

		api::user_token = token_;
	}

	client::~client() noexcept {
		sock_.reset();
		sockthread_.join();
		heartbeat_.reset();
		heartbeatthread_.join();
	}

	void client::Identify(websocketpp::client<websocketpp::config::asio_tls_client>* c, websocketpp::connection_hdl hdl) {
		rapidjson::Document json;
		json.SetObject();
		json.AddMember("op", 2, json.GetAllocator());

		rapidjson::Document d;
		d.SetObject();
		rapidjson::Value val(token_.c_str(), token_.size(), d.GetAllocator());
		d.AddMember("token", val, d.GetAllocator());
		d.AddMember("intents", intent_, d.GetAllocator());

		rapidjson::Document props;
		props.SetObject();
		props.AddMember("$os", "library", props.GetAllocator());
		props.AddMember("$browser", "library", props.GetAllocator());
		props.AddMember("$device", "library", props.GetAllocator());

		d.AddMember("properties", props, d.GetAllocator());
		json.AddMember("d", d, json.GetAllocator());

		c->send(hdl, JsonStringify(json), websocketpp::frame::opcode::text);
	}

	void client::Resume(websocketpp::client<websocketpp::config::asio_tls_client>* c, websocketpp::connection_hdl hdl) {
		rapidjson::Document json;
		json.SetObject();
		json.AddMember("op", 6, json.GetAllocator());

		rapidjson::Document d;
		d.SetObject();
		rapidjson::Value token(token_.c_str(), token_.size(), d.GetAllocator());
		d.AddMember("token", token, d.GetAllocator());
		rapidjson::Value sessionid(sessiondid_.c_str(), sessiondid_.size(), d.GetAllocator());
		d.AddMember("session_id", sessionid, d.GetAllocator());
		d.AddMember("seq", heartbeat_->GetLastLastsequenceId(), d.GetAllocator());

		json.AddMember("d", d, json.GetAllocator());

		c->send(hdl, JsonStringify(json), websocketpp::frame::opcode::text);
	}

	std::string client::JsonStringify(rapidjson::Document& json) {
		rapidjson::StringBuffer strbuf;
		strbuf.Clear();

		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		json.Accept(writer);

		return strbuf.GetString();
	}

	void client::message_handler(websocketpp::client<websocketpp::config::asio_tls_client>* c, websocketpp::connection_hdl hdl, const message_ptr& msg) {
		try {
			rapidjson::Document json;
			if (!json.Parse(msg->get_payload().c_str()).HasParseError()) {
				if (json.HasMember("op")) {
					int opcode = json["op"].GetInt();

					if (json.HasMember("s")) {
						if (json["s"].IsInt())
							heartbeat_->SetLastLastsequenceId(json["s"].GetInt());
					}

					switch (opcode) {
						case opcodes::kDispatch: {
							if (json["t"].GetString() == std::string("READY")) {
								if (json["d"].HasMember("session_id"))
									sessiondid_ = json["d"]["session_id"].GetString();
								localuser_ = datatypes::user::user_t(json["d"]["user"].GetObject());
								std::cout << fmt::format("Logged in as {0}#{1}\n", localuser_.username, localuser_.discriminator);
							}

							eventhandler_.HandleEvent(json["t"].GetString(), msg->get_payload());
							break;
						}
						case opcodes::kHello: {
							std::cout << "connected\n";
							redoidentify:

							if (json["d"].IsObject()) {
								int hearbeat_interval = 41250;
								if (json["d"].HasMember("heartbeat_interval")) {
									hearbeat_interval = json["d"]["heartbeat_interval"].GetInt();
								}
								heartbeatthread_ = std::thread([this, hearbeat_interval, c, hdl] {
									heartbeat_->run(hearbeat_interval, c, hdl);
								});
							} else {
								std::cout << "hello event misses data\ninvalid session occurred?\n";
							}

							Identify(c, hdl);
							break;
						}
						case opcodes::kHeartbeatACK: {
							heartbeat_->SetLastReceived(true);
							break;
						}
						case opcodes::kReconnect: {
							Resume(c, hdl);
							break;
						}
						case opcodes::kInvalidSession: {
							goto redoidentify; // in most cases resuming won't work here
							break;
						}
						default: {
							std::cerr << fmt::format("Unkown opcode received: {0}\n", opcode);
							break;
						}
					}
				}
			} else {
				std::cerr << fmt::format("Rapidjson parse error: {0}", std::to_string(json.GetParseError()));
			}
		} catch(websocketpp::exception &e) {
			std::cerr << e.what() << "\n";
		}
	}
}