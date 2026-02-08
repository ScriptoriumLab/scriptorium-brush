#pragma once

#include <string>
#include <windows.h>

namespace modian::brush::service {
	class input_protocol_service {
	public:
		static std::string build_key_event_request(const WPARAM& key);
	};
}
