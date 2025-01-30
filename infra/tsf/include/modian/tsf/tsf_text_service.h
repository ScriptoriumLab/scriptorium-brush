#ifndef TSF_TEXT_SERVICE_H
#define TSF_TEXT_SERVICE_H

#include <windows.h>
#include <msctf.h>

namespace modian::tsf {
	class tsf_text_service : public ITfTextInputProcessor {
	public:
		STDMETHODIMP Activate(ITfThreadMgr* p_thread_mgr, TfClientId tf_client_id) override;
		STDMETHODIMP Deactivate() override;

		STDMETHODIMP QueryInterface(REFIID riid, void** ppv_obj) override;
		STDMETHODIMP_(ULONG) AddRef() override;
		STDMETHODIMP_(ULONG) Release() override;
	private:
        ULONG ref_count_;
        TfClientId client_id_ = TF_CLIENTID_NULL;
        ITfThreadMgr* thread_mgr_ = nullptr;
	};
}

#endif //TSF_TEXT_SERVICE_H
