#include "modian/core/protocol/composition_protocol.h"

namespace modian::brush::core::protocol {
	composition_protocol::composition_protocol() : type{message_type::NONE} {}
	composition_protocol::composition_protocol(const message_type& type, std::string payload) : type{type}, payload{payload} {}
}
