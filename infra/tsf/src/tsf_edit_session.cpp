#include "modian/tsf/tsf_edit_session.h"
#include "modian/core/logger/logger_service.h"

namespace modian::brush::infra::tsf {

	STDMETHODIMP tsf_edit_session::DoEditSession(TfEditCookie ec) {
		TF_SELECTION tfSelection;
		ULONG cFetched;
		if (FAILED(context_->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched)) || cFetched != 1) {
			return E_FAIL;
		}

		ITfRange* range = tfSelection.range;

		HRESULT hr = range->SetText(ec, 0, text_.data(), static_cast<LONG>(text_.size()));

		if (SUCCEEDED(hr)) {
			range->Collapse(ec, TF_ANCHOR_END);

			TF_SELECTION sel;
			sel.range = range;
			sel.style.ase = TF_AE_NONE;
			sel.style.fInterimChar = FALSE;

			context_->SetSelection(ec, 1, &sel);
		} else {
			core::logger_service::logger()->error("Failed to insert text");
		}

		range->Release();
		return hr;
	}

	STDMETHODIMP tsf_edit_session::QueryInterface(const IID& riid, void** ppv_object) {
		if (!ppv_object) return E_POINTER;
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfEditSession)) {
			*ppv_object = static_cast<ITfEditSession*>(this);
			AddRef();
			return S_OK;
		}
		*ppv_object = nullptr;
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) tsf_edit_session::AddRef() {
		return ++ref_count_;
	}

	STDMETHODIMP_(ULONG) tsf_edit_session::Release() {
		const ULONG count = --ref_count_;
		if (count == 0) delete this;
		return count;
	}
}