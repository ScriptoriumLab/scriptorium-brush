#include "gmock/gmock-function-mocker.h"
#include "gtest/gtest.h"

#include "modian/ui/core/renderer/candidate_renderer.h"
#include "modian/ui/win32/candidate_window.h"

class mock_candidate_renderer final : public modian::infra::ui::core::renderer::candidate_renderer {
public:
	MOCK_METHOD(void, set_context, (const std::shared_ptr<modian::infra::ui::graphic_context>& context), (override));
	MOCK_METHOD(void, begin_frame, (), (override));
	MOCK_METHOD(void, draw_list, (const std::vector<std::wstring>& items), (override));
	MOCK_METHOD(void, end_frame, (), (override));
};

TEST(candidate_window_test, should_successfully_call_renderer_when_update_candidates) {
	const auto mock_renderer = std::make_shared<mock_candidate_renderer>();

	modian::infra::ui::win32::candidate_window window(GetModuleHandle(nullptr), mock_renderer);

	EXPECT_CALL(*mock_renderer, set_context(testing::_)).Times(1);
	EXPECT_CALL(*mock_renderer, begin_frame()).Times(2);
	EXPECT_CALL(*mock_renderer, draw_list(testing::_)).Times(2);
	EXPECT_CALL(*mock_renderer, end_frame()).Times(2);

	window.create();
	window.show();
	window.update_candidates({L"你", L"尼", L"泥"});
	window.update_candidates({L"好", L"号", L"豪"});
}