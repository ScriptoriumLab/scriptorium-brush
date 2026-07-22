#include "scriptorium/tsf/tsf_edit_session.h"

namespace scriptorium::brush::infra::tsf {
	STDMETHODIMP tsf_edit_session::DoEditSession(TfEditCookie ec) {
		if (!context_ || !service_) return E_FAIL;

		_cleanup_zombie_composition();

		if (!_ensure_active_composition(ec)) {
			return S_OK;
		}

		_update_composition_text(ec);

		if (is_commit_) {
			_commit_and_destroy(ec);
		}

		return S_OK;
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

	void tsf_edit_session::_cleanup_zombie_composition() {
		if (!service_->current_composition_) return;

		ITfRange* p_range = nullptr;
		// 试探性获取 Range，失败说明是僵尸
		if (FAILED(service_->current_composition_->GetRange(&p_range))) {
			service_->current_composition_->Release();
			service_->current_composition_ = nullptr;
		} else {
			// 记得 Release 试探用的指针
			p_range->Release();
		}
	}

	bool tsf_edit_session::_ensure_active_composition(const TfEditCookie ec) {
		if (service_->current_composition_) return true;

		if (text_.empty()) return false;

		TF_SELECTION tf_selection;
		ULONG c_fetched;
		if (FAILED(context_->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tf_selection, &c_fetched))) {
			return false;
		}

		ITfContextComposition* p_context_comp = nullptr;
		bool success = false;

		if (SUCCEEDED(context_->QueryInterface(IID_ITfContextComposition, (void**)&p_context_comp))) {
			HRESULT hr = p_context_comp->StartComposition(
				ec, tf_selection.range, service_, &service_->current_composition_);

			if (SUCCEEDED(hr) && service_->current_composition_) {
				success = true;
			}
			p_context_comp->Release();
		}

		tf_selection.range->Release();

		return success;
	}

	void tsf_edit_session::_update_composition_text(const TfEditCookie ec) {
		if (!service_->current_composition_) return;

		ITfRange* p_range = nullptr;
		if (FAILED(service_->current_composition_->GetRange(&p_range))) return;

		if (text_.empty()) {
			p_range->SetText(ec, 0, L"", 0);

			service_->current_composition_->EndComposition(ec);

			if (service_->current_composition_) {
				service_->current_composition_->Release();
				service_->current_composition_ = nullptr;
			}
		} else {
			p_range->SetText(ec, 0, text_.c_str(), static_cast<LONG>(text_.length()));

			p_range->Collapse(ec, TF_ANCHOR_END);
			TF_SELECTION sel = {0};
			sel.range = p_range;
			context_->SetSelection(ec, 1, &sel);
		}

		p_range->Release();
	}

	void tsf_edit_session::_commit_and_destroy(const TfEditCookie ec) {
		if (!service_->current_composition_) return;

		service_->current_composition_->EndComposition(ec);

		if (service_->current_composition_) {
			service_->current_composition_->Release();
			service_->current_composition_ = nullptr;
		}
	}
}
