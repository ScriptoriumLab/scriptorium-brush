#pragma once

#include <Unknwn.h>
#include <atomic>

namespace modian::infra::tsf {
	extern std::atomic<long> g_server_lock;
	extern std::atomic<long> g_active_objects;

	class class_factory final : public IClassFactory {
	public:
		class_factory() : m_ref_count{1} {}
		virtual ~class_factory() = default;

		// IUnknown
		STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
		STDMETHODIMP_(ULONG) AddRef() override;
		STDMETHODIMP_(ULONG) Release() override;

		// IClassFactory
		STDMETHODIMP CreateInstance(IUnknown *p_unk_outer, REFIID riid, void **ppv) override;
		STDMETHODIMP LockServer(BOOL f_lock) override;

	private:
		std::atomic<ULONG> m_ref_count;
	};
}