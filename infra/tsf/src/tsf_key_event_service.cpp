#include "modian/tsf/tsf_key_event_service.h"

#include <cwctype>

#include "modian/core/logger/logger_service.h"

namespace modian::brush::infra::tsf {
    tsf_key_event_service::tsf_key_event_service(manager::engine_manager engine_manager)
        : ref_count_{1}, engine_manager_{std::move(engine_manager)} {}

    bool tsf_key_event_service::_is_key_supported(const WPARAM vk_code) {
        return (vk_code >= 'A' && vk_code <= 'Z');
    }

    STDMETHODIMP tsf_key_event_service::OnTestKeyDown(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
        if (!pf_eaten) return E_POINTER;

        if (_is_key_supported(w_param)) {
            *pf_eaten = TRUE;
        } else {
            *pf_eaten = FALSE;
        }
        return S_OK;
    }

    STDMETHODIMP tsf_key_event_service::OnKeyDown(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
        if (!pf_eaten) return E_POINTER;

        if (_is_key_supported(w_param)) {
            core::logger_service::logger()->info("Key intercepted: {}", static_cast<char>(w_param));

            const auto lower_char = std::towlower(static_cast<wchar_t>(w_param));

            engine_manager_.update_input_state(lower_char);

            *pf_eaten = TRUE;
        } else {
            *pf_eaten = FALSE;
        }

        return S_OK;
    }

    STDMETHODIMP tsf_key_event_service::OnKeyUp(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
        *pf_eaten = FALSE;
        return S_OK;
    }

    STDMETHODIMP tsf_key_event_service::OnTestKeyUp(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) {
        *pf_eaten = FALSE;
        return S_OK;
    }

    STDMETHODIMP tsf_key_event_service::OnPreservedKey(ITfContext* pic, const GUID& r_guid, BOOL* pf_eaten) {
        *pf_eaten = FALSE;
        return S_OK;
    }

    HRESULT tsf_key_event_service::OnSetFocus(BOOL f_foreground) {
        return S_OK;
    }

    STDMETHODIMP tsf_key_event_service::QueryInterface(const IID& riid, void** ppv_object) {
        if (!ppv_object) return E_POINTER;

        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfKeyEventSink)) {
            *ppv_object = static_cast<ITfKeyEventSink*>(this);
            AddRef();
            return S_OK;
        }

        *ppv_object = nullptr;
        return E_NOINTERFACE;
    }

    STDMETHODIMP_(ULONG) tsf_key_event_service::AddRef() {
        return ++ref_count_;
    }

    STDMETHODIMP_(ULONG) tsf_key_event_service::Release() {
        const ULONG count = --ref_count_;
        if (count == 0) {
            delete this;
        }
        return count;
    }
}