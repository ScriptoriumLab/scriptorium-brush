#include "modian/tsf/tsf_text_service.h"

namespace modian::tsf {

	STDMETHODIMP tsf_text_service::Activate(ITfThreadMgr* p_thread_mgr, TfClientId tf_client_id) {
		thread_mgr_ = p_thread_mgr;
		client_id_ = tf_client_id;
		return S_OK;
	}

	STDMETHODIMP tsf_text_service::Deactivate() {
		thread_mgr_ = nullptr;
		client_id_ = TF_CLIENTID_NULL;
		return S_OK;
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
