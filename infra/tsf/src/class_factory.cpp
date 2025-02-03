#include "modian/tsf/class_factory.h"
#include "modian/tsf/tsf_text_service.h"
#include <wil/com.h>

STDMETHODIMP class_factory::QueryInterface(const IID& riid, void** ppv) {
	if (ppv == nullptr) {
		return E_POINTER;
	}

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory)) {
		*ppv = static_cast<IClassFactory*>(this);
		AddRef();
		return S_OK;
	}
	*ppv = nullptr;
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) class_factory::AddRef() {
	return 2;
}

STDMETHODIMP_(ULONG) class_factory::Release() {
	return 1;
}

STDMETHODIMP class_factory::CreateInstance(IUnknown* p_unk_outer, const IID& riid, void** ppv) {
	if (p_unk_outer) return CLASS_E_NOAGGREGATION;

	const wil::com_ptr_nothrow service = new (std::nothrow) modian::tsf::tsf_text_service();
	if (!service) return E_OUTOFMEMORY;

	InterlockedIncrement(&g_active_objects);
	HRESULT hr = service->QueryInterface(riid, ppv);
	if (FAILED(hr)) InterlockedDecrement(&g_active_objects);

	return hr;
}

STDMETHODIMP class_factory::LockServer(BOOL f_lock) {
	if (f_lock) {
		InterlockedIncrement(&g_server_lock);
	} else {
		InterlockedDecrement(&g_server_lock);
	}

	return S_OK;
}
