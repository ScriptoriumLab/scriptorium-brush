#include <modian/core/protocol/composition_protocol.h>

#include "gtest/gtest.h"

TEST(composition_protocol_test, should_get_commit_and_correct_candidate_when_decode) {
	const auto& protocol = modian::brush::core::protocol::composition_protocol::decode("C:你");
	ASSERT_EQ(protocol.type, modian::brush::core::protocol::composition_protocol::message_type::COMMIT);
	ASSERT_EQ(protocol.payload, "你");
}

TEST(composition_protocol_test, should_get_update_and_correct_message_when_decode) {
	const auto& protocol = modian::brush::core::protocol::composition_protocol::decode("U:dia");
	ASSERT_EQ(protocol.type, modian::brush::core::protocol::composition_protocol::message_type::UPDATE);
	ASSERT_EQ(protocol.payload, "dia");
}

TEST(composition_protocol_test, should_get_none_when_decode_empty_message) {
	const auto& protocol = modian::brush::core::protocol::composition_protocol::decode("");
	ASSERT_EQ(protocol.type, modian::brush::core::protocol::composition_protocol::message_type::NONE);
	ASSERT_EQ(protocol.payload, "");
}