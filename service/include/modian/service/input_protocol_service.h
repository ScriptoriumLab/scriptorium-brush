#pragma once

#include <string>
#include <windows.h>

#include "modian/core/protocol/instruction.h"

namespace modian::brush::service {
	class input_protocol_service {
	public:
		static std::string build_key_event_request(const WPARAM& key);
		static core::protocol::instruction parse_instruction_response(std::string response);
	};
}
