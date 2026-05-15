#include "gtest/gtest.h"

#include "modian/service/input_protocol_service.h"

TEST(input_protocol_service_test, should_get_commit_and_correct_candidate_when_decode) {
	const auto& protocol = modian::brush::service::input_protocol_service::parse_instruction_response("C:你");
	ASSERT_EQ(protocol.type, modian::common::core::protocol::input::v1::message_type::COMMIT);
	ASSERT_EQ(protocol.payload, "你");
}

TEST(input_protocol_service_test, should_get_update_and_correct_message_when_decode) {
	const auto& protocol = modian::brush::service::input_protocol_service::parse_instruction_response("U:dia");
	ASSERT_EQ(protocol.type, modian::common::core::protocol::input::v1::message_type::UPDATE);
	ASSERT_EQ(protocol.payload, "dia");
}

TEST(input_protocol_service_test, should_get_none_when_decode_empty_message) {
	const auto& protocol = modian::brush::service::input_protocol_service::parse_instruction_response("");
	ASSERT_EQ(protocol.type, modian::common::core::protocol::input::v1::message_type::NONE);
	ASSERT_EQ(protocol.payload, "");
}
