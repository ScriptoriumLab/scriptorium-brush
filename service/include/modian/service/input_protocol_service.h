#pragma once

#include <string>
#include <windows.h>

#include "modian/common/core/protocol/v1/input/instruction.h"

namespace modian::brush::service {
	class input_protocol_service {
	public:
		static std::string build_key_event_request(const WPARAM& key);
		static common::core::protocol::input::v1::instruction parse_instruction_response(std::string response);
	};
}
