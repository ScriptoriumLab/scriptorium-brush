#ifndef CLASS_FACTORY_H
#define CLASS_FACTORY_H

#include <Unknwn.h>

extern volatile long g_server_lock;
extern volatile long g_active_objects;

class class_factory final : public IClassFactory {
public:
	virtual ~class_factory() = default;
	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // IClassFactory
    STDMETHODIMP CreateInstance(IUnknown *p_unk_outer, REFIID riid, void **ppv) override;
    STDMETHODIMP LockServer(BOOL f_lock) override;
};

#endif //CLASS_FACTORY_H
