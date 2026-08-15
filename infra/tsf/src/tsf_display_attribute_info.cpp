#include "scriptorium/tsf/tsf_display_attribute_info.h"

#include "scriptorium/tsf/dll/info/registry_info.h"

namespace scriptorium::brush::infra::tsf {

	tsf_display_attribute_info::tsf_display_attribute_info() = default;

	STDMETHODIMP tsf_display_attribute_info::QueryInterface(REFIID riid, void** ppvObj) {
		if (!ppvObj) return E_POINTER;
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfDisplayAttributeInfo)) {
			*ppvObj = static_cast<ITfDisplayAttributeInfo*>(this);
			AddRef();
			return S_OK;
		}
		*ppvObj = nullptr;
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) tsf_display_attribute_info::AddRef() {
		return ++ref_count_;
	}

	STDMETHODIMP_(ULONG) tsf_display_attribute_info::Release() {
		const ULONG count = --ref_count_;
		if (count == 0) delete this;
		return count;
	}

	STDMETHODIMP tsf_display_attribute_info::GetGUID(GUID* pguid) {
		if (!pguid) return E_INVALIDARG;
		*pguid = dll::info::SCRIPTORIUM_IME_GUID_DISPLAY_ATTRIBUTE_INPUT;
		return S_OK;
	}

	STDMETHODIMP tsf_display_attribute_info::GetDescription(BSTR* pbstrDesc) {
		if (!pbstrDesc) return E_INVALIDARG;
		*pbstrDesc = SysAllocString(L"Scriptorium Input Pinyin Underline");
		return *pbstrDesc ? S_OK : E_OUTOFMEMORY;
	}

	STDMETHODIMP tsf_display_attribute_info::GetAttributeInfo(TF_DISPLAYATTRIBUTE* pda) {
		if (!pda) return E_INVALIDARG;

		pda->crText.type = TF_CT_NONE;
		pda->crBk.type = TF_CT_NONE;
		// line style：can be TF_LS_SOLID, TF_LS_DOT, TF_LS_SQUIGGLE
		pda->lsStyle = TF_LS_DOT;
		pda->fBoldLine = FALSE;
		pda->crLine.type = TF_CT_COLORREF;
		pda->crLine.cr = RGB(255, 255, 255);
		pda->bAttr = TF_ATTR_INPUT;

		return S_OK;
	}

	STDMETHODIMP tsf_display_attribute_info::SetAttributeInfo(const TF_DISPLAYATTRIBUTE* pda) {
		return E_NOTIMPL;
	}

	STDMETHODIMP tsf_display_attribute_info::Reset() {
		return E_NOTIMPL;
	}
}
