#include "modian/tsf/tsf_key_event_service.h"

modian::tsf::tsf_key_event_service::tsf_key_event_service() : ref_count_{1} {}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnKeyDown(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
	*pf_eaten = FALSE;
	return S_OK;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnKeyUp(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
	*pf_eaten = FALSE;
	return S_OK;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnTestKeyDown(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
	*pf_eaten = FALSE;
	return S_OK;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnTestKeyUp(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
	*pf_eaten = FALSE;
	return S_OK;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::OnPreservedKey(ITfContext* pic, const GUID& r_guid, BOOL* pf_eaten) {
	*pf_eaten = FALSE;
	return S_OK;
}

HRESULT modian::tsf::tsf_key_event_service::OnSetFocus(BOOL f_foreground) {
	return S_OK;
}

STDMETHODIMP modian::tsf::tsf_key_event_service::QueryInterface(const IID& riid, void** ppv_object) {
	if (!ppv_object) return E_POINTER;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfKeyEventSink)) {
		*ppv_object = static_cast<ITfKeyEventSink*>(this);
		AddRef();
		return S_OK;
	}

	*ppv_object = nullptr;
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) modian::tsf::tsf_key_event_service::AddRef() {
	return InterlockedIncrement(&ref_count_);
}

STDMETHODIMP_(ULONG) modian::tsf::tsf_key_event_service::Release() {
	const ULONG count = InterlockedDecrement(&ref_count_);
	if (count == 0) {
		delete this;
	}
	return count;
}
