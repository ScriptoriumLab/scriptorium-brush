#include "modian/tsf/tsf_key_event_service.h"

#include <sstream>
#include <spdlog/spdlog.h>

#include "modian/tsf/util/logger/log_util.h"
#include "modian/core/engine/pinyin_engine.h"

modian::tsf::tsf_key_event_service::tsf_key_event_service() : ref_count_{1} {
	char* userprofile{nullptr};
	size_t size = 0;

	if (const errno_t err = _dupenv_s(&userprofile, &size, "USERPROFILE"); err != 0 || userprofile == nullptr) {
		spdlog::error("Failed to retrieve USERPROFILE.");
		return;
	}

	const std::string dictionary_path = std::string(userprofile) + "/Modian/Local/pinyin_dictionary.txt";
	input_engine_ = std::make_shared<core::pinyin_engine>(core::pinyin_engine::get_instance(dictionary_path));
}

void modian::tsf::tsf_key_event_service::load_engine(const std::shared_ptr<core::input_engine>& input_engine) {
	input_engine_ = input_engine;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnKeyDown(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
	spdlog::info("Handling on key down");
	// TODO: 改一下处理键盘输入的逻辑
	if (!pf_eaten) return E_POINTER;

	if (const auto character{static_cast<wchar_t>(w_param)}; (character >= L'a' && character <= L'z') || (character >= L'A' && character <= L'Z')) {
		input_pinyin_.push_back(towlower(character));

		if (const auto candidates = input_engine_->convert(input_pinyin_); !candidates.empty()) {
            spdlog::info("Get potential candidates");
            for (const auto& candidate : candidates) {
                spdlog::info("Candidates: {}", util::logger::wstring_to_string(candidate));
            }

			input_pinyin_.clear();
		}
	}

	spdlog::debug("Finished handling on key down");
	return S_OK;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnKeyUp(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
	*pf_eaten = FALSE;
	return S_OK;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnTestKeyDown(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
	*pf_eaten = FALSE;
	return S_OK;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnTestKeyUp(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
	*pf_eaten = FALSE;
	return S_OK;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnPreservedKey(ITfContext* pic, const GUID& r_guid, BOOL* pf_eaten) {
	*pf_eaten = FALSE;
	return S_OK;
}

HRESULT modian::tsf::tsf_key_event_service::OnSetFocus(BOOL f_foreground) {
	return S_OK;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::QueryInterface(const IID& riid, void** ppv_object) {
	if (!ppv_object) return E_POINTER;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfKeyEventSink)) {
		*ppv_object = static_cast<ITfKeyEventSink*>(this);
		AddRef();
		return S_OK;
	}

	*ppv_object = nullptr;
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) modian::tsf::tsf_key_event_service::AddRef() {
	return InterlockedIncrement(&ref_count_);
}

STDMETHODIMP_(ULONG) modian::tsf::tsf_key_event_service::Release() {
	const ULONG count = InterlockedDecrement(&ref_count_);
	if (count == 0) {
		delete this;
	}
	return count;
}
