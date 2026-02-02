#include "modian/tsf/tsf_key_event_service.h"

#include <cwctype>
#include <windows.h>
#include <modian/core/protocol/composition_protocol.h>

#include "modian/tsf/tsf_edit_session.h"
#include "modian/core/logger/logger_service.h"

namespace modian::brush::infra::tsf {
    tsf_key_event_service::tsf_key_event_service()
        : ref_count_{1},
          ipc_client_{std::make_shared<ipc::ipc_client>()} {}

    bool tsf_key_event_service::_is_key_supported(const WPARAM vk_code) {
        return (vk_code >= 'A' && vk_code <= 'Z') || (vk_code == VK_BACK);
    }

    // TODO: move to utils
    std::wstring utf8_to_wstring(const std::string& str) {
        if (str.empty()) return {};
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
        return wstrTo;
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

        const bool is_backspace = (w_param == VK_BACK);

        if (is_backspace && current_composition_ == nullptr) {
            *pf_eaten = FALSE;
            return S_OK;
        }

        if (_is_key_supported(w_param)) {
            core::logger_service::logger()->info("Key intercepted: {}", static_cast<char>(w_param));

            std::string req_data;
            if (is_backspace) {
                req_data = "\b";
            } else {
                req_data = std::string(1, static_cast<char>(w_param));
            }

            const std::string response = ipc_client_->send_and_wait(req_data);
            const auto protocol = core::protocol::composition_protocol::decode(response);
            const auto content = utf8_to_wstring(protocol.payload);
            const bool is_commit = protocol.type == core::protocol::composition_protocol::message_type::COMMIT;

            if (pic != nullptr && client_id_ != TF_CLIENTID_NULL) {
                if (!content.empty() || current_composition_) {
                    auto* session = new tsf_edit_session(pic, this, content, is_commit);
                    HRESULT hr = S_OK;
                    pic->RequestEditSession(client_id_, session, TF_ES_READWRITE | TF_ES_ASYNCDONTCARE, &hr);
                    session->Release();
                }
            }

            *pf_eaten = TRUE;
            return S_OK;
        }

        *pf_eaten = FALSE;
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

        } else if (IsEqualIID(riid, IID_ITfCompositionSink)) {
            *ppv_object = static_cast<ITfCompositionSink*>(this);
        } else {
            *ppv_object = nullptr;
            return E_NOINTERFACE;
        }

        AddRef();
        return S_OK;
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

    STDMETHODIMP tsf_key_event_service::OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition* pComposition) {
        if (current_composition_ == pComposition) {
            current_composition_ = nullptr;
        }

        return S_OK;
    }
}
