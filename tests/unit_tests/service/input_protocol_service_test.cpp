#include "gtest/gtest.h"

#include "modian/service/input_protocol_service.h"

TEST(input_protocol_service_test, should_build_backspace_request_when_user_input_is_vb_back) {
	constexpr WPARAM key{VK_BACK};
	const auto request = modian::brush::service::input_protocol_service::build_key_event_request(key);

	ASSERT_EQ(request, "\b");
}

TEST(input_protocol_service_test, should_build_request_when_user_input_other_keys) {
	constexpr WPARAM key{'N'};
	const auto request = modian::brush::service::input_protocol_service::build_key_event_request(key);

	ASSERT_EQ(request, "N");
}