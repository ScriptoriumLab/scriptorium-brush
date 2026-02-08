#pragma once

#include <string>
#include <windows.h>

#include "modian/core/protocol/composition_protocol.h"

namespace modian::brush::service {
	class input_protocol_service {
	public:
		static std::string build_key_event_request(const WPARAM& key);
		static core::protocol::composition_protocol parse_instruction_response(const std::string& response);
	};
}
