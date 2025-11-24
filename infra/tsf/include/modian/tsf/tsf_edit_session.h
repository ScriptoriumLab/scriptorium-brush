#pragma once

#include <msctf.h>
#include <string>
#include <atomic>

namespace modian::brush::infra::tsf {
	class tsf_edit_session final : public ITfEditSession {
	public:
		tsf_edit_session(ITfContext* context, std::wstring text, size_t backspace_count = 0)
			: ref_count_(1), context_(context), text_(std::move(text)), backspace_count_(backspace_count) {
			context_->AddRef();
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
	};
}