#pragma once

#include <string>

#include "modian/common/core/protocol/v1/input/key_event.h"
#include "modian/common/core/protocol/v1/input/instruction.h"

namespace modian::brush::service {
	class input_protocol_service {
	public:
		static std::string build_key_event_request(const common::core::protocol::input::v1::key_event& key_event);
		static common::core::protocol::input::v1::instruction parse_instruction_response(std::string response);
	};
}
