#pragma once

#include <string>

namespace modian::brush::core::protocol::input::v1 {
	enum class message_type {
		NONE = 'N',
		UPDATE = 'U',
		COMMIT = 'C'
	};

	struct instruction {
		message_type type;
		std::string payload;

		template<typename ...F>
		auto unpack(F... rules) const {
			return std::make_tuple(rules(*this)...);
		}
	};
}
