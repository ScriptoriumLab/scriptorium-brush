#include "modian/tsf/dll/register.h"

#include <msctf.h>
#include <modian/info/registry_info.h>
#include <iostream>

modian::tsf::dll::auto_com::auto_com() : b_init{false} {
	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr)) return;
	b_init = true;
}

modian::tsf::dll::auto_com::~auto_com() {
	if (b_init) {
		CoUninitialize();
	}
}
