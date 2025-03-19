#include <codecvt>
#include <iostream>
#include <locale>
#include <msctf.h>
#include <ostream>
#include <Windows.h>

#include "modian/core/logger/logger_service.h"
#include "modian/tsf/tsf_key_event_service.h"
#include "modian/manager/engine_manager.h"
#include "modian/manager/candidate_manager.h"
#include "modian/ui/core/platform/ui_platform.h"
#include "modian/core/engine/pinyin_engine.h"
#include "modian/core/engine/input_engine.h"

#define NOT_USED(__val__) ((void)(__val__))

class test_pinyin_engine final : public modian::core::pinyin_engine {
public:
	static constexpr std::string_view id{"test pinyin engine"};
	test_pinyin_engine() {
		load_dictionary(std::string{PROJECT_SOURCE_DIR}.append("/data/pinyin_dictionary.txt"));
	}
};

HRESULT typing(modian::infra::tsf::tsf_key_event_service& key_event_service, const std::wstring& input);

void input_method() {
	std::cout << "!!!!!!!!!!! in input method" << std::endl;
	auto ui_observer = modian::infra::ui::core::platform::ui_platform::instance();

	modian::manager::candidate_manager candidate_manager;
	candidate_manager.add_observer(ui_observer);

	modian::manager::engine_manager engine_manager{candidate_manager};
	engine_manager.add_new_engine(modian::core::lazy_load_dictionary<test_pinyin_engine>());
	engine_manager.select_engine("test pinyin engine");

	auto event_service = modian::infra::tsf::tsf_key_event_service{engine_manager};

	auto hr = typing(event_service, L"ni");
	NOT_USED(hr);

	hr = typing(event_service, L"hao");
	NOT_USED(hr);

	hr = typing(event_service, L"mo");
	NOT_USED(hr);

	hr = typing(event_service, L"dian");
	NOT_USED(hr);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	std::thread input_method_thread{input_method};
	modian::infra::ui::core::platform::ui_platform::instance()->ui_thread_func();

	return 0;
}

HRESULT typing(modian::infra::tsf::tsf_key_event_service& key_event_service, const std::wstring& input) {
	BOOL pf_eaten = FALSE;
	HRESULT hr = S_OK;

	for (const auto& character : input) {
		ITfContext* context = nullptr;
		constexpr LPARAM l_param = 0;
		hr = key_event_service.OnKeyDown(context, character, l_param, &pf_eaten);

		if (hr != S_OK) {
			return hr;
		}
	}

	return hr;
}