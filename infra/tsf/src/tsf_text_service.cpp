#include "modian/tsf/tsf_text_service.h"

#include <future>

#include "modian/core/logger/logger_service.h"

namespace modian::brush::infra::tsf {
	tsf_text_service::~tsf_text_service() {
		if (thread_mgr_) {
			thread_mgr_->Release();
			thread_mgr_ = nullptr;
		}

		--g_active_objects;
	}

	STDMETHODIMP tsf_text_service::Activate(ITfThreadMgr* p_thread_mgr, TfClientId tf_client_id) {
		if (is_active_.exchange(true)) {
			return S_OK;
		}

		core::logger_service::logger()->info("Activating Modian IME...");

		if (thread_mgr_) thread_mgr_->Release();
		thread_mgr_ = p_thread_mgr;
		thread_mgr_->AddRef();

		client_id_ = tf_client_id;

		ITfKeystrokeMgr* keystroke_mgr{nullptr};
		HRESULT hr = p_thread_mgr->QueryInterface(IID_ITfKeystrokeMgr, reinterpret_cast<void**>(&keystroke_mgr));
		if (SUCCEEDED(hr)) {
			core::logger_service::logger()->info("Activating Modian IME key event handler...");
			hr = keystroke_mgr->AdviseKeyEventSink(tf_client_id, &key_event_service_, TRUE);
			keystroke_mgr->Release();
		}

		return hr;
	}

	STDMETHODIMP tsf_text_service::Deactivate() {
		core::logger_service::logger()->info("Deactivating Modian IME...");
		ITfKeystrokeMgr* keystroke_mgr{nullptr};
		HRESULT hr = thread_mgr_->QueryInterface(IID_ITfKeystrokeMgr, reinterpret_cast<void**>(&keystroke_mgr));
		if (SUCCEEDED(hr)) {
			hr = keystroke_mgr->UnadviseKeyEventSink(client_id_);
			keystroke_mgr->Release();
		}

		thread_mgr_ = nullptr;
		client_id_ = TF_CLIENTID_NULL;
		is_active_.store(false);

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
		return ++ref_count_;
	}

	STDMETHODIMP_(ULONG) tsf_text_service::Release() {
		const ULONG count = --ref_count_;
		if (count == 0) delete this;
		return count;
	}
}
