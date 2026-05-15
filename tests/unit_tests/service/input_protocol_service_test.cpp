#include "gtest/gtest.h"

#include "modian/service/input_protocol_service.h"

TEST(input_protocol_service_test, should_build_backspace_request_when_user_input_is_vb_back) {
	constexpr WPARAM key{VK_BACK};
	const auto request = modian::brush::service::input_protocol_service::build_key_event_request(key);

	ASSERT_EQ(request, "cmd:backspace");
}

TEST(input_protocol_service_test, should_build_right_request_when_user_input_is_vb_right) {
	constexpr WPARAM key{VK_RIGHT};
	const auto request = modian::brush::service::input_protocol_service::build_key_event_request(key);

	ASSERT_EQ(request, "cmd:right");
}

TEST(input_protocol_service_test, should_build_left_request_when_user_input_is_vb_left) {
	constexpr WPARAM key{VK_LEFT};
	const auto request = modian::brush::service::input_protocol_service::build_key_event_request(key);

	ASSERT_EQ(request, "cmd:left");
}

TEST(input_protocol_service_test, should_build_space_request_when_user_input_is_vb_space) {
	constexpr WPARAM key{VK_SPACE};
	const auto request = modian::brush::service::input_protocol_service::build_key_event_request(key);

	ASSERT_EQ(request, "cmd:space");
}

TEST(input_protocol_service_test, should_build_request_when_user_input_other_keys) {
	constexpr WPARAM key{'N'};
	const auto request = modian::brush::service::input_protocol_service::build_key_event_request(key);

	ASSERT_EQ(request, "N");
}

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
