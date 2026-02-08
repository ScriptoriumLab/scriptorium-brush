#include "modian/core/protocol/instruction.h"

namespace modian::brush::core::protocol::input::v1 {
	instruction::instruction() : type{message_type::NONE} {}
	instruction::instruction(const message_type& type, std::string payload) : type{type}, payload{payload} {}
}
