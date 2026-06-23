#include "modian/tsf/tsf_key_event_service.h"

#include <windows.h>

#include "modian/tsf/tsf_edit_session.h"
#include "modian/common/core/logger/logger_service.h"

#include "modian/common/core/protocol/v1/input/key_event.h"
#include "modian/common/service/protocol/input_protocol_service.h"

#include "modian/common/infra/ipc/ipc_client_factory.h"

namespace modian::brush::infra::tsf {
	const std::string INPUT_PROTOCOL_PIPE_NAME = R"(\\.\pipe\modian_input_protocol_pipe)";

    tsf_key_event_service::tsf_key_event_service(IUnknown* owner)
        : owner_{owner},
          input_protocol_ipc_client_{modian::common::infra::ipc::ipc_client_factory::create_sync_ipc_client(INPUT_PROTOCOL_PIPE_NAME)} {}

    bool tsf_key_event_service::_is_key_supported(const WPARAM vk_code) {
        return (vk_code >= 'A' && vk_code <= 'Z') || (vk_code == VK_BACK) || (vk_code == VK_SPACE) || (vk_code == VK_LEFT) || (vk_code == VK_RIGHT);
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

        if (w_param == VK_BACK && current_composition_ == nullptr) {
            *pf_eaten = FALSE;
            return S_OK;
        }

        if (_is_key_supported(w_param)) {
            common::core::logger_service::logger()->info("Key intercepted: {}", static_cast<char>(w_param));

            const auto key_event = common::core::protocol::input::v1::key_event::from_os_key(w_param);
            const std::string req_data = common::service::input_protocol_service::build_key_event_request(key_event);
            const std::string response = input_protocol_ipc_client_->sync_send(req_data);
            const auto instruction = common::service::input_protocol_service::parse_instruction_response(response);
            const auto content = instruction.payload;
            const auto is_commit = instruction.type == common::core::protocol::input::v1::message_type::COMMIT;

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
        if (owner_) {
            return owner_->AddRef();
        }
        return 0;
    }

    STDMETHODIMP_(ULONG) tsf_key_event_service::Release() {
        if (owner_) {
            return owner_->Release();
        }
        return 0;
    }

    STDMETHODIMP tsf_key_event_service::OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition* pComposition) {
        if (current_composition_ == pComposition) {
            current_composition_ = nullptr;
        }

        return S_OK;
    }
}
