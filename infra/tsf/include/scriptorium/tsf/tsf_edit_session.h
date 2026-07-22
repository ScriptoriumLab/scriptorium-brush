#pragma once

#include <msctf.h>
#include <string>
#include <atomic>

#include "utils/utils.h"
#include "tsf_key_event_service.h"

namespace scriptorium::brush::infra::tsf {
	class tsf_edit_session final : public ITfEditSession {
	public:
		tsf_edit_session(ITfContext* context, tsf_key_event_service* service, const std::string& text, bool is_commit)
			: context_{context}, text_{utils::utf8_to_wstring(text)}, backspace_count_{0}, service_{service}, is_commit_{is_commit} {
			context_->AddRef();
			service_->AddRef();
		}

		virtual ~tsf_edit_session() {
			context_->Release();
		}

		STDMETHODIMP DoEditSession(TfEditCookie ec) override;

		STDMETHODIMP QueryInterface(const IID& riid, void** ppv_object) override;
		STDMETHODIMP_(ULONG) AddRef() override;
		STDMETHODIMP_(ULONG) Release() override;

	private:
		std::atomic<ULONG> ref_count_;
		ITfContext* context_;
		std::wstring text_;
		size_t backspace_count_;
		tsf_key_event_service* service_;
		bool is_commit_;

		void _cleanup_zombie_composition();
		bool _ensure_active_composition(TfEditCookie ec);
		void _update_composition_text(TfEditCookie ec);
		void _commit_and_destroy(TfEditCookie ec);
	};
}