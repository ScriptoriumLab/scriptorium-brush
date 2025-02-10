#include "modian/tsf/tsf_text_service.h"

#include <spdlog/spdlog.h>

#include "modian/info/modian_info.h"
#include "modian/core/pinyin_engine.h"

namespace modian::tsf {
	tsf_text_service::tsf_text_service() {
		core::pinyin_engine::get_instance(DICTIONARY_PATH);
	}

	STDMETHODIMP tsf_text_service::Activate(ITfThreadMgr* p_thread_mgr, TfClientId tf_client_id) {
		spdlog::info("Activating Modian IME...");

		thread_mgr_ = p_thread_mgr;
		client_id_ = tf_client_id;

		ITfKeystrokeMgr* keystroke_mgr{nullptr};
		HRESULT hr = p_thread_mgr->QueryInterface(IID_ITfKeystrokeMgr, reinterpret_cast<void**>(&keystroke_mgr));
		if (SUCCEEDED(hr)) {
			spdlog::info("Activating Modian IME key event handler...");
			hr = keystroke_mgr->AdviseKeyEventSink(tf_client_id, &key_event_service_, TRUE);
			p_thread_mgr->Release();
		}

		return hr;
	}

	STDMETHODIMP tsf_text_service::Deactivate() {
		ITfKeystrokeMgr* keystroke_mgr{nullptr};
		HRESULT hr = thread_mgr_->QueryInterface(IID_ITfKeystrokeMgr, reinterpret_cast<void**>(&keystroke_mgr));
		if (SUCCEEDED(hr)) {
			hr = keystroke_mgr->UnadviseKeyEventSink(client_id_);
			keystroke_mgr->Release();
		}

		thread_mgr_ = nullptr;
		client_id_ = TF_CLIENTID_NULL;

		return hr;
	}

	STDMETHODIMP tsf_text_service::QueryInterface(REFIID riid, void** ppv_obj) {
		if (!ppv_obj) return E_POINTER;

		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfTextInputProcessor)) {
			*ppv_obj = static_cast<ITfTextInputProcessor*>(this);
			AddRef();
			return S_OK;
		}

		*ppv_obj = nullptr;
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) tsf_text_service::AddRef() {
		return InterlockedIncrement(&ref_count_);
	}

	STDMETHODIMP_(ULONG) tsf_text_service::Release() {
		ULONG count = InterlockedDecrement(&ref_count_);
		if (count == 0) delete this;
		return count;
	}

} // namespace modian::tsf
