#include "modian/core/protocol/composition_protocol.h"

namespace modian::brush::core::protocol {
	composition_protocol::composition_protocol() : type{message_type::NONE} {}
	composition_protocol::composition_protocol(const message_type& type, std::string payload) : type{type}, payload{payload} {}

	composition_protocol composition_protocol::decode(const std::string& raw) {
		if (raw.empty()) return { message_type::NONE, "" };

		if (raw.size() >= 2 && raw.at(1) == ':') {
			switch (raw.at(0)) {
				case static_cast<char>(message_type::COMMIT):
					return { message_type::COMMIT, raw.substr(2) };
				case static_cast<char>(message_type::UPDATE):
					return { message_type::UPDATE, raw.substr(2) };
				default:
					return {};
			}
		}

		return { message_type::UPDATE, raw };
	}
}
