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

	core::protocol::composition_protocol input_protocol_service::parse_instruction_response(const std::string& response) {
		return core::protocol::composition_protocol::decode(response);
	}
}
