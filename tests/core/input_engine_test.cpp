#include "gtest/gtest.h"
#include "modian/core/input_engine.h"

class dummy_engine : public modian::core::input_engine {
public:
	std::vector<std::wstring> convert(const std::wstring& input) override {
		return {L"测试"};
	}

	void load_dictionary(const std::string& path) override {}
};

TEST(input_engine_test, should_successfully_return_dummy_return) {
	dummy_engine engine;
	auto result = engine.convert(L"test");
	ASSERT_FALSE(result.empty());
	EXPECT_EQ(result[0], L"测试");
}