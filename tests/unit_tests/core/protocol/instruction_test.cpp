#include "gtest/gtest.h"

#include "modian/core/protocol/instruction.h"

TEST(instruction_test, should_return_values_according_to_rules_when_pass_with_customized_rules) {
	const modian::brush::core::protocol::input::v1::instruction protocol = { modian::brush::core::protocol::input::v1::message_type::COMMIT, "你" };
	const auto& [parsed_content, is_commit] = protocol.unpack(
		[](const auto& p){ return p.payload; },
		[](const auto& p){ return p.type == modian::brush::core::protocol::input::v1::message_type::COMMIT; }
	);

	ASSERT_EQ(parsed_content, "你");
	ASSERT_TRUE(is_commit);
}
