/*
 * Syntherceptor
 * Main synthesiser code
 * Author: James Teh <jamie@jantrid.net>
 * Copyright 2025 James Teh
 * License: GNU General Public License version 2.0
 */

#include "syntherceptor.h"

#include <string>

#include "nvdaController.h"

Syntherceptor::Syntherceptor() : refCount(1) {}

STDMETHODIMP Syntherceptor::QueryInterface(REFIID riid, void** ppv) {
	if (!ppv)
		return E_POINTER;
	*ppv = nullptr;

	if (riid == IID_IUnknown || riid == __uuidof(ISpTTSEngine))
		*ppv = static_cast<ISpTTSEngine*>(this);
	else if (riid == __uuidof(ISpObjectWithToken))
		*ppv = static_cast<ISpObjectWithToken*>(this);
	else
		return E_NOINTERFACE;

	AddRef();
	return S_OK;
}

ULONG Syntherceptor::AddRef() {
	return ++refCount;
}

ULONG Syntherceptor::Release() {
	ULONG c = --refCount;
	if (!c)
		delete this;
	return c;
}

STDMETHODIMP Syntherceptor::Speak(
	DWORD flags, REFGUID, const WAVEFORMATEX*, const SPVTEXTFRAG* fragList,
	ISpTTSEngineSite* site
) {
	// Note that for ISpTTSEngine ,flags will only ever contain SPF_NLP_SPEAK_PUNC.
	// In particular, SPF_PURGEBEFORESPEAK will not be exposed here - that is
	// handled by SAPI itself - so we can't use that to know when to cancel speech.
	// See:
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ms719574(v=vs.85)#parameters
	auto siteNum = (uintptr_t)(void*)site;
	if (siteNum != lastSite) {
		nvdaController_cancelSpeech();
		lastSite = siteNum;
	}
	bool spoken = false;
	for (auto frag = fragList; frag; frag = frag->pNext) {
		if (frag->pTextStart && frag->ulTextLen) {
			std::wstring text(frag->pTextStart, frag->ulTextLen);
			nvdaController_speakText(text.c_str());
			spoken = true;
		}
	}

	if (!spoken) {
		return S_OK;
	}
	// We can't determine when nvdaController_speakText is finished, but SAPI
	// expects this method to block until it finishes speaking or until abort is
	// requested. If we return early, we won't know about requests to abort speech.
	// For now, block for ~3sec. We might be able to do better with
	// nvdaController_speakSsml, but that will require us to run it on another
	// thread.
	for (size_t c = 0; c < 60; ++c) {
		if (site->GetActions() & SPVES_ABORT) {
			nvdaController_cancelSpeech();
			break;
		}
		Sleep(50);
	}

	return S_OK;
}

STDMETHODIMP Syntherceptor::
	GetOutputFormat(const GUID*, const WAVEFORMATEX*, GUID* formatId, WAVEFORMATEX**) {
	*formatId = SPDFID_Text;
	return S_OK;
}

STDMETHODIMP Syntherceptor::SetObjectToken(ISpObjectToken* pToken) {
	token = pToken;
	return S_OK;
}

STDMETHODIMP Syntherceptor::GetObjectToken(ISpObjectToken** ppToken) {
	if (!ppToken)
		return E_POINTER;
	*ppToken = token;
	if (*ppToken)
		(*ppToken)->AddRef();
	return S_OK;
}
