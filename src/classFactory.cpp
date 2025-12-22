#include <windows.h>
#include "syntherceptor.h"

static long g_lockCount = 0;

class ClassFactory : public IClassFactory {
	public:
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) {
		if (riid == IID_IUnknown || riid == IID_IClassFactory) {
			*ppv = this;
			AddRef();
			return S_OK;
		}
		*ppv = nullptr;
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) AddRef() {
		return 2;
	}

	STDMETHODIMP_(ULONG) Release() {
		return 1;
	}

	STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv) {
		if (pUnkOuter)
			return CLASS_E_NOAGGREGATION;
		auto* engine = new Syntherceptor();
		return engine->QueryInterface(riid, ppv);
	}

	STDMETHODIMP LockServer(BOOL lock) {
		if (lock)
			InterlockedIncrement(&g_lockCount);
		else
			InterlockedDecrement(&g_lockCount);
		return S_OK;
	}
};

STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv) {
	static const CLSID CLSID_Syntherceptor = {
		0x12345678, 0x1234, 0x1234, {0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34}
	};

	if (clsid != CLSID_Syntherceptor)
		return CLASS_E_CLASSNOTAVAILABLE;

	static ClassFactory factory;
	return factory.QueryInterface(riid, ppv);
}

STDAPI DllCanUnloadNow() {
	return g_lockCount == 0 ? S_OK : S_FALSE;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) {
	return TRUE;
}
