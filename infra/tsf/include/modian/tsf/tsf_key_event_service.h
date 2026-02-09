#pragma once

#include <msctf.h>
#include <string>
#include <memory>

#include "modian/core/protocol/instruction.h"
#include "modian/ipc/input_protocol_pipe_client.h"

namespace modian::brush::infra::tsf {
	constexpr auto parse_content = [](const auto& p){ return p.payload; };
	constexpr auto parse_commit_flag = [](const auto& p){ return p.type == core::protocol::input::v1::message_type::COMMIT; };

	class tsf_key_event_service final : public ITfKeyEventSink, public ITfCompositionSink {
	public:
		explicit tsf_key_event_service(IUnknown* owner);
		virtual ~tsf_key_event_service() = default;

		void set_client_id(const TfClientId id) { client_id_ = id; }

		// ITfKeyEventSink interface
		STDMETHODIMP OnKeyDown(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) override;
		STDMETHODIMP OnKeyUp(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) override;
		STDMETHODIMP OnTestKeyDown(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) override;
		STDMETHODIMP OnTestKeyUp(ITfContext* pic, WPARAM w_param, LPARAM l_param, BOOL* pf_eaten) override;

		STDMETHODIMP OnPreservedKey(ITfContext* pic, const GUID& r_guid, BOOL* pf_eaten) override;
		STDMETHODIMP OnSetFocus(BOOL f_foreground) override;

		// IUnknown interface
		STDMETHODIMP QueryInterface(const IID& riid, void** ppv_object) override;
		STDMETHODIMP_(ULONG) AddRef() override;
		STDMETHODIMP_(ULONG) Release() override;

		STDMETHODIMP OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition* pComposition) override;

	private:
		[[nodiscard]] static bool _is_key_supported(WPARAM vk_code);

		IUnknown* owner_{nullptr};

		std::wstring input_pinyin_;
		TfClientId client_id_ = TF_CLIENTID_NULL;

		std::shared_ptr<ipc::input_protocol_pipe_client> input_protocol_pipe_client_;

	public:
		ITfComposition* current_composition_{nullptr};
	};
}