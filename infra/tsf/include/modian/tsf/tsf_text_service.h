#pragma once

#include <memory>
#include <windows.h>
#include <msctf.h>

#include "class_factory.h"
#include "tsf_key_event_service.h"

namespace modian::brush::infra::tsf {
	class tsf_text_service final : public ITfTextInputProcessor {
	public:
		tsf_text_service() {
			++g_active_objects;
		}
		virtual ~tsf_text_service();
		STDMETHODIMP Activate(ITfThreadMgr* p_thread_mgr, TfClientId tf_client_id) override;
		STDMETHODIMP Deactivate() override;

		STDMETHODIMP QueryInterface(REFIID riid, void** ppv_obj) override;
		STDMETHODIMP_(ULONG) AddRef() override;
		STDMETHODIMP_(ULONG) Release() override;
	private:
		std::atomic<bool> is_active_{false};
		std::atomic<ULONG> ref_count_{1};
        TfClientId client_id_ = TF_CLIENTID_NULL;
        ITfThreadMgr* thread_mgr_ = nullptr;
		std::shared_ptr<manager::candidate_manager> candidate_manager_ = std::make_shared<manager::candidate_manager>();
		manager::engine_manager engine_manager_{candidate_manager_};
		tsf_key_event_service key_event_service_{engine_manager_};
	};
}
