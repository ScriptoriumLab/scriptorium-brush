#include "scriptorium/tsf/tsf_edit_session.h"

#include <wrl/client.h>

#include "scriptorium/tsf/utils/utils.h"
#include "scriptorium/tsf/dll/info/registry_info.h"

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
		if (FAILED(service_->current_composition_->GetRange(&p_range))) {
			service_->current_composition_->Release();
			service_->current_composition_ = nullptr;
		} else {
			p_range->Release();
		}
	}

	bool tsf_edit_session::_ensure_active_composition(const TfEditCookie ec) {
		if (service_->current_composition_) return true;

		if (candidate_info_.word.empty()) return false;

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

		std::wstring display_text;
		std::vector<LONG> segment_lengths;

		if (is_commit_) {
			display_text = utils::utf8_to_wstring(candidate_info_.word);
		} else {
			const auto& path = candidate_info_.spelling_path;
			if (!path.empty()) {
				for (size_t i = 0; i < path.size(); ++i) {
					std::wstring segment = utils::utf8_to_wstring(path[i]);
					if (segment.empty()) continue;

					if (i > 0) {
						display_text.push_back(L'\'');
					}
					
					display_text += segment;
					segment_lengths.push_back(static_cast<LONG>(segment.length()));
				}
			} else {
				display_text = utils::utf8_to_wstring(candidate_info_.word);
			}
		}

		if (display_text.empty()) {
			p_range->SetText(ec, 0, L"", 0);
			service_->current_composition_->EndComposition(ec);
			if (service_->current_composition_) {
				service_->current_composition_->Release();
				service_->current_composition_ = nullptr;
			}
		} else {
			p_range->SetText(ec, 0, display_text.c_str(), static_cast<LONG>(display_text.length()));
			if (!is_commit_ && !segment_lengths.empty()) {
				_apply_display_attributes(ec, p_range, segment_lengths);
			}

			p_range->Collapse(ec, TF_ANCHOR_END);
			TF_SELECTION sel = {0};
			sel.range = p_range;
			context_->SetSelection(ec, 1, &sel);
		}

		p_range->Release();
	}

    void tsf_edit_session::_apply_display_attributes(const TfEditCookie ec, ITfRange* composition_range, const std::vector<LONG>& segment_lengths) {
		ITfProperty* p_prop = nullptr;
		if (FAILED(context_->GetProperty(GUID_PROP_ATTRIBUTE, &p_prop))) {
			return;
		}

		TfGuidAtom attr_atom = TF_INVALID_GUIDATOM;
        Microsoft::WRL::ComPtr<ITfCategoryMgr> category_mgr;
		if (SUCCEEDED(CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&category_mgr)))) {
			category_mgr->RegisterGUID(dll::info::SCRIPTORIUM_IME_GUID_DISPLAY_ATTRIBUTE_INPUT, &attr_atom);
		}

		if (attr_atom == TF_INVALID_GUIDATOM) {
			p_prop->Release();
			return;
		}

		LONG current_offset = 0;

		for (size_t i = 0; i < segment_lengths.size(); ++i) {
			LONG seg_len = segment_lengths[i];

			ITfRange* p_segment_range = nullptr;
			if (SUCCEEDED(composition_range->Clone(&p_segment_range))) {
				p_segment_range->Collapse(ec, TF_ANCHOR_START);

				LONG cch_shifted = 0;
				p_segment_range->ShiftStart(ec, current_offset, &cch_shifted, nullptr);
				p_segment_range->ShiftEnd(ec, seg_len, &cch_shifted, nullptr);

				VARIANT var;
				VariantInit(&var);
				var.vt = VT_I4;
				var.lVal = static_cast<LONG>(attr_atom);

				p_prop->SetValue(ec, p_segment_range, &var);

				p_segment_range->Release();
			}

			current_offset += seg_len + 1; 
		}

		p_prop->Release();
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
