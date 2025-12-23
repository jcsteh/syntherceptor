/*
 * Syntherceptor
 * Class factory code
 * Author: James Teh <jamie@jantrid.net>
 * Copyright 2025 James Teh
 * License: GNU General Public License version 2.0
 */

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
		// a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35
		0xa2f5f730, 0xcf28, 0x4ac7, {0x8f, 0x7f, 0x0e, 0x69, 0xa6, 0xd1, 0x0c, 0x35}
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
