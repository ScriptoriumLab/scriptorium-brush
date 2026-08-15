#pragma once

#include <memory>
#include <atomic>
#include <windows.h>
#include <msctf.h>

#include "class_factory.h"
#include "tsf_key_event_service.h"

namespace scriptorium::brush::manager {
	class candidate_manager;
	class engine_manager;
}

namespace scriptorium::brush::infra::tsf {
	class tsf_text_service final : public ITfTextInputProcessor, public ITfDisplayAttributeProvider {
	public:
		tsf_text_service();
		virtual ~tsf_text_service();

		STDMETHODIMP Activate(ITfThreadMgr* p_thread_mgr, TfClientId tf_client_id) override;
		STDMETHODIMP Deactivate() override;
		STDMETHODIMP QueryInterface(REFIID riid, void** ppv_obj) override;
		STDMETHODIMP_(ULONG) AddRef() override;
		STDMETHODIMP_(ULONG) Release() override;

        STDMETHODIMP EnumDisplayAttributeInfo(IEnumTfDisplayAttributeInfo** ppEnum) override;
		STDMETHODIMP GetDisplayAttributeInfo(REFGUID guid, ITfDisplayAttributeInfo** ppInfo) override;
	private:
		std::atomic<bool> is_active_{false};
		std::atomic<ULONG> ref_count_{1};
		TfClientId client_id_ = TF_CLIENTID_NULL;
		ITfThreadMgr* thread_mgr_ = nullptr;

		tsf_key_event_service key_event_service_;
	};
}
