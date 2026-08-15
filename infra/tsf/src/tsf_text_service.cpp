#include "scriptorium/tsf/tsf_text_service.h"

#include <future>
#include <wrl/client.h>

#include "scriptorium/felt/core/logger/logger_service.h"

#include "scriptorium/tsf/tsf_display_attribute_info.h"
#include "scriptorium/tsf/dll/info/registry_info.h"

namespace scriptorium::brush::infra::tsf {
	tsf_text_service::tsf_text_service() : key_event_service_(static_cast<ITfTextInputProcessor*>(this)) {
		++g_active_objects;
	}
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

        felt::core::logger_service::logger()->info("Activating Scriptorium IME...");

		if (thread_mgr_) thread_mgr_->Release();
		thread_mgr_ = p_thread_mgr;
		thread_mgr_->AddRef();

		client_id_ = tf_client_id;
		key_event_service_.set_client_id(client_id_);

		ITfKeystrokeMgr* keystroke_mgr{nullptr};
		HRESULT hr = p_thread_mgr->QueryInterface(IID_ITfKeystrokeMgr, reinterpret_cast<void**>(&keystroke_mgr));
		if (SUCCEEDED(hr)) {
            felt::core::logger_service::logger()->info("Activating Scriptorium IME key event handler...");
			hr = keystroke_mgr->AdviseKeyEventSink(tf_client_id, &key_event_service_, TRUE);
			keystroke_mgr->Release();
		}

		return hr;
	}

	STDMETHODIMP tsf_text_service::Deactivate() {
        felt::core::logger_service::logger()->info("Deactivating Scriptorium IME...");
		if (thread_mgr_ && client_id_ != TF_CLIENTID_NULL) {
			Microsoft::WRL::ComPtr<ITfKeystrokeMgr> keystroke_mgr;
			if (SUCCEEDED(thread_mgr_->QueryInterface(IID_PPV_ARGS(&keystroke_mgr)))) {
				keystroke_mgr->UnadviseKeyEventSink(client_id_);
			}
		}

		if (thread_mgr_) {
			thread_mgr_->Release();
			thread_mgr_ = nullptr;
		}

		client_id_ = TF_CLIENTID_NULL;
		is_active_.store(false);

		return S_OK;
	}

    STDMETHODIMP tsf_text_service::QueryInterface(REFIID riid, void** ppv_obj) {
		if (!ppv_obj) return E_POINTER;

		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfTextInputProcessor)) {
			*ppv_obj = static_cast<ITfTextInputProcessor*>(this);
		} else if (IsEqualIID(riid, IID_ITfDisplayAttributeProvider)) {
			*ppv_obj = static_cast<ITfDisplayAttributeProvider*>(this);
		} else {
			*ppv_obj = nullptr;
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	STDMETHODIMP_(ULONG) tsf_text_service::AddRef() {
		return ++ref_count_;
	}

	STDMETHODIMP_(ULONG) tsf_text_service::Release() {
		const ULONG count = --ref_count_;
		if (count == 0) delete this;
		return count;
	}

    class tsf_display_attribute_enum final : public IEnumTfDisplayAttributeInfo {
	public:
		tsf_display_attribute_enum() = default;
		
		STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override {
			if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IEnumTfDisplayAttributeInfo)) {
				*ppv = static_cast<IEnumTfDisplayAttributeInfo*>(this);
				AddRef(); return S_OK;
			}
			*ppv = nullptr; return E_NOINTERFACE;
		}
		STDMETHODIMP_(ULONG) AddRef() override { return ++ref_count_; }
		STDMETHODIMP_(ULONG) Release() override {
			ULONG count = --ref_count_;
			if (count == 0) delete this;
			return count;
		}

		STDMETHODIMP Clone(IEnumTfDisplayAttributeInfo** ppEnum) override {
			auto* pClone = new tsf_display_attribute_enum();
			pClone->index_ = index_;
			*ppEnum = pClone;
			return S_OK;
		}
		
		STDMETHODIMP Next(ULONG ulCount, ITfDisplayAttributeInfo** rgInfo, ULONG* pcFetched) override {
			ULONG fetched = 0;
			// 咱们店里只有一款产品 (index_ == 0 时返回)
			if (index_ == 0 && ulCount > 0) {
				rgInfo[0] = new tsf_display_attribute_info();
				fetched = 1;
				index_++;
			}
			if (pcFetched) *pcFetched = fetched;
			return fetched == ulCount ? S_OK : S_FALSE;
		}
		
		STDMETHODIMP Reset() override { index_ = 0; return S_OK; }
		STDMETHODIMP Skip(ULONG ulCount) override { index_ += ulCount; return (index_ > 1) ? S_FALSE : S_OK; }

	private:
		std::atomic<ULONG> ref_count_{1};
		ULONG index_{0};
	};

    STDMETHODIMP tsf_text_service::GetDisplayAttributeInfo(REFGUID guid, ITfDisplayAttributeInfo** ppInfo) {
		if (!ppInfo) return E_POINTER;
		
		if (IsEqualGUID(guid, dll::info::SCRIPTORIUM_IME_GUID_DISPLAY_ATTRIBUTE_INPUT)) {
			*ppInfo = new tsf_display_attribute_info();
			return S_OK;
		}
		
		*ppInfo = nullptr;
		return E_INVALIDARG;
	}

	STDMETHODIMP tsf_text_service::EnumDisplayAttributeInfo(IEnumTfDisplayAttributeInfo** ppEnum) {
		if (!ppEnum) return E_POINTER;
		*ppEnum = new tsf_display_attribute_enum();
		return S_OK;
	}
}
