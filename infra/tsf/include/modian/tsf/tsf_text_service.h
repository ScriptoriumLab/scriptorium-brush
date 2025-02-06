#pragma once

#include <windows.h>
#include <msctf.h>

namespace modian::tsf {
	class tsf_text_service final : public ITfTextInputProcessor {
	public:
		virtual ~tsf_text_service() = default;
		STDMETHODIMP Activate(ITfThreadMgr* p_thread_mgr, TfClientId tf_client_id) override;
		STDMETHODIMP Deactivate() override;

		STDMETHODIMP QueryInterface(REFIID riid, void** ppv_obj) override;
		STDMETHODIMP_(ULONG) AddRef() override;
		STDMETHODIMP_(ULONG) Release() override;
	private:
        ULONG ref_count_{0};
        TfClientId client_id_ = TF_CLIENTID_NULL;
        ITfThreadMgr* thread_mgr_ = nullptr;
	};
}