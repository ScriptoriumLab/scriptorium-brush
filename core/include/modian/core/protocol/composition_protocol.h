#pragma once

#include <string>

namespace modian::brush::core::protocol {
	class composition_protocol {
	public:
		enum class message_type {
			NONE = 'N',
			UPDATE = 'U',
			COMMIT = 'C'
		};

		message_type type;
		std::string payload;

		composition_protocol();
		composition_protocol(const message_type& type, std::string payload);

		template<typename ...F>
		auto unpack(F... rules) const {
			return std::make_tuple(rules(*this)...);
		}
	};
}