#include "modian/tsf/tsf_key_event_service.h"

#include <sstream>

#include "modian/info/modian_info.h"
#include "modian/core/pinyin_engine.h"
#include "modian/util/log_util.h"

modian::tsf::tsf_key_event_service::tsf_key_event_service() : ref_count_{1} {}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnKeyDown(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
	// TODO: 改一下处理键盘输入的逻辑
	if (!pf_eaten) return E_POINTER;

	if (const auto character{static_cast<wchar_t>(w_param)}; (character >= L'a' && character <= L'z') || (character >= L'A' && character <= L'Z')) {
		input_pinyin_.push_back(towlower(character));

		auto& engine = modian::core::pinyin_engine::get_instance(DICTIONARY_PATH);
		std::vector<std::wstring> candidates = engine.convert(input_pinyin_);

		// 在这里，可以调用候选词 UI 模块来更新显示候选词
		// 例如，输出调试信息：
		std::wstringstream ss;
		ss << L"Current composition: " << input_pinyin_ << L"\nCandidates: ";
		for (const auto& word : candidates) {
			ss << word << L" ";
		}
		ss << L"\n";
	}

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
