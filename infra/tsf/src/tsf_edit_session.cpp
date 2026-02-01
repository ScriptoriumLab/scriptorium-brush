#include "modian/tsf/tsf_edit_session.h"
#include "modian/core/logger/logger_service.h"

namespace modian::brush::infra::tsf {
	STDMETHODIMP tsf_edit_session::DoEditSession(TfEditCookie ec) {
		if (!context_ || !service_) return E_FAIL;

		ITfRange* p_range = nullptr;
		HRESULT hr = S_OK;

		// Zombie Check
		if (service_->current_composition_) {
			hr = service_->current_composition_->GetRange(&p_range);
			if (FAILED(hr)) {
				service_->current_composition_->Release();
				service_->current_composition_ = nullptr;
				p_range = nullptr;
			}
		}

		// Start Transaction
		if (service_->current_composition_ == nullptr) {
			if (text_.empty()) return S_OK;

			TF_SELECTION tf_selection;
			ULONG c_fetched;
			// Get cursor start point
			if (SUCCEEDED(context_->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tf_selection, &c_fetched))) {
				ITfContextComposition* p_context_comp = nullptr;
				if (SUCCEEDED(context_->QueryInterface(IID_ITfContextComposition, reinterpret_cast<void**>(&p_context_comp)))) {
					hr = p_context_comp->StartComposition(
						ec,
						tf_selection.range,
						service_,
						&service_->current_composition_
					);

					p_context_comp->Release();
				}
				// Get new range
				if (SUCCEEDED(hr) && service_->current_composition_) {
					service_->current_composition_->GetRange(&p_range);
				}
				tf_selection.range->Release();
			}
		}

		// Update Buffer
		if (SUCCEEDED(hr) && p_range) {
			hr = p_range->SetText(ec, 0, text_.c_str(), static_cast<LONG>(text_.length()));

			p_range->Collapse(ec, TF_ANCHOR_END);
			TF_SELECTION sel{nullptr};
			sel.range = p_range;
			context_->SetSelection(ec, 1, &sel);

			// Commit Transaction
			if (is_commit_ && service_->current_composition_) {
				// Trigger tsf event service's OnCompositionTerminated
				service_->current_composition_->EndComposition(ec);

				if (service_->current_composition_) {
					service_->current_composition_->Release(); // 释放引用计数
					service_->current_composition_ = nullptr;  // 彻底切断联系
				}
			}

			p_range->Release();
		}

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
