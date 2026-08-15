#pragma once

#include <msctf.h>
#include <atomic>

namespace scriptorium::brush::infra::tsf {
	class tsf_display_attribute_info final : public ITfDisplayAttributeInfo {
	public:
		tsf_display_attribute_info();
		virtual ~tsf_display_attribute_info() = default;

		// IUnknown
		STDMETHODIMP QueryInterface(REFIID riid, void** ppvObj) override;
		STDMETHODIMP_(ULONG) AddRef() override;
		STDMETHODIMP_(ULONG) Release() override;

		// ITfDisplayAttributeInfo
		STDMETHODIMP GetGUID(GUID* pguid) override;
		STDMETHODIMP GetDescription(BSTR* pbstrDesc) override;
		STDMETHODIMP GetAttributeInfo(TF_DISPLAYATTRIBUTE* pda) override;
		STDMETHODIMP SetAttributeInfo(const TF_DISPLAYATTRIBUTE* pda) override;
		STDMETHODIMP Reset() override;

	private:
		std::atomic<ULONG> ref_count_{1};
	};
}
