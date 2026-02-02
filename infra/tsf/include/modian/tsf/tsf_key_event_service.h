#pragma once

#include <msctf.h>
#include <string>
#include <memory>
#include <atomic>

#include "modian/tsf/utils/utils.h"
#include "modian/core/protocol/composition_protocol.h"
#include "modian/ipc/ipc_client.h"

namespace modian::brush::infra::tsf {
	constexpr auto parse_content = [](const auto& p){ return utils::utf8_to_wstring(p.payload); };
	constexpr auto parse_commit_flag = [](const auto& p){ return p.type == core::protocol::composition_protocol::message_type::COMMIT; };

	class tsf_key_event_service final : public ITfKeyEventSink, public ITfCompositionSink {
	public:
		explicit tsf_key_event_service();
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

		std::atomic<ULONG> ref_count_;

		std::wstring input_pinyin_;
		TfClientId client_id_ = TF_CLIENTID_NULL;

		std::shared_ptr<ipc::ipc_client> ipc_client_;

	public:
		ITfComposition* current_composition_{nullptr};
	};
}