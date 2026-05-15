#include "modian/service/input_protocol_service.h"

namespace modian::brush::service {
	std::string input_protocol_service::build_key_event_request(const WPARAM& key) {
		// 1. 处理特殊功能键
		switch (key) {
			case VK_LEFT:  return "cmd:left";
			case VK_RIGHT: return "cmd:right";
			case VK_SPACE: return "cmd:space";
			case VK_BACK:  return "cmd:backspace";
		}

		if (key >= 'A' && key <= 'Z') {
			return std::string(1, static_cast<char>(key));
		}

		return "";
	}

    common::core::protocol::input::v1::instruction input_protocol_service::parse_instruction_response(std::string response) {
		if (response.empty()) return { common::core::protocol::input::v1::message_type::NONE, "" };

		if (response.size() >= 2 && response.at(1) == ':') {
			switch (response.at(0)) {
            case static_cast<char>(common::core::protocol::input::v1::message_type::COMMIT):
				return { common::core::protocol::input::v1::message_type::COMMIT, response.substr(2) };
            case static_cast<char>(common::core::protocol::input::v1::message_type::UPDATE):
				return { common::core::protocol::input::v1::message_type::UPDATE, response.substr(2) };
			default:
				return {};
			}
		}

		return { common::core::protocol::input::v1::message_type::UPDATE, response };
	}
}
