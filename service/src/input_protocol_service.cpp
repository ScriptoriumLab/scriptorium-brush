#include "modian/service/input_protocol_service.h"

namespace modian::brush::service {
	std::string input_protocol_service::build_key_event_request(const WPARAM& key) {
		std::string req_data;

		if (key == VK_BACK) {
			req_data = "\b";
		} else {
			req_data = std::string(1, static_cast<char>(key));
		}

		return req_data;
	}

	core::protocol::instruction input_protocol_service::parse_instruction_response(std::string response) {
		if (response.empty()) return { core::protocol::instruction::message_type::NONE, "" };

		if (response.size() >= 2 && response.at(1) == ':') {
			switch (response.at(0)) {
			case static_cast<char>(core::protocol::instruction::message_type::COMMIT):
				return { core::protocol::instruction::message_type::COMMIT, response.substr(2) };
			case static_cast<char>(core::protocol::instruction::message_type::UPDATE):
				return { core::protocol::instruction::message_type::UPDATE, response.substr(2) };
			default:
				return {};
			}
		}

		return { core::protocol::instruction::message_type::UPDATE, response };
	}
}
