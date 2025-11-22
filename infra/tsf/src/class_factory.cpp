#include "modian/tsf/class_factory.h"

#include <new>
#include <wrl/client.h>

#include "modian/core/logger/logger_service.h"
#include "modian/tsf/tsf_text_service.h"

using Microsoft::WRL::ComPtr;

namespace modian::brush::infra::tsf {
    std::atomic<long> g_server_lock{0};
    std::atomic<long> g_active_objects{0};

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
        return ++m_ref_count;
    }

    STDMETHODIMP_(ULONG) class_factory::Release() {
        const ULONG count = --m_ref_count;
        if (count == 0) {
            delete this;
        }
        return count;
    }

    STDMETHODIMP class_factory::CreateInstance(IUnknown* p_unk_outer, const IID& riid, void** ppv) {
        core::logger_service::logger()->info("Creating class_factory instance...");

        if (p_unk_outer) return CLASS_E_NOAGGREGATION;
        if (!ppv) return E_POINTER;

        *ppv = nullptr;

        auto* p_service = new (std::nothrow) tsf_text_service();
        if (!p_service) return E_OUTOFMEMORY;

        const HRESULT hr = p_service->QueryInterface(riid, ppv);

        p_service->Release();

        return hr;
    }

    STDMETHODIMP class_factory::LockServer(BOOL f_lock) {
        if (f_lock) {
            ++g_server_lock;
        } else {
            --g_server_lock;
        }

        return S_OK;
    }
}