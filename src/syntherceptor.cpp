/*
 * Syntherceptor
 * Main synthesiser code
 * Author: James Teh <jamie@jantrid.net>
 * Copyright 2025 James Teh
 * License: GNU General Public License version 2.0
 */

#include "syntherceptor.h"

#include <chrono>
#include <sstream>

#include "nvdaController.h"

using namespace std::chrono_literals;

Syntherceptor::Syntherceptor() : refCount(1) {
	speakThread = std::thread([this] {
		speakWorker();
	});
}

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
	if (!c) {
		nvdaController_cancelSpeech();
		ssml = L"";
		ssmlEvent.release();
		speakThread.join();
		delete this;
	}
	return c;
}

void Syntherceptor::speakWorker() {
	for (; ;) {
		ssmlEvent.acquire();
		if (ssml.empty()) {
			return;
		}
		// This will block until speech has completed or been cancelled.
		nvdaController_speakSsml(ssml.c_str(), SYMBOL_LEVEL_UNCHANGED,
			SPEECH_PRIORITY_NORMAL, false);
		doneEvent.release();
	}
}

STDMETHODIMP Syntherceptor::Speak(
	DWORD flags, REFGUID, const WAVEFORMATEX*, const SPVTEXTFRAG* fragList,
	ISpTTSEngineSite* site
) {
	std::wostringstream s;
	for (auto frag = fragList; frag; frag = frag->pNext) {
		if (site->GetActions() & SPVES_ABORT) {
			nvdaController_cancelSpeech();
			return S_OK;
		}
		if (frag->pTextStart && frag->ulTextLen) {
			if (s.tellp() == 0) {
				s << "<speak>";
			} else {
				s << " ";
			}
			s << std::wstring(frag->pTextStart, frag->ulTextLen);
		}
	}
	if (s.tellp() == 0) {
		return S_OK;
	}
	s << "</speak>";
	// Dispatch the SSML to the speak worker.
	ssml = s.str();
	ssmlEvent.release();
	// Wait for speech to complete while polling to determine whether we need
	// to cancel.
	do {
		if (site->GetActions() & SPVES_ABORT) {
			nvdaController_cancelSpeech();
			// Wait for the cancellation to occur.
			doneEvent.acquire();
			break;
		}
	} while (!doneEvent.try_acquire_for(50ms));
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
