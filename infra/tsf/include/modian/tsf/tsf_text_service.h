#pragma once

#include <memory>
#include <windows.h>
#include <msctf.h>

#include "tsf_key_event_service.h"

namespace modian::infra::tsf {
	class tsf_text_service final : public ITfTextInputProcessor {
	public:
		tsf_text_service() = default;
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
		manager::candidate_manager candidate_manager_{};
		manager::engine_manager engine_manager_{candidate_manager_};
		tsf_key_event_service key_event_service_{engine_manager_};
	};
}
