#include "modian/tsf/tsf_key_event_service.h"

#include <cwctype>
#include <windows.h>

#include "modian/tsf/tsf_edit_session.h"
#include "modian/core/logger/logger_service.h"

namespace modian::brush::infra::tsf {
    tsf_key_event_service::tsf_key_event_service(std::shared_ptr<manager::engine_manager> engine_manager)
        : ref_count_{1},
          engine_manager_{std::move(engine_manager)},
          ipc_client_{std::make_shared<ipc::ipc_client>()} {}

    bool tsf_key_event_service::_is_key_supported(const WPARAM vk_code) {
        return (vk_code >= 'A' && vk_code <= 'Z');
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

        if (w_param == VK_BACK) {
            core::logger_service::logger()->info("Brush: Sending Backspace to Inkstone");
            ipc_client_->send(std::string(1, '\b'));

            if (pinyin_len_ > 0) {
                pinyin_len_--;
            }

            *pf_eaten = FALSE;
            return S_OK;
        }

        if (_is_key_supported(w_param)) {
            core::logger_service::logger()->info("Key intercepted: {}", static_cast<char>(w_param));

            const auto lower_char = std::towlower(static_cast<wchar_t>(w_param));

            std::string msg(1, static_cast<char>(lower_char)); // 简单转换，假定 ASCII
            auto response = ipc_client_->send_and_wait(msg);

            // TODO: should remove engine manager later
            engine_manager_->update_input_state(lower_char);

            if (pic != nullptr && client_id_ != TF_CLIENTID_NULL) {
                std::wstring text_to_insert(1, lower_char);
                size_t backspace_count = 0;

                if (!response.empty()) {
                    core::logger_service::logger()->info("Inkstone returned candidate: {}", response);

                    text_to_insert = utf8_to_wstring(response);

                    backspace_count = pinyin_len_;

                    pinyin_len_ = 0;
                } else {
                    text_to_insert = std::wstring(1, lower_char);

                    backspace_count = 0;

                    pinyin_len_++;
                }

                auto* session = new tsf_edit_session(pic, text_to_insert, backspace_count);

                HRESULT hr = S_OK;
                pic->RequestEditSession(client_id_, session, TF_ES_READWRITE | TF_ES_ASYNCDONTCARE, &hr);
                session->Release();
            }

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